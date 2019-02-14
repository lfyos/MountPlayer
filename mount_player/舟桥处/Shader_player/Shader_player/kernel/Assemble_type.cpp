#include "StdAfx.h"
#include "Assemble_type.h"

void Assemble_type::init(ifstream &f,Assemble_type *my_parent)
{
	char buffer[MAX_BUFFER_LENGTH];
	int child_number;

	child=NULL;
	left_brother=this;
	right_brother=this;

	if((parent=my_parent)!=NULL){
		if(parent->child==NULL)
			parent->child=this;
		else{
			left_brother=parent->child;
			right_brother=left_brother->right_brother;
			left_brother->right_brother=this;
			right_brother->left_brother=this;
		}
	}

	child_number=0;
	list_id=NULL;
	f>>buffer>>child_number;
	caption_name=buffer;

	if(child_number<=0){		
		
		f>>buffer;
		part_name=buffer;
		f>>buffer;
		assemble_name=buffer;
		f>>buffer;
		movement_name=buffer;

		child_number=0;
	}else{
		assemble_name=_T("");
		movement_name=_T("");
		for(int i=0;i<child_number;i++)
			new Assemble_type(directory_name,f,this);
	}
}

Assemble_type::~Assemble_type(void)
{
	while(child!=NULL)
		delete child;
	if(parent!=NULL){
		if(left_brother==this)
			parent->child=NULL;
		else{
			if(parent->child==this)
				parent->child=right_brother;
			left_brother->right_brother=right_brother;
			right_brother->left_brother=left_brother;
		}
	}
}
