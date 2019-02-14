#pragma once

#include "movement_base.h"
#include <fstream>
using namespace std;


#define MAX_MOVEMENT_POINT 10

class movement :
	public movement_base
{
	void reset_point(double time_length);
	void reset_point(int point_number, double time_length);
public:
	movement(ifstream *f);
	movement(void);
	virtual ~movement(void);

	HTREEITEM main_list_id,sub_list_id;
	CString name,message,sound_file;
	int main_step,sub_step;

	int step_number;
	struct movement_point{	
		double time_length;
		double location[16];
	}relative_point[MAX_MOVEMENT_POINT],bak_relative_point[MAX_MOVEMENT_POINT];

	BOOL end_visible_state;
	double start_time,start_time_length;
	part_tree *tree_node;

	double get_movement_time_length();
	double link_to_tree(part_tree *tree,CString name,double start_t,double start_t_length,BOOL end_state);
	virtual BOOL get_relative_location(double move_relative_location[],double mount_speed,double manual_mount_proportion);
	void unlink_from_tree(void);
	void reverse(BOOL flag);
	void copy_from(movement *mov);
	
	void load(ifstream *f);
	void flush(ofstream  *f);

	void replace_move_point(int point_id,double my_time_length,double  a[]);
	int add_move_point(CString my_name , CString my_message , CString my_sound_file,int my_main_step , int my_sub_step);
	
	void init_movement(void);	
};