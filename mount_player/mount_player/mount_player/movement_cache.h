#pragma once

#include "const.h"
#include "movement.h"

class movement_cache
{
	int move_number;
	movement *move[MAX_MOVEMENT_STEP];

	void delete_all_movements();

	int load(ifstream *f);
	void flush(ofstream  *f);
public:

	CString label_name,tree_file_name;
	int cache_type;

	movement_cache(void);
	~movement_cache(void);

	void insert_movement(movement *m);

	void save_movement_to_cache(CString file_name);
	void delete_movement_from_cache(CString file_name,int id_number);
	void add_movement_cache_label_to_menu(CMenu * pPopup ,int cache_main_step,int cache_sub_step,
		int directory_name_length,CString file_name,part_tree * t,int begin_command_id,BOOL cache_insert_or_delete_flag);
	int get_movement_from_cache(CString file_name,int id_number);
	movement *get_movement();
};
