#ifndef LIVOX_COMMAND_H
#define LIVOX_COMMAND_H

#include <iostream>
#include <string.h>

#define LIVOX_CMD_TYPE_CMD 0
#define LIVOX_CMD_TYPE_ACK 1
#define LIVOX_CMD_TYPE_MSG 2

#define LIVOX_CMD_SET 0
#define LIVOX_CMD_ID_HANDSHAKE 1
#define LIVOX_CMD_HANDSHAKE_OK 0
#define LIVOX_CMD_HANDSHAKE_FAIL 1

#define LIVOX_CMD_ID_HEARTBEAT_ACK 3

#define LIVOX_CMD_ID_START_SAMPLE 4
#define LIVOX_CMD_START_SAMPLE_ENABLE 1
#define LIVOX_CMD_START_SAMPLE_DISENABLE 0

typedef enum {
	kCommandTypeCmd = 0, //requires response from the receiver
	kCommandTypeAck = 1, //response of command type
	kCommandTypeMsg = 2  //sent at a specified frequency
} CommandType;

typedef enum {
	kCommandSetGeneral = 0, //general command set
	kCommandSetLidar,       //LiDAR command set
	kCommandSetHub          //hub command set
} CommandSet;

typedef enum {
	kCommandIDGeneralBroadcast = 0,
	kCommandIDGeneralHandshake = 1,
	kCommandIDGeneralDeviceInfo = 2,
	kCommandIDGeneralHeartbeat = 3,
	kCommandIDGeneralControlSample = 4,
	kCommandIDGeneralCoordinateSystem = 5,
	kCommandIDGeneralDisconnect = 6,
	kCommandIDGeneralPushAbnormalState = 7,
	kCommandIDGeneralConfigureStaticDynamicIp = 8,
	kCommandIDGeneralGetDeviceIpInformation = 9,
	kCommandIDGeneralRebootDevice = 0x0a,
	kCommandIDGeneralSetDeviceParam = 0x0b,
	kCommandIDGeneralGetDeviceParam = 0x0c,
	kCommandIDGeneralResetDeviceParam = 0x0d,
	kCommandIDGeneralCommandCount
} GeneralCommandID;

typedef enum {
	kCommandIDLidarSetMode = 0,
	kCommandIDLidarSetExtrinsicParameter = 1,
	kCommandIDLidarGetExtrinsicParameter = 2,
	kCommandIDLidarControlRainFogSuppression = 3,
	kCommandIDLidarControlFan = 4,
	kCommandIDLidarGetFanState = 5,
	kCommandIDLidarSetPointCloudReturnMode = 6,
	kCommandIDLidarGetPointCloudReturnMode = 7,
	kCommandIDLidarSetImuPushFrequency = 8,
	kCommandIDLidarGetImuPushFrequency = 9,
	kCommandIDLidarSetSyncTime = 0x0a,
	kCommandIDLidarCommandCount
} LidarCommandID;

#pragma pack(1)
/*handshake 0x00 0x01*/
struct HandshakeRequest {
	uint32_t ip_addr;     //IP address of the device
	uint16_t data_port;   //UDP port of the data connection
	uint16_t cmd_port;    //UDP port of the command connection
	uint16_t sensor_port; //UDP port of the sensor connection
};
struct HandshakeRequestResponse
{
	uint8_t ret_code;
};

/*inquiry 0x00 0x02*/
struct DeviceInquiry
{
	uint8_t data[0];
};
struct DeviceInquiryResponse
{
	uint8_t ret_code;
	uint32_t version;
};

/*heartbeat 0x00 0x03*/
struct Heartbeat {
	uint8_t data[0];
};
struct HeartbeatResponse {
	uint8_t ret_code;        //Return code
	uint8_t state;           //Working state
	uint8_t feature;         //LiDAR feature
	uint32_t error_union;    //LiDAR work state
};

/*sample 0x00 0x04*/
struct Sample
{
	uint8_t sample_ctrl;
};
struct SampleResponse
{
	uint8_t ret_code;
};

/*chance coordinate 0x00 0x05*/
struct ChangeCoord
{
	uint8_t coordinate_type;
};
struct ChangeCoordResponse
{
	uint8_t ret_code;
};

/*disconnect 0x00 0x06*/
struct Disconnect
{
	uint8_t data[0];
};
struct DisconnectResponse
{
	uint8_t ret_code;
};

/*exception info 0x00 0x07*/
struct ExceptionInfo
{
	uint8_t status_code;
};
struct ExceptionInfoResponse
{
	uint8_t data[0];
};

/*ipaddr set 0x00 0x08*/
struct IpaddrSet
{
	uint8_t ip_mode;
	uint32_t ip;
	uint32_t net_mask;
	uint32_t gate_way;
};
struct IpaddrSetResponse
{
	uint8_t ret_code;
};

/*getipaddr 0x00 0x09*/
struct GetIp
{
	uint8_t data[0];
};
struct GetIpResponse
{
	uint8_t ret_code;
	uint8_t ip_mode;
	uint32_t ip;
	uint32_t net_mask;
	uint32_t gate_way;
};

