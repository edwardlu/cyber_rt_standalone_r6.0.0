#include "livox_sdk_daemon.hpp"

livox_daemon::livox_daemon(uint16_t server_port,int inface_id)
{
	command.resize(MAX_MESSAGE_BUF_SIZE);
	controllers.resize(MAX_DEVICES);
	
	//get local ip address
	lidar_dev_manager::get_instance().find_device(inface_id,lidar_dev);
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(server_port);

	sock_len = static_cast<socklen_t>(sizeof(device_addr));
	
	prot = std::make_shared<livox_protocol>(0x4c49,0x564f580a);
	session = std::make_shared<Session>();
	int ret = session->Socket(AF_INET, SOCK_DGRAM, 0);
	if(ret<0)
	{
		std::cout << "livox_daemon create socket failed." << std::endl;
	}

	if (session->Bind((struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		std::cout << "livox_daemon bind to port[" << server_port << "] failed." << std::endl;
	}
}

bool livox_daemon::check_boardcast_id(uint8_t *id,std::string b_code)
{
	if(b_code.empty())
		return false;

	for(uint8_t i=0; i<MAX_DEVICES; i++)
	{
		lidar_dev_manager::get_instance().find_device(i,lidar_dev);
#ifdef DEBUG_COMMUNICATION_INFO
		printf("input boardcast code is  %s\n",b_code.c_str());
		printf("target boardcast code is %s\n",lidar_dev.broadcast_code);
#endif
		if(strncmp(lidar_dev.broadcast_code,b_code.c_str(),BOARDCAST_CODE_LEN) == 0)
		{
			if(lidar_dev.handshaked == false)
			{	
				*id = lidar_dev.id;
				return true;
			}
		}
	}
	return false;
}

livox_daemon::~livox_daemon()
{
#ifdef GRACEFULE_QUIT_LIVOX
	std::cout<<"livox_daemom finished ..."<<std::endl;
#endif
	quit();
	session->Close();
}

void livox_daemon::quit()
{
	DeviceInfo lidar_dev;
	for(int i=0; i<MAX_DEVICES; i++)
	{
		lidar_dev_manager::get_instance().find_device(i,lidar_dev);
		if(lidar_dev.handshaked == true)
		{
			if(controllers[i] != nullptr)
			{
				controllers[i]->stop_lidar_dev_controller();
			}
		}
	}
}

void livox_daemon::daemon()
{
	int ret;
	int nbytes = 0;
	uint8_t  dev_id;
	char remote_ip[IP_STRING_LEN];
	char buffer_recv[MAX_MESSAGE_BUF_SIZE];
	bool checked;

	CommPacket out_pack;

	while (true) {
		nbytes = static_cast<int>(session->RecvFrom(buffer_recv, MAX_MESSAGE_BUF_SIZE, 0, (struct sockaddr*)&device_addr, &sock_len));
		if (nbytes < 0) {
			std::cout <<"recv from client failed."<<std::endl;
			continue;
		}
		if(inet_ntop(AF_INET,&device_addr.sin_addr.s_addr,remote_ip,sizeof(remote_ip))<0)
		{
			std::cout<<"inet_ntop failed"<<std::endl;
		}
		if(ntohs(device_addr.sin_port)<0)
		{
			std::cout<<"inet_ntohs failed"<<std::endl;
		}
#ifdef DEBUG_COMMUNICATION_INFO
		std::cout<<"receive livox broadcast from   : "<<remote_ip<<" : "<<ntohs(device_addr.sin_port)<<std::endl;
#endif
		ret = prot->ParsePacket(reinterpret_cast<uint8_t *>(buffer_recv), nbytes ,&out_pack);

		if(ret < 0)
			std::cout<<"ParsePacket error"<<std::endl;
		else {
			checked = check_boardcast_id(&dev_id,"ZHITO_LIDAR_LIVOX_0");
			//update device info
			if(checked == true){
				lidar_dev_manager::get_instance().set_device_status(dev_id,true);
				controllers[dev_id] = new lidar_dev_controller(dev_id);
				controllers[dev_id]->set_device_info(device_addr);
				auto func = std::bind(&lidar_dev_controller::lidar_dev_monitor,controllers[dev_id]);
				apollo::cyber::scheduler::Instance()->CreateTask(func,lidar_dev.broadcast_code);
			}
		}
	}
}
