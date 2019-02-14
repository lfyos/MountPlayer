
// change_movementView.h : Cchange_movementView 类的接口
//


#pragma once


class Cchange_movementView : public CView
{
protected: // 仅从序列化创建
	Cchange_movementView();
	DECLARE_DYNCREATE(Cchange_movementView)

// 属性
public:
	Cchange_movementDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~Cchange_movementView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // change_movementView.cpp 中的调试版本
inline Cchange_movementDoc* Cchange_movementView::GetDocument() const
   { return reinterpret_cast<Cchange_movementDoc*>(m_pDocument); }
#endif

