// subtitle_input_dialog.cpp : implementation file
//

#include "stdafx.h"
#include "mount_player.h"
#include "subtitle_input_dialog.h"


// subtitle_input_dialog dialog

IMPLEMENT_DYNAMIC(subtitle_input_dialog, CDialog)

subtitle_input_dialog::subtitle_input_dialog(CWnd* pParent /*=NULL*/)
	: CDialog(subtitle_input_dialog::IDD, pParent)
{

}

subtitle_input_dialog::~subtitle_input_dialog()
{
}

void subtitle_input_dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SUBTITLE_EDIT, subtitle);
}


BEGIN_MESSAGE_MAP(subtitle_input_dialog, CDialog)

END_MESSAGE_MAP()


// subtitle_input_dialog message handlers

void subtitle_input_dialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);
	
	int i,n=subtitle.GetLength();
	CString str=subtitle;

	for(i=0,subtitle="";i<n;i++){
		switch(str.GetAt(i)){
		default:
			subtitle+=str.GetAt(i);
		case ' ':
		case '	':
		case '\n':
		case '\r':
			break;
		}
	}

	UpdateData(FALSE);

	if(!(subtitle.IsEmpty()))
		CDialog::OnOK();
	return;
}