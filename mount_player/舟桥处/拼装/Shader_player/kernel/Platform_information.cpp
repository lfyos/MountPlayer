#include "StdAfx.h"
#include "Platform_information.h"

static double time_modify_value=0,stop_timer_value=0;

double Platform_information::get_current_time()
{
	return ((double)::GetTickCount())-time_modify_value;
}
void Platform_information::stop_timer()
{
	stop_timer_value=(double)::GetTickCount();
}
void Platform_information::start_timer()
{
	time_modify_value+=((double)::GetTickCount())-stop_timer_value;
}