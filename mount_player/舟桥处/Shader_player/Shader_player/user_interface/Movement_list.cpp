
#include "stdafx.h"
#include "../mainfrm.h"
#include "Movement_list.h"
#include "../Resource.h"
#include "../Shader_player.h"

#include "Mount_step_dialog.h"
#include "Mount_information_dialog.h"
#include "Mount_cache_information_dialog.h"
#include "Mount_step_tag_dialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView


BEGIN_MESSAGE_MAP(Movement_list,List_base)

	ON_NOTIFY(TVN_SELCHANGED, 4, OnModify)

	ON_COMMAND          (ID_PROPERTIES, OnRedrawTreeList)

	ON_COMMAND          (ID_MOUNT_STEP_DIVISION, OnMountStepDivision)
	ON_UPDATE_COMMAND_UI(ID_MOUNT_STEP_DIVISION, &Movement_list::OnEnableDisable)
	
	ON_COMMAND          (ID_MOUNT_STEP_MERGE, OnMountStepMerge)
	ON_UPDATE_COMMAND_UI(ID_MOUNT_STEP_MERGE, &Movement_list::OnEnableDisable)

	ON_COMMAND          (ID_MOUNT_TOGETHER, OnMountTogether)
	ON_UPDATE_COMMAND_UI(ID_MOUNT_TOGETHER, &Movement_list::OnEnableDisable)


	ON_COMMAND          (ID_MOUNT_MAIN_STEP_SORT,OnMountMainStepSort)
	ON_UPDATE_COMMAND_UI(ID_MOUNT_MAIN_STEP_SORT, &Movement_list::OnEnableDisable)

	ON_COMMAND          (ID_MOUNT_SUB_STEP_SORT, OnMountSubStepSort)
	ON_UPDATE_COMMAND_UI(ID_MOUNT_SUB_STEP_SORT, &Movement_list::OnEnableDisable)

	ON_COMMAND          (ID_MOUNT_STEP_MOVE, OnMountStepMove)
	ON_UPDATE_COMMAND_UI(ID_MOUNT_STEP_MOVE, &Movement_list::OnEnableDisable)

	ON_COMMAND          (ID_MOUNT_STEP_EXCHANGE, OnMountStepExchange)
	ON_UPDATE_COMMAND_UI(ID_MOUNT_STEP_EXCHANGE, &Movement_list::OnEnableDisable)
	
	ON_COMMAND          (ID_MOUNT_TITLE_AND_SOUND,OnMountTitleAndSound)
	ON_UPDATE_COMMAND_UI(ID_MOUNT_TITLE_AND_SOUND, &Movement_list::OnEnableDisable)

	ON_COMMAND          (ID_MOUNT_TITLE_FILE,OnMountTitleFile)
	ON_UPDATE_COMMAND_UI(ID_MOUNT_TITLE_FILE, &Movement_list::OnEnableDisable)

	ON_COMMAND          (ID_MOUNT_STEP_TAG, &Movement_list::OnMountStepTag)
	ON_UPDATE_COMMAND_UI(ID_MOUNT_STEP_TAG, &Movement_list::OnEnableDisable)

	ON_COMMAND          (ID_MOUNT_SAVE_TO_CACHE,OnMountSaveToCache)
	ON_UPDATE_COMMAND_UI(ID_MOUNT_SAVE_TO_CACHE, &Movement_list::OnEnableDisable)

	ON_COMMAND          (ID_MOUNT_STEP_DELETE,OnMountStepDelete)
	ON_UPDATE_COMMAND_UI(ID_MOUNT_STEP_DELETE, &Movement_list::OnEnableDisable)

	ON_COMMAND          (ID_MOUNT_RESTORE_DESIGN, OnMountRestoreDesign)
	ON_UPDATE_COMMAND_UI(ID_MOUNT_RESTORE_DESIGN, &Movement_list::OnEnableDisable)

	ON_COMMAND          (ID_MOUNT_SAVE_DESIGN, &Movement_list::OnMountSaveDesign)
	ON_UPDATE_COMMAND_UI(ID_MOUNT_SAVE_DESIGN, &Movement_list::OnUpdateMountSaveDesign)

	ON_COMMAND          (ID_MOUNT_ALL_STEP_MERGE, &Movement_list::OnMountAllStepMerge)
	ON_UPDATE_COMMAND_UI(ID_MOUNT_ALL_STEP_MERGE, &Movement_list::OnEnableDisable)

	ON_COMMAND          (ID_MOUNT_STEP_FORCE, &Movement_list::OnMountStepForce)
	ON_UPDATE_COMMAND_UI(ID_MOUNT_STEP_FORCE, &Movement_list::OnEnableDisable)

	ON_COMMAND          (ID_MOUNT_STEP_FORCE_MANUAL, &Movement_list::OnMountStepForceManual)
	ON_UPDATE_COMMAND_UI(ID_MOUNT_STEP_FORCE_MANUAL, &Movement_list::OnEnableDisable)
	
