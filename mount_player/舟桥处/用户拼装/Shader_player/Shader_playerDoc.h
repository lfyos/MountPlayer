
// Shader_playerDoc.h : interface of the CShader_playerDoc class
//


#pragma once


class CShader_playerDoc : public CDocument
{
protected: // create from serialization only
	CShader_playerDoc();
	DECLARE_DYNCREATE(CShader_playerDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CShader_playerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


