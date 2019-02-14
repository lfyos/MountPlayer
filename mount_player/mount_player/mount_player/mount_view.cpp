#include "StdAfx.h"
#include "mount_view.h"
#include <math.h>
#include "const.h"

#include <fstream>
using namespace std;

#include "movement_base.h"
#include "mount_bridge.h"
#include "part_tree.h"
#include "part_info.h"
#include "part_image.h"


mount_view::mount_view(CString directory_name,CString structure_file_name,CString information_file_name,CString light_file_name,CString sort_name,double min_value)
{
	bridge=new mount_bridge(directory_name,structure_file_name,information_file_name,sort_name,min_value);

	view_data_bak_number=0;
	view_data_bak_index=0;

	view_modifier_flag=FALSE;
	clip_plane_flag=FALSE;
	defined_area.flag=FALSE;
	display_reference_frame_flag=TRUE;

	speed=1.0;

	set_equal_view();

	load_light(directory_name+light_file_name);

	return;
}

mount_view::~mount_view(void)
{
	delete bridge;
}
void mount_view::set_equal_view()
{
	double distance,xyz;
	
	xyz=bridge->tree->box[0]-bridge->tree->box[3];			distance =xyz*xyz;
	xyz=bridge->tree->box[1]-bridge->tree->box[4];			distance+=xyz*xyz;
	xyz=bridge->tree->box[2]-bridge->tree->box[5];			distance+=xyz*xyz;
	distance=sqrtl(distance);

	xyz=distance*sqrtl(3)/3;
	eye_x=xyz;
	eye_y=xyz;
	eye_z=xyz;

	center_x=0;
	center_y=0;
	center_z=0;

	up_x=0;
	up_y=distance*sqrtl(2);
	up_z=0;

	return;
}
void mount_view::save_view_data()
{
	view_data_bak[view_data_bak_index].center_x=center_x;
	view_data_bak[view_data_bak_index].center_y=center_y;
	view_data_bak[view_data_bak_index].center_z=center_z;

	view_data_bak[view_data_bak_index].eye_x=eye_x;
	view_data_bak[view_data_bak_index].eye_y=eye_y;
	view_data_bak[view_data_bak_index].eye_z=eye_z;

	view_data_bak[view_data_bak_index].up_x=up_x;
	view_data_bak[view_data_bak_index].up_y=up_y;
	view_data_bak[view_data_bak_index].up_z=up_z;

	view_data_bak_index=(1+view_data_bak_index)%MAX_VIEW_DATA;
	view_data_bak_number++;
	if(view_data_bak_number>MAX_VIEW_DATA)
		view_data_bak_number=MAX_VIEW_DATA;

	return;
}
void mount_view::restore_view_data()
{
	if(view_data_bak_number<=1)
		return;
	view_data_bak_number--;
	view_data_bak_index=(view_data_bak_index+MAX_VIEW_DATA-1)%MAX_VIEW_DATA;

	center_x=view_data_bak[view_data_bak_index].center_x;
	center_y=view_data_bak[view_data_bak_index].center_y;
	center_z=view_data_bak[view_data_bak_index].center_z;

	eye_x=view_data_bak[view_data_bak_index].eye_x;
	eye_y=view_data_bak[view_data_bak_index].eye_y;
	eye_z=view_data_bak[view_data_bak_index].eye_z;

	up_x=view_data_bak[view_data_bak_index].up_x;
	up_y=view_data_bak[view_data_bak_index].up_y;
	up_z=view_data_bak[view_data_bak_index].up_z;

	return;
}
void mount_view::adjust_view()
{
	double A,B,C,D,t,d,dx,dy,dz;
	double tx_min,tx_max,ty_min,ty_max,tz_min,tz_max;
	BOOL x_flag,y_flag,z_flag;

	dx=eye_x-center_x;
	dy=eye_y-center_y;
	dz=eye_z-center_z;

	d=sqrtl(dx*dx+dy*dy+dz*dz);
	dx=dx/d;
	dy=dy/d;
	dz=dz/d;
	
	x_flag=FALSE;
	y_flag=FALSE;
	z_flag=FALSE;

	if(fabs(dx)>MIN_VALUE){
		tx_min=(bridge->tree->box[0]-center_x)/dx;
		tx_max=(bridge->tree->box[3]-center_x)/dx;
		if(tx_min>tx_max){
			t=tx_min;
			tx_min=tx_max;
			tx_max=t;
		}
		x_flag=TRUE;
	}

	if(fabs(dy)>MIN_VALUE){
		ty_min=(bridge->tree->box[1]-center_y)/dy;
		ty_max=(bridge->tree->box[4]-center_y)/dy;
		if(ty_min>ty_max){
			t=ty_min;
			ty_min=ty_max;
			ty_max=t;
		}
		y_flag=TRUE;
	}

	if(fabs(dz)>MIN_VALUE){
		tz_min=(bridge->tree->box[2]-center_z)/dz;
		tz_max=(bridge->tree->box[5]-center_z)/dz;
		if(tz_min>tz_max){
			t=tz_min;
			tz_min=tz_max;
			tz_max=t;
		}
		z_flag=TRUE;
	}

	if(x_flag){
		if(y_flag){
			if((tx_max<ty_min)||(tx_min>ty_max))
				y_flag=FALSE;
			else{
				ty_min=(tx_min>ty_min)?tx_min:ty_min;
				ty_max=(tx_max<ty_max)?tx_max:ty_max;
			}
		}else{			
			ty_min=tx_min;
			ty_max=tx_max;
			y_flag=TRUE;
		}
	}
	if(z_flag){
		if(y_flag){
			if((tz_max<ty_min)||(tz_min>ty_max))
				y_flag=FALSE;
			else{
				ty_min=(tz_min>ty_min)?tz_min:ty_min;
				ty_max=(tz_max<ty_max)?tz_max:ty_max;
			}
		}else{			
			ty_min=tz_min;
			ty_max=tz_max;
			y_flag=TRUE;
		}
	}

	if(y_flag){
		if(ty_max<0)
			t=ty_max;
		else if(ty_min>0)
			t=ty_min;
		else
			t=0;
		if(fabs(t)>MIN_VALUE){

			dx*=t;
			dy*=t;
			dz*=t;

			center_x+=dx;
			center_y+=dy;
			center_z+=dz;

			eye_x+=dx;
			eye_y+=dy;
			eye_z+=dz;

			up_x+=dx;
			up_y+=dy;
			up_z+=dz;
		}
	}
	A=center_x-eye_x;
	B=center_y-eye_y;
	C=center_z-eye_z;
	D=(A*eye_x+B*eye_y+C*eye_z);
		
	t=(A*up_x+B*up_y+C*up_z-D)/(A*A+B*B+C*C);

	up_x=up_x-A*t;
	up_y=up_y-B*t;
	up_z=up_z-C*t;

	dx=up_x-eye_x;
	dy=up_y-eye_y;
	dz=up_z-eye_z;

	d=sqrtl(dx*dx+dy*dy+dz*dz);

	up_x=eye_x+(up_x-eye_x)/d;
	up_y=eye_y+(up_y-eye_y)/d;
	up_z=eye_z+(up_z-eye_z)/d;

	return;
}
double mount_view::get_view_depth()
{
	double depth=0,d;

	d=bridge->tree->box[3]-bridge->tree->box[0];	depth+=d*d;
	d=bridge->tree->box[4]-bridge->tree->box[1];	depth+=d*d;
	d=bridge->tree->box[5]-bridge->tree->box[2];	depth+=d*d;

	return sqrtl(depth);
}

