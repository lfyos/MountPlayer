#pragma once

#include "..\kernel\User_interface.h"
#include "..\kernel\Sound_player.h"

#define MAX_STEP_NUMBER			20
#define MAX_MATCH_NUMBER		5

class Movement
{
	void clear_component_movement_id(Component *p)
	{
		Component *q;
		if((q=p->child)==NULL)
			p->movement_id=0;
		else
			do{	
				clear_component_movement_id(q);
			}while(p->child!=(q=q->left_brother));
	}
public:
	Whole_system *ws;

	int main_step,sub_step;
	CString main_tag,sub_tag;

	Component *comp;
	CString name,sound_file,text;

	bool start_state_flag,end_state_flag;
	int step_number;
	double total_time_length,time_length[MAX_STEP_NUMBER+1];
	Location move[MAX_STEP_NUMBER+1];

	int match_number;
	Movement_match component_match[MAX_MATCH_NUMBER];

	bool is_between(int begin_main_step,int begin_sub_step,int end_main_step,int end_sub_step)
	{
		if(begin_main_step>end_main_step)
			return false;
		if(begin_main_step==end_main_step)
			if(begin_sub_step>end_sub_step)
				return false;
		if((main_step<begin_main_step)||(main_step>end_main_step))
			return false;
		if((main_step>begin_main_step)&&(main_step<end_main_step))
			return true;
		if(begin_main_step==end_main_step){
			if((sub_step<begin_sub_step)||(sub_step>end_sub_step))
				return false;
			else
				return true;
		}
		if(begin_main_step==main_step){
			if(sub_step<begin_sub_step)
				return false;
			else
				return true;
		}
		if(sub_step>end_sub_step)
				return false;
		else
				return true;
	}
	void flush(ofstream &f);
	void caculate_total_time_length()
	{
		total_time_length=0;
		for(int i=0;i<step_number;i++)
			total_time_length+=time_length[i];
		if(total_time_length<MAX_STEP_NUMBER)
			total_time_length=MAX_STEP_NUMBER;
	}
	
	Movement(ifstream &f,Whole_system *my_ws);
	Movement(Component *my_comp,Whole_system *my_ws)
	{
		ws=my_ws;

		main_step=1;
		sub_step=1;
		main_tag=_T("没有标记");
		sub_tag=_T("没有标记");

		name=_T("没有杆件名称");
		sound_file=_T("没有配音文件");
		text=_T("没有字幕文件");

		if((comp=my_comp)!=NULL)
			name=comp->name;

		start_state_flag=false;
		end_state_flag=true;

		step_number=0;
		for(int i=0;i<MAX_STEP_NUMBER;i++){
			time_length[i]=1;
			move[i]=Location();
		}
		total_time_length=MAX_STEP_NUMBER;
		for(int i=0;i<(sizeof(move_list_id)/sizeof(move_list_id[0]));i++)
			move_list_id[i]=NULL;
		match_number=0;
	}
	~Movement(void)
	{
		if((comp!=NULL)&&(ws!=NULL)){
			comp->display_flag|=6;
			clear_component_movement_id(comp);
		}
	}
	HTREEITEM move_list_id[2];
};

