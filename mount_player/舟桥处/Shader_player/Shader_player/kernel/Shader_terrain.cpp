#include "StdAfx.h"
#include "Shader_terrain.h"

#include "GL/glew.h"
#include "User_interface.h"

static Shader_terrain a(_T("µÿ–Œ‰÷»æ∆˜"));

Shader_terrain::Shader_terrain(CString my_class_name,CString my_directory_name,CString my_file_name)
	:Shader_base(my_class_name,my_directory_name,my_file_name)
{
	for(int i=0;i<sizeof(terrain.texure_object_id)/sizeof(terrain.texure_object_id[0]);i++)
		terrain.texure_object_id[i]=0;
	need_reload_parameter=true;
}

Shader_terrain::~Shader_terrain(void)
{
	set_reload_material();
}

void  Shader_terrain::set_reload_material()
{
	for(int i=0;i<sizeof(terrain.texure_object_id)/sizeof(terrain.texure_object_id[0]);i++)
		if(terrain.texure_object_id[i]!=0){
			::glDeleteTextures(1,&(terrain.texure_object_id[i]));
			terrain.texure_object_id[i]=0;
		}
	need_reload_parameter=true;
}

static Location mirror_projection_matrix;
static int frame_buffer_id=0,render_buffer_id=0,texture_id=0;
static bool render_flag=true;

void Shader_terrain::set_material(Component_display_parameter *par,Component_display_flag *flag)
{
	if(need_reload_parameter){
		need_reload_parameter=false;
		
		ifstream f(directory_name+file_name);
		char buffer[MAX_BUFFER_LENGTH];
		
		f>>buffer>>(terrain.x0)>>(terrain.y0)>>(terrain.z0);
		f>>buffer>>(terrain.length)>>(terrain.width)>>(terrain.height)>>(terrain.depth);
		f>>buffer>>(terrain.texture_scale);
		f>>buffer>>(terrain.lod);


		::glGenTextures(sizeof(terrain.texure_object_id)/sizeof(terrain.texure_object_id[0]),terrain.texure_object_id);
		for(int i=0;i<sizeof(terrain.texure_object_id)/sizeof(terrain.texure_object_id[0]);i++){
			Texture picture;
			float *data_pointer;
			int width,height;

			::glBindTexture(GL_TEXTURE_2D,terrain.texure_object_id[i]);
			f>>buffer>>buffer;
			data_pointer=picture.load_from_file(directory_name+CString(buffer),&width,&height);
			::gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,width,height,GL_RGBA,GL_FLOAT,data_pointer);
			::glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
			::glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
			::glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
			::glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
		}
		f>>buffer>>(water.amplitude);
		f>>buffer>>(water.height);
		f>>buffer>>(water.wave_length);
		f>>buffer>>(water.cycle);
		f>>buffer>>(water.a)>>(water.b);
		f>>buffer>>(water.alf);
		f>>buffer>>(water.mirror_texture_precision);
		double d=sqrtl((water.a)*(water.a)+(water.b)*(water.b));
		water.a/=d;
		water.b/=d;
		f.close();
	}
	const char *text_object_name[]={"height","water","surface","mountain","bottom"};
	for(int i=0;(i<sizeof(text_object_name)/sizeof(text_object_name[0]))&&(i<sizeof(terrain.texure_object_id)/sizeof(terrain.texure_object_id[0]));i++){
		::glActiveTexture(i+GL_TEXTURE2);
		::glBindTexture(GL_TEXTURE_2D,terrain.texure_object_id[i]);
		::glEnable(GL_TEXTURE_2D);
		::glUniform1i(::glGetUniformLocation(get_program_id(),text_object_name[i]),i+2);
	}

	float data[16]={
		(float)(terrain.x0),(float)(terrain.y0),(float)(terrain.z0),1.0,
		(float)(par->camera->center_point.x),(float)(par->camera->center_point.y),(float)(par->camera->center_point.z),1.0,
		(float)(water.amplitude),	(float)(water.height),	(float)(water.wave_length),	(float)(water.cycle),
		(float)(water.a),		(float)(water.b),	(float)(water.alf),		1.0,

	};
	double h=::abs(par->camera->distance)+::abs(par->camera->center_point.y);
	double far_value=((par->camera->far_point)-(par->camera->eye_point)).distance();
	
	float parameter[16]={
		(float)1.0,				(float)(Platform_information::get_current_time()),(float)(terrain.texture_scale),	1.0,
		(float)(terrain.length),(float)(terrain.width),							(float)(terrain.height),		(float)(terrain.depth),
		(float)h,				(float)::atanl(far_value/h),					(float)(terrain.lod),			(float)((water.mirror_texture_precision>0.0)?1.0:-1.0),
		0,						1,												0,								(float)(water.height),
	};

	Graphics_system::set_user_parameter(4,data,0);
	Graphics_system::set_user_parameter(4,parameter,4);
	
	::glActiveTexture(GL_TEXTURE1);
	::glBindTexture(GL_TEXTURE_2D,0);
	::glDisable(GL_TEXTURE_2D);

	if(water.mirror_texture_precision>0){
		mirror_projection_matrix.get_location_float_data(parameter);
		Graphics_system::set_user_parameter(4,parameter,8);
		::glBindTexture(GL_TEXTURE_2D,texture_id);
		::glEnable(GL_TEXTURE_2D);
	}
	::glUniform1i(::glGetUniformLocation(get_program_id(),"surface_texture"),1);
}