void mount_view::draw_background(graphics_system *gs)
{
	int i;
	gs->clear_background(background_red_color,background_green_color,background_blue_color);

	if(!defined_area.flag)
		return;
	gs->turn_on_clip_plane(NULL);
	for(i=0;i<4;i++)
		gs->display_line(1,0,0,
				defined_area.x[(i+1)%4],defined_area.y[(i+1)%4],defined_area.z[(i+1)%4],
				defined_area.x[i],defined_area.y[i],defined_area.z[i]);
	return;
}
int mount_view::do_mark_display(int index_id,graphics_system *gs,double box[],double manual_mount_proportion)
{
	int i,j,k;	
	double x[2],y[2],z[2],xx,yy,min_x,min_y,max_x,max_y,size_x,size_y;
	double A,B,C,D,tt,dx,dy,dz;
	double view_depth,eye_my_x,eye_my_y,eye_my_z,center_my_x,center_my_y,center_my_z,up_my_x,up_my_y,up_my_z;
	
	DWORD cur_time;
	double alf,bta;

	x[0]=box[0];	
	x[1]=box[3];	
	y[0]=box[1];
	y[1]=box[4];	
	z[0]=box[2];	
	z[1]=box[5];

	
	view_depth=sqrtl((x[1]-x[0])*(x[1]-x[0])+(y[1]-y[0])*(y[1]-y[0])+(z[1]-z[0])*(z[1]-z[0]));

//	set initial value 

	center_my_x=(box[0]+box[3])/2;
	center_my_y=(box[1]+box[4])/2;
	center_my_z=(box[2]+box[5])/2;

	cur_time=::GetTickCount();

	i=(int)(cur_time%10000);
	alf=((double)i)/10000;
	alf*=2*PI;

	j=(int)(cur_time/5000);
	j=j%5000;
	bta=((double)j)/5000;
	bta*=2*PI;

	bta =PI/4;

	eye_my_x=center_my_x+cosl(bta)*cos(alf);
	eye_my_z=center_my_z+cosl(bta)*sin(alf);
	eye_my_y=center_my_y+sinl(bta);

	up_my_x=eye_my_x;
	up_my_y=eye_my_y+1;
	up_my_z=eye_my_z;

//  modify up value

	A=center_my_x-eye_my_x;
	B=center_my_y-eye_my_y;
	C=center_my_z-eye_my_z;
	D=(A*eye_my_x+B*eye_my_y+C*eye_my_z);
		
	tt=(A*up_my_x+B*up_my_y+C*up_my_z-D)/(A*A+B*B+C*C);

	up_my_x=up_my_x-A*tt;
	up_my_y=up_my_y-B*tt;
	up_my_z=up_my_z-C*tt;

	dx=up_my_x-eye_my_x;
	dy=up_my_y-eye_my_y;
	dz=up_my_z-eye_my_z;

	tt=sqrtl(dx*dx+dy*dy+dz*dz);

	up_my_x=eye_my_x+(up_my_x-eye_my_x)/tt;
	up_my_y=eye_my_y+(up_my_y-eye_my_y)/tt;
	up_my_z=eye_my_z+(up_my_z-eye_my_z)/tt;

//	caculate scale portion

	gs->get_view_size(&size_x,&size_y);
	gs->setup_view(view_depth,eye_my_x,eye_my_y,eye_my_z,center_my_x,center_my_y,center_my_z,up_my_x,up_my_y,up_my_z);

	for(i=0;i<2;i++){
		for(j=0;j<2;j++){
			for(k=0;k<2;k++){
				if(!(gs->get_screen_xy(x[i],y[j],z[k],&xx,&yy)))
					return 0;
				if((i==0)&&(j==0)&&(k==0)){
					min_x=xx;
					min_y=yy;
					max_x=xx;
					max_y=yy;
				}
				if(xx<min_x)
					min_x=xx;
				if(yy<min_y)
					min_y=yy;
				if(xx>max_x)
					max_x=xx;
				if(yy>max_y)
					max_y=yy;
			}
		}
	}

	xx=fabs((max_x-min_x)/size_x);
	yy=fabs((max_y-min_y)/size_y);
	tt=(xx>yy)?xx:yy;

//  modify up value again

	tt*=1.2;

	dx=(center_my_x+(eye_my_x-center_my_x)*tt)-eye_my_x;
	dy=(center_my_y+(eye_my_y-center_my_y)*tt)-eye_my_y;
	dz=(center_my_z+(eye_my_z-center_my_z)*tt)-eye_my_z;
	
	eye_my_x+=dx;
	eye_my_y+=dy;
	eye_my_z+=dz;

	up_my_x+=dx;
	up_my_y+=dy;
	up_my_z+=dz;

	gs->setup_view(view_depth*5,eye_my_x,eye_my_y,eye_my_z,center_my_x,center_my_y,center_my_z,up_my_x,up_my_y,up_my_z);

	draw_background(gs);

	setup_light(gs,sizeof(light)/sizeof(light[0]),light,eye_my_x,eye_my_y,eye_my_z,center_my_x,center_my_y,center_my_z,up_my_x,up_my_y,up_my_z);
	gs->turn_on_clip_plane(NULL);

	return bridge->display(index_id,MARK_PART_DISPLAY_METHOD,
			NULL,gs,&selected_material_property,
			NULL,eye_x,eye_y,eye_z,center_x,center_y,center_z,
			speed,FALSE,manual_mount_proportion,FALSE);
}
int mount_view::do_display(int index_id,graphics_system *gs,double manual_mount_proportion)
{
	int ret_val;

	ret_val=process_modifier();
	adjust_view();
	gs->setup_view(get_view_depth(),eye_x,eye_y,eye_z,center_x,center_y,center_z,up_x,up_y,up_z);

	draw_background(gs);

	setup_light(gs,sizeof(light)/sizeof(light[0]),light,eye_x,eye_y,eye_z,center_x,center_y,center_z,up_x,up_y,up_z);

	if(display_reference_frame_flag){
		double length;
		length =(bridge->tree->box[0]-bridge->tree->box[3])*((bridge->tree->box[0]-bridge->tree->box[3]));
		length+=(bridge->tree->box[1]-bridge->tree->box[4])*((bridge->tree->box[1]-bridge->tree->box[4]));
		length+=(bridge->tree->box[2]-bridge->tree->box[5])*((bridge->tree->box[2]-bridge->tree->box[5]));
		length=2*sqrtl(length);
	
		gs->display_line(1,0,0,	length+center_x,center_y,center_z,	center_x,center_y,center_z);
		gs->display_line(0,1,0,	center_x,length+center_y,center_z,	center_x,center_y,center_z);
		gs->display_line(0,0,1,	center_x,center_y,length+center_z,	center_x,center_y,center_z);
	}

	gs->turn_on_clip_plane(clip_plane_flag?clip_plane_data:NULL);

	ret_val+=bridge->display(index_id,-1,
			NULL,gs,&selected_material_property,
			clip_plane_flag?clip_plane_data:NULL,
			eye_x,eye_y,eye_z,center_x,center_y,center_z,
			speed,TRUE,manual_mount_proportion,display_reference_frame_flag);
	gs->turn_on_clip_plane(NULL);

	return ret_val;
}

