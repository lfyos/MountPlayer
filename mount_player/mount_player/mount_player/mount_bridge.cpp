#include "StdAfx.h"
#include "mount_bridge.h"
#include "const.h"

#include <fstream>
using namespace std;

#include "math.h"
#include "part_face.h"
#include "part_tree.h"
#include "part_info.h"
#include "part_body.h"
#include "part_image.h"


mount_bridge::mount_bridge(CString directory_name,CString structure_file_name,CString information_file_name,CString sort_name,double min_value)
{
	int i;
	ifstream f(directory_name+structure_file_name);	
	char buf[MAX_MESSAGE_LENGTH];
	
	part_number=0;
	image_number=0;
	part=NULL;
	tree=NULL;
	first_part_tree=NULL;
	part_index=NULL;

	if(f.fail()){
		AfxMessageBox(_T("打开文件: "+directory_name+structure_file_name+" 失败!"));
		return;
	}
	tree =new part_tree(&f,NULL);

	f>>buf>>part_number;
	part=new part_info *[part_number];
	
	for(i=0;i<part_number;i++)
		part[i]=new part_info(&f,directory_name+information_file_name);

	f>>buf>>buf;
	f.close();

	tree->reset_relative_location();
	tree->caculate_box(part);
		
	set_reference(get_sort_id_from_name(sort_name),min_value);
	tree->sort_tree_node(get_sort_id_from_name(sort_name),min_value);

	initialize_images(directory_name);
	
	directory_name_length=get_system_tree_directory(tree,get_system_part_directory()).GetLength();
		
	return;
}
mount_bridge::~mount_bridge()
{
	int i;
	if(tree!=NULL)
		delete tree;

	if((part_number>0)&&(part!=NULL)){
		for(i=0;i<part_number;i++)
			delete part[i];	
		delete []part;
	}

	for(i=0;i<image_number;i++){
		delete images[i];
		images[i]=NULL;
	}
	if(first_part_tree!=NULL)
		delete []first_part_tree;
	if(part_index!=NULL)
		delete []part_index;

	part_number=0;
	image_number=0;
	part=NULL;
	tree=NULL;
	first_part_tree=NULL;
	part_index=NULL;

	return;
}
int mount_bridge::create_call_list(int index_id,int start_call_list_id,graphics_system *gs)
{
	int i,call_list_id;
		
	for(i=0;i<image_number;i++)
		images[i]->create_texture_object(index_id,gs);

	for(i=0,call_list_id=start_call_list_id;i<part_number;i++)
		call_list_id=part[i]->create_call_list(index_id,call_list_id,images,gs);

	return call_list_id;
}
int mount_bridge::register_image_file_name(CString directory_name,CString file_name)
{
	int i;

	for(i=0;i<image_number;i++)
		if(images[i]->image_file_name==file_name)
			return i;

	if(image_number>=sizeof(images)/sizeof(images[0]))
		return -1;
	images[image_number]=new part_image(directory_name+"..\\"+file_name);
	if(images[image_number]->is_valid()){
		image_number++;
		return (image_number-1);
	}
	delete (images[image_number]);
	images[image_number]=NULL;

	return -1;
}
void mount_bridge::initialize_images(CString directory_name)
{
	int i,j,k;
	CString file_name;

	for(image_number=0,i=0;i<sizeof(images)/sizeof(images[0]);i++)
		images[i]=NULL;

	for(i=0;i<part_number;i++){
		if(part[i]->material.texture_flag)
			part[i]->material.texture_image_id=register_image_file_name(directory_name,part[i]->material.texture_name);
		else 
			part[i]->material.texture_image_id=-1;
		for(j=0;j<part[i]->body_count;j++){
			if(part[i]->body[j]->material.texture_flag)
				part[i]->body[j]->material.texture_image_id=register_image_file_name(directory_name,part[i]->body[j]->material.texture_name);
			else
				part[i]->body[j]->material.texture_image_id=part[i]->material.texture_image_id;
			for(k=0;k<part[i]->body[j]->face_count;k++){
				if(part[i]->body[j]->face[k]->material.texture_flag)
					part[i]->body[j]->face[k]->material.texture_image_id=register_image_file_name(directory_name,part[i]->body[j]->face[k]->material.texture_name);
				else
					part[i]->body[j]->face[k]->material.texture_image_id=part[i]->body[j]->material.texture_image_id;
			}
		}
	}
	return;
}
void mount_bridge::set_reference(int sort_type_id,double min_value)
{
	int i,j,k,n,*part_component_number;
	part_tree *t,**tree_array;

	part_index=new int [part_number];
	for(i=0;i<part_number;i++)
		part_index[i]=i;
	for(i=0;i<(part_number-1);i++)
		for(j=i+1;j<part_number;j++)
			if(part[part_index[i]]->information.name>part[part_index[j]]->information.name){
				k=part_index[i];
				part_index[i]=part_index[j];
				part_index[j]=k;
			}

	first_part_tree=new part_tree *[part_number];
	part_component_number=new int[part_number];

	for(i=0;i<part_number;i++){
		first_part_tree[i]=NULL;
		part_component_number[i]=0;		
	}
	tree->set_part_reference(first_part_tree,part_component_number);

	for(i=0;i<part_number;i++)
		if((t=first_part_tree[i])!=NULL){
			n=part_component_number[i];
			tree_array=new part_tree *[n];
			for(j=0;j<n;j++,t=t->tree_node_union.part.back_part)
				tree_array[j]=t;
			for(j=0;j<(n-1);j++)
				for(k=j+1;k<n;k++)
					if(tree_array[j]->compare_tree_node(tree_array[k],sort_type_id,min_value)){
						t=tree_array[j];
						tree_array[j]=tree_array[k];
						tree_array[k]=t;
					}
			for(j=0;j<n;j++){
				tree_array[j]->tree_node_union.part.back_part =tree_array[(j+1+n)%n];
				tree_array[j]->tree_node_union.part.front_part=tree_array[(j-1+n)%n];
				tree_array[j]->tree_node_union.part.part_component_id=j;
				tree_array[j]->tree_node_union.part.part_component_number=n;
			}
			first_part_tree[i]=tree_array[0];
			delete []tree_array;
		}
	delete []part_component_number;
	return;
}
int mount_bridge::get_sort_id_from_name(CString sort_name)
{
	int sort_type_id;
		
	if((sort_name=="xyz")||(sort_name=="XYZ"))
		sort_type_id=0;
	else if((sort_name=="xzy")||(sort_name=="XZY"))
		sort_type_id=1;
	else if((sort_name=="yxz")||(sort_name=="YXZ"))
		sort_type_id=2;
	else if((sort_name=="yzx")||(sort_name=="YZX"))
		sort_type_id=3;
	else if((sort_name=="zxy")||(sort_name=="ZXY"))
		sort_type_id=4;
	else if((sort_name=="zyx")||(sort_name=="ZYX"))
		sort_type_id=5;
	else 
		sort_type_id=0;
	return sort_type_id;
}
void mount_bridge::only_display_us(part_tree *buffer[],int max_number)
{
	int i,j,n;
	part_tree *p,*q;

	for(i=0;i<part_number;i++){
		p=first_part_tree[i];
		n=p->tree_node_union.part.part_component_number;
		for(j=0;j<n;j++,p=p->tree_node_union.part.back_part)
			for(q=p;q!=NULL;q=q->parent)
				q->set_visible_flag((q->type!=0)?TRUE:FALSE);
	}
	for(i=0;i<max_number;i++)
		buffer[i]->set_visible_flag(TRUE,TRUE);
	return;
}
int mount_bridge::display(int index_id,int display_method,
		part_tree *selected_tree,graphics_system *gs,
		mount_material_property *selected_material_property,
		double *clip_plane_data,
		double eye_x,double eye_y,double eye_z,
		double center_x,double center_y,double center_z,
		double mount_speeed,BOOL my_mark_flag,
		double manual_mount_proportion,
		BOOL display_reference_frame_flag)
{
	int ret_val;
	ret_val=tree->display(index_id,display_method,part_number,part,
		selected_tree,gs,selected_material_property,
		clip_plane_data,eye_x,eye_y,eye_z,center_x,center_y,center_z,
		mount_speeed,my_mark_flag,manual_mount_proportion,FALSE,images,
		display_reference_frame_flag);
	switch(display_method){
	case USER_SELECTED_PART_DISPLAY_METHOD:
	case PART_BEING_SELECTED_DISPLAY_METHOD:
		return ret_val;
	case MARK_PART_DISPLAY_METHOD:
	default:	
		return ret_val+tree->display(index_id,display_method,part_number,part,
			selected_tree,gs,selected_material_property,
			clip_plane_data,eye_x,eye_y,eye_z,center_x,center_y,center_z,
			mount_speeed,my_mark_flag,manual_mount_proportion,TRUE,images,
			display_reference_frame_flag);
	}
}

