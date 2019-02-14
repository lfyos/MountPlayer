#include "StdAfx.h"
#include "Part_parameter.h"

Part_parameter::Part_parameter(CString &my_directory_name,ifstream &f_part_file)
{
		char buffer[MAX_BUFFER_LENGTH];
		directory_name=my_directory_name;
		f_part_file>>buffer;				f_part.open(directory_name+CString(buffer));
		f_part_file>>precision;
		f_part_file>>buffer;				class_name=buffer;

		for(int i=0;i<MAX_SHADER_NUMBER;i++){
			f_part_file>>buffer;
			shader_file_name[i]=CString(buffer);
		}
}

Part_parameter::~Part_parameter()
{
		f_part.close();
}

bool Part_parameter::load()
{
		char buffer[MAX_BUFFER_LENGTH];

		f_part>>buffer;		user_name				=buffer;
		f_part>>buffer;		part_name				=buffer;

		f_part>>buffer;		mesh_file_name			=buffer;

		f_part>>buffer;		material_file_name		=buffer;
		f_part>>buffer;		description_file_name	=buffer;
		f_part>>buffer;		sound_file_name			=buffer;
		
		if((f_part.eof())||(f_part.fail()))
			return false;
		else
			return true;
}
