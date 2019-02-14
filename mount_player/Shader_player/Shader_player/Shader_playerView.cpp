
// Shader_playerView.cpp : implementation of the CShader_playerView class
//

#include "stdafx.h"
#include "Shader_player.h"

#include "Shader_playerDoc.h"
#include "Shader_playerView.h"
#include "MainFrm.h"

#include "user_interface/Location_dialog.h"
#include "user_interface/Mount_information_dialog.h"
#include "user_interface/Mount_time_length_dialog.h"
#include "user_interface/Mount_step_dialog.h"
#include "user_interface/Mount_cache_information_dialog.h"
#include "kernel/Shader_size_tag.h"
#include "kernel/Shader_coordinate.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CShader_playerView

IMPLEMENT_DYNCREATE(CShader_playerView, CView)

BEGIN_MESSAGE_MAP(CShader_playerView, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_SIZE()

	ON_WM_PAINT()
	ON_COMMAND(ID_COMPONENT_SET_FRAME_SHOW_DETAIL, &CShader_playerView::OnComponentSetFrameShowDetail)
	ON_COMMAND(ID_COMPONENT_CLEAR_FRAME_SHOW_DETAIL, &CShader_playerView::OnComponentClearFrameShowDetail)

	ON_COMMAND(ID_COMPONENT_SET_FRAME_SHOW_HIDE,	&CShader_playerView::OnComponentSetFrameShowHide)
	ON_COMMAND(ID_COMPONENT_SET_FRAME_SHOW_SHOW,	&CShader_playerView::OnComponentSetFrameShowShow)
	ON_COMMAND(ID_COMPONENT_SET_FRAME_SHOW_STIPPLE, &CShader_playerView::OnComponentSetFrameShowStipple)

	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_CONTEXTMENU()

	ON_WM_RBUTTONDOWN()


	ON_COMMAND          (ID_ADVANCE_SUB_STEP, &CShader_playerView::OnAdvanceSubStep)
	ON_UPDATE_COMMAND_UI(ID_ADVANCE_SUB_STEP, &CShader_playerView::OnEnableDisable)

	ON_COMMAND          (ID_ADVANCE_MAIN_STEP, &CShader_playerView::OnAdvanceMainStep)
	ON_UPDATE_COMMAND_UI(ID_ADVANCE_MAIN_STEP, &CShader_playerView::OnEnableDisable)

	ON_COMMAND          (ID_ADVANCE_FOREVER, &CShader_playerView::OnAdvanceForever)
	ON_UPDATE_COMMAND_UI(ID_ADVANCE_FOREVER, &CShader_playerView::OnEnableDisable)

	ON_COMMAND(ID_RETREAT_SUB_STEP, &CShader_playerView::OnRetreatSubStep)
	ON_COMMAND(ID_RETREAT_MAIN_STEP, &CShader_playerView::OnRetreatMainStep)
	ON_COMMAND(ID_RETREAT_FOREVER, &CShader_playerView::OnRetreatForever)


	ON_COMMAND(ID_PLAY_SOUND, &CShader_playerView::OnPlaySound)
	ON_UPDATE_COMMAND_UI(ID_PLAY_SOUND, &CShader_playerView::OnUpdatePlaySound)


	ON_COMMAND(ID_LOCATE_VIEW, &CShader_playerView::OnLocateView)
	ON_UPDATE_COMMAND_UI(ID_LOCATE_VIEW, &CShader_playerView::OnUpdateLocateView)

	ON_COMMAND(ID_EXPAND_VIEW, &CShader_playerView::OnExpandView)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_VIEW, &CShader_playerView::OnUpdateExpandView)


	ON_COMMAND(ID_STOP_MOUNT, &CShader_playerView::OnStopMount)

	ON_COMMAND          (ID_JUMP_MOUNT, &CShader_playerView::OnJumpMount)
	ON_UPDATE_COMMAND_UI(ID_JUMP_MOUNT, &CShader_playerView::OnEnableDisable)

	ON_COMMAND(ID_VIRTUAL_MOUNT, &CShader_playerView::OnVirtualMount)
	ON_UPDATE_COMMAND_UI(ID_VIRTUAL_MOUNT, &CShader_playerView::OnUpdateVirtualMount)

	ON_COMMAND(ID_COMPONENT_HIDE, &CShader_playerView::OnComponentHide)
	ON_COMMAND(ID_COMPONENT_SHOW, &CShader_playerView::OnComponentShow)


	ON_COMMAND(ID_COMPONENT_CLEAR_FRAME_SHOW, &CShader_playerView::OnComponentClearFrameShow)
	ON_COMMAND(ID_COMPONENT_SET_FRAME_SHOW, &CShader_playerView::OnComponentSetFrameShow)
	ON_COMMAND(ID_COMPONENT_SET_ONLY_FRAME_SHOW, &CShader_playerView::OnComponentSetOnlyFrameShow)


	ON_COMMAND(ID_SHOW_TRANSPARENT, &CShader_playerView::OnShowTransparent)
	ON_COMMAND(ID_SHOW_NOT_TRANSPARENT, &CShader_playerView::OnShowNotTransparent)
	ON_COMMAND(ID_SELECT_UP_COMPONENT, &CShader_playerView::OnSelectUpComponent)
	ON_COMMAND(ID_SELECT_DOWN_COMPONENT, &CShader_playerView::OnSelectDownComponent)
	ON_COMMAND(ID_CUT_COMPONENT_START, &CShader_playerView::OnCutComponentStart)
	ON_COMMAND(ID_CUT_COMPONENT_END, &CShader_playerView::OnCutComponentEnd)
	ON_COMMAND(ID_COMPONENT_RESET, &CShader_playerView::OnComponentReset)
	ON_COMMAND(ID_COMPONENT_LOCATION, &CShader_playerView::OnComponentLocation)
	ON_COMMAND(ID_COMPONENT_CLEAR_SELECT, &CShader_playerView::OnComponentClearSelect)
	ON_COMMAND(ID_RESTORE_LAST_VIEW, &CShader_playerView::OnRestoreLastView)

	
	ON_COMMAND(ID_MOUNT_DESIGN_START, &CShader_playerView::OnMountDesignStart)
	

	
	ON_COMMAND(ID_MOUNT_DOCUMENT, &CShader_playerView::OnMountDocument)
	ON_COMMAND(ID_MOUNT_MANUAL, &CShader_playerView::OnMountManual)
	
	ON_COMMAND(ID_MODIFY_SHADER, &CShader_playerView::OnModifyShader)
	ON_COMMAND(ID_RELOAD_MATERIAL, &CShader_playerView::OnReloadMaterial)
	ON_COMMAND(ID_RELOAD_LIGHT, &CShader_playerView::OnReloadLight)
	ON_COMMAND(ID_RELEASE_VERTEX, &CShader_playerView::OnReleaseVertex)
	ON_COMMAND(ID_EDIT_MATERIAL, &CShader_playerView::OnEditMaterial)
	

	ON_COMMAND(ID_MOUNT_SAVE_TO_CACHE, &CShader_playerView::OnMountSaveToCache)


	ON_COMMAND_RANGE(BEGIN_RIGHT_BUTTON_COMMOND_ID,END_RIGHT_BUTTON_COMMOND_ID,right_button_add_movement)

	ON_COMMAND(ID_SET_CAMERA_0, &CShader_playerView::OnSetCamera0)
	ON_COMMAND(ID_SET_CAMERA_1, &CShader_playerView::OnSetCamera1)
	ON_COMMAND(ID_SET_CAMERA_2, &CShader_playerView::OnSetCamera2)
	ON_COMMAND(ID_SET_CAMERA_3, &CShader_playerView::OnSetCamera3)
	ON_COMMAND(ID_SET_CAMERA_4, &CShader_playerView::OnSetCamera4)
	ON_COMMAND(ID_SET_CAMERA_5, &CShader_playerView::OnSetCamera5)
	ON_COMMAND(ID_SET_CAMERA_6, &CShader_playerView::OnSetCamera6)
	ON_COMMAND(ID_SET_CAMERA_7, &CShader_playerView::OnSetCamera7)
	ON_COMMAND(ID_SET_CAMERA_8, &CShader_playerView::OnSetCamera8)

	ON_UPDATE_COMMAND_UI(ID_SET_CAMERA_0, &CShader_playerView::OnUpdateSetCamera0)
	ON_UPDATE_COMMAND_UI(ID_SET_CAMERA_1, &CShader_playerView::OnUpdateSetCamera1)
	ON_UPDATE_COMMAND_UI(ID_SET_CAMERA_2, &CShader_playerView::OnUpdateSetCamera2)
	ON_UPDATE_COMMAND_UI(ID_SET_CAMERA_3, &CShader_playerView::OnUpdateSetCamera3)
	ON_UPDATE_COMMAND_UI(ID_SET_CAMERA_4, &CShader_playerView::OnUpdateSetCamera4)
	ON_UPDATE_COMMAND_UI(ID_SET_CAMERA_5, &CShader_playerView::OnUpdateSetCamera5)
	ON_UPDATE_COMMAND_UI(ID_SET_CAMERA_6, &CShader_playerView::OnUpdateSetCamera6)
	ON_UPDATE_COMMAND_UI(ID_SET_CAMERA_7, &CShader_playerView::OnUpdateSetCamera7)
	

	ON_COMMAND(ID_SET_MOUSE_WHEEL_0, &CShader_playerView::OnSetMouseWheel0)
	ON_COMMAND(ID_SET_MOUSE_WHEEL_1, &CShader_playerView::OnSetMouseWheel1)
	ON_COMMAND(ID_SET_MOUSE_WHEEL_2, &CShader_playerView::OnSetMouseWheel2)
	ON_COMMAND(ID_SET_MOUSE_WHEEL_3, &CShader_playerView::OnSetMouseWheel3)
	ON_COMMAND(ID_SET_MOUSE_WHEEL_4, &CShader_playerView::OnSetMouseWheel4)
	ON_COMMAND(ID_SET_MOUSE_WHEEL_5, &CShader_playerView::OnSetMouseWheel5)
	ON_COMMAND(ID_SET_MOUSE_WHEEL_6, &CShader_playerView::OnSetMouseWheel6)

	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_WHEEL_0, &CShader_playerView::OnUpdateSetMouseWheel0)
	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_WHEEL_1, &CShader_playerView::OnUpdateSetMouseWheel1)
	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_WHEEL_2, &CShader_playerView::OnUpdateSetMouseWheel2)
	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_WHEEL_3, &CShader_playerView::OnUpdateSetMouseWheel3)
	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_WHEEL_4, &CShader_playerView::OnUpdateSetMouseWheel4)
	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_WHEEL_5, &CShader_playerView::OnUpdateSetMouseWheel5)
	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_WHEEL_6, &CShader_playerView::OnUpdateSetMouseWheel6)

	ON_COMMAND(ID_SET_VIEW_NO_DIRECTION, &CShader_playerView::OnSetViewNoDirection)
	ON_COMMAND(ID_SET_VIEW_0, &CShader_playerView::OnSetView0)
	ON_COMMAND(ID_SET_VIEW_1, &CShader_playerView::OnSetView1)
	ON_COMMAND(ID_SET_VIEW_2, &CShader_playerView::OnSetView2)
	ON_COMMAND(ID_SET_VIEW_3, &CShader_playerView::OnSetView3)
	ON_COMMAND(ID_SET_VIEW_4, &CShader_playerView::OnSetView4)
	ON_COMMAND(ID_SET_VIEW_5, &CShader_playerView::OnSetView5)
	ON_COMMAND(ID_SET_VIEW_6, &CShader_playerView::OnSetView6)
	ON_COMMAND(ID_SET_VIEW_7, &CShader_playerView::OnSetView7)
	ON_COMMAND(ID_SET_VIEW_8, &CShader_playerView::OnSetView8)
	ON_COMMAND(ID_SET_VIEW_9, &CShader_playerView::OnSetView9)

	ON_COMMAND(ID_SET_VIEW_10, &CShader_playerView::OnSetView10)
	ON_COMMAND(ID_SET_VIEW_11, &CShader_playerView::OnSetView11)
	ON_COMMAND(ID_SET_VIEW_12, &CShader_playerView::OnSetView12)
	ON_COMMAND(ID_SET_VIEW_13, &CShader_playerView::OnSetView13)
	ON_COMMAND(ID_SET_VIEW_14, &CShader_playerView::OnSetView14)
	ON_COMMAND(ID_SET_VIEW_15, &CShader_playerView::OnSetView15)
	ON_COMMAND(ID_SET_VIEW_16, &CShader_playerView::OnSetView16)
	ON_COMMAND(ID_SET_VIEW_17, &CShader_playerView::OnSetView17)

	ON_COMMAND(ID_SET_MOUSE_MOVE_0, &CShader_playerView::OnSetMouseMove0)
	ON_COMMAND(ID_SET_MOUSE_MOVE_1, &CShader_playerView::OnSetMouseMove1)
	ON_COMMAND(ID_SET_MOUSE_MOVE_2, &CShader_playerView::OnSetMouseMove2)
	ON_COMMAND(ID_SET_MOUSE_MOVE_3, &CShader_playerView::OnSetMouseMove3)
	ON_COMMAND(ID_SET_MOUSE_MOVE_4, &CShader_playerView::OnSetMouseMove4)
	ON_COMMAND(ID_SET_MOUSE_MOVE_5, &CShader_playerView::OnSetMouseMove5)
	ON_COMMAND(ID_SET_MOUSE_MOVE_6, &CShader_playerView::OnSetMouseMove6)
	ON_COMMAND(ID_SET_MOUSE_MOVE_7, &CShader_playerView::OnSetMouseMove7)
	ON_COMMAND(ID_SET_MOUSE_MOVE_8, &CShader_playerView::OnSetMouseMove8)
	ON_COMMAND(ID_SET_MOUSE_MOVE_9, &CShader_playerView::OnSetMouseMove9)
	ON_COMMAND(ID_SET_MOUSE_MOVE_10, &CShader_playerView::OnSetMouseMove10)
	ON_COMMAND(ID_SET_MOUSE_MOVE_11, &CShader_playerView::OnSetMouseMove11)
	ON_COMMAND(ID_SET_MOUSE_MOVE_12, &CShader_playerView::OnSetMouseMove12)

	ON_COMMAND(ID_SET_MOUSE_MOVE_21, &CShader_playerView::OnSetMouseMove21)
	ON_COMMAND(ID_SET_MOUSE_MOVE_22, &CShader_playerView::OnSetMouseMove22)
	ON_COMMAND(ID_SET_MOUSE_MOVE_23, &CShader_playerView::OnSetMouseMove23)
	ON_COMMAND(ID_SET_MOUSE_MOVE_24, &CShader_playerView::OnSetMouseMove24)
	ON_COMMAND(ID_SET_MOUSE_MOVE_40, &CShader_playerView::OnSetMouseMove40)

		
	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_MOVE_0, &CShader_playerView::OnUpdateSetMouseMove0)
	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_MOVE_1, &CShader_playerView::OnUpdateSetMouseMove1)
	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_MOVE_2, &CShader_playerView::OnUpdateSetMouseMove2)
	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_MOVE_3, &CShader_playerView::OnUpdateSetMouseMove3)
	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_MOVE_4, &CShader_playerView::OnUpdateSetMouseMove4)
	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_MOVE_5, &CShader_playerView::OnUpdateSetMouseMove5)
	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_MOVE_6, &CShader_playerView::OnUpdateSetMouseMove6)
	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_MOVE_7, &CShader_playerView::OnUpdateSetMouseMove7)
	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_MOVE_8, &CShader_playerView::OnUpdateSetMouseMove8)
	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_MOVE_9, &CShader_playerView::OnUpdateSetMouseMove9)
	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_MOVE_10, &CShader_playerView::OnUpdateSetMouseMove10)
	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_MOVE_11, &CShader_playerView::OnUpdateSetMouseMove11)
	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_MOVE_12, &CShader_playerView::OnUpdateSetMouseMove12)

	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_MOVE_21, &CShader_playerView::OnUpdateSetMouseMove21)
	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_MOVE_22, &CShader_playerView::OnUpdateSetMouseMove22)
	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_MOVE_23, &CShader_playerView::OnUpdateSetMouseMove23)
	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_MOVE_24, &CShader_playerView::OnUpdateSetMouseMove24)
	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_MOVE_25, &CShader_playerView::OnUpdateSetMouseMove25)
	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_MOVE_26, &CShader_playerView::OnUpdateSetMouseMove26)
	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_MOVE_27, &CShader_playerView::OnUpdateSetMouseMove27)
	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_MOVE_28, &CShader_playerView::OnUpdateSetMouseMove28)
	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_MOVE_29, &CShader_playerView::OnUpdateSetMouseMove29)
	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_MOVE_30, &CShader_playerView::OnUpdateSetMouseMove30)

	ON_UPDATE_COMMAND_UI(ID_SET_MOUSE_MOVE_40, &CShader_playerView::OnUpdateSetMouseMove40)

	ON_COMMAND(ID_CLEAR_ALL_SIZE_TAG, &CShader_playerView::OnClearAllSizeTag)

	ON_COMMAND(ID_CREATE_ASSEMBLE_PART, &CShader_playerView::OnCreateAssemblePart)

	ON_COMMAND(ID_TURN_ONFF_COORDINATOR, &CShader_playerView::OnTurnOnffCoordinator)
	ON_UPDATE_COMMAND_UI(ID_TURN_ONFF_COORDINATOR, &CShader_playerView::OnUpdateTurnOnffCoordinator)
	
	ON_COMMAND(ID_SET_BLACK_COLOR, &CShader_playerView::OnSetBlackColor)
	

	
	ON_COMMAND(ID_PROJECTION_MATRIX_TYPE, &CShader_playerView::OnProjectionMatrixType)
	ON_UPDATE_COMMAND_UI(ID_PROJECTION_MATRIX_TYPE, &CShader_playerView::OnUpdateProjectionMatrixType)
	ON_COMMAND(ID_MEMORY_RELEASE_MAP, &CShader_playerView::OnMemoryReleaseMap)
	ON_UPDATE_COMMAND_UI(ID_MEMORY_RELEASE_MAP, &CShader_playerView::OnUpdateMemoryReleaseMap)

	ON_COMMAND(ID_BOX_FUNCTION, &CShader_playerView::OnBoxFunction)
	ON_UPDATE_COMMAND_UI(ID_BOX_FUNCTION, &CShader_playerView::OnUpdateBoxFunction)
	ON_COMMAND(ID_LOCK_VIEW_IN_BOX, &CShader_playerView::OnLockViewInBox)
	ON_UPDATE_COMMAND_UI(ID_LOCK_VIEW_IN_BOX, &CShader_playerView::OnUpdateLockViewInBox)
	ON_COMMAND(ID_VIEW_ROTATE_TYPE, &CShader_playerView::OnViewRotateType)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ROTATE_TYPE, &CShader_playerView::OnUpdateViewRotateType)
	ON_COMMAND(ID_ROTATE_VIEW_X_45, &CShader_playerView::OnRotateViewX45)
	ON_COMMAND(ID_ROTATE_VIEW_X_90, &CShader_playerView::OnRotateViewX90)
	ON_COMMAND(ID_ROTATE_VIEW_X_180, &CShader_playerView::OnRotateViewX180)
	ON_COMMAND(ID_ROTATE_VIEW_Y_45, &CShader_playerView::OnRotateViewY45)
	ON_COMMAND(ID_ROTATE_VIEW_Y_90, &CShader_playerView::OnRotateViewY90)
	ON_COMMAND(ID_ROTATE_VIEW_Y_180, &CShader_playerView::OnRotateViewY180)
	ON_COMMAND(ID_ROTATE_VIEW_Z_45, &CShader_playerView::OnRotateViewZ45)
	ON_COMMAND(ID_ROTATE_VIEW_Z_90, &CShader_playerView::OnRotateViewZ90)
	ON_COMMAND(ID_ROTATE_VIEW_Z_180, &CShader_playerView::OnRotateViewZ180)
	ON_COMMAND(ID_ROTATE_VIEW_DIRECTION, &CShader_playerView::OnRotateViewDirection)
	ON_UPDATE_COMMAND_UI(ID_ROTATE_VIEW_DIRECTION, &CShader_playerView::OnUpdateRotateViewDirection)
	ON_COMMAND(ID_SET_TAG_TYPE_SPACE_DISTANCE, &CShader_playerView::OnSetTagTypeSpaceDistance)
	ON_UPDATE_COMMAND_UI(ID_SET_TAG_TYPE_SPACE_DISTANCE, &CShader_playerView::OnUpdateSetTagTypeSpaceDistance)
	ON_COMMAND(ID_SET_TAG_TYPE_X, &CShader_playerView::OnSetTagTypeX)
	ON_UPDATE_COMMAND_UI(ID_SET_TAG_TYPE_X, &CShader_playerView::OnUpdateSetTagTypeX)
	ON_COMMAND(ID_SET_TAG_TYPE_Y, &CShader_playerView::OnSetTagTypeY)
	ON_UPDATE_COMMAND_UI(ID_SET_TAG_TYPE_Y, &CShader_playerView::OnUpdateSetTagTypeY)
	ON_COMMAND(ID_SET_TAG_TYPE_Z, &CShader_playerView::OnSetTagTypeZ)
	ON_UPDATE_COMMAND_UI(ID_SET_TAG_TYPE_Z, &CShader_playerView::OnUpdateSetTagTypeZ)
	ON_COMMAND(ID_SET_TAG_TYPE_LEFT_RIGHT, &CShader_playerView::OnSetTagTypeLeftRight)
	ON_UPDATE_COMMAND_UI(ID_SET_TAG_TYPE_LEFT_RIGHT, &CShader_playerView::OnUpdateSetTagTypeLeftRight)
	ON_COMMAND(ID_SET_TAG_TYPE_TOP_DOWN, &CShader_playerView::OnSetTagTypeTopDown)
	ON_UPDATE_COMMAND_UI(ID_SET_TAG_TYPE_TOP_DOWN, &CShader_playerView::OnUpdateSetTagTypeTopDown)
	ON_COMMAND(ID_SET_TAG_TYPE_VIEW_PLANE, &CShader_playerView::OnSetTagTypeViewPlane)
	ON_UPDATE_COMMAND_UI(ID_SET_TAG_TYPE_VIEW_PLANE, &CShader_playerView::OnUpdateSetTagTypeViewPlane)
	ON_WM_KEYDOWN()

	ON_COMMAND(ID_BACKUP_DATA, &CShader_playerView::OnBackupData)
	
	ON_COMMAND(ID_RESTORE_DATA, &CShader_playerView::OnRestoreData)
	ON_UPDATE_COMMAND_UI(ID_RESTORE_DATA, &CShader_playerView::OnEnableDisable)

	ON_COMMAND(ID_BACKUP_CURRENT_DATA, &CShader_playerView::OnBackupCurrentData)
	ON_COMMAND(ID_DELETE_DATA, &CShader_playerView::OnDeleteData)
	END_MESSAGE_MAP()

