
// Shader_playerView.h : interface of the CShader_playerView class
//


#pragma once

class CMainFrame;

class CShader_playerView : public CView
{
protected: // create from serialization only
	CShader_playerView();
	DECLARE_DYNCREATE(CShader_playerView)

// Attributes
public:
	CShader_playerDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CShader_playerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

public:

	Graphics_system *gs;
	int camera_id;

	afx_msg void OnPaint();

	afx_msg void OnComponentSetFrameShowDetail();
	afx_msg void OnComponentClearFrameShowDetail();

	afx_msg void OnComponentSetFrameShowShow();
	afx_msg void OnComponentSetFrameShowHide();
	afx_msg void OnComponentSetFrameShowStipple();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint point);

	void right_button_add_movement(UINT menu_id);


	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnAdvanceSubStep();
	afx_msg void OnAdvanceMainStep();
	afx_msg void OnAdvanceForever();
	afx_msg void OnRetreatSubStep();
	afx_msg void OnRetreatMainStep();
	afx_msg void OnRetreatForever();
	afx_msg void OnPlaySound();
	afx_msg void OnUpdatePlaySound(CCmdUI *pCmdUI);
	
	afx_msg void OnLocateView();
	afx_msg void OnUpdateLocateView(CCmdUI *pCmdUI);
	afx_msg void OnExpandView();
	afx_msg void OnUpdateExpandView(CCmdUI *pCmdUI);

	afx_msg void OnStopMount();
	afx_msg void OnJumpMount();

	afx_msg void OnVirtualMount();
	afx_msg void OnUpdateVirtualMount(CCmdUI *pCmdUI);

	afx_msg void OnComponentHide();
	afx_msg void OnComponentShow();

	afx_msg void OnComponentSetFrameShow();
	afx_msg void OnComponentSetOnlyFrameShow();
	afx_msg void OnComponentClearFrameShow();


	afx_msg void OnShowTransparent();
	afx_msg void OnShowNotTransparent();
	afx_msg void OnSelectUpComponent();
	afx_msg void OnSelectDownComponent();
	afx_msg void OnCutComponentStart();
	afx_msg void OnCutComponentEnd();
	afx_msg void OnComponentReset();
	afx_msg void OnComponentLocation();
	afx_msg void OnComponentClearSelect();
	afx_msg void OnRestoreLastView();

		
	afx_msg void OnMountDesignStart();

	afx_msg void OnMountDocument();
	afx_msg void OnMountManual();

	afx_msg void OnModifyShader();
	afx_msg void OnReloadMaterial();
	afx_msg void OnReloadLight();
	afx_msg void OnEditMaterial();

	afx_msg void OnMountSaveToCache();

private:
	void SetCamera(int new_camera_id);
	void UpdateSetCamera(int new_camera_id,CCmdUI *pCmdUI);

	void SetMouseWheel(int new_mouse_wheel_id);
	void UpdateSetMouseWheel(int new_mouse_wheel_id,CCmdUI *pCmdUI);

	void SetView(int view_id);

	void SetMouseMove(int move_id);
	void UpdateSetMouseMove(int move_id,CCmdUI *pCmdUI);

