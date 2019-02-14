
// precisionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "precision.h"
#include "precisionDlg.h"

#include <fstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CprecisionDlg 对话框




CprecisionDlg::CprecisionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CprecisionDlg::IDD, pParent)
	, chord(0)
	, length(0)
{
	ifstream f(_T("C:\\WINDOWS\\system32\\precision.lfy"));
	f>>slider;
	if(f.fail())
		slider=0;
	if(slider<0)
		slider=0;
	if(slider>100)
		slider=100;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CprecisionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_SLIDER, slider);
	DDV_MinMaxInt(pDX, slider, 0, 100);
	DDX_Text(pDX, IDC_CHORD_EDIT, chord);
	DDX_Text(pDX, IDC_LENGTH_EDIT, length);
}

BEGIN_MESSAGE_MAP(CprecisionDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER, &CprecisionDlg::OnNMCustomdrawSlider)
	ON_EN_CHANGE(IDC_CHORD_EDIT, &CprecisionDlg::OnEnChangeChordEdit)
	ON_EN_CHANGE(IDC_LENGTH_EDIT, &CprecisionDlg::OnEnChangeLengthEdit)
END_MESSAGE_MAP()


// CprecisionDlg 消息处理程序

BOOL CprecisionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	SetWindowPos(&wndTopMost ,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE); 

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CprecisionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CprecisionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CprecisionDlg::OnNMCustomdrawSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	display();
}

void CprecisionDlg::OnEnChangeChordEdit()
{
	display();
}

void CprecisionDlg::OnEnChangeLengthEdit()
{
	display();
}

void CprecisionDlg::display(void)
{
	UpdateData(TRUE);
	ofstream f(_T("C:\\WINDOWS\\system32\\precision.lfy"));
	f<<slider<<"\n";
	f<<chord<<"\n";
	f<<length<<"\n";

	CString str;

	str.Format(_T("精度:%d,    弦精度:%lf,    最小弦长:%lf"),slider,chord,length);
	SetWindowText(str);
}
