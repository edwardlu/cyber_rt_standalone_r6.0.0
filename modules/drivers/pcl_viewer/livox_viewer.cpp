#include <signal.h>

#include <iostream>
#include <chrono>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

#include "cyber_rt_standalone_r6.0.0/modules/drivers/livox/shared_mem/block_interface.h"
#include "cyber_rt_standalone_r6.0.0/modules/drivers/livox/shared_mem/block_map.h"
#include "cyber_rt_standalone_r6.0.0/modules/drivers/livox/communication/protocol/livox_protocol.hpp"

#include <pcl/filters/passthrough.h>
#include <pcl/filters/voxel_grid.h>
#include "cuda_runtime.h"
#include "../include/cudaFilter.h"

#define SHARED_MEM_ID 123456789111

using std::chrono::high_resolution_clock;
bool stop;

uint64_t Now() {
	auto now = high_resolution_clock::now();
	auto nano_time_point = std::chrono::time_point_cast<std::chrono::nanoseconds>(now);
	auto epoch = nano_time_point.time_since_epoch();
	uint64_t now_nano = std::chrono::duration_cast<std::chrono::nanoseconds>(epoch).count();

	return now_nano;
}

void dump_block_info(struct block_desc *b_desc)
{
	std::cout<<"block ptr  : "<<(uint64_t *)b_desc<<std::endl;
	std::cout<<"block name : "<<b_desc->blk_name<<std::endl;
	std::cout<<"block size : "<<b_desc->size<<std::endl;
	std::cout<<"block id   : "<<b_desc->blk_id<<std::endl;
}

void Getinfo(void)
{
	cudaDeviceProp prop;

	int count = 0;
	cudaGetDeviceCount(&count);
	printf("\nGPU has cuda devices: %d\n", count);
	for (int i = 0; i < count; ++i) {
		cudaGetDeviceProperties(&prop, i);
		printf("----device id: %d info----\n", i);
		printf("  GPU : %s \n", prop.name);
		printf("  Capbility: %d.%d\n", prop.major, prop.minor);
		printf("  Global memory: %luMB\n", prop.totalGlobalMem >> 20);
		printf("  Const memory: %luKB\n", prop.totalConstMem  >> 10);
		printf("  SM in a block: %luKB\n", prop.sharedMemPerBlock >> 10);
		printf("  warp size: %d\n", prop.warpSize);
		printf("  threads in a block: %d\n", prop.maxThreadsPerBlock);
		printf("  block dim: (%d,%d,%d)\n", prop.maxThreadsDim[0], prop.maxThreadsDim[1], prop.maxThreadsDim[2]);
		printf("  grid dim: (%d,%d,%d)\n", prop.maxGridSize[0], prop.maxGridSize[1], prop.maxGridSize[2]);
	}
	printf("\n");
}

void sig_int(int sig_no)
{
	stop = true;
}

int main()
{
	signal(SIGINT,sig_int);

	uint64_t timer1,timer2;

	pcl::PointCloud<pcl::PointXYZ>::Ptr clouds(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloudDst(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointXYZ point;
	pcl::visualization::PCLVisualizer viewer("viewer");
	cloudDst->resize(1024*8);

	float *inputData; //(float *)cloudSrc->points.data();
	float *outputData; //(float *)cloudSrc->points.data();
	float *input = NULL;
	float *output = NULL;
	unsigned int countLeft = 0;

	block_interface shm_intf(SHARED_MEM_ID);
	struct block_desc *ptr1 = shm_intf.get_block(0);
	struct block_desc *ptr2 = shm_intf.get_block(1);

	dump_block_info(ptr1);
	dump_block_info(ptr2);

	//CUDA functions
	cudaStream_t stream = NULL;
	cudaStreamCreate(&stream);

	cudaMallocManaged(&input, sizeof(float)*4*24000,cudaMemAttachHost);
	cudaStreamAttachMemAsync(stream, input);

	cudaMallocManaged(&output,sizeof(float)*4*24000,cudaMemAttachHost);
	cudaStreamAttachMemAsync(stream, output);

	FilterType_t type;
	cudaFilter filter(stream);
	FilterParam_t setP;
	type = VOXELGRID;
	setP.type = type;
	setP.voxelX = 0.1;
	setP.voxelY = 0.1;
	setP.voxelZ = 0.1;
	filter.set(setP);

	LivoxEthPacket *lidar_data;
	LivoxExtendRawPoint *pt;
	uint8_t *p;

	stop = false;
	while(!stop) {
		if((ptr1->in_use.load() == false)&&(ptr2->in_use.load() == true))
		{
			std::cout<<"a show"<<std::endl;
			p = shm_intf.get_writeable_area(0);
			for(int lines = 0;lines<250;lines++)
			{
				lidar_data = reinterpret_cast<LivoxEthPacket *>(p);
				LivoxExtendRawPoint *pt = reinterpret_cast<LivoxExtendRawPoint *>(lidar_data->data);
				for(int points = 0;points<96;points++)
				{
					if(pt[points].reflectivity < 30)
					{
						point.x = 0;
						point.y = 0;
						point.z = 0;
						clouds->points.push_back(point);
					} else {
						point.x = pt[points].x/1000.0f;
						point.y = pt[points].y/1000.0f;
						point.z = pt[points].z/1000.0f;
						clouds->points.push_back(point);
					}
				}
				p = p+1362;
			}
		} else if((ptr1->in_use.load() == true)&&(ptr2->in_use.load() == false)) {
			std::cout<<"b show"<<std::endl;
			p = shm_intf.get_writeable_area(1);
			for(int lines = 0;lines<250;lines++)
			{
				lidar_data = reinterpret_cast<LivoxEthPacket *>(p);
				LivoxExtendRawPoint *pt = reinterpret_cast<LivoxExtendRawPoint *>(lidar_data->data);
				for(int points = 0;points<96;points++)
				{
					if(pt[points].reflectivity < 0)
					{
						point.x = 0;
						point.y = 0;
						point.z = 0;
						clouds->points.push_back(point);
					} else {
						point.x = pt[points].x/1000.0f;
						point.y = pt[points].y/1000.0f;
						point.z = pt[points].z/1000.0f;
						clouds->points.push_back(point);
					}
				}
				p = p+1362;
			}
		}
		
		timer1 = Now();
		inputData = (float *)clouds->points.data();
		outputData = (float *)cloudDst->points.data();
		cudaMemcpyAsync(input,inputData,sizeof(float)*4*24000,cudaMemcpyHostToDevice,stream);
		cudaStreamSynchronize(stream);

		filter.filter(output, &countLeft, input, 24000);
		checkCudaErrors(cudaMemcpyAsync(outputData, output, sizeof(float)*4*countLeft, cudaMemcpyDeviceToHost, stream));
		cudaStreamSynchronize(stream);
		timer2 = Now();

		std::cout<<"Filter use total : second(s) "<<(timer2-timer1)/1000000000UL<<"ms "<<(timer2-timer1)%1000000000UL/1000/1000<<std::endl;

		viewer.addPointCloud<pcl::PointXYZ>(cloudDst,"livox");
		viewer.spinOnce(50);
		viewer.removePointCloud("livox");
		clouds->clear();

	}

	std::cout<<"################ End of pcl view function ################"<<std::endl;
	cudaFree(input);
	cudaFree(output);
	cudaStreamDestroy(stream);

	return 0;
}
