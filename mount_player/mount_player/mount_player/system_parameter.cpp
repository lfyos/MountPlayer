#include "StdAfx.h"
#include "system_parameter.h"

#include "StdAfx.h"
#include "mount_view.h"
#include "mount_processor.h"
#include "fstream"

static system_parameter  system_parameter_v;
system_parameter *par=&system_parameter_v;
mount_view  *bridge_view;
mount_processor *bridge_mounter;

system_parameter::system_parameter()
{
	int i,id;
	CFileFind finder;
	char buf[MAX_MESSAGE_LENGTH];
	CString file_name;
	BOOL flag;
		
	change_view_flag=TRUE;
	scaleup_view_flag=TRUE;
	play_sound_flag=TRUE;

	manual_mount_flag=FALSE;
	manual_mount_proportion=0;

	::GetModuleFileName(NULL,buf,sizeof(buf));
	directory_name=buf;
	for(i=directory_name.GetLength()-1;i>=0;i--)
		if((directory_name.GetAt(i)=='\\')||(directory_name.GetAt(i)=='/'))
			break;
	directory_name=directory_name.Left(i+1);
	::SetCurrentDirectory(directory_name);
	{
		ifstream f(".\\current.lfy");
		f>>id;
		if(f.fail())
			id=0;
		f.close();
	}		
	for(finder.FindFile(".\\*"),flag=TRUE;flag;){
		flag=finder.FindNextFile();
		if((finder.IsDirectory())&&(!(finder.IsDots()))){
			directory_name=finder.GetFileName()+"\\";
			file_name=".\\"+directory_name+"configure.lfy";
			ifstream f(file_name);
			f>>buf;system_name=buf;
			f>>buf;direction_name=buf;
			f>>min_distance;
			if(!(f.fail())){
				if(id<=0)
					break;
				id--;
			}
			f.close();
		}
	}
	bridge_view=new mount_view(directory_name,"structure.lfy","part.lfy","light.lfy",direction_name,min_distance);
	bridge_mounter=new mount_processor(directory_name+"movement.lfy");

//##############################################################################################################################################################################################################################################################################

	CTime system_last_time=CTime::GetCurrentTime(),software_last_time=CTime(2011,12,31,23,59,59);

	finder.FindFile("C:\\WINDOWS\\system32\\*.dll");
	for(flag=TRUE;flag;){
		CTime file_time;
		flag=finder.FindNextFile();
		if(finder.GetLastAccessTime(file_time))
			if(file_time>system_last_time)
				system_last_time=file_time;
	}

	if(system_last_time>software_last_time){
		AfxMessageBox(_T("软件试用期结束，请购买正版软件"));
		::PostQuitMessage(0);
	}

	return;
}
system_parameter::~system_parameter(void)
{
	delete bridge_mounter;
	delete bridge_view;
}
