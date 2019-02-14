#pragma once

#include "mount_material_property.h"

class part_face;

class part_body
{
public:
	CString body_name;
	mount_material_property material;

	int face_count;
	part_face **face;

	part_body(ifstream *f);
	virtual ~part_body();
};
