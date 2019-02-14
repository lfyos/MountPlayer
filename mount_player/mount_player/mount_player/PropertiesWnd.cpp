
#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "mount_player.h"

#include "mount_processor.h"
#include "gl/gl.h"
#include "system_parameter.h"
#include "movement.h"
#include "mount_bridge.h"
#include "part_tree.h"
#include "part_info.h"
#include "mount_playerDoc.h"
#include "mount_playerView.h"
#include "part_image.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
{
	gs=NULL;
	display_image_flag=TRUE;
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()	
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()

	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)

	ON_CBN_SELCHANGE(1, &CPropertiesWnd::OnSystemChange)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);

	m_wndObjectCombo.GetWindowRect(&rectCombo);

	int cyCmb = rectCombo.Size().cy;
	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), 5000, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	if(display_image_flag)
		m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb)-rectClient.Width(), SWP_NOACTIVATE | SWP_NOZORDER);
	else
		m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create combo:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Failed to create Properties Combo \n");
		return -1;      // fail to create
	}
//	m_wndObjectCombo.InsertString(0,par->system_name);
//	m_wndObjectCombo.InsertString(1,_T("Application"));
//	m_wndObjectCombo.InsertString(2,_T("Properties Window"));
	{
		int index_id=insert_system_name_in_combox();
		m_wndObjectCombo.SetFont(CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT)));
		m_wndObjectCombo.SetCurSel(index_id);
	}
	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

	InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
//	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* Is locked */);
	m_wndToolBar.LoadToolBar(IDR_PROPERTY_TOOLBAR, 0, 0, TRUE /* Is locked */);
	
	m_wndToolBar.CleanUpLockedImages();
//	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* Locked */);
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDR_PROPER_TOOLBAR : IDR_PROPER_TOOLBAR, 0, 0, TRUE /* Locked */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();

	SetTimer(101,20,NULL);

	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* pCmdUI)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
	// TODO: Add your command handler code here
	display_image_flag=display_image_flag?FALSE:TRUE;
	AdjustLayout();
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: Add your command handler code here
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void CPropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	return;

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Appearance"));

	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("3D Look"), (_variant_t) false, _T("Specifies the window's font will be non-bold and controls will have a 3D border")));

	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("Border"), _T("Dialog Frame"), _T("One of: None, Thin, Resizable, or Dialog Frame"));
	pProp->AddOption(_T("None"));
	pProp->AddOption(_T("Thin"));
	pProp->AddOption(_T("Resizable"));
	pProp->AddOption(_T("Dialog Frame"));
	pProp->AllowEdit(FALSE);

	pGroup1->AddSubItem(pProp);
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Caption"), (_variant_t) _T("About"), _T("Specifies the text that will be displayed in the window's title bar")));

	m_wndPropList.AddProperty(pGroup1);

	CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("Window Size"), 0, TRUE);

	pProp = new CMFCPropertyGridProperty(_T("Height"), (_variant_t) 250l, _T("Specifies the window's height"));
	pProp->EnableSpinControl(TRUE, 0, 1000);
	pSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty( _T("Width"), (_variant_t) 150l, _T("Specifies the window's width"));
	pProp->EnableSpinControl();
	pSize->AddSubItem(pProp);

	m_wndPropList.AddProperty(pSize);

	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("Font"));

	LOGFONT lf;
	CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	font->GetLogFont(&lf);

	lstrcpy(lf.lfFaceName, _T("Arial"));

	pGroup2->AddSubItem(new CMFCPropertyGridFontProperty(_T("Font"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("Specifies the default font for the window")));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("Use System Font"), (_variant_t) true, _T("Specifies that the window uses MS Shell Dlg font")));

	m_wndPropList.AddProperty(pGroup2);

	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("Misc"));
	pProp = new CMFCPropertyGridProperty(_T("(Name)"), _T("Application"));
	pProp->Enable(FALSE);
	pGroup3->AddSubItem(pProp);

	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("Window Color"), RGB(210, 192, 254), NULL, _T("Specifies the default window color"));
	pColorProp->EnableOtherButton(_T("Other..."));
	pColorProp->EnableAutomaticButton(_T("Default"), ::GetSysColor(COLOR_3DFACE));
	pGroup3->AddSubItem(pColorProp);

	static TCHAR BASED_CODE szFilter[] = _T("Icon Files(*.ico)|*.ico|All Files(*.*)|*.*||");
	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Icon"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("Specifies the window icon")));

	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Folder"), _T("c:\\")));

	m_wndPropList.AddProperty(pGroup3);

	CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("Hierarchy"));

	CMFCPropertyGridProperty* pGroup41 = new CMFCPropertyGridProperty(_T("First sub-level"));
	pGroup4->AddSubItem(pGroup41);

	CMFCPropertyGridProperty* pGroup411 = new CMFCPropertyGridProperty(_T("Second sub-level"));
	pGroup41->AddSubItem(pGroup411);

	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Item 1"), (_variant_t) _T("Value 1"), _T("This is a description")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Item 2"), (_variant_t) _T("Value 2"), _T("This is a description")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Item 3"), (_variant_t) _T("Value 3"), _T("This is a description")));

	pGroup4->Expand(FALSE);
	m_wndPropList.AddProperty(pGroup4);
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
}

void CPropertiesWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CDockablePane::OnTimer(nIDEvent);

	if(display_image_flag){
		CPaintDC dc(this);	
		if(gs!=NULL)
			gs->begin_display(&dc);
		else{
			gs=new opengl_graphics_system(&dc);
			gs->begin_display(&dc);
			bridge_view->bridge->create_call_list(1,2000,gs);
		}

		CRect rectClient;
		GetClientRect(rectClient);

		glViewport(0,0,rectClient.Width(),rectClient.Width());

		bridge_view->do_mark_display(1,gs,bridge_mounter->move_parts_box,par->manual_mount_flag?par->manual_mount_proportion:-1);

		for(int i=0;i<bridge_view->bridge->image_number;i++)
			bridge_view->bridge->images[i]->process_picture(1,gs);

		gs->end_display(&dc,TRUE);
	}
	return;
}

void CPropertiesWnd::OnDestroy()
{
	KillTimer(101);

	CDockablePane::OnDestroy();

	if(gs!=NULL)
		delete gs;

	// TODO: Add your message handler code here
}
CString CPropertiesWnd::display_mount_information(int visible_number[],int hide_number[],int step_id)
{
	int i,j,part_number;
	CMFCPropertyGridProperty *p;
	part_info **part;
	CString str,statusbar_str;
	
	part=bridge_view->bridge->part;
	part_number=bridge_view->bridge->part_number;

	m_wndPropList.RemoveAll();

	statusbar_str.Format("%d,%d:",bridge_mounter->move[step_id]->main_step,bridge_mounter->move[step_id]->sub_step);

	str.Format("%d",bridge_mounter->move[step_id]->main_step);	
	m_wndPropList.AddProperty(
		p=new CMFCPropertyGridProperty(_T("主拼装步骤"), str, bridge_mounter->move[step_id]->message));

	str.Format("%d",bridge_mounter->move[step_id]->sub_step);	
	m_wndPropList.AddProperty(
		new CMFCPropertyGridProperty(_T("次拼装步骤"), str, bridge_mounter->move[step_id]->message));

	m_wndPropList.AddProperty(
		new CMFCPropertyGridProperty(_T("配音文件"), bridge_mounter->move[step_id]->sound_file, bridge_mounter->move[step_id]->message));

	int total_visible_number=0,total_hide_number=0;

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("拼装杆件"));
	for(i=0;i<part_number;i++){
		j=bridge_view->bridge->part_index[i];
		total_visible_number+=visible_number[j];
		total_hide_number   +=hide_number[j];
		if((visible_number[j]+hide_number[j])>0){
			str.Format(_T("%d件"),visible_number[j]+hide_number[j]);

			statusbar_str+=str+part[j]->information.name;

			pGroup1->AddSubItem(new CMFCPropertyGridProperty(part[j]->information.name, (_variant_t) str,part[j]->get_description("\n")));
		}
	}
	str.Format("%d件",total_visible_number+total_hide_number);
	pGroup1->AddSubItem(new CMFCPropertyGridProperty("杆件总计", (_variant_t) str,"杆件总计"+str));

	m_wndPropList.AddProperty(pGroup1);

	m_wndPropList.SetCurSel(p);

	return statusbar_str+_T(":")+(bridge_mounter->move[step_id]->message);

}
CString CPropertiesWnd::display_mount_information(int start_step_id,int end_step_id,BOOL mark_flag)
{
	int i;
	CMFCPropertyGridProperty *p;
	CString str,statusbar_str;

	if(start_step_id>end_step_id){
		i=end_step_id;
		end_step_id=start_step_id;
		start_step_id=i;
	}
	if(end_step_id<0)
		end_step_id=bridge_mounter->current_step;
	if(end_step_id<0)
		end_step_id=0;
	if(end_step_id>=bridge_mounter->move_number)
		end_step_id=bridge_mounter->move_number-1;

	statusbar_str.Format("%d,%d:",bridge_mounter->move[end_step_id]->main_step,bridge_mounter->move[end_step_id]->sub_step);

	m_wndPropList.ShowWindow(SW_HIDE);
	m_wndPropList.RemoveAll();

	str.Format("%d",bridge_mounter->move[end_step_id]->main_step);	
	m_wndPropList.AddProperty(
		p=new CMFCPropertyGridProperty(_T("主拼装步骤"), str, bridge_mounter->move[end_step_id]->message));

	str.Format("%d",bridge_mounter->move[end_step_id]->sub_step);	
	m_wndPropList.AddProperty(
		new CMFCPropertyGridProperty(_T("次拼装步骤"), str, bridge_mounter->move[end_step_id]->message));

	m_wndPropList.AddProperty(
		new CMFCPropertyGridProperty(_T("配音文件"), bridge_mounter->move[end_step_id]->sound_file, bridge_mounter->move[end_step_id]->message));

	statusbar_str+=display_mount_part_information(start_step_id,end_step_id,mark_flag);

	m_wndPropList.SetCurSel(p);
	m_wndPropList.ShowWindow(SW_SHOW);

	return statusbar_str+_T(":")+(bridge_mounter->move[end_step_id]->message);
}
CString CPropertiesWnd::display_mount_part_information(int  start_step_id, int  end_step_id,BOOL mark_flag)
{
	int i,j,part_number,*visible_part_number,*hide_part_number;
	part_info **part;
	part_tree  *t;
	CString str,statusbar_str;
	CMFCPropertyGridProperty *p=NULL;
	
	part=bridge_view->bridge->part;
	part_number=bridge_view->bridge->part_number;

	visible_part_number	=new int[part_number];
	hide_part_number	=new int[part_number];

	for(i=0;i<part_number;i++){
		visible_part_number[i]=0;
		hide_part_number[i]=0;
	}
	bridge_view->bridge->tree->clear_mark_flag();

	for(i=start_step_id;i<=end_step_id;i++){
		t=bridge_mounter->move[i]->tree_node;
		if(t==NULL)
			t=bridge_view->bridge->tree->get_node_by_name(bridge_mounter->move[i]->name);
		if(t!=NULL){
			t->set_mark_flag(mark_flag);
			t->get_part_reference_number(visible_part_number,hide_part_number,TRUE);
		}
	}
	int total_visible_number=0,total_hide_number=0;

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("拼装杆件"));

	for(i=0;i<part_number;i++){
		j=bridge_view->bridge->part_index[i];
		total_visible_number+=visible_part_number[j];
		total_hide_number   +=hide_part_number[j];
		if((visible_part_number[j]+hide_part_number[j])>0){
			str.Format(_T("%d件"),visible_part_number[j]+hide_part_number[j]);
			statusbar_str+=str+(part[j]->information.name);
			pGroup1->AddSubItem(p=new CMFCPropertyGridProperty(part[j]->information.name, (_variant_t) str,part[j]->get_description("\n")));
		}
	}
	str.Format("%d件",total_visible_number+total_hide_number);
	pGroup1->AddSubItem(p=new CMFCPropertyGridProperty("杆件总计", (_variant_t) str,"杆件总计"+str));

	m_wndPropList.AddProperty(pGroup1);

	delete []hide_part_number;
	delete []visible_part_number;

	if(p!=NULL)
		m_wndPropList.SetCurSel(p);
	return statusbar_str;
}

