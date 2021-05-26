#include "../block_map.h"
#include "../block_interface.h"

#include <unistd.h>
#include <vector>
#include <sstream>
#include <string.h>
#include <thread>

bool stop = false;

void dump_block_info(struct block_desc *b_desc)
{
	std::cout<<"block ptr  : "<<(uint64_t *)b_desc<<std::endl;
	std::cout<<"block name : "<<b_desc->blk_name<<std::endl;
	std::cout<<"block size : "<<b_desc->size<<std::endl;
	std::cout<<"block id   : "<<b_desc->blk_id<<std::endl;
	std::cout<<"block addr : "<<(uint64_t *)b_desc->start_addr<<std::endl;
}

int main()
{
	//alloc in shm
	block_interface shm_intf(123456789111);
	shm_intf.init_block_info();

	struct block_desc *ptr1 = shm_intf.get_block(0);
	struct block_desc *ptr2 = shm_intf.get_block(1);
	
	ptr1->in_use.store(true);
	ptr2->in_use.store(true);
	sleep(5);	
	dump_block_info(ptr1);
	dump_block_info(ptr2);
	ptr1->in_use.store(false);
	ptr2->in_use.store(false);

#if 0	
	std::thread t1([&stop](struct block_desc * b_desc){
		while(!stop)
		{
			if(b_desc->inited == false)
				std::cout<<"not inited"<<std::endl;
			else
				std::cout<<"inited"<<std::endl;

		}
	},ptr1);

	std::thread t2([&stop](struct block_desc * b_desc){
		while(!stop)
		{
			sleep(1);
			b_desc->inited = true;
			sleep(1);
			b_desc->inited = false;
		}
	},ptr1);

	sleep(10);
	stop = true;

	t1.join();
	t2.join();
#endif
	sleep(5);
	return 0;
}
