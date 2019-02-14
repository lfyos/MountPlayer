#include "StdAfx.h"
#include "const.h"
#include "part_face.h"
#include "part_body.h"
part_body::part_body(ifstream *f)
{
	int i;
	CString str;
	char tmp[MAX_MESSAGE_LENGTH];

	*f>>tmp>>tmp;
	body_name=tmp;

	material.load(f);

	*f>>tmp>>face_count;

	if(face_count<=0){
		face_count=0;
		face=NULL;
	}else{
		face=new part_face*[face_count];
		for(i=0;i<face_count;i++)
			face[i]=new part_face(f);
	}	
	return;
}

part_body::~part_body()
{
	int i;
	if(face_count<=0)
		return;
	for(i=0;i<face_count;i++)
		delete (face[i]);
	delete []face;
	return;
}