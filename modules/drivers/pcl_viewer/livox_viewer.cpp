#include <chrono>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

#include "/media/lubin/extend/apollo_module_standalone/modules/drivers/livox/shared_mem/block_interface.h"
#include "/media/lubin/extend/apollo_module_standalone/modules/drivers/livox/shared_mem/block_map.h"
#include "/media/lubin/extend/apollo_module_standalone/modules/drivers/livox/communication/protocol/livox_protocol.hpp"

#define SHARED_MEM_ID 123456789111

using std::chrono::high_resolution_clock;

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

int main()
{
	pcl::PointCloud<pcl::PointXYZ>::Ptr clouds(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointXYZ point;
	pcl::visualization::PCLVisualizer viewer("test");
	
	block_interface shm_intf(SHARED_MEM_ID);
	struct block_desc *ptr1 = shm_intf.get_block(0);
	struct block_desc *ptr2 = shm_intf.get_block(1);
		
	dump_block_info(ptr1);
	dump_block_info(ptr2);
	
	LivoxEthPacket *lidar_data;
	LivoxExtendRawPoint *pt;
	uint8_t *p;
	
	while(true) {
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
					if(pt[points].reflectivity < 50)
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
		viewer.addPointCloud<pcl::PointXYZ>(clouds,"livox");
		viewer.spinOnce(50);
		viewer.removePointCloud("livox");
		clouds->clear();
	}

	while(!viewer.wasStopped())
	{
		viewer.spinOnce(1000);  // Give the GUI 1000ms to handle events, then return
	}

	return 0;
}
