
// mount_playerView.cpp : Cmount_playerView 类的实现
//
#include "stdafx.h"
#include "mount_player.h"

#include "mount_playerDoc.h"
#include "mount_playerView.h"


#include "mount_light_dialog.h"
#include "system_parameter.h"
#include "virtual_mount_dialog.h"
#include "mount_location_dialog.h"
#include "subtitle_input_dialog.h"
#include "input_main_sub_step_dialog.h"
#include "movement.h"
#include "mount_bridge.h"
#include "part_tree.h"
#include "movement_index_of_part.h"
#include "movement_cache.h"
#include "movement_label_dialog.h"
#include "coordinate_computation.h"
#include "part_image.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cmount_playerView

BOOL Cmount_playerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

void Cmount_playerView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

// Cmount_playerView 诊断

#ifdef _DEBUG
void Cmount_playerView::AssertValid() const
{
	CView::AssertValid();
}

void Cmount_playerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cmount_playerDoc* Cmount_playerView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cmount_playerDoc)));
	return (Cmount_playerDoc*)m_pDocument;
}
#endif //_DEBUG

// Cmount_playerView 构造/析构
Cmount_playerView::~Cmount_playerView()
{
}
Cmount_playerView::Cmount_playerView()
{
	// TODO: 在此处添加构造代码
	bridge_gs=NULL;
	m_hAccel=NULL;
}
// Cmount_playerView 消息处理程序

IMPLEMENT_DYNCREATE(Cmount_playerView, CView)

BEGIN_MESSAGE_MAP(Cmount_playerView, CView)

ON_COMMAND_RANGE(BEGIN_RIGHT_BUTTON_COMMOND_ID,END_RIGHT_BUTTON_COMMOND_ID,right_button_add_movement)
ON_WM_TIMER()
ON_WM_CREATE()	
ON_WM_DESTROY()

ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_WM_MOUSEWHEEL()

ON_WM_CONTEXTMENU()

ON_COMMAND(MENU_HIDE_COMPONENT, &Cmount_playerView::OnHideComponent)
ON_COMMAND(MENU_HIDE_ALL_COMPONENT, &Cmount_playerView::OnHideAllComponent)
ON_COMMAND(MENU_CLEAR_HIDE_COMPONENT, &Cmount_playerView::OnClearHideComponent)
ON_COMMAND(MENU_DISPLAY_BOX, &Cmount_playerView::OnDisplayBox)
ON_COMMAND(MENU_CLEAR_DISPLAY_BOX, &Cmount_playerView::OnClearDisplayBox)
ON_COMMAND(MENU_UP_SELECTION, &Cmount_playerView::OnUpSelection)
ON_COMMAND(MENU_DOWN_SELECTION, &Cmount_playerView::OnDownSelection)
ON_COMMAND(MENU_CLEAR_COMPONENT_TRANSPARENT, &Cmount_playerView::OnClearComponentTransparent)
ON_COMMAND(MENU_SET_COMPONENT_TRANSPARENT, &Cmount_playerView::OnSetComponentTransparent)
ON_COMMAND(MENU_SET_CLIP_PLANE, &Cmount_playerView::OnSetClipPlane)
ON_COMMAND(MENU_LOCK_COMPONENT, &Cmount_playerView::OnLockComponent)
ON_COMMAND(MENU_PUT_IN_VIEW, &Cmount_playerView::OnPutInView)
ON_COMMAND(MENU_PUT_IN_EQUAL_VIEW, &Cmount_playerView::OnPutInEqualView)
ON_COMMAND(MENU_VIEW_X, &Cmount_playerView::OnViewX)
ON_COMMAND(MENU_VIEW_Y, &Cmount_playerView::OnViewY)
ON_COMMAND(MENU_VIEW_Z, &Cmount_playerView::OnViewZ)
ON_COMMAND(MENU_BACKWARD_VIEW, &Cmount_playerView::OnBackwardView)
ON_COMMAND(MENU_PIN_POINT, &Cmount_playerView::OnPinPoint)
ON_COMMAND(MENU_INTERACT_TRANSLATION, &Cmount_playerView::OnInteractTranslation)
ON_COMMAND(MENU_INTERACT_ROTATION, &Cmount_playerView::OnInteractRotation)
ON_COMMAND(MENU_INTERACT_ENLARGE_AREA, &Cmount_playerView::OnInteractEnlargeArea)
ON_COMMAND(MENU_INTERACT_SELECTION, &Cmount_playerView::OnInteractSelection)
ON_COMMAND(MENU_INTERACT_SCALE, &Cmount_playerView::OnInteractScale)
ON_COMMAND(MENU_INTERACT_SPEED, &Cmount_playerView::OnInteractSpeed)
ON_COMMAND(MENU_INTERACT_CLIP_PLANE, &Cmount_playerView::OnInteractClipPlane)
ON_COMMAND(MENU_INTERACT_COMPONENT_TRANSPARENCE, &Cmount_playerView::OnInteractComponentTransparence)
ON_COMMAND(MENU_INTERACT_MANUAL_MOUNT, &Cmount_playerView::OnInteractManualMount)

ON_UPDATE_COMMAND_UI(MENU_PIN_POINT, &Cmount_playerView::OnUpdatePinPoint)
ON_UPDATE_COMMAND_UI(MENU_INTERACT_TRANSLATION, &Cmount_playerView::OnUpdateInteractTranslation)
ON_UPDATE_COMMAND_UI(MENU_INTERACT_ROTATION, &Cmount_playerView::OnUpdateInteractRotation)
ON_UPDATE_COMMAND_UI(MENU_INTERACT_ENLARGE_AREA, &Cmount_playerView::OnUpdateInteractEnlargeArea)
ON_UPDATE_COMMAND_UI(MENU_INTERACT_SELECTION, &Cmount_playerView::OnUpdateInteractSelection)
ON_UPDATE_COMMAND_UI(MENU_INTERACT_SCALE, &Cmount_playerView::OnUpdateInteractScale)
ON_UPDATE_COMMAND_UI(MENU_INTERACT_SPEED, &Cmount_playerView::OnUpdateInteractSpeed)
ON_UPDATE_COMMAND_UI(MENU_INTERACT_CLIP_PLANE, &Cmount_playerView::OnUpdateInteractClipPlane)
ON_UPDATE_COMMAND_UI(MENU_INTERACT_COMPONENT_TRANSPARENCE, &Cmount_playerView::OnUpdateInteractComponentTransparence)
ON_UPDATE_COMMAND_UI(MENU_INTERACT_MANUAL_MOUNT, &Cmount_playerView::OnUpdateInteractManualMount)

ON_COMMAND(MENU_STOP_MOUNT, &Cmount_playerView::OnStopMount)
ON_COMMAND(MENU_FORWARD_ONE, &Cmount_playerView::OnForwardOne)
ON_COMMAND(MENU_FORWARD_ALL, &Cmount_playerView::OnForwardAll)
ON_COMMAND(MENU_BACKWARD_ONE, &Cmount_playerView::OnBackwardOne)
ON_COMMAND(MENU_BACKWARD_ALL, &Cmount_playerView::OnBackwardAll)
ON_COMMAND(MENU_BACKWARD_BIG, &Cmount_playerView::OnBackwardBig)
ON_COMMAND(MENU_FORWARD_BIG, &Cmount_playerView::OnForwardBig)

