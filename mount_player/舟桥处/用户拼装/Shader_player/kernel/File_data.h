#include "Platform_information.h"


class File_data
{
	char *buffer;
	int length;
public:
	
	inline void clear_file_data()
	{
		if(buffer!=NULL){
			delete []buffer;
			buffer=NULL;
		}
		length=0;
	}
	inline void load_string_data(CString &str)
	{
		clear_file_data();
		
		if(str.GetLength()>0){
			CString tmp=str;
			BSTR   bstrText	=	tmp.AllocSysString();
			buffer			=	_com_util::ConvertBSTRToString(bstrText);     
			SysFreeString(bstrText);
			length=get_data_length(buffer);
		}
	}

	inline int load_file_data(CString file_name)
	{
		clear_file_data();

		if((length=get_file_length(file_name))<=0)
			return 0;

		CFile f(file_name,CFile::modeRead);
		if(f.m_hFile==CFile::hFileNull){
			f.Close();
			return 0;
		}
		buffer=new char[length+16];
		for(int i=0;i<(length+16);i++)
			buffer[i]='\0';
		if((int)(f.Read(buffer,length))!=length){
			delete []buffer;
			buffer=NULL;
			length=0;
			f.Close();

			return 0;
		}
		f.Close();
		return length;
	}
	inline void save_file_data(CString file_name)
	{
		CFile f(file_name,CFile::modeWrite|CFile::modeCreate );
		if(f.m_hFile!=CFile::hFileNull)
			if((length>0)&&(buffer!=NULL))
				f.Write(buffer,length);
		f.Close();
	}
	inline void insert_file_data(CString file_name)
	{
		File_data f;
		f.load_file_data(file_name);
		if(f.data_length()>0){
			if(data_length()>0)
				load_string_data(CString(f.data_pointer())+CString(data_pointer()));
			else
				load_string_data(CString(f.data_pointer()));
		}	
	}
	inline void append_file_data(CString file_name)
	{
		File_data f;
		f.load_file_data(file_name);
		if(f.data_length()>0){
			if(data_length()>0)
				load_string_data(CString(data_pointer())+CString(f.data_pointer()));
			else
				load_string_data(CString(f.data_pointer()));
		}	
	}


	inline void insert_data(CString data_string)
	{
			if(data_length()>0)
				load_string_data(data_string+CString(data_pointer()));
			else
				load_string_data(data_string);
	}
	inline void append_data(CString data_string)
	{
			if(data_length()>0)
				load_string_data(CString(data_pointer())+data_string);
			else
				load_string_data(data_string);
	}

	inline char *data_pointer()
	{
		return buffer;
	}
	inline int data_length()
	{
		return length;
	}
	inline ~File_data()
	{
		clear_file_data();
	}

	inline File_data()
	{
		length=0;
		buffer=NULL;
	}
	inline File_data(CString file_name)
	{
		length=0;
		buffer=NULL;
		append_file_data(file_name);
	}


	inline static void output_string(ofstream &f, CString str)
	{
		File_data output_data;
		output_data.load_string_data(str);
		f<<(output_data.data_pointer());
	}

	inline static int get_data_length(char *data_pointer)
	{
		if(data_pointer==NULL)
			return 0;
		for(int i=0;;i++)
			if(data_pointer[i]=='\0')
				return i+1;
	}

	static int get_file_length(CString file_name)
	{
		CFileStatus file_status;
		if(CFile::GetStatus(file_name,file_status))
			if(file_status.m_size>=0)
					return (int)(file_status.m_size);
		return -1;
	}

	inline static CString get_head_string(CString &my_str)
	{
		int i,j,length;
		for(i=0,length=my_str.GetLength();i<length;i++)
			if(my_str.GetAt(i)==' ')
				break;
		for(j=i,length=my_str.GetLength();j<length;j++)
			if(my_str.GetAt(j)!=' ')
				break;
		CString str=my_str.Mid(1,i-1);
		my_str=my_str.Right(my_str.GetLength()-j);

		return str;
	}
};

