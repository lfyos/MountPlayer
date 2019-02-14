#pragma once

class part_info;
class part_tree;
class part_image;
class graphics_system;
class mount_material_property;

#define MAX_BRIDGE_IMAGE_NUMBER 10000
class mount_bridge
{
public:
	int part_number,image_number,*part_index,directory_name_length;
	part_info **part;
	part_tree *tree,**first_part_tree;
	
	part_image *images[MAX_BRIDGE_IMAGE_NUMBER];

	mount_bridge(CString directory_name,CString structure_file_name,CString information_file_name,CString sort_name,double min_value);
	virtual ~mount_bridge(void);

private:
	int register_image_file_name(CString directory_name,CString file_name);
	void initialize_images(CString directory_name);

	void set_reference(int sort_type_id,double min_value);
	CString get_system_part_directory(void);
	CString get_system_tree_directory(part_tree *t,CString str);
public:
	int get_sort_id_from_name(CString sort_name);
	int create_call_list(int index_id,int start_call_list_id,graphics_system *gs);
	void only_display_us(part_tree *buffer[],int max_number);

	int display(int index_id,int display_method,
		part_tree *selected_tree,graphics_system *gs,
		mount_material_property *selected_material_property,
		double *clip_plane_data,
		double eye_x,double eye_y,double eye_z,
		double center_x,double center_y,double center_z,
		double mount_speeed,BOOL my_mark_flag,
		double manual_mount_proportion,
		BOOL display_reference_frame_flag);
	int collect_mark_tree_nodes(int max_buffer_number, part_tree *buffer[], double  mark_tree_node_box[]);
	int collect_visible_tree_nodes(int max_buffer_number, part_tree *buffer[], double  mark_tree_node_box[]);
private:
	void compute_tree_nodes_box(int max_buffer_number, part_tree *buffer[], double  mark_tree_node_box[]);
};
