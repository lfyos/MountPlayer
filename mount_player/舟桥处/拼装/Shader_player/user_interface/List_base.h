
#pragma once

#include "../ViewTree.h"

class List_toolbar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class List_base : public CDockablePane
{
// Construction
	UINT  popup_menu;
public:
	List_base(UINT my_popup_menu)
	{
		popup_menu=my_popup_menu;
	};

	void AdjustLayout();
	void OnChangeVisualStyle();

// Attributes
public:

	CPoint popup_menu_location;
	CViewTree list;
	CImageList images;
	List_toolbar toolbar;

// Implementation
public:
	virtual ~List_base(){};

	virtual void file_list()=NULL;
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()
};

