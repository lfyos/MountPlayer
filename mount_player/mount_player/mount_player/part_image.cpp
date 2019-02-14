#include "StdAfx.h"
#include "part_image.h"

#include "cxcore.h"
#include "cv.h"
#include "cvaux.h"
#include "highgui.h"
#include "cv.h"

#pragma comment( lib,"cxcore200.lib")
#pragma comment( lib,"cv200.lib")
#pragma comment( lib,"cvaux200.lib")
#pragma comment( lib,"highgui200.lib")

#include "graphics_system.h"

part_image::~part_image()
{
	if(capture!=NULL)
		cvReleaseCapture(&capture);
	if(image_data!=NULL)
		delete []image_data;
	image_data=NULL;
	capture=NULL;
	width=0;
	height=0;
}
part_image::part_image(CString file_name)
{
	int i;
	IplImage *original_image;
	
	image_data=NULL;
	capture=NULL;
	width=0;
	height=0;

	image_file_name=file_name;

	for(i=0;i<sizeof(texture_object_id)/sizeof(texture_object_id[0]);i++)
		texture_object_id[i]=-1;

	original_image=cvLoadImage(file_name.GetBuffer());
	file_name.ReleaseBuffer();
	if(original_image!=NULL){
		get_data(original_image);
		cvReleaseImage(&original_image);
		return ;
	}
	capture=cvCaptureFromFile(file_name.GetBuffer());//如果打不开avi文件，请安装解码软件
	file_name.ReleaseBuffer();

	if(capture==NULL){
		const char *camera_name[]={
					"0",		"1",		"2",		"3",		"4",
					"5",		"6",		"7",		"8",		"9",
		};
		CString ext_str=file_name;
		for(i=file_name.GetLength()-1;i>=0;i--)
			if((file_name.GetAt(i)=='/')||(file_name.GetAt(i)=='\\')){
				ext_str=file_name.Right(file_name.GetLength()-1-i);
				break;
			}
		for(i=0;i<sizeof(camera_name)/sizeof(camera_name[0]);i++)
			if(ext_str==camera_name[i])
				break;
		if(i>=sizeof(camera_name)/sizeof(camera_name[0]))
			return ;
		if((capture=cvCaptureFromCAM(i))==NULL)
			return ;
	}
	if((original_image=cvQueryFrame(capture))==NULL){
		cvReleaseCapture(&capture);
		capture=NULL;
		return ;
	}	
	get_data(original_image);
	return;
}
void part_image::delete_texture_object(int index_id,graphics_system *gs)
{
	if(texture_object_id[index_id]>0)
		gs->unregister_texure_object(texture_object_id[index_id]);
	texture_object_id[index_id]=-1;
}
void part_image::create_texture_object(int index_id,graphics_system *gs)
{
	delete_texture_object(index_id,gs);
	if(is_valid())
		texture_object_id[index_id]=gs->register_texure_object(width,height,image_data,(capture==NULL)?TRUE:FALSE);
	return;
}
BOOL part_image::process_picture(int index_id,graphics_system *gs)
{
	IplImage *original_image;
	if(capture==NULL)
		return FALSE;
	if(index_id==0){
		if(cvGetCaptureProperty(capture,CV_CAP_PROP_POS_AVI_RATIO)>=1.0)
			cvSetCaptureProperty(capture,CV_CAP_PROP_POS_AVI_RATIO,0.0);
		if((original_image=cvQueryFrame(capture))==NULL)
			return FALSE;
		get_data(original_image);
	}
	if(texture_object_id[index_id]<0)
		return FALSE;
	gs->replace_texure_object(texture_object_id[index_id],width,height,image_data);
	return TRUE;
}
void part_image::get_data(void *image)
{
	int i,j;
	float *p;
	IplImage *my_image;
	
	my_image=(IplImage *)image;
	if(is_valid()){
		if((height!=my_image->height)||(width!=my_image->width)){
			delete []image_data;
			image_data=NULL;
		}
	}
	if(!is_valid()){
		height=my_image->height;
		width=my_image->width;
		image_data=new float[4*height*width];
	}
	for(i=0,p=image_data;i<height;i++){
		for(j=0;j<width;j++){
			*p++=(float)(CV_IMAGE_ELEM(my_image, uchar, i, 3*j+2))/255;
			*p++=(float)(CV_IMAGE_ELEM(my_image, uchar, i, 3*j+1))/255;
			*p++=(float)(CV_IMAGE_ELEM(my_image, uchar, i, 3*j+0))/255;
			*p++=(float)1;
		}
	}
}