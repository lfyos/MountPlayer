
// change_movementDoc.h : Cchange_movementDoc ��Ľӿ�
//


#pragma once


#include <math.h>
#include <fstream>
using namespace std;

class Cchange_movementDoc : public CDocument
{
protected: // �������л�����
	Cchange_movementDoc();
	DECLARE_DYNCREATE(Cchange_movementDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~Cchange_movementDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnFileOpen();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};