void CPropertiesWnd:: display_part_information(part_info *p)
{
	int i,j;
	CString str;
	part_tree *t;
	CMFCPropertyGridProperty *pp;

	m_wndPropList.RemoveAll();

	m_wndPropList.AddProperty(
		pp=new CMFCPropertyGridProperty(_T("杆件名称"), p->information.name, p->get_description("\n")));
	
	for(i=0;i<bridge_view->bridge->part_number;i++){
		j=bridge_view->bridge->part_index[i];
		if(bridge_view->bridge->part[j]==p){
			t=bridge_view->bridge->first_part_tree[j];
			str.Format("%d",t->tree_node_union.part.part_component_number);
			m_wndPropList.AddProperty(
				new CMFCPropertyGridProperty(_T("杆件件数"), str, p->get_description("\n")));
		}
	}
	if(pp!=NULL)
		m_wndPropList.SetCurSel(pp);
	return;
}
void CPropertiesWnd::display_part_information(part_tree *t)
{
	CString str;
	part_info *p;
	CMFCPropertyGridProperty *pp;

	m_wndPropList.RemoveAll();

	if(t->type!=0)
		return;
	p=bridge_view->bridge->part[t->tree_node_union.part.part_id];
	m_wndPropList.AddProperty(
		pp=new CMFCPropertyGridProperty(_T("杆件名称"), p->information.name, p->get_description("\n")));
	
	str.Format("%d",t->tree_node_union.part.part_component_number);
	m_wndPropList.AddProperty(
		new CMFCPropertyGridProperty(_T("杆件件数"), str, p->get_description("\n")));

	str.Format("%d",t->tree_node_union.part.part_component_id+1);
	m_wndPropList.AddProperty(
		new CMFCPropertyGridProperty(_T("杆件序号"), str, p->get_description("\n")));	

	if(pp!=NULL)
		m_wndPropList.SetCurSel(pp);
	
	return;

}