END_MESSAGE_MAP()

void Movement_list::display_step()
{
	CString str;

	if(theApp.mounter->move_number<=0){
		theApp.mounter->move_number=0;
		str=_T("拼装步骤");
	}else{
		if(theApp.mounter->current_step<=0){
			theApp.mounter->current_step=0;
			str.Format(_T("拼装步骤:%d/%d[开始]"),theApp.mounter->move[0]->main_step,theApp.mounter->move[0]->sub_step);
		}else if(theApp.mounter->current_step>=(theApp.mounter->move_number)){
			theApp.mounter->current_step=theApp.mounter->move_number;
			str.Format(_T("拼装步骤:%d/%d[结束]"),theApp.mounter->move[theApp.mounter->move_number-1]->main_step,theApp.mounter->move[theApp.mounter->move_number-1]->sub_step);
		}else
			str.Format(_T("拼装步骤:%d/%d"),theApp.mounter->move[theApp.mounter->current_step]->main_step,theApp.mounter->move[theApp.mounter->current_step]->sub_step);
	}
	SetWindowText(str);
}
void Movement_list::do_file_list(bool do_set_item_flag)
{
	CString str;
	Whole_system *ws=theApp.ws;
	Movement_processor  *mounter=theApp.mounter;

	list.DeleteAllItems();
	set_component_list_id(ws->root_component,NULL);

	root=list.InsertItem(_T("拼装步骤"),2,2);	
	for(int i=0,j,k;i<mounter->move_number;i=j){
		str.Format(_T("%d"),mounter->move[i]->main_step);
		if(mounter->move[i]->main_tag!=_T("没有标记"))
			str=str+_T(":")+(mounter->move[i]->main_tag);

		HTREEITEM main_root=list.InsertItem(str,1,1,root);	
		for(j=i;j<mounter->move_number;j=k){
			if(mounter->move[i]->main_step!=mounter->move[j]->main_step)
				break;
			str.Format(_T("%d"),mounter->move[j]->sub_step);
			if(mounter->move[j]->sub_tag!=_T("没有标记"))
				str=str+_T(":")+(mounter->move[j]->sub_tag);

			HTREEITEM sub_root=list.InsertItem(str,(j<mounter->current_step)?3:5,(j<mounter->current_step)?3:5,main_root);
			
			for(k=j;k<mounter->move_number;k++){
				Movement *m=mounter->move[k];
				if(m->main_step!=(mounter->move[j]->main_step))
					break;
				if((mounter->move[j])->sub_step!=(m->sub_step))
					break;
				m->move_list_id[0]=main_root;
				m->move_list_id[1]=sub_root;
				if(m->comp!=NULL)
					set_component_list_id(m->comp,sub_root);

				for(int match_index=0;match_index<(m->match_number);match_index++){
					Movement_match *move_match=&(m->component_match[match_index]);
					if((move_match->source_body_id>=0)||(move_match->destination_body_id>=0)){
						move_match->match_list_id=list.InsertItem((move_match->match_tag==_T("没有标记"))?_T(" "):(move_match->match_tag),1,1,sub_root);
						if(move_match->source_body_id>=0)
							move_match->source_list_id=list.InsertItem(_T(" "),3,3,move_match->match_list_id);
						if(move_match->destination_body_id>=0)
							move_match->destination_list_id=list.InsertItem(_T(" "),5,5,move_match->match_list_id);
					}
				}
			}
		}
	}
	list.SetItemState(root, TVIS_BOLD, TVIS_BOLD);
	list.Expand(root, TVE_EXPAND);

	if(do_set_item_flag){
		if((mounter->current_step<0)||(mounter->current_step>=mounter->move_number))
			list.SelectItem(NULL);
		else
			list.SelectItem(mounter->move[mounter->current_step]->move_list_id[1]);
	}
	display_step();
}
void Movement_list::OnModify(NMHDR* pNMHDR, LRESULT* pResult)
{
	CMainFrame*  pFrame=(CMainFrame*)(theApp.m_pMainWnd);
	if(movement_list_modify_flag&&(pFrame!=NULL)){

		theApp.ws->root_component->clear_select_flag();

		theApp.ws->match_component_ids.reset();

		HTREEITEM id=list.GetSelectedItem();		
		Part_collector collector(theApp.ws->parts);
		int main_step=0,sub_step=0,component_number=0;
		CString str,sound_file;
		double current_time=Platform_information::get_current_time();
		
		for(int i=0;i<theApp.mounter->move_number;i++){
			bool find_flag=false;
			Movement *m=theApp.mounter->move[i];
			for(int j=0;j<(m->match_number);j++){
				Component *comp;
				Movement_match *m_m=&(m->component_match[j]);

				if(m_m->match_list_id==id){
							find_flag=true;
				}else if(m_m->source_list_id==id){
							if((comp=theApp.ws->root_component->get_node_by_name(m_m->source_name))!=NULL)
								theApp.ws->match_component_ids.reset_from_match(comp,m_m->source_body_id,m_m->source_face_id,theApp.ws->parts);
							find_flag=true;
				}else if(m_m->destination_list_id==id){
							if((comp=theApp.ws->root_component->get_node_by_name(m_m->destination_name))!=NULL)
								theApp.ws->match_component_ids.reset_from_match(comp,m_m->destination_body_id,m_m->destination_face_id,theApp.ws->parts);
							find_flag=true;
				}
			}
			if(find_flag||(m->move_list_id[0]==id)||(m->move_list_id[1]==id)||(root==id)){
				main_step=m->main_step;
				sub_step=m->sub_step;
				str=m->text;
				sound_file=m->sound_file;
				if(m->comp!=NULL){
					m->comp->selected_time=current_time;
					collector.collect(m->comp);
					if(!find_flag)
						m->comp->set_component_selected();
				}
			}
		}
		collector.sort();
		if(collector.box_component_number>0)
			theApp.ws->mount_on_box(
				(theApp.ws->match_component_ids.has_box_flag)
					?(theApp.ws->match_component_ids.box)
					:(collector.component_box),
				1.5,theApp.time_length);
		
		display_step();
		if(virtual_mount_flag)
			collector.clear();

		pFrame->m_property_list.display_mount_message(collector,main_step,sub_step,str,sound_file);
	}
}

