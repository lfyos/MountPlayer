
#include "stdafx.h"

#include "OutputWnd.h"
#include "Resource.h"
#include "MainFrm.h"

#include "FileView.h"

#include "mount_player.h"
#include "mount_view.h"
#include "mount_playerDoc.h"
#include "mount_playerView.h"

#include "mount_processor.h"
#include "system_parameter.h"
#include "movement.h"
#include "mount_bridge.h"
#include "part_tree.h"
#include "part_info.h"
#include "part_index_of_movement.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

BridgeMoveView::BridgeMoveView()
{
}

BridgeMoveView::~BridgeMoveView()
{
}

BEGIN_MESSAGE_MAP(BridgeMoveView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_PROPERTIES, &BridgeMoveView::OnProperties)

//	ON_NOTIFY(NM_CLICK, 4, OnClickPartTree)
	ON_NOTIFY(TVN_SELCHANGED, 4, OnClickPartTree)
//	ON_NOTIFY(NM_DBLCLK, 4, OnClickPartTree)
	
END_MESSAGE_MAP()

int BridgeMoveView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
		CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create view:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_MoveView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}

	// Load view images:
	m_MoveViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_MoveView.SetImageList(&m_MoveViewImages, TVSIL_NORMAL);

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

void BridgeMoveView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}



void BridgeMoveView::FillFileView(void)
{
	fill_tree_item();
}

void BridgeMoveView::AdjustLayout(void)
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_MoveView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);

}

void BridgeMoveView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_MoveView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void BridgeMoveView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_MoveView.SetFocus();
}

void BridgeMoveView::OnChangeVisualStyle(void)
{

	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* Locked */);

	m_MoveViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

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

	m_MoveViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_MoveViewImages.Add(&bmp, RGB(255, 0, 0));

	m_MoveView.SetImageList(&m_MoveViewImages, TVSIL_NORMAL);

}

void BridgeMoveView::OnProperties()
{
	// TODO: Add your command handler code here
	fill_tree_item();
}