public:
	afx_msg void OnSetCamera0(){SetCamera(0);};
	afx_msg void OnSetCamera1(){SetCamera(1);};
	afx_msg void OnSetCamera2(){SetCamera(2);};
	afx_msg void OnSetCamera3(){SetCamera(3);};
	afx_msg void OnSetCamera4(){SetCamera(4);};
	afx_msg void OnSetCamera5(){SetCamera(5);};
	afx_msg void OnSetCamera6(){SetCamera(6);};
	afx_msg void OnSetCamera7(){SetCamera(7);};
	afx_msg void OnSetCamera8(){SetCamera(8);};
	

	afx_msg void OnUpdateSetCamera0(CCmdUI *pCmdUI){UpdateSetCamera(0,pCmdUI);};
	afx_msg void OnUpdateSetCamera1(CCmdUI *pCmdUI){UpdateSetCamera(1,pCmdUI);};
	afx_msg void OnUpdateSetCamera2(CCmdUI *pCmdUI){UpdateSetCamera(2,pCmdUI);};
	afx_msg void OnUpdateSetCamera3(CCmdUI *pCmdUI){UpdateSetCamera(3,pCmdUI);};
	afx_msg void OnUpdateSetCamera4(CCmdUI *pCmdUI){UpdateSetCamera(4,pCmdUI);};
	afx_msg void OnUpdateSetCamera5(CCmdUI *pCmdUI){UpdateSetCamera(5,pCmdUI);};
	afx_msg void OnUpdateSetCamera6(CCmdUI *pCmdUI){UpdateSetCamera(6,pCmdUI);};
	afx_msg void OnUpdateSetCamera7(CCmdUI *pCmdUI){UpdateSetCamera(7,pCmdUI);};

	afx_msg void OnSetMouseWheel0(){SetMouseWheel(0);};
	afx_msg void OnSetMouseWheel1(){SetMouseWheel(1);};
	afx_msg void OnSetMouseWheel2(){SetMouseWheel(2);};
	afx_msg void OnSetMouseWheel3(){SetMouseWheel(3);};
	afx_msg void OnSetMouseWheel4(){SetMouseWheel(4);};
	afx_msg void OnSetMouseWheel5(){SetMouseWheel(5);};
	afx_msg void OnSetMouseWheel6(){SetMouseWheel(6);};

	afx_msg void OnUpdateSetMouseWheel0(CCmdUI *pCmdUI){UpdateSetMouseWheel(0,pCmdUI);};
	afx_msg void OnUpdateSetMouseWheel1(CCmdUI *pCmdUI){UpdateSetMouseWheel(1,pCmdUI);};
	afx_msg void OnUpdateSetMouseWheel2(CCmdUI *pCmdUI){UpdateSetMouseWheel(2,pCmdUI);};
	afx_msg void OnUpdateSetMouseWheel3(CCmdUI *pCmdUI){UpdateSetMouseWheel(3,pCmdUI);};
	afx_msg void OnUpdateSetMouseWheel4(CCmdUI *pCmdUI){UpdateSetMouseWheel(4,pCmdUI);};
	afx_msg void OnUpdateSetMouseWheel5(CCmdUI *pCmdUI){UpdateSetMouseWheel(5,pCmdUI);};
	afx_msg void OnUpdateSetMouseWheel6(CCmdUI *pCmdUI){UpdateSetMouseWheel(6,pCmdUI);};