void Movement_list::OnMountStepDivision()
{
	if(theApp.mounter->move_number<=1)
		return;

	HTREEITEM select_id=list.GetSelectedItem(),main_id;
	int i,j,main_step,next_main_step=theApp.mounter->move[theApp.mounter->move_number-1]->main_step;

	for(i=0;i<(theApp.mounter->move_number);i++)
		if(theApp.mounter->move[i]->move_list_id[1]==select_id){
			main_step=theApp.mounter->move[i]->main_step;
			main_id=theApp.mounter->move[i]->move_list_id[0];
			{
				bool flag=true;
				for(j=i-1;j>=0;j--){
					if(main_step==(theApp.mounter->move[j]->main_step))
						if((theApp.mounter->move[j]->sub_step)!=(theApp.mounter->move[i]->sub_step)){
							flag=false;
							break;
						}
				}
				if(flag){
					AfxMessageBox(_T("无法拆分起始次拼装步骤"));
					return;
				}
			}
			for(j=i;j<(theApp.mounter->move_number);j++){
				if(theApp.mounter->move[j]->main_step>main_step)
					if(theApp.mounter->move[j]->main_step<next_main_step)
						next_main_step=theApp.mounter->move[j]->main_step;
			}
			if(next_main_step==main_step)
				next_main_step+=100;
			else if((next_main_step-main_step)<=1){
				AfxMessageBox(_T("连续主拼装步骤编号,无法拆分次拼装步骤编号"));
				return;
			}else
				next_main_step=main_step+(next_main_step-main_step)/2;
		
			theApp.mounter->push_movement();

			for(j=i;j<(theApp.mounter->move_number);j++){
				if(theApp.mounter->move[j]->main_step!=main_step)
					break;
				theApp.mounter->move[j]->main_step=next_main_step;
			}
			do_file_list(false);
			list.SelectItem(theApp.mounter->move[i]->move_list_id[1]);
			theApp.redraw_flag=true;

			return;
		}
}
void Movement_list::OnMountStepMerge()
{
	if(theApp.mounter->move_number<=1)
		return;

	HTREEITEM select_id=list.GetSelectedItem();
	int i,j,main_step,max_sub_step,next_main_step;

	for(i=0;i<(theApp.mounter->move_number);i++){
		if(theApp.mounter->move[i]->move_list_id[0]==select_id){
			main_step=theApp.mounter->move[i]->main_step;
			for(j=i,next_main_step=main_step;j<(theApp.mounter->move_number);j++)
				if((next_main_step=theApp.mounter->move[j]->main_step)!=main_step)
					break;
			if(next_main_step==main_step)
				return;

			max_sub_step=theApp.mounter->move[i]->sub_step;
			for(j=i;j<(theApp.mounter->move_number);j++)
				if(theApp.mounter->move[j]->main_step==main_step)
					if(theApp.mounter->move[j]->sub_step>max_sub_step)
						max_sub_step=theApp.mounter->move[j]->sub_step;
			max_sub_step++;
			for(j=i;j<(theApp.mounter->move_number);j++)
				if(theApp.mounter->move[j]->main_step==next_main_step){
					max_sub_step-=theApp.mounter->move[j]->sub_step;
					break;
				}			
			for(;i<(theApp.mounter->move_number);i++)
				if(theApp.mounter->move[i]->main_step==next_main_step){	
					theApp.mounter->push_movement();
					for(j=i;j<(theApp.mounter->move_number);j++){
						if(theApp.mounter->move[j]->main_step!=next_main_step)
							break;
						theApp.mounter->move[j]->main_step=main_step;
						theApp.mounter->move[j]->sub_step+=max_sub_step;
					}
					do_file_list(false);
					list.Expand(theApp.mounter->move[i]->move_list_id[0],TVE_EXPAND);
					list.SelectItem(theApp.mounter->move[i]->move_list_id[0]);
					theApp.redraw_flag=true;

					return ;
				}
			return;
		}
	}
}
void Movement_list::OnMountTogether()
{
	if(theApp.mounter->move_number<=0)
		return;

	Movement *m;
	HTREEITEM select_id=list.GetSelectedItem();

	for(int i=0;i<(theApp.mounter->move_number);i++){
		if((m=theApp.mounter->move[i])->move_list_id[1]==select_id){
			int main_step=m->main_step,sub_step=m->sub_step;
			int start_step_id=i;
			for(;i<(theApp.mounter->move_number);i++){
				if((m=theApp.mounter->move[i])->main_step!=main_step)
					return;
				if(m->sub_step!=sub_step){
					theApp.mounter->push_movement();					
					for(int old_sub_step=m->sub_step;i<(theApp.mounter->move_number);i++){
						if((m=theApp.mounter->move[i])->main_step!=main_step)
							break;
						if(m->sub_step!=old_sub_step)
							break;
						m->sub_step=sub_step;
					}
					do_file_list(false);
					select_id=theApp.mounter->move[start_step_id]->move_list_id[1];
					list.Expand(select_id,TVE_EXPAND);
					list.SelectItem(select_id);					
					theApp.redraw_flag=true;
					return;
				}
			}
			return;
		}
	}
	return;
}

