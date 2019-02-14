
#pragma once

#include "List_base.h"

class Movement_list:public List_base
{
	void set_component_list_id(Component *comp,HTREEITEM my_list_id)
	{
		Component *p;
		if((p=comp->child)==NULL){
			comp->component_list_id[1]=my_list_id;
			return;
		}		
		comp->component_list_id[1]=NULL;
		do{
			set_component_list_id(p,my_list_id);
		}while((p=p->left_brother)!=(comp->child));
	}
public:
	void do_file_list(bool do_set_item_flag=true);

	void display_step();
	void add_match();


	bool virtual_mount_flag;
	bool movement_list_modify_flag;
	HTREEITEM root;
	void select_component(Component *p)
	{
		Component *q=p->child;
		if(q!=NULL)
			do{
				select_component(q);
			}while((q=q->left_brother)!=(p->child));
		else{
			movement_list_modify_flag=false;
			list.SelectItem(NULL);
			list.SelectItem(p->component_list_id[1]);
		}
	}
	Movement_list()
		:List_base(IDR_POPUP_MOVEMENT)
	{
		movement_list_modify_flag=false;
		virtual_mount_flag=false;
	}	
	virtual void file_list()
	{
		do_file_list();
	};
	afx_msg void OnRedrawTreeList()
	{
		movement_list_modify_flag=false;
		do_file_list();
	}

	
	afx_msg void OnEnableDisable(CCmdUI *pCmdUI);

	afx_msg void OnModify(NMHDR* pNMHDR, LRESULT* pResult);


	afx_msg void OnMountStepDivision();
	afx_msg void OnMountStepMerge();
	afx_msg void OnMountTogether();

	afx_msg void OnMountMainStepSort();
	afx_msg void OnMountSubStepSort();

	afx_msg void OnMountStepMove();
	afx_msg void OnMountStepExchange();
	afx_msg void OnMountTitleAndSound();
	afx_msg void OnMountTitleFile();
	afx_msg void OnMountStepTag();
	afx_msg void OnMountSaveToCache();
	
	afx_msg void OnMountStepDelete();
	afx_msg void OnMountRestoreDesign();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnMountSaveDesign();
	afx_msg void OnUpdateMountSaveDesign(CCmdUI *pCmdUI);
	afx_msg void OnMountAllStepMerge();
	afx_msg void OnMountStepForce();
	afx_msg void OnMountStepForceManual();
};

