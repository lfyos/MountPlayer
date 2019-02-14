#include "StdAfx.h"
#include "movement_base.h"
#include "part_index_of_movement.h"
#include "part_tree.h"


movement_base::movement_base(void)
{
	part_index_of_move=NULL;
	return;
}

movement_base::~movement_base(void)
{
	clear_part_index_of_movement();
	return;
}

void movement_base::clear_part_index_of_movement(void)
{
	if(part_index_of_move!=NULL)
		delete part_index_of_move;
	part_index_of_move=NULL;
	return;
}

void movement_base::add_part_index_of_movement(part_tree *t,int part_index[])
{
	int i,n;
	part_index_of_movement *p;

	if(t->type!=0){
		n=t->tree_node_union.assembly.child_number;
		t=t->tree_node_union.assembly.first_child;
		for(i=0;i<n;i++,t=t->back_brother)
			add_part_index_of_movement(t,part_index);
		return;
	}
	part_index_of_move=new part_index_of_movement(
		t->tree_node_union.part.part_id,
		t->tree_node_union.part.part_component_id,
		part_index_of_move);

	for(p=part_index_of_move;;p=p->next_part_index)
		if(!exchange_part_index(p,p->next_part_index,part_index))
			break;
	return;
}
BOOL movement_base::exchange_part_index(part_index_of_movement * s, part_index_of_movement * d, int * part_index)
{
	BOOL flag;
	int x;

	if(d==NULL)
		flag=FALSE;
	else if((part_index[s->part_id])<(part_index[d->part_id]))
		flag=FALSE;
	else if((part_index[s->part_id])>(part_index[d->part_id]))
		flag=TRUE;
	else if((s->part_component_id)>(d->part_component_id))
		flag=TRUE;
	else if((s->part_component_id)<(d->part_component_id))
		flag=FALSE;
	else 
		flag=FALSE;
	if(flag){
		x=s->part_id;
		s->part_id=d->part_id;
		d->part_id=x;
		x=s->part_component_id;
		s->part_component_id=d->part_component_id;
		d->part_component_id=x;
	}
	return flag;
}
