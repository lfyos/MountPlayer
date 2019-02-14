#include "StdAfx.h"
#include "mount_processor.h"
#include "resource.h"
#include "mount_sound_player.h"
#include "mount_player.h"
#include "fstream"
#include "movement.h"
#include "mount_view.h"
#include "mount_bridge.h"
#include "part_tree.h"
#include "part_info.h"



mount_processor::mount_processor(CString movement_file_name)
{
	int i;

	move_parts_box[0]=0;
	move_parts_box[1]=0;
	move_parts_box[2]=0;
	move_parts_box[3]=1;
	move_parts_box[4]=1;
	move_parts_box[5]=1;

	for(i=0;i<MAX_MOVEMENT_STEP;i++)
			move[i]=NULL;
	move_number=0;

	start_time=(double)::GetTickCount();
	time_length=1000;
	current_step=0;
	target_step=0;

	load_movement(movement_file_name);

	modify_sub_step=0;
	if(move_number<=0){
		cache_main_step=100;
		cache_sub_step=100;
	}else{
		cache_main_step=move[0]->main_step-1;
		cache_sub_step=100;
	}
	return;
}
mount_processor::~mount_processor(void)
{
	int i;
	for(i=0;i<MAX_MOVEMENT_STEP;i++)
		if(move[i]!=NULL){
			delete (move[i]);
			move[i]=NULL;
		}	
	move_number=0;
	return;
}
void mount_processor::sort_movement(void)
{
	int i,j;
	for(i=0;i<(move_number-1);i++)
		for(j=i+1;j<move_number;j++){
			BOOL flag;
			if(move[i]->main_step>move[j]->main_step)
				flag=TRUE;
			else if(move[i]->main_step<move[j]->main_step)
				flag=FALSE;
			else if(move[i]->sub_step>move[j]->sub_step)
				flag=TRUE;
			else 
				flag=FALSE;
			if(flag){
				movement *p;
				p=move[i];
				move[i]=move[j];
				move[j]=p;
			}
		}
	return;
}
void mount_processor::load_movement(CString movement_file_name)
{
	int i;
	ifstream f(movement_file_name);

	should_flush_movement_flag=FALSE;
	bak_movement_file_name=movement_file_name;

	move_number=0;
	for(i=0;i<MAX_MOVEMENT_STEP;i++)
		if(move[i]!=NULL){
			delete (move[i]);
			move[i]=NULL;
		}	
	for(i=0;i<MAX_MOVEMENT_STEP;i++){
		move[i]=new movement(&f);
		if(f.eof()||f.fail()){
			delete move[i];
			move_number=i;
			move[i]=NULL;
			break;
		}
	}
	f.close();

	if(current_step<0)
		current_step=0;
	if(current_step>move_number)
		current_step=move_number;

	sort_movement();

	bridge_view->bridge->tree->clear_visible_flag();
	initialize_visible_state_and_movement_index();

	return;
}
void mount_processor::flush_movement(CString movement_file_name)
{
	int i;
	ofstream  f(movement_file_name);

	sort_movement();
	for(i=0;i<move_number;i++)
			move[i]->flush(&f);
	f.close();
	
	should_flush_movement_flag=FALSE;

	return;
}
int mount_processor::process_mount(double start_time_length,int *start_step_id,int *end_step_id,mount_sound_player *player,CString directory_name)
{
	double cur_time_length;
	int i,return_value,main_step,sub_step;
	BOOL direction_flag;
	part_tree *t;
	
	if((double)::GetTickCount()<(start_time+(time_length/(bridge_view->speed))))
		return -1;

	if(move_number<=0){
		move_number=0;
		return -1;
	}
	if(move_number>(sizeof(move)/sizeof(move[0])))
		move_number=sizeof(move)/sizeof(move[0]);
	if(current_step<0)
		current_step=0;
	if(current_step>move_number)
		current_step=move_number;

	if(current_step==target_step)
		return -1;
	
	if(current_step<target_step){
		if(current_step==move_number){
			target_step=current_step;
			return -1;
		}
		direction_flag=TRUE;
	}else if(current_step>target_step){
		if(current_step==0){
			target_step=0;
			return -1;
		}
		direction_flag=FALSE;
		current_step--;
	}

	if(player!=NULL){
		CString sound_file=directory_name+move[current_step]->sound_file;
		ifstream f(sound_file);
		if(!(f.fail())){
			f.close();
			if(player->play(sound_file)){
				if(!direction_flag)
					current_step++;
				return -1;
			}
		}
	}
	start_time=::GetTickCount();
	time_length=0;
	main_step=move[current_step]->main_step;
	sub_step=move[current_step]->sub_step;
	*start_step_id=current_step;
	*end_step_id=current_step;

	bridge_view->bridge->tree->clear_mark_flag();

	for(return_value=0;;){
		*end_step_id=current_step;
		move[current_step]->reverse(direction_flag?FALSE:TRUE);
		cur_time_length=move[current_step]->link_to_tree(
					bridge_view->bridge->tree,move[current_step]->name,
					start_time,start_time_length,direction_flag);
		if((t=move[current_step]->tree_node)==NULL){
			delete (move[current_step]);
			move_number--;
			for(i=current_step;i<move_number;i++)
				move[i]=move[i+1];
			move[move_number]=NULL;
			
			should_flush_movement_flag=TRUE;

			if(direction_flag)
				current_step--;
			else 
				current_step++;
		}else{
			if(time_length<cur_time_length)
				time_length=cur_time_length;
			return_value++;
			if(return_value==1){
				move_parts_box[0]=t->box[0];
				move_parts_box[1]=t->box[1];
				move_parts_box[2]=t->box[2];
				move_parts_box[3]=t->box[3];
				move_parts_box[4]=t->box[4];
				move_parts_box[5]=t->box[5];
			}else{
				move_parts_box[0]=(move_parts_box[0]<t->box[0])?move_parts_box[0]:t->box[0];
				move_parts_box[1]=(move_parts_box[1]<t->box[1])?move_parts_box[1]:t->box[1];
				move_parts_box[2]=(move_parts_box[2]<t->box[2])?move_parts_box[2]:t->box[2];
				move_parts_box[3]=(move_parts_box[3]>t->box[3])?move_parts_box[3]:t->box[3];
				move_parts_box[4]=(move_parts_box[4]>t->box[4])?move_parts_box[4]:t->box[4];
				move_parts_box[5]=(move_parts_box[5]>t->box[5])?move_parts_box[5]:t->box[5];
			}
			t->set_mark_flag(FALSE);
		}		
		if(direction_flag){
			current_step++;
			if(current_step>=move_number)
				break;
		}else{
			if(current_step<=0)
				break;
			current_step--;
		}
		if((main_step!=move[current_step]->main_step)||(sub_step!=move[current_step]->sub_step)){
			if(!direction_flag)
				current_step++;
			break;
		}
	}

	if((*start_step_id)>(*end_step_id)){
		i=(*start_step_id);
		(*start_step_id)=(*end_step_id);
		(*end_step_id)=i;
	}
	return return_value;
}
void mount_processor::advance_one_step(void)
{
	int main_step,sub_step;

	start_time=(double)::GetTickCount();
	time_length=0;
	if(current_step>=move_number)
		return;
	main_step=move[current_step]->main_step;
	sub_step=move[current_step]->sub_step;
	target_step=current_step+1;
	for(;target_step<move_number;target_step++){
		if(main_step!=(move[target_step]->main_step))
			break;
		if(sub_step!=(move[target_step]->sub_step))
			break;
	}
	return;
}
void mount_processor::retreat_one_step(void)
{
	int main_step,sub_step;

	start_time=(double)::GetTickCount();
	time_length=0;
	if(current_step<=0)
		return;
	main_step=move[current_step-1]->main_step;
	sub_step=move[current_step-1]->sub_step;
	target_step=current_step-1;
	for(;target_step>=0;target_step--){
		if(main_step!=(move[target_step]->main_step)){
			target_step++;
			break;
		}
		if(sub_step!=(move[target_step]->sub_step)){
			target_step++;
			break;
		}
	}
	return;
}
void mount_processor::advance_big_step(void)
{
	int main_step;

	start_time=(double)::GetTickCount();
	time_length=0;
	if(current_step>=move_number)
		return;
	main_step=move[current_step]->main_step;	
	for(target_step=current_step+1;target_step<move_number;target_step++)
		if(main_step!=(move[target_step]->main_step))
			break;
	return;
}
void mount_processor::retreat_big_step(void)
{
	int main_step;

	start_time=(double)::GetTickCount();
	time_length=0;
	if(current_step<=0)
		return;
	main_step=move[current_step-1]->main_step;
	for(target_step=current_step-1;target_step>=0;target_step--)
		if(main_step!=(move[target_step]->main_step)){
			target_step++;
			break;
		}
	return;
}
void mount_processor::advance_forever(void)
{
	start_time=(double)::GetTickCount();
	time_length=0;
	target_step=1+sizeof(move)/sizeof(move[0]);
	return;
}

