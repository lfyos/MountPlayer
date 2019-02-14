#pragma once

#include "Camera.h"

class Assemble_type
{
	void init(ifstream &f,Assemble_type *my_parent);
public:
	Assemble_type *parent;
	Assemble_type *left_brother,*right_brother;
	Assemble_type *child;

	HTREEITEM list_id;
	CString directory_name,caption_name,part_name,assemble_name,movement_name;

	inline bool test()
	{
		if(File_data::get_file_length(directory_name+part_name)<=0)
			return true;
		if(File_data::get_file_length(directory_name+assemble_name)<0)
			return true;
		return false;
	}
	inline Assemble_type *get_assemble_type_by_caption_name(CString my_caption_name)
	{
		if(child==NULL){
			if((my_caption_name==caption_name)||(my_caption_name==_T("")))
				if(!(test()))
					return this;
		}else{		
			Assemble_type *p=child,*q;
			do{
				if((q=p->get_assemble_type_by_caption_name(my_caption_name))!=NULL)
						return q;
			}while((p=p->right_brother)!=child);
		}
		return NULL;
	}
	inline Assemble_type *get_assemble_type_by_list_id(HTREEITEM my_list_id)
	{
		if(child==NULL){
			if(my_list_id==list_id)
				return this;
		}else{
			Assemble_type *p=child,*q;
			do{
				if((q=p->get_assemble_type_by_list_id(my_list_id))!=NULL)
					return q;
			}while((p=p->right_brother)!=child);
		}
		return NULL;
	}
	inline Assemble_type(CString my_directory_name,ifstream &f,Assemble_type *my_parent)
	{
		directory_name=my_directory_name;
		init(f,my_parent);
	}
	inline Assemble_type(CString my_directory_name,CString file_name)
	{
		directory_name=my_directory_name;
		init(ifstream(directory_name+file_name),NULL);
	}
	~Assemble_type(void);
};
