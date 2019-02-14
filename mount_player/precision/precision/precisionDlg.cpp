
// precisionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "precision.h"
#include "precisionDlg.h"

#include <fstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CprecisionDlg �Ի���




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


// CprecisionDlg ��Ϣ�������

BOOL CprecisionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	SetWindowPos(&wndTopMost ,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE); 

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CprecisionDlg::OnPaint()
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

	str.Format(_T("����:%d,    �Ҿ���:%lf,    ��С�ҳ�:%lf"),slider,chord,length);
	SetWindowText(str);
}