ON_COMMAND(MENU_JUMP_STEP, &Cmount_playerView::OnJumpStep)
ON_COMMAND(MENU_MODIFY_LIGHT, &Cmount_playerView::OnModifyLight)
ON_COMMAND(MENU_PLAY_SOUND, &Cmount_playerView::OnPlaySound)
ON_UPDATE_COMMAND_UI(MENU_PLAY_SOUND, &Cmount_playerView::OnUpdatePlaySound)
ON_COMMAND(MENU_CHANGE_VIEW, &Cmount_playerView::OnChangeView)
ON_UPDATE_COMMAND_UI(MENU_CHANGE_VIEW, &Cmount_playerView::OnUpdateChangeView)
ON_COMMAND(MENU_SCALEUP_VIEW, &Cmount_playerView::OnScaleupView)
ON_UPDATE_COMMAND_UI(MENU_SCALEUP_VIEW, &Cmount_playerView::OnUpdateScaleupView)

ON_COMMAND(MENU_VIRTUAL_MOUNT, &Cmount_playerView::OnVirtualMount)

ON_COMMAND(MENU_DESIGN_INPUT, &Cmount_playerView::OnDesignInput)
ON_COMMAND(MENU_DESIGN_DELETE, &Cmount_playerView::OnDesignDelete)
ON_COMMAND(MENU_DESIGN_INPUT_MESSAGE, &Cmount_playerView::OnDesignInputMessage)
ON_COMMAND(MENU_DESIGN_MAIN_SORT, &Cmount_playerView::OnDesignMainSort)
ON_COMMAND(MENU_DESIGN_SUB_SORT, &Cmount_playerView::OnDesignSubSort)

ON_COMMAND(MENU_SAVE_MOVEMENT, &Cmount_playerView::OnSaveMovement)
ON_COMMAND(MENU_RESTORE_MOVEMENT, &Cmount_playerView::OnRestoreMovement)
ON_COMMAND(MENU_MOVE_MOVEMENT, &Cmount_playerView::OnMoveMovement)
ON_COMMAND(MENU_COMBINE_STEP, &Cmount_playerView::OnCombineStep)
ON_COMMAND(MENU_SEPARATE_STEP, &Cmount_playerView::OnSeparateStep)

ON_COMMAND(MENU_MOUNT_PROGRESS_TABLE, &Cmount_playerView::OnMountProgressTable)
ON_COMMAND(MENU_MAIN_STEP_DECREASE, &Cmount_playerView::OnMainStepDecrease)
ON_COMMAND(MENU_MAIN_STEP_INCREASE, &Cmount_playerView::OnMainStepIncrease)
ON_COMMAND(MENU_CURRENT_MOUNT_STEP, &Cmount_playerView::OnCurrentMountStep)
ON_COMMAND(MENU_SUB_STEP_DECREASE, &Cmount_playerView::OnSubStepDecrease)
ON_COMMAND(MENU_SUB_STEP_INCREASE, &Cmount_playerView::OnSubStepIncrease)
ON_COMMAND(MENU_CACHE_INSERT_OR_DELETE, &Cmount_playerView::OnCacheInsertOrDelete)

ON_UPDATE_COMMAND_UI(MENU_CACHE_INSERT_OR_DELETE, &Cmount_playerView::OnUpdateCacheInsertOrDelete)
ON_COMMAND(MENU_CACHE_SUB_DECREASE_MODE, &Cmount_playerView::OnCacheSubDecreaseMode)
ON_UPDATE_COMMAND_UI(MENU_CACHE_SUB_DECREASE_MODE, &Cmount_playerView::OnUpdateCacheSubDecreaseMode)
ON_COMMAND(MENU_CACHE_SUB_INCREASE_MODE, &Cmount_playerView::OnCacheSubIncreaseMode)
ON_UPDATE_COMMAND_UI(MENU_CACHE_SUB_INCREASE_MODE, &Cmount_playerView::OnUpdateCacheSubIncreaseMode)
ON_COMMAND(MENU_CACHE_SUB_REMAIN_MODE, &Cmount_playerView::OnCacheSubRemainMode)
ON_UPDATE_COMMAND_UI(MENU_CACHE_SUB_REMAIN_MODE, &Cmount_playerView::OnUpdateCacheSubRemainMode)

ON_COMMAND(MENU_SET_MANUAL_MOUNT, &Cmount_playerView::OnSetManualMount)
ON_UPDATE_COMMAND_UI(MENU_SET_MANUAL_MOUNT, &Cmount_playerView::OnUpdateSetManualMount)

ON_COMMAND(MENU_MOUNT_PART_TABLE, &Cmount_playerView::OnMountPartTable)
ON_COMMAND(MENU_DISPLAY_REFERENCE_FRAME, &Cmount_playerView::OnDisplayReferenceFrame)
ON_UPDATE_COMMAND_UI(MENU_DISPLAY_REFERENCE_FRAME, &Cmount_playerView::OnUpdateDisplayReferenceFrame)
ON_COMMAND(MENU_TREE_MOVEMENT_SAVE_TO_CACHE, &Cmount_playerView::OnTreeMovementSaveToCache)
ON_UPDATE_COMMAND_UI(MENU_SET_CLIP_PLANE, &Cmount_playerView::OnUpdateSetClipPlane)
END_MESSAGE_MAP()


// CbridgeView 消息处理程序
int Cmount_playerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	SetTimer(100,20,NULL);

	SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE );

	m_hAccel = ::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	return 0;
}
BOOL Cmount_playerView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if(m_hAccel!=NULL)
		if(::TranslateAccelerator(m_hWnd,m_hAccel,pMsg))   
			return   TRUE; 

	return CView::PreTranslateMessage(pMsg);
}
void Cmount_playerView::OnDestroy()
{
	KillTimer(100);

	CView::OnDestroy();

	// TODO: Add your message handler code here
	if(bridge_mounter!=NULL)
	if(bridge_mounter->should_flush_movement_flag)
	if(AfxMessageBox(_T("您修改了拼装信息，修改的信息写回文件中吗？"),MB_YESNO)==IDYES)
			bridge_mounter->flush_movement(bridge_mounter->bak_movement_file_name);

	if(bridge_gs!=NULL)
		delete bridge_gs;

	return;
}
static BOOL need_redraw_sreen_flag=TRUE;
void Cmount_playerView::OnDraw(CDC* pDC)
{
	Cmount_playerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	
	if(bridge_gs!=NULL)
		bridge_gs->begin_display(pDC);
	else{
		bridge_gs=new opengl_graphics_system(pDC);

		bridge_gs->begin_display(pDC);

		bridge_view->bridge->create_call_list(0,100,bridge_gs);

		bridge_view->save_view_data();
		bridge_view->put_box_in_view(bridge_gs,bridge_view->bridge->tree->box);
		bridge_view->set_modifier();
	}
	DWORD start_display_time=::GetTickCount();

	int return_value=bridge_view->do_display(0,bridge_gs,par->manual_mount_flag?par->manual_mount_proportion:-1);

	for(int i=0;i<bridge_view->bridge->image_number;i++)
		if(bridge_view->bridge->images[i]->process_picture(0,bridge_gs))
			need_redraw_sreen_flag=TRUE;

	bridge_gs->end_display(pDC,TRUE);

	display_time_length=::GetTickCount()-start_display_time;
	if(return_value>0)
		need_redraw_sreen_flag=TRUE;
	return;
}
void Cmount_playerView::OnHideComponent()
{
	// TODO: Add your command handler code here
	int i,max_number;
	part_tree *buffer[MAX_TREE_NODE_COLLECTION_NUMBER];
	double my_box[6];

	if((max_number=bridge_view->bridge->collect_mark_tree_nodes(MAX_TREE_NODE_COLLECTION_NUMBER,buffer,my_box))>0){
		for(i=0;i<max_number;i++)
			buffer[i]->set_visible_flag(FALSE);
		((CMainFrame*)(theApp.m_pMainWnd))->fill_tree_item();
		need_redraw_sreen_flag=TRUE;
	}
	return;
}
void Cmount_playerView::OnHideAllComponent()
{
	// TODO: Add your command handler code here
	int max_number;
	part_tree *buffer[MAX_TREE_NODE_COLLECTION_NUMBER];
	double my_box[6];

	if((max_number=bridge_view->bridge->collect_mark_tree_nodes(MAX_TREE_NODE_COLLECTION_NUMBER,buffer,my_box))>0){
		bridge_view->bridge->only_display_us(buffer,max_number);
		((CMainFrame*)(theApp.m_pMainWnd))->fill_tree_item();
		need_redraw_sreen_flag=TRUE;
	}
	return;
}
void Cmount_playerView::OnClearHideComponent()
{
	// TODO: Add your command handler code here
	int i,max_number;
	part_tree *buffer[MAX_TREE_NODE_COLLECTION_NUMBER];
	double my_box[6];

	need_redraw_sreen_flag=TRUE;
	bridge_mounter->stop_mount();

	if((max_number=bridge_view->bridge->collect_mark_tree_nodes(MAX_TREE_NODE_COLLECTION_NUMBER,buffer,my_box))<=0){
		max_number=1;
		buffer[0]=bridge_view->bridge->tree;
	}
	for(i=0;i<max_number;i++)
		buffer[i]->clear_visible_flag();

	bridge_mounter->initialize_visible_state_and_movement_index();
	((CMainFrame*)(theApp.m_pMainWnd))->fill_tree_item();
	
	return;
}
void Cmount_playerView::OnDisplayBox()
{
	// TODO: Add your command handler code here
	int i,max_number;
	part_tree *buffer[MAX_TREE_NODE_COLLECTION_NUMBER];
	double my_box[6];

	if((max_number=bridge_view->bridge->collect_mark_tree_nodes(MAX_TREE_NODE_COLLECTION_NUMBER,buffer,my_box))>0){
		for(i=0;i<max_number;i++)
			buffer[i]->set_display_box_or_body(TRUE,TRUE,FALSE);

		need_redraw_sreen_flag=TRUE;
	}
	return;
}
void Cmount_playerView::OnClearDisplayBox()
{
	// TODO: Add your command handler code here
	int i,max_number;
	part_tree *buffer[MAX_TREE_NODE_COLLECTION_NUMBER];
	double my_box[6];

	if((max_number=bridge_view->bridge->collect_mark_tree_nodes(MAX_TREE_NODE_COLLECTION_NUMBER,buffer,my_box))<=0){
		max_number=1;
		buffer[0]=bridge_view->bridge->tree;
	}
	
	for(i=0;i<max_number;i++)
		buffer[i]->set_display_box_or_body(FALSE,TRUE,TRUE);

	need_redraw_sreen_flag=TRUE;

	return;
}

