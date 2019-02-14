#pragma once

#include "Whole_system.h"

class User_interface
{
public:

	CDC *dc;
	double view_width,view_height;
	Box *selection_box;
	bool swap_flag;

	Clipper clipper;
	
	int camera_id;
	
	Component *do_screen_selection(int display_map,double sx, double sy);

	Whole_system *ws;
	inline Camera *camera_pointer()
	{
		return (ws->camera[camera_id]);
	}

	User_interface(Whole_system &my_ws,Graphics_system &my_gs,CDC &my_dc,int my_camera_id,Box *my_selection_box=NULL,double *mirror_plane=NULL);
	inline ~User_interface(void)
	{
		Graphics_system::end_display(*dc,swap_flag);
	}
	void change_camera(int new_camera_id,double new_view_width,double new_view_height,double *mirror_plane);

	void create_graphics_system_object(int display_map,Component *comp,double precision_scale);
	void delete_graphics_system_object();

	void create_depth_texture(int display_map,double precision_scale);
	int paint(int display_map,double precision_scale);
	static int do_paint(double precision_scale,CDC &dc,Whole_system &my_ws,Graphics_system &my_gs,int my_camera_id);
	
	inline void modify_view(Location loca,double time_length=-1.0)
	{
		camera_pointer()->eye_component->modify_move_relative_location(loca,*(ws->parts));
		if(time_length>0)
				camera_pointer()->push_restore_record(*(ws->parts),time_length,true);
	}
	inline void rotate_view(int rotate_type,int last_x, int last_y, int this_x, int this_y)
	{
		if((last_x!=this_x)||(last_y!=this_y))
			modify_view(camera_pointer()->rotate_view(rotate_type,last_x,last_y,this_x,this_y,ws->rotate_scale));
	}
	inline void translate_view(int last_x, int last_y, int this_x, int this_y,Point &select_flag)
	{
		if((last_x!=this_x)||(last_y!=this_y))
			modify_view(camera_pointer()->translate_view(last_x,last_y,this_x,this_y,select_flag));
	}

	void set_view(int view_id,double time_length);

	void enlarge_selected_area(int mouse_down_x,int mouse_down_y,int mouse_up_x,int mouse_up_y,double time_length);
	
	void translate_component(int last_x, int last_y, int this_x, int this_y,Point &world_select_flag,Point &local_select_flag);
	void rotate_component(int rotate_type,int last_x, int last_y, int this_x, int this_y);
	
	double adjust_mouse_distance(double mouse_x,double mouse_y,double scale_value);
	double adjust_fovy(double delta);

public:
	
	static Component *select_component_from_screen(Whole_system &my_ws,Graphics_system &my_gs,CDC &my_dc,int my_camera_id,double sx, double sy);

	Component *get_most_recent_selected_component(Component *p);

	void select_parent_component();

	void select_child_component(Component *component_pointer);
	void select_child_component();

	double set_component_transparence(double scale,bool transparence_flag);
	void clear_component_transparence();

	void reset_component();

	inline void modify_clip_plane(int clip_plane_id,double scale)
	{
		camera_pointer()->modify_clip_plane(clip_plane_id,scale);
	}
	void area_select_component(int my_display_map,double win_x0, double win_y0, double win_x1, double win_y1);

	void hide_component(int display_map);
	void show_component(int display_map);

	void set_component_frame_show(bool only_display_frame_flag);
	void clear_component_frame_show();

	void set_component_frame_show_detail(bool frame_display_detail_flag);
	void set_component_hide_line_type(int hide_line_type_id);//0:hide 1:stipple	2:visible


	void release_component_mesh();

	static void edit_part_material(Whole_system *my_ws);
	void set_all_material_reload();

	void create_assemble_part(CString assemble_part_file_name,bool material_id_increase_flag);
};


