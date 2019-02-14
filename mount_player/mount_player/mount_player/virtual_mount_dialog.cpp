// virtual_mount_dialog.cpp : implementation file
//

#include "stdafx.h"
#include "mount_player.h"
#include "virtual_mount_dialog.h"
#include "MainFrm.h"
#include "mount_playerDoc.h"
#include "mount_playerView.h"
#include "system_parameter.h"
#include "movement.h"
#include "mount_bridge.h"
#include "part_tree.h"
#include "part_info.h"

// virtual_mount_dialog dialog

IMPLEMENT_DYNAMIC(virtual_mount_dialog, CDialog)

virtual_mount_dialog::virtual_mount_dialog(CWnd* pParent /*=NULL*/)
	: CDialog(virtual_mount_dialog::IDD, pParent)
	, manual_mount_state_flag(0)
{

}

virtual_mount_dialog::~virtual_mount_dialog()
{
}

void virtual_mount_dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PART_LIST, m_list);
}


BEGIN_MESSAGE_MAP(virtual_mount_dialog, CDialog)
	ON_LBN_SELCHANGE(IDC_PART_LIST, &virtual_mount_dialog::OnLbnSelchangePartList)
	ON_LBN_DBLCLK(IDC_PART_LIST, &virtual_mount_dialog::OnLbnDblclkPartList)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// virtual_mount_dialog message handlers

BOOL virtual_mount_dialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	int i,j,k,kk,*visible_number,*hide_number,part_number;
	part_tree *t;
	CString str,tmp;

	m_list.ResetContent();

	part_number=bridge_view->bridge->part_number;
	visible_number=new int[part_number];
	hide_number=new int[part_number];


	for(i=0;i<sizeof(select_id)/sizeof(select_id[0]);i++)
		select_id[i]=LB_ERR ;

	for(i=0;i<bridge_mounter->move_number;){
		for(j=0;j<part_number;j++){
			visible_number[j]=0;
			hide_number[j]=0;
		}
		for(j=i;j<bridge_mounter->move_number;j++){
			if((bridge_mounter->move[j]->main_step)!=(bridge_mounter->move[i]->main_step))
				break;
			if((bridge_mounter->move[j]->sub_step)!=(bridge_mounter->move[i]->sub_step))
				break;
			if((t=bridge_view->bridge->tree->get_node_by_name(bridge_mounter->move[j]->name))!=NULL)
				t->get_part_reference_number(visible_number,hide_number,TRUE);
		}
		for(k=0,str="";k<part_number;k++){
			kk=bridge_view->bridge->part_index[k];
			if((visible_number[kk]+hide_number[kk])>0){
				tmp.Format("%d¼þ",visible_number[kk]+hide_number[kk]);
				str+=tmp+bridge_view->bridge->part[kk]->information.name;
			}
		}
		tmp.Format("%3d,%3d : ",bridge_mounter->move[i]->main_step,bridge_mounter->move[i]->sub_step);
		kk=m_list.AddString(tmp+str);
		for(;i<j;i++)
			select_id[i]=kk;
	}
	delete []visible_number;
	delete []hide_number;


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void virtual_mount_dialog::OnLbnSelchangePartList()
{
	// TODO: Add your control notification handler code here
	int i,j;
	part_tree *t;
	double box[6];

	bridge_view->bridge->tree->clear_mark_flag();
	for(i=0,j=0;i<bridge_mounter->move_number;i++){
		if(select_id[i]==m_list.GetCurSel())
			if((t=bridge_view->bridge->tree->get_node_by_name(bridge_mounter->move[i]->name))!=NULL){
				t->set_mark_flag();
				j++;
				if(j==1){
					box[0]=t->box[0];
					box[1]=t->box[1];
					box[2]=t->box[2];
					box[3]=t->box[3];
					box[4]=t->box[4];
					box[5]=t->box[5];
				}else{
					box[0]=(t->box[0]<box[0])?(t->box[0]):(box[0]);
					box[1]=(t->box[1]<box[1])?(t->box[1]):(box[1]);
					box[2]=(t->box[2]<box[2])?(t->box[2]):(box[2]);

					box[3]=(t->box[3]>box[3])?(t->box[3]):(box[3]);
					box[4]=(t->box[4]>box[4])?(t->box[4]):(box[4]);
					box[5]=(t->box[5]>box[5])?(t->box[5]):(box[5]);
				}
			}
	}

	if(j>=1)
		bridge_mounter->set_box(box);

	CMainFrame*  pFrame;
	if((pFrame=(CMainFrame*)(theApp.m_pMainWnd))==NULL)
		return;
	Cmount_playerView* pView;
	if((pView=(Cmount_playerView*)(pFrame->GetActiveView()))==NULL)
		return;

	pView->InvalidateRect(NULL,FALSE);

	return;
}

void virtual_mount_dialog::OnLbnDblclkPartList()
{
	// TODO: Add your control notification handler code here
	if(manual_mount_state_flag!=1)
		return;
	if(select_id[bridge_mounter->current_step]==m_list.GetCurSel()){
		par->manual_mount_flag=TRUE;
		par->manual_mount_proportion=0;
		bridge_mounter->advance_one_step();

		manual_mount_state_flag=2;
		ShowWindow(SW_HIDE);
		SetTimer(102,250,NULL);
	}
	return;
}

void virtual_mount_dialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	OnLbnDblclkPartList();

//	CDialog::OnOK();
}

void virtual_mount_dialog::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

//	CDialog::OnCancel();

	manual_mount_state_flag=0;
	par->manual_mount_flag=FALSE;

	ShowWindow(SW_HIDE);	
}

void virtual_mount_dialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnTimer(nIDEvent);
	if(manual_mount_state_flag==2){
		int i;
		for(i=0;i<bridge_mounter->move_number;i++)
			if(bridge_mounter->move[i]->tree_node!=NULL)
				return;		
		manual_mount_state_flag=1;			
		par->manual_mount_flag=TRUE;
		par->manual_mount_proportion=0;
		ShowWindow(SW_SHOW);
		KillTimer(102);
	}
}