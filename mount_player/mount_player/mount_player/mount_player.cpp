
// mount_player.cpp : 定义应用程序的类行为。
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
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(MENU_MANUAL_VIEW, &Cmount_playerApp::OnManualView)
END_MESSAGE_MAP()


// Cmount_playerApp 构造

Cmount_playerApp::Cmount_playerApp()
{

	m_bHiColorIcons = TRUE;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 Cmount_playerApp 对象

Cmount_playerApp theApp;


// Cmount_playerApp 初始化


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
	//改变工具栏标题字体	

//	set_font_size(&(afxGlobalData.fontBold),					12,		"宋体");
//	set_font_size(&(afxGlobalData.fontDefaultGUIBold),			12,		"宋体");
//	set_font_size(&(afxGlobalData.fontDefaultGUIUnderline),		12,		"宋体");
//	set_font_size(&(afxGlobalData.fontMarlett),					12,		"宋体");
	set_font_size(&(afxGlobalData.fontRegular),					15,		"宋体");
//	set_font_size(&(afxGlobalData.fontSmall),					12,		"宋体");
//	set_font_size(&(afxGlobalData.fontTooltip),					12,		"宋体");
//	set_font_size(&(afxGlobalData.fontUnderline),				12,		"宋体");
//	set_font_size(&(afxGlobalData.fontVert),					12,		"宋体");
//	set_font_size(&(afxGlobalData.fontVertCaption),				12,		"宋体");


	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(0);  // 加载标准 INI 文件选项(包括 MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(Cmount_playerDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(Cmount_playerView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 启用“DDE 执行”
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	// 启用拖/放
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// 用于运行对话框的应用程序命令
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

// Cmount_playerApp 消息处理程序