void Cmount_playerView::OnUpSelection()
{
	// TODO: Add your command handler code here

	part_tree *t;

	if((t=bridge_view->bridge->tree->get_last_mark_node(FALSE))==NULL)
		t=bridge_view->bridge->tree->get_last_mark_node(TRUE);
	if(t==NULL)
		return;
	if(t->parent!=NULL)
		t=t->parent;

	bridge_view->bridge->tree->clear_mark_flag();
	t->set_mark_flag();
	
	((CMainFrame*)(theApp.m_pMainWnd))->display_tree_information(t);
	((CMainFrame*)(theApp.m_pMainWnd))->clear_selection();
	((CMainFrame*)(theApp.m_pMainWnd))->update_display(t,t,t->get_min_step());
	bridge_mounter->set_box(t->box);
	
	SetFocus();
	need_redraw_sreen_flag=TRUE;
	return;
}
void Cmount_playerView::OnDownSelection()
{
	// TODO: Add your command handler code here
	
	part_tree *t,*p;

	if((t=bridge_view->bridge->tree->get_last_mark_node(FALSE))==NULL)
		t=bridge_view->bridge->tree->get_last_mark_node(TRUE);
	if(t==NULL)
		return;
	if(t->type!=0){
		int i,n;
		n=t->tree_node_union.assembly.child_number;
		p=t->tree_node_union.assembly.first_child;

		for(i=0,t=p;i<n;i++,p=p->back_brother)
			if(p->mark_number>t->mark_number)
				t=p;
	}

	bridge_view->bridge->tree->clear_mark_flag();
	t->set_mark_flag();
	
	((CMainFrame*)(theApp.m_pMainWnd))->display_tree_information(t);
	((CMainFrame*)(theApp.m_pMainWnd))->clear_selection();
	((CMainFrame*)(theApp.m_pMainWnd))->update_display(t,t,t->get_min_step());
	bridge_mounter->set_box(t->box);
	
	SetFocus();
	need_redraw_sreen_flag=TRUE;

	return;
}
int max_transparent_tree_node_number=0;
static part_tree *transparent_tree[MAX_TREE_NODE_COLLECTION_NUMBER];
static double transparent_ratio=0.5;
void Cmount_playerView::OnClearComponentTransparent()
{
	// TODO: Add your command handler code here
	int i;
	double my_box[6];

	if((max_transparent_tree_node_number=bridge_view->bridge->collect_mark_tree_nodes(MAX_TREE_NODE_COLLECTION_NUMBER,transparent_tree,my_box))<=0){
		max_transparent_tree_node_number=1;
		transparent_tree[0]=bridge_view->bridge->tree;
	}
	bridge_view->bridge->tree->clear_mark_flag();
	for(i=0;i<max_transparent_tree_node_number;i++)
		transparent_tree[i]->set_transparence(-1);

	need_redraw_sreen_flag=TRUE;

	return;
}

void Cmount_playerView::OnSetComponentTransparent()
{
	// TODO: Add your command handler code here

	int i;
	double my_box[6];

	if((max_transparent_tree_node_number=bridge_view->bridge->collect_mark_tree_nodes(MAX_TREE_NODE_COLLECTION_NUMBER,transparent_tree,my_box))>0){
		bridge_view->bridge->tree->clear_mark_flag();
		for(i=0;i<max_transparent_tree_node_number;i++)
			transparent_tree[i]->set_transparence(transparent_ratio);
	}
	need_redraw_sreen_flag=TRUE;

	return;
}

void Cmount_playerView::OnSetClipPlane()
{
	// TODO: Add your command handler code here
	bridge_view->set_clip_plane(
		bridge_view->clip_plane_flag?FALSE:TRUE,
		bridge_view->center_x,
		bridge_view->center_y,
		bridge_view->center_z);
	need_redraw_sreen_flag=TRUE;	
	return;
}
void Cmount_playerView::OnUpdateSetClipPlane(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(bridge_view->clip_plane_flag);
	pCmdUI->SetText(bridge_view->clip_plane_flag?"取消剖分":"启动剖分");
}
void Cmount_playerView::OnLockComponent()
{
	// TODO: Add your command handler code here
	part_tree *buffer[MAX_TREE_NODE_COLLECTION_NUMBER];
	double my_box[6],*p;

	if((bridge_view->bridge->collect_mark_tree_nodes(MAX_TREE_NODE_COLLECTION_NUMBER,buffer,my_box))>0)
				p=&(my_box[0]);
	else if((bridge_view->bridge->collect_visible_tree_nodes(MAX_TREE_NODE_COLLECTION_NUMBER,buffer,my_box))>0)
				p=&(my_box[0]);
	else
				p=&(bridge_view->bridge->tree->box[0]);
		
	put_view_on_box(p,FALSE);
	return;
}
void Cmount_playerView::OnPutInView()
{
	// TODO: Add your command handler code here
	part_tree *buffer[MAX_TREE_NODE_COLLECTION_NUMBER];
	double my_box[6],*p;

	if((bridge_view->bridge->collect_mark_tree_nodes(MAX_TREE_NODE_COLLECTION_NUMBER,buffer,my_box))>0)
				p=&(my_box[0]);
	else if((bridge_view->bridge->collect_visible_tree_nodes(MAX_TREE_NODE_COLLECTION_NUMBER,buffer,my_box))>0)
				p=&(my_box[0]);
	else
				p=&(bridge_view->bridge->tree->box[0]);
	put_view_on_box(p,TRUE);
	return;
}

