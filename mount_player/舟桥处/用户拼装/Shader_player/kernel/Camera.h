#pragma once

#include "Light.h"

class Camera
{
	int restore_number;
	struct Camera_restore_stack{
		double fovy,distance;
		Location move;
	}restore[MAX_CAMERA_RETREAT_NUMBER+1];
	double restore_start_time,restore_terminate_time;

	void push_restore_record_process(double time_length,bool flag);
	void pop_restore_record_process(double time_length,bool flag);

	double fovy,near_value_ratio,far_value_ratio;

	inline void scale_view(Box &b,double scale)
	{
		if(expand_mount_flag){
			double box_distance=b.distance()*scale;
			if((box_distance>MIN_VALUE)&&(distance>MIN_VALUE))
				distance=0.5*box_distance/::tanl(M_PI*fovy/360);
		}
	}
public:

	void save(ofstream &f_camera);

	inline void set_fovy(double new_fovy)
	{
		fovy=new_fovy;
		half_fovy_tanl=::tanl(fovy*M_PI/360);
	}
	inline double get_fovy()
	{
		return fovy;
	}
	double distance;
	Component *eye_component;

	Point center_point,eye_point,up_point,near_point,far_point;
	double half_fovy_tanl,display_precision;
	
	Location project_matrix;

	Camera(Component *root_component,Part_set &part_array,CString attatch_name,
		double my_fovy,double my_near_value_ratio,double my_far_value_ratio,double my_display_precision,
		bool my_locate_mount_flag,bool my_expand_mount_flag,bool my_lock_camera_flag,bool my_rotate_view_flag,int my_view_id);

	inline virtual ~Camera(void)
	{
	}

	Component *attatch_to_component(CString &component_name,Component *root_component);
	
	void caculate_camera(double aspect_value,Part_set &part_array,double *mirror_plane);

	void locate_view(bool push_record_flag,Box &b,double scale,double time_length,Part_set &part_array);
	void set_view(bool push_record_flag,Box &b,double scale,double time_length,Part_set &part_array,int view_type=-1);

	Location translate_view(double down_x,double down_y,double move_x,double move_y,Point &select_flagy);
	Location rotate_view(int rotate_type,double down_x,double down_y,double move_x,double move_y,double rotate_scale);

	void enlarge_selected_area(int mouse_down_x,int mouse_down_y,int mouse_up_x,int mouse_up_y,double portion,double depth_value,Part_set &part_array);
	void enlarge_mouse_point(double mouse_x,double mouse_y,double scale_value,Box volume_box,Point *mouse_point);
			
	double process_restore_record(Part_set &part_array);
	
	inline void push_restore_record(Part_set &part_array,double time_length,bool flag)
	{
		push_restore_record_process(time_length,flag);
		process_restore_record(part_array);
	}
	inline void pop_restore_record(Part_set &part_array,double time_length,bool flag)
	{
		pop_restore_record_process(time_length,flag);
		process_restore_record(part_array);
	}

	int view_id;
	bool locate_mount_flag,expand_mount_flag,project_matrix_flag,lock_camera_flag,rotate_view_flag;

	struct Camera_clip_plane{
		double A,B,C,D;
	}clip_plane[MAX_CLIP_PLANE_NUMBER];
	int clip_plane_number;

	inline void clear_clip_plane()
	{
		clip_plane_number=0;
	}
	inline void add_clip_plane()
	{
		if((clip_plane_number>=0)&&(clip_plane_number<MAX_CLIP_PLANE_NUMBER)){
			Point d=(eye_point-center_point).expand(1);
			clip_plane[clip_plane_number].A=d.x;
			clip_plane[clip_plane_number].B=d.y;
			clip_plane[clip_plane_number].C=d.z;
			clip_plane[clip_plane_number].D=0.0-(d.x)*(center_point.x)-(d.y)*(center_point.y)-(d.z)*(center_point.z);
			clip_plane_number++;
		}
	}
	inline void modify_clip_plane(int clip_plane_id,double scale)
	{
		if((clip_plane_id>=0)&&(clip_plane_id<clip_plane_number))
			clip_plane[clip_plane_id].D+=scale*distance;
	}
	inline double caculate_precision2(Component *comp)
	{
		if(comp->component_box_flag){
			double d2=(comp->component_box.center()-eye_point).distance2();			
			if(d2>=MIN_VALUE){
				double box_d2=comp->part_box.distance2();
				double precision2=box_d2/d2;
				precision2/=(half_fovy_tanl*half_fovy_tanl);
				precision2*=(display_precision*display_precision);
				return precision2;
			}
		}
		return (1.0/(MIN_VALUE*MIN_VALUE));
	}
	int paint_part_number,paint_triangle_number;
	double paint_time_length;
};
