#include "StdAfx.h"
#include "Assemble_type_list.h"
#include "..\MainFrm.h"

BEGIN_MESSAGE_MAP(Assemble_type_list,List_base)
	ON_COMMAND(ID_PROPERTIES, file_list)
	ON_NOTIFY(TVN_SELCHANGED, 4, OnModify)
END_MESSAGE_MAP()


void Assemble_type_list::file_list()
{
	list.DeleteAllItems();
	file_list_item(theApp.ws->assemble_type);
	list.Expand(theApp.ws->assemble_type->list_id,TVE_EXPAND);
}

void Assemble_type_list::file_list_item(Assemble_type *assemble_type)
{
	HTREEITEM parent_list_id;
	Assemble_type *p;
	CString caption_name;

	if(assemble_type==NULL)
		return;

	if(assemble_type->parent==NULL){
		parent_list_id=NULL;
		caption_name=assemble_type->caption_name;
	}else{
		parent_list_id=assemble_type->parent->list_id;
		int length=(assemble_type->parent->caption_name+_T("/")).GetLength();
		length=assemble_type->caption_name.GetLength()-length;
		caption_name=assemble_type->caption_name.Right(length);
	}

	if((p=assemble_type->child)==NULL)
		assemble_type->list_id=list.InsertItem(caption_name,5,5,parent_list_id);
	else{
		assemble_type->list_id=list.InsertItem(caption_name,1,1,parent_list_id);
		
		do{
			file_list_item(p);
		}while((p=p->left_brother)!=(assemble_type->child));
	}
}

void Assemble_type_list::OnModify(NMHDR* pNMHDR, LRESULT* pResult)
{
	CMainFrame*  pFrame=(CMainFrame*)(theApp.m_pMainWnd);
	if(pFrame==NULL)
		return;

	Assemble_type *assemble_type;
	
	assemble_type=theApp.ws->assemble_type->get_assemble_type_by_list_id(list.GetSelectedItem());
	if(assemble_type==NULL)
		return;
	if(assemble_type->test()){
		CString str;
		str =_T("杆件信息文件或装配体信息文件错误");
		str+=_T("\n配装类型标题:  ")+assemble_type->caption_name;
		str+=_T("\n杆件列表文件:  ")+assemble_type->part_name;
		str+=_T("\n装配体文件:    ")+assemble_type->assemble_name;
		str+=_T("\n文件目录:      ")+assemble_type->directory_name;
		AfxMessageBox(str);
		return;
	}
	
	if(assemble_type->child!=NULL)
		return;
	
	delete (theApp.mounter);
	theApp.mounter=NULL;

	pFrame->m_property_list.OnClearDisplay();

	theApp.ws->current_assemble_type=assemble_type;
	theApp.ws->reload_component(assemble_type->caption_name,assemble_type->part_name,assemble_type->assemble_name);
	theApp.mounter=new Movement_processor_with_virtual_mount(theApp.ws,theApp.ws->current_assemble_type->movement_name);
	theApp.mounter->load();

	theApp.redraw_flag=true;

	pFrame->m_component_list.file_list();
	pFrame->m_part_list.file_list();
	pFrame->m_movement_list.file_list();

	pFrame->m_property_list.m_wndObjectCombo.ResetContent();
	pFrame->m_property_list.m_wndObjectCombo.SetCurSel(pFrame->m_property_list.m_wndObjectCombo.AddString(theApp.ws->caption_name));

	CString file_name=(theApp.ws->plat_info->directory_name)+(theApp.ws->caption_file_name);
	ofstream f(file_name);
	File_data::output_string(f,assemble_type->caption_name+_T("\n\n\n\n\n"));
	f.close();
}