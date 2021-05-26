#ifndef LIVOX_PROTOCOL_H
#define LIVOX_PROTOCOL_H


#include <stdint.h>
#include <string.h>
#include "./FastCRC/FastCRC.h"

#pragma pack(1)

struct CommandPacket {
	uint8_t packet_type;
	uint8_t protocol;
	uint8_t protocol_version;
	uint8_t cmd_set;
	uint32_t cmd_code;
	uint32_t sender;
	uint32_t sub_sender;
	uint32_t receiver;
	uint32_t sub_receiver;
	uint32_t seq_num;
	uint8_t *data;
	uint16_t data_len;
	uint32_t padding;
};

struct SdkPackagePreamble {
	uint8_t sof;
	uint8_t version;
	uint16_t length;
	uint8_t packet_type;
	uint16_t seq_num;
	uint16_t preamble_crc;
};

struct SdkPackage {
	uint8_t sof;
	uint8_t version;
	uint16_t length;
	uint8_t packet_type;
	uint16_t seq_num;
	uint16_t preamble_crc;
	uint8_t cmd_set;
	uint8_t cmd_id;
	uint8_t data[1];
};

/** Point cloud packet. */
struct LivoxEthPacket {
	uint8_t version;              //Packet protocol version.
	uint8_t slot;                 //Slot number used for connecting LiDAR.
	uint8_t id;                   //LiDAR id.
	uint8_t rsvd;                 //Reserved.
	uint32_t err_code;            //Device error status indicator information.
	uint8_t timestamp_type;       //Timestamp type.
				      //Point cloud coordinate format, refer to \ref PointDataType.
	uint8_t data_type;
	uint8_t timestamp[8];         //Nanosecond or UTC format timestamp.
	uint8_t data[1];              //Point cloud data.
};

/** Cartesian coordinate format. */
struct LivoxRaw {
	int32_t x;            //X axis, Unit:mm
	int32_t y;            //Y axis, Unit:mm
	int32_t z;            //Z axis, Unit:mm
	uint8_t reflectivity; //Reflectivity
};

/** Spherical coordinate format. */
struct LivoxSpher {
	uint32_t depth;       //Depth, Unit: mm
	uint16_t theta;       //Zenith angle[0, 18000], Unit: 0.01 degree
	uint16_t phi;         //Azimuth[0, 36000], Unit: 0.01 degree
	uint8_t reflectivity; //Reflectivity
};

/** Extend cartesian coordinate format. */
struct LivoxExtendRaw {
	int32_t x;            //X axis, Unit:mm
	int32_t y;            //Y axis, Unit:mm
	int32_t z;            //Z axis, Unit:mm
	uint8_t reflectivity; //Reflectivity
	uint8_t tag;          //Tag
};

/** Extend spherical coordinate format. */
struct LivoxExtendSpher {
	uint32_t depth;       //Depth, Unit: mm
	uint16_t theta;       //Zenith angle[0, 18000], Unit: 0.01 degree
	uint16_t phi;         //Azimuth[0, 36000], Unit: 0.01 degree
	uint8_t reflectivity; //Reflectivity
	uint8_t tag;          //Tag
};

/** Dual extend cartesian coordinate format. */
struct LivoxDualExtendRaw {
	int32_t x1;            //X axis, Unit:mm
	int32_t y1;            //Y axis, Unit:mm
	int32_t z1;            //Z axis, Unit:mm
	uint8_t reflectivity1; //Reflectivity
	uint8_t tag1;          //Tag 
	int32_t x2;            //X axis, Unit:mm
	int32_t y2;            //Y axis, Unit:mm
	int32_t z2;            //Z axis, Unit:mm
	uint8_t reflectivity2; //Reflectivity
	uint8_t tag2;          //Tag
};

/** Dual extend spherical coordinate format. */
struct LivoxDualExtendSpher {
	uint16_t theta;        //Zenith angle[0, 18000], Unit: 0.01 degree
	uint16_t phi;          //Azimuth[0, 36000], Unit: 0.01 degree
	uint32_t depth1;       //Depth, Unit: mm
	uint8_t reflectivity1; //Reflectivity
	uint8_t tag1;          //Tag
	uint32_t depth2;       //Depth, Unit: mm
	uint8_t reflectivity2; //Reflectivity
	uint8_t tag2;          //Tag
};

