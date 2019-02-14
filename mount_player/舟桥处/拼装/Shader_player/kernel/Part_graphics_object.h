#pragma once

#include "Part.h"

class Part_graphics_object{
	float *vertex_data;

	int caculate_vertex_data_from_part(int map[],bool frame_object_flag,Part *p,float *buffer_pointer=NULL);

	void caculate_vertex_data_from_box(Point &p0,Point &p1,Point &p2,Point &p3,Point &p4,Point &p5,Point &p6,Point &p7);

	void caculate_edge_data_from_part(Part *p);
	void caculate_point_data_from_part(Part *p);

public:
	int part_buffer_object_id;;	
	int texture_number,triangle_number;

	Part_graphics_object *next_graphics_object_pointer;

	inline int get_stride()
	{
		return 4*(4+texture_number);
	}
	inline int get_volume()
	{
		return 3*triangle_number*get_stride();
	}
	inline float *get_vertex_data()
	{
		return vertex_data;
	}
	inline void set_normal(double x,double y,double z)
	{
		float *p=vertex_data;
		for(int i=0;i<triangle_number;i++){
			for(int j=0;j<3;j++,p+=get_stride()){
				p[4]=(float)x;
				p[5]=(float)y;
				p[6]=(float)z;
				p[7]=(float)1.0;
			}
		}
	}

	~Part_graphics_object()
	{
		if(part_buffer_object_id!=0)
			Graphics_system::delete_buffer_object(part_buffer_object_id);
		
		if(next_graphics_object_pointer!=NULL)
			delete next_graphics_object_pointer;
		next_graphics_object_pointer=NULL;

		if(vertex_data!=NULL){
			delete []vertex_data;
			vertex_data=NULL;
		}
	}

	Part_graphics_object(Part *p,int my_texture_number,Part_graphics_object *my_next_graphics_object_pointer)	
	{
		part_buffer_object_id=0;
		next_graphics_object_pointer=my_next_graphics_object_pointer;

		if(my_texture_number>=0){						//part face display
			int map[]={0,1,2,-1};

			texture_number=my_texture_number;
			triangle_number=caculate_vertex_data_from_part(map,false,p);
			vertex_data=new float[get_volume()];
			caculate_vertex_data_from_part(map,false,p,vertex_data);
		}else if(my_texture_number==-1){					//part edge display
			caculate_edge_data_from_part(p);
		}else if(my_texture_number==-2){					//part point display
			caculate_point_data_from_part(p);
		}else if(my_texture_number==-3){					//part frame display
			int map[]={0,1,1,2,2,0,-1};
			texture_number=0;
			triangle_number=2*caculate_vertex_data_from_part(map,true,p);
			vertex_data=new float[get_volume()];
			caculate_vertex_data_from_part(map,true,p,vertex_data);

		}else{
			vertex_data=NULL;
			texture_number=0;
			triangle_number=0;
		}
		return;
	}

	Part_graphics_object(								//box display
		Point &p0,Point &p1,Point &p2,Point &p3,Point &p4,Point &p5,Point &p6,Point &p7,Part_graphics_object *my_next_graphics_object_pointer)	
	{
		part_buffer_object_id=0;
		next_graphics_object_pointer=my_next_graphics_object_pointer;

		caculate_vertex_data_from_box(p0,p1,p2,p3,p4,p5,p6,p7);
	}

	Part_graphics_object(Box *b,Part_graphics_object *my_next_graphics_object_pointer)	//box display 
	{
		part_buffer_object_id=0;
		next_graphics_object_pointer=my_next_graphics_object_pointer;
	
		double 	x0=b->p[0].x,	y0=b->p[0].y,	z0=b->p[0].z;
		double 	x1=b->p[1].x,	y1=b->p[1].y,	z1=b->p[1].z;
		caculate_vertex_data_from_box(Point(x0,y0,z0),Point(x0,y0,z1),Point(x0,y1,z0),Point(x0,y1,z1),Point(x1,y0,z0),Point(x1,y0,z1),Point(x1,y1,z0),Point(x1,y1,z1));
	}

	inline int create_buffer_object()
	{
		if(vertex_data!=NULL)
			if(part_buffer_object_id==0)
				if((part_buffer_object_id=Graphics_system::create_buffer_object(vertex_data,get_volume()))!=0){
					delete []vertex_data;
					vertex_data=NULL;
				}
		return part_buffer_object_id;
	}
};