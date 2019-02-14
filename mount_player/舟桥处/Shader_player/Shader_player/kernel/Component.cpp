#include "StdAfx.h"
#include "Component.h"

int Component_private_data_storage::get_order_parameter(int matrix_id,Location *loca,int &instance_number,float *&order_parameter_data)
{
	static float a[20];

	order_parameter_data=a;

	order_parameter_data[0]	=(float)0.25+(float)paint_method_id;
	order_parameter_data[1]	=(float)0.25+(float)matrix_id;
	order_parameter_data[2]	=(float)0.25+(float)(comp->component_id);
	order_parameter_data[3]	=(float)(comp->transparence);

	if(loca==NULL)
		loca=&(comp->absulate_location);
	loca->get_location_float_data(&(order_parameter_data[4]));

	instance_number=1;

	return 20;
}

Component::Component(ifstream &f,int my_display_flag,Part_set *current_part_array,Component *my_parent)
{
	int i,child_number;
	char buffer[MAX_BUFFER_LENGTH];
	
	static int component_counter=2;

	hide_line_type_id=0;//0:hide 1:stipple	2:visible
	frame_display_detail_flag=false;

	if((component_counter<2)||(my_parent==NULL))
		component_counter=2;
	component_id=component_counter++;

	if(component_id>=MAX_COMPONENT_NUMBER){
		component_counter=2;
		CString str;
		str.Format(_T("系统最多支持的杆件数量为%d件\n装入装配体时杆件编号已超出该最大值\n"),MAX_COMPONENT_NUMBER);
		AfxMessageBox(str);
	}
	movement_id=-1;

	for(i=0;i<(sizeof(component_list_id)/sizeof(component_list_id[0]));i++)
		component_list_id[i]=NULL;
	
	child=NULL;
	parent=my_parent;
	display_flag=(~0);
	
	transparence=1.0;
	
	selected_flag=false;
	frame_display_flag=false;
	
	selected_time=0;

	f>>buffer>>buffer;			name=buffer;
	f>>buffer>>buffer;			id_name=buffer;

	f>>buffer;	
	relative_location=Location(f);
	move_location=Location();
	
	if(parent!=NULL){
		if(parent->child==NULL){
			parent->child=this;
			left_brother=this;
			right_brother=this;
		}else{
			left_brother=parent->child;
			right_brother=left_brother->right_brother;

			left_brother->right_brother=this;
			right_brother->left_brother=this;
		}
	}

	f>>buffer>>child_number;
	if(f.eof())
		child_number=0;

	part_id=current_part_array->get_part_id(id_name);
	part_id=current_part_array->get_merge_part_id(part_id);
	
	if(child_number<=0){
		child_number=0;
		if(part_id<0)
			private_data_storage_pointer=NULL;
		else
			private_data_storage_pointer=current_part_array->part_array[part_id]->part_shader->create_component_private_data_storage(this,f);
		if(private_data_storage_pointer==NULL)
			private_data_storage_pointer=new Component_private_data_storage(this);
	}else{
		private_data_storage_pointer=new Component_private_data_storage(this);
		for(i=0;i<child_number;i++)
			new Component(f,my_display_flag,current_part_array,this);
	}
	if(part_id>=0){
		while(child!=NULL)
			delete child;
		display_flag=my_display_flag;
	}
}

bool Component::get_component_box(Part_set &part_array)
{	
	if(child!=NULL){
		int i=0,j=0;
		Component *p_com=child;
		Box b;
		do{
			if(p_com->get_component_box(part_array)){
				if((i++)==0)
					component_box=p_com->component_box;
				else
					component_box=component_box+(p_com->component_box);
			}
			if((j++)==0)
				b=(p_com->component_box);
			else
				b=b+(p_com->component_box);

		}while ((p_com=p_com->left_brother)!=child);

		if(i>0){
			part_box=component_box;
			if(i==j)
				component_box_flag=true;
			else
				component_box_flag=false;
			return component_box_flag;
		}
		component_box_flag=false;
		part_box=b;
		component_box=b;
		return false;
	}

	Part_with_shader *ps;
	if((part_id>=0)&&(part_id<part_array.part_number))
		if((ps=part_array.part_array[part_id])!=NULL)
			if(ps->has_load_flag){				
				component_box_flag=ps->part_shader->get_part_information()->shader_has_box_flag;
				if(component_box_flag){
					part_box=ps->rude_part->box;
					component_box=absulate_location*part_box;
					return true;
				}
			}

	component_box_flag=false;
	part_box=Box();
	component_box=absulate_location*part_box;
	
	return false;
}

