#include "StdAfx.h"
#include "movement.h"
#include "const.h"
#include "coordinate_computation.h"
#include "part_tree.h"

movement::movement(ifstream *f)
{
	init_movement();
	load(f);
}
movement::movement(void)
{
	init_movement();
}
void movement::init_movement(void)
{
	name="";
	message="";
	sound_file="";
	main_step=0;
	sub_step=0;

	step_number=0;
	reset_point(1000);

	end_visible_state=TRUE;
	start_time=::GetTickCount();
	tree_node=NULL;

	return;
}

movement::~movement(void)
{
	unlink_from_tree();
}

void movement::reset_point(int point_id, double time_length)
{
	coordinate_computation x;

	    relative_point[point_id].time_length=time_length;
	bak_relative_point[point_id].time_length=time_length;	

	x.load_identity_matrix(    relative_point[point_id].location);
	x.load_identity_matrix(bak_relative_point[point_id].location);
}
void movement::reset_point(double time_length)
{
	int i;	
	for(i=0;i<sizeof(relative_point)/sizeof(relative_point[0]);i++)
		reset_point(i,time_length);
	return;
}
double movement::get_movement_time_length()
{
	int i;
	double time_length;

	for(time_length=0,i=0;i<step_number;i++)
		time_length+=relative_point[i].time_length;

	return time_length;
}
double movement::link_to_tree(part_tree *tree,CString name,double start_t,double start_t_length,BOOL end_state)
{
	unlink_from_tree();

	start_time=start_t;
	start_time_length=start_t_length;
	if((tree_node=tree->get_node_by_name(name))==NULL)
		return 0;

	tree_node->set_display_box_or_body(FALSE,FALSE,TRUE);
	end_visible_state=end_state;
	tree_node->move=this;

	return start_t_length+get_movement_time_length();
}

BOOL movement::get_relative_location(double relative_location[],double mount_speed,double manual_mount_proportion)
{
	int i,j;
	double start_t,end_t,cur,p;

	if((manual_mount_proportion+MIN_VALUE)<1.0){
		start_t=start_time+(start_time_length/mount_speed);	
		cur=(manual_mount_proportion<0)?(double)(::GetTickCount()):start_t+manual_mount_proportion*get_movement_time_length()/mount_speed;
	
		if(cur<start_t){
			for(j=0;j<16;j++)
				relative_location[j]=relative_point[0].location[j];
			return TRUE;
		}
		for(i=0;i<step_number;i++,start_t=end_t){
			end_t=start_t+relative_point[i].time_length/mount_speed;
			if((start_t<=cur)&&(cur<=end_t)){
				p=(cur-start_t)/(end_t-start_t);
				for(j=0;j<16;j++)
					relative_location[j]=(1-p)*relative_point[i].location[j]+p*relative_point[i+1].location[j];
				if(tree_node!=NULL)
					tree_node->set_visible_flag(TRUE);
				return TRUE;
			}
		}
	}
	if(manual_mount_proportion<(-MIN_VALUE))
		unlink_from_tree();
	return FALSE;
}
void movement::unlink_from_tree(void)
{
	if(tree_node==NULL)
		return;	
	tree_node->set_transparence(-1);
	tree_node->set_visible_flag(end_visible_state);
	tree_node->restore_display_box_or_body();
	tree_node->move=NULL;
	tree_node=NULL;	
	return;
}

