#include "block_map.h"

block_map::block_map()
{

}

block_map::~block_map()
{
	
}

void block_map::block_map_insert(block_ptr& b_desc)
{
	blk_map.insert(std::make_pair<>(b_desc.get()->blk_id,b_desc));
}

struct block_desc* block_map::get_block_desc(int block_id)
{
	return blk_map[block_id].get();
}