void mount_view::set_modifier(double my_start_time,double my_time_length)
{
	if(view_data_bak_number<=1)
		return;

	if(my_start_time<=0)
		my_start_time=(double)::GetTickCount();
	if(my_time_length<=0)
		my_time_length=1000;

	start_time=my_start_time;
	time_length=my_time_length;

	view_end.eye_x		=eye_x;
	view_end.eye_y		=eye_y;
	view_end.eye_z		=eye_z;
	view_end.center_x	=center_x;
	view_end.center_y	=center_y;
	view_end.center_z	=center_z;
	view_end.up_x		=up_x;
	view_end.up_y		=up_y;
	view_end.up_z		=up_z;

	view_start.center_x=view_data_bak[(view_data_bak_index+MAX_VIEW_DATA-1)%MAX_VIEW_DATA].center_x;
	view_start.center_y=view_data_bak[(view_data_bak_index+MAX_VIEW_DATA-1)%MAX_VIEW_DATA].center_y;
	view_start.center_z=view_data_bak[(view_data_bak_index+MAX_VIEW_DATA-1)%MAX_VIEW_DATA].center_z;

	view_start.eye_x=view_data_bak[(view_data_bak_index+MAX_VIEW_DATA-1)%MAX_VIEW_DATA].eye_x;
	view_start.eye_y=view_data_bak[(view_data_bak_index+MAX_VIEW_DATA-1)%MAX_VIEW_DATA].eye_y;
	view_start.eye_z=view_data_bak[(view_data_bak_index+MAX_VIEW_DATA-1)%MAX_VIEW_DATA].eye_z;

	view_start.up_x=view_data_bak[(view_data_bak_index+MAX_VIEW_DATA-1)%MAX_VIEW_DATA].up_x;
	view_start.up_y=view_data_bak[(view_data_bak_index+MAX_VIEW_DATA-1)%MAX_VIEW_DATA].up_y;
	view_start.up_z=view_data_bak[(view_data_bak_index+MAX_VIEW_DATA-1)%MAX_VIEW_DATA].up_z;

	view_modifier_flag=TRUE;

}
void mount_view::set_retreat_modifier(double my_start_time,double my_time_length)
{
	if(view_data_bak_number<=1)
		return;
	if(my_start_time<=0)
		my_start_time=(double)::GetTickCount();;
	if(my_time_length<=0)
		my_time_length=1000;

	start_time=my_start_time;
	time_length=my_time_length;

	view_start.eye_x		=		eye_x;
	view_start.eye_y		=		eye_y;
	view_start.eye_z		=		eye_z;
	view_start.center_x		=		center_x;
	view_start.center_y		=		center_y;
	view_start.center_z		=		center_z;
	view_start.up_x			=		up_x;
	view_start.up_y			=		up_y;
	view_start.up_z			=		up_z;

	restore_view_data();

	view_end.eye_x			=		eye_x;
	view_end.eye_y			=		eye_y;
	view_end.eye_z			=		eye_z;
	view_end.center_x		=		center_x;
	view_end.center_y		=		center_y;
	view_end.center_z		=		center_z;
	view_end.up_x			=		up_x;
	view_end.up_y			=		up_y;
	view_end.up_z			=		up_z;

	view_modifier_flag=TRUE;

	return;
}
int mount_view::process_modifier()
{
	double p,q,current_time,end_time;

	if(!view_modifier_flag)
		return 0;

	current_time=(double)(::GetTickCount());
	end_time=start_time+time_length/speed;
	
	p=(current_time-start_time)/(end_time-start_time);
	if(p<0)
		p=0;
	else if(p>=1.0){
		p=1.0;
		view_modifier_flag=FALSE;
	}
	q=1-p;

	eye_x=view_start.eye_x*q+view_end.eye_x*p;
	eye_y=view_start.eye_y*q+view_end.eye_y*p;
	eye_z=view_start.eye_z*q+view_end.eye_z*p;

	center_x=view_start.center_x*q+view_end.center_x*p;
	center_y=view_start.center_y*q+view_end.center_y*p;
	center_z=view_start.center_z*q+view_end.center_z*p;

	up_x=view_start.up_x*q+view_end.up_x*p;
	up_y=view_start.up_y*q+view_end.up_y*p;
	up_z=view_start.up_z*q+view_end.up_z*p;

	return 1;
}
void mount_view::process_scale(double p)
{
	double dx,dy,dz;

	dx=(center_x+(eye_x-center_x)*p)-eye_x;
	dy=(center_y+(eye_y-center_y)*p)-eye_y;
	dz=(center_z+(eye_z-center_z)*p)-eye_z;
	
	eye_x+=dx;
	eye_y+=dy;
	eye_z+=dz;

	up_x+=dx;
	up_y+=dy;
	up_z+=dz;

	return;
}
void mount_view::put_box_in_view(graphics_system *gs,double component_box[],double box_p)
{

	int i,j,k;	
	double x[2],y[2],z[2],xx,yy,zz,min_x,min_y,max_x,max_y,d,size_x,size_y;
	
	gs->setup_view(get_view_depth(),eye_x,eye_y,eye_z,center_x,center_y,center_z,up_x,up_y,up_z);
	gs->get_view_size(&size_x,&size_y);

	x[0]=component_box[0];	y[0]=component_box[1];	z[0]=component_box[2];	
	x[1]=component_box[3];	y[1]=component_box[4];	z[1]=component_box[5];

	for(i=0;i<2;i++){
		for(j=0;j<2;j++){
			for(k=0;k<2;k++){
				if(!(gs->get_screen_xy(x[i],y[j],z[k],&xx,&yy)))
					return;
				if((i==0)&&(j==0)&&(k==0)){
					min_x=xx;
					min_y=yy;
					max_x=xx;
					max_y=yy;
				}
				if(xx<min_x)
					min_x=xx;
				if(yy<min_y)
					min_y=yy;
				if(xx>max_x)
					max_x=xx;
				if(yy>max_y)
					max_y=yy;
			}
		}
	}
	xx=fabs((max_x-min_x)/size_x);
	yy=fabs((max_y-min_y)/size_y);
	d=(xx>yy)?xx:yy;
	d=d/box_p;

	process_scale(d);	

	gs->setup_view(get_view_depth(),eye_x,eye_y,eye_z,center_x,center_y,center_z,up_x,up_y,up_z);

	xx=center_x-(component_box[0]+component_box[3])/2;
	yy=center_y-(component_box[1]+component_box[4])/2;
	zz=center_z-(component_box[2]+component_box[5])/2;

	center_x-=xx;
	center_y-=yy;
	center_z-=zz;

	eye_x-=xx;
	eye_y-=yy;
	eye_z-=zz;

	up_x-=xx;
	up_y-=yy;
	up_z-=zz;

	return;
}
BOOL mount_view::get_point_space_locaton_on_plane(CPoint *screen_point,double data[],double *x,double *y,double *z,graphics_system *gs)
{
	double t,dx,dy,dz;

	if(!(gs->get_xy(screen_point->x,screen_point->y,0,x,y,z)))
		return FALSE;
	if(!(gs->get_xy(screen_point->x,screen_point->y,1,&dx,&dy,&dz)))
		return FALSE;

	dx=dx-(*x);
	dy=dy-(*y);
	dz=dz-(*z);

	t=(data[0]*(*x)+data[1]*(*y)+data[2]*(*z)+data[3])/(data[0]*dx+data[1]*dy+data[2]*dz);
	(*x)-=(t*dx);
	(*y)-=(t*dy);
	(*z)-=(t*dz);

	return TRUE;
}
BOOL mount_view::get_point_space_locaton(CPoint *screen_point,double data[],double *x,double *y,double *z,graphics_system *gs)
{
	double d[4];
	
	d[0]=(data[4]-data[1])*(data[8]-data[2])-(data[5]-data[2])*(data[7]-data[1]);
	d[1]=(data[5]-data[2])*(data[6]-data[0])-(data[3]-data[0])*(data[8]-data[2]);
	d[2]=(data[3]-data[0])*(data[7]-data[1])-(data[4]-data[1])*(data[6]-data[0]);
	d[3]=(-(data[0]*d[0]+data[1]*d[1]+data[2]*d[2]));

	return get_point_space_locaton_on_plane(screen_point,d,x,y,z,gs);
}
void mount_view::change_relative_points_to_absulate_points(part_tree *t, double data[],double mount_speeed,double manual_mount_proportion)
{
	int i,j,k;
	double a[4][4],b[4][4],c[4][4],x[4],y[4];

	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			a[i][j]=0;
			b[i][j]=0;
			c[i][j]=0;
		}
		a[i][i]=1;
		b[i][i]=1;
		c[i][i]=1;
	}

	for(;t!=NULL;t=t->parent){
		double move_relative_location[16];
		if(t->move!=NULL){
			double portion;
			if((portion=t->move->get_relative_location(move_relative_location,mount_speeed,manual_mount_proportion))>=0){
				if(portion<=1.0){
					for(i=0,k=0;i<4;i++)
						for(j=0;j<4;j++,k++)
							a[j][i]=move_relative_location[k];

					for(i=0;i<4;i++)
						for(j=0;j<4;j++)
							b[i][j]=c[i][j];


					for(i=0;i<4;i++)
						for(j=0;j<4;j++)
							for(k=0,c[i][j]=0;k<4;k++)
								c[i][j]+=a[i][k]*b[k][j];

				}
			}
		}

		for(i=0,k=0;i<4;i++)
			for(j=0;j<4;j++,k++)
				a[j][i]=t->relative_location[k];

		for(i=0;i<4;i++)
			for(j=0;j<4;j++)
				b[i][j]=c[i][j];


		for(i=0;i<4;i++)
			for(j=0;j<4;j++)
				for(k=0,c[i][j]=0;k<4;k++)
					c[i][j]+=a[i][k]*b[k][j];
	}
	for(k=0;k<18;k+=3){
		for(i=0;i<3;i++)
			x[i]=data[k+i];
		x[3]=1;
		for(i=0;i<4;i++)
			for(j=0,y[i]=0;j<4;j++)
				y[i]+=x[j]*c[i][j];
		for(i=0;i<3;i++)
			data[k+i]=y[i];
	}
	return;
}
part_tree  * mount_view::get_tree_body_face_triangle_and_xyz(int index_id,CPoint *screen_point,
		int *body_id, int *face_id, int *triangle_id, BOOL *xyz_flag,
		double *x, double *y, double *z,graphics_system *gs,double mount_speeed,double manual_mount_proportion)
{
	double 	color[3],data[18];
	int part_id,n,part_component_id,part_component_number,select_triangle_id;
	part_tree *t;
	CString str;

	*body_id=-1;
	*face_id=-1;
	*triangle_id=-1;
	*xyz_flag=FALSE;
	*x=0;
	*y=0;
	*z=0;

	adjust_view();
	gs->setup_view(get_view_depth(),eye_x,eye_y,eye_z,center_x,center_y,center_z,up_x,up_y,up_z);
	setup_light(gs,-1,light,eye_x,eye_y,eye_z,center_x,center_y,center_z,up_x,up_y,up_z);
	gs->turn_on_clip_plane(clip_plane_flag?clip_plane_data:NULL);
	gs->clear_background(0,0,0);	
	
	bridge->display(index_id,USER_SELECTED_PART_DISPLAY_METHOD,NULL,gs,
		&selected_material_property,clip_plane_flag?clip_plane_data:NULL,
		eye_x,eye_y,eye_z,center_x,center_y,center_z,speed,TRUE,
		manual_mount_proportion,FALSE);

	gs->get_screen_color(screen_point->x,screen_point->y,color);
	
	part_id=(int)((color[0])*(double)(bridge->part_number+2)-1);

	if((part_id<0)||(part_id>=bridge->part_number)){
		str.Format("没有选中杆件或杆件类型错(%d)",part_id);
//		AfxMessageBox(str);
		return NULL;
	}
	part_component_number=bridge->first_part_tree[part_id]->tree_node_union.part.part_component_number;

	n=3+(int)sqrtl((double)part_component_number);
	part_component_id=n*(int)((color[2])*(double)(n+2)-1)+(int)((color[1])*(double)(n+2)-1);	

	if((part_component_id<0)||(part_component_id>=part_component_number)){
		str.Format("没有选中杆件或或杆件编号错,杆件类型:%d,杆件编号:%d,同种杆件总数:%d",
			part_id,part_component_id,part_component_number);
//		AfxMessageBox(str);
		return NULL;
	}

	for(t=bridge->first_part_tree[part_id];;t=t->tree_node_union.part.back_part)
		if((t->tree_node_union.part.part_component_id)==part_component_id)
			break;
	gs->clear_background(0,0,0);
	bridge->display(index_id,USER_SELECTED_PART_DISPLAY_METHOD,t,gs,
		&selected_material_property,clip_plane_flag?clip_plane_data:NULL,
		eye_x,eye_y,eye_z,center_x,center_y,center_z,speed,TRUE,
		manual_mount_proportion,FALSE);
	gs->get_screen_color(screen_point->x,screen_point->y,color);

	n =        (int)(200*color[0]);
	n+=    200*(int)(200*color[1]);
	n+=200*200*(int)(200*color[2]);

	if(n<=0)
		return t;
	if(n==1){
		if(clip_plane_flag)
			*xyz_flag=get_point_space_locaton_on_plane(screen_point,clip_plane_data,x,y,z,gs);
		return t;
	}
	select_triangle_id=n-2;
	
	if(!(bridge->part[part_id]->get_body_face_triangle(select_triangle_id,body_id,face_id,triangle_id,&(data[0]),&(data[9]))))
		return t;
	change_relative_points_to_absulate_points(t,data,mount_speeed,manual_mount_proportion);
	*xyz_flag=get_point_space_locaton(screen_point,data,x,y,z,gs);

	return t;
}
void mount_view::set_clip_plane(BOOL flag,double x,double y,double z)
{
	double d;

	clip_plane_flag=flag;

	if(clip_plane_flag){
		clip_plane_data[0]=center_x-eye_x;
		clip_plane_data[1]=center_y-eye_y;
		clip_plane_data[2]=center_z-eye_z;

		d=sqrtl( clip_plane_data[0]*clip_plane_data[0]+clip_plane_data[1]*clip_plane_data[1]+clip_plane_data[2]*clip_plane_data[2]);

		(clip_plane_data[0])/=d;
		(clip_plane_data[1])/=d;
		(clip_plane_data[2])/=d;
		clip_plane_data[3]=-(clip_plane_data[0]*x+clip_plane_data[1]*y+clip_plane_data[2]*z);
	}
	return;
}
void mount_view::aim_at_box(double box[])
{
	double dx,dy,dz;

	dx=(box[0]+box[3])/2-center_x;;
	dy=(box[1]+box[4])/2-center_y;;
	dz=(box[2]+box[5])/2-center_z;;

	center_x+=dx;
	center_y+=dy;
	center_z+=dz;

	eye_x+=dx;
	eye_y+=dy;
	eye_z+=dz;

	up_x+=dx;
	up_y+=dy;
	up_z+=dz;

	return;
}
void mount_view::set_x_view()
{
	double d,dx,dy,dz;

	dx=center_x-eye_x;
	dy=center_y-eye_y;
	dz=center_z-eye_z;

	d=sqrtl(dx*dx+dy*dy+dz*dz);

	eye_x=center_x-d;
	eye_y=center_y;
	eye_z=center_z;

	up_x=eye_x;
	up_y=eye_y+1;
	up_z=eye_z;

	return;
}
void mount_view::set_y_view()
{
	double d,dx,dy,dz;

	dx=center_x-eye_x;
	dy=center_y-eye_y;
	dz=center_z-eye_z;

	d=sqrtl(dx*dx+dy*dy+dz*dz);

	eye_x=center_x;
	eye_y=center_y+d;
	eye_z=center_z;

	up_x=eye_x+1;
	up_y=eye_y;
	up_z=eye_z;

	return;
}
void mount_view::set_z_view()
{
	double d,dx,dy,dz;

	dx=center_x-eye_x;
	dy=center_y-eye_y;
	dz=center_z-eye_z;

	d=sqrtl(dx*dx+dy*dy+dz*dz);

	eye_x=center_x;
	eye_y=center_y;
	eye_z=center_z+d;

	up_x=eye_x;
	up_y=eye_y+1;
	up_z=eye_z;

	return;
}
BOOL mount_view::get_xy(double sx, double sy, double *x, double *y,double *z,graphics_system *gs)
{
	double x0,y0,z0,x1,y1,z1,dx,dy,dz,t,A,B,C,D;

	if(!(gs->get_xy(sx,sy,0,&x0,&y0,&z0)))
		return FALSE;
	if(!(gs->get_xy(sx,sy,1,&x1,&y1,&z1)))
		return FALSE;

	dx=x1-x0;
	dy=y1-y0;
	dz=z1-z0;

	if(sqrtl(t=dx*dx+dy*dy+dz*dz)<(MIN_VALUE*MIN_VALUE))
		return FALSE;
	
	A=center_x-eye_x;
	B=center_y-eye_y;
	C=center_z-eye_z;
	D=0-(A*eye_x+B*eye_y+C*eye_z);

	if(fabs(t=A*dx+B*dy+C*dz)<(MIN_VALUE*MIN_VALUE))
		return FALSE;

	t=(A*x0+B*y0+C*z0+D)/t;
	*x=x0-t*dx;
	*y=y0-t*dy;
	*z=z0-t*dz;

	return TRUE;
}
void mount_view::process_translation(graphics_system *gs,CPoint last_point,CPoint move_point)
{
	double last_x,last_y,last_z,move_x,move_y,move_z,dx,dy,dz;
	
	if(last_point==move_point)
		return;
	if(!get_xy(last_point.x,last_point.y,&last_x,&last_y,&last_z,gs))
		return;
	if(!get_xy(move_point.x,move_point.y,&move_x,&move_y,&move_z,gs))
		return;

	dx=(last_x-move_x);
	dy=(last_y-move_y);
	dz=(last_z-move_z);	

	eye_x+=dx;
	eye_y+=dy;
	eye_z+=dz;

	center_x+=dx;
	center_y+=dy;
	center_z+=dz;

	up_x+=dx;
	up_y+=dy;
	up_z+=dz;

	return;
}
BOOL mount_view::process_rotation_routine(double last_x, double last_y, double last_z, double move_x, double move_y, double move_z, 
	double rotate_x, double rotate_y, double rotate_z, double *x, double *y, double *z)
{
	double x1,y1,z1,x2,y2,z2,dx,dy,dz;
	double aix_x,aix_y,aix_z,alf,d,t;

	x1=rotate_x-move_x;		/*D1=ROTATE_MOVE*/
	y1=rotate_y-move_y;
	z1=rotate_z-move_z;
	
	x2=last_x-rotate_x;		/*D2=LAST-MOVE*/
	y2=last_y-rotate_y;
	z2=last_z-rotate_z;

	aix_x=(y1*z2-y2*z1);		/*AIX=D1*D2*/
	aix_y=(x2*z1-x1*z2);
	aix_z=(x1*y2-x2*y1);

	d=(x1*x1+y1*y1+z1*z1)*(x2*x2+y2*y2+z2*z2);
	if(d<MIN_VALUE)
		return FALSE;
	alf=(x1*x2+y1*y2+z1*z2)/sqrtl(d);		/*cos(alf)=|A*B|/(|A|*|B|)*/
	alf=acosl(-alf);

	if(fabs(alf)>(45*PI/180))
		return FALSE;

	alf=tanl(alf);

//modify new position

	x1=rotate_x-(*x);
	y1=rotate_y-(*y);
	z1=rotate_z-(*z);

	x2=aix_x;
	y2=aix_y;
	z2=aix_z;

	dx=y1*z2-y2*z1;			/*DIRECTION=(ROTATE-X)*AIX*/
	dy=x2*z1-x1*z2;
	dz=x1*y2-x2*y1;

	d=sqrtl(dx*dx+dy*dy+dz*dz);
	dx=dx/d;
	dy=dy/d;
	dz=dz/d;

	d=sqrtl((*x-rotate_x)*(*x-rotate_x)+(*y-rotate_y)*(*y-rotate_y)+(*z-rotate_z)*(*z-rotate_z));

	dx*=d*alf;
	dy*=d*alf;
	dz*=d*alf;

	t =dx*(move_x-last_x);
	t+=dy*(move_y-last_y);
	t+=dz*(move_z-last_z);

	if(t>0){
		dx=0-dx;
		dy=0-dy;
		dz=0-dz;
	}

	x1=*x+dx;
	y1=*y+dy;
	z1=*z+dz;

	t=d/sqrtl((x1-rotate_x)*(x1-rotate_x)+(y1-rotate_y)*(y1-rotate_y)+(z1-rotate_z)*(z1-rotate_z));
		
	*x=rotate_x*(1-t)+x1*t;
	*y=rotate_y*(1-t)+y1*t;
	*z=rotate_z*(1-t)+z1*t;
	
	return TRUE;
}

