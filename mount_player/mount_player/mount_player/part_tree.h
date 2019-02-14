#pragma once

#include <fstream>
using namespace std;

class part_info;
class movement_base;
class movement_index_of_part;
class graphics_system;
class mount_material_property;
class part_image;

class part_tree
{
	BOOL visible_flag,mark_flag,selected_flag;
public:	
	int mark_number;
	
	part_tree * get_last_mark_node(BOOL flag);	
	void set_mark_flag(BOOL new_selected_flag=TRUE);
	void clear_mark_flag(void);
	
	movement_base *move;
	HTREEITEM structure_tree_id;

	CString name,tree_file_name;
	double location[16],relative_location[16],box[6];
	int type;	/* 0 leaf, other NOT leaf*/	
	union tree_node_union{
		struct part{
			int part_id,part_component_id,part_component_number;
			part_tree *front_part,*back_part;
			double transparence_ratio;
			BOOL display_box_flag,bak_display_box_flag;
			HTREEITEM part_list_id;

			movement_index_of_part *move_index_of_part;
		}part;
		struct assembly{
			part_tree *first_child;
			int child_number;
		}assembly;
	}tree_node_union;	

	part_tree *parent,*front_brother,*back_brother;
	
	part_tree(ifstream *f,part_tree *my_parent);
	virtual ~part_tree(void);

	void reset_relative_location();

	void get_part_reference_number(int *visible_reference_number, int *hide_reference_number, BOOL current_visible_flag);
	CString create_name(part_tree * tree[], int tree_number, part_info * part[], int part_number, CString insert_string , BOOL total_number_flag);
	CString create_name(part_info **part,int part_number,CString insert_string,BOOL total_number_flag)
	{
		part_tree *t=this;
		return create_name(&t,1,part,part_number,insert_string ,total_number_flag);
	};

	int display(int index_id,int display_method,
		int part_number,part_info **part,
		part_tree *selected_tree,graphics_system *gs,
		mount_material_property *selected_material_property,
		double *clip_plane_data,
		double eye_x,double eye_y,double eye_z,
		double center_x,double center_y,double center_z,
		double mount_speeed,BOOL my_mark_flag,
		double manual_mount_proportion,
		BOOL transparence_flag,part_image *images[],BOOL display_reference_frame_flag);

	void set_transparence(double new_transparence_ratio);

	void clear_visible_flag();
	void set_visible_flag(BOOL my_visible_flag,BOOL with_children_flag=FALSE);
	BOOL get_visible_flag();
	BOOL is_visible();
	
	int get_total_triangle_slice_number(part_info **part);
	part_tree *get_node_by_name(CString my_name);

	void set_display_box_or_body(BOOL display_box_flag,BOOL modify_bak_flag,BOOL modify_invisible_flag);
	void restore_display_box_or_body();

	void caculate_box(part_info **part);

	void set_part_reference(part_tree *first_part_tree[],int part_number[]);

	part_tree * get_selected_node_by_part_tree(HTREEITEM tree_item_id);

private:
	BOOL display_part(int index_id,part_info **part,graphics_system *gs,BOOL transparence_flag,part_image *images[],double *transparence_ratio);
	void draw_cooradiator(graphics_system *gs);
public:
	BOOL compare_tree_node(part_tree *d,int sort_type_id,double min_value);
	void sort_tree_node(int sort_type_id,double min_value);	

	void clear_movement_index_of_part();
	void add_movement_index_of_part(int my_move_index_of_part);
	
	int get_min_step(int my_step=-1);
	int collect_mark_tree_nodes(int max_buffer_number, part_tree *buffer[]);
	int collect_visible_tree_nodes(int max_buffer_number, part_tree *buffer[]);
};

#define USER_SELECTED_PART_DISPLAY_METHOD		0
#define PART_BEING_SELECTED_DISPLAY_METHOD		1
#define MARK_PART_DISPLAY_METHOD				2

