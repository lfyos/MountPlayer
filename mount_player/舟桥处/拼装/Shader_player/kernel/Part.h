#pragma once

#include "shader_base.h"

class  Triangle_point
{
public:
	int p0,p1,p2;
};

class Triangle
{
public:
	int material_id,edge_flag;
	Triangle_point vertex,normal,*texture;

	void load(ifstream &f,int texture_number);
	inline Triangle()
	{
		material_id=0;		edge_flag=0;
		vertex.p0=0;		vertex.p1=0;		vertex.p2=0;
		normal.p0=0;		normal.p1=0;		normal.p2=0;
		texture=NULL;
	}
	inline virtual ~Triangle()
	{
		if(texture!=NULL)
			delete []texture;
		texture=NULL;
	}
};
class Triangle_set
{
public:
	int triangle_number;
	Triangle *triangle_array;

	inline bool release_triangle_set_memory()
	{
		if(triangle_array!=NULL){
			delete []triangle_array;
			triangle_array=NULL;
			return true;
		}
		return false;
	}

	void load(ifstream &f,int texture_number);
	
	inline Triangle_set()
	{
		triangle_array=NULL;
		triangle_number=0;
	};
	inline virtual ~Triangle_set()
	{
		release_triangle_set_memory();
	};
};

class Face
{
public:
	CString name;
	int texture_number;
	int min_material_id,max_material_id;

	Point_set *textures;
	Point_set vertices,normals;
	Triangle_set triangles;

	Face_curve face_curve;

	inline bool release_face_memory(int release_memory_flag_map)
	{
		bool ret_val=false;

		if((release_memory_flag_map&(1<<0))==0)			//release curve  memory
			ret_val|=face_curve.release_face_curve_memory();
		if((release_memory_flag_map&(1<<1))==0)			//release vertex  memory
			ret_val|=vertices.release_point_set_memory();		
		if((release_memory_flag_map&(1<<2))==0)			//release normal  memory
			ret_val|=normals.release_point_set_memory();
		if((release_memory_flag_map&(1<<3))==0)			//release triangle  memory
			ret_val|=triangles.release_triangle_set_memory();
		for(int i=0;i<texture_number;i++)				//release texture  memory
			if((release_memory_flag_map&(1<<(4+i)))==0)	
				ret_val|=textures[i].release_point_set_memory();
		return ret_val;
	}

	void load(ifstream &f);
	bool flush_for_create_assemble_part(ofstream &f,Location loca,int material_id_base);

	inline void face_get_material_id_range()
	{
		min_material_id=0;
		max_material_id=0;
		for(int i=0;i<(triangles.triangle_number);i++){
			if(i==0){
				min_material_id=triangles.triangle_array[0].material_id;
				max_material_id=min_material_id;
			}else{
				int j=triangles.triangle_array[i].material_id;
				if(min_material_id>j)
					min_material_id=j;
				if(max_material_id<j)
					max_material_id=j;
			}
		}
	}

	inline Face(void)
	{
		name=_T("无名表面");
		texture_number=0;
		
		min_material_id=0;
		max_material_id=0;
		
		textures=NULL;
	}
	inline virtual ~Face(void)
	{
		if(textures!=NULL)
			delete []textures;
		textures=NULL;
	}
	bool has_box_flag;
	Box box;
	inline void caculate_box()
	{
		vertices.caculate_box();
		has_box_flag=vertices.has_box_flag;
		box=vertices.box;
	}
};

class Body
{
public:
	CString name;
	
	int face_number,max_texture_number;

	int min_material_id,max_material_id;

	Face *face_array;

	inline bool release_body_memory(int release_memory_flag_map)
	{
		bool ret_val=false;

		for(int i=0;i<face_number;i++)
			ret_val|=face_array[i].release_face_memory(release_memory_flag_map);

		return ret_val;
	}

	void load(ifstream &f);
	
	inline Body()
	{
		name=_T("无名形体");
		face_number=0;
		max_texture_number=0;

		min_material_id=0;
		max_material_id=0;

		face_array=NULL;
	};
	inline virtual ~Body()
	{
		if(face_array!=NULL)
			delete []face_array;
	};

	bool has_box_flag;
	Box box;
	inline void caculate_box()
	{
		has_box_flag=false;
		box=Box();

		for(int i=0,j=0;i<face_number;i++){
			if(face_array[i].has_box_flag){
				has_box_flag=true;
				if((j++)==0)
					box=face_array[i].box;
				else
					box=box+(face_array[i].box);
			}
		}
	}

	inline void body_get_material_id_range()
	{
		min_material_id=0;
		max_material_id=0;
		for(int i=0;i<face_number;i++){
			Face *f=&(face_array[i]);
			f->face_get_material_id_range();
			if(i==0){					
					min_material_id=f->min_material_id;
					max_material_id=f->max_material_id;
			}else{
					if(min_material_id>(f->min_material_id))
						min_material_id=f->min_material_id;
					if(max_material_id<(f->max_material_id))
						max_material_id=f->max_material_id;
				}
		}
	}
};

class Part
{
public:
	inline void part_clear()
	{
		if(body_array!=NULL)
			delete []body_array;
		body_array=NULL;

		body_number=0;
		total_triangle_number=0;
		max_texture_number=0;
	}
public:

