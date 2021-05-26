#include "dev_controller.hpp"

lidar_dev_controller::lidar_dev_controller(uint8_t id):need_handshake(true),stop(false)
{
	package.resize(MAX_MESSAGE_BUF_SIZE);
	command.resize(MAX_MESSAGE_BUF_SIZE);
	
	lidar_dev_manager::get_instance().find_device(id,lidar_dev);
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = lidar_dev.s_ip;
	server_addr.sin_port = htons(lidar_dev.cmd_port);

#ifdef DEBUG_CONNECT_INFO
	char local_ip[IP_STRING_LEN];
	::show_socket_connect_info(local_ip,server_addr);
	std::cout<<"lidar_dev_controller create connection on "<<local_ip<<std::endl;
#endif

	sock_len = static_cast<socklen_t>(sizeof(livox_addr));
	
	prot = std::make_shared<livox_protocol>(0x4c49,0x564f580a);

	session = std::make_shared<Session>();
	session->Socket(AF_INET, SOCK_DGRAM, 0);
	if(session->Bind((struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		std::cout<<"device_controller bind to server_addr failed."<<std::endl;
	}
}

lidar_dev_controller::~lidar_dev_controller()
{
	//for not capture SIGINT to stop and recycle resource
	//to use public function : stop_lidar_dev_controller
}

void lidar_dev_controller::stop_lidar_dev_controller()
{
#ifdef GRACEFULE_QUIT_LIVOX
	std::cout<<"stop_lidar_dev_controller"<<std::endl;
#endif
	pcl_receiver->stop_dev_pcl_receiver();
	stop = true;
	delete pcl_receiver;
}

void lidar_dev_controller::set_device_info(struct sockaddr_in &livox)
{
	livox_addr.sin_family      = livox.sin_family;
	livox_addr.sin_addr.s_addr = livox.sin_addr.s_addr;
	livox_addr.sin_port        = livox.sin_port;
}

bool lidar_dev_controller::get_device_status(uint8_t id)
{
	DeviceInfo dev_info;
	lidar_dev_manager::get_instance().find_device(id,dev_info);
	return dev_info.handshaked;
}

uint32_t lidar_dev_controller::create_package(std::shared_ptr<livox_protocol> prot_,
		uint8_t cmd_type,uint8_t cmd_set,uint32_t cmd_code,
		uint16_t cmd_len,uint8_t *cmd)
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

	prot_->Pack(package.data(),MAX_MESSAGE_BUF_SIZE,&o_len,packet);

	int32_t ret = prot_->CheckPacket(package.data());
	if(ret < 0)
	{
		std::cout<<"create_package error"<<std::endl;
	} else {
		return o_len;
	}

	return 0;
}

void lidar_dev_controller::lidar_dev_monitor()
{
	int nbytes = 0;
	int cmd_length;
	uint32_t ret;

	while(!stop) {
		if(need_handshake == true)
		{
#ifdef DEBUG_CONNECT_INFO
			std::cout<<"create handshake message "<<" cmd port "<< lidar_dev.cmd_port <<" data port "<< lidar_dev.data_port <<" sensor port "<< lidar_dev.sensor_port <<std::endl;
			char local_ip[IP_STRING_LEN];
			::show_socket_connect_info(local_ip,server_addr);
			std::cout<<"lidar_dev_controller create connection on "<<local_ip<<std::endl;
#endif
			cmd->create_instruction_handshake(command.data(),&cmd_length,server_addr.sin_addr.s_addr,lidar_dev.data_port,lidar_dev.cmd_port,lidar_dev.sensor_port);
			
			ret = create_package(prot,LIVOX_CMD_TYPE_MSG,LIVOX_CMD_SET,LIVOX_CMD_ID_HANDSHAKE,cmd_length,command.data());
			if(ret == 0)
				std::cout<<"Create handshake package failed"<<std::endl;
			else {
				nbytes = session->SendTo(package.data(), ret, 0, (const struct sockaddr*)&livox_addr, sock_len);
#ifdef DEBUG_COMMUNICATION_INFO
				if(nbytes == (int)ret)
					std::cout<<"Send handshake"<<std::endl;
#endif
			}
		}
		nbytes = static_cast<int>(session->RecvFrom(package.data(), 
				MAX_MESSAGE_BUF_SIZE, 0, 
				(struct sockaddr*)&livox_addr, &sock_len));
		if (nbytes < 0) {
			std::cout <<"recv from client failed."<<std::endl;
			continue;
		}
		//check handshake ACK 
		prot->ParsePacket(package.data(),ret,&cmd_packet);
		if((cmd_packet.cmd_set == LIVOX_CMD_SET)&&(cmd_packet.cmd_code == LIVOX_CMD_ID_HANDSHAKE))
		{
			struct HandshakeRequestResponse *handshake_rsp = 
				reinterpret_cast<struct HandshakeRequestResponse *>(cmd_packet.data);
			if(handshake_rsp->ret_code == LIVOX_CMD_HANDSHAKE_OK)
			{
				need_handshake = false;
				
				pcl_receiver  = new lidar_dev_pcl_receiver(server_addr.sin_addr.s_addr,lidar_dev.data_port);
				pcl_receiver->set_pcl_device_addr(livox_addr,lidar_dev.data_port);
				//create pcl and imu receiver
				pcl_receiver->start_pcl_data_receiver();
				
				//start sample 
				cmd->create_instruction_sample(command.data(),&cmd_length,LIVOX_CMD_START_SAMPLE_ENABLE);
				ret = create_package(prot,LIVOX_CMD_TYPE_MSG,LIVOX_CMD_SET,LIVOX_CMD_ID_START_SAMPLE,cmd_length,command.data());
				if(ret == 0)
					std::cout<<"create sample package failed"<<std::endl;
				else {
					nbytes = session->SendTo(package.data(), ret, 0,
						(const struct sockaddr*)&livox_addr, sock_len);
#ifdef DEBUG_COMMUNICATION_INFO
					if(nbytes == (int)ret)
						std::cout<<"Send sample ok"<<std::endl;
#endif
				}
				
				//start timer
				timer_.reset(new apollo::cyber::Timer(1000, [this]() { this->heartbeat(); }, false));
				timer_->Start();
			} else {
				std::cout<<"check handshake ACK FAILED"<<std::endl;
			}
		}
	}
}

void lidar_dev_controller::heartbeat()
{
	int nbytes = 0;
	int cmd_length;
	uint32_t ret;

	cmd->create_instruction_heartbeat(command.data(),&cmd_length);
	ret = create_package(prot,LIVOX_CMD_TYPE_MSG,LIVOX_CMD_SET,LIVOX_CMD_ID_HEARTBEAT_ACK,cmd_length,command.data());
	nbytes = session->SendTo(package.data(), ret, 0, (const struct sockaddr*)&livox_addr, sock_len);

	if(nbytes == (int)ret)
	{
#ifdef DEBUG_COMMUNICATION_INFO
		std::cout<<"send heart beat ok"<<std::endl;
#endif
	}
}