/*reboot 0x00 0x0A*/
struct Reboot{
	uint16_t timeout;
};
struct RebootResponse
{
	uint8_t ret_code;
};

/*Writedata 0x00 0x0B*/
struct WriteData
{
	uint8_t data[128];
};
struct WriteDataResponse
{
	uint8_t ret_code;
	uint16_t error_key;
	uint8_t error_code;
};

/*ReadData 0x00 0x0C*/
struct ReadData
{
	uint8_t param_num;
	uint8_t key[128];
};
struct ReadDataResponse
{
	uint8_t ret_code;
	uint16_t error_key;
	uint8_t error_code;
	uint8_t data[128];
};

/*changemode 0x01 0x00 */
struct ChangeMode
{
	uint8_t mode;
};
struct ChangeModeResponse
{
	uint8_t ret_code;
};

/*set extrinsic calibration 0x01 0x01*/
struct SetExtrinsicCali
{
	float roll;
	float pitch;
	float yaw;
	uint32_t x;
	uint32_t y;
	uint32_t z;
};
struct SetExtrinsicCaliResponse
{
	uint8_t ret_code;
};

/*get extrinsic calibration 0x01 0x02*/
struct GetExtrinsicCali
{
	uint8_t data[0];
};
struct GetExtrinsicCaliResponse
{
	uint8_t ret_code;
	float roll;
        float pitch;
        float yaw;
        uint32_t x;
        uint32_t y;
        uint32_t z;
};

/*Anti-fog 0x00 0x03*/
struct AntiFog
{
	uint8_t state;
};
struct AntiFogResponse
{
	uint8_t ret_code;
};

/*Fancontorl 0x01 0x04*/
struct Fancontorl
{
	uint8_t state;
};
struct FancontorlResponse
{
	uint8_t ret_code;
};

/*ReadFancontrolstate 0x01 0x05*/
struct ReadFancontrolState
{
	uint8_t data[0];
};
struct ReadFancontrolStateResponse
{
	uint8_t ret_code;
	uint8_t state;
};

/*SetEchoModle 0x01 0x06*/
struct SetEchoModle
{
	uint8_t mode;
};
struct SetEchoModleResponse
{
	uint8_t ret_mode;
};

/*GetEchoModle 0x01 0x07*/
struct GetEchoModle
{
	uint8_t data[0];
};
struct GetEchoModleResponse
{
	uint8_t ret_code;
	uint8_t mode;
};

/*SetIMU 0x01 0x08*/
struct SetImuFreq
{
	uint8_t freq_mode;
};
struct SetImuFreqResponse {
	uint8_t ret_code;
};

/*GetIMU 0x01 0x09*/
struct GetImuFreq
{
	uint8_t data[0];
};
struct GetImuFreqResponse {
	uint8_t ret_code;
	uint8_t freq_mode;
};
#pragma pack()
class livox_command
{
public:
	livox_command() = default;
	~livox_command() = default;

	//create handshake
	void create_instruction_handshake(uint8_t *data,int *data_len,uint32_t ip,uint16_t d_port,uint16_t c_port,uint16_t s_port);
	void create_instruction_inquiry(uint8_t *data,int *data_len);
	void create_instruction_heartbeat(uint8_t *data,int *data_len);
	void create_instruction_sample(uint8_t *data,int *data_len,uint8_t s_ctrl);
	void create_instruction_changecoord(uint8_t *data,int *data_len,uint8_t co_type);
	void create_instruction_disconnect(uint8_t *data,int *data_len);
	void create_instruction_exception(uint8_t *data,int *data_len,uint8_t s_code);
	void create_instruction_set_ipaddr(uint8_t *data,int *data_len,uint8_t mode,uint32_t ip_addr,uint32_t mask,uint32_t gate);
	void create_instruction_get_ipaddr(uint8_t *data,int *data_len);
	void create_instruction_reboot(uint8_t *data,int *data_len,uint16_t t_out);
	void create_instruction_write(uint8_t *data,int *data_len,uint8_t *ptr);
	void create_instruction_read(uint8_t *data,int *data_len,uint8_t p_num);
	void create_instruction_change_mode(uint8_t *data,int *data_len,uint8_t mod);
	void create_instruction_set_calib(uint8_t *data,int *data_len,float r,float p,float y,uint32_t x_,uint32_t y_,uint32_t z_);
	void create_instruction_get_calib(uint8_t *data,int *data_len);
	void create_instruction_fog(uint8_t *data,int *data_len,uint8_t state_);
	void create_instruction_fancontrol(uint8_t *data,int *data_len,uint8_t state_);
	void create_instruction_get_fan(uint8_t *data,int *data_len);
	void create_instruction_set_echo(uint8_t *data,int *data_len,uint8_t mode_);
	void create_instruction_get_echo(uint8_t *data,int *data_len);
	void create_instruction_set_imu(uint8_t *data,int *data_len,uint8_t freq);
	void create_instruction_get_imu(uint8_t *data,int *data_len);

private:
	//livox_device_info dev; //get ip port[command] port[imu] port[pcl]
};

#endif