	bool rude_part_has_box_flag;
	Box box;
	inline void caculate_box()
	{
		rude_part_has_box_flag=false;
		box=Box();

		for(int i=0,j=0;i<body_number;i++){
			if(body_array[i].has_box_flag){
				rude_part_has_box_flag=true;
				if((j++)==0)
					box=body_array[i].box;
				else
					box=box+(body_array[i].box);
			}
		}
	}
	inline void caculate_triangle_number()
	{
		total_triangle_number=0;
		if(body_array!=NULL)
			for(int i=0;i<body_number;i++)
				if(body_array[i].face_array!=NULL)
					for(int j=0;j<body_array[i].face_number;j++)
						total_triangle_number+=body_array[i].face_array[j].triangles.triangle_number;
	}
public:
	int body_number,total_triangle_number,max_texture_number;
	CString directory_name,file_name;

	Body *body_array;

	inline bool release_part_memory(int release_memory_flag_map)
	{
		bool ret_val=false;
		for(int i=0;i<body_number;i++)
			ret_val|=body_array[i].release_body_memory(release_memory_flag_map);

		return ret_val;
	}

	HTREEITEM list_token;

	void rude_part_load();
	bool flush_for_create_assemble_part(ofstream &f,int &body_id_base,Location loca,int material_id_base);

	int min_material_id,max_material_id;

	inline void part_get_material_id_range()
	{
		min_material_id=0;
		max_material_id=0;
		for(int i=0;i<body_number;i++){
			Body *b=&(body_array[i]);
			b->body_get_material_id_range();
			if(i==0){
					min_material_id=b->min_material_id;
					max_material_id=b->max_material_id;
			}else{
					if(min_material_id>(b->min_material_id))
						min_material_id=b->min_material_id;
					if(max_material_id<(b->max_material_id))
						max_material_id=b->max_material_id;
			}
		}
	}

	inline Part(CString my_directory_name,CString my_file_name)
	{
		body_number=0;
		total_triangle_number=0;
		max_texture_number=0;

		min_material_id=0;
		max_material_id=0;
				
		directory_name=my_directory_name;
		file_name=my_file_name;

		body_array=NULL;
		list_token=NULL;
	}

	inline virtual ~Part(void)
	{
		part_clear();
	}
};

class Part_with_shader
{
public:
		CString user_name,part_name,description_file_name,sound_file_name;
		double precision,access_time;
				
		Part *rude_part;
		Shader_base *part_shader;

		bool has_load_flag,need_load_flag,information_has_been_released_flag;

		Part_with_shader *next_part;

		void part_with_shader_release();
		void part_with_shader_load(int release_memory_flag_map);

		inline void exchange_part_with_shader(Part_with_shader *pws)
		{
			CString str;
			double x;
			Part *rp;
			Shader_base *ps;
			bool flag;
			
			str=user_name;					user_name=pws->user_name;								pws->user_name=str;
			str=part_name;					part_name=pws->part_name;								pws->part_name=str;
			str=description_file_name;		description_file_name=pws->description_file_name;		pws->description_file_name=str;
			str=sound_file_name;			sound_file_name=pws->sound_file_name;					pws->sound_file_name=str;

			x=precision;					precision=pws->precision;								pws->precision=x;
			x=access_time;					access_time=pws->access_time;							pws->access_time=x;

			rp=rude_part;					rude_part=pws->rude_part;								pws->rude_part=rp;
			ps=part_shader;					part_shader=pws->part_shader;							pws->part_shader=ps;
	
			flag=has_load_flag;				has_load_flag=pws->has_load_flag;						pws->has_load_flag=flag;
			flag=need_load_flag;			need_load_flag=pws->need_load_flag;						pws->need_load_flag=flag;

			return;
		}

		Part_with_shader(Part_parameter &par,Shader_base *my_part_shader,Part_with_shader *my_next_part);
		~Part_with_shader();

		Part_graphics_object *paint_graphics_objects[5];
		
		int paint_component_number[10],cross_clip_plane_number;
		Component **paint_component_pointer[10];
};

class Part_set
{
	int part_index[MAX_PART_NUMBER],part_merge[MAX_PART_NUMBER];
public:	
	int part_number;
	Part_with_shader *part_array[MAX_PART_NUMBER];

	inline Part_set()
	{
		part_number=0;
		for(int i=0;i<MAX_PART_NUMBER;i++){
			part_array[i]=NULL;
			part_index[i]=0;
		}
	}
	inline virtual ~Part_set()
	{
		for(int i=0;i<MAX_PART_NUMBER;i++)
			if(part_array[i]!=NULL)
				delete (part_array[i]);
		part_number=0;
	}

	Shader_base *add_part(Part_parameter &par);
	inline int get_part_id(CString my_part_name)
	{
		for(int begin_id=0,end_id=part_number-1;begin_id<=end_id;){
			int mid_id	=(begin_id+end_id)/2;
			int mid_part_id	=part_index[mid_id];
			if     ((part_array[mid_part_id]->part_name)<my_part_name)
				begin_id=mid_id+1;
			else if((part_array[mid_part_id]->part_name)>my_part_name)
				end_id=mid_id-1;
			else
				return mid_part_id;
		}
		return -1;
	}
	inline int get_merge_part_id(int part_id)
	{
		if((part_id<0)||(part_id>=part_number))
			return -1;
		else 
			return part_merge[part_id];
	}
	inline int load_part(int release_memory_flag_map,CWnd *disp)
	{
		for(int i=0;i<part_number;i++)
			for(Part_with_shader *p=part_array[i];p!=NULL;p=p->next_part)			
				if(p->need_load_flag){
						Platform_information::stop_timer();
						disp->SetWindowText(_T("正在装入:")+(p->user_name));
						p->part_with_shader_load(release_memory_flag_map);
						disp->SetWindowText(_T("装入完毕:")+(p->user_name));
						Platform_information::start_timer();
						return 1;
					}
		return 0;
	}

	void list_all_part(ofstream &f_part_simple,ofstream &f_part_complete);

	void merge_same_part(CString same_part_file_name);

};

