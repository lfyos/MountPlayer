
#include "stdafx.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"
#include "mount_player.h"
#include "mount_view.h"
#include "mount_playerDoc.h"
#include "mount_playerView.h"
#include "system_parameter.h"
#include "mount_bridge.h"
#include "part_tree.h"
#include "part_info.h"
#include "movement_index_of_part.h"
#include "movement.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView

BridgepartView::BridgepartView()
{
}

BridgepartView::~BridgepartView()
{
}

BEGIN_MESSAGE_MAP(BridgepartView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()

	ON_NOTIFY(TVN_SELCHANGED, 4, OnClickPartTree)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

int BridgepartView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create view:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}

	// Load view images:
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 255, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// Fill in some static tree view data (dummy code, nothing magic here)
	FillFileView();
	AdjustLayout();

	return 0;
}

void BridgepartView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void BridgepartView::FillFileView()
{

	fill_tree_item(bridge_view->bridge->tree);
	return;

	HTREEITEM hRoot = m_wndFileView.InsertItem(_T("FakeApp files"), 0, 0);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM hSrc = m_wndFileView.InsertItem(_T("FakeApp Source Files"), 0, 0, hRoot);

	m_wndFileView.InsertItem(_T("FakeApp.cpp"), 1, 1, hSrc);
	m_wndFileView.InsertItem(_T("FakeApp.rc"), 1, 1, hSrc);
	m_wndFileView.InsertItem(_T("FakeAppDoc.cpp"), 1, 1, hSrc);
	m_wndFileView.InsertItem(_T("FakeAppView.cpp"), 1, 1, hSrc);
	m_wndFileView.InsertItem(_T("MainFrm.cpp"), 1, 1, hSrc);
	m_wndFileView.InsertItem(_T("StdAfx.cpp"), 1, 1, hSrc);

	HTREEITEM hInc = m_wndFileView.InsertItem(_T("FakeApp Header Files"), 0, 0, hRoot);

	m_wndFileView.InsertItem(_T("FakeApp.h"), 2, 2, hInc);
	m_wndFileView.InsertItem(_T("FakeAppDoc.h"), 2, 2, hInc);
	m_wndFileView.InsertItem(_T("FakeAppView.h"), 2, 2, hInc);
	m_wndFileView.InsertItem(_T("Resource.h"), 2, 2, hInc);
	m_wndFileView.InsertItem(_T("MainFrm.h"), 2, 2, hInc);
	m_wndFileView.InsertItem(_T("StdAfx.h"), 2, 2, hInc);

	HTREEITEM hRes = m_wndFileView.InsertItem(_T("FakeApp Resource Files"), 0, 0, hRoot);

	m_wndFileView.InsertItem(_T("FakeApp.ico"), 2, 2, hRes);
	m_wndFileView.InsertItem(_T("FakeApp.rc2"), 2, 2, hRes);
	m_wndFileView.InsertItem(_T("FakeAppDoc.ico"), 2, 2, hRes);
	m_wndFileView.InsertItem(_T("FakeToolbar.bmp"), 2, 2, hRes);

	m_wndFileView.Expand(hRoot, TVE_EXPAND);
	m_wndFileView.Expand(hSrc, TVE_EXPAND);
	m_wndFileView.Expand(hInc, TVE_EXPAND);

}

void BridgepartView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
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
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void BridgepartView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void BridgepartView::OnFileOpen()
{
	// TODO: Add your command handler code here
}

void BridgepartView::OnFileOpenWith()
{
	// TODO: Add your command handler code here
}

void BridgepartView::OnDummyCompile()
{
	// TODO: Add your command handler code here
}

void BridgepartView::OnEditCut()
{
	// TODO: Add your command handler code here
}

void BridgepartView::OnEditCopy()
{
	// TODO: Add your command handler code here
}

void BridgepartView::OnEditClear()
{
	// TODO: Add your command handler code here
}

void BridgepartView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void BridgepartView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void BridgepartView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* Locked */);

	m_FileViewImages.DeleteImageList();

//	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;
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

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
//	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));
	m_FileViewImages.Add(&bmp, RGB(255, 0, 0));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}
static BOOL update_flag=TRUE;

