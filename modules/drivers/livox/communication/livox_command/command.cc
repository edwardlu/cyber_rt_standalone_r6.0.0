#include "command.hpp"
#include <arpa/inet.h>

void livox_command::create_instruction_handshake(uint8_t *data,int *data_len,uint32_t ip,uint16_t d_port,uint16_t c_port,uint16_t s_port)
{
	if(data != nullptr)
	{
		struct HandshakeRequest handshake_req;
		*data_len = sizeof(handshake_req);

		handshake_req.ip_addr = ip;
		handshake_req.cmd_port = c_port;
		handshake_req.data_port = d_port;
		handshake_req.sensor_port = s_port;
		memcpy(data,(uint8_t *)&handshake_req,*data_len);
	}
}

void livox_command::create_instruction_inquiry(uint8_t *data,int *data_len)
{
	*data_len = 0;
}

void livox_command::create_instruction_heartbeat(uint8_t *data,int *data_len)
{
	*data_len = 0;
}

void livox_command::create_instruction_sample(uint8_t *data,int *data_len,uint8_t s_ctrl)
{
	if(data != nullptr){
		struct Sample samp;
		*data_len = sizeof(Sample);
		
		samp.sample_ctrl = s_ctrl;
		memcpy(data,(uint8_t *)&samp,*data_len);
	}
}

void livox_command::create_instruction_changecoord(uint8_t *data,int *data_len,uint8_t co_type)
{
	if(data != nullptr){
		struct ChangeCoord coord;
		*data_len = sizeof(coord);
		
		coord.coordinate_type = co_type;
		memcpy(data,(uint8_t *)&coord,*data_len);
	}
}

void livox_command::create_instruction_disconnect(uint8_t *data,int *data_len)
{
	*data_len = 0;
}

void livox_command::create_instruction_exception(uint8_t *data,int *data_len,uint8_t s_code)
{
	if(data != nullptr){
		struct ExceptionInfo excpinfo;
		*data_len = sizeof(excpinfo);
		
		excpinfo.status_code = s_code;
		memcpy(data,(uint8_t *)&excpinfo,*data_len);
	}
}

void livox_command::create_instruction_set_ipaddr(uint8_t *data,int *data_len,uint8_t mode,uint32_t ip_addr,uint32_t mask,uint32_t gate)
{
	if(data != nullptr)
	{
		struct IpaddrSet ipset;
		*data_len = sizeof(ipset);
		
		ipset.ip_mode = mode;
		ipset.ip = ip_addr;
		ipset.net_mask = mask; 
		ipset.gate_way = gate;
		memcpy(data,(uint8_t *)&ipset,*data_len);
	}
}

void livox_command::create_instruction_get_ipaddr(uint8_t *data,int *data_len)
{
	*data_len = 0;
}

void livox_command::create_instruction_reboot(uint8_t *data,int *data_len,uint16_t t_out)
{
	if(data != nullptr)
	{
		struct Reboot reb;
		*data_len = sizeof(reb);
		
		reb.timeout = t_out;
		memcpy(data,(uint8_t *)&reb,*data_len);
	}
}

void livox_command::create_instruction_write(uint8_t *data,int *data_len,uint8_t *ptr)
{
	if(data != nullptr)
	{
	//memcpy(ptr,"0",127);
	}
}

void livox_command::create_instruction_read(uint8_t *data,int *data_len,uint8_t p_num)
{
	if(data != nullptr)
	{
	//memcpy(ptr,"0",127);
	}

}

void livox_command::create_instruction_change_mode(uint8_t *data,int *data_len,uint8_t mod)
{
	if(data != nullptr)
	{
		struct ChangeMode cha_mode;
		*data_len = sizeof(cha_mode);
		
		cha_mode.mode = mod;
		memcpy(data,(uint8_t *)&cha_mode,*data_len);
	}
}

void livox_command::create_instruction_set_calib(uint8_t *data,int *data_len,float r,float p,float y,uint32_t x_,uint32_t y_,uint32_t z_)
{
	if(data != nullptr)
	{
		struct SetExtrinsicCali cali;
		*data_len = sizeof(cali);
		
		cali.roll = r;
		cali.pitch = p;
		cali.yaw = y;
		cali.x = x_;
		cali.y = y_;
		cali.z = z_;
		memcpy(data,(uint8_t *)&cali,*data_len);
	}
}

void livox_command::create_instruction_get_calib(uint8_t *data,int *data_len)
{
	*data_len = 0;
}

void livox_command::create_instruction_fog(uint8_t *data,int *data_len,uint8_t state_)
{
	if(data != nullptr)
	{
		struct AntiFog fog;
		*data_len = sizeof(fog);
		
		fog.state = state_;
		memcpy(data,(uint8_t *)&fog,*data_len);
	}
}

void livox_command::create_instruction_fancontrol(uint8_t *data,int *data_len,uint8_t state_)
{
	if(data != nullptr)
	{
		struct Fancontorl fctrl;
		*data_len = sizeof(fctrl);
		
		fctrl.state = state_;
		memcpy(data,(uint8_t *)&fctrl,*data_len);
	}
}

void livox_command::create_instruction_get_fan(uint8_t *data,int *data_len)
{
	*data_len = 0;
}

void livox_command::create_instruction_set_echo(uint8_t *data,int *data_len,uint8_t mode_)
{
	if(data != nullptr)
	{
		struct SetEchoModle echo;
		*data_len = sizeof(echo);
		
		echo.mode = mode_;
		memcpy(data,(uint8_t *)&echo,*data_len);
	}
}

void livox_command::create_instruction_get_echo(uint8_t *data,int *data_len)
{
	*data_len = 0;
}

void livox_command::create_instruction_set_imu(uint8_t *data,int *data_len,uint8_t freq)
{
	if(data != nullptr)
	{
		struct SetImuFreq imufreq;
		*data_len = sizeof(imufreq);
		
		imufreq.freq_mode = freq;
		memcpy(data,(uint8_t *)&imufreq,*data_len);
	}
}

void livox_command::create_instruction_get_imu(uint8_t *data,int *data_len)
{
	*data_len = 0;
}
