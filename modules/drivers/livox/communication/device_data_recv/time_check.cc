#include "time_check.hpp"

uint64_t Now() {
	auto now = std::chrono::high_resolution_clock::now();
	auto nano_time_point = std::chrono::time_point_cast<std::chrono::nanoseconds>(now);
	auto epoch = nano_time_point.time_since_epoch();
	uint64_t now_nano = std::chrono::duration_cast<std::chrono::nanoseconds>(epoch).count();
	return now_nano;
}

void create_time_point(uint64_t time,struct time_delt *out)
{
	out->seconds      = time/1000000000UL;
	out->nanoseconds  = time%1000000000UL;
	out->check_status = true;
}

void update_time_point(struct time_delt *in,struct time_delt *out)
{
	if(in->check_status == true)
	{
		out->seconds      = in->seconds;
		out->nanoseconds  = in->nanoseconds;
		out->check_status = true;
	}
}

void compare_time_point(struct time_delt *before,struct time_delt *after,struct time_delt *result)
{
	if((before == nullptr)||(after == nullptr)||(result == nullptr))
	{
		std::cout<<"invalidated time point"<<std::endl;
		result->check_status = false;
	}

	if(after->seconds == before->seconds)
	{
		if(after->nanoseconds < before->nanoseconds)
		{
			result->check_status = false;
		} else {
			result->seconds = 0;
			result->nanoseconds = after->nanoseconds - before->nanoseconds;
			result->check_status = true;
		}
	} else if(after->seconds > before->seconds) { //normal
		result->seconds = ((after->seconds*1000000000UL+after->nanoseconds)
			- (before->seconds*1000000000UL+before->nanoseconds))/1000000000UL;
		result->nanoseconds = ((after->seconds*1000000000UL+after->nanoseconds)
			- (before->seconds*1000000000UL+before->nanoseconds))%1000000000UL;
		result->check_status = true;
	} else if(after->seconds < before->seconds) {
		result->check_status = false;
	}
}

