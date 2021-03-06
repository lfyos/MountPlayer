#include "stdafx.h"
#include "resource.h"
#include "get_part_data.h"
#include <math.h>
#include <string.h>


get_part_data::get_part_data(CComPtr<ISldWorks> my_sw,double quality)
{
	HRESULT res;	
	long i,j,doctype,nBodyHighIndex=-1,nRetVal = -1;
	CComVariant vBodyArr;
	BSTR tmp;
	
	sw=my_sw;
	pRootModelDoc=NULL;
	pRootModelDoc_ext=NULL;
	pPartDoc=NULL;
	body_count=-1;
	path_name=_T("没有文件名");
	texture_object=NULL;

	res=sw->get_IActiveDoc2(&pRootModelDoc);
	if(res!=S_OK||pRootModelDoc==NULL){
		pRootModelDoc=NULL;
		::MessageBox(NULL,_T("取得活跃文档失败"),_T("取得活跃文档失败"),MB_OK);
		return;
	}

	pRootModelDoc->ShowNamedView2 (_T("等轴测"),7);
	pRootModelDoc->ViewZoomtofit2 ( );
	pRootModelDoc->GraphicsRedraw2(); 

	pRootModelDoc->SetTessellationQuality((long)quality);
	pRootModelDoc->Rebuild(swForceRebuildAll);
	pRootModelDoc->GraphicsRedraw2(); 

	res=pRootModelDoc->get_Extension ( &pRootModelDoc_ext );
	if((res!=S_OK)||(pRootModelDoc_ext==NULL)){
		pRootModelDoc.Release();
		pRootModelDoc=NULL;
		pRootModelDoc_ext=NULL;	
		::MessageBox(NULL,_T("取得活跃文档扩充失败"),_T("取得活跃文档扩充失败"),MB_OK);
		return;
	}
	res=pRootModelDoc->GetType(&doctype);
	if(res!=S_OK||doctype!=swDocPART){
		pRootModelDoc.Release();
		pRootModelDoc_ext.Release();
		pRootModelDoc=NULL;
		pRootModelDoc_ext=NULL;	
		::MessageBox(NULL,_T("取得文档类型失败"),_T("取得文档类型失败"),MB_OK);
		return;
	}
	res=pRootModelDoc->QueryInterface(IID_IPartDoc, (LPVOID *)&pPartDoc);
	if(res!=S_OK||pPartDoc==NULL){
		pRootModelDoc.Release();
		pRootModelDoc_ext.Release();
		pRootModelDoc=NULL;
		pRootModelDoc_ext=NULL;
		pPartDoc=NULL;
		::MessageBox(NULL,_T("取得零件体失败"),_T("取得零件体失败"),MB_OK);
		return;
	}    
	res=pPartDoc->GetBodies2(swSolidBody,TRUE,&vBodyArr);
	if((res!=S_OK)||(vBodyArr.pparray==NULL)){
		pRootModelDoc.Release();
		pRootModelDoc_ext.Release();
		pPartDoc.Release();
		pRootModelDoc=NULL;
		pRootModelDoc_ext=NULL;
		pPartDoc=NULL;
		::MessageBox(NULL,_T("取得零件体BODY失败"),_T("取得零件体BODY失败"),MB_OK);
		return;
	}
	SAFEARRAY *psaBody = V_ARRAY(&vBodyArr);
	LPDISPATCH *pBodyDispArray=NULL;
    res = SafeArrayAccessData(psaBody, (void **) &pBodyDispArray); 
	if((res!=S_OK)||(pBodyDispArray==NULL)){
		pRootModelDoc.Release();
		pRootModelDoc_ext.Release();
		pPartDoc.Release();
		pRootModelDoc=NULL;
		pRootModelDoc_ext=NULL;
		pPartDoc=NULL;
		::MessageBox(NULL,_T("访问动态数组失败"),_T("访问动态数组失败"),MB_OK);
		return;
	}
    if(SafeArrayGetUBound(psaBody,1,&nBodyHighIndex)!=S_OK){
		pRootModelDoc.Release();
		pRootModelDoc_ext.Release();
		pPartDoc.Release();
		pRootModelDoc=NULL;
		pRootModelDoc_ext=NULL;
		pPartDoc=NULL;
		::MessageBox(NULL,_T("访问动态数组大小失败"),_T("访问动态数组大小失败"),MB_OK);
		return;
	}

    body_count = nBodyHighIndex + 1;
    for (i = 0; i < body_count; i++){		
        if(((pBody[i])=pBodyDispArray[i])==NULL){
//			SafeArrayUnaccessData(psaBody);
			SafeArrayDestroy(psaBody);
			pRootModelDoc.Release();

			body_count=i+1;
			::MessageBox(NULL,_T("取得零件体BODY数目失败"),_T("取得零件体BODY数目失败"),MB_OK);
			return;
		}	
    }
//	SafeArrayUnaccessData(psaBody);
    SafeArrayDestroy(psaBody);
	
	if(pRootModelDoc->GetPathName(&tmp)==S_OK)
		path_name=tmp;
	else
		path_name="没有文件名";

	part_name=path_name;
	
	for(i=part_name.GetLength()-1;i>=0;i--)
		if(CString(part_name.GetAt(i))==_T(".")){
			part_name=part_name.Left(i);
			break;
		}
	directory_name=_T("\\");
	for(i=part_name.GetLength()-1,j=0;i>=0;i--,j++)
		if(CString(part_name.GetAt(i))==_T("\\")){
			directory_name=part_name.Left(i+1);
			part_name=part_name.Right(j);
			break;
		}
	return;
}

