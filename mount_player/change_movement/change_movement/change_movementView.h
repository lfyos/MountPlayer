
// change_movementView.h : Cchange_movementView ��Ľӿ�
//


#pragma once


class Cchange_movementView : public CView
{
protected: // �������л�����
	Cchange_movementView();
	DECLARE_DYNCREATE(Cchange_movementView)

// ����
public:
	Cchange_movementDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~Cchange_movementView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // change_movementView.cpp �еĵ��԰汾
inline Cchange_movementDoc* Cchange_movementView::GetDocument() const
   { return reinterpret_cast<Cchange_movementDoc*>(m_pDocument); }
#endif

