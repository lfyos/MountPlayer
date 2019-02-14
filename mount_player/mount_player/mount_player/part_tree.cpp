#include "StdAfx.h"
#include <math.h>

#include "const.h"
#include "part_tree.h"
#include "movement_index_of_part.h"
#include "movement_base.h"
#include "part_info.h"
#include "graphics_system.h"
#include "coordinate_computation.h"

BOOL part_tree::compare_tree_node(part_tree *d,int sort_type_id,double min_value)
{
	BOOL flag;
	double x0,y0,z0,x1,y1,z1;

	x0=(	box[0]+	box[3])/2;
	y0=(	box[1]+	box[4])/2;
	z0=(	box[2]+	box[5])/2;

	x1=(d->	box[0]+	d->box[3])/2;
	y1=(d->	box[1]+	d->box[4])/2;
	z1=(d->	box[2]+	d->box[5])/2;

	switch(sort_type_id){
	default:
	case 0:		/*xyz*/
		if(fabsl(x1-x0)>min_value)
			flag=(x0>x1)?TRUE:FALSE;
		else if(fabsl(y1-y0)>min_value)
			flag=(y0>y1)?TRUE:FALSE;
		else 
			flag=(z0>z1)?TRUE:FALSE;
	case 1:	/*xzy*/
		if(fabsl(x1-x0)>min_value)
			flag=(x0>x1)?TRUE:FALSE;
		else if(fabsl(z1-z0)>min_value)
			flag=(z0>z1)?TRUE:FALSE;
		else 
			flag=(y0>y1)?TRUE:FALSE;
		break;
	case 2:	/*yxz*/
		if(fabsl(y1-y0)>min_value)
			flag=(y0>y1)?TRUE:FALSE;
		else if(fabsl(x1-x0)>min_value)
			flag=(x0>x1)?TRUE:FALSE;
		else 
			flag=(z0>z1)?TRUE:FALSE;
		break;
	case 3:	/*yzx*/
		if(fabsl(y1-y0)>min_value)
			flag=(y0>y1)?TRUE:FALSE;
		else if(fabsl(z1-z0)>min_value)
			flag=(z0>z1)?TRUE:FALSE;
		else 
			flag=(x0>x1)?TRUE:FALSE;
		break;
	case 4:	/*zxy*/
		if(fabsl(z1-z0)>min_value)
			flag=(z0>z1)?TRUE:FALSE;
		else if(fabsl(x1-x0)>min_value)
			flag=(x0>x1)?TRUE:FALSE;
		else 
			flag=(y0>y1)?TRUE:FALSE;
		break;
	case 5:	/*zyx*/
		if(fabsl(z1-z0)>min_value)
			flag=(z0>z1)?TRUE:FALSE;
		else if(fabsl(y1-y0)>min_value)
			flag=(y0>y1)?TRUE:FALSE;
		else 
			flag=(x0>x1)?TRUE:FALSE;
		break;
	}
	return flag;
}
part_tree::part_tree(ifstream *f,part_tree *my_parent)
: mark_flag(false)
{
	int i;
	char buf[MAX_MESSAGE_LENGTH];
	CString tmp;

	move=NULL;
	visible_flag=TRUE;
	mark_number=0;
	mark_flag=FALSE;
	selected_flag=FALSE;

	parent=my_parent;
	tree_node_union.assembly.first_child=NULL;

	if(parent==NULL){		
		front_brother=this;
		back_brother=this;
	}else if(parent->tree_node_union.assembly.first_child==NULL){		
		front_brother=this;
		back_brother=this;
		parent->tree_node_union.assembly.first_child=this;
	}else{
		part_tree *front,*back;

		front=parent->tree_node_union.assembly.first_child;
		back=front->back_brother;

		front->back_brother=this;
		back->front_brother=this;

		front_brother=front;
		back_brother=back;
	}

	*f>>buf;

	*f>>location[ 0];	*f>>location[ 1];	*f>>location[ 2];
	*f>>location[ 4];	*f>>location[ 5];	*f>>location[ 6];
	*f>>location[ 8];	*f>>location[ 9];	*f>>location[10];

	*f>>location[12];	*f>>location[13];	*f>>location[14];
	*f>>location[15];

	*f>>location[ 3];	*f>>location[ 7];	*f>>location[11];


	*f>>buf>>buf;	name=buf;
	*f>>buf>>buf;	tree_file_name=buf;

	*f>>buf;	tmp=buf;

	if(tmp=="Part:"){
		type=0;
		tree_node_union.part.transparence_ratio=-1;
		tree_node_union.part.display_box_flag=FALSE;
		tree_node_union.part.bak_display_box_flag=FALSE;
		*f>>tree_node_union.part.part_id;

		tree_node_union.part.move_index_of_part=NULL;

		return;
	}

	type=1;		//tmp=="Assembly:"
	*f>>tree_node_union.assembly.child_number;
	if(tree_node_union.assembly.child_number<=0){
		tree_node_union.assembly.child_number=0;
		return;
	}
	for(i=0;i<tree_node_union.assembly.child_number;i++){
		part_tree *child=new part_tree(f,this);			
		if(child->type!=0)
			if(child->tree_node_union.assembly.child_number==0)
				delete child;
	}
	return;
}
part_tree::~part_tree()
{
	if(type==0){
		if(tree_node_union.part.move_index_of_part!=NULL)
			delete (tree_node_union.part.move_index_of_part);
		tree_node_union.part.move_index_of_part=NULL;
	}else{
		for(;tree_node_union.assembly.first_child!=NULL;)
			delete tree_node_union.assembly.first_child;
	}
	if(parent!=NULL){
		if(front_brother==this)
			parent->tree_node_union.assembly.first_child=NULL;
		else
			parent->tree_node_union.assembly.first_child=front_brother;

		front_brother->back_brother=back_brother;
		back_brother->front_brother=front_brother;

		front_brother=this;
		back_brother=this;
	}
	return;
}

