#include "../device_info.hpp"

int main()
{
	DeviceInfo dev_info;
	lidar_dev_manager::get_instance().find_device(0,dev_info);

	std::cout<<"broadcast code "<<dev_info.broadcast_code<<std::endl;
	std::cout<<"data port      "<<dev_info.data_port<<std::endl;
	std::cout<<"cmd  port      "<<dev_info.cmd_port<<std::endl;
	std::cout<<"sensor port    "<<dev_info.sensor_port<<std::endl;
	std::cout<<"server ip      "<<dev_info.server_ip<<std::endl;
	std::cout<<"device ip      "<<dev_info.device_ip<<std::endl;
	return 0;
}
