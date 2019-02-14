// User_interface\Mount_time_length_dialog.cpp : implementation file
//

#include "stdafx.h"
#include "../Shader_player.h"
#include "Mount_time_length_dialog.h"
#include "Location_dialog.h"

#include "../MainFrm.h"


// Mount_time_length_dialog dialog

IMPLEMENT_DYNAMIC(Mount_time_length_dialog, CDialog)

Mount_time_length_dialog::Mount_time_length_dialog()
: CDialog(Mount_time_length_dialog::IDD)
,time_length_edit(1000)
{
}

Mount_time_length_dialog::~Mount_time_length_dialog()
{

}

void Mount_time_length_dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TIME_LENGTH_EDIT, time_length_edit);
	DDX_Slider(pDX, IDC_TIME_LENGTH_SLIDER, time_length_slider);
	DDX_Control(pDX, IDC_TIME_LENGTH_SLIDER, slider_controller);
}

BEGIN_MESSAGE_MAP(Mount_time_length_dialog, CDialog)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TIME_LENGTH_SLIDER, &Mount_time_length_dialog::OnNMCustomdrawTimeLengthSlider)
	ON_BN_CLICKED(ID_MIDDLE_POINT, &Mount_time_length_dialog::OnBnClickedMiddlePoint)
	ON_BN_CLICKED(ID_TERMINATATION, &Mount_time_length_dialog::OnBnClickedTerminatation)
	ON_COMMAND(ID_COMPONENT_LOCATION, &Mount_time_length_dialog::OnComponentLocation)
	ON_BN_CLICKED(ID_COMPONENT_RESET_FOR_MOVEMENT, &Mount_time_length_dialog::OnBnClickedComponentResetForMovement)
	ON_BN_CLICKED(ID_MOVEMENT_RESET_FOR_MOVEMENT, &Mount_time_length_dialog::OnBnClickedMovementResetForMovement)
END_MESSAGE_MAP()


// Mount_time_length_dialog message handlers

void Mount_time_length_dialog::OnNMCustomdrawTimeLengthSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	UpdateData(TRUE);
	time_length_edit=time_length_slider;
	UpdateData(FALSE);
}

BOOL Mount_time_length_dialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	slider_controller.SetRange(100,10000);
	slider_controller.SetPos(1000);

	return TRUE;  
}

void Mount_time_length_dialog::set_title()
{
	CString str;

	Movement_processor *p=theApp.mounter;
	Movement *m;

	if(p->move_number>=(MAX_MOVEMENT_NUMBER-1))
		str=_T("运动信息过多，无法继续设计拼装");
	else if((m=p->move[p->move_number])==NULL)
		str=_T("还没有开始拼装设计");
	else
		str.Format(_T("拼装设计:%d/%d"),m->step_number+1,MAX_STEP_NUMBER);

	SetWindowText(str);
}

void Mount_time_length_dialog::OnBnClickedMiddlePoint()
{
	// TODO: Add your control notification handler code here

	CMainFrame*  pFrame=(CMainFrame*)(theApp.m_pMainWnd);
	if(pFrame==NULL)
		return;	

	UpdateData(TRUE);
	
	Component *comp;
	Movement_processor *p=theApp.mounter;
	Part_set *ps=theApp.ws->parts;

	Movement *m;

	if(p->move_number>=(MAX_MOVEMENT_NUMBER-1))
		return;
	if((m=p->move[p->move_number])==NULL){
		AfxMessageBox(_T("没有开始添加拼装步骤"));
		return;
	}
	if((comp=m->comp)==NULL){
		delete (p->move[p->move_number]);
		p->move[p->move_number]=NULL;
		AfxMessageBox(_T("没有相关杆件或者杆件组合"));
		return;
	}
	if(m->step_number<MAX_STEP_NUMBER){
		m->move[m->step_number]=comp->move_location;
		m->time_length[m->step_number]=time_length_edit;
		m->step_number++;
		m->caculate_total_time_length();
	}
	set_title();

	return;
}

