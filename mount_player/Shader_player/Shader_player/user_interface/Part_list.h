#pragma once

#include "List_base.h"
#include "../Shader_player.h"

class Part_list :public List_base
{
	void file_part_list(Component *p,Part_collector &collector);
	void create_select_collector(Component *p,Part_collector &collector,Part_collector &selected_collector,HTREEITEM id,bool flag=false);
public:	

	bool part_list_modify_flag;

	void select_component(Component *p)
	{
		Component *q;
		if((q=p->child)!=NULL)
			do{
				select_component(q);
			}while((q=q->left_brother)!=(p->child));
		else{
			part_list_modify_flag=false;
			list.SelectItem(p->component_list_id[2]);
		}
	}
	Part_list()
		:List_base(IDR_POPUP_EXPLORER)
	{
		part_list_modify_flag=false;
	}	
	virtual void file_list()
	{
		OnRedrawTreeList();
	}
	afx_msg void OnRedrawTreeList()
	{
		Part_collector collector(theApp.ws->root_component,theApp.ws->parts);
		file_part_list(theApp.ws->part_list_root_component,collector);
	}
	afx_msg void OnModify(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};
