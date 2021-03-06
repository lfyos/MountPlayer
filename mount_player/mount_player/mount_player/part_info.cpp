#include "StdAfx.h"
#include "part_info.h"
#include "const.h"
#include <math.h>
#include "part_face.h"
#include "part_body.h"
#include "part_image.h"
#include "graphics_system.h"

part_info::part_info(ifstream *f,CString information_file)
{
	int i;
	char tmp[MAX_MESSAGE_LENGTH];
	CString str;

	*f>>tmp>>tmp>>tmp>>tmp;
	part_file_name=tmp;

	material.load(f);

	*f>>tmp>>body_count;
	if(body_count<0){
		body_count=0;
		body=NULL;
	}else{
		body=new part_body*[body_count];
		for(i=0;i<body_count;i++){
			*f>>tmp>>tmp;
			body[i]=new part_body(f);
		}
	}
	caculate_box();
	set_part_information(information_file);
	return;
}

part_info::~part_info(void)
{
	int i;
	if(body_count<0)
		return;
	for(i=0;i<body_count;i++)
		delete (body[i]);
	delete []body;
	return;
}

void part_info::caculate_box(void)
{
	int i,j,k,n;

	for(i=0,n=0;i<body_count;i++){
		for(j=0;j<body[i]->face_count;j++){
			part_face *f=body[i]->face[j];
			for(k=0;k<9*(f->tesstrianglecount);k=k+3,n++){
				if(n==0){
					box[0]=f->tesstriangles[k+0];
					box[1]=f->tesstriangles[k+1];
					box[2]=f->tesstriangles[k+2];

					box[3]=f->tesstriangles[k+0];
					box[4]=f->tesstriangles[k+1];
					box[5]=f->tesstriangles[k+2];
				}else{
					box[0]=((box[0])>(f->tesstriangles[k+0]))?(f->tesstriangles[k+0]):(box[0]);
					box[1]=((box[1])>(f->tesstriangles[k+1]))?(f->tesstriangles[k+1]):(box[1]);
					box[2]=((box[2])>(f->tesstriangles[k+2]))?(f->tesstriangles[k+2]):(box[2]);

					box[3]=((box[3])<(f->tesstriangles[k+0]))?(f->tesstriangles[k+0]):(box[3]);
					box[4]=((box[4])<(f->tesstriangles[k+1]))?(f->tesstriangles[k+1]):(box[4]);
					box[5]=((box[5])<(f->tesstriangles[k+2]))?(f->tesstriangles[k+2]):(box[5]);
				}
			}
		}
	}

}
void part_info::set_part_information(CString information_file)
{
	int i;
	char tmp[MAX_MESSAGE_LENGTH];	
	CString id,my_id,name,length_width,thickness,weight,comment;	
	ifstream f(information_file);
	
	information.name="名称未知";
	information.length_width="长度宽度未知";
	information.thickness="厚度未知";
	information.weight="重量未知";
	information.comment="";	


	name=part_file_name;
	for(i=name.GetLength()-1;i>=0;i--){
		if((name.GetAt(i)=='\\')||(name.GetAt(i)=='/')){
			name=name.Right(name.GetLength()-i-1);
			break;
		}
	}
	for(i=name.GetLength()-1;i>=0;i--){
		if(name.GetAt(i)=='.'){
			name=name.Left(i);
			break;
		}
	}
	my_id=name;	

	for(;;){
		f>>tmp;id=tmp;
		f>>tmp;name=tmp;
		f>>tmp;length_width=tmp;
		f>>tmp;thickness=tmp;
		f>>tmp;weight=tmp;
		f>>tmp;comment=tmp;

		if(f.eof()||f.bad()||f.fail())				
			break;	
		if(id==my_id){
			information.name=name;
			information.length_width=length_width;
			information.thickness=thickness;
			information.weight=weight;
			information.comment=comment;
			break;
		}

	}
	f.close();
	return;
}
void part_info::display_transparently(int index_id,part_image *images[],graphics_system *gs,double transparence_ratio)
{
	int i,j;
	part_face *f;
	mount_material_property *color_material,*texture_material;

	transparence_ratio=1-transparence_ratio;

	for(i=0;i<body_count;i++){
		for(j=0;j<(body[i]->face_count);j++){
			f=body[i]->face[j];
			color_material=(f->material.material_flag)?(&(f->material)):(body[i]->material.material_flag)?(&(body[i]->material)):(&(material));
			gs->display_material(color_material,transparence_ratio);
			
			if(f->material.texture_image_id<0)				
				gs->execute_call_list(f->no_texture_call_list_id[index_id]);
			else{
				texture_material=(f->material.texture_flag)?(&(f->material)):(body[i]->material.texture_flag)?(&(body[i]->material)):(&material);
				gs->begin_texure_display(images[f->material.texture_image_id]->get_texture_object_id(index_id),texture_material,color_material,transparence_ratio);
				gs->execute_call_list(f->texture_call_list_id[index_id]);
				gs->end_texure_display();
			}
		}
	}
	return;
}
int part_info::create_call_list(int index_id,int start_call_list_id,part_image *images[],graphics_system *gs)
{
	int i,j;

	for(i=0;i<body_count;i++)
		for(j=0;j<(body[i]->face_count);j++)
			start_call_list_id=body[i]->face[j]->create_call_list(index_id,start_call_list_id,gs);

	gs->begin_create_call_list(start_call_list_id);	
	display_transparently(index_id,images,gs,1);
	gs->end_create_call_list();
	call_list_id[index_id]=start_call_list_id;
	start_call_list_id++;

	gs->begin_create_call_list(start_call_list_id);
	for(i=0;i<body_count;i++){
		for(j=0;j<(body[i]->face_count);j++)
			gs->execute_call_list(body[i]->face[j]->no_texture_call_list_id[index_id]);
	}
	gs->end_create_call_list();
	no_material_call_list_id[index_id]=start_call_list_id;
	start_call_list_id++;	

	gs->begin_create_call_list(start_call_list_id);
	gs->display_material(&material);
	gs->display_box(box);	
	gs->end_create_call_list();
	box_call_list_id[index_id]=start_call_list_id;
	start_call_list_id++;

	return start_call_list_id;
}
void part_info::change_to_absulate(double current_matrix[],double *x, double *y, double *z)
{
	int i,j,k;
	double a[4][4],last_x,last_y,last_z,last_w,w;

	for(i=0,k=0;i<4;i++)
		for(j=0;j<4;j++,k++)
			a[j][i]=current_matrix[k];
	
	last_x=*x;
	last_y=*y;
	last_z=*z;
	last_w=1.0;

	w =(a[3][0]*last_x+a[3][1]*last_y+a[3][2]*last_z+a[3][3]*last_w)/1;
	*z=(a[2][0]*last_x+a[2][1]*last_y+a[2][2]*last_z+a[2][3]*last_w)/w;
	*y=(a[1][0]*last_x+a[1][1]*last_y+a[1][2]*last_z+a[1][3]*last_w)/w;
	*x=(a[0][0]*last_x+a[0][1]*last_y+a[0][2]*last_z+a[0][3]*last_w)/w;

	return;
}
CString part_info::get_description(CString id_str)
{
	CString str;

	str ="名称:"+information.name+id_str;
	str+="长宽:"+information.length_width+id_str;
	str+="厚度:"+information.thickness+id_str;
	str+="重量:"+information.weight+id_str;
	str+=information.comment;

	return str;
}
int part_info::get_total_triangle_slice_number()
{
	int i,j,n;

	for(i=0,n=0;i<body_count;i++)
		for(j=0;j<body[i]->face_count;j++)
			n+=body[i]->face[j]->tesstrianglecount;
	return n;
}
BOOL part_info::get_body_face_triangle(int select_triangle_number,int *body_id, int *face_id, int *triangle_id, double *tesstriangles, double *tessnorms)
{
	int i,j,k;
	double *p1,*p2;

	for(i=0;i<body_count;i++)
		for(j=0;j<body[i]->face_count;j++)
			for(k=0;k<body[i]->face[j]->tesstrianglecount;k++,select_triangle_number--)
				if(select_triangle_number<=0){
					*body_id=i;
					*face_id=j;
					*triangle_id=k;
					
					p1=9*k+(body[i]->face[j]->tesstriangles);
					p2=9*k+(body[i]->face[j]->tessnorms);
					for(i=0;i<9;i++,p1++,p2++){
						tesstriangles[i]=(*p1);
						tessnorms[i]=(*p2);
					}
					return TRUE;
				}
	return FALSE;
}
void part_info::display(int index_id,graphics_system *gs)
{
	gs->execute_call_list(call_list_id[index_id]);
	return;
}
void part_info::display_box(int index_id,graphics_system *gs)
{
	gs->execute_call_list(box_call_list_id[index_id]);
	return;
}
void part_info::display_selected_part(int index_id,graphics_system *gs,mount_material_property *selected_material_property)
{
	gs->display_material(selected_material_property);
	gs->execute_call_list(no_material_call_list_id[index_id]);
	return;
}
void part_info::display_selecting_part(int index_id,int part_id,int part_number,int part_component_id,int part_component_number,graphics_system *gs)
{
	int n;
	double red,green,blue;
	n=3+(int)sqrtl((double)(part_component_number));
	red		=((double)part_id+(double)1.25)/(double)(part_number+2);
	green	=((double)(part_component_id%n)	+(double)1.25)/(double)(n+2);
	blue	=((double)(part_component_id/n)	+(double)1.25)/(double)(n+2);
	gs->display_color(red,green,blue);
	gs->execute_call_list(no_material_call_list_id[index_id]);
	return;
}
void part_info::display_selecting_face(graphics_system *gs)
{
	int i,j,k,id,r,g,b;
	part_face *f;

	for(i=0,id=2;i<body_count;i++){
		for(j=0;j<body[i]->face_count;j++){
			f=body[i]->face[j];
			for(k=0;k<f->tesstrianglecount;k++,id++){
				b=id;
				r=b%200;	b=b/200;
				g=b%200;	b=b/200;
				b=b%200;

				gs->display_color(((double)r+0.25)/200.0,((double)g+0.25)/200.0,((double)b+0.25)/200.0);

				gs->specify_face_point(FALSE,(f->tesstriangles)+9*k,	(f->tessnorms)+9*k,(f->tesstexture)+9*k);
				gs->display_face(1);
			}
		}
	}
	return;
}