void Movement_list::OnMountMainStepSort()
{
	Mount_step_dialog dlg(this,_T("输入主拼装步骤步长"));
	if(dlg.DoModal()!=IDOK)
		return;
	
	int i,j,k,main_step;

	theApp.mounter->push_movement();

	for(i=0,j=dlg.step;i<(theApp.mounter->move_number);j+=dlg.step){
		for(k=i,main_step=theApp.mounter->move[i]->main_step;k<(theApp.mounter->move_number);k++)
			if(theApp.mounter->move[k]->main_step!=main_step)
				break;
		for(;i<k;i++)
			theApp.mounter->move[i]->main_step=j;
	}

	do_file_list(false);
	theApp.redraw_flag=true;
	return;
}
void Movement_list::OnMountSubStepSort()
{
	HTREEITEM select_id=list.GetSelectedItem();
			
	Mount_step_dialog dlg(this,_T("输入次拼装步骤步长"));
	if(dlg.DoModal()!=IDOK)
		return;
	for(int i=0;i<(theApp.mounter->move_number);i++)
		if(theApp.mounter->move[i]->move_list_id[0]==select_id){
			theApp.mounter->push_movement();
			int main_step=theApp.mounter->move[i]->main_step;
			for(int k,j=dlg.step;i<(theApp.mounter->move_number);j+=dlg.step){
				if(theApp.mounter->move[i]->main_step!=main_step)
					break;
				int sub_step=theApp.mounter->move[i]->sub_step;
				for(k=i;k<(theApp.mounter->move_number);k++){
					if(theApp.mounter->move[k]->main_step!=main_step)
						break;
					if(theApp.mounter->move[k]->sub_step!=sub_step)
						break;
				}
				for(;i<k;i++)
					theApp.mounter->move[i]->sub_step=j;
			}
			do_file_list();

			theApp.redraw_flag=true;
			return;
		}
}
void Movement_list::OnMountStepMove()
{
	int begin_step,end_step,target_step;

	if(theApp.mounter->move_number<=1)
		return;
	
	HTREEITEM select_id=list.GetSelectedItem();

	{
		Mount_step_dialog dlg(this,_T("输入起始主拼装步骤"));
		if(dlg.DoModal()!=IDOK)
			return;
		begin_step=dlg.step;
	}
	{
		Mount_step_dialog dlg(this,_T("输入终止主拼装步骤"));
		if(dlg.DoModal()!=IDOK)
			return;
		end_step=dlg.step;
	}
	if(begin_step>end_step){
		target_step=end_step;
		end_step=begin_step;
		begin_step=target_step;
	}
	
	for(int i=0;i<(theApp.mounter->move_number);i++)
		if((theApp.mounter->move[i]->move_list_id[0]==select_id)||(theApp.mounter->move[i]->move_list_id[1]==select_id)){
			
			theApp.mounter->push_movement();

			Movement *m=theApp.mounter->move[i];
			target_step=m->main_step;
			if(target_step<begin_step)
				for(i=0;i<(theApp.mounter->move_number);i++){
					int main_step=theApp.mounter->move[i]->main_step;
					if((main_step>=begin_step)&&(main_step<=end_step))
						theApp.mounter->move[i]->main_step=target_step+main_step-begin_step;
					else if((main_step<begin_step)&&(main_step>=target_step))
						theApp.mounter->move[i]->main_step=main_step+end_step-begin_step+1;
				}
			else if(target_step>end_step)
				for(i=0;i<(theApp.mounter->move_number);i++){
					int main_step=theApp.mounter->move[i]->main_step;
					if((main_step>=begin_step)&&(main_step<=end_step))
						theApp.mounter->move[i]->main_step=main_step+target_step-end_step;
					else if((main_step<=target_step)&&(main_step>end_step))
						theApp.mounter->move[i]->main_step=main_step-(end_step-begin_step)-1;
				}
			else{
				AfxMessageBox(_T("目标主拼装步骤编号在移动范围之内,因此无法移动"));
				return;
			}

			theApp.mounter->sort();
			do_file_list(false);
			theApp.redraw_flag=true;

			movement_list_modify_flag=false;
			list.Expand(m->move_list_id[0],TVE_EXPAND);
			list.SelectItem(m->move_list_id[0]);

			return;
		}
}

