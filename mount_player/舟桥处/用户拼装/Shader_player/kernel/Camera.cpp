#include "StdAfx.h"
#include "camera.h"
void Camera::save(ofstream &f_camera)
{
	if(eye_component!=NULL){
		File_data attatch_name;
		attatch_name.load_string_data(eye_component->name);

		if(attatch_name.data_pointer()!=NULL){
			f_camera<<"\n安装组件		"<<(attatch_name.data_pointer());
			f_camera<<"\n视角大小		"<<fovy;
			f_camera<<"\n近距离比率		"<<near_value_ratio;
			f_camera<<"\n远距离比率		"<<far_value_ratio;
			f_camera<<"\n显示精度		"<<display_precision;
			f_camera<<"\n定位标志		"<<(locate_mount_flag?"YES":"NO");
			f_camera<<"\n放大标志		"<<(expand_mount_flag?"YES":"NO");

			f_camera<<"\n视口锁定标志		"<<(lock_camera_flag?"YES":"NO");
			f_camera<<"\n视口旋转标志		"<<(rotate_view_flag?"YES":"NO");

			f_camera<<"\n方向标志		"<<view_id<<"\n\n";
		}
	}
}
Camera::Camera(Component *root_component,Part_set &part_array,CString attatch_name,
			   double my_fovy,double my_near_value_ratio,double my_far_value_ratio,double my_display_precision,
			   bool my_locate_mount_flag,bool my_expand_mount_flag,bool my_lock_camera_flag,bool my_rotate_view_flag,int my_view_id)
{
	view_id=my_view_id;
	display_precision=my_display_precision;

	paint_part_number=0;
	paint_triangle_number=0;
	paint_time_length=0;

	near_value_ratio=my_near_value_ratio;
	far_value_ratio=my_far_value_ratio;

	distance=1.0;
	locate_mount_flag=my_locate_mount_flag;
	expand_mount_flag=my_expand_mount_flag;
	lock_camera_flag=my_lock_camera_flag;
	rotate_view_flag=my_rotate_view_flag;
	project_matrix_flag=true;

	set_fovy(my_fovy);

	attatch_to_component(attatch_name,root_component);

	restore_number=0;
	restore_start_time=Platform_information::get_current_time();
	restore_terminate_time=restore_start_time;

	clip_plane_number=0;

	if((view_id>=0)&&(root_component!=NULL)){
		
		while(root_component->parent!=NULL)
			root_component=root_component->parent;
		
		locate_mount_flag=true;
		expand_mount_flag=true;

		locate_view(false,root_component->component_box,1.0,1000,part_array);
		set_view   (false,root_component->component_box,1.0,1000,part_array,view_id);
		
		push_restore_record_process(1000,false);
		
		locate_mount_flag=my_locate_mount_flag;
		expand_mount_flag=my_expand_mount_flag;
	}
}

Component *Camera::attatch_to_component(CString &component_name,Component *root_component)
{
	if((eye_component=root_component->get_node_by_name(component_name))==NULL)
		eye_component=root_component;
	eye_component->selected_time=0;
	return eye_component;
}

