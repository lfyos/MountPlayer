#include "StdAfx.h"
#include "Movement_processor.h"
#include "..\kernel\Sound_player.h"

#include "..\Shader_player.h"
#include "..\MainFrm.h"

bool Movement_processor::movement_greater(int i,int j)
{
	if((move[i]->main_step)>(move[j]->main_step))
		return true;
	if((move[i]->main_step)<(move[j]->main_step))
		return false;
	if((move[i]->sub_step)>(move[j]->sub_step))
		return true;
	if((move[i]->sub_step)<(move[j]->sub_step))
		return false;
	return true;
}
void Movement_processor::release_movement()
{
	for(move_number=0;move_number<MAX_MOVEMENT_NUMBER;move_number++)
		if(move[move_number]!=NULL){
			delete (move[move_number]);
			move[move_number]=NULL;
		}
	move_number=0;
}
void Movement_processor::load(CString file_name)
{
	release_movement();

	if(file_name==_T(""))
		file_name=movement_file_name;
	ifstream f(ws->plat_info->directory_name+file_name);
	
	move_number=0;
	for(;move_number<MAX_MOVEMENT_NUMBER;){
		move[move_number]=new Movement(f,ws);
		if(f.eof()||f.fail()){
			delete (move[move_number]);
			move[move_number]=NULL;
			break;
		}
		move_number++;
	}
	if(current_step>=move_number)
		current_step=move_number;
	if(current_step<0)
		current_step=0;
	sort();
	{
		ofstream f(ws->plat_info->directory_name+_T("temp\\not_find_movement.lfy"));
		for(int i=0;i<move_number;i++){
			if(move[i]!=NULL){
				if(move[i]->comp==NULL){
					CString str;
					str.Format(_T("\n%d	%d			"),move[i]->main_step,move[i]->sub_step);
					File_data::output_string(f,str+(move[i]->name));
				}
			}
		}
		f.close();
	}
}
void Movement_processor::sort(int add_step_number)
{
	int i,j,k;

	if(add_step_number<0){
		for(i=1;i<move_number;i++){
			for(j=i;j>0;j=k){
				k=(j-1)/2;
				if(movement_greater(k,j))
					break;
				Movement *p=move[j];
				move[j]=move[k];
				move[k]=p;
			}		
		}	
		for(i=move_number-1;i>0;i--){
			Movement *p=move[0];
			move[0]=move[i];
			move[i]=p;
			for(j=0;;j=k){
				if((k=j+j+1)>=i)
					break;
				if((k+1)<i)
					if(movement_greater(k+1,k))
						k++;
				if(movement_greater(j,k))
					break;
				p=move[j];
				move[j]=move[k];
				move[k]=p;
			}
		}
	}else{
		for(i=move_number-1,j=move_number-add_step_number;i>=0;i--)
			move[i+j]=move[i];
		for(i=0;i<j;i++){
			move[i]=move[i+move_number];
			move[i+move_number]=NULL;
		}
		for(i=j-1;i>=0;i--){
			for(j=i+1;j<move_number;j++){
				if(movement_greater(j,j-1))
					break;
				Movement *p=move[j];
				move[j]=move[j-1];
				move[j-1]=p;
			}
		}
	}
	start_time=0;
	state_id=0;
	reset_visible_state();
	ws->root_component->set_movement_id(-1);
	for(i=0;i<move_number;i++){
		if(move[i]->comp!=NULL)
			move[i]->comp->set_movement_id(i);
	}
}

Movement_processor::Movement_processor(Whole_system *my_ws,CString my_movement_file_name)
{
	do_push_flush_movement_flag=true;


	movement_file_name=my_movement_file_name;

	for(move_number=0;move_number<MAX_MOVEMENT_NUMBER;move_number++)
		move[move_number]=NULL;
	move_number=0;
	
	current_step=0;
	start_time=0;
	
	state_id=0;
	back_stack_number=0;

	ws=my_ws;
	return;
}
double Movement_processor::compute_start_and_and_step(int step,int &start_step,int &end_step)
{
	if(step<=0)
			step=0;		
	if(step>=move_number){
		start_step=move_number;
		end_step=start_step;
		return 1;
	}
	int main_step=move[step]->main_step;
	int sub_step =move[step]->sub_step;

	for(start_step=step;start_step>0;start_step--){
		if(move[start_step-1]->main_step!=main_step)
			break;
		if(move[start_step-1]->sub_step!=sub_step)
			break;
	}
	for(end_step=step;end_step<(move_number-1);end_step++){
		if(move[end_step+1]->main_step!=main_step)
			break;
		if(move[end_step+1]->sub_step!=sub_step)
			break;
	}
	
	double time_length,max_time_length=move[start_step]->total_time_length;

	for(int i=start_step;i<=end_step;i++)
		if(max_time_length<(time_length=move[i]->total_time_length))
			max_time_length=time_length;
	return max_time_length;
}

