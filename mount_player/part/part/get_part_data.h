#pragma once

#include "stdafx.h"
#include "part.h"

const double PI=3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679;

#define MAX_LOOP_NUMBER			100000
#define MAX_EDGE_NUMBER			100000
#define MAX_BODY_NUMBER			100000
#define MAX_FACE_NUMBER			100000
#define MAX_MATERIAL_NUMBER		2000


class material_data
{
public:
	BOOL material_flag;
	double material[9];

	double rotation_angle,scale_ratio;
	BOOL blend_flag;
	CString texture_file_name;
	material_data()
	{
	};
	void get_data(material_data* a)
	{
		int i;
		material_flag=a->material_flag;
		for(i=0;i<sizeof(material)/sizeof(material[0]);i++)
			material[i]=a->material[i];
		rotation_angle=a->rotation_angle;
		scale_ratio=a->scale_ratio;
		blend_flag=a->blend_flag;
		texture_file_name=a->texture_file_name;

		return;		
	}
	BOOL is_equal(material_data* a)
	{
		int i;
		if(material_flag)
			if(!(a->material_flag))
				return FALSE;
		if(!(material_flag))
			if(a->material_flag)
				return FALSE;
		for(i=0;i<sizeof(material)/sizeof(material[0]);i++)
			if(material[i]!=(a->material[i]))
				return FALSE;
		if(texture_file_name!=(a->texture_file_name))
			return FALSE;
		if(texture_file_name=="")
			return TRUE;
		if(blend_flag)
			if(!(a->blend_flag))
				return FALSE;
		if(!(blend_flag))
			if(a->blend_flag)
				return FALSE;
		if(rotation_angle!=(a->rotation_angle))
			return FALSE;
		if(scale_ratio!=(a->scale_ratio))
			return FALSE;
		return TRUE;
	};
};

class get_part_data
{
public:
	CComPtr		<ISldWorks>				sw;
	CComPtr		<IModelDoc2>			pRootModelDoc;
	CComPtr		<IModelDocExtension>	pRootModelDoc_ext;
	CComPtr		<IPartDoc>				pPartDoc;	
	long		body_count;
	CComQIPtr	<IBody2>				pBody[MAX_BODY_NUMBER];
	CString		path_name,directory_name,part_name;

	CComPtr <ITexture> texture_object;	
	material_data material;

	void get_material_and_texture();

	get_part_data(CComPtr<ISldWorks> my_sw,double quality);
	~get_part_data(void);
};

class get_body_data
{
public:
	CComQIPtr	<IBody2> pBody;
	CString  name;
	long		face_count;
	CComPtr		<IFace2> face[MAX_FACE_NUMBER];	

	CComPtr <ITexture> texture_object;	
	material_data material;

	void get_material_and_texture();

	get_body_data(CComQIPtr	<IBody2> my_pBody);
	~get_body_data(void);
};

class get_face_data
{
public:
	CComPtr <IFace2> face;
	long TessTriangleCount;
	float *TessTriangles,*TessNorms,*TessTexture;

	CComPtr <ITexture> texture_object;
	material_data material;

	get_face_data(CComPtr	<IFace2> my_face);
	~get_face_data(void);

	void get_material_and_texture();

	CComPtr	<ISurface> surface;

	CString	surface_str;
	int parameter_number;
	double	surface_parameter[32];

	int loop_number;
	CComQIPtr <ILoop2> loop[MAX_LOOP_NUMBER];
};

class get_loop_data
{
public:
	CComQIPtr <ILoop2>	loop;

	int edge_number;
	CComQIPtr <IEdge> edge[MAX_EDGE_NUMBER];

	get_loop_data(CComQIPtr <ILoop2> my_loop);
	~get_loop_data();
};

class get_edge_data
{
public:
	CComQIPtr <IEdge>	edge;

	CComQIPtr <IVertex>	start_vertex,end_vertex;
	CString start_point_flag,end_point_flag;
	double start_point[3],end_point[3];

	get_edge_data(CComQIPtr <IEdge> my_edge,double chordTolerance,double lengthTolerance);
	~get_edge_data();

	CComQIPtr <ICurve>curve;
	CString curve_flag;
	int parameter_number;
	double curve_parameter[64];


	int TessPtsSize;
	double *TessPts;
};
