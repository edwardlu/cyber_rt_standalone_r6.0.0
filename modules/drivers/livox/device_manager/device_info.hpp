#ifndef DEVICE_MAP_H
#define DEVICE_MAP_H

#include <map>
#include <vector>
#include <string.h>
#include <sstream>
#include "socket_util.hpp"
#include "../common/common_def.hpp"

//#define IP_LEN 32
#define BROADCAST_CODE_LEN 32
#define FIRMWARE_VERSION_LEN 4
#define BASE_PORT_CMD 55001
#define BASE_PORT_DATA 55002
#define BASE_PORT_SENSOR 55003

#define DEBUG_DEVICEINFO_DEBUG 1

typedef enum {
	kLidarFeatureNone = 0,       //No feature.
	kLidarFeatureRainFog = 1  //Rain and fog feature.
} LidarFeature;

typedef enum {
	kDeviceTypeHub = 0,          //Livox Hub.
	kDeviceTypeLidarMid40 = 1,   //Mid-40.
	kDeviceTypeLidarTele = 2,    // Tele.
	kDeviceTypeLidarHorizon = 3, //Horizon.
	kDeviceTypeLidarMid70 = 6,    //Livox Mid-70.
	kDeviceTypeLidarAvia = 7     // Avia.
} DeviceType;

struct LidarStatus {
	uint32_t temp_status : 2;     // 0: Temperature in Normal State. 1: High or Low. 2: Extremely High or Extremely Low. 
	uint32_t volt_status : 2;        // 0: Voltage in Normal State. 1: High. 2: Extremely High. */
	uint32_t motor_status : 2;   // 0: Motor in Normal State. 1: Motor in Warning State. 2:Motor in Error State, Unable to Work. 
	uint32_t dirty_warn : 2;       // 0: Not Dirty or Blocked. 1: Dirty or Blocked.
	uint32_t firmware_err : 1;    // 0: Firmware is OK. 1: Firmware is Abnormal, Need to be Upgraded.
	uint32_t pps_status : 1;          // 0: No PPS Signal. 1: PPS Signal is OK.
	uint32_t device_status : 1;   // 0: Normal. 1: Warning for Approaching the End of Service Life.
	uint32_t fan_status : 1;          //0: Fan in Normal State. 1: Fan in Warning State.
	uint32_t self_heating : 1;     // 0: Normal. 1: Low Temperature Self Heating On.
	uint32_t ptp_status : 1;         // 0: No 1588 Signal. 1: 1588 Signal is OK.
	uint32_t time_sync_status : 3;	/** 0: System dose not start time synchronization.
								* 1: Using PTP 1588 synchronization.
								* 2: Using GPS synchronization.
								* 3: Using PPS synchronization.
								* 4: System time synchronization is abnormal.(The highest priority synchronization signal is abnormal)
								*/
	uint32_t rsvd : 13;		//Reserved.
	uint32_t system_status : 2;	// 0: Normal. 1: Warning. 2: Error.
};

struct DeviceInfo {
	char broadcast_code[BROADCAST_CODE_LEN]; //Device broadcast code, null-terminated string, 15 characters at most.
	uint8_t id;                                           // LiDAR id.
	uint8_t type;                                     // Device type, refer to \ref DeviceType.
	uint16_t cmd_port;                       // Control command UDP port.
	uint16_t data_port;                       // Point cloud data UDP port.
	uint16_t sensor_port;                  // IMU data UDP port.
	char server_ip[IP_STRING_LEN];               //server(local interface) IP address char.
	uint32_t s_ip;							//server(local interface) IP address uint32.
	bool handshaked;
	LidarFeature feature;                  // LiDAR feature.
	uint8_t firmware_version[FIRMWARE_VERSION_LEN];            // Firmware version.
};

class lidar_dev_manager
{
public:
	lidar_dev_manager(const lidar_dev_manager& dev) = delete;
	lidar_dev_manager &operator=(const lidar_dev_manager& dev) = delete;
	~lidar_dev_manager();
	
	static lidar_dev_manager& get_instance() {
		static lidar_dev_manager instance;
		return instance;
	}
	
	void set_device_status(uint8_t dev_id,bool stat);

	bool find_device(uint8_t dev_id, DeviceInfo &info);
	bool is_device_connected(uint8_t dev_id);
	bool IsLidarMid40(uint8_t dev_id);
	bool IsLidarMid70(uint8_t dev_id);
	bool IsLidarTele(uint8_t dev_id);
	bool IsLidarAvia(uint8_t dev_id);
	bool GetLidarFirmwareVersion(uint8_t dev_id, uint32_t &firm_ver);

private:
	lidar_dev_manager();
	socket_util sock_util;
	void init_devices(std::vector<std::string> &interfaces);
	std::map<int,DeviceInfo> device_map;
};

#endif 
