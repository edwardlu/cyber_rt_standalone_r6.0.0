#include "../block_map.h"
#include "../block_interface.h"

#include <unistd.h>
#include <vector>
#include <sstream>
#include <string.h>
#include <thread>

int main()
{

#if 0
	//alloc by make_shared
	std::shared_ptr<struct block_desc> ptr1 = std::make_shared<struct block_desc>("blk_1",512,1,(void *)0x00001111);
	std::shared_ptr<struct block_desc> ptr2 = std::make_shared<struct block_desc>("blk_2",512,2,(void *)0x00001112);
	std::shared_ptr<struct block_desc> ptr3 = std::make_shared<struct block_desc>("blk_3",512,3,(void *)0x00001113);
#endif

	block_map b_map;
	//alloc in heap
	char *mem_base = new char[1024*1024*1024];
	std::cout<<"##### "<<(void *)mem_base<<" #####"<<std::endl;
	std::cout<<"##### "<<BLOCK_DESC_OFFSET<<" #####"<<std::endl;
	for(int i = 0; i<TOTAL_BLOCK_NUM; i++){
		std::ostringstream b_name;
		b_name << "BLOCK_" << i;

		std::shared_ptr<struct block_desc> ptr1(new (mem_base+i*BLOCK_DESC_OFFSET)
			block_desc(b_name.str().c_str(),BLOCK_LEN_SMALL,i,(void *)mem_base+TOTAL_BLOCK_NUM*BLOCK_DESC_OFFSET+i*BLOCK_LEN_SMALL),
			[](block_desc * b_desc){
			std::cout << "Deleter function called" << std::endl;
			b_desc->~block_desc();
			});

		b_map.block_map_insert(ptr1);
	}
#if 0
	for(int i = 0; i<TOTAL_BLOCK_NUM; i++){
		std::cout<<b_map.get_block_desc(i)->blk_name<<std::endl;
		std::cout<<b_map.get_block_desc(i)->size<<std::endl;
		std::cout<<b_map.get_block_desc(i)->blk_id<<std::endl;
		std::cout<<b_map.get_block_desc(i)->start_addr<<std::endl;
	}

#endif

	std::vector<std::thread> ths;
	std::thread t1([](struct block_desc * b_desc){ 
		while(1)
		{
			if(b_desc->inited == false)
				std::cout<<"not inited"<<std::endl;
			else
				std::cout<<"inited"<<std::endl;

		}
		},b_map.get_block_desc(0));

	std::thread t2([](struct block_desc * b_desc){
		while(1)
		{
			b_desc->inited = true;
			sleep(1);
			b_desc->inited = false;
			sleep(1);
		}
		},b_map.get_block_desc(0));

	t1.join();
	t2.join();

	delete [] mem_base;

	return 0;
}