void mount_view::process_rotation(graphics_system *gs,CPoint last_point,CPoint move_point)
{
	double last_x,last_y,last_z,move_x,move_y,move_z;

	if(last_point==move_point)
		return;
	if(!get_xy(last_point.x,last_point.y,&last_x,&last_y,&last_z,gs))
		return;
	if(!get_xy(move_point.x,move_point.y,&move_x,&move_y,&move_z,gs))
		return;
	process_rotation_routine(last_x,last_y,last_z,move_x,move_y,move_z,center_x,center_y,center_z,
		&eye_x,&eye_y,&eye_z);
	process_rotation_routine(last_x,last_y,last_z,move_x,move_y,move_z,center_x,center_y,center_z,
		&up_x,&up_y,&up_z);
	return;
}
void mount_view::process_area_define(graphics_system *gs,CPoint last_point,CPoint move_point)
{
	int i;
	double A,B,C,D,t,x[4],y[4],z[4];

	t =(center_x-eye_x)*(center_x-eye_x);
	t+=(center_y-eye_y)*(center_y-eye_y);
	t+=(center_z-eye_z)*(center_z-eye_z);
	t=sqrtl(t);
	t=(t-get_view_depth())/t;

	x[0]=eye_x+t*(center_x-eye_x);
	y[0]=eye_y+t*(center_y-eye_y);
	z[0]=eye_z+t*(center_z-eye_z);

	A=center_x-eye_x;
	B=center_y-eye_y;
	C=center_z-eye_z;
	D=-(A*x[0]+B*y[0]+C*z[0]);

	if(!get_xy(last_point.x,last_point.y,&(x[0]),&(y[0]),&(z[0]),gs))
		return;
	if(!get_xy(move_point.x,last_point.y,&(x[1]),&(y[1]),&(z[1]),gs))
		return;
	if(!get_xy(move_point.x,move_point.y,&(x[2]),&(y[2]),&(z[2]),gs))
		return;
	if(!get_xy(last_point.x,move_point.y,&(x[3]),&(y[3]),&(z[3]),gs))
		return;

	for(i=0;i<4;i++){
		t=(A*x[i]+B*y[i]+C*z[i]+D)/(A*A+B*B+C*C);
		x[i]-=A*t;
		y[i]-=B*t;
		z[i]-=C*t;
	}

	for(i=0;i<4;i++){
		defined_area.x[i]=x[i];
		defined_area.y[i]=y[i];
		defined_area.z[i]=z[i];
	}
	defined_area.flag=TRUE;
	return;
}

