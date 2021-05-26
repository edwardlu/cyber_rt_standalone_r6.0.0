#ifndef BLOCK_INTERFACE_H
#define BLOCK_INTERFACE_H

#include "block_map.h"
#include <unistd.h>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>

#define DEBUG 1

class block_interface
{
public:
	block_interface() = delete;
	block_interface(uint64_t shm_key_id);
	~block_interface();
	
	struct block_desc* get_block(int id);
	void init_block_info();
	void recycle_shared_mem();
	uint8_t * get_writeable_area(int id);
private:
	struct block_desc* parse_block_desc();

	key_t key_;
	int shmid;
	char *shm_addr;

	block_map blk_map;
	//fix it : make it single instance
};

#endif
