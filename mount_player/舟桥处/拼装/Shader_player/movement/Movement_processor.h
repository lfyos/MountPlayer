#pragma once

#include "Movement.h"

#define MAX_MOVEMENT_NUMBER		100000

class CMainFrame;

class Movement_processor
{
	CString movement_file_name;
private:
	int state_id,back_stack_number;
	bool need_display_message_flag;

	bool movement_greater(int i,int j);
	
public:
	Whole_system *ws;
	int move_number;
	Movement *move[MAX_MOVEMENT_NUMBER];

	double compute_start_and_and_step(int step,int &start_step,int &end_step);
private:	
	void release_movement();

	void adjust_current_step(bool direction_flag);
	
	void mark_visible_bitmap(int start_step,int end_step,bool start_end_flag);
	void mark_visible_bitmap(int step,bool start_end_flag)
	{
		int start_step,end_step;
		compute_start_and_and_step(step,start_step,end_step);
		mark_visible_bitmap(start_step,end_step,start_end_flag);
	}
	void flush(CString file_name=_T(""))
	{
		if(do_push_flush_movement_flag){
			if(file_name==_T(""))
				file_name=movement_file_name;
			ofstream f(ws->plat_info->directory_name+file_name);
			for(int i=0;i<move_number;i++)
				move[i]->flush(f);
			f.close();
		}
	}
public:
	
	void reset_visible_state()
	{
		if(move_number<=0)
			return;
		
		int start_step,end_step;
		compute_start_and_and_step(current_step,start_step,end_step);
		current_step=start_step;

		if(current_step<move_number)
			mark_visible_bitmap(current_step,move_number-1,false);
		if(current_step>0)
			 mark_visible_bitmap(0,current_step-1,true);
	}
	void reset_component_location();

	void step_jump(HTREEITEM my_id)
	{
		for(int i=0;i<move_number;i++){
			if((move[i]->move_list_id[0]==my_id)||(move[i]->move_list_id[1]==my_id)){
				current_step=i;
				reset_visible_state();
				break;
			}
		}
		reset_component_location();
	}
	void sort(int add_step_number=-1);
	void load(CString file_name=_T(""));

	bool do_push_flush_movement_flag;

	void push_movement()
	{
		if(do_push_flush_movement_flag){
			CString file_name;
			file_name.Format(_T("temp\\Movement%d.bak"),back_stack_number++);
			flush(file_name);
		}
	}
	bool pop_movement()
	{
		if(do_push_flush_movement_flag){
			if(back_stack_number<=0)
				AfxMessageBox(_T("已经回退到起始拼装信息"));
			else{
				CString file_name;
				file_name.Format(_T("temp\\Movement%d.bak"),--back_stack_number);
				load(file_name);
				CFile::Remove(ws->plat_info->directory_name+file_name);
				return true;
			}
		}
		return false;
	}
	Movement_processor(Whole_system *my_ws,CString my_movement_file_name);
	~Movement_processor()
	{
		do_push_flush_movement_flag=true;
		flush();
		release_movement();

		CFileFind finder;
		BOOL bWorking = finder.FindFile(ws->plat_info->directory_name+_T("temp\\Movement*.bak"));
		while (bWorking){
		   bWorking = finder.FindNextFile();
		   if(!(finder.IsDots()))
			   if(!(finder.IsDirectory()))
				   if(!(finder.IsSystem()))
						CFile::Remove(finder.GetFilePath());
		}
	}	
	CString compare(int step,Part_collector *s);

	void load_step_part(int start_step,int end_step,CMainFrame*  pFrame);
	inline void load_step_part(CMainFrame*  pFrame)
	{
		int start_step,end_step;
		compute_start_and_and_step(current_step,start_step,end_step);
		load_step_part(start_step,end_step,pFrame);
	}
	void display_message(int start_step,int end_step,double time_length);
	
	int current_step;
	double start_time;

	bool do_movement(double time_length);

	void advance_sub_step(double time_length);
	void advance_main_step(double time_length);
	void advance_forever(double time_length);
	
