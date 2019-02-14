#include "StdAfx.h"
#include "Texture.h"

#include "cxcore.h"
#include "cv.h"
#include "cvaux.h"
#include "highgui.h"
#include "cv.h"

#pragma comment( lib,"cv200.lib")
#pragma comment( lib,"cvaux200.lib")
#pragma comment( lib,"cvhaartraining.lib")
#pragma comment( lib,"cxcore200.lib")
#pragma comment( lib,"cxts200.lib")
#pragma comment( lib,"highgui200.lib")
#pragma comment( lib,"ml200.lib")
#pragma comment( lib,"opencv_ffmpeg200.lib")

void Texture::get_data_from_image(void *image)
{
	int i,j;
	float *p,depth;
	IplImage *my_image=(IplImage *)image;

	if(image_data!=NULL){
		if((height!=my_image->height)||(width!=my_image->width)){
			delete []image_data;
			image_data=NULL;
		}
	}
	if(image_data==NULL){
		height=my_image->height;
		width=my_image->width;
		image_data=new float[4*height*width];
	}
	switch(my_image->depth){
	case IPL_DEPTH_8U:
		depth=256.0-1.0;
		break;
	case IPL_DEPTH_8S:
		depth=128.0;
		break;
	case IPL_DEPTH_16U:
		depth=256.0*256.0-1.0;
		break;
	case IPL_DEPTH_16S:
		depth=128.0*256.0;
		break;
	case IPL_DEPTH_32S:
		depth=(float)(128.0*256.0*256.0*256.0);
		break;
	default:
		depth=1.0;
		break;
	}
	for(i=0,p=image_data;i<height;i++){
		for(j=0;j<width;j++){
			CvScalar s(cvGet2D(my_image,i,j));
			*p++=(float)(s.val[2])/depth;
			*p++=(float)(s.val[1])/depth;
			*p++=(float)(s.val[0])/depth;
			*p++=(float)1;
		}
	}
	return;
}
bool Texture::video_process()
{
	IplImage *original_image;
	
	if(capture==NULL)
		return false;
	if((original_image=cvQueryFrame(capture))==NULL)
		return false;
	get_data_from_image(original_image);
	if(cvGetCaptureProperty(capture,CV_CAP_PROP_POS_FRAMES)>=cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_COUNT))
		cvSetCaptureProperty(capture,CV_CAP_PROP_POS_FRAMES,0.0);
	return true;
}
Texture::Texture()
{
	image_data=NULL;
	capture=NULL;
	width=0;
	height=0;		
	return;
}
Texture::~Texture(void)
{
	if(capture!=NULL)
		cvReleaseCapture(&capture);
	if(image_data!=NULL)
		delete []image_data;
}
float * Texture::load_from_file(CString my_image_file_name,int *my_width,int *my_height)
{
	int i;
	IplImage *original_image;

	if(capture!=NULL){
		cvReleaseCapture(&capture);
		capture=NULL;
	}
	if(image_data!=NULL){
		delete []image_data;
		image_data=NULL;
	}
	width=0;
	height=0;

	image_file_name=my_image_file_name;

	BSTR   bstrText   =   my_image_file_name.AllocSysString();
	char*  lpszText2   =   _com_util::ConvertBSTRToString(bstrText);     
	original_image=cvLoadImage(lpszText2);

	if(original_image!=NULL){
		get_data_from_image(original_image);
		cvReleaseImage(&original_image);

		SysFreeString(bstrText);
		delete []lpszText2;

		return get_image_data(my_width,my_height);
	}
	capture=cvCaptureFromFile(lpszText2);
	SysFreeString(bstrText);
	delete []lpszText2;

	if(capture==NULL){
		const char *camera_name[]={	"0",		"1",		"2",		"3",		"4",		"5",		"6",		"7",		"8",		"9",	"10"};
		CString ext_str=my_image_file_name;
		for(i=my_image_file_name.GetLength()-1;i>=0;i--)
			if((my_image_file_name.GetAt(i)==CString('/'))||(my_image_file_name.GetAt(i)==CString('\\'))){
				ext_str=my_image_file_name.Right(my_image_file_name.GetLength()-1-i);
				break;
			}
		for(i=0;i<sizeof(camera_name)/sizeof(camera_name[0]);i++){
			if(ext_str==CString(camera_name[i]))
				break;
		}		
		if(i>=sizeof(camera_name)/sizeof(camera_name[0]))
			return get_image_data(my_width,my_height);
		if((capture=cvCaptureFromCAM(i))==NULL)
			return get_image_data(my_width,my_height);
	}
	if((original_image=cvQueryFrame(capture))!=NULL){
		get_data_from_image(original_image);
	}else{
		cvReleaseCapture(&capture);
		capture=NULL;
	}	
	return get_image_data(my_width,my_height);
}
