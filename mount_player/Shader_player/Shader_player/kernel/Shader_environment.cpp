#include "StdAfx.h"
#include "Shader_environment.h"

#include "GL/glew.h"
#include "Whole_system.h"

static Shader_environment a(_T("»·¾³äÖÈ¾Æ÷"));

Shader_environment::Shader_environment(CString my_class_name,CString my_directory_name,CString my_file_name)
	:Shader_base(my_class_name,my_directory_name,my_file_name)
{
	load_flag=true;
	for(int i=0;i<sizeof(texure_object_id)/sizeof(texure_object_id[0]);i++)
		texure_object_id[i]=0;
	
}

Shader_environment::~Shader_environment(void)
{
	set_reload_material();
}

void   Shader_environment::set_reload_material()
{
	load_flag=true;
	for(int i=0;i<sizeof(texure_object_id)/sizeof(texure_object_id[0]);i++)
		if(texure_object_id[i]!=0){
			::glDeleteTextures(1,&(texure_object_id[i]));
			texure_object_id[i]=0;
		}
}

void Shader_environment::set_material(Component_display_parameter *par,Component_display_flag *flag)
{
	int i,width,height;
	float *data_pointer;

	if(load_flag){
		load_flag=false;
		ifstream f(directory_name+file_name);
		char buffer[MAX_BUFFER_LENGTH];

		::glGenTextures(sizeof(texure_object_id)/sizeof(texure_object_id[0]),texure_object_id);
		for(i=0;i<sizeof(texure_object_id)/sizeof(texure_object_id[0]);i++){
			::glActiveTexture(i+GL_TEXTURE1);
			::glBindTexture(GL_TEXTURE_2D,texure_object_id[i]);
			f>>buffer>>buffer;
			data_pointer=picture[i].load_from_file(directory_name+CString(buffer),&width,&height);
			::glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_FLOAT,data_pointer);

			::glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			::glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			::glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
			::glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
		}
		f.close();
	}
	const char *text_object_name[6]={"positive_x","negative_x","negative_z","positive_z","positive_y","negative_y"};
	for(i=0;i<sizeof(texure_object_id)/sizeof(texure_object_id[0]);i++){
		::glActiveTexture(i+GL_TEXTURE1);
		::glBindTexture(GL_TEXTURE_2D,texure_object_id[i]);
		if(picture[i].video_process()){
			data_pointer=picture[i].get_image_data(&width,&height);
			::glTexSubImage2D(GL_TEXTURE_2D,0,0,0,width,height,GL_RGBA,GL_FLOAT,data_pointer);
		}
		::glEnable(GL_TEXTURE_2D);
		::glUniform1i(::glGetUniformLocation(get_program_id(),text_object_name[i]),i+1);
	}

	GLint viewport[4];
	::glGetIntegerv(GL_VIEWPORT, viewport);	
	double p=::abs((double)(viewport[2])/double(viewport[3]));
	float a[32];
	
	double far_distance=((par->camera->far_point)-(par->camera->eye_point)).distance()*0.99;
	Point center_point=par->camera->eye_point+(par->camera->far_point-par->camera->eye_point).expand(far_distance);

	double view_height=par->camera->half_fovy_tanl;
	if(par->camera->project_matrix_flag)
		view_height*=far_distance;
	else
		view_height*=par->camera->distance;
	double view_width=p*view_height;

	Point y_direction=(par->camera->up_point)-(par->camera->center_point);
	y_direction=y_direction.expand(view_height);

	Point x_direction=(par->camera->center_point)-(par->camera->eye_point);
	x_direction=x_direction.cross((par->camera->up_point)-(par->camera->center_point));
	x_direction=x_direction.expand(view_width);

	Location(center_point,x_direction,y_direction,par->camera->eye_point).get_location_float_data(&(a[ 0]));

	if(!(par->camera->project_matrix_flag))
		center_point=par->camera->eye_point+(par->camera->far_point-par->camera->eye_point).expand(par->camera->distance);
	Location(center_point,x_direction,y_direction,par->camera->eye_point).get_location_float_data(&(a[16]));

	Graphics_system::set_user_parameter(7,a,0);
}

int Shader_environment::test_display(Component *comp,Component_display_parameter *par,Component_display_flag *flag,Component_display_list *display_list)
{
	if((flag->paint_method_id!=0)&&(flag->paint_method_id!=1))
		return -1;
	display_list->set_only_display_body();
	return 0;
}

Shader_part_information *Shader_environment::get_part_information()
{
	info.display_flag=false;
	info.shader_has_box_flag=false;
	info.max_user_parameter_number=7;

	return &info;
}