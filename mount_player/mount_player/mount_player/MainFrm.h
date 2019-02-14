
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "FileView.h"
#include "ClassView.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"
#include "movement.h"
#include "mount_view.h"
#include "mount_bridge.h"

class movement;


class CMainFrame : public CFrameWndEx
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	BridgepartView        m_PartView;
	BridgetreeView        m_TreeView;
	BridgeMoveView        m_MoveView;
	CPropertiesWnd    m_wndProperties;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();

public:
	void modify_tree_image(part_tree *t,BOOL flag);
	void modify_image(movement *m,BOOL flag)
	{
		m_MoveView.modify_image(m,flag);
		modify_tree_image(m->tree_node,flag);
	}
	void update_display(part_tree *p_tree,part_tree *t_tree,int step_id)
	{
		if(t_tree!=NULL)
			m_TreeView.update_display(t_tree);
		if(p_tree!=NULL)
			m_PartView.update_display(p_tree);
		if(step_id>=0)
			m_MoveView.set_mount_step(step_id);
	}
	void clear_selection()
	{
		m_TreeView.clear_selection();
		m_PartView.clear_selection();
		m_MoveView.clear_selection();

	}
	void statusbar_display(CString statusbar_text)
	{
		m_wndStatusBar.SetWindowTextA(statusbar_text);
	}
	void fill_tree_item()
	{
		bridge_view->bridge->tree->clear_mark_flag();
		m_PartView.fill_tree_item(bridge_view->bridge->tree);
		m_TreeView.fill_tree_item(bridge_view->bridge->tree);
		m_MoveView.fill_tree_item();
	}
	int get_select_step_id(int *end_step_id)
	{
		return m_MoveView.get_select_step_id(end_step_id);
	}
	void display_mount_information(int start_step_id,int end_step_id,BOOL mark_flag)
	{
		statusbar_display(m_wndProperties.display_mount_information(start_step_id,end_step_id,mark_flag));
	}
	void display_mount_information(int visible_number[],int hide_number[],int step_id)
	{
		statusbar_display(m_wndProperties.display_mount_information(visible_number,hide_number,step_id));
	}
	void display_part_information(part_info *p)
	{
		m_wndProperties.display_part_information(p);
	}
	void display_part_information(part_tree *t)
	{
		m_wndProperties.display_part_information(t);
	}
	void display_tree_information(part_tree *t)
	{
		m_wndProperties.display_tree_information(t);
	}
};
