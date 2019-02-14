#include "StdAfx.h"
#include "Shader_coordinate.h"
#include "GL/glew.h"
#include "Whole_system.h"

static float coordinate_length[100];
static float loca_data[16*sizeof(coordinate_length)/sizeof(coordinate_length[0])];
static int coordinate_number=1;

class coordinate_data_storage:public Component_private_data_storage
{
public:
	coordinate_data_storage(Component *comp):Component_private_data_storage(comp)
	{
	}
	virtual int get_order_parameter(int matrix_id,Location *loca,int &instance_number,float *&order_parameter_data)
	{
		Component_private_data_storage::get_order_parameter(matrix_id,loca,instance_number,order_parameter_data);
		instance_number=coordinate_number;
		return 4;
	}
};

void Coordinate_management::reset()
{
	coordinate_number=1;
}

void Coordinate_management::register_coordinate(Location loca,double length)
{
	if(coordinate_number<(sizeof(coordinate_length)/sizeof(coordinate_length[0]))){
		loca.get_location_float_data(&(loca_data[16*coordinate_number]));
		coordinate_length[coordinate_number]=(float)length;
		coordinate_number++;
	}
}

static Shader_coordinate a(_T("×ø±êÖáäÖÈ¾Æ÷"));
static bool display_flag=false;

void Shader_coordinate::set_display_flag(bool new_display_flag)
{
	display_flag=new_display_flag;
}

bool Shader_coordinate::get_display_flag()
{
	return display_flag;
}

int Shader_coordinate::test_display(Component *comp,Component_display_parameter *par,Component_display_flag *flag,Component_display_list *display_list)
{
	if(!display_flag)
		if(flag->paint_method_id==0)
			if(par->camera->eye_component==comp){
				Component *p=comp;
				while(p->parent!=NULL)
					p=p->parent;
				coordinate_length[0]=(float)(p->component_box.distance());
				comp->absulate_location.get_location_float_data(loca_data);
				display_list->set_only_display_body();
				return 0;
			}
	return -1;
}

void Shader_coordinate::set_material(Component_display_parameter *par,Component_display_flag *flag)
{	
	::glUniformMatrix4fv(::glGetUniformLocation(get_program_id(),"place"),	coordinate_number,	GL_FALSE,	loca_data);
	::glUniform1fv      (::glGetUniformLocation(get_program_id(),"length"),	coordinate_number,				coordinate_length);
}

Shader_part_information *Shader_coordinate::get_part_information()
{
	info.object_type_id[0]=GL_LINE_STRIP;
	info.display_flag=false;
	info.shader_has_box_flag=false;

	return &info;
}

Component_private_data_storage *Shader_coordinate::create_component_private_data_storage(Component *comp,ifstream &f)
{
	return new coordinate_data_storage(comp);
}