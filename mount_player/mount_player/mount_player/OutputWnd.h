#include "FileView.h"

#pragma once

class movement;

class BridgeMoveView : public CDockablePane
{
// Construction
public:
	BridgeMoveView();

// Attributes
protected:

	CViewTree m_MoveView;
	CImageList m_MoveViewImages;
	BridgepartToolBar m_wndToolBar;
	
// Implementation
public:
	virtual ~BridgeMoveView();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnClickPartTree(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
public:
	void FillFileView(void);
	void AdjustLayout(void);
	void OnChangeVisualStyle(void);
	void fill_tree_item(void);
	void modify_image(movement *m,BOOL flag);
	int get_select_step_id(int *end_step_id);

	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnProperties();	
	void set_mount_step(int step_id);
	void clear_selection()
	{
		m_MoveView.SelectItem(NULL);
	}

private:
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

