#include "StdAfx.h"
#include "Clip_component.h"


void Clipper::add_selection_clipper(double win_x0, double win_y0, double win_x1, double win_y1,Location &project_matrix,bool do_clip_flag)
{
	if(win_x0>win_x1){
		double x=win_x0;
		win_x0=win_x1;
		win_x1=x;
	}
	if(win_y0>win_y1){
		double x=win_y0;
		win_y0=win_y1;
		win_y1=x;
	}
	if(((win_x1-win_x0)>MIN_VALUE)&&((win_y1-win_y0)>MIN_VALUE)){
			Point left_up_1		=Graphics_system::screen_to_object(Point(win_x0,win_y0,0.875),project_matrix);
			Point left_down_1	=Graphics_system::screen_to_object(Point(win_x0,win_y1,0.875),project_matrix);
			Point right_up_1	=Graphics_system::screen_to_object(Point(win_x1,win_y0,0.875),project_matrix);
			Point right_down_1	=Graphics_system::screen_to_object(Point(win_x1,win_y1,0.875),project_matrix);

			Point left_up_0		=Graphics_system::screen_to_object(Point(win_x0,win_y0,0.125),project_matrix);
			Point left_down_0	=Graphics_system::screen_to_object(Point(win_x0,win_y1,0.125),project_matrix);
			Point right_up_0	=Graphics_system::screen_to_object(Point(win_x1,win_y0,0.125),project_matrix);
			Point right_down_0	=Graphics_system::screen_to_object(Point(win_x1,win_y1,0.125),project_matrix);


			add_clip_component(right_up_0,	right_up_1,		left_up_1,		false,do_clip_flag);		//up plane
			add_clip_component(left_down_0,	left_down_1,	right_down_1,	false,do_clip_flag);		//down plane
			add_clip_component(left_up_0,	left_up_1,		left_down_1,	false,do_clip_flag);		//left plane
			add_clip_component(right_down_0,right_down_1,	right_up_1,		false,do_clip_flag);		//right plane
	}
}

void Clipper::add_near_far_clipper(Point direction,Point near_point,Point far_point)
{
	add_clip_component(	-(direction.x),	-(direction.y),	-(direction.z),	 (direction.x)*(near_point.x)	+(direction.y)*(near_point.y)	+(direction.z)*(near_point.z),	false,false);		//near plane
	add_clip_component(	 (direction.x),	 (direction.y),	 (direction.z),	-(direction.x)*( far_point.x)	-(direction.y)*( far_point.y)	-(direction.z)*( far_point.z),	false,false);		//far plane
}
