#include "block_interface.h"

block_interface::block_interface(uint64_t shm_key_id)
{
	key_ = static_cast<key_t>(shm_key_id);
	shmid = shmget(key_,TOTAL_BLOCK_NUM*(BLOCK_LEN_SMALL+BLOCK_DESC_OFFSET), 0666 | IPC_CREAT);
	if(shmid == -1)
	{
#ifdef DEBUG
		std::cout<<"shmget failed"<<std::endl;
#endif
	} else {
		shm_addr = (char *)shmat(shmid, nullptr, 0);
		if (shm_addr == reinterpret_cast<void*>(-1)) {
#ifdef DEBUG
			std::cout<<"shmat failed"<<std::endl;
#endif
			shmctl(shmid, IPC_RMID, 0);
		}
	}
}

block_interface::~block_interface()
{

}

void block_interface::recycle_shared_mem()
{
	int shmid = shmget(key_, 0, 0644);
	if (shmid == -1 || shmctl(shmid, IPC_RMID, 0) == -1) {
#ifdef DEBUG
		std::cout<<"remove shm failed, error code: " << strerror(errno)<<std::endl;
#endif
	}
#ifdef DEBUG
	std::cout<< "remove success."<<std::endl;
#endif

}

struct block_desc* block_interface::get_block(int id)
{
	if(id > TOTAL_BLOCK_NUM)
	{
		std::cout<<"block id is outof range"<<std::endl;
		return nullptr;
	}
	
	return reinterpret_cast<struct block_desc *>(shm_addr+id*BLOCK_DESC_OFFSET);
}

uint8_t * block_interface::get_writeable_area(int id)
{
	return reinterpret_cast<uint8_t *>(shm_addr+TOTAL_BLOCK_NUM*BLOCK_DESC_OFFSET+id*BLOCK_LEN_SMALL);
}

/* MEMORY LAYOUT :
 *
 * block_desc[0] block_desc[1] block_desc[2] block_desc[3] 
 * block_desc[4] block_desc[5] block_desc[6] block_desc[7]
 * -------------------------------------------------------
 * block_desc[0].start    //shm_start + TOTAL_BLOCK_NUM*BLOCK_DESC_OFFSET
 * ...... [512 bytes]
 * block_desc[1].start    //shm_start + TOTAL_BLOCK_NUM*BLOCK_DESC_OFFSET*1
 * ...... [512 bytes]
 * block_desc[2].start    //shm_start + TOTAL_BLOCK_NUM*BLOCK_DESC_OFFSET*2
 * ...... 
 * ......
 * block_desc[7].start    //shm_start + TOTAL_BLOCK_NUM*BLOCK_DESC_OFFSET*7
 * ...... [512 bytes]
 * -------------------------------------------------------
 *
 */
void block_interface::init_block_info()
{
	for(int i = 0; i<TOTAL_BLOCK_NUM; i++)
	{
		std::ostringstream b_name;
		b_name<<"BLOCK_"<<i;

		std::shared_ptr<struct block_desc> blk_ptr(new (shm_addr+ i*BLOCK_DESC_OFFSET)
				//struct block_desc(b_name.str().c_str(),BLOCK_LEN_SMALL,i,reinterpret_cast<char *>(shm_addr+TOTAL_BLOCK_NUM*BLOCK_DESC_OFFSET+i*BLOCK_LEN_SMALL)),
				struct block_desc(b_name.str().c_str(),BLOCK_LEN_SMALL,i),
				[](block_desc * b_desc){
#ifdef DEBUG
				std::cout << "Deleter function called" << std::endl;
#endif
				b_desc->~block_desc();
				});

		blk_map.block_map_insert(blk_ptr);
	}
}

struct block_desc* block_interface::parse_block_desc()
{
	//parse from protobuf config file
	return nullptr;
}