void Cmount_playerView::OnPutInEqualView()
{
	// TODO: Add your command handler code here
	
	part_tree *buffer[MAX_TREE_NODE_COLLECTION_NUMBER];
	double my_box[6],*p;
	CPaintDC dc(this);

	if((bridge_view->bridge->collect_mark_tree_nodes(MAX_TREE_NODE_COLLECTION_NUMBER,buffer,my_box))>0)
				p=&(my_box[0]);
	else if((bridge_view->bridge->collect_visible_tree_nodes(MAX_TREE_NODE_COLLECTION_NUMBER,buffer,my_box))>0)
				p=&(my_box[0]);
	else
				p=&(bridge_view->bridge->tree->box[0]);

	bridge_gs->begin_display(&dc);
	bridge_view->save_view_data();
	bridge_view->set_equal_view();
	bridge_view->put_box_in_view(bridge_gs,p);
	bridge_view->set_modifier();
	bridge_gs->end_display(&dc,FALSE);
	
	need_redraw_sreen_flag=TRUE;	

	return;
}

void Cmount_playerView::OnViewX()
{
	// TODO: Add your command handler code here
	
	part_tree *buffer[MAX_TREE_NODE_COLLECTION_NUMBER];
	double my_box[6],*p;
	CPaintDC dc(this);

	if((bridge_view->bridge->collect_mark_tree_nodes(MAX_TREE_NODE_COLLECTION_NUMBER,buffer,my_box))>0)
				p=&(my_box[0]);
	else if((bridge_view->bridge->collect_visible_tree_nodes(MAX_TREE_NODE_COLLECTION_NUMBER,buffer,my_box))>0)
				p=&(my_box[0]);
	else
				p=&(bridge_view->bridge->tree->box[0]);

	bridge_gs->begin_display(&dc);
	bridge_view->save_view_data();
	bridge_view->set_x_view();
	bridge_view->adjust_view();
	bridge_view->put_box_in_view(bridge_gs,p);
	bridge_view->set_modifier();
	bridge_gs->end_display(&dc,FALSE);

	need_redraw_sreen_flag=TRUE;	

	return;
}

void Cmount_playerView::OnViewY()
{
	// TODO: Add your command handler code here
	
	part_tree *buffer[MAX_TREE_NODE_COLLECTION_NUMBER];
	double my_box[6],*p;
	CPaintDC dc(this);

	if((bridge_view->bridge->collect_mark_tree_nodes(MAX_TREE_NODE_COLLECTION_NUMBER,buffer,my_box))>0)
				p=&(my_box[0]);
	else if((bridge_view->bridge->collect_visible_tree_nodes(MAX_TREE_NODE_COLLECTION_NUMBER,buffer,my_box))>0)
				p=&(my_box[0]);
	else
				p=&(bridge_view->bridge->tree->box[0]);

	bridge_gs->begin_display(&dc);
	bridge_view->save_view_data();
	bridge_view->set_y_view();
	bridge_view->adjust_view();
	bridge_view->put_box_in_view(bridge_gs,p);
	bridge_view->set_modifier();
	bridge_gs->end_display(&dc,FALSE);

	need_redraw_sreen_flag=TRUE;	

	return;
}

void Cmount_playerView::OnViewZ()
{
	// TODO: Add your command handler code here
	
	part_tree *buffer[MAX_TREE_NODE_COLLECTION_NUMBER];
	double my_box[6],*p;
	CPaintDC dc(this);

	if((bridge_view->bridge->collect_mark_tree_nodes(MAX_TREE_NODE_COLLECTION_NUMBER,buffer,my_box))>0)
				p=&(my_box[0]);
	else if((bridge_view->bridge->collect_visible_tree_nodes(MAX_TREE_NODE_COLLECTION_NUMBER,buffer,my_box))>0)
				p=&(my_box[0]);
	else
				p=&(bridge_view->bridge->tree->box[0]);

	bridge_gs->begin_display(&dc);
	bridge_view->save_view_data();
	bridge_view->set_z_view();
	bridge_view->adjust_view();
	bridge_view->put_box_in_view(bridge_gs,p);
	bridge_view->set_modifier();
	bridge_gs->end_display(&dc,FALSE);

	need_redraw_sreen_flag=TRUE;	

	return;
}

void Cmount_playerView::OnBackwardView()
{
	// TODO: Add your command handler code here
	bridge_view->set_retreat_modifier();
	need_redraw_sreen_flag=TRUE;	
	return;
}

static int mouse_left_key_function_id=4,mouse_wheel_function_id=0;
static BOOL is_left_key_down_flag=FALSE;
static CPoint left_key_down_point;

void Cmount_playerView::OnPinPoint()
{
	// TODO: Add your command handler code here
	mouse_left_key_function_id=0;
}
void Cmount_playerView::OnUpdatePinPoint(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(mouse_left_key_function_id==0);
}
void Cmount_playerView::OnInteractTranslation()
{
	// TODO: Add your command handler code here
	mouse_left_key_function_id=1;
}
void Cmount_playerView::OnUpdateInteractTranslation(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(mouse_left_key_function_id==1);
}
void Cmount_playerView::OnInteractRotation()
{
	// TODO: Add your command handler code here
	mouse_left_key_function_id=2;
}
void Cmount_playerView::OnUpdateInteractRotation(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(mouse_left_key_function_id==2);
}
void Cmount_playerView::OnInteractEnlargeArea()
{
	// TODO: Add your command handler code here
	mouse_left_key_function_id=3;
}
void Cmount_playerView::OnUpdateInteractEnlargeArea(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(mouse_left_key_function_id==3);
}
void Cmount_playerView::OnInteractSelection()
{
	// TODO: Add your command handler code here
	mouse_left_key_function_id=4;
}
void Cmount_playerView::OnUpdateInteractSelection(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(mouse_left_key_function_id==4);

}
void Cmount_playerView::OnInteractScale()
{
	// TODO: Add your command handler code here
	mouse_wheel_function_id=0;
}
void Cmount_playerView::OnUpdateInteractScale(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(mouse_wheel_function_id==0);
}
void Cmount_playerView::OnInteractSpeed()
{
	// TODO: Add your command handler code here
	mouse_wheel_function_id=1;
}
void Cmount_playerView::OnUpdateInteractSpeed(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(mouse_wheel_function_id==1);
}

void Cmount_playerView::OnInteractClipPlane()
{
	// TODO: Add your command handler code here
	mouse_wheel_function_id=2;
}
void Cmount_playerView::OnUpdateInteractClipPlane(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(mouse_wheel_function_id==2);
}
void Cmount_playerView::OnInteractComponentTransparence()
{
	// TODO: Add your command handler code here
	mouse_wheel_function_id=3;
}
void Cmount_playerView::OnUpdateInteractComponentTransparence(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(mouse_wheel_function_id==3);
}
void Cmount_playerView::OnInteractManualMount()
{
	// TODO: Add your command handler code here
	mouse_wheel_function_id=4;
}
void Cmount_playerView::OnUpdateInteractManualMount(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(mouse_wheel_function_id==4);
}
void Cmount_playerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