void Movement_list::OnMountStepExchange()
{
	HTREEITEM select_id=list.GetSelectedItem();
	if(theApp.mounter->move_number<=1)
		return;
	int i,main_step_id,other_main_step_id;
	for(i=0;;i++){
		if(i>=(theApp.mounter->move_number))
				return;
		if((theApp.mounter->move[i]->move_list_id[0]==select_id)||(theApp.mounter->move[i]->move_list_id[1]==select_id)){
			main_step_id=theApp.mounter->move[i]->main_step;
			break;
		}
	}
	for(;;i++){
		if(i>=(theApp.mounter->move_number))
			return;
		if(main_step_id!=(theApp.mounter->move[i]->main_step)){
			other_main_step_id=theApp.mounter->move[i]->main_step;
			break;
		}
	}
	theApp.mounter->push_movement();
	for(i=0;i<(theApp.mounter->move_number);i++){
		if(main_step_id==(theApp.mounter->move[i]->main_step))
			theApp.mounter->move[i]->main_step=other_main_step_id;
		else if(other_main_step_id==(theApp.mounter->move[i]->main_step))
			theApp.mounter->move[i]->main_step=main_step_id;
	}
	theApp.mounter->sort();
	do_file_list(false);
	theApp.redraw_flag=true;

	for(i=0;i<(theApp.mounter->move_number);i++)
		if(other_main_step_id==(theApp.mounter->move[i]->main_step)){
			movement_list_modify_flag=false;
			list.Expand(theApp.mounter->move[i]->move_list_id[0],TVE_EXPAND);
			list.SelectItem(theApp.mounter->move[i]->move_list_id[0]);
			return;
		}
}
void Movement_list::OnMountTitleAndSound()
{
	int i,j,main_step=0,sub_step=0;
	CString sound_file=_T("没有配音文件"),title=_T("没有字幕");
	HTREEITEM select_id=list.GetSelectedItem();
	Movement *m;

	for(i=0;i<theApp.mounter->move_number;i++){
		for(j=0,m=theApp.mounter->move[i];j<(sizeof(m->move_list_id)/sizeof(m->move_list_id[0]));j++)
			if(m->move_list_id[j]==select_id){
				main_step=m->main_step;
				sub_step=m->sub_step;
				sound_file=m->sound_file;
				title=m->text;

				i=theApp.mounter->move_number;
				break;
			}
	}		
	if(Mount_information_dialog(&main_step,&sub_step,&sound_file,&title,this).DoModal()!=IDOK)
		return;
	for(i=0;;i++){
		if(i>=(sound_file.GetLength()))
			break;
		switch(sound_file.GetAt(i)){
		case  ' ':
		case '\b':
		case '\f':
		case '\n':
		case '\r':
		case '\t':
		case '\v':
			sound_file=sound_file.Left(i)+sound_file.Right(sound_file.GetLength()-i-1);
			i--;
			break;
		}
	}
	for(i=0;;i++){
		if(i>=(title.GetLength()))
			break;
		switch(title.GetAt(i)){
		case  ' ':
		case '\b':
		case '\f':
		case '\n':
		case '\r':
		case '\t':
		case '\v':
			title=title.Left(i)+title.Right(title.GetLength()-i-1);
			i--;
			break;
		}

	}
	for(i=0;i<theApp.mounter->move_number;i++){
		if((m=theApp.mounter->move[i])->main_step==main_step){
			m->sound_file=sound_file;
			m->text=title;
		}
	}
}
void Movement_list::OnMountTitleFile()
{
	theApp.mounter->create_title_file();
}
void Movement_list::OnMountStepTag()
{
	HTREEITEM select_id=list.GetSelectedItem();
	list.SetFocus();
	for(int i=0;i<(theApp.mounter->move_number);i++){
		Movement *m=theApp.mounter->move[i];
		if(m->move_list_id[0]==select_id){			
			CString str,new_str=_T("");
			if((str=m->main_tag)==_T("没有标记"))
				str=_T("");
			Mount_step_tag_dialog dlg(str,popup_menu_location.x,popup_menu_location.y,this);

			if(dlg.DoModal()== IDOK){
				theApp.mounter->push_movement();

				str=dlg.tag_string;
				for(int j=0;j<(str.GetLength());j++)
					if((str.GetAt(j)!=' ')&&(str.GetAt(j)!='\t'))
						new_str=new_str+(str.GetAt(j));
				if(new_str.GetLength()<=0)
					new_str=_T("没有标记");
				for(int j=i;j<(theApp.mounter->move_number);j++){
					if(theApp.mounter->move[j]->move_list_id[0]!=select_id)
						break;
					theApp.mounter->move[j]->main_tag=new_str;
				}
				str.Format(_T("%d"),m->main_step);
				if(new_str!=_T("没有标记"))
					str=str+_T(":")+new_str;
				list.SetItemText(select_id,str);
			}
			return;
		}else if(m->move_list_id[1]==select_id){
			theApp.mounter->push_movement();
			CString str,new_str=_T("");
			if((str=m->sub_tag)==_T("没有标记"))
				str=_T("");
			Mount_step_tag_dialog dlg(str,popup_menu_location.x,popup_menu_location.y,this);

			if(dlg.DoModal()== IDOK){
				str=dlg.tag_string;
				for(int j=0;j<(str.GetLength());j++)
					if((str.GetAt(j)!=' ')&&(str.GetAt(j)!='\t'))
						new_str=new_str+(str.GetAt(j));
				if(new_str.GetLength()<=0)
					new_str=_T("没有标记");
				for(int j=i;j<(theApp.mounter->move_number);j++){
					if(theApp.mounter->move[j]->move_list_id[1]!=select_id)
						break;
					theApp.mounter->move[j]->sub_tag=new_str;
				}
				str.Format(_T("%d"),m->sub_step);
				if(new_str!=_T("没有标记"))
					str=str+_T(":")+new_str;
				list.SetItemText(select_id,str);
			}
			return;
		}else{
			for(int i=0;i<(m->match_number);i++){
				if(m->component_match[i].match_list_id==select_id){
					theApp.mounter->push_movement();
					CString str,new_str=_T("");
					if((str=m->component_match[i].match_tag)==_T("没有标记"))
						str=_T("");
					Mount_step_tag_dialog dlg(str,popup_menu_location.x,popup_menu_location.y,this);

					if(dlg.DoModal()== IDOK){
						str=dlg.tag_string;
						for(int j=0;j<(str.GetLength());j++)
							if((str.GetAt(j)!=' ')&&(str.GetAt(j)!='\t'))
								new_str=new_str+(str.GetAt(j));
						str=new_str;
						if(new_str.GetLength()<=0){
							str=_T(" ");
							new_str=_T("没有标记");
						}
						m->component_match[i].match_tag=new_str;
						list.SetItemText(select_id,str);
					}
					return;
				}
			}
		}
	}
}

