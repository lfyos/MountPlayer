#include "data_creator.h"
#include   <comutil.h>     
#include   <stdio.h>     
#pragma   comment(lib,   "comsupp.lib")     


data_creator::data_creator(CComPtr <ISldWorks> sw,double quality,double chordTolerance,double lengthTolerance)
	:get_part_data(sw,quality)
{
	my_quality=quality;
	my_chordTolerance=chordTolerance;
	my_lengthTolerance=lengthTolerance;
}
data_creator::~data_creator(void)
{	
}
void data_creator::output_string(ofstream * f, CString str)
{
	CString tmp=str;
	BSTR   bstrText   =   tmp.AllocSysString();
	char*  lpszText2   =   _com_util::ConvertBSTRToString(bstrText);     
	(*f)<<lpszText2;

	SysFreeString(bstrText);
	delete []lpszText2;

	return;
}
int data_creator::register_material(material_data* m)
{
	int i;
	for(i=0;i<material_number;i++)
		if(material[i].is_equal(m))
			return i;
	if(m->material_flag){		
		material[material_number].get_data(m);
		material_number++;
		return material_number-1;
	}
	return -1;
}
CString data_creator::create_mesh_data()
{
	int i,j,k,part_m_id,body_m_id,face_m_id;
	float *p;
	
	CString str,file_name=path_name+_T(".mesh");

	ofstream f(file_name);

	my_triangle_number=0;
	material_number=0;
	get_material_and_texture();
	part_m_id=register_material(&(get_part_data::material));

	f<<"形体数          "<<body_count;

	for(i=0;i<body_count;i++){		
		get_body_data *b=new get_body_data(pBody[i]);		
		f<<"\n\n    第  "<<i<<"  个形体名称    ";
		data_creator::output_string(&f,b->name);
		f<<"    表面数    "<<(b->face_count);

		b->get_material_and_texture();
		body_m_id=register_material(&(b->material));

		for(j=0;j<b->face_count;j++){
			int texture_number;
			
			get_face_data *fa=new get_face_data(b->face[j]);
			fa->get_material_and_texture();
			face_m_id=register_material(&(fa->material));

			f<<"\n\n        第  "<<j<<"  个表面名称    ";			
			str.Format(_T("的第%d个表面"),j);
			data_creator::output_string(&f,b->name+str);	

			texture_number=(((fa->texture_object==NULL)&&(b->texture_object==NULL)&&(this->texture_object==NULL))?0:1);
			f<<"\n\n            纹理坐标数    "<<texture_number;

			f<<"\n\n            表面三角片顶点数    "<<3*(fa->TessTriangleCount);
			p=fa->TessTriangles;
			for(k=0;k<3*(fa->TessTriangleCount);k++,p=p+3){
				f<<"\n                第  "<<k<<"  个三角片顶点    "<<p[0]<<"    "<<p[1]<<"    "<<p[2];
			}

			f<<"\n\n            表面法线顶点数    "<<3*(fa->TessTriangleCount);
			p=fa->TessNorms;
			for(k=0;k<3*(fa->TessTriangleCount);k++,p=p+3){
				f<<"\n                第  "<<k<<"  个法线顶点    "<<p[0]<<"    "<<p[1]<<"    "<<p[2];
			}
			
			if(texture_number>0){
				f<<"\n\n            表面第 "<<0<<" 纹理坐标顶点数    "<<3*(fa->TessTriangleCount);
				p=fa->TessTexture;
				for(k=0;k<3*(fa->TessTriangleCount);k++,p=p+2){
					f<<"\n                第  "<<k<<"  个纹理坐标顶点    "<<p[0]<<"    "<<p[1]<<"    "<<0;
				}
			}

			f<<"\n\n            表面三角片数    "<<(fa->TessTriangleCount);
			for(k=0;k<(fa->TessTriangleCount);k++){
				my_triangle_number++;
				f<<"\n                第  "<<k<<"  个三角片材质编号        "<<((face_m_id>=0)?face_m_id:(body_m_id>=0)?body_m_id:part_m_id);
				f<<"\n                第  "<<k<<"  个三角片边线标志        "<<0;
				f<<"\n                第  "<<k<<"  个三角片顶点坐标索引    "<<3*k<<"    "<<3*k+1<<"    "<<3*k+2;
				f<<"\n                第  "<<k<<"  个三角片法线坐标索引    "<<3*k<<"    "<<3*k+1<<"    "<<3*k+2;
				if(texture_number>0)
					f<<"\n                第  "<<k<<"  个三角片第  0  个纹理坐标索引    "<<3*k<<"    "<<3*k+1<<"    "<<3*k+2;
				f<<"\n";
			}
			f<<"\n\n            表面类型    ";
			output_string(&f,fa->surface_str+_T("    参数个数    "));
			f<<(fa->parameter_number)<<"    参数";
			for(k=0;k<(fa->parameter_number);k++)
				f<<"    "<<(fa->surface_parameter[k]);

			f<<"\n\n            表面环路数    "<<(fa->loop_number);
			for(k=0;k<(fa->loop_number);k++){
				get_loop_data *my_loop=new get_loop_data(fa->loop[k]);
				f<<"\n\n                第  "<<k<<"  个表面环路边数        "<<(my_loop->edge_number);
				for(int m=0;m<(my_loop->edge_number);m++){
					get_edge_data *my_edge=new get_edge_data(my_loop->edge[m],my_chordTolerance,my_lengthTolerance);
					f<<"\n\n                    第  "<<m<<"  个表面环路边        ";
					output_string(&f,_T("\n                        ")+(my_edge->start_point_flag)+_T("    "));
					for(int n=0;n<(sizeof(my_edge->start_point)/sizeof(my_edge->start_point[0]));n++)
						f<<(my_edge->start_point[n])<<"    ";

					output_string(&f,_T("\n                        ")+(my_edge->end_point_flag)+_T("    "));
					for(int n=0;n<(sizeof(my_edge->end_point)/sizeof(my_edge->end_point[0]));n++)
						f<<(my_edge->end_point[n])<<"    ";

					output_string(&f,_T("\n                        曲线类型    ")+(my_edge->curve_flag)+_T("    参数个数    "));
					f<<(my_edge->parameter_number)<<"    参数";
					for(int n=0;n<(my_edge->parameter_number);n++)
						f<<"    "<<(my_edge->curve_parameter[n]);


					f<<"\n                        顶点个数    "<<(my_edge->TessPtsSize);
					for(int n=0;n<(my_edge->TessPtsSize);n++){
						f<<"\n                        第    "<<n<<"    个顶点坐标";
						f<<"    "<<(my_edge->TessPts[3*n])<<"    "<<(my_edge->TessPts[1+3*n])<<"    "<<(my_edge->TessPts[2+3*n]);
					}

					delete my_edge;
				}
				delete my_loop;
			}
			delete fa;
		}
		delete b;
	}
	f.close();
	return _T("\n网格文件名称:")+file_name;
}