void part_tree::reset_relative_location()
{
	int i,n;
	part_tree *t;
	coordinate_computation c;

	if(parent!=NULL)
		c.caculate_relative_location(parent->location,location,relative_location);
	else
		for(i=0;i<16;i++)
			relative_location[i]=location[i];

	if(type==0)
		return;
	t=tree_node_union.assembly.first_child;
	n=tree_node_union.assembly.child_number;
	for(i=0;i<n;i++,t=t->back_brother)
		t->reset_relative_location();
	return;	
}


void part_tree::caculate_box(part_info **part)
{
	int i,j,k,n;
	part_tree *p;

	if(type!=0){
		p=tree_node_union.assembly.first_child;
		n=tree_node_union.assembly.child_number;
		for(i=0;i<n;i++,p=p->back_brother)
			p->caculate_box(part);
		for(i=0;i<sizeof(box)/sizeof(box[0]);i++)
			box[i]=p->box[i];
		for(i=0;i<n;i++,p=p->back_brother){
			box[0]=(box[0]>(p->box[0]))?(p->box[0]):(box[0]);
			box[1]=(box[1]>(p->box[1]))?(p->box[1]):(box[1]);
			box[2]=(box[2]>(p->box[2]))?(p->box[2]):(box[2]);

			box[3]=(box[3]<(p->box[3]))?(p->box[3]):(box[3]);
			box[4]=(box[4]<(p->box[4]))?(p->box[4]):(box[4]);
			box[5]=(box[5]<(p->box[5]))?(p->box[5]):(box[5]);
		}
		return;
	}
	for(i=0;i<sizeof(box)/sizeof(box[0]);i++)
		box[i]=part[tree_node_union.part.part_id]->box[i];
	part[tree_node_union.part.part_id]->change_to_absulate(
		location,&(box[0]),&(box[1]),&(box[2]));
	part[tree_node_union.part.part_id]->change_to_absulate(
		location,&(box[3]),&(box[4]),&(box[5]));
	for(i=0;i<=3;i=i+3)
		for(j=1;j<=4;j=j+3)
			for(k=2;k<=5;k=k+3){
				double x,y,z;
				x=part[tree_node_union.part.part_id]->box[i];
				y=part[tree_node_union.part.part_id]->box[j];
				z=part[tree_node_union.part.part_id]->box[k];
				part[tree_node_union.part.part_id]->change_to_absulate(location,&x,&y,&z);

				box[0]=(box[0]>x)?x:box[0];
				box[1]=(box[1]>y)?y:box[1];
				box[2]=(box[2]>z)?z:box[2];

				box[3]=(box[3]<x)?x:box[3];
				box[4]=(box[4]<y)?y:box[4];
				box[5]=(box[5]<z)?z:box[5];
			}
	return;
}
void part_tree::get_part_reference_number(int *visible_reference_number, int *hide_reference_number, BOOL current_visible_flag)
{
	int i;
	part_tree *p;

	current_visible_flag=current_visible_flag&visible_flag;
	
	if(type!=0){
		p=tree_node_union.assembly.first_child;
		for(i=0;i<tree_node_union.assembly.child_number;i++,p=p->back_brother)
			p->get_part_reference_number(visible_reference_number,hide_reference_number,current_visible_flag);
		return;
	}

	if(current_visible_flag)
		visible_reference_number[tree_node_union.part.part_id]++;
	else
		hide_reference_number[tree_node_union.part.part_id]++;

	return;
}