// CShader_playerView construction/destruction

CShader_playerView::CShader_playerView()
{
	// TODO: add construction code here

}

CShader_playerView::~CShader_playerView()
{
}


void CShader_playerView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

// CShader_playerView diagnostics

#ifdef _DEBUG
void CShader_playerView::AssertValid() const
{
	CView::AssertValid();
}

void CShader_playerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CShader_playerDoc* CShader_playerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CShader_playerDoc)));
	return (CShader_playerDoc*)m_pDocument;
}
#endif //_DEBUG


BOOL CShader_playerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style|=WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	return CView::PreCreateWindow(cs);
}
// CShader_playerView message handlers

int CShader_playerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	camera_id=2;
	theApp.redraw_flag=true;

	gs=new Graphics_system(CPaintDC(this),(theApp.ws->system_max_user_parameter_number),theApp.ws->plat_info->directory_name);
	if(gs->graphics_system_flag){
		SetTimer(1,40,NULL);
		return 0;
	}
	delete gs;
	gs=NULL;
	return -1;
}

void CShader_playerView::OnDestroy()
{
	if(gs!=NULL)
		delete gs;
	KillTimer(1);
	CView::OnDestroy();

	// TODO: Add your message handler code here
}
void CShader_playerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	User_interface a(*(theApp.ws),*gs,CPaintDC(this),camera_id);
	
	a.delete_graphics_system_object();
	Shader_base::delete_all_program_object();

	theApp.redraw_flag=true;
		
	// TODO: Add your message handler code here
}
void CShader_playerView::OnDraw(CDC* pDC)
{
	CShader_playerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}
