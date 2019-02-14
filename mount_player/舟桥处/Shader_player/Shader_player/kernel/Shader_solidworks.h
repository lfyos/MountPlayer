#pragma once
#include "shader_base.h"
#include "Texture.h"

#define MAX_SOLIDWORKS_MATERIAL_NUMBER 32

class Shader_solidworks :public Shader_base
{
public:
	float parameter[16*MAX_SOLIDWORKS_MATERIAL_NUMBER];

	int material_number;
	bool prepare_flag;
	struct Solidworks_material_item{
		CString file_name;
		float red,green,blue,ambient,diffuse,specular,shininess,transparency,emission;		
		float angle,cos_v,sin_v,scale;
		bool blend_flag;
		Texture  tex;
		int texture_object_id;
	}material[MAX_SOLIDWORKS_MATERIAL_NUMBER];

	Shader_part_information info;

	void prepare(Component_display_parameter *par,Component_display_flag *flag);

	void save_material();

	Shader_solidworks(CString my_class_name,CString my_directory_name=_T(""),CString my_file_name=_T(""));
	virtual ~Shader_solidworks(void);

	virtual Shader_base *clone_shader(CString my_directory_name,CString my_file_name)
	{
		return new Shader_solidworks(class_name,my_directory_name,my_file_name);
	}
	virtual void   set_reload_material();
	virtual void modify_material();
	virtual void set_material(Component_display_parameter *par,Component_display_flag *flag);
	virtual int test_display(Component *comp,Component_display_parameter *par,Component_display_flag *flag,Component_display_list *display_list);
	virtual Shader_part_information *get_part_information();
};