void Component::compute_absulate_location()
{
	Component *p;
	
	if(parent==NULL)
		absulate_location=relative_location*move_location;
	else
		absulate_location=(parent->absulate_location)*relative_location*move_location;

	if((p=child)!=NULL)
		do{
			p->compute_absulate_location();
		}while(child!=(p=p->left_brother));
	return;
}

void Component::modify_move_absulate_location(Location &new_move_location,Part_set &part_array)
{
	move_location=new_move_location;
	compute_absulate_location();
	get_component_box(part_array);
	
	for(Component *p=parent;p!=NULL;p=p->parent){
		int i=0,j=0;
		Component *c=p->child;
		Box b;

		do{
			if(c->component_box_flag){
				if((i++)==0)
					p->component_box=c->component_box;
				else
					p->component_box=(p->component_box)+(c->component_box);	
			}
			if((j++)==0)
				b=c->component_box;
			else
				b=c->component_box+b;

		}while((c=c->left_brother)!=(p->child));

		if(i<=0){
				p->component_box_flag=false;
				p->component_box=b;
		}else if(i==j)
				p->component_box_flag=true;
		else
				p->component_box_flag=false;
		p->part_box=p->component_box;
	}
}
void Component::reset_component(Part_set &part_array,bool first_flag)
{
	Component *p;

	move_location=Location();
	if((p=child)!=NULL){
		do{
			p->reset_component(part_array,false);
		}while (child!=(p=p->left_brother));
	}
	if(first_flag)
		modify_move_absulate_location(Location(),part_array);
}
void Component::translate_component(double down_x,double down_y,double move_x,double move_y,double screen_z,Point &world_select_flag,Point &local_select_flag,Part_set &part_array,Location &project_matrix)
{
	Location loca(-absulate_location);
	Point down_point=Graphics_system::screen_to_object(Point(down_x,down_y,screen_z),project_matrix);
	Point move_point=Graphics_system::screen_to_object(Point(move_x,move_y,screen_z),project_matrix);

	move_point=Point(down_point.x+(move_point.x-down_point.x)*(world_select_flag.x),down_point.y+(move_point.y-down_point.y)*(world_select_flag.y),down_point.z+(move_point.z-down_point.z)*(world_select_flag.z));
	Point distance=(loca*move_point)-(loca*down_point);
	loca=Location::move((distance.x)*(local_select_flag.x),(distance.y)*(local_select_flag.y),	(distance.z)*(local_select_flag.z));

	modify_move_relative_location(loca,part_array);
}
void Component::rotate_component(int rotate_type,double down_x,double down_y,double move_x,double move_y,Part_set &part_array,Location &project_matrix,double depth)
{
	Point p0=absulate_location*Point(0,0,0);

	Point down_point=Graphics_system::screen_to_object(Point(down_x,down_y,depth),project_matrix);
	Point move_point=Graphics_system::screen_to_object(Point(move_x,move_y,depth),project_matrix);

	Location loca=(-absulate_location);
	switch(rotate_type){
	default:
		return;
	case 0:
		down_point=loca*down_point;
		move_point=loca*move_point;
		break;
	case 1:
		down_point=loca*down_point;
		move_point=loca*move_point;
		down_point.x=0;
		move_point.x=0;
		break;
	case 2:
		down_point=loca*down_point;
		move_point=loca*move_point;
		down_point.y=0;
		move_point.y=0;
		break;
	case 3:
		down_point=loca*down_point;
		move_point=loca*move_point;
		down_point.z=0;
		move_point.z=0;
		break;
	case 4:
		down_point.x=p0.x;
		move_point.x=p0.x;
		down_point=loca*down_point;
		move_point=loca*move_point;
		break;
	case 5:
		down_point.y=p0.y;
		move_point.y=p0.y;
		down_point=loca*down_point;
		move_point=loca*move_point;
		break;
	case 6:
		down_point.z=p0.z;
		move_point.z=p0.z;
		down_point=loca*down_point;
		move_point=loca*move_point;
		break;
	}
	
	Point old_x=Point(1,0,0),new_x;
	Point old_y=Point(0,1,0),new_y;
	Point old_z=Point(0,0,1),new_z;
	Point old_c=Point(0,0,0),new_c=Point(0,0,0);


	Location rotate_matrix=Location::rotation(down_point,move_point);
	new_z=rotate_matrix*old_z;
	new_y=rotate_matrix*old_y;

	new_y=new_y.expand(1);
	new_x=new_y.cross(new_z).expand(1);
	new_z=new_x.cross(new_y).expand(1);

	Location a(old_x,old_y,old_z,old_c),b(new_x,new_y,new_z,new_c);

	modify_move_relative_location(b*(-a),part_array);
}