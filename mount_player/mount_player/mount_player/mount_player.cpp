
// mount_player.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "mount_player.h"
#include "MainFrm.h"

#include "mount_playerDoc.h"
#include "mount_playerView.h"
#include "system_parameter.h"

#include <fstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cmount_playerApp

BEGIN_MESSAGE_MAP(Cmount_playerApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &Cmount_playerApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(MENU_MANUAL_VIEW, &Cmount_playerApp::OnManualView)
END_MESSAGE_MAP()


// Cmount_playerApp ����

Cmount_playerApp::Cmount_playerApp()
{

	m_bHiColorIcons = TRUE;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� Cmount_playerApp ����

Cmount_playerApp theApp;


// Cmount_playerApp ��ʼ��


void Cmount_playerApp::set_font_size(CFont *my_font,int my_size,char *font_name)
{
	my_font->DeleteObject();
	my_font->CreateFont(
        my_size,                   // nHeight
        0,                         // nWidth
        0,                         // nEscapement
        0,                         // nOrientation
        FW_NORMAL,                 // nWeight
        FALSE,                     // bItalic
        FALSE,                     // bUnderline
        0,                         // cStrikeOut
        ANSI_CHARSET,              // nCharSet
        OUT_DEFAULT_PRECIS,        // nOutPrecision
        CLIP_DEFAULT_PRECIS,       // nClipPrecision
        DEFAULT_QUALITY,           // nQuality
        DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
        _T(font_name));
}
BOOL Cmount_playerApp::InitInstance()
{
	//�ı乤������������	

//	set_font_size(&(afxGlobalData.fontBold),					12,		"����");
//	set_font_size(&(afxGlobalData.fontDefaultGUIBold),			12,		"����");
//	set_font_size(&(afxGlobalData.fontDefaultGUIUnderline),		12,		"����");
//	set_font_size(&(afxGlobalData.fontMarlett),					12,		"����");
	set_font_size(&(afxGlobalData.fontRegular),					15,		"����");
//	set_font_size(&(afxGlobalData.fontSmall),					12,		"����");
//	set_font_size(&(afxGlobalData.fontTooltip),					12,		"����");
//	set_font_size(&(afxGlobalData.fontUnderline),				12,		"����");
//	set_font_size(&(afxGlobalData.fontVert),					12,		"����");
//	set_font_size(&(afxGlobalData.fontVertCaption),				12,		"����");


	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(0);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(Cmount_playerDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(Cmount_playerView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ���á�DDE ִ�С�
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����
	// ������/��
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void Cmount_playerApp::OnAppAbout()
{
//	CAboutDlg aboutDlg;
//	aboutDlg.DoModal();

	ifstream chm(_T(par->directory_name+".\\document.chm"));
	CString file_name;
	
	if(chm.fail()){
		ifstream html(_T(par->directory_name+".\\document.html"));
		if(html.fail()){
			ifstream htm(_T(par->directory_name+".\\document.html"));
			if(htm.fail()){
				file_name=".\\document.doc";
			}else{
				htm.close();
				file_name=".\\document.htm";
			}
		}else{
			html.close();
			file_name=".\\document.html";
		}
	}else{
		chm.close();
		file_name=".\\document.chm";
	}
		
	::ShellExecute(NULL,_T("open"),par->directory_name+file_name,NULL,NULL,SW_SHOWNORMAL);
	return;
}
void Cmount_playerApp::OnManualView()
{
	// TODO: Add your command handler code here

	ifstream f(_T(".\\manual.chm"));
	if(f.fail()){
		f.close();
		::ShellExecute(NULL,_T("open"),_T(".\\manual.doc"),NULL,NULL,SW_SHOWNORMAL);
	}else{
		f.close();
		::ShellExecute(NULL,_T("open"),_T(".\\manual.chm"),NULL,NULL,SW_SHOWNORMAL);
	}
}

// Cmount_playerApp customization load/save methods

void Cmount_playerApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void Cmount_playerApp::LoadCustomState()
{
}

void Cmount_playerApp::SaveCustomState()
{
}

// Cmount_playerApp ��Ϣ�������

