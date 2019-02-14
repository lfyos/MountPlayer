#pragma once

#include "Assemble_type.h"


class Component_display_parameter
{
	bool flicker_flag;
	Select_component_ids *select_ids,*match_ids;

	float data_array[16+16*MAX_LIGHT_NUMBER+16*MAX_LIGHT_NUMBER+16];
	float clip_array[4*MAX_CLIP_PLANE_NUMBER],no_clip_array[4*MAX_CLIP_PLANE_NUMBER];
	
	inline void init_no_clip_array()
	{
		for(int i=0;i<(sizeof(no_clip_array)/sizeof(no_clip_array[0]));i++)
			no_clip_array[i]=0;
	}
	inline void init_clip_array()
	{
		int do_clip_number=0;
		Clip_component *cp;

		for(int i=0;i<(clipper->clipper_number);i++){
			if((cp=clipper->clip[i])->do_clip_flag){
				clip_array[4*do_clip_number+0]=(float)(cp->A);
				clip_array[4*do_clip_number+1]=(float)(cp->B);
				clip_array[4*do_clip_number+2]=(float)(cp->C);
				clip_array[4*do_clip_number+3]=(float)(cp->D);
				do_clip_number++;
			}
		}
		for(int i=do_clip_number;i<MAX_CLIP_PLANE_NUMBER;i++){
			clip_array[4*i+0]=0;
			clip_array[4*i+1]=0;
			clip_array[4*i+2]=0;
			clip_array[4*i+3]=0;
		}
	}
	inline void init_display_parameter()
	{
			int data_length=0;
			{
				float *project_matrix_pointer=&(data_array[data_length]);
				camera->project_matrix.get_location_float_data(project_matrix_pointer);
				data_length+=16;
			}

			{
				float *a=&(data_array[data_length]);		
				for(int i=0;i<MAX_LIGHT_NUMBER;i++)
					lights->light_array[i].light_matrix.get_location_float_data(a+16*i);
				data_length+=16*MAX_LIGHT_NUMBER;
			}
			{
				float *shadow_matrix_pointer=&(data_array[data_length]);
				for(int i=0;i<(lights->light_number);i++)
					if((lights->light_array[i].shadow_texture_unit_id>=0)&&(lights->light_array[i].shadow_texture_precision>0.0))
						lights->light_array[i].projection_matrix.get_location_float_data(&(shadow_matrix_pointer[16*i]));
				data_length+=16*MAX_LIGHT_NUMBER;
			}
			{
				float *select_and_match_component=&(data_array[data_length]);

				int body_id=-1,face_id=-1,loop_id=-1,edge_id=-1,edge_point_id=-1;
				Component *mouse_on_component_pointer=NULL;

				if(select_ids!=NULL){
					mouse_on_component_pointer=select_ids->comp;
					body_id=select_ids->body_id;
					face_id=select_ids->face_id;
					loop_id=select_ids->loop_id;
					edge_id=select_ids->edge_id;
					edge_point_id=select_ids->edge_point_id;
				}
				
				if(mouse_on_component_pointer!=NULL)
					select_and_match_component[0]=(float)(0.25)+(float)(mouse_on_component_pointer->component_id);
				else
					select_and_match_component[0]=(float)(-1.25);
						
				select_and_match_component[ 1]=(body_id<0)      ?(float)(-5):((float)body_id      +(float)0.25);
				select_and_match_component[ 2]=(face_id<0)      ?(float)(-5):((float)face_id      +(float)0.25);
				select_and_match_component[ 3]=(loop_id<0)      ?(float)(-5):((float)loop_id      +(float)0.25);
				select_and_match_component[ 4]=(edge_id<0)      ?(float)(-5):((float)edge_id      +(float)0.25);
				select_and_match_component[ 5]=(edge_point_id<0)?(float)(-5):((float)edge_point_id+(float)0.25);
			}
	
			{
				float *select_and_match_component=&(data_array[data_length]);
				int component_id=-1,body_id=-1,face_id=-1,loop_id=-1,edge_id=-1,edge_point_id=-1;
					
				if(match_ids!=NULL){
					if(flicker_flag){
						if(match_ids->comp!=NULL)
							component_id=match_ids->comp->component_id;
						body_id=match_ids->body_id;
						face_id=match_ids->face_id;
						loop_id=match_ids->loop_id;
						edge_id=match_ids->edge_id;
						edge_point_id=match_ids->edge_point_id;
					}
				}
						
				select_and_match_component[ 6]=(component_id<0) ?(float)(-5):((float)component_id +(float)0.25);
				select_and_match_component[ 7]=(body_id<0)      ?(float)(-5):((float)body_id      +(float)0.25);
				select_and_match_component[ 8]=(face_id<0)      ?(float)(-5):((float)face_id      +(float)0.25);
				select_and_match_component[ 9]=(loop_id<0)      ?(float)(-5):((float)loop_id      +(float)0.25);
				select_and_match_component[10]=(edge_id<0)      ?(float)(-5):((float)edge_id      +(float)0.25);
				select_and_match_component[11]=(edge_point_id<0)?(float)(-5):((float)edge_point_id+(float)0.25);

				data_length+=16;
			}
	}

public:
	Part_set *part_array;
	Camera *camera;
	Light_set *lights;
	