void get_part_data::get_material_and_texture()
{
	BSTR tmp;
	VARIANT_BOOL color_operation;

	if((pRootModelDoc==NULL)||(pRootModelDoc_ext==NULL))
		return;

	material.material_flag=(pRootModelDoc->get_IMaterialPropertyValues(material.material)==S_OK)?TRUE:FALSE;

	material.texture_file_name="";
	if(pRootModelDoc_ext->GetTexture(NULL,&texture_object)!=S_OK)
		texture_object=NULL;
	if(texture_object==NULL)
		return;
	if(texture_object->get_MaterialName(&tmp)!=S_OK){
		texture_object.Release();
		texture_object=NULL;
	}
	material.texture_file_name=tmp;
	if(texture_object->get_Angle(&(material.rotation_angle))!=S_OK)
		material.rotation_angle=0;
	material.rotation_angle*=PI/180;
	if(texture_object->get_ScaleFactor(&(material.scale_ratio))!=S_OK)
		material.scale_ratio=1.0;	
	if(texture_object->get_BlendColor(&color_operation)!=S_OK)
		color_operation=FALSE;
	material.blend_flag=color_operation?TRUE:FALSE;

	return;
}

get_part_data::~get_part_data(void)
{
	for(int i=0;i<body_count;i++)
		pBody[i].Release();
	body_count=0;

	if(texture_object!=NULL){
		texture_object.Release();
		texture_object=NULL;
	}
	if(pRootModelDoc!=NULL){
		pRootModelDoc->Rebuild(swForceRebuildAll);
		pRootModelDoc->GraphicsRedraw2(); 
		pRootModelDoc->Save();
		pRootModelDoc.Release();
		pRootModelDoc=NULL;
	}
	if(pRootModelDoc_ext!=NULL){
		pRootModelDoc_ext.Release();
		pRootModelDoc_ext=NULL;
	}
	if(pPartDoc!=NULL){
		pPartDoc.Release();
		pPartDoc=NULL;
	}
	sw->CloseDoc(path_name.GetBuffer(path_name.GetLength()));
	return;
}


//#############################################################################################################################################################


get_body_data::get_body_data(CComQIPtr	<IBody2> my_pBody)
{
	BSTR tmp;

	pBody=my_pBody;
	texture_object=NULL;

	if(pBody->get_Name(&tmp)==S_OK)
		name=tmp;
	else
		name="没有实体名字";

	if(pBody->GetFaceCount(&face_count)!=S_OK)
		face_count=0;
	if(face_count<=0)
		return;
	if(pBody->IGetFirstFace(&(face[0]))!=S_OK){
		face_count=0;
		return;
	}
	for(face_count=1;face_count<(MAX_FACE_NUMBER-1);face_count++){
		if(face[face_count-1]->IGetNextFace(&(face[face_count]))!=S_OK){
			face[face_count]=NULL;
			return;
		}
		if(face[face_count]==NULL)
			return;
	}
	return;
}
void get_body_data::get_material_and_texture()
{
	BSTR tmp;
	VARIANT_BOOL color_operation;

	material.material_flag=(pBody->get_IMaterialPropertyValues(material.material)==S_OK)?TRUE:FALSE;

	material.texture_file_name="";
	if(pBody->GetTexture(NULL,&texture_object)!=S_OK)
		texture_object=NULL;
	if(texture_object==NULL)
		return;
	if(texture_object->get_MaterialName(&tmp)!=S_OK){
		texture_object.Release();
		texture_object=NULL;
	}
	material.texture_file_name=tmp;
	if(texture_object->get_Angle(&(material.rotation_angle))!=S_OK)
		material.rotation_angle=0;
	material.rotation_angle*=PI/180;
	if(texture_object->get_ScaleFactor(&(material.scale_ratio))!=S_OK)
		material.scale_ratio=1.0;	
	if(texture_object->get_BlendColor(&color_operation)!=S_OK)
		color_operation=FALSE;
	material.blend_flag=color_operation?TRUE:FALSE;

	return;
}
get_body_data::~get_body_data(void)
{
	int i;

	if(texture_object!=NULL)
		texture_object.Release();

	for(i=0;i<face_count;i++)
		face[i].Release();
	return;
}

