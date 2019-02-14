#pragma once

class part_index_of_movement
{
public:
	HTREEITEM tree_item_id_of_part_index;

	int part_id,part_component_id;
	part_index_of_movement *next_part_index;

	part_index_of_movement(int my_part_id,int my_part_component_id,part_index_of_movement *my_next_part_index);
	~part_index_of_movement(void);
};
