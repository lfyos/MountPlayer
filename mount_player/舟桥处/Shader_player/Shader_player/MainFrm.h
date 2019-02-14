
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "user_interface/Component_list.h"
#include "user_interface/Part_list.h"
#include "user_interface/Movement_list.h"
#include "user_interface/Part_property.h"
#include "user_interface/Assemble_type_list.h"


#define MAX_TOOLBAR_NUMBER		6


class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
protected:
	CSplitterWnd m_wndSplitter;
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
public:
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar[MAX_TOOLBAR_NUMBER];
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;

public:
	Assemble_type_list	m_assemble_type_list;
	Component_list		m_component_list;
	Part_list			m_part_list;
	Movement_list		m_movement_list;
	Part_property		m_property_list;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
};


