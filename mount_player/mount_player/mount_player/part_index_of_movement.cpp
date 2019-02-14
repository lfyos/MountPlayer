#include "StdAfx.h"
#include "part_index_of_movement.h"

part_index_of_movement::part_index_of_movement(int my_part_id,int my_part_component_id,part_index_of_movement *my_next_part_index)
{
	part_id=my_part_id;
	part_component_id=my_part_component_id;
	next_part_index=my_next_part_index;

	return;
}

part_index_of_movement::~part_index_of_movement(void)
{
	if(next_part_index!=NULL)
		delete next_part_index;
	next_part_index=NULL;
	return;
}
