#include "device_data_recv.hpp"

/*PCL receiver*/
lidar_dev_pcl_receiver::lidar_dev_pcl_receiver(uint32_t server_ip,uint16_t server_port):stop(false)
{
	int status;
	int on = -1;
	struct timeval timeout;
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = server_ip;
	server_addr.sin_port = htons(server_port);
#ifdef DEBUG_CONNECT_INFO	
	char local_ip[IP_STRING_LEN];
	::show_socket_connect_info(local_ip,server_addr);
	std::cout<<"Create lidar_dev_pcl_receiver : "<<local_ip<<" port : "<<server_port<<std::endl;
#endif
	sock_fd = socket(AF_INET,SOCK_DGRAM,0);
	if(sock_fd < 0)
	{
		std::cout<<"Create pcl data socket failed"<<std::endl;
	}
	
	timeout.tv_sec = 0;
	timeout.tv_usec = 300; //more than 250us one frame
	if (setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
	{
		printf("time out setting failed\n");
	}
    
	status = setsockopt(sock_fd,SOL_SOCKET,SO_REUSEADDR,(char *)&on,sizeof(on));
	if(status != 0){
		std::cout<<"Set pcl data socket reuseaddr failed"<<std::endl;
	}
	
	status = bind(sock_fd,(struct sockaddr *)&server_addr,sizeof(server_addr));
	if(status != 0)
	{
		std::cout<<"Bind pcl data socket failed"<<std::endl;
	}
	
	sock_len = sizeof(pcl_device_addr);
}

lidar_dev_pcl_receiver::~lidar_dev_pcl_receiver()
{
	//for not capture SIGINT to stop and recycle resource
	//to use public function : stop_dev_pcl_receiver
}

void lidar_dev_pcl_receiver::stop_dev_pcl_receiver()
{
#ifdef GRACEFULE_QUIT_LIVOX
	std::cout<<"Exit pcl data recv function"<<std::endl;
#endif
	stop = true;
	if(pcl_thread.joinable())
	{
		pcl_thread.join(); //block here if not recycle the pcl_receiver
		close(sock_fd);
	}
	block_ptr->recycle_shared_mem();
}

void lidar_dev_pcl_receiver::set_pcl_device_addr(struct sockaddr_in &livox,uint16_t data_port)
{
	pcl_device_addr.sin_family      = livox.sin_family;
	pcl_device_addr.sin_addr.s_addr = livox.sin_addr.s_addr;
	pcl_device_addr.sin_port        = data_port;
#ifdef DEBUG_CONNECT_INFO
	char local_ip[IP_STRING_LEN];
	::show_socket_connect_info(local_ip,pcl_device_addr);
	std::cout<<"Set livox device ip : "<<local_ip<<" pcl receive port : "<<pcl_device_addr.sin_port<<std::endl;
#endif
}

void lidar_dev_pcl_receiver::start_pcl_data_receiver()
{
	stop = false;
	pcl_thread = std::thread(&lidar_dev_pcl_receiver::receiver_function, this);
	apollo::cyber::scheduler::Instance()->SetInnerThreadAttr("pcl_receiver", &pcl_thread);
}

bool lidar_dev_pcl_receiver::check_pcl_package(uint8_t *ptr,uint64_t count)
{
	static struct time_delt last;
	struct time_delt current;
	struct time_delt result;

	LivoxEthPacket *lidar_data;
	lidar_data = reinterpret_cast<LivoxEthPacket *>(ptr);
	
	create_time_point(*((uint64_t *)(lidar_data->timestamp)),&current);
	compare_time_point(&last,&current,&result);
#ifdef DEBUG_SYNC_FRAME_INFO 
        std::cout<<"delt time in package check result : "<<(int)result.check_status<<std::endl;
        std::cout<<"delt time in package "<<result.seconds<<std::endl;
        std::cout<<"delt time in package "<<result.nanoseconds/1000<<std::endl;
#endif

	update_time_point(&current,&last);

	if(result.nanoseconds/1000<=408) //add %2 delay tolerance
		return true;
	else
		return false;
}

bool lidar_dev_pcl_receiver::check_pcl_frame(uint8_t *ptr)
{
	struct time_delt livox_time;
	struct time_delt local_time;
	struct time_delt result;

	LivoxEthPacket *lidar_data;
	lidar_data = reinterpret_cast<LivoxEthPacket *>(ptr);

	create_time_point(*((uint64_t *)(lidar_data->timestamp)),&livox_time);
	create_time_point(Now(),&local_time);
	compare_time_point(&livox_time,&local_time,&result);

#ifdef DEBUG_SYNC_FRAME_INFO 
	//std::cout<<"livox time stamp : "<<" seconds : "<<livox_time.seconds<<" nanoseconds : "<<livox_time.nanoseconds/1000<<std::endl;
	//std::cout<<"local time stamp : "<<" seconds : "<<local_time.seconds<<" nanoseconds : "<<local_time.nanoseconds/1000<<std::endl;
	std::cout<<"delt time in frame check result : "<<(int)result.check_status<<std::endl;
	std::cout<<"delt time in frame "<<result.seconds<<std::endl;
	std::cout<<"delt time in frame "<<result.nanoseconds/1000<<std::endl;
#endif
	//ptp4l on linux use hardware sync seconds delay is 1~2     mic-seconds less than 320000 (stable running more than one hour)
	//ptpd  on linux use software sync seconds delay is 37-48 mic-seconds less than 400    (stable running mote than one hour)
	//ptpd  on qnx   use hardware sync
	
	if(result.nanoseconds/1000<=300000) 
		return true;
	else
		return false;
}

void lidar_dev_pcl_receiver::reset_count_info(bool partitions)
{
	package_count = 1;
	if(partitions == true)
	{
		buf_a->in_use.store(true);
		buf_b->in_use.store(false);
		pcl_ptr = block_ptr->get_writeable_area(0);
		std::cout<<"A block name : "<<buf_a->blk_name<<std::endl;
		std::cout<<"A block size : "<<buf_a->size<<std::endl;
		std::cout<<"A block id   : "<<buf_a->blk_id<<std::endl;
	} else {
		buf_a->in_use.store(false);
		buf_b->in_use.store(true);
		pcl_ptr = block_ptr->get_writeable_area(1);
		std::cout<<"B block name : "<<buf_b->blk_name<<std::endl;
		std::cout<<"B block size : "<<buf_b->size<<std::endl;
		std::cout<<"B block id   : "<<buf_b->blk_id<<std::endl;
	}
}

void lidar_dev_pcl_receiver::lidar_dev_pcl_init_shared_mem()
{
	block_ptr = std::make_shared<struct block_interface>(SHARED_MEM_ID);
	block_ptr->init_block_info();
	buf_a = block_ptr->get_block(0); //A part
	buf_b = block_ptr->get_block(1); //B part
#if 0
	block_ptr = new block_interface(SHARED_MEM_ID);
	block_ptr->init_block_info(); //only in first caller
	buf_a = block_ptr->get_block(0); //A part
	buf_b = block_ptr->get_block(1); //B part
#endif
}

void lidar_dev_pcl_receiver::receiver_function()
{
	int nbytes;
	bool ab_part = true;
	lidar_dev_pcl_init_shared_mem();
	reset_count_info(ab_part);
	
	while(!stop)
	{
		nbytes = static_cast<int>(recvfrom(sock_fd, pcl_ptr, MAX_DATA_SIZE, 0, (struct sockaddr *)&pcl_device_addr, &sock_len));
		if (nbytes < 0 && errno == EAGAIN) {
			std::cout <<"Recv from pcl device timeout"<<std::endl;
			continue;
		}
#ifdef DEBUG_COMMUNICATION_INFO
		std::cout<<"package_count is : "<<package_count<<" received : "<<nbytes<<std::endl;
#endif
		if(check_pcl_package(pcl_ptr,package_count) != true)
		{
#ifdef DEBUG_SYNC_FRAME_INFO
                        std::cout<<"check package failed "<<std::endl;
#endif
		}

		if(0 == package_count%250)
		{
			check_pcl_frame(pcl_ptr);
			if(ab_part == true)
			{
				std::cout<<"frame a"<<std::endl;
				ab_part = false;
				reset_count_info(ab_part);
			} else {
				std::cout<<"frame b"<<std::endl;
				ab_part = true;
				reset_count_info(ab_part);
			}
		} else {
			pcl_ptr += nbytes;
			package_count += 1;
		}
	}
}
