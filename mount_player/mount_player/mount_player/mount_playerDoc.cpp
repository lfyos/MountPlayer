
// mount_playerDoc.cpp : Cmount_playerDoc ���ʵ��
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


// Cmount_playerDoc ����/����

Cmount_playerDoc::Cmount_playerDoc()
{
	// TODO: �ڴ����һ���Թ������

}

Cmount_playerDoc::~Cmount_playerDoc()
{
}

BOOL Cmount_playerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// Cmount_playerDoc ���л�

void Cmount_playerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// Cmount_playerDoc ���

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

