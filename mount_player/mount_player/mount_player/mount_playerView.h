
// mount_playerView.h : Cmount_playerView 类的接口
//

#include "mount_view.h"
#include "mount_processor.h"

#include "MainFrm.h"
#include "opengl_graphics_system.h"
#include <math.h>
#include "mount_sound_player.h"

#pragma once


class Cmount_playerView : public CView
{

protected: // 仅从序列化创建
	Cmount_playerView();
	DECLARE_DYNCREATE(Cmount_playerView)

// 属性
public:
	Cmount_playerDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~Cmount_playerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnHideComponent();
	afx_msg void OnHideAllComponent();
	afx_msg void OnClearHideComponent();
	afx_msg void OnDisplayBox();
	afx_msg void OnClearDisplayBox();
	afx_msg void OnUpSelection();
	afx_msg void OnDownSelection();
	afx_msg void OnClearComponentTransparent();
	afx_msg void OnSetComponentTransparent();
	afx_msg void OnSetClipPlane();
	afx_msg void OnLockComponent();
	afx_msg void OnPutInView();
	afx_msg void OnPutInEqualView();
	afx_msg void OnViewX();
	afx_msg void OnViewY();
	afx_msg void OnViewZ();
	afx_msg void OnBackwardView();
	afx_msg void OnPinPoint();
	afx_msg void OnInteractTranslation();
	afx_msg void OnInteractRotation();
	afx_msg void OnInteractEnlargeArea();
	afx_msg void OnInteractSelection();
	afx_msg void OnInteractScale();
	afx_msg void OnInteractSpeed();
	afx_msg void OnInteractClipPlane();
	afx_msg void OnInteractComponentTransparence();
	afx_msg void OnInteractManualMount();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnUpdatePinPoint(CCmdUI *pCmdUI);
	afx_msg void OnUpdateInteractTranslation(CCmdUI *pCmdUI);
	afx_msg void OnUpdateInteractRotation(CCmdUI *pCmdUI);
	afx_msg void OnUpdateInteractEnlargeArea(CCmdUI *pCmdUI);
	afx_msg void OnUpdateInteractSelection(CCmdUI *pCmdUI);
	afx_msg void OnUpdateInteractScale(CCmdUI *pCmdUI);
	afx_msg void OnUpdateInteractSpeed(CCmdUI *pCmdUI);
	afx_msg void OnUpdateInteractClipPlane(CCmdUI *pCmdUI);
	afx_msg void OnUpdateInteractComponentTransparence(CCmdUI *pCmdUI);
	afx_msg void OnUpdateInteractManualMount(CCmdUI *pCmdUI);

	afx_msg void OnStopMount();
	afx_msg void OnForwardOne();
	afx_msg void OnForwardAll();
	afx_msg void OnBackwardOne();
	afx_msg void OnBackwardAll();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnJumpStep();
	afx_msg void OnModifyLight();
	afx_msg void OnPlaySound();
	afx_msg void OnUpdatePlaySound(CCmdUI *pCmdUI);
	afx_msg void OnChangeView();
	afx_msg void OnUpdateChangeView(CCmdUI *pCmdUI);
	afx_msg void OnScaleupView();
	afx_msg void OnUpdateScaleupView(CCmdUI *pCmdUI);	
	afx_msg void OnVirtualMount();
	afx_msg void OnDesignInput();
	afx_msg void OnDesignDelete();
	afx_msg void OnDesignInputMessage();
	afx_msg void OnDesignMainSort();
	afx_msg void OnDesignSubSort();
	afx_msg void OnSaveMovement();
	afx_msg void OnRestoreMovement();
	afx_msg void OnMoveMovement();
	afx_msg void OnCombineStep();
	afx_msg void OnSeparateStep();
	afx_msg void OnMountProgressTable();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);	
	afx_msg void OnMainStepDecrease();
	afx_msg void OnMainStepIncrease();
	afx_msg void OnCurrentMountStep();
	afx_msg void OnSubStepDecrease();
	afx_msg void OnSubStepIncrease();
	afx_msg void OnCacheInsertOrDelete();
	afx_msg void OnUpdateCacheInsertOrDelete(CCmdUI *pCmdUI);
	afx_msg void OnCacheSubDecreaseMode();
	afx_msg void OnUpdateCacheSubDecreaseMode(CCmdUI *pCmdUI);
	afx_msg void OnCacheSubIncreaseMode();
	afx_msg void OnUpdateCacheSubIncreaseMode(CCmdUI *pCmdUI);
	afx_msg void OnCacheSubRemainMode();
	afx_msg void OnUpdateCacheSubRemainMode(CCmdUI *pCmdUI);
	afx_msg void OnBackwardBig();
	afx_msg void OnForwardBig();

private:
	graphics_system *bridge_gs;
	DWORD display_time_length;
	mount_sound_player sound_player;

public:
	void right_button_add_movement(UINT menu_id);
	void put_view_on_box(double box[],BOOL scaleup_flag);
	afx_msg void OnSetManualMount();
	afx_msg void OnUpdateSetManualMount(CCmdUI *pCmdUI);
	afx_msg void OnMountPartTable();
	afx_msg void OnDisplayReferenceFrame();
	afx_msg void OnUpdateDisplayReferenceFrame(CCmdUI *pCmdUI);
	afx_msg void OnTreeMovementSaveToCache();
	afx_msg void OnUpdateSetClipPlane(CCmdUI *pCmdUI);


	HACCEL m_hAccel;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
};

#ifndef _DEBUG  // mount_playerView.cpp 中的调试版本
inline Cmount_playerDoc* Cmount_playerView::GetDocument() const
   { return reinterpret_cast<Cmount_playerDoc*>(m_pDocument); }
#endif

