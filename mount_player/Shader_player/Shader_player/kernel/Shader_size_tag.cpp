#include "StdAfx.h"
#include "Shader_size_tag.h"
#include "GL\glew.h"
#include "User_interface.h"
#include "..\Shader_player.h"
#include "Texture.h"

static Shader_size_tag a(_T("标注线条着色器")),b(_T("标注文字着色器"));
static Size_tag tag[10];


void Shader_size_tag::delete_tag_object(Select_component_ids &ids,Part_set &parts)
{
	ids.compute_pointer(&parts);

	if(ids.part_shader==NULL)
		return;
	if((ids.part_shader->class_name)!=(b.class_name))
		return;
	if(ids.face==NULL)
		return;
	if((ids.face_id<0)||(ids.face_id>=(sizeof(tag)/sizeof(tag[0]))))
		return;
	tag[ids.face_id].reset();
	return;
}
void Shader_size_tag::add_tag_object(Select_component_ids &ids,Part_set &parts,int my_tag_direction_flag)
{
	ids.compute_pointer(&parts);

	for(;;){
		if(ids.part_shader==NULL)
			break;
		if((ids.part_shader->class_name)!=(b.class_name))
			break;
		if(ids.face==NULL)
			break;
		if((ids.face_id<0)||(ids.face_id>=(sizeof(tag)/sizeof(tag[0]))))
			break;
		if(tag[ids.face_id].modify_tag_screen_location(my_tag_direction_flag))
			return;
		break;
	}
	int state_id=-1,index_id=-1;
	for(int i=0;i<(sizeof(tag)/sizeof(tag[0]));i++)
		if(!(tag[i].finish_flag))
			if(tag[i].get_state_id()>state_id){
				state_id=tag[i].get_state_id();
				index_id=i;
			}
	if(index_id>=0)
		tag[index_id].add_tag_point(ids,parts,my_tag_direction_flag);
}

void Shader_size_tag::clear_all_tag_object()
{
	for(int i=0;i<(sizeof(tag)/sizeof(tag[0]));i++)
		tag[i].reset();
}
Shader_size_tag::Shader_size_tag(CString my_class_name,CString my_directory_name,CString my_file_name)
		:Shader_base(my_class_name,my_directory_name,my_file_name)
{
	if(my_class_name==_T("标注线条着色器"))
		info.object_type_id[0]=GL_LINES;
	else
		info.object_type_id[0]=GL_TRIANGLES;

	info.display_flag=false;

	font_width=1.0;
	font_height=1.0;
	font_texture_object_id=0;
}
Shader_size_tag::~Shader_size_tag(void)
{
	set_reload_material();
}