CString part_tree::create_name(part_tree * tree[], int tree_number, part_info * part[], int part_number, CString insert_string , BOOL total_number_flag)
{
	int i,j,s,t,*part_index,*visible_part_number,*hide_part_number;
	CString name,tmp;

	part_index			=new int[part_number];
	visible_part_number	=new int[part_number];
	hide_part_number	=new int[part_number];

	for(i=0;i<part_number;i++){
		part_index[i]=i;
		visible_part_number[i]=0;
		hide_part_number[i]=0;
	}

	for(i=0;i<part_number-1;i++)
		for(j=i+1;j<part_number;j++)
			if((part[part_index[i]]->information.name)>(part[part_index[j]]->information.name)){
				s=part_index[i];
				part_index[i]=part_index[j];
				part_index[j]=s;
			}
	for(i=0;i<tree_number;i++)
		tree[i]->get_part_reference_number(visible_part_number,hide_part_number,TRUE);

	for(i=0,t=0,s=0,name=_T("");i<part_number;i++){
		j=part_index[i];
		if((visible_part_number[j]+hide_part_number[j])>0){
			tmp.Format(_T("%d件"),visible_part_number[j]+hide_part_number[j]);
			if(s>0)
				tmp=insert_string+tmp;
			name=name+tmp+part[j]->information.name;
			t++;
			s+=visible_part_number[j]+hide_part_number[j];
		}
	}
	if(s<=0)
		name=_T("没有任何杆件");
	else if(total_number_flag){
		tmp.Format(_T("总计%d种%d件,其中"),t,s);
		name=tmp+insert_string+name;
	}

	delete []hide_part_number;
	delete []visible_part_number;
	delete []part_index;

	return name;
}
void part_tree::draw_cooradiator(graphics_system *gs)
{
	double coordinator_length;
	coordinator_length =(box[0]-box[3])*(box[0]-box[3]);
	coordinator_length+=(box[1]-box[4])*(box[1]-box[4]);
	coordinator_length+=(box[2]-box[5])*(box[2]-box[5]);
	coordinator_length=1.5*sqrtl(coordinator_length);
	gs->display_line(1,0,0,	coordinator_length,0,0);
	gs->display_line(0,1,0,	0,coordinator_length,0);
	gs->display_line(0,0,1,	0,0,coordinator_length);
	return;
}
BOOL part_tree::display_part(int index_id,part_info **part,graphics_system *gs,BOOL transparence_flag,part_image *images[],double *transparence_ratio)
{
	*transparence_ratio=tree_node_union.part.transparence_ratio;
	if(((*transparence_ratio)<=0)||((*transparence_ratio)>=1))
		*transparence_ratio=part[tree_node_union.part.part_id]->material.material_value[7];

	if(((*transparence_ratio)<=0)||((*transparence_ratio)>=1)){
		*transparence_ratio=1.0;
		if(!transparence_flag){
			if(tree_node_union.part.display_box_flag)
				part[tree_node_union.part.part_id]->display_box(index_id,gs);
			else
				part[tree_node_union.part.part_id]->display(index_id,gs);
		}
		return FALSE;
	}
	if(!transparence_flag){
		*transparence_ratio=1.0;
		return FALSE;
	}	
	gs->begin_transparence_display();
	if(tree_node_union.part.display_box_flag)
		part[tree_node_union.part.part_id]->display_box(index_id,gs);
	else
		part[tree_node_union.part.part_id]->display_transparently(index_id,images,gs,*transparence_ratio);
	gs->end_transparence_display();
	return TRUE;
}
int part_tree::display(int index_id,int display_method,
		int part_number,part_info **part,
		part_tree *selected_tree,graphics_system *gs,
		mount_material_property *selected_material_property,
		double *clip_plane_data,
		double eye_x,double eye_y,double eye_z,
		double center_x,double center_y,double center_z,
		double mount_speeed,BOOL my_mark_flag,
		double manual_mount_proportion,
		BOOL transparence_flag,part_image *images[],
		BOOL display_reference_frame_flag)
{
	int i,n,ret_val;
	double move_relative_location[16],transparence_ratio=1.0;;
	part_tree *p;
		
	if(mark_flag)
		my_mark_flag=TRUE;

	gs->push_matrix();
	gs->mult_matrix(relative_location);

	ret_val=0;
	if(move!=NULL){
		ret_val++;
		if(move->get_relative_location(move_relative_location,mount_speeed,manual_mount_proportion))
			if(display_method!=MARK_PART_DISPLAY_METHOD)
				gs->mult_matrix(move_relative_location);
	}
	if(!visible_flag){
		if(display_method!=MARK_PART_DISPLAY_METHOD){
			gs->pop_matrix();
			return ret_val;
		}
	}
	if((selected_flag)&&(display_method<0)){
		display_method=PART_BEING_SELECTED_DISPLAY_METHOD;
		if(display_reference_frame_flag)
			draw_cooradiator(gs);
	}
	if(type!=0){
		p=tree_node_union.assembly.first_child;
		n=tree_node_union.assembly.child_number;
		for(i=0;i<n;i++,p=p->back_brother)
			ret_val+=p->display(index_id,display_method,part_number,part,selected_tree,gs,
						selected_material_property,clip_plane_data,eye_x,eye_y,eye_z,center_x,center_y,center_z,
						mount_speeed,my_mark_flag,manual_mount_proportion,transparence_flag,images,display_reference_frame_flag);
		gs->pop_matrix();
		return ret_val;
	}
	if(my_mark_flag){
		switch(display_method){
		case USER_SELECTED_PART_DISPLAY_METHOD:
			if(selected_tree==NULL)
				part[tree_node_union.part.part_id]->display_selecting_part(index_id,
					tree_node_union.part.part_id,			part_number,
					tree_node_union.part.part_component_id,	tree_node_union.part.part_component_number,
					gs);
			else if(selected_tree==this){
				part[tree_node_union.part.part_id]->display_selecting_face(gs);
				if(clip_plane_data!=NULL)
					gs->display_color(((double)1.05)/((double)200),0,0);
			}
			break;
		case PART_BEING_SELECTED_DISPLAY_METHOD:
			part[tree_node_union.part.part_id]->display_selected_part(index_id,gs,selected_material_property);
			break;
		case MARK_PART_DISPLAY_METHOD:
		default:			
			transparence_flag=display_part(index_id,part,gs,transparence_flag,images,&transparence_ratio);
			if(clip_plane_data!=NULL){
				BOOL flag_bak;

				if(transparence_flag)
					gs->begin_transparence_display();

				flag_bak=part[tree_node_union.part.part_id]->material.texture_flag;
				part[tree_node_union.part.part_id]->material.texture_flag=FALSE;
				gs->display_material(&(part[tree_node_union.part.part_id]->material),transparence_ratio);
				part[tree_node_union.part.part_id]->material.texture_flag=flag_bak;


			}
			break;
		}
		if(clip_plane_data!=NULL){
			gs->begin_display_box_on_clip_plane();
			part[tree_node_union.part.part_id]->display_box_on_clip_plane(
				gs,clip_plane_data,
				eye_x,eye_y,eye_z,
				center_x,center_y,center_z);
			gs->end_display_box_on_clip_plane();
		
			if(transparence_flag)
				gs->end_transparence_display();
		}
	}
	gs->pop_matrix();
	return ret_val;
}
void part_tree::set_transparence(double new_transparence_ratio)
{
	int i,n;
	part_tree *t;

	if(type==0)
		tree_node_union.part.transparence_ratio=new_transparence_ratio;
	else{
		t=tree_node_union.assembly.first_child;
		n=tree_node_union.assembly.child_number;
		if((t!=NULL)&&(n>0)){
			for(i=0;i<n;i++,t=t->back_brother)
				t->set_transparence(new_transparence_ratio);
		}
	}
	return;
}
void part_tree::set_visible_flag(BOOL my_visible_flag,BOOL with_children_flag)
{
	int i,n;
	part_tree *t;

	visible_flag=my_visible_flag;

	if((type!=0)&&with_children_flag){
		t=tree_node_union.assembly.first_child;
		n=tree_node_union.assembly.child_number;
		for(i=0;i<n;i++,t=t->back_brother)
			t->set_visible_flag(my_visible_flag,with_children_flag);
	}
}

