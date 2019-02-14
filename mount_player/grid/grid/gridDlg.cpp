
// gridDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "grid.h"
#include "gridDlg.h"


#define _USE_MATH_DEFINES

#include <math.h>
#include <fstream>
using namespace std;



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CgridDlg 对话框




CgridDlg::CgridDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CgridDlg::IDD, pParent)
	, length(64)
	, height(32)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CgridDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LENGTH_EDIT, length);
	DDX_Text(pDX, IDC_WIDTH_EDIT, height);
}

BEGIN_MESSAGE_MAP(CgridDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CgridDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CgridDlg 消息处理程序

BOOL CgridDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CgridDlg::OnPaint()
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
HCURSOR CgridDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CgridDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();

	if(length<1)
		length=1;
	if(height<1)
		height=1;
	CString str;
	str.Format(_T("grid_%d_%d.mesh"),length,height);

	wchar_t buf[2048];	
	::GetModuleFileName(NULL,buf,sizeof(buf));
	CString file_name=buf;
	for(int i=file_name.GetLength()-1;i>=0;i--)
		if((file_name.GetAt(i)=='\\')||(file_name.GetAt(i)=='/')){
			file_name=file_name.Left(i+1);
			break;
		}
	file_name=file_name+str;
	
	ofstream f(file_name);



	f<<"形体数          1\n";
    f<<"	第  0  个形体名称    网格    表面数    1\n\n";
	f<<"		第  0  个表面名称    网格面    纹理坐标数    0\n";


	
	int i,j,k;
	f<<"\n			表面三角片顶点数    "<<((1+length)*(1+height))<<"\n";

	for(i=0,k=0;i<=height;i++)
		for(j=0;j<=length;j++,k++){
			f<<"				第  "<<k<<"  个三角片顶点    ";
			f<<((double)j/(double)length)<<"	"<<((double)i/(double)height)<<"		0.0\n";
		}

	f<<"\n			表面法线顶点数    1\n";
	f<<"				第  0  个法线顶点		1	1	1\n";
	

	f<<"\n			表面三角片数    "<<(2*length*height)<<"\n";
	for(i=0,k=0;i<height;i++){
		for(j=0;j<length;j++){
			f<<"				第  "<<k<<"  个三角片材质编号    "<<k<<"\n";
			f<<"				第  "<<k<<"  个三角片边线标志    "<<k<<"\n";
			f<<"				第  "<<k<<"  个三角片顶点坐标索引    ";
			f<<(0+j)+(0+i)*(1+length)<<"	";
			f<<(1+j)+(0+i)*(1+length)<<"	";
			f<<(1+j)+(1+i)*(1+length)<<"	\n";
			f<<"				第  "<<k<<"  个三角片法线坐标索引    0	0	0\n\n";
			k++;


			f<<"				第  "<<k<<"  个三角片材质编号    "<<k<<"\n";
			f<<"				第  "<<k<<"  个三角片边线标志    "<<k<<"\n";
			f<<"				第  "<<k<<"  个三角片顶点坐标索引    ";
			f<<(0+j)+(0+i)*(1+length)<<"	";
			f<<(1+j)+(1+i)*(1+length)<<"	";
			f<<(0+j)+(1+i)*(1+length)<<"	\n";
			f<<"				第  "<<k<<"  个三角片法线坐标索引    0	0	0\n\n";
			k++;

		}
	}
	f<<"\n\n			表面类型    未知    参数个数    0    参数";   
	f<<"\n\n			表面环路数    0\n\n\n\n";
}
