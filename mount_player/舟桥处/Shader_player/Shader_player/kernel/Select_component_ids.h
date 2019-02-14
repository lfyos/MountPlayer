#pragma once

#include "Clip_component.h"

class Movement_match{
		
public:
		HTREEITEM match_list_id;
		CString match_tag;

		CString source_name;
		int source_body_id,source_face_id;
		HTREEITEM source_list_id;

		CString destination_name;
		int destination_body_id,destination_face_id;
		HTREEITEM destination_list_id;

		inline void reset_destination()
		{
			destination_name=_T("没有配合");
			destination_body_id=-1;
			destination_face_id=-1;
			destination_list_id=NULL;
		}
		inline void reset_source()
		{
			source_name=_T("没有配合");
			source_body_id=-1;
			source_face_id=-1;
			source_list_id=NULL;
		}
		inline void reset()
		{
			match_list_id=NULL;
			match_tag=_T("没有标记");

			reset_destination();
			reset_source();
		}
		inline void copy_from(Movement_match &source)
		{
			match_list_id		=source.match_list_id;
			match_tag			=source.match_tag;

			source_name			=source.source_name;
			source_body_id		=source.source_body_id;
			source_face_id		=source.source_face_id;
			source_list_id		=source.source_list_id;

			destination_name	=source.destination_name;
			destination_body_id	=source.destination_body_id; 
			destination_face_id	=source.destination_face_id;
			destination_list_id	=source.destination_list_id;

			return;
		}
		void load(ifstream &f)
		{
			char buffer[MAX_BUFFER_LENGTH];

			reset();

			f>>buffer;
			match_tag=buffer;

			f>>buffer>>source_body_id>>source_face_id;
			source_name=buffer;
			f>>buffer>>destination_body_id>>destination_face_id;
			destination_name=buffer;
		}
		void flush(ofstream &f)
		{
			File_data::output_string(f,match_tag+_T("\n")+source_name);
			f<<"	"<<source_body_id<<"	"<<source_face_id<<"\n";
			File_data::output_string(f,destination_name);
			f<<"	"<<destination_body_id<<"	"<<destination_face_id<<"\n";
		};
		Movement_match()
		{
			reset();
		}
		~Movement_match()
		{
		}
};

class Select_component_ids
{
public:
	int body_id,face_id,vertex_id,loop_id,edge_id,edge_point_id;
	Point selection_point;

	Component *comp;
	Part_with_shader *part_with_shader;
	Shader_base *part_shader;
	Part *rude_part;
	Body *body;
	Face *face;
	Face_loop *face_loop;
	Face_edge *edge;
	Point start_point,end_point;

	Box box;
	bool has_box_flag;

	void compute_pointer(Part_set *parts);

	inline void reset()
	{
		body_id=-5;
		face_id=-5;
		vertex_id=-5;
		loop_id=-5;
		edge_id=-5;
		edge_point_id=-5;
		selection_point=Point();

		comp=NULL;
		part_with_shader=NULL;
		part_shader=NULL;
		rude_part=NULL;
		body=NULL;
		face=NULL;
		face_loop=NULL;
		edge=NULL;

		start_point=Point();
		end_point=Point();

		box=Box();
		has_box_flag=false;
	}
	inline void reset_from_match(Component *my_comp,int my_body_id,int my_face_id,Part_set *parts)
	{
		reset();
		if(my_comp==NULL){
			comp=NULL;
			body_id=-1;
			face_id=-1;
		}else{
			comp=my_comp;
			body_id=my_body_id;
			face_id=my_face_id;
		}
		compute_pointer(parts);
	}
	inline void copy_from(Select_component_ids &ids)
	{
		body_id				=ids.body_id;
		face_id				=ids.face_id;
		vertex_id			=ids.vertex_id;
		loop_id				=ids.loop_id;
		edge_id				=ids.edge_id;
		edge_point_id		=ids.edge_point_id;
		selection_point		=ids.selection_point;

		comp				=ids.comp;
		part_with_shader	=ids.part_with_shader;
		part_shader			=ids.part_shader;
		rude_part			=ids.rude_part;
		body				=ids.body;
		face				=ids.face;
		face_loop			=ids.face_loop;
		edge				=ids.edge;

		start_point			=ids.start_point;
		end_point			=ids.end_point;

	}
	inline Select_component_ids()
	{
		reset();
	}
	CString get_part_description(Part_set *parts);
};

class Size_tag
{
	inline Point caculate_min_distance_point(Point s,Point e,Point p)
	{
		double t;
		Point d(e-s);					
		if((t=d.distance2())<MIN_VALUE)
			return ((s+e)*0.5);
		else
			return (s+d*(d.dot(p-s)/t));
	}

	bool get_point_point_tag_point(Point &first_point,Point &second_point);
	bool get_point_line_tag_point(Point &first_point,Point &second_point);
	bool get_line_point_tag_point(Point &first_point,Point &second_point);
	bool get_line_line_tag_point(Point &first_point,Point &second_point);

	bool get_tag_point(Point &first_point,Point &second_point);

	int state_id;
	Select_component_ids first_tag,second_tag;

public:
	bool finish_flag;
	inline int get_state_id()
	{
		return state_id;
	}

	inline Size_tag()
	{
		reset();
	}
	inline ~Size_tag()
	{
		reset();
	}
	inline void reset()
	{
		state_id=0;
		finish_flag=false;

		first_tag.reset();
		second_tag.reset();
	}
	void add_tag_point(Select_component_ids &ids,Part_set &parts,int my_tag_direction_flag);
	bool modify_tag_screen_location(int my_tag_direction_flag);
	bool get_tag_point(double sx,double sy,Point eye_position,Location pro_loca);
	void get_bitmap(Camera *camera);	
	
	Point tag_point[4];
	int bitmap[16],tag_direction_flag;
};




