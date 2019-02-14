#pragma once

#include "Component_collector.h"

class Whole_system
{	
	void list_not_find_component(Component *comp,ofstream &f)
	{
		if(comp==NULL)
			return;

		Component *p=comp->child;
		if(p!=NULL){
			do {
					list_not_find_component(p,f);
			}while((p=p->left_brother)!=(comp->child));
			return;
		}
		if((comp->part_id<0)||((comp->part_id)>=(parts->part_number))){
			CString str;
			str.Format(_T("				%d\n"),comp->part_id);
			str=comp->name+str;
			str=_T("				")+str;
			str=comp->id_name+str;
			File_data::output_string(f,str);
		}
		return;
	}
	void init_part(ifstream &f_part_file,CString merge_file_name);
	void init_component(ifstream &f);
	void init_camera();
	void save_camera();

	void allocate_component_model_maxtrix_memory();
public:
	bool exit_flag;

	int system_max_user_parameter_number,release_memory_flag_map;
	double offset_factor,offset_units;

	Assemble_type *assemble_type,*current_assemble_type;
	
	CString caption_name,caption_file_name;
	Platform_information *plat_info;
	Part_set *parts;
	Component *root_component,*part_list_root_component;
	Light_set *lights;

	CString camera_file_name;
	Point background_color;
	Camera *camera[MAX_CAMERA_NUMBER];
	
	double selected_line_width,selected_point_size,minnimal_fovy_tanl,rotate_scale;
	int line_stipple_factor,line_stipple_pattern;

	inline ~Whole_system(void)
	{
		save_camera();

		for(int i=0;i<MAX_CAMERA_NUMBER;i++)
			if(camera[i]!=NULL){
				delete (camera[i]);
				camera[i]=NULL;
			}
		if(lights!=NULL)
			delete lights;
		if(part_list_root_component!=NULL)
			delete part_list_root_component;
		if(root_component!=NULL)
			delete root_component;
		if(parts!=NULL)
			delete parts;
		if(plat_info!=NULL)
			delete plat_info;
		if(assemble_type!=NULL)
			delete assemble_type;
	};
	void reload_component(CString my_caption_name,CString part_list_file_name,CString configure_file_name);
	Whole_system(CString sub_directory_name,CString my_part_file_name,CString my_merge_file_name,CString my_configure_file_name,CString my_caption_file_name);
	
	bool mount_on_box(Box mount_box,double scale,double time_length)
	{
		int number=0;
		for(int camera_id=0;camera_id<MAX_CAMERA_NUMBER;camera_id++)
			if(camera[camera_id]!=NULL)
				 if(camera[camera_id]->locate_mount_flag){
					number++;
					camera[camera_id]->locate_view(true,mount_box,scale,time_length,*parts);
				}
		return (number>0)?true:false;
	}
	inline bool load_ws_part(int release_memory_flag_map,CWnd *disp)
	{
		if(parts->load_part(release_memory_flag_map,disp)>0){
			root_component->get_component_box(*parts);
			return true;
		}
		return false;
	}
	Select_component_ids select_component_ids,match_component_ids;

	inline CString get_part_description()
	{
		return select_component_ids.get_part_description(parts);
	}
};

