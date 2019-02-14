
// change_movementView.cpp : Cchange_movementView 类的实现
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

// Cchange_movementView 构造/析构

Cchange_movementView::Cchange_movementView()
{
	// TODO: 在此处添加构造代码

}

Cchange_movementView::~Cchange_movementView()
{
}

BOOL Cchange_movementView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// Cchange_movementView 绘制

void Cchange_movementView::OnDraw(CDC* /*pDC*/)
{
	Cchange_movementDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
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


// Cchange_movementView 诊断

#ifdef _DEBUG
void Cchange_movementView::AssertValid() const
{
	CView::AssertValid();
}

void Cchange_movementView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cchange_movementDoc* Cchange_movementView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cchange_movementDoc)));
	return (Cchange_movementDoc*)m_pDocument;
}
#endif //_DEBUG


// Cchange_movementView 消息处理程序
