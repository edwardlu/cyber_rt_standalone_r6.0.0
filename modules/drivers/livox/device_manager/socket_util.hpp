#ifndef SOCKET_UTIL_H
#define SOCKET_UTIL_H

#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

bool show_socket_connect_info(char *out_ip,struct sockaddr_in &in_addr);

class socket_util
{
public:
	socket_util() = default;
	~socket_util() = default;

	bool get_local_addr();
	uint32_t get_ip_by_name(std::string interface);

private:
	struct ifaddrs *ifaddr;
};

#endif
