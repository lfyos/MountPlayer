
// Shader_playerDoc.cpp : implementation of the CShader_playerDoc class
//

#include "stdafx.h"
#include "Shader_player.h"

#include "Shader_playerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CShader_playerDoc

IMPLEMENT_DYNCREATE(CShader_playerDoc, CDocument)

BEGIN_MESSAGE_MAP(CShader_playerDoc, CDocument)
END_MESSAGE_MAP()


// CShader_playerDoc construction/destruction

CShader_playerDoc::CShader_playerDoc()
{
	// TODO: add one-time construction code here

}

CShader_playerDoc::~CShader_playerDoc()
{
}

BOOL CShader_playerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CShader_playerDoc serialization

void CShader_playerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CShader_playerDoc diagnostics

#ifdef _DEBUG
void CShader_playerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CShader_playerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CShader_playerDoc commands