void mount_view::process_area_enlarge(graphics_system *gs,CPoint last_point,CPoint move_point)
{
	double sx,sy;
	double p_x,p_y,p,x0,y0,z0,x1,y1,z1,dx,dy,dz,size_x,size_y;
	double t,A,B,C,D,old_center_x,old_center_y,old_center_z;

	defined_area.flag=FALSE;
	gs->get_view_size(&size_x,&size_y);

	if(move_point.x==last_point.x)
		return;
	if(move_point.y==last_point.y)
		return;

	p_x=(double)(move_point.x-last_point.x)/size_x;
	p_y=(double)(move_point.y-last_point.y)/size_y;

	if(p_x<0)
		p_x=0-p_x;
	if(p_y<0)
		p_y=0-p_y;

	if(p_x<p_y)
		p=p_y;
	else
		p=p_x;

	gs->setup_view(get_view_depth(),eye_x,eye_y,eye_z,center_x,center_y,center_z,up_x,up_y,up_z);

	if(!get_xy((move_point.x+last_point.x)/2,(move_point.y+last_point.y)/2,&x0,&y0,&z0,gs))
		return;
	if(p<0.02)
		return;

	process_scale(p*1.2);
	
	gs->setup_view(get_view_depth(),eye_x,eye_y,eye_z,center_x,center_y,center_z,up_x,up_y,up_z);
	gs->get_view_size(&sx,&sy);

	if(!get_xy(sx/2,sy/2,&x1,&y1,&z1,gs))
		return;

	dx=x0-x1;
	dy=y0-y1;
	dz=z0-z1;

	old_center_x=center_x;
	old_center_y=center_y;
	old_center_z=center_z;

	eye_x+=dx;
	eye_y+=dy;
	eye_z+=dz;

	center_x+=dx;
	center_y+=dy;
	center_z+=dz;
	
	up_x+=dx;
	up_y+=dy;
	up_z+=dz;

	A=center_x-eye_x;
	B=center_y-eye_y;
	C=center_z-eye_z;
	D=-(A*old_center_x+B*old_center_y+C*old_center_z);

	t=-(A*center_x+B*center_y+C*center_z+D)/(A*A+B*B+C*C);
	dx=A*t;
	dy=B*t;
	dz=C*t;


	eye_x+=dx;
	eye_y+=dy;
	eye_z+=dz;

	center_x+=dx;
	center_y+=dy;
	center_z+=dz;
	
	up_x+=dx;
	up_y+=dy;
	up_z+=dz;

	return;
}
void mount_view::process_clip_plane_modification(double portion)
{
	double dx,dy,dz;

	dx=center_x-eye_x;
	dy=center_y-eye_y;
	dz=center_z-eye_z;

	clip_plane_data[3]+=(0.1*(1-portion)*sqrt(dx*dx+dy*dy+dz*dz));

	return;
}
void mount_view::setup_light(graphics_system *gs,
				int max_light_number,struct graphics_system_light light_data[],
				double eye_x,double eye_y,double eye_z,
				double center_x,double center_y,double center_z,
				double up_x,double up_y,double up_z)
{
	int i;
	double location[MAX_LIGHT][3],dx_x,dx_y,dx_z,dy_x,dy_y,dy_z,dz_x,dz_y,dz_z,d,x,y,z;

	for(i=0;i<MAX_LIGHT;i++){
		location[i][0]=light[i].x;
		location[i][1]=light[i].y;
		location[i][2]=light[i].z;
	};
	
	dz_x=eye_x-center_x;
	dz_y=eye_y-center_y;
	dz_z=eye_z-center_z;
	d=sqrtl(dz_x*dz_x+dz_y*dz_y+dz_z*dz_z);
	dz_x/=d;
	dz_y/=d;
	dz_z/=d;

	dy_x=up_x-eye_x;
	dy_y=up_y-eye_y;
	dy_z=up_z-eye_z;
	d=sqrtl(dy_x*dy_x+dy_y*dy_y+dy_z*dy_z);
	dy_x/=d;
	dy_y/=d;
	dy_z/=d;

	dx_x=dy_y*dz_z-dy_z*dz_y;
	dx_y=dy_z*dz_x-dy_x*dz_z;
	dx_z=dy_x*dz_y-dz_x*dy_y;

	d=-sqrtl(dx_x*dx_x+dx_y*dx_y+dx_z*dx_z);
	dx_x/=d;
	dx_y/=d;
	dx_z/=d;

	for(i=0;i<MAX_LIGHT;i++)
		if(light[i].effective_flag){
			x=light[i].x;
			y=light[i].y;
			z=light[i].z;

			if(!(light[i].absolute_or_relative_flag)){
				light[i].x=eye_x+x*dx_x+y*dy_x+z*dz_x;
				light[i].y=eye_y+x*dx_y+y*dy_y+z*dz_y;;
				light[i].z=eye_z+x*dx_z+y*dy_z+z*dz_z;
			}
		}
	gs->graphics_setup_light(max_light_number,light_data,environment_red_color,environment_green_color,environment_blue_color,separate_specular_color_flag);

	for(i=0;i<MAX_LIGHT;i++){
		light[i].x=location[i][0];
		light[i].y=location[i][1];
		light[i].z=location[i][2];
	};
}