void CShader_playerView::OnTimer(UINT_PTR nIDEvent)
{
	if((gs!=NULL)&&(theApp.mounter!=NULL)){
		static int flag=0;

		{
			User_interface a(*(theApp.ws),*gs,CPaintDC(this),camera_id);
			theApp.redraw_flag|=theApp.mounter->do_movement(theApp.time_length);		
			theApp.redraw_flag|=a.camera_pointer()->process_restore_record(*(theApp.ws->parts))>(Platform_information::get_current_time()-100);
		}
		if(theApp.redraw_flag)
			flag=0;
		else{
			flag++;
			if(flag>10000)
				flag=10;
		}
		if(theApp.redraw_flag||flag<5){			
			theApp.redraw_flag=false;
			POSITION cur_pos=GetDocument()->GetFirstViewPosition();
			for(;cur_pos!=NULL;){
				CView *vp=GetDocument()->GetNextView(cur_pos);
				vp->InvalidateRect(NULL,FALSE);
			}
		}
	}
	CView::OnTimer(nIDEvent);
}

void CShader_playerView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CMainFrame*  pFrame=(CMainFrame*)(theApp.m_pMainWnd);
	if(pFrame==NULL)
		return;

	theApp.redraw_flag=true;
	theApp.mouse_move_point=point;
	
	User_interface::select_component_from_screen(*(theApp.ws),*gs,CPaintDC(this),camera_id,
						(double)(theApp.mouse_move_point.x),(double)(theApp.mouse_move_point.y));
	if(theApp.ws->select_component_ids.has_box_flag)
		User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id).camera_pointer()->locate_view(true,theApp.ws->select_component_ids.box,2.0,theApp.time_length,*(theApp.ws->parts));

	CView::OnLButtonDblClk(nFlags, point);
}
void CShader_playerView::test_match_face(CMainFrame*  pFrame,CPoint point)
{
	User_interface::select_component_from_screen(*(theApp.ws),*gs,CPaintDC(this),camera_id,(double)(point.x),(double)(point.y));
	theApp.ws->select_component_ids.compute_pointer(theApp.ws->parts);
	if(theApp.ws->select_component_ids.comp!=NULL){
		HTREEITEM match_list_id;
		pFrame->m_wndStatusBar.SetWindowText(
			(theApp.mounter->match_test(theApp.time_length,theApp.ws->select_component_ids.comp->name,
			theApp.ws->select_component_ids.body_id,theApp.ws->select_component_ids.face_id,match_list_id))
			?_T("成功配合"):_T("配合失败"));
		if(match_list_id!=NULL)
			pFrame->m_movement_list.list.SelectItem(match_list_id);

	}
}
static int tag_type_id=0;

