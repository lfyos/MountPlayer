
#include "get_part_data.h"
#include <fstream>
using namespace std;

class data_creator:public get_part_data
{
	int material_number,my_triangle_number;
	double my_quality,my_chordTolerance,my_lengthTolerance;
	material_data material[MAX_MATERIAL_NUMBER];

	static void output_string(ofstream * f, CString str);
	int register_material(material_data* m);

	CString create_mesh_data();	
	CString create_material_data();

public:	
	data_creator(CComPtr <ISldWorks> sw,double quality,double chordTolerance,double lengthTolerance);
	~data_creator(void);

	bool create_data(bool display_flag);

	static void create_all_data(CComPtr <ISldWorks> sw,double quality,double chordTolerance,double lengthTolerance);
};
