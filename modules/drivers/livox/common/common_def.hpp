#include "cyber/cyber.h"
#include "cyber/init.h"
#include "cyber/io/session.h"

using apollo::cyber::Time;
using apollo::cyber::io::Session;

#define MAX_MESSAGE_BUF_SIZE 1500
#define IP_STRING_LEN 16
#define MAX_DEVICES 5
#define BOARDCAST_CODE_LEN 32

//debug info
//#define DEBUG_CONNECT_INFO 1
#define GRACEFULE_QUIT_LIVOX 1
//#define DEBUG_COMMUNICATION_INFO 1
//#define DEBUG_SYNC_FRAME_INFO 1

#define SHARED_MEM_ID 123456789111