void BridgepartView::fill_tree_item(part_tree * t)
{
	int i,j,id,n;
	CString str,str1;
	BOOL visible_flag;
	static HTREEITEM hSrc,hRoot;
	movement_index_of_part *i_t;

	update_flag=TRUE;

	m_wndFileView.DeleteAllItems();
	hRoot=m_wndFileView.InsertItem(_T("杆件列表"), 2, 2);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	for(i=0;i<(bridge_view->bridge->part_number);i++){
		id=bridge_view->bridge->part_index[i];
		t=bridge_view->bridge->first_part_tree[id];
		n=t->tree_node_union.part.part_component_number;

		str.Format("：共计%d件",n);
		str=bridge_view->bridge->part[id]->information.name+str;

		hSrc=m_wndFileView.InsertItem(str+str1, 1, 1, hRoot);
		bridge_view->bridge->part[id]->part_list_id=hSrc;

		for(j=0;j<n;j++,t=t->tree_node_union.part.back_part){
			str.Format("第%4d/%d件 ",j+1,n);
			str+=bridge_view->bridge->part[id]->information.name;
			visible_flag=t->is_visible();
			t->tree_node_union.part.part_list_id=m_wndFileView.InsertItem(str, visible_flag?5:3, visible_flag?5:3, hSrc);
			for(i_t=t->tree_node_union.part.move_index_of_part;i_t!=NULL;i_t=i_t->next_movement_index){
				str.Format("拼装步骤：%d，%d",
						bridge_mounter->move[i_t->movement_id]->main_step,
						bridge_mounter->move[i_t->movement_id]->sub_step);
				i_t->tree_item_id_of_movement_index=m_wndFileView.InsertItem(str,
					visible_flag?5:3, visible_flag?5:3,t->tree_node_union.part.part_list_id);
			}
		}
	}
	m_wndFileView.Expand(hRoot,TVE_EXPAND);
}

void BridgepartView::OnClickPartTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	int i,j,k,n;
	double box[6];
	part_tree *t,*p;
	CString str;
	HTREEITEM hSrc;
	BOOL which_is_equal_flag;
	movement_index_of_part *i_p;

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

	pView->InvalidateRect(NULL,FALSE);

	bridge_view->bridge->tree->clear_mark_flag();

	hSrc=m_wndFileView.GetSelectedItem();
	for(i=0,k=0;i<(bridge_view->bridge->part_number);i++){
		t=bridge_view->bridge->first_part_tree[i];
		n=t->tree_node_union.part.part_component_number;
		for(j=0;j<n;j++,t=t->tree_node_union.part.back_part)
			if((hSrc==(t->tree_node_union.part.part_list_id))||(bridge_view->bridge->part[i]->part_list_id==hSrc)){
				if(hSrc==(t->tree_node_union.part.part_list_id))
					which_is_equal_flag=TRUE;
				else
					which_is_equal_flag=FALSE;
				t->set_mark_flag();
				k++;
				if(k==1){
					p=t;
					box[0]=t->box[0];
					box[1]=t->box[1];
					box[2]=t->box[2];
					box[3]=t->box[3];
					box[4]=t->box[4];
					box[5]=t->box[5];
					pFrame->statusbar_display(bridge_view->bridge->part[i]->get_description(""));
				}else{
					box[0]=(box[0]<t->box[0])?box[0]:t->box[0];
					box[1]=(box[1]<t->box[1])?box[1]:t->box[1];
					box[2]=(box[2]<t->box[2])?box[2]:t->box[2];
					box[3]=(box[3]>t->box[3])?box[3]:t->box[3];
					box[4]=(box[4]>t->box[4])?box[4]:t->box[4];
					box[5]=(box[5]>t->box[5])?box[5]:t->box[5];
				}
			}
	}	
	if(k>=1){
		if(k==1)
			pFrame->update_display(NULL,p,-1);
		if(which_is_equal_flag)
			pFrame->display_part_information(p);
		else
			pFrame->display_part_information(bridge_view->bridge->part[p->tree_node_union.part.part_id]);
		if(par->change_view_flag)
				pView->put_view_on_box(box,par->scaleup_view_flag);	
		bridge_mounter->set_box(box);

		return;
	}
	hSrc=m_wndFileView.GetSelectedItem();
	for(i=0;i<(bridge_view->bridge->part_number);i++){
		t=bridge_view->bridge->first_part_tree[i];
		n=t->tree_node_union.part.part_component_number;
		for(j=0;j<n;j++,t=t->tree_node_union.part.back_part)
			for(i_p=t->tree_node_union.part.move_index_of_part;i_p!=NULL;i_p=i_p->next_movement_index)
				if(hSrc==(i_p->tree_item_id_of_movement_index)){
					int main_step,sub_step;
					if((i_p->movement_id<0)||(i_p->movement_id>=bridge_mounter->move_number))
						return;
					main_step=bridge_mounter->move[i_p->movement_id]->main_step;
					sub_step=bridge_mounter->move[i_p->movement_id]->sub_step;
					for(i=i_p->movement_id-1;i>=0;i--){
						if(main_step!=(bridge_mounter->move[i]->main_step))
							break;
						if(sub_step!=(bridge_mounter->move[i]->sub_step))
							break;
					}
					for(j=i_p->movement_id+1;j<(bridge_mounter->move_number);j++){
						if(main_step!=(bridge_mounter->move[j]->main_step))
							break;
						if(sub_step!=(bridge_mounter->move[j]->sub_step))
							break;
					}
					pFrame->update_display(NULL,NULL,i+1);
					pFrame->display_mount_information(i+1,j-1,TRUE);
					view_lock_at_step(main_step,sub_step,pView);

					return;
				}
	}
	return;
}
void BridgepartView::OnProperties()
{
	fill_tree_item(bridge_view->bridge->tree);
}
void BridgepartView::update_display(part_tree *t)
{
	if(t==NULL)
		return;
	if(t->type!=0)
		return;
	if(m_wndFileView.GetSelectedItem()==(t->tree_node_union.part.part_list_id))
		return;

	update_flag=TRUE;

	m_wndFileView.SetFocus();

	m_wndFileView.SelectItem(NULL);
	m_wndFileView.SetItemState(NULL,TVIS_SELECTED,TVIS_SELECTED);

	m_wndFileView.SelectItem(t->tree_node_union.part.part_list_id);
	m_wndFileView.SetItemState(t->tree_node_union.part.part_list_id,TVIS_SELECTED,TVIS_SELECTED);

	return;
}