void mount_view::load_light(CString file_name)
{
	int i;
	CString str;
	ifstream f(light_file_name=file_name);
	char tmp[MAX_MESSAGE_LENGTH];
	
	f>>tmp>>background_red_color>>background_green_color>>background_blue_color;
	f>>tmp>>environment_red_color>>environment_green_color>>environment_blue_color;
	f>>tmp;str=tmp;
	if(str=="SeparateSpecularColor")
		separate_specular_color_flag=TRUE;
	else
		separate_specular_color_flag=FALSE;
		
	selected_material_property.load(&f);

	for(i=0;i<MAX_LIGHT;i++){
		light[i].x=0;
		light[i].y=0;
		light[i].z=0;

		light[i].R=0;
		light[i].G=0;
		light[i].B=0;

		light[i].Ambient=0;
		light[i].Diffuse=0;
		light[i].Specular=0;
		light[i].Shininess=0;
		light[i].Transparency=0;
		light[i].Emission=0;

		f>>tmp>>tmp;
		str=tmp;
		light[i].effective_flag=((str=="ON")||(str=="on"))?TRUE:FALSE;

		f>>tmp;
		str=tmp;
		light[i].absolute_or_relative_flag=((str=="绝对坐标")||(str=="absolute")||(str=="ABSOLUTE"))?TRUE:FALSE;
		

		f>>light[i].x;
		f>>light[i].y;
		f>>light[i].z;

		f>>light[i].R;
		f>>light[i].G;
		f>>light[i].B;

		f>>light[i].Ambient;
		f>>light[i].Diffuse;
		f>>light[i].Specular;
		f>>light[i].Shininess;
		f>>light[i].Transparency;
		f>>light[i].Emission;
	}
	f.close();
}
void mount_view::save_light()
{
	int j;
	ofstream f(light_file_name);

	f<<"BackgroundColor:		";
	f<<background_red_color<<"	";
	f<<background_green_color<<"	";
	f<<background_blue_color<<"\n";

	f<<"EnvironmentColor:	";
	f<<environment_red_color<<"	";
	f<<environment_green_color<<"	";
	f<<environment_blue_color<<"\n";

	f<<(separate_specular_color_flag?"SeparateSpecularColor":"SingleColor")<<"\n";


	selected_material_property.save(&f,"SelectedMaterialProperty:	");

	for(j=0;j<MAX_LIGHT;j++){
			f<<"\nLight"<<j+1<<":  "<<(light[j].effective_flag?"ON ":"OFF")<<"     ";
			f<<(light[j].absolute_or_relative_flag?"绝对坐标":"相对坐标");

			f<<"\n			";
			f<<light[j].x<<"	";
			f<<light[j].y<<"	";
			f<<light[j].z;

			f<<"\n			";
			f<<light[j].R<<"	";
			f<<light[j].G<<"	";
			f<<light[j].B<<"	";

			f<<"\n			";
			f<<light[j].Ambient<<"	";
			f<<light[j].Diffuse<<"	";
			f<<light[j].Specular<<"	";
			f<<light[j].Shininess<<"	";
			f<<light[j].Transparency<<"	";
			f<<light[j].Emission<<"	";
	}
	f.close();
	return;
}