//	CView::OnLButtonDown(nFlags, point);

	int body_id,face_id,triangle_id;
	BOOL xyz_flag;
	double x,y,z;
	CString str;

	CPaintDC my_dc(this);
	part_tree  *t;

	is_left_key_down_flag=TRUE;
	left_key_down_point=point;

	bridge_gs->begin_display(&my_dc);	

	switch(mouse_left_key_function_id){
	case 4:
		bridge_view->bridge->tree->clear_mark_flag();
	case 0:
		t=bridge_view->get_tree_body_face_triangle_and_xyz(0,&point,
				&body_id,&face_id,&triangle_id,&xyz_flag,&x,&y,&z,bridge_gs,
				bridge_view->speed,	par->manual_mount_flag?par->manual_mount_proportion:-1);

		if(t!=NULL){
			if(mouse_left_key_function_id==4){
				t->set_mark_flag();
				((CMainFrame*)(theApp.m_pMainWnd))->display_part_information(t);
				((CMainFrame*)(theApp.m_pMainWnd))->clear_selection();
				((CMainFrame*)(theApp.m_pMainWnd))->update_display(t,t,t->get_min_step());
			}else if(xyz_flag){
				str.Format("坐标:x=%lf,y=%lf,z=%lf",x,y,z);
				((CMainFrame*)(theApp.m_pMainWnd))->statusbar_display(str);
			}
		}else{
			if(mouse_left_key_function_id==4)
				((CMainFrame*)(theApp.m_pMainWnd))->update_display(t,t,-1);
		}
		SetFocus();
		break;
	case 1:
	case 2:
	case 3:
		bridge_view->save_view_data();
		if(display_time_length>100)
			bridge_view->bridge->tree->set_display_box_or_body(TRUE,FALSE,FALSE);
		break;
	default:
		break;
	}
	bridge_gs->end_display(&my_dc,FALSE);
	need_redraw_sreen_flag=TRUE;

	CView::OnLButtonDown(nFlags,point);
	return;
}
void Cmount_playerView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CPaintDC my_dc(this);
	
	if(is_left_key_down_flag){
		is_left_key_down_flag=FALSE;
		bridge_gs->begin_display(&my_dc);

		switch(mouse_left_key_function_id){
		case 3:
			bridge_view->process_area_enlarge(bridge_gs,left_key_down_point,point);
			bridge_view->set_modifier();		
		case 1:
		case 2:
			bridge_view->bridge->tree->restore_display_box_or_body();
			break;
		case 0:
		case 4:
		default:
			break;
		}
		bridge_gs->end_display(&my_dc,FALSE);
		need_redraw_sreen_flag=TRUE;
	}
	CView::OnLButtonUp(nFlags, point);
}
void Cmount_playerView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(is_left_key_down_flag){
		CPaintDC my_dc(this);
		
		bridge_gs->begin_display(&my_dc);	

		switch(mouse_left_key_function_id){
		case 1:
			bridge_view->process_translation(bridge_gs,left_key_down_point,point);
			left_key_down_point=point;
			break;
		case 2:
			bridge_view->process_rotation(bridge_gs,left_key_down_point,point);
			left_key_down_point=point;
			break;
		case 3:
			bridge_view->process_area_define(bridge_gs,left_key_down_point,point);
			break;
		default:
			break;
		}
		bridge_gs->end_display(&my_dc,FALSE);
		need_redraw_sreen_flag=TRUE;
	}
	CView::OnMouseMove(nFlags, point);
	return;
}
BOOL Cmount_playerView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	CString str;

	switch(mouse_wheel_function_id){
	case 0:
		bridge_view->process_scale(expl(((double)zDelta)/1500));
		break;
	case 1:
		bridge_view->speed*=expl(0-((double)zDelta)/2000);
		if(bridge_view->speed<0.01)
			bridge_view->speed=0.01;
		if(bridge_view->speed>100)
			bridge_view->speed=100;
		str.Format("拼装速度:%lf",bridge_view->speed);
		((CMainFrame*)(theApp.m_pMainWnd))->statusbar_display(str);
		break;
	case 2:
		bridge_view->process_clip_plane_modification(expl(((double)zDelta)/1500));
		str.Format("剖分平面:%lf,%lf,%lf,%lf",
			bridge_view->clip_plane_data[0],
			bridge_view->clip_plane_data[1],
			bridge_view->clip_plane_data[2],
			bridge_view->clip_plane_data[3]);
		((CMainFrame*)(theApp.m_pMainWnd))->statusbar_display(str);
		break;
	case 3:
		transparent_ratio*=expl(0-((double)zDelta)/5000);
		if(transparent_ratio<0.1)
			transparent_ratio=0.1;
		else if(transparent_ratio>0.9)
			transparent_ratio=0.9;
		{
			int i;
			for(i=0;i<max_transparent_tree_node_number;i++)
				transparent_tree[i]->set_transparence(transparent_ratio);
		}
				
		str.Format("透明度:%lf",transparent_ratio);
		((CMainFrame*)(theApp.m_pMainWnd))->statusbar_display(str);
		break;
	case 4:
		par->manual_mount_proportion+=((double)zDelta)/5000;
		if(par->manual_mount_proportion<0)
			par->manual_mount_proportion=0;
		else if(par->manual_mount_proportion>1.0)
			par->manual_mount_proportion=1.0;

		str.Format("手动拼装:%lf",par->manual_mount_proportion);
		((CMainFrame*)(theApp.m_pMainWnd))->statusbar_display(str);
		break;
	default:
		break;
	}
	need_redraw_sreen_flag=TRUE;
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void Cmount_playerView::put_view_on_box(double box[],BOOL scaleup_flag)
{
	CPaintDC my_dc(this);

	bridge_gs->begin_display(&my_dc);
	bridge_view->save_view_data();

	if(scaleup_flag)
			bridge_view->put_box_in_view(bridge_gs,box);
	else
			bridge_view->aim_at_box(box);

	bridge_view->set_modifier();
	bridge_gs->end_display(&my_dc,FALSE);
	need_redraw_sreen_flag=TRUE;

	return;
}
void Cmount_playerView::OnStopMount()
{
	// TODO: Add your command handler code here
	sound_player.sound_close();
	bridge_mounter->stop_mount();
}

void Cmount_playerView::OnForwardOne()
{
	// TODO: Add your command handler code here
	OnStopMount();
	par->manual_mount_proportion=0;
	bridge_mounter->advance_one_step();
}

void Cmount_playerView::OnForwardAll()
{
	// TODO: Add your command handler code here
	OnStopMount();
	if(par->manual_mount_flag){
		OnForwardOne();
		return;
	}
	bridge_mounter->advance_forever();
}

void Cmount_playerView::OnForwardBig()
{
	// TODO: Add your command handler code here
	OnStopMount();
	if(par->manual_mount_flag){
		OnForwardOne();
		return;
	}
	bridge_mounter->advance_big_step();
}
void Cmount_playerView::OnBackwardOne()
{
	// TODO: Add your command handler code here
	OnStopMount();
	par->manual_mount_proportion=0;
	bridge_mounter->retreat_one_step();
}

void Cmount_playerView::OnBackwardAll()
{
	// TODO: Add your command handler code here
	OnStopMount();
	if(par->manual_mount_flag){
		OnBackwardOne();
		return;
	}
	bridge_mounter->retreat_forever();
}
void Cmount_playerView::OnBackwardBig()
{
	// TODO: Add your command handler code here
	OnStopMount();
	if(par->manual_mount_flag){
		OnBackwardOne();
		return;
	}
	bridge_mounter->retreat_big_step();
}
void Cmount_playerView::OnJumpStep()
{
	// TODO: Add your command handler code here
	int step_id,end_step_id;
	CString str;

	OnStopMount();

	step_id=((CMainFrame*)(theApp.m_pMainWnd))->get_select_step_id(&end_step_id);
	if((step_id<0)||(step_id>(bridge_mounter->move_number))){
		AfxMessageBox("请在拼装进度表中选择跳转拼装步骤");
		return;
	}
	bridge_mounter->jump_step(step_id);
	((CMainFrame*)(theApp.m_pMainWnd))->fill_tree_item();
	step_id=bridge_mounter->current_step;
	str.Format("当前拼装步骤: 第%d大步%d小步",
		bridge_mounter->move[step_id]->main_step,bridge_mounter->move[step_id]->sub_step);
	((CMainFrame*)(theApp.m_pMainWnd))->statusbar_display(str);
	need_redraw_sreen_flag=TRUE;
	SetFocus();

	return;
}

