#pragma once
#include <fstream>
using namespace std;

class part_index_of_movement;
class part_tree;

class movement_base
{
	BOOL exchange_part_index(part_index_of_movement * s, part_index_of_movement * d, int * part_index);
public:
	part_index_of_movement *part_index_of_move;

	movement_base(void);
	~movement_base(void);

	virtual BOOL get_relative_location(double move_relative_location[],double mount_speed,double manual_mount_proportion)=NULL;

	void clear_part_index_of_movement(void);
	void add_part_index_of_movement(part_tree *t,int part_index[]);	
};