static BOOL update_flag=TRUE;
void BridgeMoveView::fill_tree_item(void)
{
	int i,j,k;
	CString str;
	HTREEITEM root_id,main_id,sub_id;

	update_flag=TRUE;

	m_MoveView.DeleteAllItems();
	root_id=m_MoveView.InsertItem(_T("拼装进度"), 2, 2);
	m_MoveView.SetItemState(root_id, TVIS_BOLD, TVIS_BOLD);

	for(i=0;i<(bridge_mounter->move_number);i=k){
		str.Format("第%4d 大步",bridge_mounter->move[i]->main_step);
		main_id=m_MoveView.InsertItem(str, 1, 1, root_id);
		for(j=i;j<(bridge_mounter->move_number);j=k){
			str.Format("第%4d 小步",bridge_mounter->move[j]->sub_step);
			sub_id=m_MoveView.InsertItem(str,(bridge_mounter->current_step<=j)?3:5, (bridge_mounter->current_step<=j)?3:5, main_id);
			for(k=j;k<(bridge_mounter->move_number);k++){
				if(bridge_mounter->move[k]->main_step!=bridge_mounter->move[j]->main_step)
					break;
				if(bridge_mounter->move[k]->sub_step!=bridge_mounter->move[j]->sub_step)
					break;
				bridge_mounter->move[k]->main_list_id=main_id;
				bridge_mounter->move[k]->sub_list_id=sub_id;

				part_index_of_movement  *i_m=bridge_mounter->move[k]->part_index_of_move;
				int l=1,m=1,last_part_id=-1;

				for(;i_m!=NULL;i_m=i_m->next_part_index,l++,m++){
					if(i_m->part_id!=last_part_id){
						l=1;
						last_part_id=i_m->part_id;
					}
					str.Format("%3d：%3d：第%4d件",m,l,i_m->part_component_id+1);
					str+=bridge_view->bridge->part[i_m->part_id]->information.name;
					i_m->tree_item_id_of_part_index=m_MoveView.InsertItem(str,
						(bridge_mounter->current_step<=k)?3:5, 
						(bridge_mounter->current_step<=k)?3:5,sub_id);
				}
			}
			if(k>=bridge_mounter->move_number)
				break;
			if(bridge_mounter->move[i]->main_step!=bridge_mounter->move[k]->main_step)
				break;
		}
	}
	m_MoveView.Expand(root_id,TVE_EXPAND);

	if((k=bridge_mounter->current_step)>=(bridge_mounter->move_number))
		return;
	if(bridge_mounter->move[k]==NULL)
		return;
	m_MoveView.SelectItem(bridge_mounter->move[k]->sub_list_id);

	return;
}
int BridgeMoveView::get_select_step_id(int *end_step_id)
{
	int i,j;
	HTREEITEM list_id;

	list_id=m_MoveView.GetSelectedItem();
	for(i=0;i<(bridge_mounter->move_number);i++){
		if(list_id==(bridge_mounter->move[i]->sub_list_id)){
			for(*end_step_id=i;;(*end_step_id)++){
				if((*end_step_id)>=(bridge_mounter->move_number)){
					(*end_step_id)--;
					break;
				}
				if(list_id!=(bridge_mounter->move[*end_step_id]->sub_list_id)){
					(*end_step_id)--;
					break;
				}
			}
			return i;
		}
		if(list_id==(bridge_mounter->move[i]->main_list_id)){
			for(j=i+1;;j++){
				if(j>=(bridge_mounter->move_number)){
					*end_step_id=bridge_mounter->move_number-1;
					break;
				}
				if(list_id!=(bridge_mounter->move[j]->main_list_id)){
					*end_step_id=j-1;
					return i;
				}
			}
			*end_step_id=bridge_mounter->move_number-1;
			return i;
		}
	}
	*end_step_id=-1;
	return -1;
}
void BridgeMoveView::set_mount_step(int step_id)
{
	HTREEITEM move_list_id;

	if(step_id<0)
		move_list_id=NULL;
	else if(step_id>=(bridge_mounter->move_number))
		move_list_id=NULL;
	else
		move_list_id=bridge_mounter->move[step_id]->sub_list_id;
	if(move_list_id==m_MoveView.GetSelectedItem())
		return;

	update_flag=TRUE;
	
//	m_MoveView.SetFocus();

//	m_MoveView.SelectItem(NULL);
//	m_MoveView.SetItemState(NULL,TVIS_SELECTED,TVIS_SELECTED);

	m_MoveView.SelectItem(move_list_id);
	m_MoveView.SetItemState(move_list_id,TVIS_SELECTED,TVIS_SELECTED);

	return;
}
void BridgeMoveView::OnClickPartTree(NMHDR* pNMHDR, LRESULT* pResult)
{	
	int i,j,step_id,*visible_part_number,*hide_part_number;;
	double box[6];	
	HTREEITEM list_id;
	part_tree *t;

	*pResult = 0;

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
	
	{
			step_id=-1;
			visible_part_number	=new int[bridge_view->bridge->part_number];
			hide_part_number	=new int[bridge_view->bridge->part_number];

			for(i=0;i<bridge_view->bridge->part_number;i++){
				visible_part_number[i]=0;
				hide_part_number[i]=0;
			}
	}

	bridge_view->bridge->tree->clear_mark_flag();

	for(i=0,j=0,list_id=m_MoveView.GetSelectedItem();i<(bridge_mounter->move_number);i++){
		if((list_id==bridge_mounter->move[i]->main_list_id)||(list_id==bridge_mounter->move[i]->sub_list_id)){
			if((t=bridge_view->bridge->tree->get_node_by_name(bridge_mounter->move[i]->name))!=NULL){
				t->get_part_reference_number(visible_part_number,hide_part_number,TRUE);
				step_id=i;

				t->set_mark_flag();
				if(j<=0){
						box[0]=t->box[0];
						box[1]=t->box[1];
						box[2]=t->box[2];		
						box[3]=t->box[3];
						box[4]=t->box[4];
						box[5]=t->box[5];		
				}else{
						box[0]=(box[0]<(t->box[0]))?box[0]:(t->box[0]);
						box[1]=(box[1]<(t->box[1]))?box[1]:(t->box[1]);
						box[2]=(box[2]<(t->box[2]))?box[2]:(t->box[2]);

						box[3]=(box[3]>(t->box[3]))?box[3]:(t->box[3]);
						box[4]=(box[4]>(t->box[4]))?box[4]:(t->box[4]);
						box[5]=(box[5]>(t->box[5]))?box[5]:(t->box[5]);
				}
				j++;
			}
		}
	}

	if(j>0){
		if(par->change_view_flag)
			pView->put_view_on_box(box,par->scaleup_view_flag);
		bridge_mounter->set_box(box);

		((CMainFrame*)(theApp.m_pMainWnd))->display_mount_information(visible_part_number,hide_part_number,step_id);
	}
	delete []hide_part_number;
	delete []visible_part_number;
	
	pView->InvalidateRect(NULL,FALSE);

	if(j>0)
		return;

	part_index_of_movement *i_p;

	for(i=0,list_id=m_MoveView.GetSelectedItem();i<(bridge_mounter->move_number);i++)
		for(i_p=bridge_mounter->move[i]->part_index_of_move;i_p!=NULL;i_p=i_p->next_part_index)
			if(list_id==(i_p->tree_item_id_of_part_index)){
				for(t=bridge_view->bridge->first_part_tree[i_p->part_id];t!=NULL;t=t->tree_node_union.part.back_part){
					if(t->tree_node_union.part.part_component_id==i_p->part_component_id){
						((CMainFrame*)(theApp.m_pMainWnd))->update_display(t,t,-1);
						return;
					}
				}
				return;
			};
	return;
}
void BridgeMoveView::modify_image(movement *m,BOOL flag)
{
	int id;
	part_index_of_movement *p;

	id=flag?3:5;

	m_MoveView.SetItemImage(m->sub_list_id,id,id);
	for(p=m->part_index_of_move;p!=NULL;p=p->next_part_index)
		m_MoveView.SetItemImage(p->tree_item_id_of_part_index,id,id);

	return;
}

BOOL BridgeMoveView::PreTranslateMessage(MSG* pMsg)
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