void Camera::caculate_camera(double aspect_value,Part_set &part_array,double *mirror_plane)
{	
	double box_distance,near_value,far_value;

	{
		Component *root_component=eye_component;

		while (root_component->parent!=NULL)
			root_component=root_component->parent;
		if(lock_camera_flag)
			eye_component->move_component_into_box(root_component->component_box,part_array);
		box_distance=root_component->component_box.distance();
	}
	center_point=eye_component->absulate_location*Point(0,0,0);
	up_point=eye_component->absulate_location*Point(0,1,0);
	eye_point=eye_component->absulate_location*Point(0,0,distance);

	if((near_value_ratio<=0.0)||(near_value_ratio>=1.0))
		near_value=0.1*distance;
	else
		near_value=near_value_ratio*distance;

	far_value=far_value_ratio*distance;
	if((far_value<=near_value)||(far_value<(box_distance+distance)))
		far_value=box_distance+distance;

	near_point=eye_component->absulate_location*Point(0,0,distance-near_value);
	far_point=eye_component->absulate_location*Point(0,0,distance-far_value);

	Location perspective=Location::caculate_project_matrix(half_fovy_tanl,aspect_value,near_value,far_value,distance,project_matrix_flag);

	if(mirror_plane==NULL)
		project_matrix	=perspective*(Location::lookat(eye_component->absulate_location,distance));
	else{
		center_point	=center_point.mirror(mirror_plane[0],mirror_plane[1],mirror_plane[2],mirror_plane[3]);
		up_point		=up_point.mirror(mirror_plane[0],mirror_plane[1],mirror_plane[2],mirror_plane[3]);
		eye_point		=eye_point.mirror(mirror_plane[0],mirror_plane[1],mirror_plane[2],mirror_plane[3]);
		near_point		=near_point.mirror(mirror_plane[0],mirror_plane[1],mirror_plane[2],mirror_plane[3]);
		far_point		=far_point.mirror(mirror_plane[0],mirror_plane[1],mirror_plane[2],mirror_plane[3]);

		project_matrix	=perspective*(Location::lookat(eye_point,center_point,up_point));
	}
}
Location Camera::translate_view(double down_x,double down_y,double move_x,double move_y,Point &select_flag)
{
	Location loca=(-(eye_component->absulate_location));
	double screen_z=Graphics_system::object_to_screen(center_point,project_matrix).z;
	double my_down_x=down_x,my_down_y=down_y,my_move_x=move_x,my_move_y=move_y;

	my_move_x=my_down_x+(my_move_x-my_down_x)*(select_flag.x);
	my_move_y=my_down_y+(my_move_y-my_down_y)*(select_flag.y);

	Point down_point=loca*(Graphics_system::screen_to_object(Point(my_down_x,my_down_y,screen_z),project_matrix));
	Point move_point=loca*(Graphics_system::screen_to_object(Point(my_move_x,my_move_y,screen_z),project_matrix));
	Point distance=down_point-move_point;

	return Location::move(distance.x,distance.y,distance.z);
}
Location Camera::rotate_view(int rotate_type,double down_x,double down_y,double move_x,double move_y,double rotate_scale)
{
	double depth=Graphics_system::object_to_screen(center_point+(eye_point-center_point).expand(rotate_scale*distance),project_matrix).z;

	Point window_width_height=Graphics_system::width_and_height();
	Point down_point=Graphics_system::screen_to_object(Point(window_width_height.x/2,window_width_height.y/2,depth),project_matrix);
	Point move_point=Graphics_system::screen_to_object(Point(window_width_height.x/2+(move_x-down_x),window_width_height.y/2+(move_y-down_y),depth),project_matrix);

	Location loca=(-(eye_component->absulate_location));

	switch(rotate_type){
	default:
		return Location();
	case 0:
		down_point=loca*down_point;
		move_point=loca*move_point;
		break;
	case 1:
		down_point.x=center_point.x;
		move_point.x=center_point.x;
		down_point=loca*down_point;
		move_point=loca*move_point;
		break;
	case 2:
		down_point.y=center_point.y;
		move_point.y=center_point.y;
		down_point=loca*down_point;
		move_point=loca*move_point;
		break;
	case 3:
		down_point.z=center_point.z;
		move_point.z=center_point.z;
		down_point=loca*down_point;
		move_point=loca*move_point;
		break;
	case 4:
		down_point=loca*down_point;		down_point.x=0;
		move_point=loca*move_point;		move_point.x=0;
		break;
	case 5:
		down_point=loca*down_point;		down_point.y=0;
		move_point=loca*move_point;		move_point.y=0;		
		break;
	case 6:
		down_point=Graphics_system::screen_to_object(Point(down_x,down_y,depth),project_matrix);
		move_point=Graphics_system::screen_to_object(Point(move_x,move_y,depth),project_matrix);

		down_point=loca*down_point;		down_point.z=0;
		move_point=loca*move_point;		move_point.z=0;		
		break;
	}
	Point new_center,new_x,new_y,new_z;

	loca=Location::rotation(down_point,move_point);

	new_center=Point(0,0,0);
	new_z=(loca*Point(0,0,1)).expand(1);
	new_y=(loca*Point(0,1,0)).expand(1);
	new_x= new_y.cross(new_z).expand(1);
	new_z= new_x.cross(new_y).expand(1);

	loca=Location(new_center,new_x,new_y,new_z);
	loca=Location::standard_positive()*(-loca);

	if(rotate_view_flag)
		loca=Location::move(0,0,distance)*loca*Location::move(0,0,-distance);

	return loca;
}