void Mount_time_length_dialog::OnBnClickedTerminatation()
{
	// TODO: Add your control notification handler code here

	CMainFrame*  pFrame=(CMainFrame*)(theApp.m_pMainWnd);
	if(pFrame==NULL)
		return;
	
	UpdateData(TRUE);
	ShowWindow(SW_HIDE);

	Component *comp;
	Movement_processor *p=theApp.mounter;
	Part_set *ps=theApp.ws->parts;
	Movement *m;

	if(p->move_number>=(MAX_MOVEMENT_NUMBER-1))
		return;
	if((m=p->move[p->move_number])==NULL){
		AfxMessageBox(_T("没有开始添加拼装步骤"));
		return;
	}
	if((comp=m->comp)==NULL){
		delete (p->move[p->move_number]);
		p->move[p->move_number]=NULL;
		AfxMessageBox(_T("没有相关杆件或者杆件组合"));
		return;
	}
	if(m->step_number<1){
		ShowWindow(SW_SHOW);
		AfxMessageBox(_T("需要至少输入两个运动点"));
		return;
	}
	if(m->step_number<MAX_STEP_NUMBER){
		m->move[m->step_number]=comp->move_location;
		m->time_length[m->step_number]=time_length_edit;
		m->caculate_total_time_length();
	}
	theApp.mounter->push_movement();

	theApp.mounter->move_number++;
	theApp.mounter->sort(theApp.mounter->move_number-1);
	theApp.redraw_flag=true;

	pFrame->m_movement_list.movement_list_modify_flag=false;
	
	pFrame->m_movement_list.file_list();
	pFrame->m_movement_list.list.Expand(m->move_list_id[0],TVE_EXPAND);
	pFrame->m_movement_list.list.SelectItem(m->move_list_id[1]);

	comp->reset_component(*(theApp.ws->parts));
	return;
}

void Mount_time_length_dialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

//	CDialog::OnOK();
	ShowWindow(SW_HIDE);
}

void Mount_time_length_dialog::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

//	CDialog::OnCancel();

	ShowWindow(SW_HIDE);
}

void Mount_time_length_dialog::OnComponentLocation()
{
	Component *comp;
	Movement_processor *p=theApp.mounter;
	Part_set *ps=theApp.ws->parts;
	Movement *m;

	if(p->move_number>=(MAX_MOVEMENT_NUMBER-1))
		return;
	if((m=p->move[p->move_number])==NULL){
		AfxMessageBox(_T("没有开始添加拼装步骤"));
		return;
	}
	if((comp=m->comp)==NULL){
		delete (p->move[p->move_number]);
		p->move[p->move_number]=NULL;
		AfxMessageBox(_T("没有相关杆件或者杆件组合"));
		return;
	}
	theApp.redraw_flag=true;
	Location_dialog dlg(comp,theApp.ws->parts,this);
	dlg.DoModal();
}
void Mount_time_length_dialog::OnBnClickedComponentResetForMovement()
{
	Component *comp;
	Movement_processor *p=theApp.mounter;
	Movement *m;

	if(p->move_number>=(MAX_MOVEMENT_NUMBER-1))
		return;
	if((m=p->move[p->move_number])==NULL){
		AfxMessageBox(_T("没有开始添加拼装步骤"));
		return;
	}
	if((comp=m->comp)==NULL){
		delete (p->move[p->move_number]);
		p->move[p->move_number]=NULL;
		AfxMessageBox(_T("没有相关杆件或者杆件组合"));
		return;
	}
	comp->reset_component(*(theApp.ws->parts));
	set_title();
	theApp.redraw_flag=true;
}

void Mount_time_length_dialog::OnBnClickedMovementResetForMovement()
{
	Component *comp;
	Movement_processor *p=theApp.mounter;
	Movement *m;

	if(p->move_number>=(MAX_MOVEMENT_NUMBER-1))
		return;
	if((m=p->move[p->move_number])==NULL){
		AfxMessageBox(_T("没有开始添加拼装步骤"));
		return;
	}
	if((comp=m->comp)==NULL){
		delete (p->move[p->move_number]);
		p->move[p->move_number]=NULL;
		AfxMessageBox(_T("没有相关杆件或者杆件组合"));
		return;
	}
	m->step_number=0;
	set_title();
	theApp.redraw_flag=true;
}