void Cmount_playerView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	int start_step_id,end_step_id,old_step_id=bridge_mounter->current_step;
	CString str;

	CView::OnTimer(nIDEvent);
	
	static BOOL flag=TRUE;

	if(flag){	
		flag=FALSE;
		sound_player.Create(mount_sound_player::IDD,this);
		sound_player.ShowWindow(SW_HIDE);
	}
	if(bridge_mounter->process_mount(par->change_view_flag?1000:0,&start_step_id,&end_step_id,(par->play_sound_flag)?(&sound_player):NULL,par->directory_name)>0){
		((CMainFrame*)(theApp.m_pMainWnd))->update_display(NULL,NULL,end_step_id);
		((CMainFrame*)(theApp.m_pMainWnd))->display_mount_information(start_step_id,end_step_id,FALSE);
		for(;start_step_id<=end_step_id;start_step_id++)
			((CMainFrame*)(theApp.m_pMainWnd))->modify_image(bridge_mounter->move[start_step_id],(bridge_mounter->current_step<old_step_id)?TRUE:FALSE);

		if(par->change_view_flag){
			CPaintDC dc(this);
			bridge_gs->begin_display(&dc);
			bridge_view->save_view_data();
			if(par->scaleup_view_flag)
				bridge_view->put_box_in_view(bridge_gs,bridge_mounter->move_parts_box,0.4);
			else
				bridge_view->aim_at_box(bridge_mounter->move_parts_box);
				
			bridge_view->set_modifier();
			bridge_gs->end_display(&dc,FALSE);
		}
		need_redraw_sreen_flag=TRUE;
	}
	
	if(need_redraw_sreen_flag){
		need_redraw_sreen_flag=FALSE;
		InvalidateRect(NULL,FALSE);
	}
	
	return;
}

void Cmount_playerView::OnModifyLight()
{
	// TODO: Add your command handler code here
	static mount_light_dialog *dlg=NULL;

	if(dlg==NULL){
		dlg=new mount_light_dialog(this);
		dlg->Create(mount_light_dialog::IDD,this);
	}
	dlg->ShowWindow(SW_SHOW);
}

void Cmount_playerView::OnPlaySound()
{
	// TODO: Add your command handler code here
	par->play_sound_flag=par->play_sound_flag?FALSE:TRUE;
	if(!(par->play_sound_flag))
		sound_player.sound_close();
}

void Cmount_playerView::OnUpdatePlaySound(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(par->play_sound_flag);
}

void Cmount_playerView::OnChangeView()
{
	// TODO: Add your command handler code here
	par->change_view_flag=par->change_view_flag?FALSE:TRUE;
}

void Cmount_playerView::OnUpdateChangeView(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(par->change_view_flag);
}

void Cmount_playerView::OnScaleupView()
{
	// TODO: Add your command handler code here
	par->scaleup_view_flag=par->scaleup_view_flag?FALSE:TRUE;
}

void Cmount_playerView::OnUpdateScaleupView(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(par->scaleup_view_flag);
}

void Cmount_playerView::OnVirtualMount()
{
	// TODO: Add your command handler code here
	bridge_mounter->stop_mount();
	par->manual_mount_flag=TRUE;
	par->manual_mount_proportion=0;
	

	static virtual_mount_dialog *dlg=NULL;
	if(dlg==NULL){
		dlg=new virtual_mount_dialog(this);
		dlg->Create(virtual_mount_dialog::IDD,this);
	}
	dlg->manual_mount_state_flag=1;
	dlg->ShowWindow(SW_SHOW);
}

void Cmount_playerView::OnDesignInput()
{
	// TODO: Add your command handler code here
	int i;
	part_tree *t;
	BOOL save_to_cache_flag,last_point_flag;
	movement m;
	
	OnStopMount();

	if((t=bridge_view->bridge->tree->get_last_mark_node(FALSE))==NULL)
		return;
	save_to_cache_flag=FALSE;
	last_point_flag=FALSE;

	for(i=1;(i<MAX_MOVEMENT_POINT)&&(!last_point_flag);i++){
		double a[16];
		coordinate_computation c;
		mount_location_dialog  dlg(this);
		if(dlg.DoModal()!=IDOK)
			return;
		save_to_cache_flag=dlg.save_to_cache_flag;
		last_point_flag=dlg.last_point_flag;
		
		c.get_array(a,dlg.dx,dlg.dy,dlg.dz,dlg.rx,dlg.ry,dlg.rz);
		m.add_move_point(t->name,"没有拼装信息","NO_SOUND",dlg.main_step,dlg.sub_step);
		m.replace_move_point(m.step_number-1,dlg.time_length,a);

		bridge_mounter->cache_main_step=m.main_step;
		bridge_mounter->cache_sub_step=m.sub_step;
	}
	if((save_to_cache_flag)&&(m.step_number>0)){
		movement_label_dialog  label_dlg(this);
		if(label_dlg.DoModal()==IDOK){
			movement_cache m_c;	
			m_c.label_name=label_dlg.label_name;
			m_c.tree_file_name=t->tree_file_name;
			m_c.tree_file_name=m_c.tree_file_name.Right(m_c.tree_file_name.GetLength()-bridge_view->bridge->directory_name_length);
			m_c.cache_type=1;
			m_c.insert_movement(&m);
			m_c.save_movement_to_cache(par->directory_name+"..\\cache.lfy");
		}
	}
	movement *new_m=new movement;
	new_m->copy_from(&m);
	bridge_mounter->add_movement(new_m);
	((CMainFrame*)(theApp.m_pMainWnd))->fill_tree_item();
	need_redraw_sreen_flag=TRUE;

	return;
}

void Cmount_playerView::OnDesignDelete()
{
	// TODO: Add your command handler code here

	int step_id,end_step_id,main_step_id,sub_step_id;

	step_id=((CMainFrame*)(theApp.m_pMainWnd))->get_select_step_id(&end_step_id);
	if((step_id<0)||(step_id>=(bridge_mounter->move_number))){
		AfxMessageBox("请在拼装进度表中选择跳转拼装步骤");
		return;
	}

	OnStopMount();

	main_step_id=bridge_mounter->move[step_id]->main_step;
	
	if((bridge_mounter->move[step_id]->sub_step)!=(bridge_mounter->move[end_step_id]->sub_step))
		sub_step_id=-1;
	else
		sub_step_id=bridge_mounter->move[step_id]->sub_step;

	bridge_mounter->delete_movement(main_step_id,sub_step_id);
	((CMainFrame*)(theApp.m_pMainWnd))->fill_tree_item();
	need_redraw_sreen_flag=TRUE;
	
	return;
}
void Cmount_playerView::OnDesignInputMessage()
{
	// TODO: Add your command handler code here

	int step_id,end_step_id;
	subtitle_input_dialog dlg(this);
	
	OnStopMount();

	step_id=((CMainFrame*)(theApp.m_pMainWnd))->get_select_step_id(&end_step_id);
	if((step_id<0)||(step_id>=(bridge_mounter->move_number))){
		AfxMessageBox("请在拼装进度表中选择跳转拼装步骤");
		return;
	}
	dlg.subtitle=bridge_mounter->move[step_id]->message;	
	if(dlg.DoModal()==IDOK)
		if(!(dlg.subtitle.IsEmpty()))
			bridge_mounter->replace_message(bridge_mounter->move[step_id]->main_step,dlg.subtitle);
	return;
}

