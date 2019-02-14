#pragma once

#include "Select_component_ids.h"

class Light{
public:
	int mount_camera_id,shadow_texture_unit_id;

	int shadow_framebuffer_object_id,shadow_renderbuffer_object_id,shadow_texture_object_id;

	double shadow_texture_precision;
	Location light_matrix,projection_matrix;

	inline void create_depth_texture_and_frame_buffer(int width,int height)
	{
		Graphics_system::create_depth_texture_and_frame_buffer(&shadow_framebuffer_object_id,&shadow_renderbuffer_object_id,&shadow_texture_object_id,width,height);
	}
	inline void get_depth_texture_image(int width,int height)
	{
		Graphics_system::get_depth_texture_image(shadow_texture_object_id,width,height);
	}
	inline void delete_depth_texture_and_frame_buffer()
	{
		Graphics_system::delete_framebuffer_renderbuffer_and_texture(&shadow_framebuffer_object_id,&shadow_renderbuffer_object_id,&shadow_texture_object_id);
	}
	inline void load(ifstream &f)
	{
		char buffer[MAX_BUFFER_LENGTH];		
		f>>buffer>>mount_camera_id;
		f>>buffer>>shadow_texture_unit_id;
		f>>buffer>>shadow_texture_precision;
	}
	inline Light()
	{
		shadow_framebuffer_object_id=0;
		shadow_renderbuffer_object_id=0;
		shadow_texture_object_id=0;
	}
	inline ~Light()
	{
		delete_depth_texture_and_frame_buffer();
	}
};

class Light_set{
public:
	int light_number;
	Light light_array[MAX_LIGHT_NUMBER];
		
	CString file_name;

	inline void load()
	{
		ifstream f(file_name);

		for(light_number=0;light_number<MAX_LIGHT_NUMBER;light_number++){
			light_array[light_number].load(f);
			if((f.eof())||(f.fail())){
				f.close();
				return;
			}
		}
		f.close();
		return;
	}
	inline Light_set(CString my_file_name)
	{
		file_name=my_file_name;
		load();
	}
};

