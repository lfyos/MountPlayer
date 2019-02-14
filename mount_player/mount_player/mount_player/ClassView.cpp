
#include "stdafx.h"
#include "MainFrm.h"
#include "ClassView.h"
#include "Resource.h"
#include "mount_player.h"
#include "mount_playerDoc.h"
#include "mount_playerView.h"
#include "mount_bridge.h"
#include "part_tree.h"
#include "system_parameter.h"
#include "mount_processor.h"


class BridgetreeViewMenuButton : public CMFCToolBarMenuButton
{
	friend class BridgetreeView;

	DECLARE_SERIAL(BridgetreeViewMenuButton)

public:
	BridgetreeViewMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(BridgetreeViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BridgetreeView::BridgetreeView()
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;
}

BridgetreeView::~BridgetreeView()
{

}

BEGIN_MESSAGE_MAP(BridgetreeView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
	ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, OnClassAddMemberVariable)
	ON_COMMAND(ID_CLASS_DEFINITION, OnClassDefinition)
	ON_COMMAND(ID_CLASS_PROPERTIES, OnClassProperties)
	ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)

	ON_NOTIFY(TVN_SELCHANGED, 2, OnClickPartTree)

//	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView message handlers

int BridgetreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create views:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndClassView.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("Failed to create Class View\n");
		return -1;      // fail to create
	}

	// Load images:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

/*
	CMenu menuSort;
	menuSort.LoadMenu(IDR_POPUP_SORT);

	m_wndToolBar.ReplaceButton(ID_SORT_MENU, BridgetreeViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	BridgetreeViewMenuButton* pButton =  DYNAMIC_DOWNCAST(BridgetreeViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
		pButton->SetMessageWnd(this);
	}
*/
	// Fill in some static tree view data (dummy code, nothing magic here)

	FillClassView();

	return 0;
}

void BridgetreeView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void BridgetreeView::FillClassView()
{
	fill_tree_item(bridge_view->bridge->tree);
	return;


	HTREEITEM hRoot = m_wndClassView.InsertItem(_T("FakeApp classes"), 0, 0);
	m_wndClassView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM hClass = m_wndClassView.InsertItem(_T("CFakeAboutDlg"), 1, 1, hRoot);
	m_wndClassView.InsertItem(_T("CFakeAboutDlg()"), 3, 3, hClass);

	m_wndClassView.Expand(hRoot, TVE_EXPAND);

	hClass = m_wndClassView.InsertItem(_T("CFakeApp"), 1, 1, hRoot);
	m_wndClassView.InsertItem(_T("CFakeApp()"), 3, 3, hClass);
	m_wndClassView.InsertItem(_T("InitInstance()"), 3, 3, hClass);
	m_wndClassView.InsertItem(_T("OnAppAbout()"), 3, 3, hClass);

	hClass = m_wndClassView.InsertItem(_T("CFakeAppDoc"), 1, 1, hRoot);
	m_wndClassView.InsertItem(_T("CFakeAppDoc()"), 4, 4, hClass);
	m_wndClassView.InsertItem(_T("~CFakeAppDoc()"), 3, 3, hClass);
	m_wndClassView.InsertItem(_T("OnNewDocument()"), 3, 3, hClass);

	hClass = m_wndClassView.InsertItem(_T("CFakeAppView"), 1, 1, hRoot);
	m_wndClassView.InsertItem(_T("CFakeAppView()"), 4, 4, hClass);
	m_wndClassView.InsertItem(_T("~CFakeAppView()"), 3, 3, hClass);
	m_wndClassView.InsertItem(_T("GetDocument()"), 3, 3, hClass);
	m_wndClassView.Expand(hClass, TVE_EXPAND);

	hClass = m_wndClassView.InsertItem(_T("CFakeAppFrame"), 1, 1, hRoot);
	m_wndClassView.InsertItem(_T("CFakeAppFrame()"), 3, 3, hClass);
	m_wndClassView.InsertItem(_T("~CFakeAppFrame()"), 3, 3, hClass);
	m_wndClassView.InsertItem(_T("m_wndMenuBar"), 6, 6, hClass);
	m_wndClassView.InsertItem(_T("m_wndToolBar"), 6, 6, hClass);
	m_wndClassView.InsertItem(_T("m_wndStatusBar"), 6, 6, hClass);

	hClass = m_wndClassView.InsertItem(_T("Globals"), 2, 2, hRoot);
	m_wndClassView.InsertItem(_T("theFakeApp"), 5, 5, hClass);
	m_wndClassView.Expand(hClass, TVE_EXPAND);

	return;
}

void BridgetreeView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndClassView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Select clicked item:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_SORT);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}
}

void BridgetreeView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndClassView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void BridgetreeView::OnSort(UINT id)
{
	if (m_nCurrSort == id)
	{
		return;
	}

	m_nCurrSort = id;
/*
	BridgetreeViewMenuButton* pButton =  DYNAMIC_DOWNCAST(BridgetreeViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
		m_wndToolBar.Invalidate();
		m_wndToolBar.UpdateWindow();
	}
*/
}

