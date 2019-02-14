#pragma once
#include <fstream>
using namespace std;

#include "const.h"

class part_body;
class graphics_system;
class part_image;

#include "mount_material_property.h"

class part_info
{
private:
	void caculate_box(void);
	void set_part_information(CString information_file);

	void display_clip_plane(double current_matrix[],
		double x0, double y0, double z0, 		double x1, double y1, double z1, 
		double x2, double y2, double z2, 		double x3, double y3, double z3,
		graphics_system *gs,double *clip_plane_data,
		double eye_x,double eye_y,double eye_z,	double center_x,double center_y,double center_z);
public:
	HTREEITEM part_list_id;

	CString part_file_name;

	int body_count;
	part_body **body;

	int call_list_id[MAX_CALL_LIST_INDEX_NUMBER],box_call_list_id[MAX_CALL_LIST_INDEX_NUMBER],no_material_call_list_id[MAX_CALL_LIST_INDEX_NUMBER];

	double box[6];
	struct part_information{
		CString name,length_width,thickness,weight,comment;
	}information;
	mount_material_property material;

	part_info(ifstream *f,CString information_file);
	virtual ~part_info(void);

	int create_call_list(int index_id,int start_call_list_id,part_image *images[],graphics_system *gs);

	void change_to_absulate(double current_matrix[],double *x, double *y, double *z);

	CString get_description(CString id_str);
	int get_total_triangle_slice_number();
	BOOL get_body_face_triangle(int select_triangle_number,int *body_id, int *face_id, int *triangle_id, 
		double *tesstriangles, double *tessnorms);	

	void display(int index_id,graphics_system *gs);
	void display_transparently(int index_id,part_image *images[],graphics_system *gs,double transparence_ratio);
	void display_box(int index_id,graphics_system *gs);
	void display_selected_part(int index_id,graphics_system *gs,mount_material_property *selected_material_property);
	void display_selecting_part(int index_id,int part_id,int part_number,int part_component_id,int part_component_number,graphics_system *gs);
	void display_selecting_face(graphics_system *gs);
	void display_box_on_clip_plane(graphics_system *gs,double *clip_plane_data,
		double eye_x,double eye_y,double eye_z,double center_x,double center_y,double center_z);	
};


