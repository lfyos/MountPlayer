#include "StdAfx.h"
#include "Shader_select_area.h"
#include "GL/glew.h"
#include "Camera.h"
#include "../Shader_player.h"

static Shader_select_area a(_T("ÇøÓòÑ¡ÔñäÖÈ¾Æ÷"));

void Shader_select_area::set_material(Component_display_parameter *par,Component_display_flag *flag)
{
	float parameter[16];
	double screen_z=0.5;

	(Graphics_system::screen_to_object(Point(theApp.mouse_left_key_down_point.x,	theApp.mouse_left_key_down_point.y,screen_z),par->camera->project_matrix))>>(&(parameter[0]));
	(Graphics_system::screen_to_object(Point(theApp.mouse_move_point.x,		theApp.mouse_left_key_down_point.y,screen_z),par->camera->project_matrix))>>(&(parameter[4]));
	(Graphics_system::screen_to_object(Point(theApp.mouse_move_point.x,		theApp.mouse_move_point.y,screen_z),par->camera->project_matrix))	>>(&(parameter[8]));
	(Graphics_system::screen_to_object(Point(theApp.mouse_left_key_down_point.x,	theApp.mouse_move_point.y,screen_z),par->camera->project_matrix))	>>(&(parameter[12]));

	Graphics_system::set_user_parameter(4,parameter,0);
}
int  Shader_select_area::test_display(Component *comp,Component_display_parameter *par,Component_display_flag *flag,Component_display_list *display_list)
{
	if((flag->paint_method_id!=0)&&(flag->paint_method_id!=1))
		return -1;
	if(!theApp.mouse_left_key_down_flag)
		return -1;

	if(theApp.mouse_move_id!=21)
		if(theApp.mouse_move_id!=22)
			return -1;
	if(Platform_information::get_function_key_state()>=4)
		return -1;

	display_list->set_only_display_body();
	
	return 0;
}
Shader_part_information *Shader_select_area::get_part_information()
{
	info.object_type_id[0]=GL_LINE_STRIP;
	info.display_flag=false;
	info.shader_has_box_flag=false;

	info.max_user_parameter_number=4;

	return &info;
}