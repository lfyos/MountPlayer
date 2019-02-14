#pragma once
#include "Graphics_system.h"

struct CvCapture;

class Texture
{
	int width,height;
	float *image_data;
	CvCapture *capture;

	void get_data_from_image(void *image);
public:
	CString image_file_name;
	float *get_image_data(int *my_width=NULL,int *my_height=NULL)
	{
		if(my_width!=NULL)
			*my_width=width;
		if(my_height!=NULL)
			*my_height=height;
		return image_data;
	}
	bool video_process();
	Texture();
	virtual ~Texture(void);
	float *load_from_file(CString my_image_file_name,int *my_width=NULL,int *my_height=NULL);
};
