
// gridDlg.cpp : ʵ���ļ�
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


// CgridDlg �Ի���




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


// CgridDlg ��Ϣ�������

BOOL CgridDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CgridDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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



	f<<"������          1\n";
    f<<"	��  0  ����������    ����    ������    1\n\n";
	f<<"		��  0  ����������    ������    ����������    0\n";


	
	int i,j,k;
	f<<"\n			��������Ƭ������    "<<((1+length)*(1+height))<<"\n";

	for(i=0,k=0;i<=height;i++)
		for(j=0;j<=length;j++,k++){
			f<<"				��  "<<k<<"  ������Ƭ����    ";
			f<<((double)j/(double)length)<<"	"<<((double)i/(double)height)<<"		0.0\n";
		}

	f<<"\n			���淨�߶�����    1\n";
	f<<"				��  0  �����߶���		1	1	1\n";
	

	f<<"\n			��������Ƭ��    "<<(2*length*height)<<"\n";
	for(i=0,k=0;i<height;i++){
		for(j=0;j<length;j++){
			f<<"				��  "<<k<<"  ������Ƭ���ʱ��    "<<k<<"\n";
			f<<"				��  "<<k<<"  ������Ƭ���߱�־    "<<k<<"\n";
			f<<"				��  "<<k<<"  ������Ƭ������������    ";
			f<<(0+j)+(0+i)*(1+length)<<"	";
			f<<(1+j)+(0+i)*(1+length)<<"	";
			f<<(1+j)+(1+i)*(1+length)<<"	\n";
			f<<"				��  "<<k<<"  ������Ƭ������������    0	0	0\n\n";
			k++;


			f<<"				��  "<<k<<"  ������Ƭ���ʱ��    "<<k<<"\n";
			f<<"				��  "<<k<<"  ������Ƭ���߱�־    "<<k<<"\n";
			f<<"				��  "<<k<<"  ������Ƭ������������    ";
			f<<(0+j)+(0+i)*(1+length)<<"	";
			f<<(1+j)+(1+i)*(1+length)<<"	";
			f<<(0+j)+(1+i)*(1+length)<<"	\n";
			f<<"				��  "<<k<<"  ������Ƭ������������    0	0	0\n\n";
			k++;

		}
	}
	f<<"\n\n			��������    δ֪    ��������    0    ����";   
	f<<"\n\n			���滷·��    0\n\n\n\n";
}
