#pragma once


#include "Point.h"

class Face_edge
{
	inline void init()
	{
		start_effective_flag=false;
		end_effective_flag=false;

		curve_type=-1;
		curve_parameter_number=0;
		curve_parameter=NULL;

		tessellation_point_number=0;
		tessellation=NULL;
	}
	inline void clear()
	{
		if(curve_parameter!=NULL){
			delete []curve_parameter;
			curve_parameter=NULL;
		}	
		if(tessellation!=NULL){
			delete []tessellation;
			tessellation=NULL;
		}
		init();
	}
public:
	Point start_point,end_point;
	bool start_effective_flag,end_effective_flag;

	int curve_type,curve_parameter_number;
	double *curve_parameter;

	int tessellation_point_number;
	Point *tessellation;

	inline bool release_face_edge_memory()
	{
		if(tessellation!=NULL){
			delete []tessellation;
			tessellation=NULL;
			return true;
		}
		return false;
	}

	inline void load(ifstream &f)
	{
		char buffer[MAX_BUFFER_LENGTH];	

		clear();

		f>>buffer;
		start_point=Point(f);
		if(CString(buffer)==_T("起点有效"))
			start_effective_flag=true;
		else
			start_effective_flag=false;

		f>>buffer;
		end_point=Point(f);
		if(CString(buffer)==_T("终点有效"))
			end_effective_flag=true;
		else
			end_effective_flag=false;

		f>>buffer>>buffer;	
		CString type_str(buffer);
		if(type_str==_T("直线"))
			curve_type=0;
		else if(type_str==_T("圆"))
			curve_type=1;
		else if(type_str==_T("椭圆"))
			curve_type=2;
		else 
			curve_type=(-1);

		curve_parameter_number=0;
		f>>buffer>>curve_parameter_number>>buffer;
		if(curve_parameter_number<=0){
			curve_parameter_number=0;
			curve_parameter=NULL;
		}else{
			curve_parameter=new double[curve_parameter_number];
			for(int i=0;i<curve_parameter_number;i++)
				f>>(curve_parameter[i]);
		}

		tessellation_point_number=0;
		f>>buffer>>tessellation_point_number;

		if(tessellation_point_number<=0){
			tessellation_point_number=0;
			tessellation=NULL;
		}else{
			tessellation=new Point[tessellation_point_number];
			for(int i=0;i<tessellation_point_number;i++){
				f>>buffer>>buffer>>buffer;
				tessellation[i]=Point(f);
			}
		}

		caculate_box();
	}

	inline Face_edge(void)
	{
		init();
	}
	inline ~Face_edge(void)
	{
		clear();
	}

	bool has_box_flag;
	Box box;
	inline void caculate_box()
	{
		if(tessellation==NULL)
			return;
		has_box_flag=true;
		if(tessellation_point_number>0){
			box=Box(tessellation[0]);
			for(int i=1;i<tessellation_point_number;i++)
				box=box+Box(tessellation[i]);
			if(start_effective_flag)
				box=box+Box(start_point);
			if(end_effective_flag)
				box=box+Box(end_point);
		}else if(start_effective_flag){
				box=Box(start_point);
				if(end_effective_flag)
					box=box+Box(end_point);
		}else if(end_effective_flag)
				box=Box(end_point);
		else{
				has_box_flag=false;
				box=Box();
		}
	}
};


class Face_loop{
	inline void clear()
	{
		if(edge!=NULL){
			delete []edge;
			edge=NULL;
		}
		edge_number=0;
	}
public:

	int edge_number;
	Face_edge *edge;

	inline bool release_face_loop_memory()
	{
		bool ret_val=false;
		for(int i=0;i<edge_number;i++)
			ret_val|=edge[i].release_face_edge_memory();
		return ret_val;
	}

	inline void load(ifstream &f)
	{
		char buffer[MAX_BUFFER_LENGTH];

		clear();

		edge_number=0;
		f>>buffer>>buffer>>buffer>>edge_number;

		if(edge_number<=0){
			edge_number=0;
			edge=NULL;
			return;
		}

		edge=new Face_edge[edge_number];
		for(int i=0;i<edge_number;i++){
			f>>buffer>>buffer>>buffer;
			edge[i].load(f);
		}

		caculate_box();
	}

	inline Face_loop()
	{
		edge_number=0;
		edge=NULL;
	}
	inline ~Face_loop()
	{
		clear();
	}

	bool has_box_flag;
	Box box;
	inline void caculate_box()
	{
		has_box_flag=false;
		box=Box();

		for(int i=0,j=0;i<edge_number;i++)
			if(edge[i].has_box_flag){
				has_box_flag=true;
				if((j++)==0)
					box=edge[i].box;
				else
					box=box+(edge[i].box);
			}
	}
};


class Face_curve
{
	inline void clear()
	{
		if(face_parameter!=NULL)
			delete []face_parameter;
		face_parameter=NULL;
		face_parameter_number=0;
		
		if(face_loop!=NULL)
			delete []face_loop;
		face_loop=NULL;
		face_loop_number=0;

		face_type=-1;

	}
public:
	int face_type,face_parameter_number,face_loop_number;
	double *face_parameter;

	Face_loop *face_loop;
	inline bool release_face_curve_memory()
	{
		bool ret_val=false;
		for(int i=0;i<face_loop_number;i++)
			ret_val|=face_loop[i].release_face_loop_memory();
		return ret_val;
	}

	inline void load(ifstream &f)
	{
		char buffer[MAX_BUFFER_LENGTH];

		clear();

		f>>buffer>>buffer;
		CString face_type_str(buffer);

		if(face_type_str==_T("平面"))
			face_type=0;
		else if(face_type_str==_T("柱面"))
			face_type=1;
		else if(face_type_str==_T("锥面"))
			face_type=2;
		else if(face_type_str==_T("球面"))
			face_type=3;
		else if(face_type_str==_T("环面"))
			face_type=4;
		else 
			face_type=-1;

		face_parameter_number=0;
		f>>buffer>>face_parameter_number>>buffer;
		if(face_parameter_number<=0){
			face_parameter_number=0;
			face_parameter=NULL;
		}else{
			face_parameter=new double [face_parameter_number];
			for(int i=0;i<face_parameter_number;i++)
				f>>(face_parameter[i]);
		}
		face_loop_number=0;
		f>>buffer>>face_loop_number;
		if(face_loop_number<=0){
			face_loop_number=0;
			face_loop=NULL;
		}else{
			face_loop=new Face_loop[face_loop_number];
			for(int i=0;i<face_loop_number;i++)
				face_loop[i].load(f);
		}
		caculate_box();
	}

	inline Face_curve()
	{
		face_parameter=NULL;
		face_parameter_number=0;
		
		face_loop=NULL;
		face_loop_number=0;

		face_type=-1;		;
	}
	inline ~Face_curve()
	{
		clear();
	}

	bool has_box_flag;
	Box box;
	inline void caculate_box()
	{
		has_box_flag=false;
		box=Box();

		for(int i=0,j=0;i<face_loop_number;i++)
			if(face_loop[i].has_box_flag){
				has_box_flag=true;
				if((j++)==0)
					box=face_loop[i].box;
				else
					box=box+(face_loop[i].box);
			}
	}
};