void Movement_processor::load_step_part(int start_step,int end_step,CMainFrame*  pFrame)
{
	int load_part_number=0;	
	for(int i=start_step;(i<=end_step)&&(i<move_number);i++)
		if(move[i]!=NULL)
			if(move[i]->comp!=NULL){
				load_part_number+=move[i]->comp->load_component_part(ws->release_memory_flag_map,ws->parts,&(pFrame->m_wndStatusBar));
				move[i]->comp->modify_move_absulate_location(move[i]->move[move[i]->step_number],*(ws->parts));
			}
	if(load_part_number>0)
		ws->root_component->get_component_box(*(ws->parts));
}
void Movement_processor::display_message(int start_step,int end_step,double time_length)
{
	CMainFrame*  pFrame=(CMainFrame*)(theApp.m_pMainWnd);
	if(pFrame==NULL)
		return;	

	load_step_part(start_step,end_step,pFrame);

	theApp.ws->root_component->clear_select_flag();

	Part_collector collector(ws->parts);
	for(int i=start_step;(i<=end_step)&&(i<move_number);i++)
		if(move[i]!=NULL)
			if(move[i]->comp!=NULL){
				collector.collect(move[i]->comp);
				move[i]->comp->set_component_selected();
			}
	collector.sort();

	if(collector.box_component_number>0)
		if(ws->mount_on_box(collector.component_box,1.5,time_length))
			start_time+=time_length;

	pFrame->m_movement_list.movement_list_modify_flag=false;
	pFrame->m_movement_list.list.SelectItem(NULL);
	pFrame->m_movement_list.list.SelectItem(move[start_step]->move_list_id[1]);

	pFrame->m_property_list.display_mount_message(collector,
		move[start_step]->main_step,
		move[start_step]->sub_step,
		move[start_step]->text,
		move[start_step]->sound_file);
	pFrame->m_movement_list.display_step();
}

CString Movement_processor::compare(int step,Part_collector *s)
{
	int start_step,end_step;
	Part_collector t(ws->parts);

	compute_start_and_and_step(step,start_step,end_step);
	for(int i=start_step;i<=end_step;i++)
		if(move[i]->comp!=NULL)
			t.collect(move[i]->comp);

	return t.compare_part_consistency(s);
}
void Movement_processor::adjust_current_step(bool direction_flag)
{
	int start_step,end_step;

	compute_start_and_and_step(current_step,start_step,end_step);
	
	if(direction_flag){
		current_step=end_step+1;
		if(current_step>=move_number){
			current_step=move_number;
			state_id=0;
		}
	}else{
		if(start_step<=0){
			current_step=0;
			state_id=0;
		}else {
			compute_start_and_and_step(start_step-1,start_step,end_step);
			current_step=start_step;
		}
	}
}

void Movement_processor::mark_visible_bitmap(int start_step,int end_step,bool start_end_flag)
{
	if(start_step>end_step){
		int x=end_step;
		end_step=start_step;
		start_step=x;
	}
	if(start_end_flag){
		for(int i=start_step;i<=end_step;i++)
			if((i>=0)&&(i<move_number))
				if(move[i]->comp!=NULL){
					move[i]->comp->display_flag&=(~12);
					if(move[i]->end_state_flag)
						move[i]->comp->display_flag|=4;
					else
						move[i]->comp->display_flag|=8;
				}
	}else{
		for(int i=end_step;i>=start_step;i--)
			if((i>=0)&&(i<move_number))
				if(move[i]->comp!=NULL){
					move[i]->comp->display_flag&=(~12);
					if(move[i]->start_state_flag)
						move[i]->comp->display_flag|=4;
					else
						move[i]->comp->display_flag|=8;
				}
	}
}