void movement::reverse(BOOL flag)
{
	int i,j,k;
	double x;

	if(step_number<=0){
		step_number=0;
		return;
	}
	for(i=0;i<=step_number;i++){
		relative_point[i].time_length=bak_relative_point[i].time_length;
		for(j=0;j<sizeof(relative_point[0].location)/sizeof(relative_point[0].location[0]);j++)
			relative_point[i].location[j]=bak_relative_point[i].location[j];
	}
	if(flag){
		for(i=0,j=step_number;i<j;i++,j--){
			x=relative_point[i].time_length;
			relative_point[i].time_length=relative_point[j].time_length;
			relative_point[j].time_length=x;
			for(k=0;k<sizeof(relative_point[0].location)/sizeof(relative_point[0].location[0]);k++){
				x=relative_point[i].location[k];
				relative_point[i].location[k]=relative_point[j].location[k];
				relative_point[j].location[k]=x;
			}
		}
		for(i=0;i<step_number;i++)
			relative_point[i].time_length=relative_point[i+1].time_length;
	}
	return;
}
void movement::copy_from(movement *mov)
{
	int i,j,m,n;

	unlink_from_tree();

	name		=mov->name;
	message		=mov->message;
	sound_file	=mov->sound_file;
	main_step	=mov->main_step;
	sub_step	=mov->sub_step;
	step_number	=mov->step_number;

	m=sizeof(relative_point)/sizeof(relative_point[0]);
	for(i=0;i<m;i++){
		relative_point[i].time_length		=mov->relative_point[i].time_length;
		bak_relative_point[i].time_length	=mov->bak_relative_point[i].time_length;
		n=sizeof(relative_point[i].location)/sizeof(relative_point[i].location[0]);
		for(j=0;j<n;j++){
			relative_point[i].location[j]		=mov->relative_point[i].location[j];
			bak_relative_point[i].location[j]	=mov->bak_relative_point[i].location[j];
		}
	}
	end_visible_state=TRUE;
	start_time=::GetTickCount();
	return;
}
void movement::load(ifstream *f)
{
	int i,j,n;
	char buf[MAX_MESSAGE_LENGTH];

	unlink_from_tree();

	reset_point(1000);

	start_time=0;

	*f>>main_step;
	*f>>sub_step;
	*f>>buf;name=buf;
	*f>>buf;sound_file=buf;
	*f>>buf;message=buf;
	*f>>step_number;

	n=sizeof(relative_point)/sizeof(relative_point[0]);
	if(f->eof()||f->fail())
		step_number=0;
	else if(step_number>=n)
		step_number=n-1;
	else if(step_number<0)
		step_number=0;

	for(i=0;i<step_number;i++){
		*f>>relative_point[i].time_length;		
		bak_relative_point[i].time_length=relative_point[i].time_length;
		for(j=0;j<sizeof(relative_point[i].location)/sizeof(relative_point[i].location[0]);j++){
			*f>>relative_point[i].location[j];
			bak_relative_point[i].location[j]=relative_point[i].location[j];
		}
		if(f->fail()){			
			step_number=i;
			break;
		}
	}
	reset_point(step_number,
		(step_number<=0)?1000:relative_point[step_number-1].time_length);
	return;
}
void movement::flush(ofstream  *f)
{
	int i,j;

	*f<<main_step<<"        "<<sub_step<<"\n";
	*f<<name<<"\n";
	*f<<sound_file<<"\n";
	*f<<message<<"\n";

	*f<<step_number;
	for(i=0;i<step_number;i++){
		*f<<"\n"<<bak_relative_point[i].time_length<<"	";
		for(j=0;j<sizeof(bak_relative_point[0].location)/sizeof(bak_relative_point[0].location[0]);j++)
			*f<<bak_relative_point[i].location[j]<<"	";
	}
	*f<<"\n\n\n";
	return;
}

void movement::replace_move_point(int point_id,double my_time_length,double  a[])
{
	int i,n;

	n=sizeof(relative_point)/sizeof(relative_point[0]);
	if((point_id>=0)&&(point_id<(n-1))){
		relative_point[point_id].time_length=my_time_length;
		bak_relative_point[point_id].time_length=my_time_length;

		n=sizeof(relative_point[0].location)/sizeof(relative_point[0].location[0]);
		for(i=0;i<n;i++){
			relative_point[point_id].location[i]		=a[i];
			bak_relative_point[point_id].location[i]	=a[i];
		}
	}
	return;
}
int movement::add_move_point(CString my_name,CString my_message,CString my_sound_file,int my_main_step,int my_sub_step)
{
	double my_time_length=1000;
	
	name=my_name;
	message=my_message;
	sound_file=my_sound_file;
	main_step=my_main_step;
	sub_step=my_sub_step;

	if(step_number>=(sizeof(relative_point)/sizeof(relative_point[0])))
		return step_number;
	if(step_number<0)
		step_number=0;

	if(step_number<=0)
		my_time_length=1000;
	else
		my_time_length=relative_point[step_number-1].time_length;

	step_number++;

	reset_point(step_number-1,my_time_length);
	reset_point(step_number,my_time_length);

	return step_number;
}