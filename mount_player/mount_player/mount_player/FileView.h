
#pragma once

#include "ViewTree.h"
#include "mount_view.h"
class Cmount_playerView;
class part_tree ;

class BridgepartToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class BridgepartView : public CDockablePane
{
// Construction
public:
	BridgepartView();

	void AdjustLayout();
	void OnChangeVisualStyle();

// Attributes
protected:

	CViewTree m_wndFileView;
	CImageList m_FileViewImages;
	BridgepartToolBar m_wndToolBar;

protected:
	void FillFileView();

// Implementation
public:
	virtual ~BridgepartView();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenWith();
	afx_msg void OnDummyCompile();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	afx_msg void OnClickPartTree(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
public:
	void fill_tree_item(part_tree * t);
	void update_display(part_tree *t);
	void modify_image(part_tree *t,BOOL flag);
	void clear_selection()
	{
		m_wndFileView.SelectItem(NULL);
	}
private:
	void view_lock_at_step(int main_step_id, int sub_step_id,Cmount_playerView* pView);
	
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

