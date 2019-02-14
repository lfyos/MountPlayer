#include "StdAfx.h"
#include "Part_list.h"
#include "../mainfrm.h"
#include "../Resource.h"
#include "../Shader_player.h"


BEGIN_MESSAGE_MAP(Part_list,List_base)
	ON_COMMAND(ID_PROPERTIES, OnRedrawTreeList)
	ON_NOTIFY(TVN_SELCHANGED, 4, OnModify)
END_MESSAGE_MAP()

void Part_list::file_part_list(Component *p,Part_collector &collector)
{
	Component *q;
	CString str,str1;
	int i,mount_number,unmount_number,other_number;

	if(p==NULL)
		return;

	if(p->parent==NULL){		
		for(i=0,mount_number=0,unmount_number=0,other_number=0;i<(collector.component_number);i++){
			int movement_id=collector.component_pointer[i]->movement_id;

			if((movement_id<0)||(movement_id>=(theApp.mounter->move_number)))
				other_number++;
			else if(movement_id>=(theApp.mounter->current_step))
				unmount_number++;
			else
				mount_number++;
		}		
		str.Format(_T("杆件总计%d件,其中"),collector.component_number);
		if(mount_number>0){
			str1.Format(_T("已拼装%d件"),mount_number);
			str=str+str1;
		}
		if(unmount_number>0){
			str1.Format(_T(",未拼装%d件"),unmount_number);
			str=str+str1;
		}
		if(other_number>0){
			str1.Format(_T(",不拼装%d件"),other_number);
			str=str+str1;
		}
	}else{
		str=p->id_name;
		for(i=str.GetLength()-1;i>=0;i--)
			if(str.GetAt(i)==CString(_T("."))){
				str=str.Left(i);
				break;
			}
		for(i=str.GetLength()-1;i>=0;i--)
			if((str.GetAt(i)==CString(_T("\\")))||(str.GetAt(i)==CString(_T("\\")))){
				str=str.Right(str.GetLength()-i-1);
				break;
			}
	}
	if((q=p->child)!=NULL){
		if(p->parent==NULL){
			list.DeleteAllItems();
			p->component_list_id[2]=list.InsertItem(str,1,1);
		}else{
			p->component_list_id[2]=list.InsertItem(str,1,1,p->parent->component_list_id[2]);
		}
		do{
			file_part_list(q,collector);
		}while((q=q->left_brother)!=(p->child));

		if(p->parent==NULL){
			list.SetItemState(p->component_list_id[2],TVIS_BOLD, TVIS_BOLD);
			list.Expand(p->component_list_id[2], TVE_EXPAND);
		}
		return;
	}

	int begin_index,end_index;
	if(!(collector.get_begin_and_end_part(p->part_id,begin_index,end_index))){
		if(((p->part_id)<0)||((p->part_id)>=(theApp.ws->parts->part_number))){			
			str.Format(_T("无法确定杆件名称:%d,"),p->part_id);
			str=str+p->id_name;
		}else	
			str=theApp.ws->parts->part_array[p->part_id]->user_name;
		p->component_list_id[2]=list.InsertItem(str,1,1,(p->parent==NULL)?NULL:(p->parent->component_list_id[2]));

		return;
	}
	for(i=begin_index,mount_number=0,unmount_number=0,other_number=0;i<=end_index;i++){
		int movement_id=collector.component_pointer[i]->movement_id;
		
		if((movement_id<0)||(movement_id>=(theApp.mounter->move_number)))
			other_number++;
		else if(movement_id>=(theApp.mounter->current_step))
			unmount_number++;
		else
			mount_number++;
	}
	str1.Format(_T(":杆件总数%d"),mount_number+unmount_number+other_number);
	if(mount_number>0){
			str.Format(_T(",已拼装%d件"),mount_number);
			str1=str1+str;
	}
	if(unmount_number>0){
			str.Format(_T(",未拼装%d件"),unmount_number);
			str1=str1+str;
	}
	if(other_number>0){
			str1.Format(_T(",不拼装%d件"),other_number);
			str=str+str1;
	}
	if((p->part_id<0)||(p->part_id>=(theApp.ws->parts->part_number))){
		str.Format(_T("无法确定杆件名称:%d,"),p->part_id);
		str=str+p->id_name;
	}else	
		str=theApp.ws->parts->part_array[p->part_id]->user_name;
	p->component_list_id[2]=list.InsertItem(str+str1,1,1,p->parent->component_list_id[2]);
	for(int i=begin_index;i<=end_index;i++){
		str.Format(_T("%4d"),1+i-begin_index);
		int movement_id=collector.component_pointer[i]->movement_id,main_step=-1,sub_step=-1;
		if((movement_id<0)||(movement_id>=(theApp.mounter->move_number)))
			collector.component_pointer[i]->component_list_id[2]=list.InsertItem(str+_T(":非拼装杆件"),4,4,p->component_list_id[2]);
		else{
			main_step=theApp.mounter->move[movement_id]->main_step;
			sub_step=theApp.mounter->move[movement_id]->sub_step;
			str1.Format(_T(",拼装步骤%d/%d"),main_step,sub_step);

			if(movement_id>=(theApp.mounter->current_step))
				collector.component_pointer[i]->component_list_id[2]=list.InsertItem(str+_T(":未拼装")+str1,5,5,p->component_list_id[2]);
			else
				collector.component_pointer[i]->component_list_id[2]=list.InsertItem(str+_T(":已拼装")+str1,3,3,p->component_list_id[2]);				
		}
	}
}
void Part_list::create_select_collector(Component *p,Part_collector &collector,Part_collector &selected_collector,HTREEITEM id,bool flag)
{
	if(p==NULL)
		return;
	if(p->component_list_id[2]==id)
		flag=true;

	Component *q;
	if((q=p->child)!=NULL){
		do{
			create_select_collector(q,collector,selected_collector,id,flag);
		}while((q=q->left_brother)!=(p->child));
		return;
	}
	if(flag){		
		int begin_index,end_index;
		if(collector.get_begin_and_end_part(p->part_id,begin_index,end_index))
			for(int i=begin_index;i<=end_index;i++)
				selected_collector.collect(collector.component_pointer[i]);
	}
}

void Part_list::OnModify(NMHDR* pNMHDR, LRESULT* pResult)
{
	CMainFrame*  pFrame=(CMainFrame*)(theApp.m_pMainWnd);
	if(part_list_modify_flag&&(pFrame!=NULL)){
		HTREEITEM id=list.GetSelectedItem();
		Part_collector collector(theApp.ws->root_component,theApp.ws->parts),display_collector(theApp.ws->parts);
		double current_time=Platform_information::get_current_time();
		create_select_collector(theApp.ws->part_list_root_component,collector,display_collector,id);
		for(int i=0;i<collector.component_number;i++)
			if(collector.component_pointer[i]->component_list_id[2]==id)
				display_collector.collect(collector.component_pointer[i]);

		pFrame->m_property_list.display_part_collector(display_collector);
		if(display_collector.box_component_number>0)
			theApp.ws->mount_on_box(display_collector.component_box,1.5,theApp.time_length);
	}
}