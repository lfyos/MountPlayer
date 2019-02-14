#pragma once
#include "shader_base.h"

class Shader_coordinate :public Shader_base
{
	Shader_part_information info;
public:
	static void set_display_flag(bool new_display_flag);
	static bool get_display_flag();

	Shader_coordinate(CString my_class_name,CString my_directory_name=_T(""),CString my_file_name=_T(""))
		:Shader_base(my_class_name,my_directory_name,my_file_name)
	{
	};
	virtual ~Shader_coordinate(void){};

	virtual Shader_base *clone_shader(CString my_directory_name,CString my_file_name)
	{
		return new Shader_coordinate(class_name,my_directory_name,my_file_name);
	}
	virtual void set_reload_material(){};
	virtual void set_material(Component_display_parameter *par,Component_display_flag *flag);
	virtual int test_display(Component *comp,Component_display_parameter *par,Component_display_flag *flag,Component_display_list *display_list);
	virtual Shader_part_information *get_part_information();
	virtual Component_private_data_storage *create_component_private_data_storage(Component *comp,ifstream &f);
};