void CShader_playerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	int mouse_move_id=(Platform_information::get_function_key_state()>=4)?0:(theApp.mouse_move_id);

	CMainFrame*  pFrame=(CMainFrame*)(theApp.m_pMainWnd);
	if(pFrame==NULL)
		return;

	theApp.redraw_flag=true;

	theApp.mouse_left_key_down_flag=true;
	theApp.mouse_left_key_down_point=point;
	theApp.last_point=point;
	theApp.mouse_move_point=point;

	theApp.touch_time=::GetTickCount();
	
	User_interface::select_component_from_screen(*(theApp.ws),*gs,CPaintDC(this),camera_id,(double)(point.x),(double)(point.y));

	switch(mouse_move_id){
	default:
			if((mouse_move_id>=0)&&(mouse_move_id<=20)){
				CPaintDC dc(this);
				User_interface a(*(theApp.ws),*gs,dc,camera_id);
				a.camera_pointer()->push_restore_record(*(a.ws->parts),theApp.time_length,false);
			}
			break;
	case 23:
			Shader_size_tag::delete_tag_object(theApp.ws->select_component_ids,*(theApp.ws->parts));
			break;
	case 24:
			Shader_size_tag::add_tag_object(theApp.ws->select_component_ids,*(theApp.ws->parts),tag_type_id);
			break;
	case 40:
			theApp.ws->match_component_ids.copy_from(theApp.ws->select_component_ids);
			theApp.ws->match_component_ids.compute_pointer(theApp.ws->parts);
			pFrame->m_movement_list.add_match();
			break;
	}	
	CView::OnLButtonDown(nFlags, point);
}
void CShader_playerView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	int mouse_move_id=(Platform_information::get_function_key_state()>=4)?0:(theApp.mouse_move_id);

	theApp.redraw_flag=true;
	theApp.mouse_move_point=point;
	theApp.last_point=point;
	
	if(theApp.mouse_left_key_down_flag){
		theApp.mouse_left_key_down_flag=false;
	
		switch(mouse_move_id){
		case 21:
			{
				User_interface::select_component_from_screen(*(theApp.ws),*gs,CPaintDC(this),camera_id,
					(double)(theApp.mouse_move_point.x+theApp.mouse_left_key_down_point.x)/2,
					(double)(theApp.mouse_move_point.y+theApp.mouse_left_key_down_point.y)/2);

				CPaintDC dc(this);
				User_interface a(*(theApp.ws),*gs,dc,camera_id);

				a.enlarge_selected_area(
					theApp.mouse_left_key_down_point.x,	theApp.mouse_left_key_down_point.y,
					theApp.mouse_move_point.x,			theApp.mouse_move_point.y,
					theApp.time_length);
			}
			break;
		case 22:
			{
				CPaintDC dc(this);
				User_interface a(*(theApp.ws),*gs,dc,camera_id);

				CMainFrame*  pFrame=(CMainFrame*)(theApp.m_pMainWnd);
				if(pFrame!=NULL){
					a.area_select_component(1<<camera_id,theApp.mouse_left_key_down_point.x,theApp.mouse_left_key_down_point.y,theApp.mouse_move_point.x,theApp.mouse_move_point.y);
					pFrame->m_property_list.display_part_collector(Part_collector(Component_collector_for_selected_component(a.ws->root_component),a.ws->parts));
					theApp.redraw_flag=true;
				}
			}
			break;
		}
	}
	CView::OnLButtonUp(nFlags, point);
}

void CShader_playerView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	int key_id=Platform_information::get_function_key_state();
	int mouse_move_id=(key_id>=4)?0:(theApp.mouse_move_id);

	theApp.redraw_flag=true;
	theApp.mouse_move_point=point;
		
	if((theApp.mouse_left_key_down_flag)&&(mouse_move_id>=0)&&(mouse_move_id<=20)){
		CPaintDC dc(this);
		User_interface a(*(theApp.ws),*gs,dc,camera_id);

		if(point!=theApp.last_point)
			switch(key_id%4){
			case 0:
				a.rotate_view(mouse_move_id,theApp.last_point.x,theApp.last_point.y,point.x,point.y);
				theApp.last_point=point;
				break;
			case 1:
				{
					int index_id=mouse_move_id;
					Point direction[]={Point(1,1,0),Point(1,0,0),Point(0,1,0)};
					if((index_id<0)||(index_id>=(sizeof(direction)/sizeof(direction[0]))))
						break;
					a.translate_view(theApp.last_point.x,theApp.last_point.y,point.x,point.y,direction[index_id]);
					theApp.last_point=point;
				}
				break;
			case 2:
				a.rotate_component(mouse_move_id,theApp.last_point.x,theApp.last_point.y,point.x,point.y);
				theApp.last_point=point;
				break;
			case 3:
				{
					int index_id=mouse_move_id;
					Point direction[][2]={
						{Point(1,1,1),Point(1,1,1)},			{Point(1,1,1),Point(1,0,0)},			{Point(1,1,1),Point(0,1,0)},//0,1,2
						{Point(1,1,1),Point(0,0,1)},			{Point(1,1,1),Point(0,1,1)},			{Point(1,1,1),Point(1,0,1)},//3,4,5
						{Point(1,1,1),Point(1,1,0)},			{Point(1,0,0),Point(1,1,1)},			{Point(0,1,0),Point(1,1,1)},//6,7,8
						{Point(0,0,1),Point(1,1,1)},			{Point(0,1,1),Point(1,1,1)},			{Point(1,0,1),Point(1,1,1)},//9,10,11
						{Point(1,1,0),Point(1,1,1)},//12
					};
					if((index_id<0)||(index_id>=(sizeof(direction)/sizeof(direction[0]))))
						break;
					a.translate_component(theApp.last_point.x,theApp.last_point.y,point.x,point.y,direction[index_id][0],direction[index_id][1]);
					theApp.last_point=point;
				}
				break;
			default:
				break;
			}
	}
	CView::OnMouseMove(nFlags, point);
}

