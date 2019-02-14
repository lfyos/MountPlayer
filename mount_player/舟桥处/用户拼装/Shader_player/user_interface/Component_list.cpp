
#include "stdafx.h"
#include "../mainfrm.h"
#include "Component_list.h"
#include "../Resource.h"
#include "../Shader_player.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView


BEGIN_MESSAGE_MAP(Component_list,List_base)
	ON_COMMAND(ID_PROPERTIES, OnRedrawTreeList)
	ON_NOTIFY(TVN_SELCHANGED, 4, OnModify)
END_MESSAGE_MAP()

void Component_list::fill_component_view(Component *comp,bool visible_flag)
{
	Component *p;
	CString mount_str=_T("");

	if((p=comp->child)==NULL)
		if((comp->movement_id>=0)&&(comp->movement_id<theApp.mounter->move_number)){
			mount_str.Format(_T(",拼装步骤:%d/%d"),theApp.mounter->move[comp->movement_id]->main_step,theApp.mounter->move[comp->movement_id]->sub_step);
			if(comp->movement_id<theApp.mounter->current_step)
				mount_str=_T(":  已拼装")+mount_str;
			else
				mount_str=_T(":  未拼装")+mount_str;
		}
	
	if((comp->display_flag&1)==0)
		visible_flag=false;

	if(comp->parent==NULL)
		comp->component_list_id[0]=list.InsertItem(comp->name+mount_str,1,1);
	else{
		CString str=comp->name,str_parent=comp->parent->name+_T("/");
		if(str.Left(str_parent.GetLength())==str_parent)
			str=str.Right(str.GetLength()-str_parent.GetLength());
		comp->component_list_id[0]=list.InsertItem(str+mount_str,(comp->child!=NULL)?1:visible_flag?5:3,(comp->child!=NULL)?1:visible_flag?5:3,comp->parent->component_list_id[0]);
	}
	if((p=comp->child)!=NULL){
		do{
			fill_component_view(p,visible_flag);
		}while((p=p->left_brother)!=(comp->child));	
	}
}
void Component_list::OnModify(NMHDR* pNMHDR, LRESULT* pResult)
{
	CMainFrame*  pFrame=(CMainFrame*)(theApp.m_pMainWnd);
	if(component_list_modify_flag&&(pFrame!=NULL)){
		Component *p=theApp.ws->root_component->get_node_by_list_id(list.GetSelectedItem(),0);
		if(p!=NULL){
			theApp.redraw_flag=true;
			if(p->component_box_flag)
				theApp.ws->mount_on_box(p->component_box,1.5,theApp.time_length);
			pFrame->m_property_list.display_part_collector(Part_collector(p,theApp.ws->parts));
			p->set_component_selected();
		}
	}
}
