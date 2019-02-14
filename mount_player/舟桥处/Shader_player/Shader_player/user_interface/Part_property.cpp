// Part_property.cpp : implementation file
//

#include "stdafx.h"
#include "../Shader_player.h"
#include "Part_property.h"

#include "../mainfrm.h"

// Part_property

IMPLEMENT_DYNAMIC(Part_property, CPropertiesWnd)


BEGIN_MESSAGE_MAP(Part_property, CPropertiesWnd)
	ON_COMMAND(ID_DISPLAY_ALL_COMPONENT,OnDisplayAllComponent)	
	ON_COMMAND(ID_LIST_SELECTION,OnDisplaySelection)
	ON_COMMAND(ID_CLEAR_LIST,OnClearDisplay)
	ON_COMMAND(ID_SELECTED_COMPONENT_LIST, OnMarkSelection)
	ON_COMMAND(ID_BACKWARD,OnBackWard)
	ON_COMMAND(ID_VIRTUAL_MOUNT,OnVirtualMount)
	
END_MESSAGE_MAP()

// Part_property message handlers

void  Part_property::display_part_collector_process()
{
	class Part_property_item:public CMFCPropertyGridProperty
	{
	public:
		Component *comp;
	
		virtual ~Part_property_item()
		{
			comp->display_flag&=~2;
		}
		Part_property_item(Component *my_comp,CString name,CString value,CString descr):CMFCPropertyGridProperty(name,value,descr)
		{
			comp=my_comp;
			comp->display_flag|=2;
		}
		virtual void OnClickName(CPoint C )
		{
			comp->display_flag|=2;
			if(comp->selected_flag)
				comp->clear_select_flag();
			else
				comp->set_component_selected();	
			theApp.redraw_flag=true;
			if(comp->component_box_flag)
				theApp.ws->mount_on_box(comp->component_box,1.5,theApp.time_length);
			CMainFrame*  pFrame=(CMainFrame*)(theApp.m_pMainWnd);
			if(pFrame!=NULL){
				pFrame->m_component_list.select_component(comp);
				pFrame->m_part_list.select_component(comp);
				pFrame->m_movement_list.select_component(comp);	
			}
			if(comp->part_id>=0){
				Sound_player::turn_sound_on_off();
				Sound_player::turn_sound_on_off();
				Sound_player::play((theApp.ws->parts->part_array[comp->part_id]->sound_file_name));
			}
			comp->selected_time=Platform_information::get_current_time();
		}
	};

	class Part_property_item_group:public CMFCPropertyGridProperty
	{
		bool state_flag;
	public:
		Part_collector collector;

		Part_property_item_group(Part_set *ps,CString name):CMFCPropertyGridProperty(name),collector(ps)
		{	
			state_flag=false;
		}
		virtual void OnClickName(CPoint)
		{
			double current_time=Platform_information::get_current_time();

			state_flag=state_flag?false:true;
			theApp.redraw_flag=true;
			
			for(int i=0;i<(collector.component_number);i++){
				collector.component_pointer[i]->display_flag|=2;
				if(state_flag)
					collector.component_pointer[i]->set_component_selected();
				else
					collector.component_pointer[i]->clear_select_flag();
				collector.component_pointer[i]->selected_time=current_time;
			}
			CMainFrame*  pFrame=(CMainFrame*)(theApp.m_pMainWnd);
			if((pFrame!=NULL)&&(collector.box_component_number>0))
				theApp.ws->mount_on_box(collector.component_box,1.5,theApp.time_length);

			if(collector.component_number>0)
				if(collector.component_pointer[0]->part_id>=0){
					Sound_player::turn_sound_on_off();
					Sound_player::turn_sound_on_off();
					Sound_player::play((theApp.ws->plat_info->directory_name)+(theApp.ws->parts->part_array[collector.component_pointer[0]->part_id]->sound_file_name));
				}
		}
	};

	CString str,discr;
	int i,j,k,m,type_number;

	theApp.redraw_flag=true;
	Part_collector *p=get_stack_top();

	if(p==NULL)
		return;
	if(p->component_number<=0)
		return;	

	for(i=0,type_number=0;i<(p->component_number);i=j,type_number++){		
		for(j=i+1;j<(p->component_number);j++)
			if((p->component_pointer[i]->part_id)!=(p->component_pointer[j]->part_id))
				break;
	}

	str.Format(_T("杆件共计  %d  种  %d  件"),type_number,p->component_number);

	CMFCPropertyGridProperty *pGroup = new CMFCPropertyGridProperty(str);
	Part_property_item_group *first_group=NULL;

	for(i=0,m=1;i<(p->component_number);i=j,m++){
		for(j=i;j<(p->component_number);j++)
			if((p->component_pointer[i]->part_id)!=(p->component_pointer[j]->part_id))
				break;
		str.Format(_T("%4d:%6d件  "),m,j-i);
		Part_with_shader *ps=p->ps->part_array[p->component_pointer[i]->part_id];
		if(ps!=NULL)
			str=str+(ps->user_name);
		
		Part_property_item_group *pGroup1=new Part_property_item_group(p->ps,str);
		discr=_T("");
		if(CFile::GetStatus((theApp.ws->plat_info->directory_name)+(ps->description_file_name),CFileStatus())){
			char buffer[MAX_BUFFER_LENGTH];
			CFile f((theApp.ws->plat_info->directory_name)+(ps->description_file_name),CFile::modeRead);
			k=(int)((f.GetLength()>=MAX_BUFFER_LENGTH)?MAX_BUFFER_LENGTH-1:f.GetLength());
			f.Read(buffer,k);
			buffer[k]='\0';
			f.Close();
			discr=CString(buffer);
		}
		for(k=i;k<j;k++){
			str.Format(_T("第%6d件  "),k-i+1,j-i);
			int movement_id=p->component_pointer[k]->movement_id;

			if((movement_id<0)||(movement_id>=(theApp.mounter->move_number)))
				str=str+_T("非拼装杆件");
			else{
				CString step_str; 
				step_str.Format(_T("拼装步骤%d/%d  "),theApp.mounter->move[movement_id]->main_step,theApp.mounter->move[movement_id]->sub_step);
				if(movement_id<theApp.mounter->current_step)
					str=str+_T("已拼装 ")+step_str;
				else
					str=str+_T("未拼装 ")+step_str;
			}
			{
				CMFCPropertyGridProperty *prop1=new Part_property_item(p->component_pointer[k],ps->user_name,str,discr);
				pGroup1->AddSubItem(prop1);
				pGroup1->collector.collect(p->component_pointer[k]);
			}
		}
		pGroup->AddSubItem(pGroup1);
		pGroup1->Expand(FALSE);
		pGroup1->SetDescription(discr);

		if(first_group==NULL)
			first_group=pGroup1;
	}
	list.AddProperty(pGroup);
	pGroup->Expand(TRUE);
	if((first_group!=NULL)&&(type_number==1))
		list.SetCurSel(first_group);
	return ;
}