void part_info::display_box_on_clip_plane(graphics_system *gs,double *clip_plane_data,
	double eye_x,double eye_y,double eye_z,double center_x,double center_y,double center_z)
{
	double current_matrix[16];

	gs->get_current_matrix(current_matrix);

	display_clip_plane(current_matrix,		box[0+0],box[1+0],box[2+0],		box[0+3],box[1+0],box[2+0],		box[0+3],box[1+0],box[2+3],		box[0+0],box[1+0],box[2+3],		gs,
			clip_plane_data,eye_x,eye_y,eye_z,center_x,center_y,center_z);
	display_clip_plane(current_matrix,		box[0+0],box[1+0],box[2+0],		box[0+0],box[1+3],box[2+0],		box[0+3],box[1+3],box[2+0],		box[0+3],box[1+0],box[2+0],		gs,
			clip_plane_data,eye_x,eye_y,eye_z,center_x,center_y,center_z);
	display_clip_plane(current_matrix,		box[0+0],box[1+0],box[2+0],		box[0+0],box[1+0],box[2+3],		box[0+0],box[1+3],box[2+3],		box[0+0],box[1+3],box[2+0],		gs,
			clip_plane_data,eye_x,eye_y,eye_z,center_x,center_y,center_z);
	display_clip_plane(current_matrix,		box[0+3],box[1+3],box[2+3],		box[0+3],box[1+3],box[2+0],		box[0+0],box[1+3],box[2+0],		box[0+0],box[1+3],box[2+3],		gs,
			clip_plane_data,eye_x,eye_y,eye_z,center_x,center_y,center_z);
	display_clip_plane(current_matrix,		box[0+3],box[1+3],box[2+3],		box[0+0],box[1+3],box[2+3],		box[0+0],box[1+0],box[2+3],		box[0+3],box[1+0],box[2+3],		gs,
			clip_plane_data,eye_x,eye_y,eye_z,center_x,center_y,center_z);
	display_clip_plane(current_matrix,		box[0+3],box[1+3],box[2+3],		box[0+3],box[1+0],box[2+3],		box[0+3],box[1+0],box[2+0],		box[0+3],box[1+3],box[2+0],		gs,
			clip_plane_data,eye_x,eye_y,eye_z,center_x,center_y,center_z);
	return;
}