void mount_processor::retreat_forever(void)
{
	start_time=(double)::GetTickCount();
	time_length=0;
	target_step=-1;
	return;
}
void mount_processor::stop_mount(void)
{
	int i;
	for(i=0;i<move_number;i++)
			move[i]->unlink_from_tree();
	start_time=(double)::GetTickCount();
	time_length=0;
	target_step=current_step;
	return;
}
void mount_processor::initialize_visible_state_and_movement_index()
{
	int i,j,*part_index,part_number;
	part_tree *t;
		
	part_number=bridge_view->bridge->part_number;
	part_index=new int[part_number];

	for(i=0;i<part_number;i++)
		part_index[bridge_view->bridge->part_index[i]]=i;

	if(current_step<0)
		current_step=0;
	else if(current_step>move_number)
		current_step=move_number;

	bridge_view->bridge->tree->clear_movement_index_of_part();
	for(i=0;i<move_number;i++)
		move[i]->clear_part_index_of_movement();
		
	for(i=move_number-1;i>=0;i--){		
		if((t=bridge_view->bridge->tree->get_node_by_name(move[i]->name))!=NULL){
			t->set_visible_flag(i<current_step?TRUE:FALSE);
			t->add_movement_index_of_part(i);
			for(j=i-1;j>=0;j--){
				if((move[i]->main_step!=move[j]->main_step)||(move[i]->sub_step!=move[j]->sub_step))
					break;
			}
			move[j+1]->add_part_index_of_movement(t,part_index);
		}
	}
	delete []part_index;

	return;
}
void mount_processor::jump_step(int step_id)
{
	part_tree *t;

	int main_step,sub_step;

	if(step_id<0)
		step_id=0;
	else if(step_id>=move_number)
		step_id=move_number;
	else{
		main_step=bridge_mounter->move[step_id]->main_step;
		sub_step=bridge_mounter->move[step_id]->sub_step;
		for(;;step_id--){
			if((step_id<=0)||(step_id>=move_number))
				break;
			if(main_step!=bridge_mounter->move[step_id-1]->main_step)
				break;
			if(sub_step!=bridge_mounter->move[step_id-1]->sub_step)
				break;
		}
	}	
	for(;current_step<step_id;current_step++){
		if(move[current_step]!=NULL)
			if((t=bridge_view->bridge->tree->get_node_by_name(move[current_step]->name))!=NULL)
				t->set_visible_flag(TRUE);
	}
	for(;current_step>step_id;current_step--){
		if(move[current_step]!=NULL)
			if((t=bridge_view->bridge->tree->get_node_by_name(move[current_step]->name))!=NULL)
				t->set_visible_flag(FALSE);
	}
	current_step=step_id;
	target_step=step_id;

	if((t=bridge_view->bridge->tree->get_node_by_name(move[current_step]->name))!=NULL)
			t->set_visible_flag(FALSE);
	return;
}
void mount_processor::set_box(double new_box[])
{
	int i;
	for(i=0;i<sizeof(move_parts_box)/sizeof(move_parts_box[0]);i++)
		move_parts_box[i]=new_box[i];
	return;
}

