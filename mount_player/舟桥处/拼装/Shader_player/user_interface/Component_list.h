
#pragma once

#include "List_base.h"
#include "../Shader_player.h"

class Component_list:public List_base
{
	void fill_component_view(Component *comp,bool visible_flag);
public:
	bool component_list_modify_flag;
	Component_list()
		:List_base(IDR_POPUP_EXPLORER)
	{
		component_list_modify_flag=false;
	}	
	void select_component(Component *p)
	{
		component_list_modify_flag=false;
		list.SelectItem(NULL);
		list.SelectItem(p->component_list_id[0]);
	}
	virtual void file_list()
	{
		OnRedrawTreeList();
	}
	afx_msg void OnRedrawTreeList()
	{
		list.DeleteAllItems();
		fill_component_view(theApp.ws->root_component,true);
		list.SetItemState(theApp.ws->root_component->component_list_id[0], TVIS_BOLD, TVIS_BOLD);
		list.Expand(theApp.ws->root_component->component_list_id[0], TVE_EXPAND);
	}
	afx_msg void OnModify(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};