BOOL CShader_playerView::OnMouseWheel(UINT nFlags, short zDelta, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	int key_id=Platform_information::get_function_key_state();
	int mouse_wheel_id=(key_id>=4)?0:(theApp.mouse_wheel_id);

	theApp.redraw_flag=true;
	theApp.mouse_move_point=point;
	theApp.touch_time=::GetTickCount();

	CMainFrame*  pFrame=(CMainFrame*)(theApp.m_pMainWnd);
	if(pFrame!=NULL){
		CString str;
		switch(mouse_wheel_id){
		case 0:
		case 1:
			{
				CPoint pp(point);
				ScreenToClient(&pp);
				if(mouse_wheel_id!=0)
					User_interface::select_component_from_screen(*(theApp.ws),*gs,CPaintDC(this),camera_id,(double)(pp.x),(double)(pp.y));
				else
					theApp.ws->select_component_ids.reset();

				User_interface a(*(theApp.ws),*gs,CPaintDC(this),camera_id);

				str.Format(_T("视点距离:%lf"),a.adjust_mouse_distance((double)(pp.x),(double)(pp.y),expl(((double)zDelta)/1500)));				
				
				{
					static DWORD last_wheel_time=0;
					if((::GetTickCount()-last_wheel_time)>1000)
						a.camera_pointer()->push_restore_record(*(a.ws->parts),theApp.time_length,false);
					last_wheel_time=::GetTickCount();
				}

				pFrame->m_wndStatusBar.SetWindowText(str);
			}
			break;
		case 2:
			{
				User_interface a(*(theApp.ws),*gs,CPaintDC(this),camera_id);
				str.Format(_T("视角大小:%lf"),a.adjust_fovy(expl(((double)zDelta)/9000)));
				
				{
					static DWORD last_wheel_time=0;
					if((::GetTickCount()-last_wheel_time)>1000)
						a.camera_pointer()->push_restore_record(*(a.ws->parts),theApp.time_length,false);
					last_wheel_time=::GetTickCount();
				}

				pFrame->m_wndStatusBar.SetWindowText(str);
			}
			break;
		case 3:
			{
				User_interface a(*(theApp.ws),*gs,CPaintDC(this),camera_id);
				str.Format(_T("透明度:%lf"),a.set_component_transparence(((double)zDelta)/4500,false));
				pFrame->m_wndStatusBar.SetWindowText(str);
			}
			break;
		case 4:			
			{
				User_interface a(*(theApp.ws),*gs,CPaintDC(this),camera_id);
				a.modify_clip_plane(0,((double)zDelta)/100/250);
				Camera *cam=a.camera_pointer();
				str.Format(_T("剪切面:%lf,%lf,%lf,%lf"),cam->clip_plane[0].A,cam->clip_plane[0].B,cam->clip_plane[0].C,cam->clip_plane[0].D);
				pFrame->m_wndStatusBar.SetWindowText(str);
			}
			break;
		case 5:
			{
				User_interface a(*(theApp.ws),*gs,CPaintDC(this),camera_id);
				theApp.time_length*=expl(((double)zDelta)/2000);
				str.Format(_T("时间单位:%lf毫秒"),theApp.time_length);
				pFrame->m_wndStatusBar.SetWindowText(str);
			}
			break;
		case 6:
			{
				User_interface a(*(theApp.ws),*gs,CPaintDC(this),camera_id);
				Camera *cam=a.camera_pointer();
				cam->display_precision/=expl(((double)zDelta)/2000);
				str.Format(_T("渲染精度:%8.2f, 渲染杆件:%d, 渲染三角片:%d, 渲染时间:%d"),
					(float)(cam->display_precision),cam->paint_part_number,cam->paint_triangle_number,(int)(cam->paint_time_length));
				pFrame->m_wndStatusBar.SetWindowText(str);
			}
			break;
		}		
	}
	return CView::OnMouseWheel(nFlags, zDelta, point);	
}
void CShader_playerView::OnAdvanceSubStep()
{
	theApp.mounter->advance_sub_step(theApp.time_length);
	theApp.redraw_flag=true;
}

void CShader_playerView::OnAdvanceMainStep()
{
	theApp.mounter->advance_main_step(theApp.time_length);
	theApp.redraw_flag=true;
}

void CShader_playerView::OnAdvanceForever()
{
	theApp.redraw_flag=true;
	theApp.mounter->advance_forever(theApp.time_length);
}

void CShader_playerView::OnRetreatSubStep()
{
	theApp.redraw_flag=true;
	theApp.mounter->retreat_sub_step(theApp.time_length);
}

void CShader_playerView::OnRetreatMainStep()
{
	theApp.redraw_flag=true;
	theApp.mounter->retreat_main_step(theApp.time_length);	
}

void CShader_playerView::OnRetreatForever()
{
	theApp.redraw_flag=true;
	theApp.mounter->retreat_forever(theApp.time_length);
}

void CShader_playerView::OnPlaySound()
{
	Sound_player::turn_sound_on_off();
}

void CShader_playerView::OnUpdatePlaySound(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(Sound_player::sound_state());
}

void CShader_playerView::OnLocateView()
{
	User_interface a(*(theApp.ws),*gs,CPaintDC(this),camera_id);
	a.camera_pointer()->locate_mount_flag=(a.camera_pointer()->locate_mount_flag)?false:true;			
}
void CShader_playerView::OnUpdateLocateView(CCmdUI *pCmdUI)
{
	User_interface a(*(theApp.ws),*gs,CPaintDC(this),camera_id);	
	pCmdUI->SetCheck(a.camera_pointer()->locate_mount_flag);
}
void CShader_playerView::OnExpandView()
{
	User_interface a(*(theApp.ws),*gs,CPaintDC(this),camera_id);
	a.camera_pointer()->expand_mount_flag=(a.camera_pointer()->expand_mount_flag)?false:true;
}
void CShader_playerView::OnUpdateExpandView(CCmdUI *pCmdUI)
{
	User_interface a(*(theApp.ws),*gs,CPaintDC(this),camera_id);	
	pCmdUI->SetCheck(a.camera_pointer()->expand_mount_flag);
}

void CShader_playerView::OnStopMount()
{
	theApp.mounter->stop_mount();

	CMainFrame*  pFrame;
	if((pFrame=(CMainFrame*)(theApp.m_pMainWnd))!=NULL)
		pFrame->m_movement_list.display_step();

	theApp.redraw_flag=true;
}
void CShader_playerView::OnJumpMount()
{
	CMainFrame*  pFrame=(CMainFrame*)(theApp.m_pMainWnd);
	if(pFrame==NULL)
		return;
	theApp.mounter->step_jump(pFrame->m_movement_list.list.GetSelectedItem());
	
	pFrame->m_movement_list.file_list();
	pFrame->m_movement_list.display_step();

	theApp.redraw_flag=true;
}

void CShader_playerView::OnVirtualMount()
{
	CMainFrame*  pFrame=(CMainFrame*)(theApp.m_pMainWnd);
	if(pFrame!=NULL)
		pFrame->m_property_list.OnVirtualMount();
}
void CShader_playerView::OnUpdateVirtualMount(CCmdUI *pCmdUI)
{
	CMainFrame*  pFrame=(CMainFrame*)(theApp.m_pMainWnd);
	if(pFrame!=NULL)
		pCmdUI->SetCheck(pFrame->m_movement_list.virtual_mount_flag?TRUE:FALSE);
}

void CShader_playerView::OnComponentHide()
{
	User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id). hide_component(1<<camera_id);
	theApp.redraw_flag=true;
}
void CShader_playerView::OnComponentShow()
{
	User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id).show_component(1<<camera_id);
	theApp.mounter->reset_visible_state();
	theApp.redraw_flag=true;
}
void CShader_playerView::OnComponentSetFrameShow()
{
	User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id). set_component_frame_show(false);
	theApp.redraw_flag=true;
}
void CShader_playerView::OnComponentSetOnlyFrameShow()
{
	User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id). set_component_frame_show(true);
	theApp.redraw_flag=true;
}
void CShader_playerView::OnComponentClearFrameShow()
{
	User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id). clear_component_frame_show();
	theApp.redraw_flag=true;
}

