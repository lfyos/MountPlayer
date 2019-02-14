#pragma once


#include "Class_list.h"

class Platform_file_directory_link_list
{
public:
	CString directory_name;
	Platform_file_directory_link_list *next_directory_link_list;

	inline Platform_file_directory_link_list(CString my_directory_name,Platform_file_directory_link_list *my_next_directory_link_list)
	{
		directory_name=my_directory_name;
		next_directory_link_list=my_next_directory_link_list;
	}
};

class Platform_information
{
	CFileFind finder;
	BOOL finder_result;
	Platform_file_directory_link_list *directory_link_list;

	inline void add_directory(CString root_directory_name)
	{
		directory_link_list=new Platform_file_directory_link_list(root_directory_name,directory_link_list);
	}
public:
	CTime last_time;
	int compare_file_counter;

	CString directory_name;

	inline static CString get_execute_file_name()
	{
		wchar_t buf[MAX_BUFFER_LENGTH];	
		::GetModuleFileName(NULL,buf,sizeof(buf));
		CString my_file_name=buf;
		int length=my_file_name.GetLength();
		for(int i=length-1;i>=0;i--)
			if((my_file_name.GetAt(i)=='\\')||(my_file_name.GetAt(i)=='/')){
				my_file_name=my_file_name.Right(length-i-1);
				break;
			}
		return my_file_name;
	}

	inline static CString get_directory_name()
	{
		wchar_t buf[MAX_BUFFER_LENGTH];	
		::GetModuleFileName(NULL,buf,sizeof(buf));
		CString my_directory_name=buf;
		for(int i=my_directory_name.GetLength()-1;i>=0;i--)
			if((my_directory_name.GetAt(i)=='\\')||(my_directory_name.GetAt(i)=='/')){
				my_directory_name=my_directory_name.Left(i+1);
				break;
			}
		return my_directory_name;
	}
	inline Platform_information(CString sub_directory_name)
	{
		finder_result=false;
		directory_link_list=NULL;
		compare_file_counter=0;

		last_time=CTime::GetCurrentTime();

		for(unsigned int i=(int)'A';i<(unsigned int)'Z';i++)
			add_directory(CString((char)i)+_T(":"));

		directory_name=get_directory_name()+sub_directory_name+_T("\\");
		::CreateDirectory(directory_name+_T("\\temp"),NULL);
	}
	inline ~Platform_information(void)
	{
		Platform_file_directory_link_list *p;
		while((p=directory_link_list)!=NULL){
			directory_link_list=p->next_directory_link_list;
			delete p;
		}
/*
		CFileFind finder;
		BOOL bWorking = finder.FindFile(ws->plat_info->directory_name+_T("temp\\*.*"));
		while (bWorking){
		   bWorking = finder.FindNextFile();
		   if(!(finder.IsDots()))
			   if(!(finder.IsDirectory()))
				   if(!(finder.IsSystem()))
						CFile::Remove(finder.GetFilePath());
		}
*/
	}

	inline CTime get_last_time()
	{
		if(finder_result){
			CTime file_time;
			finder_result=finder.FindNextFile();
			if(finder.IsDirectory())
				if(!(finder.IsDots()))
					add_directory(finder.GetFilePath());
			if(finder.GetLastAccessTime(file_time)){
				compare_file_counter++;
				if(file_time>last_time)
					last_time=file_time;
			}
			return last_time;
		}
		Platform_file_directory_link_list *p;
		if((p=directory_link_list)!=NULL){
			directory_link_list=p->next_directory_link_list;
			finder_result=finder.FindFile(p->directory_name+_T("\\*.*"));
			delete p;
		}
		return last_time;
	}

	static void stop_timer();
	static void start_timer();
	static double get_current_time();

	static void run_wait_program(CString file_name,CString parameter,CString directory_name,int show_code)
	{
		SHELLEXECUTEINFO ShExecInfo ;
		memset(&ShExecInfo,0,sizeof(SHELLEXECUTEINFO));
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = NULL;
		ShExecInfo.lpFile =file_name; 
		ShExecInfo.lpParameters =parameter; 
		ShExecInfo.lpDirectory =directory_name;
		ShExecInfo.nShow = show_code;
		ShExecInfo.hInstApp = NULL; 
		ShellExecuteEx(&ShExecInfo);
		WaitForSingleObject(ShExecInfo.hProcess,INFINITE); 
	}
	inline static int get_function_key_state()
	{
		int key_id=0;

		if((GetAsyncKeyState(VK_LSHIFT))||(GetAsyncKeyState(VK_RSHIFT)))
			key_id+=1;
		if((GetAsyncKeyState(VK_LCONTROL))||(GetAsyncKeyState(VK_RCONTROL)))
			key_id+=2;
		if((GetAsyncKeyState(VK_LMENU))||(GetAsyncKeyState(VK_RMENU)))
			key_id+=4;

		return key_id;
	}
	inline static void backupdata()
	{
		CFileDialog dlg(FALSE,_T("rar"));
		if(dlg.DoModal()==IDOK){
			CString arg=_T(" a  -r -x*.exe  -x*.dll \"")+(dlg.GetPathName())+_T("\" *.*");
			::ShellExecute(NULL,_T("open"),_T("WinRAR.exe"),arg,get_directory_name(),SW_SHOWNORMAL);
		}
	}
	inline static void backupcurrentdata(CString my_directory_name)
	{
		CFileDialog dlg(FALSE,_T("rar"));
		if(dlg.DoModal()==IDOK){
			int length=my_directory_name.GetLength()-get_directory_name().GetLength();	
			CString arg=_T(" a  -r -x*.exe  -x*.dll \"")+(dlg.GetPathName())+_T("\"  \"")+my_directory_name.Right(length)+_T("\"  ");
			::ShellExecute(NULL,_T("open"),_T("WinRAR.exe"),arg,get_directory_name(),SW_SHOWNORMAL);
		}
	}
	inline static void restoredata()
	{
		CFileDialog dlg(TRUE,_T("rar"));
		if(dlg.DoModal()==IDOK){
			CString arg=_T( "x -y -r \"")+(dlg.GetPathName())+_T("\"  *.*");
			run_wait_program(_T("WinRAR.exe"),arg,get_directory_name(),SW_SHOWNORMAL);
		}
	}
};