void Camera::locate_view(bool push_record_flag,Box &b,double scale,double time_length,Part_set &part_array)
{
	if(eye_component==NULL)
		return;

	Point p=(-(eye_component->absulate_location))*(b.center());
	eye_component->modify_move_relative_location(Location::move(p.x,p.y,p.z),part_array);
	
	scale_view(b,scale);
	
	if(push_record_flag)
		push_restore_record(part_array,time_length,true);
	return;
}

void Camera::set_view(bool push_record_flag,Box &b,double scale,double time_length,Part_set &part_array,int view_type)
{
	if(eye_component==NULL)
		return;
	
	Location loca=(-(eye_component->absulate_location)),a;
	Point p=eye_component->absulate_location*Point(0,0,0);
	loca=loca*Location::move(p);

	switch(view_type){
	case 0:
		a=Location();
		break;
	case 1:
		a=Location::y_rotate(0+45)*Location::x_rotate(-45);
		break;
	case 2:
		a=Location::y_rotate(90+45)*Location::x_rotate(-45);
		break;
	case 3:
		a=Location::y_rotate(180+45)*Location::x_rotate(-45);
		break;
	case 4:
		a=Location::y_rotate(270+45)*Location::x_rotate(-45);
		break;
	case 5:
		a=Location::y_rotate(0+45)*Location::x_rotate(45);
		break;
	case 6:
		a=Location::y_rotate(90+45)*Location::x_rotate(45);
		break;
	case 7:
		a=Location::y_rotate(180+45)*Location::x_rotate(45);
		break;
	case 8:
		a=Location::y_rotate(270+45)*Location::x_rotate(45);
		break;
	case 9:
		a=Location::x_rotate(90);
		break;
	case 10:
		a=Location::x_rotate(180);
		break;
	case 11:
		a=Location::x_rotate(270);
		break;
	case 12:
		a=Location::y_rotate(90);
		break;
	case 13:
		a=Location::y_rotate(180);
		break;
	case 14:
		a=Location::y_rotate(270);
		break;
	case 15:
		a=Location::z_rotate(90);
		break;
	case 16:
		a=Location::z_rotate(180);
		break;
	case 17:
		a=Location::z_rotate(270);
		break;
	default:
		return;
	}

	eye_component->modify_move_relative_location(loca*a,part_array);
	
	scale_view(b,scale);

	if(push_record_flag)
		push_restore_record(part_array,time_length,true);
	return;
}

void Camera::push_restore_record_process(double time_length,bool flag)
{	
	if(eye_component==NULL)
		return;
	if(restore_number>=MAX_CAMERA_RETREAT_NUMBER){
		int i;
		for(restore_number=0,i=MAX_CAMERA_RETREAT_NUMBER/5;i<MAX_CAMERA_RETREAT_NUMBER;i++,restore_number++){
			restore[restore_number].fovy    =restore[i].fovy;
			restore[restore_number].distance=restore[i].distance;
			restore[restore_number].move    =restore[i].move;			
		}
	}
	restore[restore_number].fovy=fovy;
	restore[restore_number].distance=distance;
	restore[restore_number].move=eye_component->move_location;
	restore_number++;
	restore[restore_number].fovy=fovy;
	restore[restore_number].distance=distance;
	restore[restore_number].move=eye_component->move_location;

	restore_start_time=Platform_information::get_current_time();
	restore_terminate_time=restore_start_time;
	if(flag)
		if(restore_number>=2)
			restore_terminate_time+=time_length;
	return;
}

void Camera::pop_restore_record_process(double time_length,bool flag)
{
	restore_start_time=Platform_information::get_current_time();
	restore_terminate_time=restore_start_time;
	if(restore_number<=1)
		return;	
	restore_number--;
	restore[restore_number].fovy=fovy;
	restore[restore_number].distance=distance;
	restore[restore_number].move=eye_component->move_location;
	if(flag)
		if(restore_number>=0)
			restore_start_time+=time_length;
}

