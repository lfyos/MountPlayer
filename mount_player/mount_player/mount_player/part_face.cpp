#include "StdAfx.h"
#include "part_face.h"
#include "const.h"
#include "graphics_system.h"
#include <math.h>

part_face::part_face(ifstream *f)
{
	int i,j;
	char tmp[MAX_MESSAGE_LENGTH];
	CString str;

	*f>>tmp>>tmp;
	face_name=tmp;

	material.load(f);

	*f>>tmp>>tesstrianglecount;

	if(tesstrianglecount<=0){
		tesstrianglecount=0;
		tesstriangles=NULL;
		tessnorms=NULL;
		tesstexture=NULL;
	}else{

		tesstriangles=new double[9*tesstrianglecount];
		tessnorms=new double[9*tesstrianglecount];
		tesstexture=NULL;

		*f>>tmp>>tmp;
		if((str=tmp)=="YES"){	
			tesstexture=new double[6*tesstrianglecount];
			for(i=0;i<tesstrianglecount;i++){
				*f>>tmp>>tmp;
				for(j=0;j<6;j++)
					(*f)>>tesstexture[j+6*i];
			}
		}
		for(i=0;i<tesstrianglecount;i++){
			*f>>tmp>>tmp;
			for(j=0;j<9;j++)
				(*f)>>tesstriangles[j+9*i];
			*f>>tmp>>tmp;
			for(j=0;j<9;j++)
				(*f)>>tessnorms[j+9*i];
		}
		for(i=0;i<(9*tesstrianglecount);i++,i++,i++){
			double a,b,c,d;
			a=tessnorms[i+0];			b=tessnorms[i+1];			c=tessnorms[i+2];
			d=sqrtl(a*a+b*b+c*c);
			tessnorms[i+0]=a/d;			tessnorms[i+1]=b/d;			tessnorms[i+2]=c/d;
		}

	}
	return;
}

part_face::~part_face()
{
	if(tesstrianglecount>0){
		delete []tesstriangles;
		delete []tessnorms;
		if(tesstexture!=NULL)
			delete []tesstexture;
	}
}
int part_face::create_call_list(int index_id,int start_call_list_id,graphics_system *gs)
{
	gs->specify_face_point(FALSE,tesstriangles,tessnorms,tesstexture);

	gs->begin_create_call_list(start_call_list_id);	
	gs->display_face(tesstrianglecount);
	gs->end_create_call_list();
	no_texture_call_list_id[index_id]=start_call_list_id;
	start_call_list_id++;

	if(tesstexture==NULL)
		texture_call_list_id[index_id]=no_texture_call_list_id[index_id];
	else{
		gs->specify_face_point(TRUE,tesstriangles,tessnorms,tesstexture);

		gs->begin_create_call_list(start_call_list_id);		
		gs->display_face(tesstrianglecount);
		gs->end_create_call_list();
		texture_call_list_id[index_id]=start_call_list_id;
		start_call_list_id++;
	}
	return start_call_list_id;
}