void Cmount_playerView::OnDesignMainSort()
{
	// TODO: Add your command handler code here
	OnStopMount();

	input_main_sub_step_dialog  dlg(this);

	if(dlg.DoModal()!=IDOK)
		return;

	bridge_mounter->main_step_re_sort(dlg.main_step_id,dlg.sub_step_id);
	((CMainFrame*)(theApp.m_pMainWnd))->fill_tree_item();

	return;
}

void Cmount_playerView::OnDesignSubSort()
{
	// TODO: Add your command handler code here

	OnStopMount();
	input_main_sub_step_dialog  dlg(this);

	if(dlg.DoModal()!=IDOK)
		return;

	bridge_mounter->sub_step_re_sort(dlg.main_step_id,dlg.sub_step_id);
	((CMainFrame*)(theApp.m_pMainWnd))->fill_tree_item();

	return;
}

void Cmount_playerView::OnSaveMovement()
{
	// TODO: Add your command handler code here
	bridge_mounter->flush_movement(bridge_mounter->bak_movement_file_name);
}

void Cmount_playerView::OnRestoreMovement()
{
	// TODO: Add your command handler code here
	OnStopMount();
	bridge_mounter->load_movement(bridge_mounter->bak_movement_file_name);	
	((CMainFrame*)(theApp.m_pMainWnd))->fill_tree_item();
	need_redraw_sreen_flag=TRUE;
}

void Cmount_playerView::OnMoveMovement()
{
	// TODO: Add your command handler code here

	int target_main_step,target_sub_step;
	input_main_sub_step_dialog   dlg(this);

	if((target_main_step=((CMainFrame*)(theApp.m_pMainWnd))->get_select_step_id(&target_sub_step))<0)
		return;
	if(target_main_step>=(bridge_mounter->move_number))
		return;
	target_main_step=bridge_mounter->move[target_main_step]->main_step;
	
	OnStopMount();

	if(dlg.DoModal()!=IDOK)
		return;
	
	bridge_mounter->move_movement(dlg.main_step_id,dlg.sub_step_id,target_main_step);
	((CMainFrame*)(theApp.m_pMainWnd))->fill_tree_item();
	need_redraw_sreen_flag=TRUE;	

	return;
}

void Cmount_playerView::OnCombineStep()
{
	// TODO: Add your command handler code here
	OnStopMount();
	input_main_sub_step_dialog  dlg(this);
	if(dlg.DoModal()!=IDOK)
		return;
	bridge_mounter->combine_step(dlg.main_step_id,dlg.sub_step_id);
	((CMainFrame*)(theApp.m_pMainWnd))->fill_tree_item();
}

void Cmount_playerView::OnSeparateStep()
{
	// TODO: Add your command handler code here

	int target_step,end_step;
	
	if((target_step=((CMainFrame*)(theApp.m_pMainWnd))->get_select_step_id(&end_step))<0)
		return;
	if(target_step>=(bridge_mounter->move_number))
		return;
	
	OnStopMount();
	
	bridge_mounter->separate_step(bridge_mounter->move[target_step]->main_step,bridge_mounter->move[target_step]->sub_step);
	((CMainFrame*)(theApp.m_pMainWnd))->fill_tree_item();

	return;
}
void Cmount_playerView::OnMountProgressTable()
{
	// TODO: Add your command handler code here
	CString file_name=_T(".\\subtitle.txt");
	int return_value;
	switch((return_value=AfxMessageBox("仅仅生成播音文件?",MB_YESNOCANCEL))){
	case IDYES:
	case IDNO:
		bridge_mounter->mount_progress_table(file_name,return_value!=IDYES);
		::ShellExecute(NULL,_T("open"),file_name,NULL,NULL,SW_SHOWMAXIMIZED);	
		break;
	}
}

static BOOL cache_insert_or_delete_flag=TRUE;

void Cmount_playerView::OnCacheInsertOrDelete()
{
	// TODO: Add your command handler code here
	cache_insert_or_delete_flag=cache_insert_or_delete_flag?FALSE:TRUE;
}

void Cmount_playerView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	OnStopMount();

	CMenu popMenu;
	if(popMenu.LoadMenu(IDR_RBUTTON_MENU)==0){
		AfxMessageBox("装入菜单资源失败");
		return;
	}
	CMenu * pPopup = popMenu.GetSubMenu(0);
	if(pPopup == NULL){
		AfxMessageBox("提取菜单资源的子菜单失败");
		return;
	}

	int body_id,face_id,triangle_id;
	BOOL xyz_flag;
	double x,y,z;
	CString str;

	CPaintDC my_dc(this);
	part_tree *t;
	CPoint p(point);

	ScreenToClient(&p);

	bridge_gs->begin_display(&my_dc);	

	bridge_view->bridge->tree->clear_mark_flag();

	t=bridge_view->get_tree_body_face_triangle_and_xyz(0,&p,
			&body_id,&face_id,&triangle_id,&xyz_flag,&x,&y,&z,bridge_gs,
			bridge_view->speed,	par->manual_mount_flag?par->manual_mount_proportion:-1);
		
	bridge_gs->end_display(&my_dc,FALSE);

	if(t!=NULL){
		movement_cache m_c;
		t->set_mark_flag();
		((CMainFrame*)(theApp.m_pMainWnd))->update_display(t,t,t->get_min_step());
		((CMainFrame*)(theApp.m_pMainWnd))->display_part_information(t);

		m_c.add_movement_cache_label_to_menu(pPopup,bridge_mounter->cache_main_step,bridge_mounter->cache_sub_step,
			bridge_view->bridge->directory_name_length,par->directory_name+"..\\cache.lfy",t,BEGIN_RIGHT_BUTTON_COMMOND_ID,cache_insert_or_delete_flag);
	}

	SetFocus();
	need_redraw_sreen_flag=TRUE;

	pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON, point.x, point.y,this);

	return;
}
void Cmount_playerView::OnMainStepDecrease()
{
	// TODO: Add your command handler code here
	bridge_mounter->cache_main_step--;
	need_redraw_sreen_flag=TRUE;
}

void Cmount_playerView::OnMainStepIncrease()
{
	// TODO: Add your command handler code here
	bridge_mounter->cache_main_step++;
	need_redraw_sreen_flag=TRUE;
}

void Cmount_playerView::OnSubStepDecrease()
{
	// TODO: Add your command handler code here
	bridge_mounter->cache_sub_step--;
	need_redraw_sreen_flag=TRUE;
}

void Cmount_playerView::OnSubStepIncrease()
{
	// TODO: Add your command handler code here
	bridge_mounter->cache_sub_step++;
	need_redraw_sreen_flag=TRUE;
}

void Cmount_playerView::OnCurrentMountStep()
{
	// TODO: Add your command handler code here

	int step_id,end_step_id;
	
	OnStopMount();

	step_id=((CMainFrame*)(theApp.m_pMainWnd))->get_select_step_id(&end_step_id);
	if((step_id<0)||(step_id>=(bridge_mounter->move_number))){
		AfxMessageBox("请在拼装进度表中选择跳转拼装步骤");
		return;
	}
	if(bridge_mounter->move[step_id]==NULL)
		return;	
	bridge_mounter->cache_main_step=bridge_mounter->move[step_id]->main_step;
	bridge_mounter->cache_sub_step=bridge_mounter->move[step_id]->sub_step;
	return;
}
void Cmount_playerView::OnUpdateCacheInsertOrDelete(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetText(cache_insert_or_delete_flag?"删除缓存运动信息":"从缓存插入运动信息");
	pCmdUI->SetCheck(cache_insert_or_delete_flag);
}

void Cmount_playerView::OnCacheSubDecreaseMode()
{
	// TODO: Add your command handler code here
	bridge_mounter->modify_sub_step=-1;
	bridge_mounter->cache_sub_step--;
}

