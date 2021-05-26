#include "device_info.hpp"

lidar_dev_manager::lidar_dev_manager()
{
	std::vector<std::string> infas;
	//VECOM LAN1/LAN1 GB
	infas.push_back("eno1");
	infas.push_back("enp2s0");
	//VECOW PoE1~4 100MB
	infas.push_back("enp3s0f0");
	infas.push_back("enp3s0f1");
	infas.push_back("enp3s0f2");
	infas.push_back("enp3s0f3");

	init_devices(infas);
}

void lidar_dev_manager::init_devices(std::vector<std::string> &interfaces)
{
	uint8_t device_index = 0;
#ifdef DEBUG_DEVICEINFO_DEBUG
	std::cout<<"init devices : "<<std::endl;
#endif
	for(auto device : interfaces)
	{
		bool ret = sock_util.get_local_addr();
		if(ret == false)
			std::cout<<"socket get local addr failed"<<std::endl;
		uint32_t ip = sock_util.get_ip_by_name(device);
		if(ip == 0)
		{
			ip = 0; //0.0.0.0
			std::cout<<"interface was not assign correct ip address"<<std::endl;
		}

		DeviceInfo lidar_dev;

		std::ostringstream b_name;
		b_name<<"ZHITO_LIDAR_LIVOX_"<<(int)device_index;
		strncpy(lidar_dev.broadcast_code,b_name.str().c_str(),BROADCAST_CODE_LEN);
		strncpy(lidar_dev.server_ip,inet_ntoa(*(struct in_addr *)&ip),IP_STRING_LEN);
		
		lidar_dev.id = device_index;
		lidar_dev.type = kDeviceTypeLidarAvia;
		lidar_dev.cmd_port = BASE_PORT_CMD + device_index;
		lidar_dev.data_port = BASE_PORT_DATA + device_index;
		lidar_dev.sensor_port = BASE_PORT_SENSOR + device_index;
		lidar_dev.s_ip = ip;
		lidar_dev.feature = kLidarFeatureRainFog;
		lidar_dev.firmware_version[0] = 0x00;
		lidar_dev.firmware_version[1] = 0x01;
		lidar_dev.firmware_version[2] = 0x02;
		lidar_dev.firmware_version[3] = 0x03;
		lidar_dev.handshaked = false;
		
#ifdef DEBUG_DEVICEINFO_DEBUG
		std::cout<<"create : "<<lidar_dev.broadcast_code<<" "<<device<<" ip : "<<lidar_dev.server_ip<<" device id : "<<(int)lidar_dev.id<<std::endl;
		std::cout<<"     "<<" cmd  port : "<<lidar_dev.cmd_port<<std::endl;
		std::cout<<"     "<<" data port : "<<lidar_dev.data_port<<std::endl;
		std::cout<<"     "<<" imu  port : "<<lidar_dev.sensor_port<<std::endl;
#endif
		device_map.insert(std::pair<int, DeviceInfo>(device_index, lidar_dev));
		device_index ++;
	}
}

lidar_dev_manager::~lidar_dev_manager()
{

}

void lidar_dev_manager::set_device_status(uint8_t dev_id,bool stat)
{
	device_map[dev_id].handshaked = stat;
}

bool lidar_dev_manager::find_device(uint8_t dev_id, DeviceInfo &info)
{
	if(dev_id  > device_map.size())
		return false;
	
	info  =  device_map[dev_id];
	return true;
}

bool lidar_dev_manager::is_device_connected(uint8_t dev_id)
{
	if(dev_id  > device_map.size())
		return false;
	else {
		if(device_map[dev_id].handshaked == true)
			return true;
		else
			return false;
	}
}

bool lidar_dev_manager::IsLidarMid40(uint8_t dev_id)
{
	return true;
}

bool lidar_dev_manager::IsLidarMid70(uint8_t dev_id)
{
	return true;
}

bool lidar_dev_manager::IsLidarTele(uint8_t dev_id)
{
	return true;
}

bool lidar_dev_manager::IsLidarAvia(uint8_t dev_id)
{
	return true;
}

bool lidar_dev_manager::GetLidarFirmwareVersion(uint8_t dev_id, uint32_t &firm_ver)
{
	return true;
}
