#include "StdAfx.h"
#include "mount_material_property.h"
#include "const.h"


mount_material_property::mount_material_property(void)
{
	int i;
	material_flag = FALSE;
	texture_flag = FALSE;
	for(i=0;i<sizeof(material_value)/sizeof(material_value[0]);i++)
		material_value[i] = 0;
	texture_name="";
	texture_image_id = -1;
	return;
}

mount_material_property::~mount_material_property(void)
{

}

void mount_material_property::load(ifstream *f)
{
	int i;
	char tmp[MAX_MESSAGE_LENGTH];
	CString str;

	*f>>tmp>>tmp;
	material_flag=((str=tmp)=="YES")?TRUE:FALSE;
	for(i=0;i<sizeof(material_value)/sizeof(material_value[0]);i++)
		*f>>material_value[i];

	*f>>tmp>>tmp;
	texture_flag=((str=tmp)=="YES")?TRUE:FALSE;

	if(texture_flag){
		*f>>tmp;
		texture_name=tmp;
		*f>>tmp>>color_operation;

		switch(color_operation){
		default:
		case 1:
		case 2:
			break;
		}
	}
	return;
}
void mount_material_property::save(ofstream * f,CString name)
{
	int i;

	*f<<name<<(material_flag?"  YES":"  NO");
	for(i=0;i<sizeof(material_value)/sizeof(material_value[0]);i++)
		*f<<"    "<<(material_flag?material_value[i]:0);

	*f<<"\nTessTexture: "<<(texture_flag?"YES  ":"NO  ");

	if(texture_flag)
		*f<<texture_name<<"	color_operation:	"<<color_operation<<"\n";	
	return;
}