bool Movement_processor::do_movement(double time_length)
{
	if((move_number<=0)||(state_id<=0)||(state_id>6)){
		state_id=0;
		return false;
	}
	int i,start_step,end_step,counter,main_step,sub_step;
	double max_time_length=compute_start_and_and_step(current_step,start_step,end_step);

	if(end_step>=move_number){
		state_id=0;
		return false;
	}			
	for(counter=0,i=start_step;i<=end_step;i++){
		if((move[i]->comp!=NULL)&&(move[i]->step_number>0)){
			if(Sound_player::play(ws->plat_info->directory_name+(move[i]->sound_file)))
				return true;
			if(need_display_message_flag){
				need_display_message_flag=false;
				start_time=Platform_information::get_current_time();
				display_message(start_step,end_step,time_length);
			}

			double current_time,clock_time=Platform_information::get_current_time();
			move[i]->comp->selected_time=clock_time;

			current_time=clock_time-start_time;
			if(state_id>=4)
				current_time=max_time_length-current_time;
			current_time/=time_length/1000;
			
			move[i]->comp->display_flag&=(~12);
			if(current_time<=0){
					move[i]->comp->modify_move_absulate_location(move[i]->move[0],*(ws->parts));
					if(state_id<=3)
						counter++;
					if(move[i]->start_state_flag)
						move[i]->comp->display_flag|=4;
					else
						move[i]->comp->display_flag|=8;
			}else if(current_time>=(move[i]->total_time_length)){
					move[i]->comp->modify_move_absulate_location(move[i]->move[move[i]->step_number],*(ws->parts));
					if(state_id>=4)
						counter++;					
					if(move[i]->end_state_flag)
						move[i]->comp->display_flag|=4;
					else
						move[i]->comp->display_flag|=8;
			}else{
					counter++;
					move[i]->comp->display_flag|=4+8;
					for(int j=0;j<(move[i]->step_number);j++){
						if(current_time>(move[i]->time_length[j]))
							current_time-=(move[i]->time_length[j]);
						else{
							move[i]->comp->modify_move_absulate_location(
								move[i]->move[j].mix(move[i]->move[j+1],current_time/(move[i]->time_length[j])),*(ws->parts));
							break;
						}
					}
				}
		}
	}
	if(counter>0)
		return true;

	if((i=current_step)>=move_number)
		i=move_number-1;
	else if(i<0)
		i=0;
	main_step=move[i]->main_step;
	sub_step =move[i]->sub_step;
	
	switch(state_id){
	case 1:
			mark_visible_bitmap(start_step,end_step,true);
			adjust_current_step(true);
			state_id=0;
			break;
	case 2:
			mark_visible_bitmap(start_step,end_step,true);
			adjust_current_step(true);
			if(current_step>=move_number)
					state_id=0;
			else if(main_step!=move[current_step]->main_step)
					state_id=0;
			else
				need_display_message_flag=true;
			break;
	case 3:
			mark_visible_bitmap(start_step,end_step,true);
			adjust_current_step(true);
			if(current_step<move_number){
				need_display_message_flag=true;
				break;
			}
			if(ws->root_component!=NULL)
				ws->mount_on_box(ws->root_component->component_box,1.5,time_length);
			break;
	case 4:
			mark_visible_bitmap(start_step,end_step,false);
			current_step=start_step;
			state_id=0;
			break;
	case 5:
			mark_visible_bitmap(start_step,end_step,false);			
			if(start_step<=0){
				start_step=0;
				state_id=0;
			}else if(main_step!=move[start_step-1]->main_step)
				state_id=0;
			else{
				adjust_current_step(false);
				need_display_message_flag=true;
			}
			break;
	case 6:
			mark_visible_bitmap(start_step,end_step,false);
			if(start_step<=0)
				state_id=0;
			else{
				adjust_current_step(false);
				need_display_message_flag=true;
			}
			break;
	default:
			state_id=0;
			break;
	}
	return (state_id!=0)?true:false;
}
void Movement_processor::stop_mount()
{
		state_id=0;
		reset_visible_state();
		reset_component_location();
		
		Sound_player::turn_sound_on_off();
		Sound_player::turn_sound_on_off();
}
void Movement_processor::advance_sub_step(double time_length)
{
	need_display_message_flag=true;
	Sound_player::turn_sound_on_off();
	Sound_player::turn_sound_on_off();
	if(current_step<0)
		current_step=0;
	if(current_step>=move_number){
		current_step=move_number;
		return;
	}
	if(current_step<move_number){
		state_id=1;
		mark_visible_bitmap(current_step,false);
	}
	reset_component_location();
}
void Movement_processor::advance_main_step(double time_length)
{
	need_display_message_flag=true;
	Sound_player::turn_sound_on_off();
	Sound_player::turn_sound_on_off();
	if(current_step<0)
		current_step=0;
	if(current_step>=move_number){
		current_step=move_number;
		return;
	}
	if(current_step<move_number){
		state_id=2;
		mark_visible_bitmap(current_step,false);
	}
	reset_component_location();
}
void Movement_processor::advance_forever(double time_length)
{
	need_display_message_flag=true;
	Sound_player::turn_sound_on_off();
	Sound_player::turn_sound_on_off();
	if(current_step<0)
		current_step=0;
	if(current_step>=move_number){
		current_step=move_number;
		return;
	}
	if(current_step<move_number){
		state_id=3;
		mark_visible_bitmap(current_step,false);
	}
	reset_component_location();
}
void Movement_processor::retreat_sub_step(double time_length)
{
	need_display_message_flag=true;
	Sound_player::turn_sound_on_off();
	Sound_player::turn_sound_on_off();

	mark_visible_bitmap(current_step,false);
	if(current_step<0){
		current_step=0;		
		return;
	}
	adjust_current_step(false);
	state_id=4;
	mark_visible_bitmap(current_step,true);
	reset_component_location();
}
void Movement_processor::retreat_main_step(double time_length)
{
	need_display_message_flag=true;
	Sound_player::turn_sound_on_off();
	Sound_player::turn_sound_on_off();

	mark_visible_bitmap(current_step,false);
	if(current_step<0){
		current_step=0;		
		return;
	}
	adjust_current_step(false);
	state_id=5;
	mark_visible_bitmap(current_step,true);
	reset_component_location();
}

