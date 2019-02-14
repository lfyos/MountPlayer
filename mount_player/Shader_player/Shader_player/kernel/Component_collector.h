#pragma once

#include "Component_display_parameter.h"

class Component_collector_base
{
	inline void do_collect(Component *comp,bool my_selected_flag)
	{	
		my_selected_flag|=comp->selected_flag;
		if(operate_component(comp,my_selected_flag)){
			Component *p=comp->child;
			if(p!=NULL)
				do{
					do_collect(p,my_selected_flag);
				}while((p=p->left_brother)!=(comp->child));
		}
	}
public:
	Component_collector_base *front,*back;

	int component_number,box_component_number;
	Component **component_pointer;
	Box component_box;

	virtual bool operate_component(Component *comp,bool my_selected_flag)=NULL;

	inline void collect(Component *comp,bool my_selected_flag=false)
	{
		if(comp!=NULL){
			if(comp->parent!=NULL)
				comp->parent->need_do_clip_test_flag=true;
			comp->need_do_clip_test_flag=true;
			do_collect(comp,my_selected_flag);
		}
	}
	inline void register_component(Component *comp)
	{
		if(component_pointer==NULL){
			component_pointer=new Component *[MAX_COMPONENT_NUMBER];
			for(int i=0;i<MAX_COMPONENT_NUMBER;i++)
				component_pointer[i]=NULL;
			component_number=0;
		}
		if(component_number<MAX_COMPONENT_NUMBER){
			component_pointer[component_number]=comp;
			component_number++;
		}
	}
	inline void register_component_box(Component *comp)
	{
		if(comp->component_box_flag){
			if(box_component_number==0)
				component_box=comp->component_box;
			else
				component_box=comp->component_box+component_box;
			box_component_number++;
		}
	}
	inline void clear()
	{
		if(component_pointer!=NULL){
			delete []component_pointer;
			component_pointer=NULL;
		}
		component_number=0;
		box_component_number=0;
	}
	static void clear_all();
	Component_collector_base(void);
	virtual ~Component_collector_base(void);
};

class Component_collector_for_selected_component:public Component_collector_base
{
public:
	inline Component_collector_for_selected_component(Component *comp)
	{
		collect(comp);
	}
	virtual bool operate_component(Component *comp,bool my_selected_flag)
	{
		if(my_selected_flag&&(comp->child==NULL))
			register_component_box(comp);
		if(comp->selected_flag)
			register_component(comp);
		return true;
	}
};

class Part_collector:public Component_collector_base
{
	bool part_less_than(Component *s,Component *t);
public:
	Part_set *ps;
	virtual bool operate_component(Component *comp,bool my_selected_flag);

	void sort();
	inline void copy_from(Part_collector &s)
	{
		if(this==&s)
			return;
		clear();
		ps=s.ps;
		for(int i=0;i<(s.component_number);i++)
			collect(s.component_pointer[i]);
		sort();
	}

	bool get_begin_and_end_part(int part_id,int &begin_index,int &end_index);

	inline Part_collector(Part_set *my_ps)
	{
		ps=my_ps;
	}
	inline Part_collector(Component *comp,Part_set *my_ps)
	{
		ps=my_ps;
		collect(comp);
		sort();
	}
	inline Part_collector(Component_collector_for_selected_component &selected,Part_set *my_ps)
	{
		ps=my_ps;
		for(int i=0;i<(selected.component_number);i++)
			collect(selected.component_pointer[i]);
		sort();
	}
	inline Part_collector(Part_collector &s)
	{
		copy_from(s);
	}	
	inline ~Part_collector()
	{
	};
	CString compare_part_consistency(Part_collector *s);
};

class Component_collector_for_display:public Component_collector_base
{
	bool not_do_lod_flag;
	
	double precision_scale2,minnimal_fovy_tanl2;

	Component_display_parameter *par;
	Component_display_flag *flag;
	
	Select_component_ids *select_ids;

	Box clip_box;
	
	void register_display_component(Part_with_shader *ps,Component *comp,bool my_selected_flag,int matrix_id);
public:
	int return_value;
	int total_part_number,total_display_triangle_number;
	
	Component_collector_for_display(
		double my_precision_scale,bool my_not_do_lod_flag,double my_minnimal_fovy_tanl,
		Component_display_parameter &my_par,Component_display_flag &my_flag,
		Select_component_ids *my_select_ids,Select_component_ids *my_match_ids);
	
	virtual bool operate_component(Component *comp,bool my_selected_flag);

	void display_components(double offset_factor,double offset_units,int line_stipple_factor,int line_stipple_pattern,bool create_mirror_picture_flag=false);
};