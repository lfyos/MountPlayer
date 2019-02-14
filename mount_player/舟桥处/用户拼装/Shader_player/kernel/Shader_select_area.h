#pragma once
#include "shader_base.h"

class Shader_select_area :
	public Shader_base
{
	Shader_part_information info;
public:
	Shader_select_area(CString my_class_name,CString my_directory_name=_T(""),CString my_file_name=_T("")):Shader_base(my_class_name,my_directory_name,my_file_name)
	{
	};
	~Shader_select_area(void)
	{
	}
	virtual Shader_base *clone_shader(CString my_directory_name,CString my_file_name)
	{
		return new Shader_select_area(class_name,my_directory_name,my_file_name);
	}
	virtual void   set_reload_material(){};
	virtual void set_material(Component_display_parameter *par,Component_display_flag *flag);
	virtual int  test_display(Component *comp,Component_display_parameter *par,Component_display_flag *flag,Component_display_list *display_list);
	virtual Shader_part_information *get_part_information();
};

