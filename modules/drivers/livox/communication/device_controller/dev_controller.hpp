#ifndef LIVOX_DEV_CONTROLLER_H
#define LIVOX_DEV_CONTROLLER_H

#include "../../device_manager/device_info.hpp"
#include "../protocol/livox_protocol.hpp"
#include "../livox_command/command.hpp"
#include "../device_data_recv/device_data_recv.hpp"
#include "../../common/common_def.hpp"

#include <iostream>
#include <memory>
#include <vector>

class lidar_dev_controller
{
public:
	lidar_dev_controller(uint8_t id);
	~lidar_dev_controller();
	void set_device_info(struct sockaddr_in &livox);
	bool get_device_status(uint8_t id);
	uint32_t create_package(std::shared_ptr<livox_protocol> prot_,
		uint8_t cmd_type,uint8_t cmd_set,uint32_t cmd_code,
		uint16_t cmd_len,uint8_t *cmd);
	void lidar_dev_monitor();
	void stop_lidar_dev_controller();

private:
	bool need_handshake;
	bool stop;
	
	socklen_t sock_len;
	struct sockaddr_in livox_addr;
	struct sockaddr_in server_addr;
	
	DeviceInfo lidar_dev;
	CommPacket cmd_packet;
	
	lidar_dev_pcl_receiver *pcl_receiver;
	//lidar_dev_pcl_receiver *imu_receiver;
	
	std::shared_ptr<livox_protocol> prot;
	std::shared_ptr<livox_command> cmd;
	std::shared_ptr<Session> session;
	
	std::vector<uint8_t> package;
	std::vector<uint8_t> command;
	
	std::unique_ptr<apollo::cyber::Timer> timer_;
	void heartbeat();
};

#endif