void Shader_terrain::create_graphics_system_object(int display_map,Component *comp,User_interface *interface_pointer,double precision_scale)
{
	if(need_reload_parameter)
		return;
	if(!(interface_pointer->camera_pointer()->project_matrix_flag))
		return;

	render_flag=false;

	double texture_width=(water.mirror_texture_precision)*(interface_pointer->view_width),texture_height=(water.mirror_texture_precision)*(interface_pointer->view_height);
	Graphics_system::create_mirror_texture_and_frame_buffer(&frame_buffer_id,&render_buffer_id,&texture_id,(int)texture_width,(int)texture_height);

	double mirror_plane[]={0,-1,0,water.height};
	interface_pointer->change_camera(interface_pointer->camera_id,texture_width,texture_height,mirror_plane);
	mirror_projection_matrix=interface_pointer->camera_pointer()->project_matrix;

	Component_collector_for_display a(	precision_scale,false,interface_pointer->ws->minnimal_fovy_tanl,
										Component_display_parameter(interface_pointer->ws->parts,interface_pointer->camera_pointer(),interface_pointer->ws->lights,&(interface_pointer->clipper),NULL,NULL), 
										Component_display_flag(0,display_map,interface_pointer->ws->selected_line_width,interface_pointer->ws->selected_point_size),
										NULL,NULL);

	Graphics_system::clear_screen(interface_pointer->ws->background_color);
	a.collect(interface_pointer->ws->root_component);
	a.display_components(interface_pointer->ws->offset_factor,interface_pointer->ws->offset_units,interface_pointer->ws->line_stipple_factor,interface_pointer->ws->line_stipple_pattern,true);

	Graphics_system::get_mirror_texture_image(texture_id,(int)texture_width,(int)texture_height);

	render_flag=true;
}

void Shader_terrain::delete_graphics_system_object()
{
	Graphics_system::delete_framebuffer_renderbuffer_and_texture(&frame_buffer_id,&render_buffer_id,&texture_id);
}

int Shader_terrain::test_display(Component *comp,Component_display_parameter *par,Component_display_flag *flag,Component_display_list *display_list)
{
	if((flag->paint_method_id!=0)&&(flag->paint_method_id!=1))
		return -1;
	if(!(par->camera->project_matrix_flag))
		return -1;

	display_list->set_only_display_body();
	display_list->transpancy_display=render_flag;

	return 1;
}

Shader_part_information *Shader_terrain::get_part_information()
{
	info.display_flag=false;
	info.shader_has_box_flag=false;
	info.max_user_parameter_number=12;

	return &info;
}