void Part_property::OnVirtualMount()
{
	CMainFrame*  pFrame=(CMainFrame*)(theApp.m_pMainWnd);
	if(pFrame==NULL)
		return;

	theApp.mounter->load_step_part(pFrame);
	theApp.redraw_flag=true;

	pFrame->m_movement_list.virtual_mount_flag=true;

	CString str;
	if(theApp.mounter->move_number<=0)
		str=_T("没有运动信息");
	else{
		int current_step=theApp.mounter->current_step;
		if(current_step<0)
			current_step=0;
		if(current_step>=(theApp.mounter->move_number))
			str=_T("拼装完毕");
		else
			str=theApp.mounter->compare(current_step,get_stack_top());
	}
	if(str!=_T(""))
		AfxMessageBox(str);
	else{
		HTREEITEM match_list_id;
		theApp.mounter->start_test(theApp.time_length,match_list_id);
		if(match_list_id!=NULL)
				pFrame->m_movement_list.list.SelectItem(match_list_id);
	}
}

void Part_property:: display_mount_message(Part_collector &collector,int main_step,int sub_step,CString msg,CString sound_file)
{	
	class movement_sound_item:public CMFCPropertyGridProperty
	{
		CString sound_file;
	public:
		movement_sound_item(CString my_sound_file):CMFCPropertyGridProperty(_T("配音文件"), my_sound_file, my_sound_file)
		{
			sound_file=my_sound_file;
		}
		virtual void OnClickName(CPoint)
		{
			Sound_player::turn_sound_on_off();
			Sound_player::turn_sound_on_off();
			Sound_player::play(theApp.ws->plat_info->directory_name+sound_file);
		}
	};
	
	push_part_collector(collector);

	clear_list();
		
	CString str;
	str.Format(_T("%d"),main_step);
	CMFCPropertyGridProperty *sel_pointer=new CMFCPropertyGridProperty(_T("主拼装步骤"), str, msg);
	list.AddProperty(sel_pointer);
	str.Format(_T("%d"),sub_step);
	list.AddProperty(new CMFCPropertyGridProperty(_T("次拼装步骤"), str, msg));
	list.AddProperty(new movement_sound_item(sound_file));

	display_part_collector_process();
	list.SetCurSel(sel_pointer);
}

