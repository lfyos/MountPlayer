#include "stdafx.h"
#pragma once

class system_parameter
{
public:
	CString directory_name,system_name,direction_name;
	double min_distance;

	BOOL change_view_flag,scaleup_view_flag,play_sound_flag;

	BOOL manual_mount_flag;
	double manual_mount_proportion;

	system_parameter(void);
	~system_parameter(void);
};

extern system_parameter  *par;