void CPropertiesWnd::display_tree_information(part_tree *t)
{
	int i,j,s,*visible_number,*hide_number,part_number,total_visible_number=0,total_hide_number=0;
	part_info *p;
	CString str;

	m_wndPropList.RemoveAll();

	part_number=bridge_view->bridge->part_number;

	visible_number=new int[part_number];
	hide_number=new int[part_number];

	for(i=0;i<part_number;i++){
		visible_number[i]=0;
		hide_number[i]=0;
	}

	t->get_part_reference_number(visible_number,hide_number,TRUE);

	for(i=0;i<part_number;i++){
		j=bridge_view->bridge->part_index[i];

		total_visible_number+=visible_number[j];
		total_hide_number   +=hide_number[j];
		s=visible_number[j]+hide_number[j];

		if(s>0){
			p=bridge_view->bridge->part[j];
			str.Format("%d件，其中拼装%d件，未拼装%d件",s,visible_number[j],hide_number[j]);
			m_wndPropList.AddProperty(
				new CMFCPropertyGridProperty(p->information.name, str, p->get_description("\n")));
		}
	}
	str.Format("%d件，其中拼装%d件，未拼装%d件",
		total_visible_number+total_hide_number,total_visible_number,total_hide_number);

	m_wndPropList.AddProperty(
		new CMFCPropertyGridProperty("杆件总计", str,"杆件总计"+str));

	delete []visible_number;
	delete []hide_number;
}
int CPropertiesWnd::insert_system_name_in_combox(void)
{
	int id;
	BOOL flag;
	CFileFind finder;
	char buf[MAX_MESSAGE_LENGTH];
	
	finder.FindFile(".\\*");
	for(id=0,flag=TRUE;flag;){
		flag=finder.FindNextFile();
		if((finder.IsDirectory())&&(!(finder.IsDots()))){
			CString file_name;
			file_name=".\\"+finder.GetFileName()+"\\configure.lfy";
			ifstream f(file_name);
			f>>buf;
			if(!(f.fail())){
				m_wndObjectCombo.InsertString(id++,buf);
			}
			f.close();
		}
	}

	ifstream f_id(".\\current.lfy");
	f_id>>id;

	if((id<0)||(f_id.fail()))
		id=0;

	return id;
}
void CPropertiesWnd::OnSystemChange()
{
	char file_name[MAX_MESSAGE_LENGTH];

	if((m_wndObjectCombo.GetCurSel())>=0){
		ofstream f;
		f.open(".\\current.lfy");
		f<<m_wndObjectCombo.GetCurSel()<<"\n";
		f.close();

		::GetModuleFileName(NULL,file_name,sizeof(file_name)/sizeof(file_name[0]));
		::ShellExecute(NULL,_T("open"),file_name,NULL,NULL,SW_SHOWNORMAL);
		
		::Sleep(10000);
		::PostQuitMessage(0);
	}
}

BOOL CPropertiesWnd::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	CMainFrame*  pFrame=(CMainFrame*)(theApp.m_pMainWnd);
	if(pFrame!=NULL){	
		Cmount_playerView* pView=(Cmount_playerView*)(pFrame->GetActiveView());
		if(pView!=NULL)		
			if(::TranslateAccelerator(pView->m_hWnd,pView->m_hAccel,pMsg))  
				return   TRUE;
	}

	return CDockablePane::PreTranslateMessage(pMsg);
}