void Movement_processor::retreat_forever(double time_length)
{
	need_display_message_flag=true;
	Sound_player::turn_sound_on_off();
	Sound_player::turn_sound_on_off();
	mark_visible_bitmap(current_step,false);
	if(current_step<0){
		current_step=0;	
		return;
	}
	adjust_current_step(false);
	state_id=6;
	mark_visible_bitmap(current_step,true);
	reset_component_location();
}

void Movement_processor::save_to_cache(CString title_name,Component *comp,int begin_main_step,int begin_sub_step,int end_main_step,int end_sub_step)
{
	int i,number;
	CString str;

	if(title_name.IsEmpty())
		return;

	for(i=0,number=0;i<move_number;i++){
		if(move[i]->is_between(begin_main_step,begin_sub_step,end_main_step,end_sub_step)){
			for(Component *p=move[i]->comp;p!=NULL;p=p->parent){
				if(p==comp){
					number++;
					break;
				}
			}
		}
	}
	if(number<0)
		return;

	str.Format(_T("%d\n%d\n\n"),comp->name.GetLength(),number);
	ofstream f(ws->plat_info->directory_name+_T("cache.lfy"),ios_base::app|ios::out);

	File_data::output_string(f,comp->id_name+_T("\n"));
	File_data::output_string(f,title_name+_T("\n"));
	File_data::output_string(f,str);

	for(i=0;i<move_number;i++){
		if(move[i]->is_between(begin_main_step,begin_sub_step,end_main_step,end_sub_step)){
			for(Component *p=move[i]->comp;p!=NULL;p=p->parent){
				if(p==comp){
					move[i]->flush(f);
					break;
				}
			}
		}
	}
	f.close();

	return;
}

bool Movement_processor::add_from_cache(Component *comp,bool cache_state_flag,int number)
{
	if(number>=MAX_CACHE_MOVEMENT_NUMBER)
		return false;

	ifstream f(ws->plat_info->directory_name+_T("cache.lfy"));
	
	int i,j,n,left_length;	
	CString id_name;

	comp->clear_select_flag();

	if(move[move_number]!=NULL)
		return false;

	for(i=0;i<=number;i++){	
		{
			char buffer[MAX_BUFFER_LENGTH];
			f>>buffer;id_name=buffer;
			f>>buffer>>left_length>>n;
		}
		for(j=0;j<n;j++)
			move[j+move_number]=new Movement(f,NULL);
		if(f.eof()||f.fail()){
			for(j=0;j<n;j++){
				delete (move[j+move_number]);
				move[j+move_number]=NULL;
			}
			break;
		}
		if((i==number)&&(n>0)){
			Component *p;
			for(p=comp;p!=NULL;p=p->parent)
				if(p->id_name==id_name)
					break;
			if(p!=NULL){
				for(j=0;j<n;j++){
					CString str=move[j+move_number]->name;
					str=str.Right(str.GetLength()-left_length);
					move[j+move_number]->name=p->name+str;
					
					if(ws->root_component==NULL)
						move[j+move_number]->comp=NULL;
					else{
						move[j+move_number]->ws=ws;
						move[j+move_number]->comp=ws->root_component->get_node_by_name(move[j+move_number]->name);
					}
				}
				int adjust_step,new_main_step=-1;
				if(move_number>0){
					if(cache_state_flag)
						adjust_step=move[n+move_number-1]->main_step-move[(current_step<move_number)?current_step:(move_number-1)]->main_step+1;
					else
						adjust_step=move[move_number]->main_step    -move[(current_step<move_number)?current_step:(move_number-1)]->main_step;
					for(j=0;j<n;j++)
						move[j+move_number]->main_step-=adjust_step;
					new_main_step=move[move_number]->main_step;
				}
				move_number+=n;
				state_id=0;
				sort(move_number-n);
				for(j=0;j<move_number;j++)
					if(move[j]->main_step==new_main_step){
						current_step=j;
						reset_visible_state();
						break;
					}
				return true;
			}
		}
		for(j=0;j<n;j++){
			delete (move[j+move_number]);
			move[j+move_number]=NULL;
		}
	}
	return false;
}

