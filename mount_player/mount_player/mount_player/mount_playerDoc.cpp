
// mount_playerDoc.cpp : Cmount_playerDoc 类的实现
//

#include "stdafx.h"
#include "mount_player.h"

#include "mount_playerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cmount_playerDoc

IMPLEMENT_DYNCREATE(Cmount_playerDoc, CDocument)

BEGIN_MESSAGE_MAP(Cmount_playerDoc, CDocument)

END_MESSAGE_MAP()


// Cmount_playerDoc 构造/析构

Cmount_playerDoc::Cmount_playerDoc()
{
	// TODO: 在此添加一次性构造代码

}

Cmount_playerDoc::~Cmount_playerDoc()
{
}

BOOL Cmount_playerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// Cmount_playerDoc 序列化

void Cmount_playerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// Cmount_playerDoc 诊断

#ifdef _DEBUG
void Cmount_playerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void Cmount_playerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

