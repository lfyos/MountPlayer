#pragma once

class movement_index_of_part
{
public:
	HTREEITEM tree_item_id_of_movement_index;
	int movement_id;
	movement_index_of_part *next_movement_index;

	movement_index_of_part(int my_movement_id,movement_index_of_part *my_next_movement_index);
	~movement_index_of_part(void);
};
