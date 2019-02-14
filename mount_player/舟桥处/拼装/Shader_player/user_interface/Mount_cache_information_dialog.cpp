// user_interface\Mount_cache_information_dialog.cpp : implementation file
//

#include "stdafx.h"
#include "..\Shader_player.h"
#include "Mount_cache_information_dialog.h"


// Mount_cache_information_dialog dialog

IMPLEMENT_DYNAMIC(Mount_cache_information_dialog, CDialog)

Mount_cache_information_dialog::Mount_cache_information_dialog(CWnd* pParent /*=NULL*/)
	: CDialog(Mount_cache_information_dialog::IDD, pParent)
	, title(_T(""))
	, start_main_step(1)
	, start_sub_step(1)
	, end_main_step(1)
	, end_sub_step(1)
{

}

Mount_cache_information_dialog::~Mount_cache_information_dialog()
{
}

void Mount_cache_information_dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TITLE_EDIT, title);
	DDX_Text(pDX, IDC_START_MAIN_EDIT, start_main_step);
	DDX_Text(pDX, IDC_START_SUB_EDIT, start_sub_step);
	DDX_Text(pDX, IDC_END_MAIN_EDIT, end_main_step);
	DDX_Text(pDX, IDC_END_SUB_EDIT, end_sub_step);
}


BEGIN_MESSAGE_MAP(Mount_cache_information_dialog, CDialog)

END_MESSAGE_MAP()


void Mount_cache_information_dialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);
	for(int i=0;i<title.GetLength();i++)
		if((title.GetAt(i)==CString(_T(" ")))||(title.IsEmpty())){
			CDialog::OnCancel();
			AfxMessageBox(_T("标题中含有空格"));
			return;
		}
	CDialog::OnOK();
}