public:
	afx_msg void OnSetViewNoDirection(){SetView(-1);};
	afx_msg void OnSetView0 (){SetView( 0);};
	afx_msg void OnSetView1 (){SetView( 1);};
	afx_msg void OnSetView2 (){SetView( 2);};
	afx_msg void OnSetView3 (){SetView( 3);};
	afx_msg void OnSetView4 (){SetView( 4);};
	afx_msg void OnSetView5 (){SetView( 5);};
	afx_msg void OnSetView6 (){SetView( 6);};
	afx_msg void OnSetView7 (){SetView( 7);};
	afx_msg void OnSetView8 (){SetView( 8);};
	afx_msg void OnSetView9 (){SetView( 9);};
	afx_msg void OnSetView10(){SetView(10);};
	afx_msg void OnSetView11(){SetView(11);};	
	afx_msg void OnSetView12(){SetView(12);};
	afx_msg void OnSetView13(){SetView(13);};
	afx_msg void OnSetView14(){SetView(14);};
	afx_msg void OnSetView15(){SetView(15);};
	afx_msg void OnSetView16(){SetView(16);};
	afx_msg void OnSetView17(){SetView(17);};

	afx_msg void OnSetMouseMove0 (){SetMouseMove( 0);};
	afx_msg void OnSetMouseMove1 (){SetMouseMove( 1);};
	afx_msg void OnSetMouseMove2 (){SetMouseMove( 2);};
	afx_msg void OnSetMouseMove3 (){SetMouseMove( 3);};
	afx_msg void OnSetMouseMove4 (){SetMouseMove( 4);};
	afx_msg void OnSetMouseMove5 (){SetMouseMove( 5);};
	afx_msg void OnSetMouseMove6 (){SetMouseMove( 6);};
	afx_msg void OnSetMouseMove7 (){SetMouseMove( 7);};
	afx_msg void OnSetMouseMove8 (){SetMouseMove( 8);};
	afx_msg void OnSetMouseMove9 (){SetMouseMove( 9);};
	afx_msg void OnSetMouseMove10(){SetMouseMove(10);};
	afx_msg void OnSetMouseMove11(){SetMouseMove(11);};
	afx_msg void OnSetMouseMove12(){SetMouseMove(12);};


	afx_msg void OnSetMouseMove21(){SetMouseMove(21);};
	afx_msg void OnSetMouseMove22(){SetMouseMove(22);};
	afx_msg void OnSetMouseMove23(){SetMouseMove(23);};
	afx_msg void OnSetMouseMove24(){SetMouseMove(24);};
	

	afx_msg void OnSetMouseMove40(){SetMouseMove(40);};


	afx_msg void OnUpdateSetMouseMove0 (CCmdUI *pCmdUI){UpdateSetMouseMove( 0,pCmdUI);};
	afx_msg void OnUpdateSetMouseMove1 (CCmdUI *pCmdUI){UpdateSetMouseMove( 1,pCmdUI);};
	afx_msg void OnUpdateSetMouseMove2 (CCmdUI *pCmdUI){UpdateSetMouseMove( 2,pCmdUI);};
	afx_msg void OnUpdateSetMouseMove3 (CCmdUI *pCmdUI){UpdateSetMouseMove( 3,pCmdUI);};
	afx_msg void OnUpdateSetMouseMove4 (CCmdUI *pCmdUI){UpdateSetMouseMove( 4,pCmdUI);};
	afx_msg void OnUpdateSetMouseMove5 (CCmdUI *pCmdUI){UpdateSetMouseMove( 5,pCmdUI);};
	afx_msg void OnUpdateSetMouseMove6 (CCmdUI *pCmdUI){UpdateSetMouseMove( 6,pCmdUI);};
	afx_msg void OnUpdateSetMouseMove7 (CCmdUI *pCmdUI){UpdateSetMouseMove( 7,pCmdUI);};
	afx_msg void OnUpdateSetMouseMove8 (CCmdUI *pCmdUI){UpdateSetMouseMove( 8,pCmdUI);};
	afx_msg void OnUpdateSetMouseMove9 (CCmdUI *pCmdUI){UpdateSetMouseMove( 9,pCmdUI);};
	afx_msg void OnUpdateSetMouseMove10(CCmdUI *pCmdUI){UpdateSetMouseMove(10,pCmdUI);};
	afx_msg void OnUpdateSetMouseMove11(CCmdUI *pCmdUI){UpdateSetMouseMove(11,pCmdUI);};
	afx_msg void OnUpdateSetMouseMove12(CCmdUI *pCmdUI){UpdateSetMouseMove(12,pCmdUI);};

	afx_msg void OnUpdateSetMouseMove21(CCmdUI *pCmdUI){UpdateSetMouseMove(21,pCmdUI);};
	afx_msg void OnUpdateSetMouseMove22(CCmdUI *pCmdUI){UpdateSetMouseMove(22,pCmdUI);};
	afx_msg void OnUpdateSetMouseMove23(CCmdUI *pCmdUI){UpdateSetMouseMove(23,pCmdUI);};
	afx_msg void OnUpdateSetMouseMove24(CCmdUI *pCmdUI){UpdateSetMouseMove(24,pCmdUI);};
	afx_msg void OnUpdateSetMouseMove25(CCmdUI *pCmdUI){UpdateSetMouseMove(25,pCmdUI);};
	afx_msg void OnUpdateSetMouseMove26(CCmdUI *pCmdUI){UpdateSetMouseMove(26,pCmdUI);};
	afx_msg void OnUpdateSetMouseMove27(CCmdUI *pCmdUI){UpdateSetMouseMove(27,pCmdUI);};
	afx_msg void OnUpdateSetMouseMove28(CCmdUI *pCmdUI){UpdateSetMouseMove(28,pCmdUI);};
	afx_msg void OnUpdateSetMouseMove29(CCmdUI *pCmdUI){UpdateSetMouseMove(29,pCmdUI);};
	afx_msg void OnUpdateSetMouseMove30(CCmdUI *pCmdUI){UpdateSetMouseMove(30,pCmdUI);};

	afx_msg void OnEnableDisable(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSetMouseMove40(CCmdUI *pCmdUI){UpdateSetMouseMove(40,pCmdUI);OnEnableDisable(pCmdUI);};

	afx_msg void OnClearAllSizeTag();

	afx_msg void OnCreateAssemblePart();
	
	afx_msg void OnTurnOnffCoordinator();
	afx_msg void OnUpdateTurnOnffCoordinator(CCmdUI *pCmdUI);
	void test_match_face(CMainFrame*  pFrame,CPoint point);

	afx_msg void OnSetBlackColor();
	afx_msg void OnReleaseVertex();
	afx_msg void OnProjectionMatrixType();
	afx_msg void OnUpdateProjectionMatrixType(CCmdUI *pCmdUI);
	afx_msg void OnMemoryReleaseMap();
	afx_msg void OnUpdateMemoryReleaseMap(CCmdUI *pCmdUI);
	
	afx_msg void OnBoxFunction();
	afx_msg void OnUpdateBoxFunction(CCmdUI *pCmdUI);
	afx_msg void OnLockViewInBox();
	afx_msg void OnUpdateLockViewInBox(CCmdUI *pCmdUI);
	afx_msg void OnViewRotateType();
	afx_msg void OnUpdateViewRotateType(CCmdUI *pCmdUI);
	afx_msg void OnRotateViewX45();
	afx_msg void OnRotateViewX90();
	afx_msg void OnRotateViewX180();
	afx_msg void OnRotateViewY45();
	afx_msg void OnRotateViewY90();
	afx_msg void OnRotateViewY180();
	afx_msg void OnRotateViewZ45();
	afx_msg void OnRotateViewZ90();
	afx_msg void OnRotateViewZ180();
	afx_msg void OnRotateViewDirection();
	afx_msg void OnUpdateRotateViewDirection(CCmdUI *pCmdUI);
	afx_msg void OnSetTagTypeSpaceDistance();
	afx_msg void OnUpdateSetTagTypeSpaceDistance(CCmdUI *pCmdUI);
	afx_msg void OnSetTagTypeX();
	afx_msg void OnUpdateSetTagTypeX(CCmdUI *pCmdUI);
	afx_msg void OnSetTagTypeY();
	afx_msg void OnUpdateSetTagTypeY(CCmdUI *pCmdUI);
	afx_msg void OnSetTagTypeZ();
	afx_msg void OnUpdateSetTagTypeZ(CCmdUI *pCmdUI);
	afx_msg void OnSetTagTypeLeftRight();
	afx_msg void OnUpdateSetTagTypeLeftRight(CCmdUI *pCmdUI);
	afx_msg void OnSetTagTypeTopDown();
	afx_msg void OnUpdateSetTagTypeTopDown(CCmdUI *pCmdUI);
	afx_msg void OnSetTagTypeViewPlane();
	afx_msg void OnUpdateSetTagTypeViewPlane(CCmdUI *pCmdUI);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnBackupData();
	afx_msg void OnRestoreData();
	afx_msg void OnBackupCurrentData();
};

#ifndef _DEBUG  // debug version in Shader_playerView.cpp
inline CShader_playerDoc* CShader_playerView::GetDocument() const
   { return reinterpret_cast<CShader_playerDoc*>(m_pDocument); }
#endif

