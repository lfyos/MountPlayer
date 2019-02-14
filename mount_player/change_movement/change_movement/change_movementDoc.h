
// change_movementDoc.h : Cchange_movementDoc 类的接口
//


#pragma once


#include <math.h>
#include <fstream>
using namespace std;

class Cchange_movementDoc : public CDocument
{
protected: // 仅从序列化创建
	Cchange_movementDoc();
	DECLARE_DYNCREATE(Cchange_movementDoc)

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~Cchange_movementDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnFileOpen();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};