void part_info::display_clip_plane(double current_matrix[],
	double x0, double y0, double z0, 	double x1, double y1, double z1, 
	double x2, double y2, double z2, 	double x3, double y3, double z3,
	graphics_system *gs,
	double *clip_plane_data,
	double eye_x,double eye_y,double eye_z,
	double center_x,double center_y,double center_z)
{
	double t,t1;
	double A,B,C,D;

	change_to_absulate(current_matrix,&x0,&y0,&z0);
	change_to_absulate(current_matrix,&x1,&y1,&z1);
	change_to_absulate(current_matrix,&x2,&y2,&z2);
	change_to_absulate(current_matrix,&x3,&y3,&z3);

	A=clip_plane_data[0];
	B=clip_plane_data[1];
	C=clip_plane_data[2];
	D=clip_plane_data[3];
	
	t=A*(center_x-eye_x)+B*(center_y-eye_y)+C*(center_z-eye_z);
	if(fabs(t)<MIN_VALUE)
		return;

	t1=(A*x0+B*y0+C*z0+D)/t;
	x0+=(-t1)*(center_x-eye_x);
	y0+=(-t1)*(center_y-eye_y);
	z0+=(-t1)*(center_z-eye_z);

	t1=(A*x1+B*y1+C*z1+D)/t;
	x1+=(-t1)*(center_x-eye_x);
	y1+=(-t1)*(center_y-eye_y);
	z1+=(-t1)*(center_z-eye_z);


	t1=(A*x2+B*y2+C*z2+D)/t;
	x2+=(-t1)*(center_x-eye_x);
	y2+=(-t1)*(center_y-eye_y);
	z2+=(-t1)*(center_z-eye_z);

	t1=(A*x3+B*y3+C*z3+D)/t;
	x3+=(-t1)*(center_x-eye_x);
	y3+=(-t1)*(center_y-eye_y);
	z3+=(-t1)*(center_z-eye_z);

	gs->push_matrix();
	gs->load_identity_matrix();
	t=sqrtl(A*A+B*B+C*C);
	gs->display_quadrangle(A/t,B/t,C/t,x0,y0,z0,x1,y1,z1,x2,y2,z2,x3,y3,z3);
	gs->pop_matrix();

	return;
}