void CShader_playerView::OnShowTransparent()
{
	User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id).set_component_transparence(0.25,true);
	theApp.redraw_flag=true;
}
void CShader_playerView::OnShowNotTransparent()
{
	User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id).clear_component_transparence();
	theApp.redraw_flag=true;
}

void CShader_playerView::OnSelectUpComponent()
{
	CMainFrame*  pFrame=(CMainFrame*)(theApp.m_pMainWnd);
	if(pFrame==NULL)
		return ;
	CPaintDC dc(this);
	User_interface a(*(theApp.ws),*gs,dc,camera_id);
	a.select_parent_component();
	
	Component_collector_for_selected_component collector(a.ws->root_component);
	for(int i=0;i<collector.component_number;i++)
		pFrame->m_component_list.select_component(collector.component_pointer[i]);
	pFrame->m_property_list.display_part_collector(Part_collector(collector,a.ws->parts));
	theApp.redraw_flag=true;
}

void CShader_playerView::OnSelectDownComponent()
{
	CMainFrame*  pFrame=(CMainFrame*)(theApp.m_pMainWnd);
	if(pFrame==NULL)
		return ;
	CPaintDC dc(this);
	User_interface a(*(theApp.ws),*gs,dc,camera_id);
	a.select_child_component();
	
	Component_collector_for_selected_component collector(a.ws->root_component);
	for(int i=0;i<collector.component_number;i++)
		pFrame->m_component_list.select_component(collector.component_pointer[i]);
	pFrame->m_property_list.display_part_collector(Part_collector(collector,a.ws->parts));
	theApp.redraw_flag=true;
}

void CShader_playerView::OnCutComponentStart()
{
	User_interface a(*(theApp.ws),*gs,CPaintDC(this),camera_id);
	a.camera_pointer()->clear_clip_plane();
	a.camera_pointer()->add_clip_plane();
	theApp.redraw_flag=true;
}

void CShader_playerView::OnCutComponentEnd()
{
	User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id).camera_pointer()->clear_clip_plane();
	theApp.redraw_flag=true;
}

void CShader_playerView::OnComponentReset()
{
	User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id).reset_component();
	theApp.mounter->reset_component_location();
	theApp.redraw_flag=true;
}

void CShader_playerView::OnComponentLocation()
{
	Component *p=User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id).get_most_recent_selected_component(theApp.ws->root_component);

	if(p!=NULL){
		Location_dialog::reset();
		Location_dialog dlg(p,theApp.ws->parts,this);
		dlg.DoModal();
	}
}

void CShader_playerView::OnComponentClearSelect()
{
	theApp.ws->root_component->clear_select_flag();
	theApp.redraw_flag=true;
}

void CShader_playerView::OnRestoreLastView()
{
	User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id).camera_pointer()->pop_restore_record(*(theApp.ws->parts),theApp.time_length,true);
	theApp.redraw_flag=true;
}
void CShader_playerView::OnMountDesignStart()
{
	Component *comp;
	Movement *m;

	Component_collector_for_selected_component collector(theApp.ws->root_component);
	if(collector.component_number<=0){
			AfxMessageBox(_T("没有选中杆件，不知确定拼装杆件"));
			return;
	}
	if(collector.component_number>1){
			AfxMessageBox(_T("选中多个杆件，不知确定拼装杆件"));
			return;
	}
	comp=collector.component_pointer[0];
	
	if(theApp.mounter->move_number>=(MAX_MOVEMENT_NUMBER-1))
		return;
	if(theApp.mounter->move[theApp.mounter->move_number]!=NULL){
		delete (theApp.mounter->move[theApp.mounter->move_number]);
		theApp.mounter->move[theApp.mounter->move_number]=NULL;
	}
	m=new Movement(comp,theApp.ws);
	if(theApp.mounter->move_number>0){
		Movement *mc;
		if(theApp.mounter->current_step<=0)
			mc=theApp.mounter->move[0];
		else if(theApp.mounter->current_step>=theApp.mounter->move_number)
			mc=theApp.mounter->move[theApp.mounter->move_number-1];
		else
			mc=theApp.mounter->move[theApp.mounter->current_step];
		m->main_step	=mc->main_step;
		m->sub_step		=mc->sub_step;
		m->sound_file	=mc->sound_file;
		m->text			=mc->text;
	}

	theApp.ws->root_component->clear_select_flag();
	comp->set_component_selected();
	theApp.redraw_flag=true;

	{
		Mount_information_dialog dlg(&(m->main_step),&(m->sub_step),&(m->sound_file),&(m->text),this);
		if(dlg.DoModal()!=IDOK){
			delete m;
			return;
		}
		m->start_state_flag=(dlg.start_state_flag)?true:false;
		m->end_state_flag=(dlg.end_state_flag)?true:false;
	}

	m->name=comp->name;
	theApp.mounter->move[theApp.mounter->move_number]=m;

	static Mount_time_length_dialog dlg,*dlg_p=NULL;

	if(dlg_p==NULL){
		dlg_p=&dlg;
		dlg_p->Create(Mount_time_length_dialog::IDD);
	}
	dlg_p->ShowWindow(SW_SHOW);
	dlg_p->set_title();
	Location_dialog::reset();
	
	return;
}

void CShader_playerView::OnMountSaveToCache()
{
	CMainFrame* pFrame;
	if((pFrame=(CMainFrame*)(theApp.m_pMainWnd))==NULL)
		return;
	pFrame->m_movement_list.OnMountSaveToCache();
}

void CShader_playerView::right_button_add_movement(UINT menu_id)
{
	CMainFrame* pFrame;
	if((pFrame=(CMainFrame*)(theApp.m_pMainWnd))==NULL)
		return;

	theApp.redraw_flag=true;

	Component *comp;
	{
		CPaintDC dc(this);
		User_interface a(*(theApp.ws),*gs,dc,camera_id);
		if((comp=a.get_most_recent_selected_component(a.ws->root_component))==NULL)
			return;
	}

	int cache_state_id=Platform_information::get_function_key_state();

	switch(cache_state_id){
	default:
		return;
	case 3:
		if(AfxMessageBox(_T("确认删除"),MB_YESNO)==IDYES)
			theApp.mounter->delete_from_cache(comp,menu_id-BEGIN_RIGHT_BUTTON_COMMOND_ID);
		return;
	case 0:
	case 1:
		{
			theApp.mounter->push_movement();
			if(theApp.mounter->add_from_cache(comp,(cache_state_id==0)?true:false,menu_id-BEGIN_RIGHT_BUTTON_COMMOND_ID)){
				pFrame->m_movement_list.file_list();
				if(theApp.mounter->move_number>0){
					int current_step=theApp.mounter->current_step;
					current_step=(current_step<0)?0:(current_step>=(theApp.mounter->move_number))?(theApp.mounter->move_number-1):current_step;
					pFrame->m_movement_list.movement_list_modify_flag=false;
					pFrame->m_movement_list.list.Expand(theApp.mounter->move[current_step]->move_list_id[0],TVE_EXPAND);
					pFrame->m_movement_list.list.SelectItem(theApp.mounter->move[current_step]->move_list_id[1]);
				}
			}
		}
		return;
	}
}

