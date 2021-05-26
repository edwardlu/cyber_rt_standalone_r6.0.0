#ifndef BLOCK_MAP_H
#define BLOCK_MAP_H

#include <iostream>
#include <atomic>
#include <utility>
#include <memory>
#include <map>
#include <string.h>

#define DEBUG 1

#define TOTAL_BLOCK_NUM 12

#define BLOCK_LEN_SMALL 1024*1024
#define BLOCK_LEN_BIG   1024*1024*4
#define BLOCK_DESC_OFFSET sizeof(struct block_desc)

struct block_desc
{
	block_desc(const char *bname,int bsize,int bid)
	{
#ifdef DEBUG
		std::cout<<"block_desc with param called"<<std::endl;
#endif
		memcpy(blk_name,bname,32);
		size = bsize;
		blk_id = bid;
	}
	~block_desc()
	{
#ifdef DEBUG
		std::cout<<"block_desc destruct called"<<std::endl;
#endif
	}

	char blk_name[32];
	int size; // BLOCK_LEN
	int blk_id;
	std::atomic<bool> in_use;
};

class block_map
{
public:
	block_map();
	~block_map();

	using block_ptr = std::shared_ptr<struct block_desc>;

	void block_map_insert(block_ptr& b_desc);
	struct block_desc* get_block_desc(int block_id);

private:
	std::map<int,block_ptr> blk_map;
};

#endif
