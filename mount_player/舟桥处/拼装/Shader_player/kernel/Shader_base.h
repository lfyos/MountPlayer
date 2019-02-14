#pragma once

#include"Coordinate_management.h"


class Shader_program_variable_index{
public:
	int order_parameter,clip_plane_parameter;
	int shadow_texture[MAX_LIGHT_NUMBER];
};

class Shader_part_information{
public:
	int object_type_id[MAX_OBJECT_TYPE_ID];
	bool display_flag;
	bool shader_has_box_flag;
	
	int max_user_parameter_number,max_user_order_number;

	inline Shader_part_information()
	{
		display_flag=true;
		shader_has_box_flag=true;

		max_user_parameter_number=2;
		max_user_order_number=5;

		Graphics_system::get_default_object_type_id(object_type_id);
	};
	inline ~Shader_part_information()
	{

	};
};

class Shader_base
{
	int program_index_id;
	Shader_base *next_shader,*class_shader;
public:
	CString class_name,directory_name,file_name;

	static void delete_all_program_object();
	static void set_general_shader_file_name_and_max_draw_instance_number(
					CString my_directory_file_name,
					CString my_configure_shader_file_name,
					CString my_uniform_shader_file_name,
					CString my_vertex_shader_file_name);
	static Shader_base *system_clone_shader(Part_parameter &par);

	Shader_base(CString my_class_name,CString my_directory_name,CString my_file_name);
	virtual ~Shader_base();

	static void set_shader_type(int my_shader_type_id);
	static bool is_same_shader(Shader_base *s,Shader_base*t);

	int get_program_index_id()
	{
		return program_index_id;
	};
	int get_program_id(Shader_program_variable_index **variable_index=NULL);
	CString *get_program_file_name();

public:

	virtual Component_private_data_storage *create_component_private_data_storage(Component *comp,ifstream &f){return NULL;};					//  只有需要创建组件私有数据时才需要实现 
	
	virtual void   create_graphics_system_object(int display_map,Component *comp,User_interface *interface_pointer,double precision_scale){};	//  只有需要生成纹理对象时才需要实现 
	virtual void   delete_graphics_system_object(){};

	virtual void   modify_material(){}																											//  只有需要修改材质信息时才需要实现 
	virtual void   set_reload_material(){};																										//  只有需要重新装入材质信息时才需要实现 
	
	virtual Shader_base *clone_shader(CString my_directory_name,CString my_file_name)=NULL;														//  由着色器实现 
	
	virtual void   set_material(Component_display_parameter *par,Component_display_flag *flag)=NULL;											//  由着色器实现
	virtual int    test_display(Component *comp,Component_display_parameter *par,Component_display_flag *flag,Component_display_list *display_list)=NULL;	//  由着色器实现
	
	virtual Shader_part_information *get_part_information()=NULL;																				//  由着色器实现 
};

class coodinator_management
{
public:
	static void reset();
	static void register_coodinator(double length,Location &location);
};