	void retreat_sub_step(double time_length);
	void retreat_main_step(double time_length);
	void retreat_forever(double time_length);

	void stop_mount();

	void save_to_cache(CString title_name,Component *comp,int begin_main_step,int begin_sub_step,int end_main_step,int end_sub_step);
	bool add_from_cache(Component *comp,bool cache_state_flag,int number);
	void delete_from_cache(Component *comp,int number);
	void create_title_file();

};

class Movement_processor_with_virtual_mount:public Movement_processor
{
	int state_id;		//0:start		1:pass component test 2~n:
public:
	Movement_processor_with_virtual_mount(Whole_system *my_ws,CString my_movement_file_name)
		:Movement_processor(my_ws,my_movement_file_name)
	{
		state_id=0;
	};
	~Movement_processor_with_virtual_mount()
	{
	}
	void start_test(double time_length,HTREEITEM &match_list_id)
	{
		int i,start_step,end_step;
	
		match_list_id=NULL;
		compute_start_and_and_step(current_step,start_step,end_step);
		for(i=start_step;(i<=end_step)&&(i<move_number);i++){
			if(move[i]->comp!=NULL)
				move[i]->comp->clear_select_flag();
		}

		for(i=start_step;(i<=end_step)&&(i<move_number);i++){
			if(move[i]->match_number>0){
				Movement_match *ma=&(move[i]->component_match[0]);
				Component *comp=ws->root_component->get_node_by_name(ma->source_name);
				if((comp==NULL)||(ma->source_body_id<0)||(ma->source_face_id<0))
					break;
				ws->match_component_ids.reset_from_match(comp,ma->source_body_id,ma->source_face_id,ws->parts);
				state_id=1;
				for(i=start_step;(i<=end_step)&&(i<move_number);i++){
					if((comp=move[i]->comp)!=NULL){
							comp->modify_move_absulate_location(move[i]->move[0],*(ws->parts));
							comp->display_flag|=6;
						}
				}
				match_list_id=ma->source_list_id;
				return;
			}
		}
		state_id=0;
		advance_sub_step(time_length);
	}

	bool match_test(double time_length,CString destination_name,int destination_body_id,int destination_face_id,HTREEITEM &match_list_id)
	{
		int start_step,end_step,counter=0,total_counter=0;

		match_list_id=NULL;
		if(state_id<=0){
			state_id=0;
			return false;
		}
		compute_start_and_and_step(current_step,start_step,end_step);
		
		for(int i=start_step;(i<=end_step)&&(i<move_number);i++)
			total_counter=total_counter+(move[i]->match_number);

		for(int i=start_step;(i<=end_step)&&(i<move_number);i++){
			for(int j=0;j<(move[i]->match_number);j++,counter++){
				if((1+counter)==state_id){
					Movement_match *ma=&(move[i]->component_match[j]);
					if(ma->destination_body_id==destination_body_id){
						if(ma->destination_face_id==destination_face_id){
							if(ma->destination_name==destination_name){
								state_id++;
								if((counter+1)>=total_counter){
									for(i=start_step;(i<=end_step)&&(i<move_number);i++)
										if(move[i]->comp!=NULL)
											move[i]->comp->reset_component(*(ws->parts));
									state_id=0;
									ws->select_component_ids.reset();
									ws->match_component_ids.reset();
									advance_sub_step(time_length);
									return true;
								}else{
									for(i=start_step,counter=0;(i<=end_step)&&(i<move_number);i++)
										for(j=0;j<(move[i]->match_number);j++,counter++){
											if((1+counter)==state_id){
												ma=&(move[i]->component_match[j]);
												Component *comp=ws->root_component->get_node_by_name(ma->source_name);
												if((comp!=NULL)&&(ma->source_body_id>=0)&&(ma->source_face_id>=0))
													ws->match_component_ids.reset_from_match(comp,ma->source_body_id,ma->source_face_id,ws->parts);
												match_list_id=ma->source_list_id;
												return true;
											}
										}
								}

							}
						}
					}
					return false;
				}
			}
		}
		return false;
	}
};