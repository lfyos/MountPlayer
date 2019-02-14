#pragma once
#include "shader_base.h"

class Select_component_ids;
class Part_set;

class Shader_size_tag :public Shader_base
{
	double font_width,font_height;
	int font_texture_object_id;	
public:	
	Shader_part_information info;

	Shader_size_tag(CString my_class_name,CString my_directory_name=_T(""),CString my_file_name=_T(""));
	~Shader_size_tag(void);

	virtual Shader_base *clone_shader(CString my_directory_name,CString my_file_name)
	{
		Shader_size_tag *ret_val=new Shader_size_tag(class_name,my_directory_name,my_file_name);
		for(int  i=0;i<MAX_OBJECT_TYPE_ID;i++)
			ret_val->info.object_type_id[i]=info.object_type_id[i];
		return ret_val;
	};
	virtual void   set_reload_material();
	virtual void set_material(Component_display_parameter *par,Component_display_flag *flag);
	virtual int test_display(Component *comp,Component_display_parameter *par,Component_display_flag *flag,Component_display_list *display_list);
	virtual Shader_part_information *get_part_information();

	static void delete_tag_object(Select_component_ids &ids,Part_set &parts);
	static void add_tag_object(Select_component_ids &ids,Part_set &parts,int my_tag_direction_flag);
	static void clear_all_tag_object();
};
