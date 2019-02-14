#include "StdAfx.h"
#include "Movement.h"

Movement::Movement(ifstream &f,Whole_system *my_ws)
{
	int i;
	char buffer[MAX_BUFFER_LENGTH];

	ws=my_ws;
	main_step=0;
	sub_step=0;

	comp=NULL;
	
	name=_T("");
	text=name;
	sound_file=name;

	step_number=0;

	for(i=0;i<(sizeof(move_list_id)/sizeof(move_list_id[0]));i++)
		move_list_id[i]=NULL;	
	
	f>>main_step>>sub_step>>buffer;		
	name=buffer;
	
	if((f.eof())||(f.fail())||(ws==NULL))
		comp=NULL;
	else if(ws->root_component==NULL)
		comp=NULL;
	else 
		comp=ws->root_component->get_node_by_name(name);

	f>>buffer;	main_tag=buffer;
	f>>buffer;	sub_tag=buffer;
	f>>buffer;	sound_file=buffer;
	f>>buffer;	text=buffer;

	f>>buffer>>buffer;	start_state_flag=(CString(buffer)==_T("ÏÔÊ¾"))?true:false;
	f>>buffer>>buffer;	end_state_flag  =(CString(buffer)==_T("ÏÔÊ¾"))?true:false;
	f>>buffer>>step_number;

	total_time_length=0;

	for(int i=0;i<MAX_STEP_NUMBER;i++){
		time_length[i]=1;
		move[i]=Location();
	}
	total_time_length=MAX_STEP_NUMBER;
	if((f.eof())||(f.fail())||(step_number<=0)){
		step_number=0;
	}else{
		if(step_number>MAX_STEP_NUMBER)
			step_number=MAX_STEP_NUMBER;
		for(i=0;i<=step_number;i++){
			double data[16];
			f>>(time_length[i]);
			if(time_length[i]<1.0)
				time_length[i]=1.0;
			if(i<step_number)
				total_time_length+=(time_length[i]);		
			for(int j=0;j<16;j++)
				f>>(data[j]);
			move[i]=Location(data);
		}
	}
	
	if(total_time_length<=0)
		total_time_length=1.0;	

	f>>buffer>>match_number;
	if(match_number<0)
		match_number=0;
	else if(match_number>=MAX_MATCH_NUMBER)
		match_number=MAX_MATCH_NUMBER-1;
	for(int i=0;i<MAX_MATCH_NUMBER;i++)
		if(i<match_number)
			component_match[i].load(f);
		else
			component_match[i].reset();
	return;
}

void Movement::flush(ofstream &f)
{
	f<<main_step<<"	"<<sub_step<<"\n";
	File_data::output_string(f,name+_T("\n")+main_tag+_T("\n")+sub_tag+_T("\n")+sound_file+_T("\n\n")+text+_T("\n\n"));
	f<<"ÆðÊ¼×´Ì¬: "<<(start_state_flag?"ÏÔÊ¾		":"Òþ²Ø		");
	f<<"ÖÕÖ¹×´Ì¬: "<<(  end_state_flag?"ÏÔÊ¾		":"Òþ²Ø		");
	f<<"ÒÆ¶¯½Úµã: "<<step_number<<"\n";
	for(int i=0;i<=step_number;i++){
		double p[16];
		f<<(time_length[i]);
		move[i].get_location_double_data(p);
		for(int j=0;j<16;j++)
			f<<"	"<<(p[j]);
		f<<"\n";
	}
	f<<"\nÅäºÏ:		"<<match_number<<"\n";
	for(int i=0;i<match_number;i++)
		component_match[i].flush(f);
	f<<"\n\n";

	return;
}
