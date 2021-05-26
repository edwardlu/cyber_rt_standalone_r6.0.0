#include "../livox_protocol.hpp"
#include "../../livox_command/command.hpp"
#include <iostream>
#include <memory>
#include <vector>
#include <stdio.h>

std::vector<uint8_t> package(1500);
std::vector<uint8_t> command(1024);

uint32_t create_package(std::shared_ptr<livox_protocol> prot_,uint8_t cmd_type,uint8_t cmd_set,uint32_t cmd_code,uint16_t cmd_len,uint8_t *cmd)
{
	uint32_t o_len;
	CommPacket packet;
	memset(&packet, 0, sizeof(packet));

	packet.packet_type = cmd_type;
	packet.seq_num     = 0;
	packet.cmd_set     = cmd_set;
	packet.cmd_code    = cmd_code;
	packet.data_len    = cmd_len;
	packet.data        = cmd;

	prot_->Pack(package.data(),1500,&o_len,packet);

	int32_t ret = prot_->CheckPacket(package.data());
        if(ret < 0)
	{
                std::cout<<"create_package error"<<std::endl;
	} else {
		return o_len;
	}

	return 0;
}

int main()
{
	int cmd_len;
	uint32_t package_len;
	CommPacket packet_test;

	std::shared_ptr<livox_protocol> prot = std::make_shared<livox_protocol>(0x4c49,0x564f580a);
	std::shared_ptr<livox_command> cmd = std::make_shared<livox_command>();

	cmd->create_instruction_handshake(command.data(),&cmd_len,12345,60000,60001,60002);
	std::cout<<"cmd_len: "<<cmd_len<<std::endl;
	package_len = create_package(prot,1,2,3,cmd_len,command.data());

	prot->ParsePacket(package.data(),package_len,&packet_test);
	
	struct HandshakeRequest *handshake = reinterpret_cast<struct HandshakeRequest *>(packet_test.data);
     	std::cout<<"cmd_len: "<<packet_test.data_len<<std::endl; 
	std::cout<<"ip     : "<<handshake->ip_addr<<std::endl;
	std::cout<<"d_port : "<<handshake->data_port<<std::endl;
	std::cout<<"c_port : "<<handshake->sensor_port<<std::endl;
	std::cout<<"s_port : "<<handshake->cmd_port<<std::endl;

	return 0;
}
