#include "StdAfx.h"
#include "movement_cache.h"
#include "part_tree.h"

movement_cache::movement_cache(void)
{
	int i;
	for(i=0;i<MAX_MOVEMENT_STEP;i++)
		move[i]=NULL;
	move_number=0;
}
movement_cache::~movement_cache(void)
{
	delete_all_movements();
}
void movement_cache::delete_all_movements()
{
	int i;
	for(i=0;i<move_number;i++)
		if(move[i]!=NULL){
			delete (move[i]);
			move[i]=NULL;
		}
	move_number=0;
}

int movement_cache::load(ifstream *f)
{
	int i;
	char buf[MAX_MESSAGE_LENGTH];

	delete_all_movements();

	(*f)>>buf;	label_name		=buf;
	(*f)>>buf;	tree_file_name	=buf;
	(*f)>>cache_type>>move_number;

	if(label_name.IsEmpty())
		label_name="No_Name";
	if(tree_file_name.IsEmpty())
		tree_file_name="No_Name";
	if(move_number<0)
			move_number=0;
	else if(move_number>MAX_MOVEMENT_STEP)
			move_number=MAX_MOVEMENT_STEP;
	
	for(i=0;i<move_number;i++)
		move[i]=new movement(f);
	
	if((f->fail())||(f->eof())){	
		for(i=0;i<move_number;i++){
			delete (move[i]);
			move[i]=NULL;
		}
		move_number=0;
	}
	return move_number;
}
void movement_cache::flush(ofstream  *f)
{
	int i,j,main_step_id;

	for(i=0;i<move_number;i++){
		for(j=i;j>0;j--){
			BOOL flag;
			movement *m;
			if((move[j-1]->main_step)<(move[j]->main_step))
						flag=FALSE;
			else if((move[j-1]->main_step)>(move[j]->main_step))
						flag=TRUE;
			else if((move[j-1]->sub_step)<=(move[j]->sub_step))
						flag=FALSE;
			else
						flag=TRUE;
			if(flag){
				m=move[j-1];
				move[j-1]=move[j];;
				move[j]=m;
			}
		}
	}
	for(i=0,j=0,main_step_id=-1234567;i<move_number;i++){
		if(move[i]->main_step!=main_step_id){
			main_step_id=move[i]->main_step;
			j++;
		}
		move[i]->main_step=j;
	}

	(*f)<<label_name<<"\n"      <<tree_file_name<<"\n";
	(*f)<<cache_type<<"        "<<move_number   <<"\n";
	for(i=0;i<move_number;i++)
		move[i]->flush(f);

	return;
}
void movement_cache::insert_movement(movement *m)
{
	if(move_number>=MAX_MOVEMENT_STEP)
		return;
	if(move_number<0)
		move_number=0;
	move[move_number]=new movement;
	move[move_number]->copy_from(m);
	move_number++;
}
void movement_cache::add_movement_cache_label_to_menu(CMenu * pPopup ,int cache_main_step,int cache_sub_step,
		int directory_name_length,CString file_name,part_tree * t,int begin_command_id,BOOL cache_insert_or_delete_flag)
{
	UINT i,separator_number,menu_position_id;
	CString str;
		
	if(t==NULL)
		return;

	for(menu_position_id=0;t!=NULL;t=t->parent){
		ifstream f(file_name);
		for(separator_number=0,i=begin_command_id;;i++){
			if(load(&f)<=0)
				break;
			str=t->tree_file_name;
			str=str.Right(str.GetLength()-directory_name_length);

			if(str==tree_file_name){
				separator_number++;	
				str="É¾³ý:";
				if(cache_insert_or_delete_flag){
					if(cache_type==1)
						str.Format("%d,%d:",cache_main_step,cache_sub_step);
					else if(cache_type==2)
						str.Format("%d:",cache_main_step);
					else
						str="";
				}					
				pPopup->InsertMenu(menu_position_id++,MF_STRING|MF_BYPOSITION,i,str+label_name);
			}
		}
		f.close();
		if(separator_number>0)
			pPopup->InsertMenu(menu_position_id++,MF_SEPARATOR|MF_BYPOSITION);
	}
}
void movement_cache::delete_movement_from_cache(CString file_name,int id_number)
{
	int i;	
	CString file_name_bak=file_name+".bak";
	remove(file_name_bak);
	rename(file_name,file_name_bak);

	ifstream f_in(file_name_bak);
	ofstream f_out(file_name);
	
	for(i=0;;i++){
		if(load(&f_in)<=0)
			break;
		if(i!=id_number)
			flush(&f_out);
	}
	f_in.close();
	f_out.close();
	remove(file_name_bak);
	return;
}

void movement_cache::save_movement_to_cache(CString file_name)
{
	CString file_name_bak=file_name+".bak";
	remove(file_name_bak);
	rename(file_name,file_name_bak);

	ifstream f_in(file_name_bak);
	ofstream f_out(file_name);
	
	flush(&f_out);
	for(;;){
		if(load(&f_in)<=0)
			break;
		flush(&f_out);
	}
	f_in.close();
	f_out.close();
	remove(file_name_bak);
	return;
}
int movement_cache::get_movement_from_cache(CString file_name,int id_number)
{
	int i;
	ifstream f_in(file_name);

	for(i=0;;i++){
		if(load(&f_in)<=0){
			delete_all_movements();
			break;
		}
		if(i==id_number)
			break;	
	}
	f_in.close();

	return move_number;
}
movement * movement_cache::get_movement()
{
	movement *m;
	if(move_number<=0)
		return NULL;
	move_number--;
	m=move[move_number];
	move[move_number]=NULL;

	return m;
}