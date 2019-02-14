
// change_movementDoc.cpp : Cchange_movementDoc 类的实现
//

#include "stdafx.h"
#include "change_movement.h"

#include "change_movementDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cchange_movementDoc

IMPLEMENT_DYNCREATE(Cchange_movementDoc, CDocument)

BEGIN_MESSAGE_MAP(Cchange_movementDoc, CDocument)
//	ON_COMMAND(ID_FILE_OPEN, &Cchange_movementDoc::OnFileOpen)
END_MESSAGE_MAP()


// Cchange_movementDoc 构造/析构

Cchange_movementDoc::Cchange_movementDoc()
{
	// TODO: 在此添加一次性构造代码

}

Cchange_movementDoc::~Cchange_movementDoc()
{
}

BOOL Cchange_movementDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}

// Cchange_movementDoc 序列化

void Cchange_movementDoc::Serialize(CArchive& ar)
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


// Cchange_movementDoc 诊断

#ifdef _DEBUG
void Cchange_movementDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void Cchange_movementDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Cchange_movementDoc 命令



BOOL Cchange_movementDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  Add your specialized creation code here

	static char tmp[20000];
	int i,j,main_step,sub_step;


	ifstream fin(lpszPathName);
	ofstream fout(CString(lpszPathName)+_T(".tmp"));

	while(1)
	{
		fin>>main_step>>sub_step;
		if(fin.eof())
			break;
		if(fin.fail())
			break;
		fout<<"\n"<<main_step<<"	"<<sub_step<<"\n";
		for(i=0;i<5;i++){
			fin>>tmp;
			fout<<tmp<<"\n";
		}
		fin>>i;
		if(i<0)
			i=0;
		fout<<"\n起始状态: 隐藏		终止状态: 显示		移动步骤数: "<<i<<"\n";

		for(;i>0;i--){
			for(j=0;j<17;j++){
				double x;
				fin>>x;
				fout<<x<<"	";
			}
			fout<<"\n";
		}

		fout<<"1000	1	0	0	0	0	1	0	0	0	0	1	0	0	0	0	1\n";	
		fout<<"\n配合数:		0\n\n";
	}

	fin.close();
	fout.close();

	CFile::Remove(lpszPathName);
	CFile::Rename(CString(lpszPathName)+_T(".tmp"),lpszPathName);

	return TRUE;
}