void mount_processor::add_movement(movement * m)
{
	should_flush_movement_flag=TRUE;

	if(move_number>=sizeof(move)/sizeof(move[0]))
		delete m;
	else{
		move[move_number]=m;
		move_number++;
		sort_movement();
		for(current_step=0;current_step<move_number;current_step++)
			if(move[current_step]==m){
				for(;current_step>0;current_step--){
					if(move[current_step-1]->main_step!=m->main_step)
						break;
					if(move[current_step-1]->sub_step!=m->sub_step)
						break;
				}
				target_step=current_step;
				break;
			}
		initialize_visible_state_and_movement_index();
	}
	return;
}

void mount_processor::delete_movement(int my_main_step, int my_sub_step)
{
	int i,j;
	part_tree *t;

	should_flush_movement_flag=TRUE;

	for(i=0,j=0;i<move_number;i++){
		if(move[i]->main_step!=my_main_step){
			move[j]=move[i];
			j++;
		}else if((my_sub_step<0)||(move[i]->sub_step==my_sub_step)){
			if(i>=current_step)
				if((t=bridge_view->bridge->tree->get_node_by_name(move[i]->name))!=NULL)
					t->set_visible_flag(TRUE,FALSE);
			delete (move[i]);
		}else {
			move[j]=move[i];
			j++;
		}
	}
	move_number=j;
	i=sizeof(move)/sizeof(move[0]);
	for(;j<i;j++)
		move[j]=NULL;

	sort_movement();
	initialize_visible_state_and_movement_index();

	return;
}