void Movement_processor::delete_from_cache(Component *comp,int number)
{
	ifstream f(ws->plat_info->directory_name+_T("cache.lfy"));
	ofstream f_out(ws->plat_info->directory_name+_T("temp.lfy"));
	
	int i,j,n,left_length;
	char buffer[MAX_BUFFER_LENGTH];
	CString id_name,titile_name;

	for(i=0;;i++){
		n=0;
		f>>buffer;						id_name=buffer;
		f>>buffer>>left_length>>n;		titile_name=buffer;
		for(j=0;j<n;j++)
			move[j+move_number]=new Movement(f,ws);
		if(f.eof()||f.fail()){
			for(j=0;j<n;j++){
				delete (move[j+move_number]);
				move[j+move_number]=NULL;
			}
			break;
		}
		if(i!=number){
			CString str;
			str.Format(_T("%d\n%d\n"),left_length,n);
			str=id_name+_T("\n")+titile_name+_T("\n")+str;
			File_data::output_string(f_out,str);	
			for(j=0;j<n;j++)
				move[j+move_number]->flush(f_out);
		}
		for(j=0;j<n;j++){
			delete (move[j+move_number]);
			move[j+move_number]=NULL;
		}
	}
	f.close();
	f_out.close();

	CFile::Remove(ws->plat_info->directory_name+_T("cache.lfy"));
	CFile::Rename(ws->plat_info->directory_name+_T("temp.lfy"),ws->plat_info->directory_name+_T("cache.lfy"));

	return;
}
void Movement_processor::create_title_file()
{
	CString str,name;
	CString file_name1=ws->plat_info->directory_name+_T("\\temp\\带杆件列表的字幕文件.txt");
	CString file_name2=ws->plat_info->directory_name+_T("\\temp\\无杆件列表的字幕文件.txt");
	ofstream f1(file_name1),f2(file_name2);
	
	for(int i=0,j=0;i<move_number;i=j){
		Part_collector collector(ws->parts);
		for(j=i;j<move_number;j++){
			if((move[i]->main_step)!=(move[j]->main_step))
				break;
			if(move[j]->comp!=NULL)
				collector.collect(move[j]->comp);
		}
		collector.sort();
		str.Format(_T("第 %d 步\n"),move[i]->main_step);
		File_data::output_string(f1,str);
		File_data::output_string(f2,str);

		for(int m=0,n=0;m<(collector.component_number);m=n){
			for(n=m;n<(collector.component_number);n++)
				if((collector.component_pointer[m]->part_id)!=(collector.component_pointer[n]->part_id))
					break;
			if(n>m){
				name=(ws->parts->part_array[collector.component_pointer[m]->part_id]->user_name);
				str.Format(_T("%10d件    "),n-m);
				File_data::output_string(f1,str+name+_T("\n"));
			}
		}
		File_data::output_string(f1,move[i]->text+_T("\n\n"));
		File_data::output_string(f2,move[i]->text+_T("\n\n"));
	}
	f1.close();
	f2.close();
	::ShellExecute(NULL,_T("open"),file_name2,NULL,NULL,SW_SHOWNORMAL);
	::ShellExecute(NULL,_T("open"),file_name1,NULL,NULL,SW_SHOWNORMAL);
}

void Movement_processor::reset_component_location()
{
	for(int i=move_number;i>=current_step;i--)
		if(move[i]!=NULL)
			if(move[i]->comp!=NULL)
				if(move[i]->step_number>0)
					move[i]->comp->modify_move_absulate_location(move[i]->move[move[i]->step_number],*(ws->parts));

	for(int i=0;i<current_step-1;i++)
		if(move[i]!=NULL)
			if(move[i]->comp!=NULL)
				if(move[i]->step_number>0)
					move[i]->comp->modify_move_absulate_location(move[i]->move[move[i]->step_number],*(ws->parts));
}