BOOL part_tree::get_visible_flag()
{
		return visible_flag;
};	
BOOL part_tree::is_visible()
{
	if(!visible_flag)
		return FALSE;
	if(parent==NULL)
		return TRUE;
	return parent->is_visible();
}
void part_tree::clear_visible_flag()
{
	int i,n;
	part_tree *p;

	visible_flag=TRUE;

	if(type==0)
		return;
	if((p=tree_node_union.assembly.first_child)==NULL)
		return;
	if((n=tree_node_union.assembly.child_number)<=0)
		return;
	for(i=0;i<n;i++,p=p->back_brother)
		p->clear_visible_flag();
	return;
}
int part_tree::get_total_triangle_slice_number(part_info **part)
{
	int i,n;
	part_tree *p;

	if(type==0)
		return part[tree_node_union.part.part_id]->get_total_triangle_slice_number();
	p=tree_node_union.assembly.first_child;
	for(i=0,n=0;i<tree_node_union.assembly.child_number;i++,p=p->back_brother)
		n+=p->get_total_triangle_slice_number(part);
	return n;
}
void part_tree::set_display_box_or_body(BOOL new_display_box_flag,BOOL modify_bak_flag,BOOL modify_invisible_flag)
{
	int i,n;
	part_tree *t;

	if(visible_flag||modify_invisible_flag){
		if(type==0){
			tree_node_union.part.display_box_flag=new_display_box_flag;
			if(modify_bak_flag)
				tree_node_union.part.bak_display_box_flag=new_display_box_flag;
		}else{
			t=tree_node_union.assembly.first_child;
			n=tree_node_union.assembly.child_number;
			if((t!=NULL)&&(n>0))
				for(i=0;i<n;i++,t=t->back_brother)
					t->set_display_box_or_body(new_display_box_flag,modify_bak_flag,modify_invisible_flag);
		}
	}
	return;
}
void part_tree::restore_display_box_or_body()
{
	int i,n;
	part_tree *t;

	if(type==0){
		tree_node_union.part.display_box_flag=tree_node_union.part.bak_display_box_flag;
	}else{
		t=tree_node_union.assembly.first_child;
		n=tree_node_union.assembly.child_number;
		if((t!=NULL)&&(n>0))
			for(i=0;i<n;i++,t=t->back_brother)
				t->restore_display_box_or_body();
	}
	return;
}
part_tree * part_tree::get_node_by_name(CString my_name)
{
	int i,n;
	part_tree *t,*p;

	if(name==my_name)
		return this;
	if(type==0)
		return NULL;
	
	if(name!=my_name.Left(name.GetLength()))
		if(parent!=NULL)
			return NULL;

	t=tree_node_union.assembly.first_child;
	n=tree_node_union.assembly.child_number;

	if((t==NULL)||(n<=0))
		return NULL;
	
	for(i=0;i<n;i++,t=t->back_brother)
		if((p=t->get_node_by_name(my_name))!=NULL)
			return p;
	return NULL;
}
void part_tree::set_part_reference(part_tree *first_part_tree[],int part_number[])
{
	int i,n,part_id;
	part_tree *p,*front,*back;

	if(type!=0){
		p=tree_node_union.assembly.first_child;
		n=tree_node_union.assembly.child_number;
		if((p!=NULL)&&(n>0))
			for(i=0;i<n;i++,p=p->back_brother)
				p->set_part_reference(first_part_tree,part_number);
		return;
	}
	part_id=tree_node_union.part.part_id;

	if((front=first_part_tree[part_id])==NULL){
		first_part_tree[part_id]=this;
		tree_node_union.part.front_part=this;
		tree_node_union.part.back_part=this;
	}else{
		back=front->tree_node_union.part.back_part;

		front->tree_node_union.part.back_part=this;
		back->tree_node_union.part.front_part=this;

		tree_node_union.part.back_part=back;
		tree_node_union.part.front_part=front;
	}

	tree_node_union.part.part_component_id=part_number[part_id];
	tree_node_union.part.part_component_number=part_number[part_id];
	part_number[part_id]++;

	return;
}