void BridgetreeView::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}

void BridgetreeView::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("Add member function..."));
}

void BridgetreeView::OnClassAddMemberVariable()
{
	// TODO: Add your command handler code here
}

void BridgetreeView::OnClassDefinition()
{
	// TODO: Add your command handler code here
}

void BridgetreeView::OnClassProperties()
{
	// TODO: Add your command handler code here
}



void BridgetreeView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndClassView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void BridgetreeView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndClassView.SetFocus();
}

void BridgetreeView::OnChangeVisualStyle()
{
	m_ClassViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_ClassViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_ClassViewImages.Add(&bmp, RGB(255, 0, 0));

	m_wndClassView.SetImageList(&m_ClassViewImages, TVSIL_NORMAL);

	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* Locked */);
}

static BOOL update_flag=TRUE;

void BridgetreeView::fill_tree_item(part_tree * t)
{

	int i,bmp_id;
	BOOL visible_flag;
	CString str,str1;
	part_tree *p;

	update_flag=TRUE;

	visible_flag=t->is_visible();
	str=t->create_name(bridge_view->bridge->part,bridge_view->bridge->part_number,",",FALSE);
	if(t->parent==NULL){
		m_wndClassView.DeleteAllItems();
		t->structure_tree_id=m_wndClassView.InsertItem(str,2,2);
		m_wndClassView.SetItemState(t->structure_tree_id, TVIS_BOLD, TVIS_BOLD);
	}else if(t->type!=0){
		bmp_id=visible_flag?1:1;
		t->structure_tree_id=m_wndClassView.InsertItem(str,bmp_id,bmp_id,t->parent->structure_tree_id);
	}else{
		str1.Format("£º%d/%d",t->tree_node_union.part.part_component_id+1,t->tree_node_union.part.part_component_number);
		bmp_id=visible_flag?5:3;
		t->structure_tree_id=m_wndClassView.InsertItem(str+str1,bmp_id,bmp_id,t->parent->structure_tree_id);
	}

	if(t->type!=0){
		for(i=0,p=t->tree_node_union.assembly.first_child;i<(t->tree_node_union.assembly.child_number);i++,p=p->back_brother)
			fill_tree_item(p);
		if(t->parent==NULL)
			m_wndClassView.Expand(t->structure_tree_id,TVE_EXPAND);
	}
	return;
}
void BridgetreeView::OnClickPartTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	part_tree *t;

	if(update_flag){
		update_flag=FALSE;
		return;
	}
	CMainFrame*  pFrame=(CMainFrame*)(theApp.m_pMainWnd);
	if(pFrame==NULL)
		return;
	Cmount_playerView* pView=(Cmount_playerView*)(pFrame->GetActiveView());
	if(pView==NULL)
		return;

	t=bridge_view->bridge->tree->get_selected_node_by_part_tree(m_wndClassView.GetSelectedItem());
	if(t==NULL)
		return;
	
	pView->InvalidateRect(NULL,FALSE);

	bridge_view->bridge->tree->clear_mark_flag();

	t->set_mark_flag();
	bridge_mounter->set_box(t->box);

	pFrame->statusbar_display(t->create_name(bridge_view->bridge->part,bridge_view->bridge->part_number,_T(","),TRUE));

	pFrame->update_display(t,NULL,-1);

	if(t->type==0)
		pFrame->display_part_information(t);
	else{
		pFrame->display_tree_information(t);
		if(par->change_view_flag)
			pView->put_view_on_box(t->box,par->scaleup_view_flag);	
	}
	return;
}
void BridgetreeView::OnNewFolder()
{
	fill_tree_item(bridge_view->bridge->tree);
}
void BridgetreeView::update_display(part_tree *t)
{
	if(t==NULL)
		return;
	if(m_wndClassView.GetSelectedItem()==(t->structure_tree_id))
		return;

	update_flag=TRUE;

	m_wndClassView.SetFocus();

	m_wndClassView.SelectItem(NULL);
	m_wndClassView.SetItemState(NULL,TVIS_SELECTED,TVIS_SELECTED);

	m_wndClassView.SelectItem(t->structure_tree_id);
	m_wndClassView.SetItemState(t->structure_tree_id,TVIS_SELECTED,TVIS_SELECTED);

	return;
}

BOOL BridgetreeView::PreTranslateMessage(MSG* pMsg)
{
	CMainFrame*  pFrame=(CMainFrame*)(theApp.m_pMainWnd);
	if(pFrame!=NULL){	
		Cmount_playerView* pView=(Cmount_playerView*)(pFrame->GetActiveView());
		if(pView!=NULL)		
			if(::TranslateAccelerator(pView->m_hWnd,pView->m_hAccel,pMsg))  
				return   TRUE;
	}
	return CDockablePane::PreTranslateMessage(pMsg);
}