void CShader_playerView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMainFrame* pFrame;
	if((pFrame=(CMainFrame*)(theApp.m_pMainWnd))==NULL)
		return;

	theApp.redraw_flag=true;

	Component *p;
	User_interface::select_component_from_screen(*(theApp.ws),*gs,CPaintDC(this),camera_id,(double)(theApp.mouse_move_point.x),(double)(theApp.mouse_move_point.y));
	
	if((p=theApp.ws->select_component_ids.comp)==NULL){
		theApp.ws->root_component->clear_select_flag();
		return;
	}
	if(pFrame->m_movement_list.virtual_mount_flag){
		ScreenToClient(&point);
		test_match_face(pFrame,point);
		return;
	}

	if(p->component_box_flag){
		CPaintDC dc(this);
		User_interface a(*(theApp.ws),*gs,dc,camera_id);
		if(a.camera_pointer()->locate_mount_flag)
			a.camera_pointer()->locate_view(true,p->component_box,2.0,theApp.time_length,*(theApp.ws->parts));
	}

	for(Component *pc=p;pc!=NULL;pc=pc->parent)
		if(pc->selected_flag){
			p=pc;
			break;
		}
	p->set_component_selected();
	pFrame->m_property_list.display_part_collector(Part_collector(p,theApp.ws->parts));
	pFrame->m_component_list.select_component(p);
	if(p->child==NULL){
		pFrame->m_movement_list.select_component(p);
		pFrame->m_part_list.select_component(p);
	}
	CMenu popMenu,*pPopup;
	if(popMenu.LoadMenu(IDR_RBUTTON_MENU)==0){
		AfxMessageBox(_T("装入菜单资源失败"));
		return;
	}
	pPopup = popMenu.GetSubMenu(0);
	if(pPopup == NULL){
		AfxMessageBox(_T("提取菜单资源的子菜单失败"));
		return;
	}
	int need_display_separator_flag=false,last_display_separator_flag=true;
	int cache_state_id=Platform_information::get_function_key_state();

	for(;p!=NULL;p=p->parent){
		int id,n;
		char buffer[MAX_BUFFER_LENGTH];
		CString id_name,title_name,str;

		ifstream f(theApp.ws->plat_info->directory_name+_T("cache.lfy"));
		for(id=0;(id<MAX_CACHE_MOVEMENT_NUMBER);id++){
			if((f.eof())||(f.fail()))
				break;
			f>>buffer;		id_name=buffer;
			f>>buffer;		title_name=buffer;
			f>>n>>n;
			
			int first_main_step=-1,last_main_step=-1;
			for(int i=0;i<n;i++){
				Movement tmp(f,NULL);
				if(i==0)
					first_main_step=tmp.main_step;
				last_main_step=tmp.main_step;
			}
			if(id_name==p->id_name){
				int current_main_step;
				if(theApp.mounter->move_number<=0)
					current_main_step=0;
				else{
					current_main_step=theApp.mounter->current_step;
					if(current_main_step>=(theApp.mounter->move_number))
						current_main_step=theApp.mounter->move_number-1;
					if(current_main_step<0)
						current_main_step=0;
					current_main_step=theApp.mounter->move[current_main_step]->main_step;
				}
				switch(cache_state_id){
				default:
					return;
				case 3:
					str=_T("删除");
					break;
				case 0:
					str.Format(_T("%d-%d"),current_main_step-1-last_main_step+first_main_step,current_main_step-1);
					break;
				case 1:
					str.Format(_T("%d-%d"),current_main_step,current_main_step+last_main_step-first_main_step);
					break;
				}

				if(last_display_separator_flag){
					pPopup->InsertMenu(0,MF_SEPARATOR|MF_BYPOSITION);
					last_display_separator_flag=false;
				}

				pPopup->InsertMenu(0,MF_STRING|MF_BYPOSITION,BEGIN_RIGHT_BUTTON_COMMOND_ID+id,str+_T(": ")+title_name);
				need_display_separator_flag=true;
			}
		}
		if(need_display_separator_flag)
			if(!last_display_separator_flag){
				last_display_separator_flag=true;
				need_display_separator_flag=false;
			}
	}
	pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON, point.x, point.y,this);
}

void CShader_playerView::OnMountDocument()
{
	::ShellExecute(NULL,_T("open"),theApp.ws->plat_info->directory_name+_T("文档资料.doc"),NULL,NULL,SW_SHOWNORMAL);
}

void CShader_playerView::OnMountManual()
{
	::ShellExecute(NULL,_T("open"),Platform_information::get_directory_name()+_T("使用手册.doc"),NULL,NULL,SW_SHOWNORMAL);	
}

void CShader_playerView::OnComponentSetFrameShowDetail()
{
	User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id).set_component_frame_show_detail(true);
	theApp.redraw_flag=true;
}

void CShader_playerView::OnComponentClearFrameShowDetail()
{
	User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id).set_component_frame_show_detail(false);
	theApp.redraw_flag=true;
}
void CShader_playerView::OnComponentSetFrameShowHide()
{
	User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id).set_component_hide_line_type(0);
	theApp.redraw_flag=true;
}
void  CShader_playerView::OnComponentSetFrameShowShow()
{
	User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id).set_component_hide_line_type(2);
	theApp.redraw_flag=true;
}
void  CShader_playerView::OnComponentSetFrameShowStipple()
{
	User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id).set_component_hide_line_type(1);
	theApp.redraw_flag=true;
}

static bool box_function_flag=true;

void CShader_playerView::OnPaint()
{
	CShader_playerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CMainFrame* pFrame;
	if((pFrame=(CMainFrame*)(theApp.m_pMainWnd))==NULL)
		return;
	DWORD start_time=::GetTickCount();
	double precision_scale=1.0;

	if(box_function_flag){
		if(theApp.mouse_left_key_down_flag)
			if(theApp.mouse_move_id<23)
				precision_scale=2*MIN_VALUE;

		switch(theApp.mouse_wheel_id){
		case 0:
		case 1:
		case 2:
			if((start_time-(theApp.touch_time))<1000)
				precision_scale=2*MIN_VALUE;
			break;
		}
	}

	if(User_interface::do_paint(precision_scale,CPaintDC(this),*(theApp.ws),*gs,camera_id)>0)
			theApp.redraw_flag=true;

	User_interface a(*(theApp.ws),*gs,CPaintDC(this),camera_id);
	int paint_part_number=a.camera_pointer()->paint_part_number;
	int paint_triangle_number=a.camera_pointer()->paint_triangle_number;

	if(theApp.ws->load_ws_part(theApp.ws->release_memory_flag_map,&(pFrame->m_wndStatusBar)))
				theApp.redraw_flag=true;
	else {
			if((theApp.mouse_move_id<24)||(theApp.mouse_move_id>30))
				theApp.ws->select_component_ids.reset();
			else{
				Component *comp=User_interface::select_component_from_screen(*(theApp.ws),*gs,CPaintDC(this),camera_id,(double)(theApp.mouse_move_point.x),(double)(theApp.mouse_move_point.y));	
				if(comp!=NULL){
					CString str;
					str.Format(_T(",渲染:%d,%d,%lf"),paint_part_number,paint_triangle_number,(double)(::GetTickCount()-start_time));
					pFrame->m_wndStatusBar.SetWindowText(theApp.ws->get_part_description()+str);
				}
			}
	}
}

void CShader_playerView::OnModifyShader()
{
	Shader_base::delete_all_program_object();
	theApp.redraw_flag=true;
}

void CShader_playerView::OnReloadMaterial()
{
	User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id).set_all_material_reload();
	theApp.redraw_flag=true;
}

void CShader_playerView::OnReloadLight()
{
	User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id).delete_graphics_system_object();
	theApp.ws->lights->load();
	theApp.redraw_flag=true;
}

void CShader_playerView::OnEditMaterial()
{
	User_interface::edit_part_material(theApp.ws);
	theApp.redraw_flag=true;
}

void CShader_playerView::SetCamera(int new_camera_id)
{
	camera_id=new_camera_id;
	theApp.redraw_flag=true;
}
void CShader_playerView::UpdateSetCamera(int new_camera_id,CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(camera_id==new_camera_id);
	switch(new_camera_id){
	case 3:
	case 4:
			OnEnableDisable(pCmdUI);
	}	
}
void CShader_playerView::SetMouseWheel(int new_mouse_wheel_id)
{
	theApp.mouse_wheel_id=new_mouse_wheel_id;
}
void CShader_playerView::UpdateSetMouseWheel(int new_mouse_wheel_id,CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(theApp.mouse_wheel_id==new_mouse_wheel_id);
}
void CShader_playerView::SetView(int view_id)
{
	User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id).set_view(view_id,theApp.time_length);
	theApp.redraw_flag=true;
}
void CShader_playerView::SetMouseMove(int move_id)
{
	theApp.mouse_move_id=move_id;
}
void CShader_playerView::UpdateSetMouseMove(int move_id,CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(theApp.mouse_move_id==move_id);
}
void CShader_playerView::OnClearAllSizeTag()
{
	Shader_size_tag::clear_all_tag_object();
	theApp.redraw_flag=true;
}

void CShader_playerView::OnCreateAssemblePart()
{
	CFileDialog dlg(FALSE,NULL,theApp.ws->plat_info->directory_name+_T("*.*"));
	
	if(dlg.DoModal()!=IDOK)
		return;
	bool material_flag=false;

	if(AfxMessageBox(_T("递增材质编号（YES）,重用材质编号(NO)"),MB_YESNO)==IDYES)
		material_flag=true;

	User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id).create_assemble_part(dlg.GetPathName(),material_flag);
}