void BridgepartView::view_lock_at_step(int main_step_id, int sub_step_id,Cmount_playerView* pView)
{
	int i,j;
	double box[6];
	part_tree *t;

	bridge_view->bridge->tree->clear_mark_flag();

	for(i=0,j=0;i<bridge_mounter->move_number;i++){
		if(bridge_mounter->move[i]->main_step==main_step_id){
			if(bridge_mounter->move[i]->sub_step==sub_step_id){
				if((t=bridge_view->bridge->tree->get_node_by_name(bridge_mounter->move[i]->name))!=NULL){
					t->set_mark_flag();
					j++;
					if(j==1){
						box[0]=t->box[0];
						box[1]=t->box[1];
						box[2]=t->box[2];
						box[3]=t->box[3];
						box[4]=t->box[4];
						box[5]=t->box[5];
					}else{
						box[0]=(t->box[0]<box[0])?t->box[0]:box[0];
						box[1]=(t->box[1]<box[1])?t->box[1]:box[1];
						box[2]=(t->box[2]<box[2])?t->box[2]:box[2];
						box[3]=(t->box[3]>box[3])?t->box[3]:box[3];
						box[4]=(t->box[4]>box[4])?t->box[4]:box[4];
						box[5]=(t->box[5]>box[5])?t->box[5]:box[5];
					}
				}
			}
		}
	}
	if(par->change_view_flag)
			pView->put_view_on_box(box,par->scaleup_view_flag);	
	if(j>0)
		bridge_mounter->set_box(box);
	return;
}
void BridgepartView::modify_image(part_tree *t,BOOL flag)
{
	int id;
	movement_index_of_part *m;

	id=flag?3:5;

	m_wndFileView.SetItemImage(t->tree_node_union.part.part_list_id,id,id);

	for(m=t->tree_node_union.part.move_index_of_part;m!=NULL;m=m->next_movement_index)
		m_wndFileView.SetItemImage(m->tree_item_id_of_movement_index,id,id);

	return;
}

BOOL BridgepartView::PreTranslateMessage(MSG* pMsg)
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
