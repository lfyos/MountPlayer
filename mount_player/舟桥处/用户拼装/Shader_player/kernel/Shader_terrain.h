#pragma once
#include "shader_base.h"
#include "Texture.h"

class Shader_terrain :
	public Shader_base
{
public:
	Shader_terrain(CString my_class_name,CString my_directory_name=_T(""),CString my_file_name=_T(""));
	~Shader_terrain(void);

	bool need_reload_parameter;

	struct{
		unsigned int texure_object_id[5];
		double x0,y0,z0;
		double length,width,height,depth;
		double texture_scale;
		double lod;
	}terrain;

	struct{
		double amplitude,height,wave_length,cycle;
		double a,b;
		double alf;
		double mirror_texture_precision;
	}water;

	Shader_part_information info;

public:
	virtual Shader_base *clone_shader(CString my_directory_name,CString my_file_name)
	{
		return new Shader_terrain(class_name,my_directory_name,my_file_name);
	}
	virtual void   set_reload_material();
	
	virtual void   create_graphics_system_object(int display_map,Component *comp,User_interface *interface_pointer,double precision_scale);
	virtual void   delete_graphics_system_object();

	virtual void set_material(Component_display_parameter *par,Component_display_flag *flag);
	virtual int test_display(Component *comp,Component_display_parameter *par,Component_display_flag *flag,Component_display_list *display_list);
	virtual Shader_part_information *get_part_information();
};
