
// change_movementView.cpp : Cchange_movementView ���ʵ��
//

#include "stdafx.h"
#include "change_movement.h"

#include "change_movementDoc.h"
#include "change_movementView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cchange_movementView

IMPLEMENT_DYNCREATE(Cchange_movementView, CView)

BEGIN_MESSAGE_MAP(Cchange_movementView, CView)
END_MESSAGE_MAP()

// Cchange_movementView ����/����

Cchange_movementView::Cchange_movementView()
{
	// TODO: �ڴ˴���ӹ������

}

Cchange_movementView::~Cchange_movementView()
{
}

BOOL Cchange_movementView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// Cchange_movementView ����

void Cchange_movementView::OnDraw(CDC* /*pDC*/)
{
	Cchange_movementDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}

void Cchange_movementView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Cchange_movementView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// Cchange_movementView ���

#ifdef _DEBUG
void Cchange_movementView::AssertValid() const
{
	CView::AssertValid();
}

void Cchange_movementView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cchange_movementDoc* Cchange_movementView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cchange_movementDoc)));
	return (Cchange_movementDoc*)m_pDocument;
}
#endif //_DEBUG


// Cchange_movementView ��Ϣ�������
