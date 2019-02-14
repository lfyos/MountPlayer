
#pragma once

#include "ViewTree.h"
#include "part_tree.h"

class BridgetreeToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class BridgetreeView : public CDockablePane
{
public:
	BridgetreeView();
	virtual ~BridgetreeView();

	void AdjustLayout();
	void OnChangeVisualStyle();

protected:
	BridgetreeToolBar m_wndToolBar;
	CViewTree m_wndClassView;
	CImageList m_ClassViewImages;
	UINT m_nCurrSort;

	void FillClassView();

// Overrides
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnClassAddMemberFunction();
	afx_msg void OnClassAddMemberVariable();
	afx_msg void OnClassDefinition();
	afx_msg void OnClassProperties();
	afx_msg void OnNewFolder();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnChangeActiveTab(WPARAM, LPARAM);
	afx_msg void OnSort(UINT id);
	afx_msg void OnUpdateSort(CCmdUI* pCmdUI);

	afx_msg void OnClickPartTree(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
	
public:
	void clear_selection()
	{
		m_wndClassView.SelectItem(NULL);
	}
	void fill_tree_item(part_tree * t);
	void update_display(part_tree *t);
	void modify_image(part_tree *t,BOOL flag)
	{
		m_wndClassView.SetItemImage(t->structure_tree_id,flag?3:5,flag?3:5);
	}
};