void CShader_playerView::OnEnableDisable(CCmdUI *pCmdUI)
{
	CMainFrame*  pFrame=(CMainFrame*)(theApp.m_pMainWnd);
	if(pFrame!=NULL)
		pFrame->m_movement_list.OnEnableDisable(pCmdUI);
}

void CShader_playerView::OnTurnOnffCoordinator()
{
	Shader_coordinate::set_display_flag(Shader_coordinate::get_display_flag()?false:true);
}

void CShader_playerView::OnUpdateTurnOnffCoordinator(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(Shader_coordinate::get_display_flag()?false:true);
}

void CShader_playerView::OnSetBlackColor()
{
	CColorDialog dlg;

	if(dlg.DoModal()==IDOK){
		COLORREF color=dlg.GetColor();
		double red_color	=(double)(GetRValue(color))/255;
		double green_color	=(double)(GetGValue(color))/255;
		double blue_color	=(double)(GetBValue(color))/255;

		theApp.ws->background_color=Point(red_color,green_color,blue_color);

		theApp.redraw_flag=true;

	};
}

void CShader_playerView::OnReleaseVertex()
{
	User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id).release_component_mesh();
	theApp.redraw_flag=true;
}

void CShader_playerView::OnProjectionMatrixType()
{
	CPaintDC dc(this);
	User_interface a(*(theApp.ws),*gs,dc,camera_id);

	a.camera_pointer()->project_matrix_flag=(a.camera_pointer()->project_matrix_flag)?false:true;
}

void CShader_playerView::OnUpdateProjectionMatrixType(CCmdUI *pCmdUI)
{
	bool flag=User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id).camera_pointer()->project_matrix_flag;

	pCmdUI->SetCheck(flag);

	if(flag)
		pCmdUI->SetText(_T("透视投影"));
	else
		pCmdUI->SetText(_T("正交投影"));

}
void CShader_playerView::OnMemoryReleaseMap()
{
	theApp.ws->release_memory_flag_map=(theApp.ws->release_memory_flag_map==0)?(~0):0;
}
void CShader_playerView::OnUpdateMemoryReleaseMap(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(theApp.ws->release_memory_flag_map==0);
	pCmdUI->SetText((theApp.ws->release_memory_flag_map==0)?_T("顶点信息：装入后释放"):_T("顶点信息：装入后不释放"));
}
void CShader_playerView::OnBoxFunction()
{
	// TODO: Add your command handler code here

	box_function_flag=box_function_flag?false:true;
	theApp.redraw_flag=true;
}
void CShader_playerView::OnUpdateBoxFunction(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(box_function_flag);
}
void CShader_playerView::OnLockViewInBox()
{
	// TODO: Add your command handler code here
	CPaintDC dc(this);
	User_interface a(*(theApp.ws),*gs,dc,camera_id);
	a.camera_pointer()->lock_camera_flag=(a.camera_pointer()->lock_camera_flag)?false:true;
}
void CShader_playerView::OnUpdateLockViewInBox(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	bool flag=User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id).camera_pointer()->lock_camera_flag;
	pCmdUI->SetCheck(flag);
}
void CShader_playerView::OnViewRotateType()
{
	// TODO: Add your command handler code here
	CPaintDC dc(this);
	User_interface a(*(theApp.ws),*gs,dc,camera_id);
	a.camera_pointer()->rotate_view_flag=(a.camera_pointer()->rotate_view_flag)?false:true;
}
void CShader_playerView::OnUpdateViewRotateType(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	bool flag=User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id).camera_pointer()->rotate_view_flag;
	pCmdUI->SetCheck(flag);
}

static double rotate_view_direction=1.0;

void CShader_playerView::OnRotateViewX45()
{
	// TODO: Add your command handler code here
		
	User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id).modify_view(Location::x_rotate(45*rotate_view_direction),theApp.time_length);
	theApp.redraw_flag=true;
}

void CShader_playerView::OnRotateViewX90()
{
	// TODO: Add your command handler code here		
	User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id).modify_view(Location::x_rotate(90*rotate_view_direction),theApp.time_length);
	theApp.redraw_flag=true;
}

void CShader_playerView::OnRotateViewX180()
{
	// TODO: Add your command handler code here
		
	User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id).modify_view(Location::x_rotate(180*rotate_view_direction),theApp.time_length);
	theApp.redraw_flag=true;
}

void CShader_playerView::OnRotateViewY45()
{
	// TODO: Add your command handler code here
	User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id).modify_view(Location::y_rotate(45*rotate_view_direction),theApp.time_length);
	theApp.redraw_flag=true;
}

void CShader_playerView::OnRotateViewY90()
{
	// TODO: Add your command handler code here
	User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id).modify_view(Location::y_rotate(90*rotate_view_direction),theApp.time_length);
	theApp.redraw_flag=true;
}

void CShader_playerView::OnRotateViewY180()
{
	// TODO: Add your command handler code here
		
	User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id).modify_view(Location::y_rotate(180*rotate_view_direction),theApp.time_length);
	theApp.redraw_flag=true;
}

void CShader_playerView::OnRotateViewZ45()
{
	// TODO: Add your command handler code here
		
	User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id).modify_view(Location::z_rotate(45*rotate_view_direction),theApp.time_length);
	theApp.redraw_flag=true;
}

void CShader_playerView::OnRotateViewZ90()
{
	// TODO: Add your command handler code here
		
	User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id).modify_view(Location::z_rotate(90*rotate_view_direction),theApp.time_length);
	theApp.redraw_flag=true;
}

void CShader_playerView::OnRotateViewZ180()
{
	// TODO: Add your command handler code here
		
	User_interface(*(theApp.ws),*gs,CPaintDC(this),camera_id).modify_view(Location::z_rotate(180*rotate_view_direction),theApp.time_length);
	theApp.redraw_flag=true;
}

void CShader_playerView::OnRotateViewDirection()
{
	// TODO: Add your command handler code here
	rotate_view_direction=(rotate_view_direction>0)?(-1.0):1.0;
}

void CShader_playerView::OnUpdateRotateViewDirection(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck((rotate_view_direction>0)?true:false);
//	pCmdUI->SetText((rotate_view_direction>0)?_T("观测视角正向旋转"):_T("观测视角逆向旋转"));
}

void CShader_playerView::OnSetTagTypeSpaceDistance()
{
	tag_type_id=0;
}

void CShader_playerView::OnUpdateSetTagTypeSpaceDistance(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((tag_type_id==0)?true:false);
}

void CShader_playerView::OnSetTagTypeX()
{
	tag_type_id=1;
}

void CShader_playerView::OnUpdateSetTagTypeX(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((tag_type_id==1)?true:false);
}

void CShader_playerView::OnSetTagTypeY()
{
	tag_type_id=2;
}

void CShader_playerView::OnUpdateSetTagTypeY(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((tag_type_id==2)?true:false);
}

void CShader_playerView::OnSetTagTypeZ()
{
	tag_type_id=3;
}

void CShader_playerView::OnUpdateSetTagTypeZ(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((tag_type_id==3)?true:false);
}

void CShader_playerView::OnSetTagTypeLeftRight()
{
	tag_type_id=4;
}

void CShader_playerView::OnUpdateSetTagTypeLeftRight(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((tag_type_id==4)?true:false);
}

void CShader_playerView::OnSetTagTypeTopDown()
{
	tag_type_id=5;
}

void CShader_playerView::OnUpdateSetTagTypeTopDown(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((tag_type_id==5)?true:false);
}

void CShader_playerView::OnSetTagTypeViewPlane()
{
	tag_type_id=6;
}

void CShader_playerView::OnUpdateSetTagTypeViewPlane(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((tag_type_id==6)?true:false);
}


void CShader_playerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch(nChar){
	case 27:
		OnComponentClearSelect();
		break;
	};
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
void CShader_playerView::OnBackupData()
{
	Platform_information::backupdata();
}
void CShader_playerView::OnBackupCurrentData()
{
	Platform_information::backupcurrentdata(theApp.ws->plat_info->directory_name);
}
void CShader_playerView::OnRestoreData()
{
	Platform_information::restoredata();
	theApp.ws->exit_flag=true;
}
void CShader_playerView::OnDeleteData()
{
	theApp.ws->exit_flag=Platform_information::delete_all_data();
}
