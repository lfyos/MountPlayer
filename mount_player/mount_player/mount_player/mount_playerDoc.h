
// mount_playerDoc.h : Cmount_playerDoc ��Ľӿ�
//


#pragma once


class Cmount_playerDoc : public CDocument
{
protected: // �������л�����
	Cmount_playerDoc();
	DECLARE_DYNCREATE(Cmount_playerDoc)

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
	virtual ~Cmount_playerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:

};