void Movement_list::OnMountSaveToCache()
{
	Component *comp;
	{
		class temp:public Component_collector_base
		{
		public:
			Component *pp;
			virtual bool operate_component(Component *comp,bool my_selected_flag)
			{
				if((comp->selected_time)>(pp->selected_time))
					pp=comp;
				return true;
			}	
			temp(Component *my_p)
			{
				pp=my_p;
				collect(pp);
			}
		};
		if((comp=temp(theApp.ws->root_component).pp)==NULL){
			AfxMessageBox(_T("无法取得保存杆件或装配体"));
			return;
		}
		if(comp->parent==NULL){
			AfxMessageBox(_T("保存杆件或装配体不能是根组件"));
			return;
		}
		if(comp->parent->parent==NULL){
			AfxMessageBox(_T("保存杆件或装配体不能是根组件"));
			return;
		}
	}
	Mount_cache_information_dialog dlg(this);

	if(dlg.DoModal()!=IDOK)
		return;
	theApp.mounter->save_to_cache(dlg.title,comp,dlg.start_main_step,dlg.start_sub_step,dlg.end_main_step,dlg.end_sub_step);
}
void Movement_list::OnMountStepDelete()
{
	int i,j;
	HTREEITEM select_id=list.GetSelectedItem();
	theApp.mounter->push_movement();

	for(i=0,j=0;i<theApp.mounter->move_number;i++){
		Movement *m=theApp.mounter->move[i];	
		if((m->move_list_id[0]==select_id)||(m->move_list_id[1]==select_id)){
			if(theApp.mounter->move[i]->comp!=NULL)
				theApp.mounter->move[i]->comp->reset_component(*(theApp.ws->parts));
			delete (theApp.mounter->move[i]);
			theApp.mounter->move[i]=NULL;
		}else{
			theApp.mounter->move[j]=m;
			j++;

			bool do_delete_flag;
			do_delete_flag=false;
			for(int k=0;k<(m->match_number);k++){
				Movement_match *move_match=&(m->component_match[k]);
				if(move_match->match_list_id==select_id){
					move_match->reset();
					do_delete_flag=true;
				}else if(move_match->source_list_id==select_id){
					move_match->reset_source();					
					do_delete_flag=true;
				}else if(move_match->destination_list_id==select_id){
					move_match->reset_destination();					
					do_delete_flag=true;
				}
			}
			if(do_delete_flag){
				int k1,k2;
				for(k1=0,k2=0;k1<(m->match_number);k1++){
					Movement_match *move_match=&(m->component_match[k1]);
					if((move_match->destination_body_id>=0)||(move_match->source_body_id>=0)){
						if(k1!=k2)
							m->component_match[k2].copy_from(m->component_match[k1]);
						k2++;
					}
				}
				for(m->match_number=k2;k2<MAX_MATCH_NUMBER;k2++)
					m->component_match[k2].reset();
			}
		}
	}
	for(theApp.mounter->move_number=j;j<MAX_MOVEMENT_NUMBER;j++)
		theApp.mounter->move[j]=NULL;

	theApp.mounter->reset_visible_state();

	do_file_list();

	theApp.redraw_flag=true;

}