//#############################################################################################################################################################

get_face_data::get_face_data(CComPtr	<IFace2> my_face)
{
	int i;
	VARIANT_BOOL flag;
	
	loop_number=0;

	face=my_face;

	TessTriangles=NULL;
	TessNorms=NULL;
	TessTexture=NULL;
	texture_object=NULL;

	if(face->GetTessTriangleCount(&TessTriangleCount)!=S_OK)
		TessTriangleCount=0;
	if(TessTriangleCount<=0)
		return;
	TessTriangles=new float[9*TessTriangleCount];
	if(face->IGetTessTriangles(TRUE,TessTriangles)!=S_OK){
		delete [] TessTriangles;
		TessTriangles=NULL;
		TessTriangleCount=0;
		return;
	}
	TessNorms=new float[9*TessTriangleCount];
	if(face->IGetTessNorms(TessNorms)!=S_OK){
		delete [] TessTriangles;
		delete [] TessNorms;		
		TessTriangles=NULL;
		TessNorms=NULL;
		TessTriangleCount=0;		
		return;
	}
	TessTexture=new float[9*TessTriangleCount];
	if(face->IGetTessTextures(TessTexture)!=S_OK){
		delete []TessTexture;
		TessTexture=NULL;
	}

/*******************************************************************************************/

	surface_str=_T("unknown");
	parameter_number=0;
	for(i=0;i<(sizeof(surface_parameter)/sizeof(surface_parameter[0]));i++)
		surface_parameter[i]=0.0;

	if(face->IGetSurface(&surface)!=S_OK)
		surface=NULL;
	else 
		for(;;){
			if(surface->IsPlane(&flag)==S_OK)
				if(flag)
					if(surface->get_IPlaneParams(surface_parameter)==S_OK){
						parameter_number=6;
						surface_str=_T("平面");
						break;
					}
			if(surface->IsCylinder(&flag)==S_OK)
				if(flag)
					if(surface->get_ICylinderParams(surface_parameter)==S_OK){
						parameter_number=7;
						surface_str=_T("柱面");
						break;
					}
			if(surface->IsCone(&flag)==S_OK)
				if(flag)
					if(surface->get_IConeParams(surface_parameter)==S_OK){
						parameter_number=8;
						surface_str=_T("锥面");
						break;
					}
			if(surface->IsSphere(&flag)==S_OK)
				if(flag)
					if(surface->get_ISphereParams(surface_parameter)==S_OK){
						parameter_number=4;
						surface_str=_T("球面");
						break;
					}
			if(surface->IsTorus(&flag)==S_OK)
				if(flag)
					if(surface->get_ITorusParams(surface_parameter)==S_OK){
						parameter_number=8;
						surface_str=_T("环面");
						break;
					}
			break;
		}

/*******************************************************************************************/

	{
		long ret_val;
		CComVariant vLoopArr;
		loop_number=0;
		if((face->GetLoopCount(&ret_val)!=S_OK)||(face->GetLoops(&vLoopArr)!=S_OK))
			return;
		SAFEARRAY *psaLoop = V_ARRAY(&vLoopArr);
		LPDISPATCH *pLoopDispArray=NULL;
	    if(SafeArrayAccessData(psaLoop,(void **) &pLoopDispArray)!=S_OK)
			return;
		ret_val=-1;
		if(SafeArrayGetUBound(psaLoop,1,&ret_val)!=S_OK){
			SafeArrayDestroy(psaLoop);
			return;
		}
		for(int i=0;(i<=ret_val)&&(i<MAX_LOOP_NUMBER);i++){
			if(((loop[i])=(pLoopDispArray[i]))==NULL)
				break;
			loop_number++;
		}
		SafeArrayDestroy(psaLoop);
	}
}

void get_face_data::get_material_and_texture()
{
	BSTR tmp;
	VARIANT_BOOL color_operation;

	material.material_flag=(face->get_IMaterialPropertyValues(material.material)==S_OK)?TRUE:FALSE;

	material.texture_file_name="";
	if(face->GetTexture(NULL,&texture_object)!=S_OK)
		texture_object=NULL;
	if(texture_object==NULL)
		return;
	if(texture_object->get_MaterialName(&tmp)!=S_OK){
		texture_object.Release();
		texture_object=NULL;
	}
	material.texture_file_name=tmp;
	if(texture_object->get_Angle(&(material.rotation_angle))!=S_OK)
		material.rotation_angle=0;
	material.rotation_angle*=PI/180;
	if(texture_object->get_ScaleFactor(&(material.scale_ratio))!=S_OK)
		material.scale_ratio=1.0;	
	if(texture_object->get_BlendColor(&color_operation)!=S_OK)
		color_operation=FALSE;
	material.blend_flag=color_operation?TRUE:FALSE;
	return;
}
get_face_data::~get_face_data(void)
{
	if(texture_object!=NULL)
		texture_object.Release();
	if(TessTriangles!=NULL)
		delete []TessTriangles;
	if(TessNorms!=NULL)
		delete []TessNorms;
	if(TessTexture!=NULL)
		delete []TessTexture;

	if(surface!=NULL)
		surface.Release();

	for(int i=0;i<MAX_LOOP_NUMBER;i++)
		loop[i].Release();
	return;
}

