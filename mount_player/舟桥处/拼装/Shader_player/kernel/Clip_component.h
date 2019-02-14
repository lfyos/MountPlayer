#pragma once

#include "Component.h"

class Clip_component
{	
	inline double test(double x,double y,double z)
	{
		return (A*x+B*y+C*z+D);
	}
public:
	double A,B,C,D;
	
	bool display_part_on_plane_flag,do_clip_flag;
	Location project_at_plane_location;
	
	inline int clip_component_test(Component &comp,bool global_box_flag)
	{
		int less_number=0;

		if(global_box_flag||(comp.child!=NULL)){
			for(int i=0;i<2;i++)
				for(int j=0;j<2;j++)
					for(int k=0;k<2;k++){
						if(test(comp.component_box.p[i].x,comp.component_box.p[j].y,comp.component_box.p[k].z)<0)
							less_number++;
					}
		}else{
			for(int i=0;i<2;i++)
				for(int j=0;j<2;j++)
					for(int k=0;k<2;k++){
						Point p(comp.part_box.p[i].x,comp.part_box.p[j].y,comp.part_box.p[k].z);
						p=comp.absulate_location*p;
						if(test(p.x,p.y,p.z)<0)
							less_number++;
					}	
		}
		return less_number;
	}

	inline Clip_component(double my_A,double my_B,double my_C,double my_D,bool my_display_part_on_plane_flag,bool my_do_clip_flag)
	{
		A=my_A;
		B=my_B;
		C=my_C;
		D=my_D;		
		display_part_on_plane_flag=my_display_part_on_plane_flag;
		do_clip_flag=my_do_clip_flag;

		double go_plane_data[16]=
				{	1-A*A,		-A*B,		-A*C,		0,
					-A*B,		1-B*B,		-B*C,		0,
					-A*C,		-B*C,		1-C*C,		0,
					-A*D,		-B*D,		-C*D,		1
				};
		project_at_plane_location=Location(go_plane_data);
	}
	inline ~Clip_component(void)
	{
	}
};
class Clipper
{
public:

	int clipper_number;
	Clip_component *clip[MAX_CLIP_PLANE_NUMBER];

	inline Clipper()
	{
		clipper_number=0;
		for(int i=0;i<MAX_CLIP_PLANE_NUMBER;i++)
			clip[i]=NULL;
		return;
	}
	inline ~Clipper()
	{
		clear_clip_component();
	}

	inline void clear_clip_component()
	{
		for(int i=0;i<MAX_CLIP_PLANE_NUMBER;i++){
			if(clip[i]!=NULL){
				delete (clip[i]);
				clip[i]=NULL;
			}
		}
		clipper_number=0;
	}

	inline int add_clip_component(double my_A,double my_B,double my_C,double my_D,bool my_display_part_on_plane_flag,bool my_do_clip_flag)
	{
		double length;
		if(clipper_number>=MAX_CLIP_PLANE_NUMBER)
			return -1;
		if((length=Point(my_A,my_B,my_C).distance2())<MIN_VALUE)
			return -1;
		length=::sqrtl(length);
		clip[clipper_number]=new Clip_component(my_A/length,my_B/length,my_C/length,my_D/length,my_display_part_on_plane_flag,my_do_clip_flag);
		clipper_number++;
		return clipper_number-1;
	}

	inline int add_clip_component(Point &p0,Point &p1,Point &p2,bool my_display_part_on_plane_flag,bool my_do_clip_flag)
	{
		Point direction=(p1-p0).cross(p2-p1);
		return add_clip_component(direction.x,direction.y,direction.z,-((direction.x)*(p0.x)+(direction.y)*(p0.y)+(direction.z)*(p0.z)),my_display_part_on_plane_flag,my_do_clip_flag);
	}

	inline bool clipper_test(Component &comp,bool global_box_flag)
	{
		if(comp.component_box_flag){
			if(comp.parent==NULL)
				comp.need_do_clip_test_flag=true;
			else
				comp.need_do_clip_test_flag=comp.parent->need_do_clip_test_flag;

			if(comp.need_do_clip_test_flag){
				comp.need_do_clip_test_flag=false;
				for(int i=0;i<clipper_number;i++){
					switch(clip[i]->clip_component_test(comp,global_box_flag)){
					case 0:
						return true;										//total box is outside,all can be clipped,unnecessary to to clip test 
					case 8:
						break;												//inner to one clip plane
					default:
						comp.need_do_clip_test_flag=true;					//some inside,some outside,clip can not be decided 
						break;
					}
				}
			}
		}else
			comp.need_do_clip_test_flag=true;
		return false;
	}
	void add_selection_clipper(double win_x0, double win_y0, double win_x1, double win_y1,Location &project_matrix,bool do_clip_flag);
	void add_near_far_clipper(Point direction,Point near_point,Point far_point);
};