part_tree * part_tree::get_selected_node_by_part_tree(HTREEITEM tree_item_id)
{
	int i,n;
	part_tree *t,*p;

	if(structure_tree_id==tree_item_id)
		return this;
	if(type==0)
		return NULL;

	t=tree_node_union.assembly.first_child;
	n=tree_node_union.assembly.child_number;

	if((t==NULL)||(n<=0))
		return NULL;
	
	for(i=0;i<n;i++,t=t->back_brother)
		if((p=t->get_selected_node_by_part_tree(tree_item_id))!=NULL)
			return p;
	return NULL;
}
void part_tree::sort_tree_node(int sort_type_id,double min_value)
{
	int i,j,n;
	part_tree **t,*p;

	if(type==0)
		return;
	
	p=tree_node_union.assembly.first_child;	
	n=tree_node_union.assembly.child_number;

	for(i=0;i<n;i++,p=p->back_brother)
		p->sort_tree_node(sort_type_id,min_value);
	if(n<=1)
		return;
		
	t=new part_tree *[n];
	for(i=0;i<n;i++,p=p->back_brother)
		t[i]=p;
	for(i=0;i<(n-1);i++){
		for(j=i+1;j<n;j++){			
			if(t[i]->compare_tree_node(t[j],sort_type_id,min_value)){
				p=t[i];
				t[i]=t[j];
				t[j]=p;
			}
		}
	}
	for(i=0;i<n;i++){
		t[i]->front_brother=t[(i-1+n)%n];
		t[i]->back_brother =t[(i+1  )%n];
	}
	tree_node_union.assembly.first_child=t[0];

	delete []t;

	return;
}
part_tree * part_tree::get_last_mark_node(BOOL flag)
{
	int i,n;
	part_tree *t,*ret_val,*new_ret_val;

	ret_val=(flag||selected_flag)?this:NULL;

	if(type!=0){
		n=tree_node_union.assembly.child_number;
		t=tree_node_union.assembly.first_child;
		for(i=0;i<n;i++,t=t->back_brother)
			if((new_ret_val=t->get_last_mark_node(flag))!=NULL){
				if(ret_val==NULL)
					ret_val=new_ret_val;
				else if((ret_val->mark_number)<(new_ret_val->mark_number))
						ret_val=new_ret_val;
			}
	}
	return ret_val;
}
void part_tree::set_mark_flag(BOOL new_selected_flag)
{
	static int new_mark_number=0;

	mark_flag=TRUE;
	selected_flag=new_selected_flag;

	if(selected_flag)
		mark_number=new_mark_number++;

	return;
}
void part_tree::clear_mark_flag(void)
{
	int i,n;
	part_tree *t;

	mark_flag=FALSE;
	selected_flag=FALSE;

	if(type!=0){
		t=tree_node_union.assembly.first_child;	
		n=tree_node_union.assembly.child_number;

		for(i=0;i<n;i++,t=t->back_brother)
			t->clear_mark_flag();
	}
	return;
}
void part_tree::clear_movement_index_of_part()
{
	int i,n;
	part_tree *t;

	if(type==0){
		if(tree_node_union.part.move_index_of_part!=NULL)
			delete (tree_node_union.part.move_index_of_part);
		tree_node_union.part.move_index_of_part=NULL;
	}else{
		t=tree_node_union.assembly.first_child;	
		n=tree_node_union.assembly.child_number;
		for(i=0;i<n;i++,t=t->back_brother)
			t->clear_movement_index_of_part();
	}
	return;
}
void part_tree::add_movement_index_of_part(int my_move_index_of_part)
{
	int i,n;
	part_tree *t;

	if(type==0)
		tree_node_union.part.move_index_of_part=new movement_index_of_part(my_move_index_of_part,tree_node_union.part.move_index_of_part);
	else{
		t=tree_node_union.assembly.first_child;	
		n=tree_node_union.assembly.child_number;
		for(i=0;i<n;i++,t=t->back_brother)
			t->add_movement_index_of_part(my_move_index_of_part);
	}
	return;
}
int part_tree::get_min_step(int my_step)
{
	int i,n,ret_val;
	part_tree *p;

	if(type==0){
		 movement_index_of_part *pm;
		 for(pm=tree_node_union.part.move_index_of_part;pm!=NULL;pm=pm->next_movement_index){
			 if((pm->movement_id<my_step)||(my_step<0))
				 my_step=pm->movement_id;
		 }
	}else{
		p=tree_node_union.assembly.first_child;
		n=tree_node_union.assembly.child_number;	
		for(i=0;i<n;i++,p=p->back_brother){
			ret_val=p->get_min_step(my_step);
			if((ret_val<my_step)||(my_step<0))
				my_step=ret_val;
		}
	}	
	return my_step;
}