/** IMU data format. */
struct LivoxImu {
	float gyro_x;        //Gyroscope X axis, Unit:rad/s
	float gyro_y;        //Gyroscope Y axis, Unit:rad/s
	float gyro_z;        //Gyroscope Z axis, Unit:rad/s
	float acc_x;         //Accelerometer X axis, Unit:g
	float acc_y;         //Accelerometer Y axis, Unit:g
	float acc_z;         //Accelerometer Z axis, Unit:g
};

/** Triple extend cartesian coordinate format. */
struct LivoxTripleExtendRaw {
	int32_t x1;            //X axis, Unit:mm
	int32_t y1;            //Y axis, Unit:mm
	int32_t z1;            //Z axis, Unit:mm
	uint8_t reflectivity1; //Reflectivity
	uint8_t tag1;          //Tag
	int32_t x2;            //axis, Unit:mm
	int32_t y2;            //Y axis, Unit:mm
	int32_t z2;            //Z axis, Unit:mm
	uint8_t reflectivity2; //Reflectivity
	uint8_t tag2;          //Tag
	int32_t x3;            //X axis, Unit:mm
	int32_t y3;            //Y axis, Unit:mm
	int32_t z3;            //Z axis, Unit:mm
	uint8_t reflectivity3; //Reflectivity
	uint8_t tag3;          //Tag
};

/** Triple extend spherical coordinate format. */
struct LivoxTripleExtendSpher {
	uint16_t theta;        //Zenith angle[0, 18000], Unit: 0.01 degree
	uint16_t phi;          //Azimuth[0, 36000], Unit: 0.01 degree
	uint32_t depth1;       //Depth, Unit: mm
	uint8_t reflectivity1; //Reflectivity
	uint8_t tag1;          //Tag
	uint32_t depth2;       //Depth, Unit: mm
	uint8_t reflectivity2; //Reflectivity
	uint8_t tag2;          //Tag
	uint32_t depth3;       //Depth, Unit: mm
	uint8_t reflectivity3; //Reflectivity
	uint8_t tag3;          //Tag
};

#pragma pack()

/** Point data type. */
typedef enum {
	kCartesian,               //artesian coordinate point cloud.
	kSpherical,               //Spherical coordinate point cloud.
	kExtendCartesian,         //Extend cartesian coordinate point cloud.
	kExtendSpherical,         //Extend spherical coordinate point cloud.
	kDualExtendCartesian,     //Dual extend cartesian coordinate  point cloud.
	kDualExtendSpherical,     //Dual extend spherical coordinate point cloud.
	kImu,                     //IMU data.
	kTripleExtendCartesian,   //Triple extend cartesian coordinate  point cloud.
	kTripleExtendSpherical,   //Triple extend spherical coordinate  point cloud.
	kMaxPointDataType         //Max Point Data Type.
} PointDataType;


typedef CommandPacket CommPacket;
typedef SdkPackagePreamble SdkPreamble;
typedef SdkPackage SdkPacket;

typedef LivoxRaw LivoxRawPoint;
typedef LivoxSpher LivoxSpherPoint;
typedef LivoxExtendRaw LivoxExtendRawPoint;
typedef LivoxDualExtendRaw LivoxDualExtendRawPoint;
typedef LivoxExtendSpher LivoxExtendSpherPoint;
typedef LivoxDualExtendSpher LivoxDualExtendSpherPoint;
typedef LivoxImu LivoxImuPoint;
typedef LivoxTripleExtendRaw LivoxTripleExtendRawPoint;
typedef LivoxTripleExtendSpher LivoxTripleExtendSpherPoint;

class livox_protocol
{
public:
	livox_protocol(uint16_t seed16, uint32_t seed32);
	~livox_protocol();

	int32_t ParsePacket(uint8_t *in_buf, uint32_t in_len, CommPacket *out_packet);
	int32_t Pack(uint8_t *out_buf, uint32_t out_buf_size, uint32_t *out_len, const CommPacket &in_packet);
	uint32_t GetPreambleLen();
	uint32_t GetPacketWrapperLen();
	uint32_t GetPacketLen(uint8_t *buf);
	int32_t CheckPreamble(uint8_t *buf);
	int32_t CheckPacket(uint8_t *buf);

private:
	FastCRC16 crc16_;
	FastCRC32 crc32_;
};

#endif
