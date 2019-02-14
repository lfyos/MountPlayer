#pragma once
#include <fstream>
using namespace std;

#include "const.h"
#include "mount_material_property.h"
class graphics_system;

class part_face
{
public:
	CString face_name;
	mount_material_property material;
	int tesstrianglecount;
	double *tesstriangles,*tessnorms,*tesstexture;

	part_face(ifstream *f);
	virtual ~part_face();

	int create_call_list(int index_id,int start_call_list_id,graphics_system *gs);
	int no_texture_call_list_id[MAX_CALL_LIST_INDEX_NUMBER],texture_call_list_id[MAX_CALL_LIST_INDEX_NUMBER];
};
