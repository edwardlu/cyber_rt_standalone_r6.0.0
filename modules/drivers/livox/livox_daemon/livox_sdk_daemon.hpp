#ifndef LIVOX_DAEMON_H
#define LIVOX_DAEMON_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "../communication/protocol/livox_protocol.hpp"
#include "../communication/device_controller/dev_controller.hpp"
#include "../communication/livox_command/command.hpp"
#include "../device_manager/device_info.hpp"

#include "../common/common_def.hpp"

class livox_daemon
{
public:
	livox_daemon(uint16_t server_port,int inface_id);
	~livox_daemon();
	bool check_boardcast_id(uint8_t *id,std::string b_code);
	void daemon();
	void quit();
private:
	std::shared_ptr<livox_protocol> prot;
	std::shared_ptr<Session> session;

	DeviceInfo lidar_dev;
	
	struct sockaddr_in device_addr;
	socklen_t sock_len;

	std::vector<lidar_dev_controller *> controllers;
	std::vector<uint8_t> command;
};

#endif