int Shader_size_tag::test_display(Component *comp,Component_display_parameter *par,Component_display_flag *flag,Component_display_list *display_list)
{
	switch(flag->paint_method_id){
	case 4:
	case 3:
	case 2:
		if(info.object_type_id[0]==GL_LINES)
			return -1;
	case 1:
	case 0:
		display_list->set_only_display_body();
		return 0;
	}
	return -1;
	return 0;
}
Shader_part_information *Shader_size_tag::get_part_information()
{
	info.shader_has_box_flag=false;

	info.max_user_parameter_number=2*16*(sizeof(tag)/sizeof(tag[0]))/4;

	return &info;
}
void  Shader_size_tag::set_reload_material()
{
	GLuint id=font_texture_object_id;
	::glDeleteTextures(1,&id);	
	font_texture_object_id=0;
}
void Shader_size_tag::set_material(Component_display_parameter *par,Component_display_flag *flag)
{
	GLuint id;

	if((id=(GLuint)font_texture_object_id)==0){		
		char buffer[MAX_BUFFER_LENGTH];
		int width,height;
		Texture picture;
		
		ifstream f(directory_name+file_name);
		f>>buffer>>font_width;
		f>>buffer>>font_height;
		f>>buffer>>buffer;
		f.close();

		::glGenTextures(1,&id);
		font_texture_object_id=(int)id;

		::glActiveTexture(GL_TEXTURE1);
		::glBindTexture(GL_TEXTURE_2D,id);
		
		float *data_pointer=picture.load_from_file(directory_name+CString(buffer),&width,&height);
		::gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,width,height,GL_RGBA,GL_FLOAT,data_pointer);
		::glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		::glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		::glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		::glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	}else{
		::glActiveTexture(GL_TEXTURE1);
		::glBindTexture(GL_TEXTURE_2D,id);
	}
	::glUniform1i(::glGetUniformLocation(get_program_id(),"font_image"),1);
	
	float tag_location[2*16*(sizeof(tag)/sizeof(tag[0]))];
	float tag_bitmap[16*(sizeof(tag)/sizeof(tag[0]))];

	for(int i=0;i<(sizeof(tag_location)/sizeof(tag_location[0]));i+=4)
		par->camera->eye_point>>(&(tag_location[i]));

	for(int i=0;i<(sizeof(tag_bitmap)/sizeof(tag_bitmap[0]));i++)
		tag_bitmap[i]=(-10);

	for(int i=0;i<(sizeof(tag)/sizeof(tag[0]));i++)
		if(tag[i].get_tag_point(theApp.mouse_move_point.x,theApp.mouse_move_point.y,par->camera->eye_point,par->camera->project_matrix)){

			if(info.object_type_id[0]==GL_LINES){
				tag[i].tag_point[1] >>(&(tag_location[ 0+32*i]));
				tag[i].tag_point[2] >>(&(tag_location[ 4+32*i]));
				tag[i].tag_point[2] >>(&(tag_location[ 8+32*i]));
				tag[i].tag_point[3] >>(&(tag_location[12+32*i]));
				tag[i].tag_point[3] >>(&(tag_location[16+32*i]));
				tag[i].tag_point[0] >>(&(tag_location[20+32*i]));
				for(int j=0;(j<16);j++)
					tag_bitmap[16*i+j]=(float)(15000.0);
			}else{
				Point up_direction=(par->camera->up_point)-(par->camera->center_point);
				Point right_direction=up_direction.cross((par->camera->eye_point)-(par->camera->center_point));
	
				Point middle_point=(tag[i].tag_point[2]+tag[i].tag_point[3])*0.5;
				double distance=(par->camera->eye_point-middle_point).distance()*(par->camera->half_fovy_tanl);
				Point x_direction =(tag[i].tag_point[1]-tag[i].tag_point[0]).expand(font_width*distance*8.0);
				if(x_direction.dot(right_direction)<0)
					x_direction=(-x_direction);

				Point y_direction =(tag[i].tag_point[3]-tag[i].tag_point[0]).expand(font_height*distance);
				if(y_direction.dot(up_direction)<0)
					y_direction=(-y_direction);
				Point p0=middle_point-x_direction,p1=middle_point+x_direction;
				Point p2=p1+y_direction,p3=p0+y_direction;
				p1>>(&(tag_location[ 0+32*i]));
				p2>>(&(tag_location[ 4+32*i]));
				p0>>(&(tag_location[ 8+32*i]));
				p2>>(&(tag_location[12+32*i]));
				p3>>(&(tag_location[16+32*i]));
				p0>>(&(tag_location[20+32*i]));

				tag[i].get_bitmap(par->camera);
				for(int j=0;(j<16)&&(j<(sizeof(tag[i].bitmap)/sizeof(tag[i].bitmap[0])));j++)
					if(tag[i].bitmap[j]>=0)
						tag_bitmap[16*i+j]=(float)(tag[i].bitmap[j])+(float)0.25;
					else
						tag_bitmap[16*i+j]=(float)(-10.0);

			}
		}
	Graphics_system::set_user_parameter(8*sizeof(tag)/sizeof(tag[0]),tag_location,0);

	::glUniformMatrix4fv(::glGetUniformLocation(get_program_id(),"tag_bitmap"),sizeof(tag_bitmap)/sizeof(tag_bitmap[0])/16,GL_FALSE,tag_bitmap);
}