void Movement_list::OnMountRestoreDesign()
{
	if(!(theApp.mounter->do_push_flush_movement_flag)){
		AfxMessageBox(_T("没有打开随时保存，不能执行恢复拼装信息"));
		return;
	}
	HTREEITEM select_id=list.GetSelectedItem();
	int main_step=-1,sub_step=-1;
	for(int i=0;i<(theApp.mounter->move_number);i++){
		Movement *m=theApp.mounter->move[i];
		if((m->move_list_id[0]==select_id)||(m->move_list_id[1]==select_id)){
			main_step=m->main_step;
			sub_step=m->sub_step;
			break;
		}
	}
	if(theApp.mounter->pop_movement()){
		theApp.redraw_flag=true;
		do_file_list();
	}
}
void Movement_list::OnMountSaveDesign()
{	
	theApp.mounter->do_push_flush_movement_flag=theApp.mounter->do_push_flush_movement_flag?false:true;
}
void Movement_list::OnUpdateMountSaveDesign(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(theApp.mounter->do_push_flush_movement_flag);
	pCmdUI->SetText(theApp.mounter->do_push_flush_movement_flag?_T("执行随时保存"):_T("不执行随时保存"));

	OnEnableDisable(pCmdUI);
}
void Movement_list::OnMountAllStepMerge()
{
	if(theApp.mounter->move_number<=1)
		return;
	theApp.mounter->push_movement();

	int main_step=theApp.mounter->move[0]->main_step;
	int sub_step=theApp.mounter->move[0]->sub_step;

	for(int i=0,j=0;i<(theApp.mounter->move_number);i=j,sub_step=(theApp.mounter->move[j-1]->sub_step)+1){
		int current_main_step=theApp.mounter->move[i]->main_step;
		int adjust_sub_step=sub_step-(theApp.mounter->move[i]->sub_step);
		
		for(j=i;j<(theApp.mounter->move_number);j++)
			if(theApp.mounter->move[j]->main_step!=current_main_step)
				break;
		for(;i<j;i++){
			theApp.mounter->move[i]->main_step=main_step;
			theApp.mounter->move[i]->sub_step+=adjust_sub_step;
		}
	}
	do_file_list();
	return;
}
void Movement_list::OnMountStepForce()
{
	Movement *m;
	{
		Component_collector_for_selected_component sel_comp(theApp.ws->root_component);
		if(sel_comp.component_number<=0){
			AfxMessageBox(_T("没有选中杆件"));
			return;
		}
		if(sel_comp.component_number>1){
			AfxMessageBox(_T("每次只能指定一个杆件或装配体的拼装步骤\n您选中了多个杆件或装配体"));
			return;
		}
		int old_step=sel_comp.component_pointer[0]->movement_id;
		if((old_step<0)||(old_step>=(theApp.mounter->move_number))){
			AfxMessageBox(_T("指定的杆件或装配体没有相关的拼装信息"));
			return;
		}
		m=theApp.mounter->move[old_step];
	}
	HTREEITEM select_id=list.GetSelectedItem();
	for(int i=0;i<(theApp.mounter->move_number);i++)
		if((theApp.mounter->move[i]->move_list_id[0]==select_id)||(theApp.mounter->move[i]->move_list_id[1]==select_id)){
			theApp.mounter->push_movement();

			m->main_step=theApp.mounter->move[i]->main_step;
			m->sub_step=theApp.mounter->move[i]->sub_step;
			theApp.mounter->sort();

			do_file_list();

			list.SelectItem(m->move_list_id[1]);

			return;
		}
}
void Movement_list::OnMountStepForceManual()
{
	Movement *m;
	{
		Component_collector_for_selected_component sel_comp(theApp.ws->root_component);
		if(sel_comp.component_number<=0){
			AfxMessageBox(_T("没有选中杆件"));
			return;
		}
		if(sel_comp.component_number>1){
			AfxMessageBox(_T("每次只能指定一个杆件或装配体的拼装步骤\n您选中了多个杆件或装配体"));
			return;
		}
		int old_step=sel_comp.component_pointer[0]->movement_id;
		if((old_step<0)||(old_step>=(theApp.mounter->move_number))){
			AfxMessageBox(_T("指定的杆件或装配体没有相关的拼装信息"));
			return;
		}
		m=theApp.mounter->move[old_step];
	}
	Mount_step_dialog main_dlg(this,_T("输入主拼装步骤"));
	if(main_dlg.DoModal()!=IDOK)
		return;

	Mount_step_dialog sub_dlg(this,_T("输入次拼装步骤"));
	if(sub_dlg.DoModal()!=IDOK)
		return;

	theApp.mounter->push_movement();

	m->main_step=main_dlg.step;
	m->sub_step=sub_dlg.step;
	theApp.mounter->sort();

	do_file_list();
	
}
void Movement_list::add_match()
{
	Component *comp;
	Movement *m;
	HTREEITEM select_id=list.GetSelectedItem();

	if((comp=theApp.ws->match_component_ids.comp)==NULL)
		return;
	if(theApp.ws->match_component_ids.body==NULL){
		AfxMessageBox(_T("没有选中体"));
		return;
	}
	if(theApp.ws->match_component_ids.face==NULL){
		AfxMessageBox(_T("没有选中面"));
		return;
	}
/*
	if(theApp.ws->match_component_ids.face_loop!=NULL){
		AfxMessageBox(_T("选中的是边线"));
		return;
	}
	if(theApp.ws->match_component_ids.edge!=NULL){
		AfxMessageBox(_T("选中的是边线"));
		return;
	}
*/
	theApp.redraw_flag=true;
	for(int i=0;i<(theApp.mounter->move_number);i++){
		if(((m=theApp.mounter->move[i])->move_list_id[1])==select_id){
			if(m->comp!=NULL){
				for(int i=0;i<MAX_MATCH_NUMBER;i++){
					if(m->component_match[i].source_body_id<0){
						if((m->name)==(comp->name.Left(m->name.GetLength()))){

							theApp.mounter->push_movement();

							m->component_match[i].source_name=comp->name;
							m->component_match[i].source_body_id=theApp.ws->match_component_ids.body_id;
							m->component_match[i].source_face_id=theApp.ws->match_component_ids.face_id;
							if(m->component_match[i].destination_body_id<0)
								m->match_number=i+1;
							

							movement_list_modify_flag=false;
				
							do_file_list(false);

							list.SelectItem(m->component_match[i].source_list_id);
							list.SelectItem(m->move_list_id[1]);
							return;
						}
					}else{
						if(m->component_match[i].destination_body_id<0){
							if((m->name)!=(comp->name.Left(m->name.GetLength()))){
								
								theApp.mounter->push_movement();

								m->component_match[i].destination_name=comp->name;
								m->component_match[i].destination_body_id=theApp.ws->match_component_ids.body_id;
								m->component_match[i].destination_face_id=theApp.ws->match_component_ids.face_id;
						

								movement_list_modify_flag=false;
								do_file_list(false);
								list.SelectItem(m->component_match[i].destination_list_id);
								list.SelectItem(m->move_list_id[1]);

								return;
							}
						}
					}
				}
			}
		}
	}
	AfxMessageBox(_T("没有成功指定配合面\n指定的拼装步骤错\n或指定的杆件面错,第一次选择拼装面，第二次选择配合面"));
}

void  Movement_list::OnEnableDisable(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(virtual_mount_flag?FALSE:TRUE);
}