get_loop_data::get_loop_data(CComQIPtr <ILoop2> my_loop)
{
	loop=my_loop;
	edge_number=0;

	CComVariant vEdgeArr;
	if(loop->GetEdges(&vEdgeArr)!=S_OK)
		return;

	SAFEARRAY *psaEdge = V_ARRAY(&vEdgeArr);
	LPDISPATCH *pEdgeDispArray=NULL;

    if(SafeArrayAccessData(psaEdge, (void **) &pEdgeDispArray)!=S_OK)
		return;

	long nBodyHighIndex=-1;
	if(SafeArrayGetUBound(psaEdge,1,&nBodyHighIndex)!=S_OK){
		SafeArrayDestroy(psaEdge);
		return;
	}
	for(int i=0;(i<=nBodyHighIndex)&&(i<MAX_EDGE_NUMBER);i++){
		if(((edge[i])=(pEdgeDispArray[i]))==NULL)
			break;
		edge_number++;
	}
	SafeArrayDestroy(psaEdge);
}

get_loop_data::~get_loop_data()
{
	for(int i=0;i<edge_number;i++)
		if(edge[i]!=NULL)
			edge[i].Release();
	if(loop!=NULL)
		loop.Release();
}

get_edge_data::get_edge_data(CComQIPtr <IEdge> my_edge,double chordTolerance,double lengthTolerance)
{
	edge=my_edge;

	start_point_flag=_T("起点无效");
	for(int i=0;i<(sizeof(start_point)/sizeof(start_point[0]));i++)
		start_point[i]=0;
	end_point_flag=_T("终点无效");
	for(int i=0;i<(sizeof(end_point)/sizeof(end_point[0]));i++)
		end_point[i]=0;
	
	curve_flag=_T("未知曲线");	
	parameter_number=0;
	for(int i=0;i<(sizeof(curve_parameter)/sizeof(curve_parameter[0]));i++)
		curve_parameter[i]=0;

	TessPtsSize=0;
	TessPts=NULL;

	if(edge->IGetStartVertex(&start_vertex)==S_OK)
		if(start_vertex!=NULL)
			if(start_vertex->IGetPoint(start_point)==S_OK)
				start_point_flag=_T("起点有效");
	
	if(edge->IGetEndVertex(&end_vertex)==S_OK)
		if(end_vertex!=NULL)
			if(end_vertex->IGetPoint(end_point)==S_OK)
				end_point_flag=_T("终点有效");;

	if(edge->IGetCurve(&curve)==S_OK)
		if(curve!=NULL){
			for(;;){
				VARIANT_BOOL flag;
				if(curve->IsLine(&flag)==S_OK){
					if(flag)
						if(curve->get_ILineParams(curve_parameter)==S_OK){
							parameter_number=6;
							curve_flag=_T("直线");
							break;
						}
				}
				if(curve->IsCircle(&flag)==S_OK){
					if(flag)
						if(curve->get_ICircleParams(curve_parameter)==S_OK){
							parameter_number=7;
							curve_flag=_T("圆");
							break;
						}
				}
				if(curve->IsEllipse(&flag)==S_OK){
					if(flag)
						if(curve->IGetEllipseParams(curve_parameter)==S_OK){
							parameter_number=11;
							curve_flag=_T("椭圆");	
							break;
						}
				}
				break;
			}
			long number;
			if(curve->IGetTessPtsSize(chordTolerance,lengthTolerance, start_point, end_point, &number)==S_OK)
				if(number>0){
					TessPts=new double [number];
					if(curve->IGetTessPts(chordTolerance,lengthTolerance,start_point,end_point,TessPts)==S_OK)
						TessPtsSize=(number/3);
					else{
						delete []TessPts;
						TessPts=NULL;
						TessPtsSize=0;
					}
				}
		}
}
get_edge_data::~get_edge_data()
{
	if(TessPts!=NULL)
		delete []TessPts;

	if(start_vertex!=NULL)
		start_vertex.Release();
	if(end_vertex!=NULL)
		end_vertex.Release();
	if(curve!=NULL)
		curve.Release();
	if(edge!=NULL)
		edge.Release();
	
}