void mount_processor::replace_message(int my_main_step, CString my_message )
{
	int i;

	should_flush_movement_flag=TRUE;

	for(i=0;i<move_number;i++)
		if(move[i]->main_step==my_main_step)
			move[i]->message=my_message;
	return;
}

void mount_processor::main_step_re_sort(int main_step_base,int main_step_step)
{
	int i,j,my_main_step;

	should_flush_movement_flag=TRUE;

	sort_movement();
	for(i=0;i<move_number;main_step_base+=main_step_step,i=j)
		for(j=i,my_main_step=move[i]->main_step;(j<move_number)&&(move[j]->main_step==my_main_step);j++)
			move[j]->main_step=main_step_base;
	sort_movement();

	return;
}

void mount_processor::sub_step_re_sort(int my_main_step, int my_step_step)
{
	int i,j,my_sub_step,my_sub_step_base;

	should_flush_movement_flag=TRUE;

	sort_movement();

	for(i=0;i<move_number;i++)
		if(move[i]->main_step==my_main_step){
			for(my_sub_step_base=my_step_step;;my_sub_step_base+=my_step_step,i=j){
				if(i>=move_number)
					return;
				if(move[i]->main_step!=my_main_step)
					return;
				my_sub_step=move[i]->sub_step;
				for(j=i;;j++){
					if(j>=move_number)
						break;
					if(move[j]->main_step!=my_main_step)
						break;
					if(move[j]->sub_step!=my_sub_step)
						break;
					move[j]->sub_step=my_sub_step_base;
				}
			}
			return;
		}
	return;
}