int part_tree::collect_mark_tree_nodes(int max_buffer_number, part_tree *buffer[])
{
	int i,n,old_max_buffer_number;
	part_tree *p;
	
	if(max_buffer_number<=0)
		return 0;
	if(mark_flag){
		buffer[0]=this;
		return max_buffer_number-1;
	}
	if(type==0)
		return max_buffer_number;

	p=tree_node_union.assembly.first_child;
	n=tree_node_union.assembly.child_number;

	for(i=0;i<n;i++,p=p->back_brother,buffer=buffer+(old_max_buffer_number-max_buffer_number)){
		old_max_buffer_number=max_buffer_number;
		if((max_buffer_number=p->collect_mark_tree_nodes(old_max_buffer_number,buffer))<=0)
			return 0;
	}

	return max_buffer_number;
}

int part_tree::collect_visible_tree_nodes(int max_buffer_number, part_tree *buffer[])
{
	
	
	if(max_buffer_number<=0)
		return 0;
	if(visible_flag){
		if(type==0){
			buffer[0]=this;
			return max_buffer_number-1;
		}
		int i,n,old_max_buffer_number;
		part_tree *p;

		p=tree_node_union.assembly.first_child;
		n=tree_node_union.assembly.child_number;

		for(i=0;i<n;i++,p=p->back_brother,buffer=buffer+(old_max_buffer_number-max_buffer_number)){
			old_max_buffer_number=max_buffer_number;
			if((max_buffer_number=p->collect_visible_tree_nodes(old_max_buffer_number,buffer))<=0)
				return 0;
		}
	}
	return max_buffer_number;
}