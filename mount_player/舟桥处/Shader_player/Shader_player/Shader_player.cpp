
// Shader_player.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "Shader_player.h"
#include "MainFrm.h"

#include "Shader_playerDoc.h"
#include "Shader_playerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CShader_playerApp

BEGIN_MESSAGE_MAP(CShader_playerApp, CWinAppEx)
//	ON_COMMAND(ID_APP_ABOUT, &CShader_playerApp::OnAppAbout)
	// Standard file based document commands
//	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
//	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_EXIT_PROGRAM, &CShader_playerApp::OnAppExit)
END_MESSAGE_MAP()


// CShader_playerApp construction

CShader_playerApp::CShader_playerApp()
{

	m_bHiColorIcons = TRUE;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CShader_playerApp object

CShader_playerApp theApp;


// CShader_playerApp initialization


BOOL CShader_playerApp::InitInstance()
{
	CString str=m_lpCmdLine,sub_directory_name,merge_file_name,part_file_name,configure_file_name,caption_file_name;

	sub_directory_name=File_data::get_head_string(str);		
	part_file_name=File_data::get_head_string(str);
	merge_file_name=File_data::get_head_string(str);
	configure_file_name=File_data::get_head_string(str);
	caption_file_name=File_data::get_head_string(str);
	
	mouse_move_id=0;
	mouse_wheel_id=0;
	mouse_left_key_down_flag=false;
	redraw_flag=true;
	time_length=1000;
	touch_time=0;

	ws=NULL;
	mounter=NULL;


	str=Platform_information::get_directory_name();
	str=str+sub_directory_name+_T("\\");

	while((File_data::get_file_length(str+part_file_name)<=0)||(File_data::get_file_length(str+configure_file_name)<=0)){
		if(AfxMessageBox(_T("相关模块还没有安装,选择数据文件进行安装（YES/NO）?"),MB_YESNO)!=IDYES)
			return FALSE;
		Platform_information::restoredata();
	}

	ws=new Whole_system(sub_directory_name,part_file_name,merge_file_name,configure_file_name,caption_file_name);

	mounter=new Movement_processor_with_virtual_mount(ws,ws->current_assemble_type->movement_name);
	mounter->load();


	//改变工具栏标题字体	
	

//	set_font_size(&(afxGlobalData.fontBold),					15,		_T("宋体"));
//	set_font_size(&(afxGlobalData.fontDefaultGUIBold),			15,		_T("宋体"));
//	set_font_size(&(afxGlobalData.fontDefaultGUIUnderline),		15,		_T("宋体"));
//	set_font_size(&(afxGlobalData.fontMarlett),					15,		_T("宋体"));
	set_font_size(&(afxGlobalData.fontRegular),					15,		_T("宋体"));
//	set_font_size(&(afxGlobalData.fontSmall),					15,		_T("宋体"));
//	set_font_size(&(afxGlobalData.fontTooltip),					15,		_T("宋体"));
//	set_font_size(&(afxGlobalData.fontUnderline),				15,		_T("宋体"));
//	set_font_size(&(afxGlobalData.fontVert),					15,		_T("宋体"));
//	set_font_size(&(afxGlobalData.fontVertCaption),				15,		_T("宋体"));


	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CShader_playerDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CShader_playerView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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

// App command to run the dialog
void CShader_playerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CShader_playerApp customization load/save methods

void CShader_playerApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);

	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);

	GetContextMenuManager()->AddMenu(_T("测试"), IDR_POPUP_MOVEMENT);
}

void CShader_playerApp::LoadCustomState()
{
}

void CShader_playerApp::SaveCustomState()
{
}

// CShader_playerApp message handlers

int CShader_playerApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	if(mounter!=NULL)
		delete mounter;
	if(ws!=NULL)
		delete ws;
	ws=NULL;
	mounter=NULL;
	
	return CWinAppEx::ExitInstance();
}
void CShader_playerApp::set_font_size(CFont *my_font,int my_size,LPCTSTR font_name)
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
        font_name);
}

BOOL CShader_playerApp::OnIdle(LONG lCount)
{
	CMainFrame*  pFrame=(CMainFrame*)m_pMainWnd;
	if(pFrame!=NULL){
		pFrame->m_component_list.component_list_modify_flag=true;
		pFrame->m_part_list.part_list_modify_flag=true;
		pFrame->m_movement_list.movement_list_modify_flag=true;
	}
	if(ws->exit_flag)
		OnAppExit();
	return CWinAppEx::OnIdle(lCount);
}