void Cmount_playerView::OnUpdateCacheSubDecreaseMode(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(bridge_mounter->modify_sub_step<0);
}

void Cmount_playerView::OnCacheSubIncreaseMode()
{
	// TODO: Add your command handler code here
	bridge_mounter->modify_sub_step=1;
	bridge_mounter->cache_sub_step++;
}

void Cmount_playerView::OnUpdateCacheSubIncreaseMode(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(bridge_mounter->modify_sub_step>0);
}

void Cmount_playerView::OnCacheSubRemainMode()
{
	// TODO: Add your command handler code here
	bridge_mounter->modify_sub_step=0;
}

void Cmount_playerView::OnUpdateCacheSubRemainMode(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(bridge_mounter->modify_sub_step==0);
}
void Cmount_playerView::OnSetManualMount()
{
	// TODO: Add your command handler code here
	par->manual_mount_flag=par->manual_mount_flag?FALSE:TRUE;
	OnStopMount();
}

void Cmount_playerView::OnUpdateSetManualMount(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(par->manual_mount_flag);
	pCmdUI->SetText(par->manual_mount_flag?"手动拼装":"自动拼装");

	return;
}

void Cmount_playerView::OnMountPartTable()
{
	// TODO: Add your command handler code here
	int begin_step_id,end_step_id;	
	double my_box[6];
	part_tree *buffer[MAX_TREE_NODE_COLLECTION_NUMBER];
	input_main_sub_step_dialog  dlg(this);

	begin_step_id=((CMainFrame*)(theApp.m_pMainWnd))->get_select_step_id(&end_step_id);

	if(begin_step_id<0){
		AfxMessageBox("请在拼装进度可停靠窗口中选择一个拼装步骤");
		return;
	}
	if(dlg.DoModal()!=IDOK)
		return;
	for(end_step_id=bridge_mounter->move_number-1;;end_step_id--){
		if(end_step_id<0)
			return;
		if((bridge_mounter->move[end_step_id]->main_step)<(dlg.main_step_id))
			break;
		if((bridge_mounter->move[end_step_id]->main_step)==(dlg.main_step_id))
			if((bridge_mounter->move[end_step_id]->sub_step)<=(dlg.sub_step_id))
				break;
	}
	((CMainFrame*)(theApp.m_pMainWnd))->display_mount_information(begin_step_id,end_step_id,FALSE);

	if((bridge_view->bridge->collect_mark_tree_nodes(MAX_TREE_NODE_COLLECTION_NUMBER,buffer,my_box))>0)
		bridge_mounter->set_box(my_box);

	need_redraw_sreen_flag=TRUE;

	return;
}
void Cmount_playerView::OnDisplayReferenceFrame()
{
	// TODO: Add your command handler code here
	need_redraw_sreen_flag=TRUE;
	bridge_view->display_reference_frame_flag=(bridge_view->display_reference_frame_flag)?FALSE:TRUE;
}
void Cmount_playerView::OnUpdateDisplayReferenceFrame(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(bridge_view->display_reference_frame_flag);
	pCmdUI->SetText(bridge_view->display_reference_frame_flag?"隐藏坐标系":"显示坐标系");
}

void Cmount_playerView::right_button_add_movement(UINT menu_id)
{
	int  id=menu_id-BEGIN_RIGHT_BUTTON_COMMOND_ID;
	movement_cache m_c;
	movement *m;
	part_tree *t;
	CString str;

	need_redraw_sreen_flag=TRUE;
	if(!cache_insert_or_delete_flag){
		m_c.delete_movement_from_cache(par->directory_name+"..\\cache.lfy",id);
		return;
	}
	if(m_c.get_movement_from_cache(par->directory_name+"..\\cache.lfy",id)<=0)
		return;

	for(t=bridge_view->bridge->tree->get_last_mark_node(FALSE);t!=NULL;t=t->parent){
		str=t->tree_file_name;
		str=str.Right(str.GetLength()-bridge_view->bridge->directory_name_length);
		if(str==(m_c.tree_file_name)){
			switch(m_c.cache_type){
			case 1:
				if((m=m_c.get_movement())!=NULL){
					m->name=t->name;
					m->main_step=bridge_mounter->cache_main_step;
					m->sub_step=bridge_mounter->cache_sub_step;
					bridge_mounter->add_movement(m);					
					bridge_mounter->cache_sub_step+=bridge_mounter->modify_sub_step;
				}
				break;
			case 2:
				{
					int old_max_move_number;

					old_max_move_number=bridge_mounter->move_number;
					for(;;){
						if(bridge_mounter->move_number>=MAX_MOVEMENT_STEP)
							break;
						if((m=m_c.get_movement())==NULL)
							break;
						m->name=t->name+"/"+m->name;
						bridge_mounter->move[bridge_mounter->move_number]=m;
						bridge_mounter->move_number++;
					}	
					if(bridge_mounter->move_number>0){
						int i,max_main_step,difference;
						max_main_step=bridge_mounter->cache_main_step;
						for(i=old_max_move_number;i<bridge_mounter->move_number;i++)
							if(max_main_step<(bridge_mounter->move[i]->main_step))
								max_main_step=bridge_mounter->move[i]->main_step;
						difference=max_main_step-(bridge_mounter->cache_main_step);
						for(i=old_max_move_number;i<bridge_mounter->move_number;i++){
							bridge_mounter->move[i]->main_step-=difference;
							if((bridge_mounter->cache_main_step)>(bridge_mounter->move[i]->main_step))
								bridge_mounter->cache_main_step=bridge_mounter->move[i]->main_step;
						}
						bridge_mounter->cache_main_step--;						
					}
				}
				bridge_mounter->current_step=0;
				bridge_mounter->target_step=0;
				bridge_mounter->sort_movement();
				bridge_mounter->initialize_visible_state_and_movement_index();
				bridge_mounter->should_flush_movement_flag=TRUE;
				break;
			default:
				break;
			};
			((CMainFrame*)(theApp.m_pMainWnd))->fill_tree_item();

			return;
		}
	}
	return;
}
void Cmount_playerView::OnTreeMovementSaveToCache()
{
	// TODO: Add your command handler code here

	int i,length;
	part_tree  *t;
	input_main_sub_step_dialog  dlg(this);
	movement_label_dialog label_dlg(this);
	CString head_str,my_str;
	movement_cache m_c;
	
	if((t=bridge_view->bridge->tree->get_last_mark_node(FALSE))==NULL)
		return;
	if(t->type==0)
		return;
	if(dlg.DoModal()!=IDOK)
		return;
	if(label_dlg.DoModal()!=IDOK)
		return;
	if(label_dlg.label_name.IsEmpty())
		return;

	head_str=t->name+"/";

	for(i=0;i<(bridge_mounter->move_number);i++){
		if(		  (bridge_mounter->move[i]->main_step>=dlg.main_step_id)
				&&(bridge_mounter->move[i]->main_step<=dlg.sub_step_id)
				&&(bridge_mounter->move[i]->step_number>0)
			)
		{
			my_str=bridge_mounter->move[i]->name;
			if((length=my_str.GetLength()-head_str.GetLength())>0)
				if(head_str==my_str.Left(head_str.GetLength())){
					movement m;
					m.copy_from(bridge_mounter->move[i]);
					m.name=my_str.Right(length);
					m_c.insert_movement(&m);
				}
		}
	}
	m_c.label_name=label_dlg.label_name;
	length =t->tree_file_name.GetLength();
	length-=bridge_view->bridge->directory_name_length;
	m_c.tree_file_name=t->tree_file_name.Right(length);
	m_c.cache_type=2;

	m_c.save_movement_to_cache(par->directory_name+"..\\cache.lfy");

	return;
}