CString data_creator::create_material_data()
{
	int i,j;
	CString str,file_name=path_name+".material";
	ofstream f(file_name);
	
	f<<"数目    "<<material_number;

	for(i=0;i<material_number;i++){
		f<<"\n\n第    "<<i<<"    种材质";
		f<<"\n        材质参数  ";
		for(j=0;j<sizeof(material[i].material)/sizeof(material[i].material[0]);j++)
			f<<material[i].material[j]<<"    ";

		if(material[i].texture_file_name==""){
			material[i].texture_file_name="没有纹理";
			material[i].rotation_angle=0;
			material[i].scale_ratio=1.0;
			material[i].blend_flag=FALSE;
		}else{
			str=material[i].texture_file_name;
			for(j=0;j<str.GetLength();j++)
				if(str.GetAt(j)=='\\'){
					material[i].texture_file_name=str.Right(str.GetLength()-j-1);
					break;
				}
		}
		
		f<<"\n        纹理文件名    ";
		data_creator::output_string(&f,material[i].texture_file_name);
		f<<"\n        旋转角度      "<<(material[i].rotation_angle);
		f<<"\n        放大倍数      "<<(material[i].scale_ratio);
		f<<"\n        混合标志      "<<(material[i].blend_flag?"混合":"不混合");

	}
	f<<"\n\n\n\n\n";
	f.close();

	return _T("\n材质文件名称:")+file_name;
}

bool data_creator::create_data(bool display_flag)
{
	if(pRootModelDoc==NULL)
		return true;
	if(pRootModelDoc_ext==NULL)
		return true;
	if(pPartDoc==NULL)
		return true;
	if(body_count<=0)
		return false;
	CString str,mstr=create_mesh_data();
	str.Format(_T("精度:%d\n三角片总数:%d"),(int)((long)my_quality),my_triangle_number);
	str+=mstr+create_material_data();
	
	if(display_flag)
		::MessageBox(NULL,str,_T("参数与结果"),MB_OK);

	return false;
}

void data_creator::create_all_data(CComPtr <ISldWorks> sw,double quality,double chordTolerance,double lengthTolerance)
{
	ofstream *f=NULL;
	for(;;){
		data_creator creator(sw,quality,chordTolerance,lengthTolerance);
		if(creator.create_data(false)){
			if(f!=NULL){
				f->close();
				delete f;
			}
			return;
		}
		if(f==NULL)
			f=new ofstream(creator.directory_name+_T("solidworks.part"));
		
		output_string(f,
						 creator.part_name+_T(            "\n                内部名称")
						+creator.path_name+_T(            "\n                网格文件")
						+creator.path_name+_T(       ".mesh\n                材质文件")
						+creator.path_name+_T(   ".material\n                介绍文件")
						+creator.path_name+_T(".description\n                配音文件")
						+creator.path_name+_T(      ".mp3\n\n\n"));
	}
}


	




