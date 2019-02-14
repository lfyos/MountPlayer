#pragma once

#include "const.h"

class mount_view;
class movement;
class mount_sound_player;
class part_tree;

class mount_processor
{
public:
	BOOL should_flush_movement_flag;
	CString bak_movement_file_name;
	int move_number;
	movement *move[MAX_MOVEMENT_STEP];

	double start_time,time_length;
	int current_step,target_step;
	
	int cache_main_step,cache_sub_step,modify_sub_step;

	double move_parts_box[6];

	mount_processor(CString movement_file_name);
	~mount_processor(void);

	void sort_movement(void);
	void load_movement(CString movement_file_name);	
	void flush_movement(CString movement_file_name);
	void initialize_visible_state_and_movement_index();
	
	int process_mount(double start_time_length,int *start_step_id,int *end_step_id,mount_sound_player *player,CString directory_name);

	void advance_one_step(void);
	void retreat_one_step(void);
	void advance_big_step(void);
	void retreat_big_step(void);
	void advance_forever(void);
	void retreat_forever(void);
	void stop_mount(void);
	
	void jump_step(int step);
	void set_box(double new_box[]);

	void add_movement(movement * m);
	void delete_movement(int my_main_step, int my_sub_step);
	void replace_message(int my_main_step, CString my_message);
	void main_step_re_sort(int main_step_base,int main_step_step);
	void sub_step_re_sort(int my_main_step, int my_step_step);
	void move_movement(int begin_step, int end_step, int target_step);
	void combine_step(int step_id_1, int step_id_2);
	void separate_step(int main_step_id, int sub_step_id);

	void mount_progress_table(CString file_name,BOOL flag);
};

extern mount_processor *bridge_mounter;
