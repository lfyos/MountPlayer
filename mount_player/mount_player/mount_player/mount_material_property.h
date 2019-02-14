#pragma once
#include <fstream>
using namespace std;

class mount_material_property
{
public:
	void save(ofstream * f,CString name);
	void load(ifstream *f);
	BOOL material_flag,texture_flag; 
	double material_value[9];
	CString texture_name;
	int texture_image_id,color_operation;

	mount_material_property(void);
	virtual ~mount_material_property(void);	
};
