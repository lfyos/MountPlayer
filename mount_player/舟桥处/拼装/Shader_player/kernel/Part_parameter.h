#pragma once

#include"Graphics_system.h"

class Part_parameter
{
	ifstream f_part;
public:
	CString directory_name,class_name,shader_file_name[MAX_SHADER_NUMBER];
	
	double precision;
	CString user_name,part_name;
	CString mesh_file_name,material_file_name,description_file_name,sound_file_name;

	bool load();
	Part_parameter(CString &my_directory_name,ifstream &f_part_file);
	~Part_parameter();
};