	Clipper *clipper;

	inline Component_display_parameter(
		Part_set *my_part_array,
		Camera *my_camera,
		Light_set *my_lights,
		Clipper *my_clipper,
		
		Select_component_ids *my_select_ids,
		Select_component_ids *my_match_ids)
	{
		flicker_flag=(((::GetTickCount()/500)%2)==0)?true:false;

		select_ids=my_select_ids;
		match_ids=my_match_ids;

		part_array	=my_part_array;
		camera		=my_camera;
		lights		=my_lights;
		clipper		=my_clipper;

		init_display_parameter();
		init_clip_array();
		init_no_clip_array();
	}
	static inline int get_display_data_size()
	{
		return 16+16*MAX_LIGHT_NUMBER+16*MAX_LIGHT_NUMBER+16;
	}

	inline float *get_display_parameter()
	{	
		return &(data_array[0]);
	}

	static inline int get_clip_data_size()
	{
		return 4*MAX_CLIP_PLANE_NUMBER;
	}
	inline float *get_no_clip_parameter()
	{
		return &(no_clip_array[0]);
	}
	inline float *get_clip_parameter()
	{
		return &(clip_array[0]);
	}
};

class Component_display_flag
{
public:
	int display_map;
	int paint_method_id;
	double selected_line_width,selected_point_size;

	inline Component_display_flag(int my_paint_method_id,int my_display_map,double my_selected_line_width,double my_selected_point_size)
	{
		paint_method_id		=my_paint_method_id;
		display_map			=my_display_map;
		selected_line_width=my_selected_line_width;
		selected_point_size=my_selected_point_size;
	};
};

class Component_display_list
{
public:
	bool hide_edge_stipple_display;
	bool hide_frame_stipple_display;
	bool color_and_depth_display;
	bool transpancy_display;
	bool depth_only_display;
	bool edge_display;
	bool selected_edge_display;
	bool point_display;
	bool frame_display;

	inline void clear_all()
	{
		hide_edge_stipple_display	=false;
		hide_frame_stipple_display	=false;
		depth_only_display			=false;
		color_and_depth_display		=false;
		edge_display				=false;
		selected_edge_display		=false;
		point_display				=false;
		transpancy_display			=false;
		frame_display				=false;
	}

	inline void copy_from(Component_display_list &s)
	{
		depth_only_display		=s.depth_only_display;
		color_and_depth_display	=s.color_and_depth_display;
		edge_display			=s.edge_display;
		selected_edge_display	=s.selected_edge_display;
		point_display			=s.point_display;
		transpancy_display		=s.transpancy_display;
		frame_display			=s.frame_display;
	}

	inline void load_default(int paint_method_id,Component *comp,bool my_selected_flag,Select_component_ids *select_ids)
	{
		switch(paint_method_id){
		case 0://component normal display
			if(!my_selected_flag){
				if(comp->transparence<=MIN_VALUE){
					depth_only_display=((comp->hide_line_type_id<2)?true:false);		//register depth display:	 hide_line_type_id 0:hide 1:stipple	2:visible
					if(comp->hide_line_type_id==1){
						if(comp->frame_display_detail_flag)
							hide_frame_stipple_display=true;
						else
							hide_edge_stipple_display=true;
					}
				}else if(comp->transparence>=(1.0-MIN_VALUE))
							color_and_depth_display=true;				//register normal display
				else
							transpancy_display=true;					//register transpance display

				if(comp->frame_display_flag){				//register edge display
					if(comp->frame_display_detail_flag)
						frame_display=true;
					else
						edge_display=true;
				}				
				if(select_ids!=NULL)
					if(select_ids->comp==comp){
						selected_edge_display=true;			//register selected_edge display
						point_display=true;					//register point display
						break;
					}
				break;
			}
		case 1://component selection display
			if((comp->transparence<=MIN_VALUE)||(comp->transparence>=(1.0-MIN_VALUE))){
				color_and_depth_display=true;				//register normal display
				edge_display=true;							//register edge display
			}
			break;
		case 2://component selection
		case 3://body selection
		case 4://face selection
		case 5://vertex selection
			if((comp->transparence<=MIN_VALUE)||(comp->transparence>=(1.0-MIN_VALUE))){
				color_and_depth_display=true;						//register normal display
				point_display=true;									//register point display
			}
			break;
		case 6://loop selection
		case 7://edge selection
			if((comp->transparence<=MIN_VALUE)||(comp->transparence>=(1.0-MIN_VALUE))){
				edge_display=true;									//register edge display
				point_display=true;									//register point display
			}
			break;
		case 8:
			if((comp->transparence<=MIN_VALUE)||(comp->transparence>=(1.0-MIN_VALUE))){
				point_display=true;									//register point display
			}
			break;
		}
	};
	inline void set_only_display_body()
	{
		clear_all();
		color_and_depth_display=true;
	}

	Component_display_list()
	{
		clear_all();
	};
	Component_display_list(Component_display_list &s)
	{
		copy_from(s);
	};

	Component_display_list(int paint_method_id,Component *comp,bool my_selected_flag,Select_component_ids *select_ids)
	{
		clear_all();
		load_default(paint_method_id,comp,my_selected_flag,select_ids);
	}
	~Component_display_list()
	{

	};
};