double Camera::process_restore_record(Part_set &part_array)
{
	double p;
	
	if(restore_terminate_time==restore_start_time){	
		if(restore_number>0){
			restore[restore_number-1].fovy=fovy;
			restore[restore_number-1].distance=distance;
			restore[restore_number-1].move=eye_component->move_location;
		}
		p=0;
	}else if(restore_start_time>restore_terminate_time){
		p=((double)(Platform_information::get_current_time()-restore_terminate_time))/((double)(restore_start_time-restore_terminate_time));
		if(p>=1.0){
			p=1.0;
			restore_terminate_time=restore_start_time;
		}else if(p<0)
			p=0;
		fovy    =p*(restore[restore_number-1].fovy    )+(1-p)*restore[restore_number].fovy;
		distance=p*(restore[restore_number-1].distance)+(1-p)*restore[restore_number].distance;
		eye_component->modify_move_absulate_location(restore[restore_number].move.mix(restore[restore_number-1].move,p),part_array);

		p=restore_start_time;
	}else{
		if(restore_number<0)
			restore_number=0;
		switch(restore_number){
		case 0:
			restore_terminate_time			=restore_start_time;
			break;
		case 1:
			restore_terminate_time			=restore_start_time;
			fovy							=restore[0].fovy;
			distance						=restore[0].distance;
			eye_component->modify_move_absulate_location(restore[0].move,part_array);
			break;
		default:
			p=((double)(Platform_information::get_current_time()-restore_start_time))/((double)(restore_terminate_time-restore_start_time));
			if(p>1.0){
				p=1.0;
				restore_start_time=restore_terminate_time;
			}else if(p<0)
				p=0;
			fovy    =p*(restore[restore_number-1].fovy    )    +(1-p)*restore[restore_number-2].fovy;
			distance=p*(restore[restore_number-1].distance)    +(1-p)*restore[restore_number-2].distance;
			eye_component->modify_move_absulate_location(restore[restore_number-2].move.mix(restore[restore_number-1].move,p),part_array);
			break;
		}
		p=restore_terminate_time;
	}
	return p;
}

void Camera::enlarge_selected_area(int mouse_down_x,int mouse_down_y,int mouse_up_x,int mouse_up_y,double portion,double depth_value,Part_set &part_array)
{
	if((depth_value<=0)||(depth_value>=1.0))
		depth_value=Graphics_system::object_to_screen(center_point,project_matrix).z;

	Point p=Point((mouse_down_x+mouse_up_x)/2.0,(mouse_down_y+mouse_up_y)/2.0,depth_value);
	p=Graphics_system::screen_to_object(p,project_matrix);	
	Point new_center_point=p;

	p=new_center_point-eye_point;
	distance=p.distance();

	double A=p.x,B=p.y,C=p.z,D=(-(A*new_center_point.x+B*new_center_point.y+C*new_center_point.z));

	p=new_center_point+(up_point-center_point);
	double t=(A*p.x+B*p.y+C*p.z+D)/(A*A+B*B+C*C);
	p=Point(p.x-A*t,p.y-B*t,p.z-C*t);

	Point new_up_point=new_center_point+(p-new_center_point).expand((up_point-center_point).distance());

	Point old_p3=center_point+(center_point-eye_point).cross(up_point-center_point).expand(1);
	Point new_p3=new_center_point+(new_center_point-eye_point).cross(new_up_point-new_center_point).expand(1);

	Location loca=(-(eye_component->absulate_location));

	Point p0	=loca*(center_point+(eye_point-center_point).expand(distance)),	new_p0=loca*eye_point;
	Point old_p1=loca*center_point,												new_p1=loca*new_center_point;
	Point old_p2=loca*up_point,													new_p2=loca*new_up_point;
	old_p3=loca*old_p3,															new_p3=loca*new_p3;

	Location a(p0,old_p1,old_p2,old_p3),b(new_p0,new_p1,new_p2,new_p3);

	eye_component->modify_move_relative_location(b*(-a),part_array);

	distance*=portion;
}
void Camera::enlarge_mouse_point(double mouse_x,double mouse_y,double scale_value,Box volume_box,Point *mouse_point)
{
	if(mouse_point!=NULL){
		Point new_center_point=(*mouse_point)+(center_point-(*mouse_point))*scale_value;

		new_center_point=(-(eye_component->absulate_location))*new_center_point;
		eye_component->move_location=eye_component->move_location*Location::move(new_center_point);
		eye_component->compute_absulate_location();
	}
	distance*=scale_value;
} 