void mount_processor::move_movement(int begin_step, int end_step, int target_step)
{
	int i;

	should_flush_movement_flag=TRUE;

	sort_movement();

	if(begin_step>end_step){
		i=begin_step;
		begin_step=end_step;
		end_step=i;
	}
	if(target_step<begin_step){
		i=end_step;
		end_step=begin_step-1;
		begin_step=target_step;
		target_step=i+1;
	}
	for(i=0;i<move_number;i++){
		if((move[i]->main_step>=begin_step)&&(move[i]->main_step<=end_step))
			move[i]->main_step+=target_step-(end_step+1);
		else if((move[i]->main_step>end_step)&&(move[i]->main_step<target_step))
			move[i]->main_step-=(end_step+1)-begin_step;
	}
	sort_movement();
	initialize_visible_state_and_movement_index();

	return;
}

void mount_processor::combine_step(int begin_step_id, int end_step_id)
{

	int i,j,main_step_id,sub_step_id;

	should_flush_movement_flag=TRUE;

	sort_movement();

	for(i=0,j=0;i<move_number;){
		main_step_id=move[i]->main_step;		
		if((main_step_id<begin_step_id)||(main_step_id>end_step_id))
			i++;
		else{
			j++;
			sub_step_id=move[i]->sub_step;
			for(;i<move_number;i++){
				if((move[i]->main_step!=main_step_id)||(move[i]->sub_step!=sub_step_id))
					break;
				move[i]->main_step=begin_step_id;
				move[i]->sub_step=j;
			}
		}
	}
}
void mount_processor::separate_step(int main_step_id, int sub_step_id)
{
	int i,j;

	should_flush_movement_flag=TRUE;

	sort_movement();

	for(i=0;i<move_number;i++){
		if((move[i]->main_step==main_step_id)&&(move[i]->sub_step==sub_step_id)){
			for(j=i+1;j<move_number;j++){
				if(move[j]->main_step>main_step_id){
						if(move[j]->main_step==(main_step_id+1))
							return;
						j=(move[j]->main_step+main_step_id)/2;
						for(;i<move_number;i++)
							if(move[i]->main_step==main_step_id)
								move[i]->main_step=j;
						return;
				}
			}
			for(;i<move_number;i++)
					move[i]->main_step++;
			return;
		}
	}
	return;
}

void mount_processor::mount_progress_table(CString file_name,BOOL flag)
{
	int i,j,k,main_step_id,*visible_number,*hide_number;
	CString str,message_str,sound_str;
	ofstream  f(file_name);
	part_tree *t;

	visible_number=new int[bridge_view->bridge->part_number];
	hide_number=new int[bridge_view->bridge->part_number];


	for(i=0;i<move_number;){
		main_step_id=move[i]->main_step;
		message_str=move[i]->message;
		sound_str.Format("sound\\%d.mp3",main_step_id);

		if(flag)
			str.Format("拼装步骤：%d\n",main_step_id);
		f<<str<<"配音文件："<<sound_str<<"\n";
		
		
		for(j=0;j<bridge_view->bridge->part_number;j++){
			visible_number[j]=0;
			hide_number[j]=0;
		}
		for(;i<move_number;i++){
			if(main_step_id!=(move[i]->main_step))
				break;
			move[i]->message=message_str;
			move[i]->sound_file=sound_str;
			if((t=bridge_view->bridge->tree->get_node_by_name(move[i]->name))!=NULL)
				t->get_part_reference_number(visible_number,hide_number,TRUE);
		}
		if(flag){
			f<<"拼装杆件：\n";
			for(j=0;j<bridge_view->bridge->part_number;j++){
				k=bridge_view->bridge->part_index[j];
				if((visible_number[k]+hide_number[k])>0){
					str.Format("    %4d件",visible_number[k]+hide_number[k]);
					f<<str<<bridge_view->bridge->part[k]->information.name<<"\n";
				}
			}
		}
		f<<message_str<<"\n\n";
	}
	f.close();

	delete []hide_number;
	delete []visible_number;

	return;
}