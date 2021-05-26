#include <iostream>
#include <chrono>
#include <unistd.h>

struct time_delt
{
	uint64_t seconds;
	uint64_t nanoseconds;
	bool check_status;
};

uint64_t Now();
void create_time_point(uint64_t time,struct time_delt *out);
void update_time_point(struct time_delt *in,struct time_delt *out);
void compare_time_point(struct time_delt *before,struct time_delt *after,struct time_delt *result);