CString mount_bridge::get_system_part_directory()
{
	int i,j,length;
	CString str1,str2;
	
	str1=part[0]->part_file_name;
	length=str1.GetLength();
	for(i=1;i<part_number;i++){
		str2=part[i]->part_file_name;
		if((j=str2.GetLength())<length)
			length=j;
		for(;length>0;length--){
			if((str1.Left(length))==(str2.Left(length)))
				break;
		}
	}
	return str1.Left(length);
}
CString mount_bridge::get_system_tree_directory(part_tree *t,CString str)
{
	int i,n,length1,length2,length;
	part_tree *p;

	if(t->parent!=NULL){
		length1=str.GetLength();
		length2=t->tree_file_name.GetLength();
		if(length1<length2)
			length=length1;
		else
			length=length2;
		for(;;length--){
			if(length<=0)
				return "";
			if((str.Left(length))==(t->tree_file_name.Left(length)))
				break;
		}
		str=str.Left(length);
	}
	if(t->type!=0){
		p=t->tree_node_union.assembly.first_child;
		n=t->tree_node_union.assembly.child_number;	
		for(i=0;i<n;i++,p=p->back_brother)
			str=get_system_tree_directory(p,str);
	}
	return str;
}
void mount_bridge::compute_tree_nodes_box(int max_buffer_number, part_tree *buffer[], double  mark_tree_node_box[])
{
	int i;
	if(max_buffer_number>0){	
		for(i=0;i<(sizeof(buffer[0]->box)/sizeof(buffer[0]->box[0]));i++)
			mark_tree_node_box[i]=buffer[0]->box[i];

		for(i=1;i<max_buffer_number;i++){
			mark_tree_node_box[0]=((buffer[i]->box[0])<(mark_tree_node_box[0]))?(buffer[i]->box[0]):(mark_tree_node_box[0]);
			mark_tree_node_box[1]=((buffer[i]->box[1])<(mark_tree_node_box[1]))?(buffer[i]->box[1]):(mark_tree_node_box[1]);
			mark_tree_node_box[2]=((buffer[i]->box[2])<(mark_tree_node_box[2]))?(buffer[i]->box[2]):(mark_tree_node_box[2]);

			mark_tree_node_box[3]=((buffer[i]->box[3])>(mark_tree_node_box[3]))?(buffer[i]->box[3]):(mark_tree_node_box[3]);
			mark_tree_node_box[4]=((buffer[i]->box[4])>(mark_tree_node_box[4]))?(buffer[i]->box[4]):(mark_tree_node_box[4]);
			mark_tree_node_box[5]=((buffer[i]->box[5])>(mark_tree_node_box[5]))?(buffer[i]->box[5]):(mark_tree_node_box[5]);

		}
	}
}
int mount_bridge::collect_mark_tree_nodes(int max_buffer_number, part_tree *buffer[], double mark_tree_node_box[])
{
	if(max_buffer_number>0)
		if((max_buffer_number=max_buffer_number-(tree->collect_mark_tree_nodes(max_buffer_number,buffer)))>0)
			compute_tree_nodes_box(max_buffer_number,buffer,mark_tree_node_box);
	return max_buffer_number;
}

int mount_bridge::collect_visible_tree_nodes(int max_buffer_number, part_tree *buffer[], double mark_tree_node_box[])
{
	if(max_buffer_number>0)
		if((max_buffer_number=max_buffer_number-(tree->collect_visible_tree_nodes(max_buffer_number,buffer)))>0)
			compute_tree_nodes_box(max_buffer_number,buffer,mark_tree_node_box);
	return max_buffer_number;
}
