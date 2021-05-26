#ifndef LIVOX_DATA_RECV_H
#define LIVOX_DATA_RECV_H

#include "../../shared_mem/block_interface.h"
#include "../../shared_mem/block_map.h"
#include "../../device_manager/device_info.hpp"
#include "../protocol/livox_protocol.hpp"
#include "../livox_command/command.hpp"
#include "time_check.hpp"

#include <sys/ioctl.h>
#include <iostream>
#include <memory>
#include <vector>


#define MAX_DATA_SIZE 1500
#define kPrefixDataSize 18

class lidar_dev_pcl_receiver
{
public:
	lidar_dev_pcl_receiver(uint32_t server_ip,uint16_t server_port);
	~lidar_dev_pcl_receiver();
	
	void set_pcl_device_addr(struct sockaddr_in &livox,uint16_t data_port);
	void start_pcl_data_receiver();
	void reset_count_info(bool partitions);
	bool check_pcl_package(uint8_t *ptr,uint64_t count);
	bool check_pcl_frame(uint8_t *ptr);
	void lidar_dev_pcl_init_shared_mem();
	void receiver_function();
	void stop_dev_pcl_receiver();
private:
	int sock_fd;
	bool stop;
	uint64_t package_count;
        uint8_t *pcl_ptr;
	
	std::shared_ptr<struct block_interface> block_ptr;
	struct block_desc *buf_a;
	struct block_desc *buf_b;

	std::thread pcl_thread;
	socklen_t sock_len;
	struct sockaddr_in pcl_device_addr;
	struct sockaddr_in server_addr;
};

#if 0
class lidar_dev_imu_receiver
{
public:
	lidar_dev_imu_receiver(uint32_t ip,uint16_t imu_port);
	~lidar_dev_imu_receiver();

	void imu_receiver();
private:
	char *imu_data;
	int package_count;
	socklen_t sock_len;
	struct sockaddr_in imu_device_addr;

	std::shared_ptr<Session> session;
	struct sockaddr_in server_addr;
};
#endif

#endif
