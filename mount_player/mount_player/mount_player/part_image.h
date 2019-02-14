#pragma once
#include "stdafx.h"
#include "const.h"

class graphics_system;
struct CvCapture;

class part_image  
{
	int width,height;
	float *image_data;
	CvCapture *capture;	
	int texture_object_id[MAX_CALL_LIST_INDEX_NUMBER];
public:
	CString image_file_name;

	BOOL is_valid()
	{
		return (image_data!=NULL);
	}
	int get_texture_object_id(int my_id)
	{
		return ((my_id<0)||(my_id>=MAX_CALL_LIST_INDEX_NUMBER))?-1:texture_object_id[my_id];
	}
	part_image(CString file_name);
	virtual ~part_image();

	void create_texture_object(int index_id,graphics_system *gs);
	void delete_texture_object(int index_id,graphics_system *gs);
	BOOL process_picture(int index_id,graphics_system *gs);
private:
	void get_data(void *image);
};
