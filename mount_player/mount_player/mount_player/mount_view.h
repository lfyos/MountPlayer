#pragma once

#include "const.h"
#include "graphics_system.h"
#include "mount_material_property.h"

class part_tree;
class mount_bridge;


#define MAX_VIEW_DATA	1000

class mount_view
{
	int view_data_bak_number,view_data_bak_index;
	struct view_data{
		double eye_x,eye_y,eye_z,center_x,center_y,center_z,up_x,up_y,up_z;
	}view_data_bak[MAX_VIEW_DATA];
	
	BOOL view_modifier_flag;
	double start_time,time_length;
	struct view_modifier_data{
		double eye_x,eye_y,eye_z,center_x,center_y,center_z,up_x,up_y,up_z;
	}view_start,view_end;

	struct defined_area{
		BOOL flag;
		double x[4],y[4],z[4];
	}defined_area;

public:
	double background_red_color,background_green_color,background_blue_color;
	double environment_red_color,environment_green_color,environment_blue_color;
	BOOL separate_specular_color_flag;

	mount_material_property selected_material_property;
	struct graphics_system_light light[MAX_LIGHT];

	BOOL clip_plane_flag,display_reference_frame_flag;
	double clip_plane_data[4];

	double eye_x,eye_y,eye_z,center_x,center_y,center_z,up_x,up_y,up_z;
	double speed;

	mount_view(CString directory_name,CString structure_file_name,CString information_file_name,CString light_file_name,CString sort_name,double min_value);
	virtual ~mount_view(void);

	void process_scale(double p);
	void set_retreat_modifier(double my_start_time=-1,double my_time_length=-1);
	void set_modifier(double my_start_time=-1,double my_time_length=-1);
	int process_modifier();
	void put_box_in_view(graphics_system *gs,double component_box[],double box_p=0.9);

	void set_equal_view();
	void save_view_data();
	void restore_view_data();

	part_tree  *get_tree_body_face_triangle_and_xyz(int index_id,CPoint *screen_point,
		int *body_id, int *face_id, int *triangle_id, BOOL *xyz_flag,
		double *x, double *y, double *z,graphics_system *gs,double mount_speeed,double manual_mount_proportion);

	int do_display(int index_id,graphics_system *gs,double manual_mount_proportion);
	int do_mark_display(int index_id,graphics_system *gs,double box[],double manual_mount_proportion);

	void set_clip_plane(BOOL flag,double x,double y,double z);

	void adjust_view();
	void aim_at_box(double box[]);

	void set_x_view();
	void set_y_view();
	void set_z_view();

	void process_translation(graphics_system *gs,CPoint last_point,CPoint move_point);
	void process_rotation(graphics_system *gs,CPoint last_point,CPoint move_point);
	void process_area_define(graphics_system *gs,CPoint last_point,CPoint move_point);
	void process_area_enlarge(graphics_system *gs,CPoint last_point,CPoint move_point);

	void process_clip_plane_modification(double portion);

	mount_bridge *bridge;
private:
	double get_view_depth();

	void draw_background(graphics_system *gs);

	void change_relative_points_to_absulate_points(part_tree *t, double data[],double mount_speeed,double manual_mount_proportion);
	BOOL get_point_space_locaton_on_plane(CPoint *screen_point,double data[],double *x,double *y,double *z,graphics_system *gs);
	BOOL get_point_space_locaton(CPoint *screen_point,double data[],double *x,double *y,double *z,graphics_system *gs);

	BOOL get_xy(double sx, double sy, double *x, double *y,double *z,graphics_system *gs);
	BOOL process_rotation_routine(
				double last_x,		double last_y,		double last_z,
				double move_x,		double move_y,		double move_z, 
				double rotate_x,	double rotate_y,	double rotate_z,
				double *x,			double *y,			double *z);
	void setup_light(graphics_system *gs,
				int max_light_number,struct graphics_system_light light_data[],
				double eye_x,double eye_y,double eye_z,
				double center_x,double center_y,double center_z,
				double up_x,double up_y,double up_z);
public:
	void save_light();
	void load_light(CString file_name);
private:
	CString light_file_name;
};

extern mount_view *bridge_view;