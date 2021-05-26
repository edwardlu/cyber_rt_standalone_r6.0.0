#include "socket_util.hpp"
#include "../common/common_def.hpp"

bool socket_util::get_local_addr()
{
	if(getifaddrs(&ifaddr) == -1) {
		std::cout<<"getifaddrs error"<<std::endl;
		return false;
	}

	return true;
}

uint32_t socket_util::get_ip_by_name(std::string interface)
{
	for(struct ifaddrs *ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
	{
		if(ifa->ifa_addr == NULL)
			continue;
		std::string name(ifa->ifa_name);
		if(name == interface)
		{
			if(ifa->ifa_addr->sa_family==AF_INET)
			{
				struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;
				return addr->sin_addr.s_addr;
			}
		}
	}

	return 0;
}

bool show_socket_connect_info(char *out_ip,struct sockaddr_in &in_addr)
{
	if(out_ip == nullptr)
	{
		std::cout<<"invalidate buffer assigned"<<std::endl;
		return false;
	}
	char local_ip[IP_STRING_LEN];
	if(inet_ntop(AF_INET,&in_addr.sin_addr.s_addr,local_ip,sizeof(local_ip))<0)
	{
		std::cout<<"inet_ntop failed"<<std::endl;
		return false;
	}
	memcpy(out_ip,local_ip,IP_STRING_LEN);

	return true;
}

