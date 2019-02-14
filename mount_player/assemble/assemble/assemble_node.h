#pragma once

#include "stdafx.h"
#include "assemble.h"
#include <fstream>
using namespace std;

#include "Location.h"


class assemble_node
{
	bool is_root_flag;

	CComPtr		<IModelDoc2>		pModelDoc;
	CComPtr		<IComponent2>		pComponent;
	CComPtr		<IMathTransform>	pTransForm; 

	void create_child();
	void get_information(void);
	void output_string(ofstream * f, CString str)
	{
		CString tmp=str;
		BSTR   bstrText   =   tmp.AllocSysString();
		char*  lpszText2   =   _com_util::ConvertBSTRToString(bstrText);     
		(*f)<<lpszText2;

		SysFreeString(bstrText);
		delete []lpszText2;
		return;
	}
	void output_space(ofstream * f,int n)
	{
		(*f)<<"\n";
		for(int i=0;i<n;i++)
			(*f)<<" ";
		(*f)<<(1+n/8)<<":";
	};
	CString get_rid_of_space(CString str)
	{
		CString ret_str=_T("");
		for(int i=0;i<str.GetLength();i++)
			if(CString(str.GetAt(i))!=_T(" "))
				ret_str=ret_str+str.GetAt(i);
		return ret_str;
	}
public:

	CString component_name,type_name,file_name;
	double location[16];

	int children_number;
	assemble_node **children;

	assemble_node(CComPtr<ISldWorks> my_sw);
	assemble_node(CComPtr<IComponent2>	my_pComponent);
	~assemble_node(void);
	
	void output_data(ofstream * f,int cut_length,Location *parent_location=NULL,int space_number=0);
	CString get_directory_name();
	CString get_min_string(CString &str1 , CString &str2);
};



