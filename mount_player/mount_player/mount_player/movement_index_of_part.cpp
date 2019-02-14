#include "StdAfx.h"
#include "movement_index_of_part.h"

movement_index_of_part::movement_index_of_part(int my_movement_id,movement_index_of_part *my_next_movement_index)
{
	movement_id=my_movement_id;
	next_movement_index=my_next_movement_index;
	return;
}
movement_index_of_part::~movement_index_of_part(void)
{
	if(next_movement_index!=NULL)
		delete next_movement_index;
	next_movement_index=NULL;
	return;
}
