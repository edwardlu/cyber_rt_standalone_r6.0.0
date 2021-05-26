#include "livox_protocol.hpp"

const uint8_t kSdkProtocolSof = 0xAA;
const uint32_t kSdkPacketCrcSize = 4;          // crc32
const uint32_t kSdkPacketPreambleCrcSize = 2;  // crc16

livox_protocol::livox_protocol(uint16_t seed16, uint32_t seed32) : crc16_(seed16), crc32_(seed32)
{

}

livox_protocol::~livox_protocol()
{

}

int32_t livox_protocol::ParsePacket(uint8_t *in_buf, uint32_t in_len, CommPacket *out_packet)
{
	SdkPackage *sdk_packet = (SdkPacket *)in_buf;

	if(in_len < GetPacketWrapperLen()) {
		return -1;  // packet lenth error
	}

	memset((void *)out_packet, 0, sizeof(CommPacket));

	out_packet->packet_type      = sdk_packet->packet_type;
	out_packet->protocol         = 0;
	out_packet->protocol_version = sdk_packet->version;
	out_packet->seq_num          = sdk_packet->seq_num;
	out_packet->cmd_set          = sdk_packet->cmd_set;
	out_packet->cmd_code         = sdk_packet->cmd_id;
	out_packet->data_len         = sdk_packet->length - GetPacketWrapperLen();
	out_packet->data             = sdk_packet->data;

	return 0;
}

int32_t livox_protocol::Pack(uint8_t *out_buf, uint32_t out_buf_size, uint32_t *out_len, const CommPacket &in_packet)
{
	SdkPackage *sdk_packet = (SdkPackage *)out_buf;

	if(0 != in_packet.protocol) {
		return -1;
	}

	sdk_packet->sof = kSdkProtocolSof;
	sdk_packet->length = in_packet.data_len + GetPacketWrapperLen();

	if (sdk_packet->length > out_buf_size) {
		return -1;
	}

	sdk_packet->version      = 1;
	sdk_packet->packet_type  = in_packet.packet_type;
	sdk_packet->seq_num      = in_packet.seq_num & 0xFFFF;
	sdk_packet->preamble_crc = crc16_.mcrf4xx_calc(out_buf, GetPreambleLen() - kSdkPacketPreambleCrcSize);
	sdk_packet->cmd_set      = in_packet.cmd_set;
	sdk_packet->cmd_id       = in_packet.cmd_code;

	memcpy(sdk_packet->data, in_packet.data, in_packet.data_len);

	uint32_t crc = crc32_.crc32_calc(out_buf, sdk_packet->length - kSdkPacketCrcSize);
	out_buf[sdk_packet->length - 4] = crc & 0xFF;
	out_buf[sdk_packet->length - 3] = (crc >> 8) & 0xFF;
	out_buf[sdk_packet->length - 2] = (crc >> 16) & 0xFF;
	out_buf[sdk_packet->length - 1] = (crc >> 24) & 0xFF;

	*out_len = sdk_packet->length;

	return 0;
}

uint32_t livox_protocol::GetPreambleLen()
{
	return sizeof(SdkPreamble);
}

uint32_t livox_protocol::GetPacketWrapperLen()
{
	return sizeof(SdkPacket) - 1 + kSdkPacketCrcSize;
}

uint32_t livox_protocol::GetPacketLen(uint8_t *buf)
{
	SdkPreamble *preamble = (SdkPreamble *)buf;
	return preamble->length;
}

int32_t livox_protocol::CheckPreamble(uint8_t *buf)
{
	SdkPreamble *preamble = (SdkPreamble *)buf;

	if((preamble->sof == kSdkProtocolSof) && 
		(crc16_.mcrf4xx_calc(buf, GetPreambleLen()) == 0)) 
	{
		return 0;
	} else {
		return -1;
	}
}

int32_t livox_protocol::CheckPacket(uint8_t *buf)
{
	SdkPacket *sdk_packet = (SdkPacket *)buf;
	uint32_t crc = ((uint32_t)(buf[sdk_packet->length - 4])) | 
		(((uint32_t)(buf[sdk_packet->length - 3])) << 8) |
		(((uint32_t)(buf[sdk_packet->length - 2])) << 16)| 
		(((uint32_t)(buf[sdk_packet->length - 1])) << 24);

	if (crc32_.crc32_calc(buf, sdk_packet->length - kSdkPacketCrcSize) == crc) {
		return 0;
	} else {
		return -1;
	}
}

