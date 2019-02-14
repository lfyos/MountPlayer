#pragma once

#include "location.h"

class Component_private_data_storage
{
public:
	Component *comp;
	int paint_method_id;
	
	virtual int get_order_parameter(int matrix_id,Location *loca,int &instance_number,float *&order_parameter_data);

	Component_private_data_storage(Component *my_comp)
	{
		comp=my_comp;
		paint_method_id=0;
	}
	virtual ~Component_private_data_storage()
	{
	}
};

class Component
{
public:
	bool need_do_clip_test_flag;

	Location move_location,relative_location;

	HTREEITEM component_list_id[3];

	CString name,id_name;
	Location absulate_location;

	int part_id,component_id,movement_id;	
	int display_flag,hide_line_type_id;//0:hide 1:stipple	2:visible
	double transparence;
	bool selected_flag,frame_display_flag,frame_display_detail_flag;
	double selected_time;

	Component *child,*parent,*left_brother,*right_brother;

	inline Component * get_node_by_name(CString my_name,int depth=0)
	{
		Component *p,*q;
		if(name==my_name)
			return this;
		if(depth>=2)
			if(name!=(my_name.Left(name.GetLength())))
				return NULL;
		if((p=child)!=NULL)
			do{
				if((q=p->get_node_by_name(my_name,depth+1))!=NULL)
					return q;
			}while(child!=(p=p->left_brother));
		return NULL;
	}
	inline Component * get_node_by_id(int my_component_id)
	{
		Component *p,*q;
		if(component_id==my_component_id)
			return this;
		if((p=child)!=NULL)
			do{
				if((q=p->get_node_by_id(my_component_id))!=NULL)
					return q;
			}while(child!=(p=p->left_brother));
		return NULL;
	}
	inline Component * get_node_by_list_id(HTREEITEM my_list_id,int index_id)
	{
		Component *p,*q;
		if(component_list_id[index_id]==my_list_id)
			return this;
		if((p=child)==NULL)
			return NULL;
		do{
			if((q=p->get_node_by_list_id(my_list_id,index_id))!=NULL)
				return q;
		}while(child!=(p=p->left_brother));
		return NULL;
	}
	inline void clear_select_flag()
	{
		Component *p;
		selected_flag=false;	
		if((p=child)==NULL)
			display_flag&=(~1);
		else
			do{
				p->clear_select_flag();
			}while(child!=(p=p->left_brother));
	}
	inline void set_movement_id(int my_movement_id)
	{
		Component *p;
		movement_id=my_movement_id;		
		if((p=child)!=NULL)
			do{
				p->set_movement_id(my_movement_id);
			}while(child!=(p=p->left_brother));
	}
private:
	inline void set_component_selected_flag()
	{
		Component *p;
		if((p=child)==NULL)
			display_flag|=1;
		else
			do{
				p->set_component_selected_flag();
			}while(child!=(p=p->left_brother));
	}
public:
	inline void set_component_selected()
	{
		clear_select_flag();
		selected_time=Platform_information::get_current_time();
		for(Component *p=parent;p!=NULL;p=p->parent)
			if(p->selected_flag)
				return;
		selected_flag=true;
		set_component_selected_flag();
	}
	inline bool is_selected()
	{
		for(Component *p=this;p!=NULL;p=p->parent)
			if(p->selected_flag)
				return true;
		return false;
	}
	inline bool collect_selected_flag()
	{
		Component *p=child;
		if(p!=NULL){
			bool child_selected_flag=true;
			do{
				child_selected_flag&=p->collect_selected_flag();
			}while(child!=(p=p->left_brother));
			if(child_selected_flag)
				set_component_selected();
		}
		return selected_flag;
	}

	Component(ifstream &f,int my_display_flag,Part_set *current_part_array,Component *my_parent=NULL);
    
	inline virtual ~Component(void)
	{
		while(child!=NULL)
			delete child;
		
		if(private_data_storage_pointer!=NULL){
			delete private_data_storage_pointer;
			private_data_storage_pointer=NULL;
		}
		
		if(parent!=NULL){
			if(left_brother==this)
				parent->child=NULL;
			else{
				if(parent->child==this)
					parent->child=left_brother;
				left_brother->right_brother=right_brother;
				right_brother->left_brother=left_brother;
			}
			left_brother=this;
			right_brother=this;
		}
	}
	bool component_box_flag;
	Box component_box,part_box;

	bool get_component_box(Part_set &part_array);

	void compute_absulate_location();	

	void modify_move_absulate_location(Location &new_move_location,Part_set &part_array);

	inline void modify_move_relative_location(Location &modify_move_location,Part_set &part_array)
	{
		modify_move_absulate_location(move_location*modify_move_location,part_array);
	}

	void reset_component(Part_set &part_array,bool first_flag=true);

	void translate_component(double down_x,double down_y,double move_x,double move_y,double screen_z,Point &world_select_flag,Point &local_select_flag,Part_set &part_array,Location &project_matrix);
	void rotate_component(int rotate_type,double down_x,double down_y,double move_x,double move_y,Part_set &part_array,Location &project_matrix,double depth);

	inline int load_component_part(int release_memory_flag_map,Part_set *ps,CWnd *disp)
	{
		if(child!=NULL){
			int ret_val=0;
			Component *p=child;
			do{
				ret_val+=p->load_component_part(release_memory_flag_map,ps,disp);
			}while(child!=(p=p->left_brother));
			return ret_val;
		}
		Part_with_shader *pws;
		if((part_id>=0)&&(part_id<(ps->part_number)))			
			if((pws=ps->part_array[part_id])!=NULL)
				if(pws->rude_part!=NULL)
					if(!(pws->has_load_flag)){
						Platform_information::stop_timer();
						disp->SetWindowText(_T("正在装入:")+(pws->user_name)+_T("......"));
						pws->part_with_shader_load(release_memory_flag_map);
						disp->SetWindowText(_T(""));
						Platform_information::start_timer();
						return 1;
					}
		return 0;
	}
	inline void move_component_into_box(Box b,Part_set &part_array)
	{
		Point p=absulate_location*Point(0,0,0);
		if(b.move_point_into_box(p))
			modify_move_relative_location(Location().move((-absulate_location)*p),part_array);
	}
	Component_private_data_storage *private_data_storage_pointer;
};
