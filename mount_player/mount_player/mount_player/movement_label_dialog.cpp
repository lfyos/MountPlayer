// movement_label_dialog.cpp : implementation file
//

#include "stdafx.h"
#include "mount_player.h"
#include "movement_label_dialog.h"


// movement_label_dialog dialog

IMPLEMENT_DYNAMIC(movement_label_dialog, CDialog)

movement_label_dialog::movement_label_dialog(CWnd* pParent /*=NULL*/)
	: CDialog(movement_label_dialog::IDD, pParent)
	, label_name(_T(""))
{

}

movement_label_dialog::~movement_label_dialog()
{
}

void movement_label_dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MOVEMENT_LABEL_EDIT, label_name);
}


BEGIN_MESSAGE_MAP(movement_label_dialog, CDialog)
END_MESSAGE_MAP()


// movement_label_dialog message handlers

void movement_label_dialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	int i,n;
	CString str;

	UpdateData(TRUE);
	n=label_name.GetLength();
	str=label_name;
	label_name="";
	for(i=0;i<n;i++)
		switch(str.GetAt(i)){
		case ' ':
		case '\n':
		case '\r':
		case '\t':
			break;
		default:
			label_name+=str.GetAt(i);
			break;
		}
	if(label_name.IsEmpty())
		return;
	UpdateData(FALSE);
	CDialog::OnOK();
}
