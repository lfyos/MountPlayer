#include "StdAfx.h"

#include "Location.h"

void Triangle::load(ifstream &f,int texture_number)
{
	int i;
	char buffer[MAX_BUFFER_LENGTH];

	f>>buffer>>buffer>>buffer>>material_id;
	f>>buffer>>buffer>>buffer>>edge_flag;
	f>>buffer>>buffer>>buffer>>(vertex.p0)>>(vertex.p1)>>(vertex.p2);
	f>>buffer>>buffer>>buffer>>(normal.p0)>>(normal.p1)>>(normal.p2);
	if(texture!=NULL)
		delete []texture;
	texture=NULL;
	if(texture_number>0){
		texture=new Triangle_point[texture_number];
		for(i=0;i<texture_number;i++)
			f>>buffer>>buffer>>buffer>>buffer>>buffer>>(texture[i].p0)>>(texture[i].p1)>>(texture[i].p2);
	}
	return;
};

void Triangle_set::load(ifstream &f,int texture_number)
{
	int i;
	char buffer[MAX_BUFFER_LENGTH];

	if(triangle_array!=NULL)
		delete []triangle_array;
	f>>buffer>>triangle_number;
	if(triangle_number<=0){
		triangle_number=0;
		triangle_array=NULL;
	}else{
		triangle_array=new Triangle[triangle_number];
		for(i=0;i<triangle_number;i++)
			triangle_array[i].load(f,texture_number);
	}
	return;
};

void Face::load(ifstream &f)
{
	int i;
	char buffer[MAX_BUFFER_LENGTH];

	f>>buffer>>buffer>>buffer>>buffer;
	name=buffer;
	f>>buffer>>texture_number;
	if(texture_number<0)
		texture_number=0;
	vertices.load(f);
	normals.load(f);
	if(textures!=NULL)
		delete []textures;
	textures=NULL;
	if(texture_number>0){
		textures=new Point_set[texture_number];
		for(i=0;i<texture_number;i++){
			f>>buffer>>buffer;
			textures[i].load(f);
		}
	}
	triangles.load(f,texture_number);

	face_curve.load(f);

	caculate_box();

	face_get_material_id_range();

	return;
}

void Body::load(ifstream &f)
{
	int i;
	char buffer[MAX_BUFFER_LENGTH];

	if(face_array!=NULL)
			delete []face_array;

	max_texture_number=0;

	f>>buffer>>buffer>>buffer>>buffer;
	name=buffer;
	f>>buffer>>face_number;
	if(face_number<=0){
		face_number=0;
		face_array=NULL;

	}else{
		face_array=new Face[face_number];

		for(i=0;i<face_number;i++){
			Face *fa=&(face_array[i]);
			fa->load(f);
			if(max_texture_number<(fa->texture_number))
				max_texture_number=fa->texture_number;
		}
	}
	caculate_box();
	body_get_material_id_range();

}
void Part::rude_part_load()
{
	int i;
	char buffer[MAX_BUFFER_LENGTH];
	ifstream f(directory_name+file_name);

	part_clear();
		
	f>>buffer>>body_number;
		
	if(body_number<=0){
			body_number=0;
			body_array=NULL;
	}else{
		body_array=new Body[body_number];
		for(i=0;i<body_number;i++){
			Body *b=&(body_array[i]);
			b->load(f);
			if(max_texture_number<(b->max_texture_number))
				max_texture_number=b->max_texture_number;
		}
	}

	f.close();
	caculate_box();
	caculate_triangle_number();
	part_get_material_id_range();
	return;
}

Part_with_shader::Part_with_shader(Part_parameter &par,Shader_base *my_part_shader,Part_with_shader *my_next_part)
{
	for(int i=0;i<(sizeof(paint_component_number)/sizeof(paint_component_number[0]));i++){
		paint_component_number[i]=0;
		paint_component_pointer[i]=NULL;
	}
	for(int i=0;i<(sizeof(paint_graphics_objects)/sizeof(paint_graphics_objects[0]));i++){
		paint_graphics_objects[i]=NULL;
	}

	has_load_flag=false;
	need_load_flag=false;
	information_has_been_released_flag=true;
	
	user_name=par.user_name;
	part_name=par.part_name;
	description_file_name=par.description_file_name;
	sound_file_name=par.sound_file_name;

	precision=par.precision;
	access_time=0;

	part_shader=my_part_shader;
	rude_part=new Part(par.directory_name,par.mesh_file_name);
	next_part=my_next_part;
}

Part_with_shader::~Part_with_shader()
{
	delete rude_part;
	delete part_shader;

	for(int i=0;i<(sizeof(paint_component_number)/sizeof(paint_component_number[0]));i++){
		if(paint_component_pointer[i]!=NULL){
			delete [](paint_component_pointer[i]);
			paint_component_pointer[i]=NULL;
		}
	}
	for(int i=0;i<(sizeof(paint_graphics_objects)/sizeof(paint_graphics_objects[0]));i++){
		if(paint_graphics_objects[i]!=NULL)
			delete (paint_graphics_objects[i]);
		paint_graphics_objects[i]=NULL;
	}
	if(next_part!=NULL)
		delete next_part;
}
void Part_with_shader::part_with_shader_release()
{
	need_load_flag=false;
	has_load_flag=false;
	information_has_been_released_flag=true;

	for(int i=0;i<(sizeof(paint_graphics_objects)/sizeof(paint_graphics_objects[0]));i++){
		if(paint_graphics_objects[i]!=NULL)
			delete (paint_graphics_objects[i]);
		paint_graphics_objects[i]=NULL;
	}
	if(rude_part!=NULL)
		rude_part->part_clear();
}

void Part_with_shader::part_with_shader_load(int release_memory_flag_map)
{
	need_load_flag=false;
	has_load_flag=true;
	information_has_been_released_flag=true;

	if((part_shader==NULL)||(rude_part==NULL))
		return;

	for(int i=0;i<(sizeof(paint_graphics_objects)/sizeof(paint_graphics_objects[0]));i++){
		if(paint_graphics_objects[i]!=NULL)
			delete (paint_graphics_objects[i]);
		paint_graphics_objects[i]=NULL;
	}

	rude_part->rude_part_load();

	for(int i=0;i<=(rude_part->max_texture_number);i++){
		Part_graphics_object *pg;	
		pg=new Part_graphics_object(rude_part,i,paint_graphics_objects[0]);
		if(pg->triangle_number<=0)
			delete pg;
		else
			paint_graphics_objects[0]=pg;
	}

	paint_graphics_objects[1]=new Part_graphics_object(&(rude_part->box),NULL);
	paint_graphics_objects[2]=new Part_graphics_object(rude_part,-1,NULL);
	paint_graphics_objects[3]=new Part_graphics_object(rude_part,-2,NULL);
	paint_graphics_objects[4]=new Part_graphics_object(rude_part,-3,NULL);

	for(int i=0;i<=4;i++){
		Part_graphics_object *pg=paint_graphics_objects[i];	
		if(pg!=NULL){
			if(pg->triangle_number<=0){
				delete pg;
				paint_graphics_objects[i]=NULL;
			}
		}
	}
	information_has_been_released_flag=rude_part->release_part_memory(release_memory_flag_map);
}

Shader_base *Part_set::add_part(Part_parameter &par)
{
	int i,index_id;
	Shader_base *clone_p;
	
	if(part_number>=MAX_PART_NUMBER)
		return NULL;
	if((clone_p=Shader_base::system_clone_shader(par))==NULL)
		return NULL;
	for(i=0;i<part_number;i++)
		if(part_array[i]->part_name==par.part_name){
			Part_with_shader *ps=new Part_with_shader(par,clone_p,part_array[i]);
			for(part_array[i]=ps;ps->next_part!=NULL;ps=ps->next_part){
				if((ps->precision)>=(ps->next_part->precision))
					break;
				ps->exchange_part_with_shader(ps->next_part);
			}
			return clone_p;
		}
	part_array[part_number]=new Part_with_shader(par,clone_p,NULL);
	part_index[part_number]=part_number;
				
	for(i=part_number;i>0;i--){
		if((part_array[part_index[i-1]]->part_name)<(part_array[part_index[i]]->part_name))
			break;
		index_id       =part_index[i-1];
		part_index[i-1]=part_index[i  ];
		part_index[i  ]=index_id;				
	}
	part_number++;

	return clone_p;
}
bool Face::flush_for_create_assemble_part(ofstream &f,Location loca,int material_id_base)
{
	CString str;

	str.Format(_T("\n\n            纹理坐标数    %d"),texture_number);
	File_data::output_string(f,str);

	if(vertices.point_number>0)
		if(vertices.vertex_array==NULL){
			AfxMessageBox(_T("顶点信息已经释放，无法生成装配体"));
			return true;
		}
	str.Format(_T("\n\n            表面三角片顶点数    %d"),vertices.point_number);
	File_data::output_string(f,str);
	for(int i=0;i<vertices.point_number;i++){
		Point p=loca*(vertices.vertex_array[i]);
		str.Format(_T("\n                第  %d  个三角片顶点    %lf    %lf    %lf"),i,p.x,p.y,p.z);
		File_data::output_string(f,str);
	}

	if(normals.point_number>0)
		if(normals.vertex_array==NULL){
			AfxMessageBox(_T("法线信息已经释放，无法生成装配体"));
			return true;
		}

	str.Format(_T("\n\n            表面法线顶点数    %d"),normals.point_number);
	File_data::output_string(f,str);
	for(int i=0;i<normals.point_number;i++){
		Point p=(loca*(normals.vertex_array[i]))-(loca*Point(0,0,0));
		str.Format(_T("\n                第  %d  个法线顶点    %lf    %lf    %lf"),i,p.x,p.y,p.z);
		File_data::output_string(f,str);
	}

	for(int i=0;i<texture_number;i++){
		str.Format(_T("\n\n            表面第  %d  纹理坐标顶点数    %d"),i,textures[i].point_number);
		File_data::output_string(f,str);

		if(textures[i].point_number>0)
			if(textures[i].vertex_array==NULL){
				AfxMessageBox(_T("纹理信息已经释放，无法生成装配体"));
				return true;
			}

		for(int j=0;j<(textures[i].point_number);j++){
			Point p=textures[i].vertex_array[j];
			str.Format(_T("\n                第  %d  个纹理坐标顶点    %lf    %lf    %lf"),j,p.x,p.y,p.z);
			File_data::output_string(f,str);			
		}
	}

	str.Format(_T("\n\n            表面三角片数    %d"),triangles.triangle_number);
	File_data::output_string(f,str);

	if(triangles.triangle_number>0){
		if(triangles.triangle_array==NULL){
			AfxMessageBox(_T("三角片信息已经释放，无法生成装配体"));
			return true;
		}
		for(int i=0;i<triangles.triangle_number;i++){
			str.Format(_T("\n                第  %d  个三角片材质编号        %d"),i,triangles.triangle_array[i].material_id+material_id_base);
			File_data::output_string(f,str);
			
			str.Format(_T("\n                第  %d  个三角片边线标志        %d"),i,triangles.triangle_array[i].edge_flag);
			File_data::output_string(f,str);

			str.Format(_T("\n                第  %d  个三角片顶点坐标索引        %d        %d        %d"),i,triangles.triangle_array[i].vertex.p0,triangles.triangle_array[i].vertex.p1,triangles.triangle_array[i].vertex.p2);
			File_data::output_string(f,str);
			
			str.Format(_T("\n                第  %d  个三角片法线坐标索引        %d        %d        %d"),i,triangles.triangle_array[i].normal.p0,triangles.triangle_array[i].normal.p1,triangles.triangle_array[i].normal.p2);
			File_data::output_string(f,str);

			for(int j=0;j<texture_number;j++){
				str.Format(_T("\n                第  %d  个三角片第  %d  个纹理坐标索引    %d    %d    %d"),i,j,
					triangles.triangle_array[i].texture[j].p0,triangles.triangle_array[i].texture[j].p1,triangles.triangle_array[i].texture[j].p2);				
				File_data::output_string(f,str);
			}
		}
	}

	str=_T("\n\n            表面类型    ");
	switch(face_curve.face_type){
	case 0:
		File_data::output_string(f,str+_T("平面     参数个数   6    参数      "));
		{
			Point p(face_curve.face_parameter[0],face_curve.face_parameter[1],face_curve.face_parameter[2]);
			Point q(face_curve.face_parameter[3],face_curve.face_parameter[4],face_curve.face_parameter[5]);
			p=(loca*p)-(loca*Point(0,0,0));
			q=loca*q;

			str.Format(_T("%lf    %lf     %lf      %lf    %lf     %lf"),p.x,p.y,p.z,q.x,q.y,q.z);
			File_data::output_string(f,str);
		}
		break;
	case 1:
		File_data::output_string(f,str+_T("柱面     参数个数   7    参数      "));
		{
			Point p(face_curve.face_parameter[0],face_curve.face_parameter[1],face_curve.face_parameter[2]);
			Point q(face_curve.face_parameter[3],face_curve.face_parameter[4],face_curve.face_parameter[5]);

			p=loca*p;
			q=(loca*q)-(loca*Point(0,0,0));
			str.Format(_T("%lf    %lf     %lf      %lf    %lf     %lf    %lf"),p.x,p.y,p.z,q.x,q.y,q.z,face_curve.face_parameter[6]);
			File_data::output_string(f,str);
		}
		break;
	case 2:
		File_data::output_string(f,str+_T("锥面     参数个数   8    参数      "));
		{
			Point p(face_curve.face_parameter[0],face_curve.face_parameter[1],face_curve.face_parameter[2]);
			Point q(face_curve.face_parameter[3],face_curve.face_parameter[4],face_curve.face_parameter[5]);

			p=loca*p;
			q=(loca*q)-(loca*Point(0,0,0));
			str.Format(_T("%lf    %lf     %lf      %lf    %lf     %lf    %lf    %lf"),p.x,p.y,p.z,q.x,q.y,q.z,face_curve.face_parameter[6],face_curve.face_parameter[7]);
			File_data::output_string(f,str);
		}
		break;
	case 3:
		File_data::output_string(f,str+_T("球面     参数个数   4    参数      "));
		{
			Point p(face_curve.face_parameter[0],face_curve.face_parameter[1],face_curve.face_parameter[2]);			
			p=loca*p;
			str.Format(_T("%lf    %lf     %lf      %lf "),p.x,p.y,p.z,face_curve.face_parameter[3]);
			File_data::output_string(f,str);
		}
		break;
	case 4:
		File_data::output_string(f,str+_T("环面     参数个数   8    参数      "));
		{
			Point p(face_curve.face_parameter[0],face_curve.face_parameter[1],face_curve.face_parameter[2]);
			Point q(face_curve.face_parameter[3],face_curve.face_parameter[4],face_curve.face_parameter[5]);

			p=loca*p;
			q=(loca*q)-(loca*Point(0,0,0));
			str.Format(_T("%lf    %lf     %lf      %lf    %lf     %lf    %lf    %lf"),p.x,p.y,p.z,q.x,q.y,q.z,face_curve.face_parameter[6],face_curve.face_parameter[7]);
			File_data::output_string(f,str);
		}
		break;
	default:
		File_data::output_string(f,str+_T("未知曲面"));
		str.Format(_T("     参数个数    %d    参数"),face_curve.face_parameter_number);
		File_data::output_string(f,str);
		for(int i=0;i<(face_curve.face_parameter_number);i++){
			str.Format(_T("    %lf"),face_curve.face_parameter[i]);
			File_data::output_string(f,str);
		}
	}

	str.Format(_T("\n\n            表面环路数    %d"),face_curve.face_loop_number);
	File_data::output_string(f,str);

	for(int i=0;i<(face_curve.face_loop_number);i++){
		str.Format(_T("\n\n                第  %d  个表面环路边数        %d"),i,face_curve.face_loop[i].edge_number);
		File_data::output_string(f,str);
		for(int j=0;j<(face_curve.face_loop[i].edge_number);j++){
			Face_edge *fe=&(face_curve.face_loop[i].edge[j]);
			if(fe->tessellation_point_number>0)
				if(fe->tessellation==NULL){
					AfxMessageBox(_T("环路边信息已经释放，无法生成装配体"));
					return true;
				}
			str.Format(_T("\n\n                    第  %d  个表面环路边 "),j);
			File_data::output_string(f,str);
			if(fe->start_effective_flag){
				Point p=loca*(fe->start_point);
				str.Format(_T("\n                        起点有效    %lf    %lf    %lf"),p.x,p.y,p.z);
			}else
				str=_T("\n                        起点无效    0    0    0");
			File_data::output_string(f,str);

			if(fe->end_effective_flag){
				Point p=loca*(fe->end_point);
				str.Format(_T("\n                        终点有效    %lf    %lf    %lf"),p.x,p.y,p.z);
			}else
				str=_T("\n                        终点无效    0    0    0");
			File_data::output_string(f,str);
			
			switch(fe->curve_type){
			case 0:
				File_data::output_string(f,_T("\n                        曲线类型    直线    参数个数    6    参数    "));
				{
					Point p(fe->curve_parameter[0],fe->curve_parameter[1],fe->curve_parameter[2]);
					Point q(fe->curve_parameter[3],fe->curve_parameter[4],fe->curve_parameter[5]);

					p=loca*p;
					q=(loca*q)-(loca*Point(0,0,0));
					str.Format(_T("%lf    %lf     %lf      %lf    %lf     %lf"),p.x,p.y,p.z,q.x,q.y,q.z);
					File_data::output_string(f,str);
				}
				break;
			case 1:
				File_data::output_string(f,_T("\n                        曲线类型    圆    参数个数    7    参数    "));
				{
					Point p(fe->curve_parameter[0],fe->curve_parameter[1],fe->curve_parameter[2]);
					Point q(fe->curve_parameter[3],fe->curve_parameter[4],fe->curve_parameter[5]);

					p=loca*p;
					q=(loca*q)-(loca*Point(0,0,0));
					str.Format(_T("%lf    %lf     %lf      %lf    %lf     %lf     %lf"),p.x,p.y,p.z,q.x,q.y,q.z,fe->curve_parameter[6]);
					File_data::output_string(f,str);
				}
				break;
			case 2:
				File_data::output_string(f,_T("\n                        曲线类型    椭圆    参数个数        11    参数    "));
				{
					Point p(fe->curve_parameter[0],fe->curve_parameter[1],fe->curve_parameter[2]);
					Point q(fe->curve_parameter[4],fe->curve_parameter[5],fe->curve_parameter[6]);
					Point r(fe->curve_parameter[8],fe->curve_parameter[9],fe->curve_parameter[10]);

					p=loca*p;
					q=(loca*q)-(loca*Point(0,0,0));
					r=(loca*r)-(loca*Point(0,0,0));

					str.Format(_T("%lf    %lf     %lf      %lf     %lf     %lf     %lf      %lf    %lf     %lf     %lf"),
						p.x,p.y,p.z,
						fe->curve_parameter[3],q.x,q.y,q.z,
						fe->curve_parameter[7],r.x,r.y,r.z);
					File_data::output_string(f,str);
				}
				break;
			default:
				File_data::output_string(f,_T("\n                        曲线类型    未知曲线    参数个数    "));
				str.Format(_T("%d    参数"),fe->curve_parameter_number);
				File_data::output_string(f,str);
				for(int k=0;k<(fe->curve_parameter_number);k++){
					str.Format(_T("    %lf"),fe->curve_parameter[k]);
					File_data::output_string(f,str);
				}
			}

			str.Format(_T("\n                        顶点个数    %d"),fe->tessellation_point_number);
			File_data::output_string(f,str);

			for(int k=0;k<(fe->tessellation_point_number);k++){
				Point p=loca*(fe->tessellation[k]);
				str.Format(_T("\n                        第    %d    个顶点坐标    %lf    %lf    %lf"),k,p.x,p.y,p.z);
				File_data::output_string(f,str);
			}
		}
	}
	File_data::output_string(f,_T("\n\n"));
	return false;
}
bool Part::flush_for_create_assemble_part(ofstream &f,int &body_id_base,Location loca,int material_id_base)
{
	for(int i=0;i<body_number;i++){
		CString str;
		Body *body=&(body_array[i]);
		str.Format(_T("\n\n    第  %d  个形体名称    形体%d"),i+body_id_base,i+body_id_base);
		File_data::output_string(f,str);

		str.Format(_T("    表面数    %d"),body->face_number);
		File_data::output_string(f,str);

		for(int j=0;j<(body->face_number);j++){
			Face *fa=&(body->face_array[j]);
			str.Format(_T("\n\n         第  %d  个表面名称    "),j);
			File_data::output_string(f,str+(fa->name));
			if(fa->flush_for_create_assemble_part(f,loca,material_id_base))
				return true;
		}
	}
	body_id_base+=body_number;

	return false;
}

void Part_set::list_all_part(ofstream &f_part_simple,ofstream &f_part_complete)
{
		int i,j,n;
		CString str;
		Part_with_shader *ps,*mps;

		for(i=0;i<part_number;i++){
			for(n=0,ps=part_array[part_index[i]];ps!=NULL;ps=ps->next_part)
				n++;

			if(n>1){
				str.Format(_T("\n\n\n******************************共有%d个相同杆件************************"),n);
				File_data::output_string(f_part_simple,str);
				File_data::output_string(f_part_complete,str);
			}
			str.Format(_T("\n%d"),i+1);			
			File_data::output_string(f_part_simple,str);
			File_data::output_string(f_part_complete,str);
			
			ps=part_array[j=part_index[i]];
			File_data::output_string(f_part_simple,_T("	")+(ps->user_name)+_T("	")+ps->part_name);
			if(part_merge[j]!=j){
				mps=part_array[part_merge[j]];
				File_data::output_string(f_part_simple,_T("		########")+(mps->user_name)+_T("########")+mps->part_name);
			}


			for(str=_T("");ps!=NULL;ps=ps->next_part,str=str+_T("		")){
				File_data::output_string(f_part_complete,_T("\n")+str+_T("用户名称:")+ps->user_name);
				File_data::output_string(f_part_complete,_T("\n")+str+_T("内部名称:")+ps->part_name);			
				File_data::output_string(f_part_complete,_T("\n")+str+_T("网格文件:")+ps->rude_part->file_name);
				File_data::output_string(f_part_complete,_T("\n")+str+_T("材质文件:")+ps->part_shader->file_name);
				File_data::output_string(f_part_complete,_T("\n")+str+_T("介绍文件:")+ps->description_file_name);
				File_data::output_string(f_part_complete,_T("\n")+str+_T("配音文件:")+ps->sound_file_name);

				File_data::output_string(f_part_complete,_T("\n")+str+_T("着色语言:"));
				CString *program_str=ps->part_shader->get_program_file_name();
				for(j=0;j<MAX_SHADER_NUMBER;j++)
					File_data::output_string(f_part_complete,_T("\n")+str+_T("	")+program_str[j]);

				File_data::output_string(f_part_complete,_T("\n\n"));
			}
			j=part_index[i];
			if(part_merge[j]!=j){
				ps=part_array[part_merge[j]];
				for(str=_T("");ps!=NULL;ps=ps->next_part,str=str+_T("	")){
					File_data::output_string(f_part_complete,_T("\n")+str+_T("########用户名称:")+ps->user_name);
					File_data::output_string(f_part_complete,_T("\n")+str+_T("########内部名称:")+ps->part_name);			
					File_data::output_string(f_part_complete,_T("\n")+str+_T("########网格文件:")+ps->rude_part->file_name);
					File_data::output_string(f_part_complete,_T("\n")+str+_T("########材质文件:")+ps->part_shader->file_name);
					File_data::output_string(f_part_complete,_T("\n")+str+_T("########介绍文件:")+ps->description_file_name);
					File_data::output_string(f_part_complete,_T("\n")+str+_T("########配音文件:")+ps->sound_file_name);

					File_data::output_string(f_part_complete,_T("\n")+str+_T("########着色语言:"));
					CString *program_str=ps->part_shader->get_program_file_name();
					for(j=0;j<MAX_SHADER_NUMBER;j++)
						File_data::output_string(f_part_complete,_T("\n	")+str+_T("########")+program_str[j]);

					File_data::output_string(f_part_complete,_T("\n\n"));
				}

			}
		}
}

void Part_set::merge_same_part(CString same_part_file_name)
{
	ifstream f(same_part_file_name);

	for(int i=0;i<MAX_PART_NUMBER;i++)
		part_merge[i]=i;

	if((part_number<=1)||(f.eof())||(f.fail()))
		return;

	class temp{
	public:
		temp *front,*back;

		bool has_done_flag;

		int destination_part_id;

		temp()
		{
			front=this;
			back=this;

			has_done_flag=false;
		}
	}*part_reference=new temp[part_number];

	for(int i=0;i<part_number;i++)
		part_reference[i].destination_part_id=i;

	do{
		int source_part_id,destination_part_id;
		char buffer[MAX_BUFFER_LENGTH];

		f>>buffer;	source_part_id=get_part_id(CString(buffer));
		f>>buffer;	destination_part_id=get_part_id(CString(buffer));
		if((f.eof())||(f.fail()))
			break;
		if((source_part_id>=0)&&(destination_part_id>=0)&&(source_part_id!=destination_part_id)){
			temp *source_this=&(part_reference[source_part_id]),*source_front=source_this->front;
			temp *destination_this=&(part_reference[destination_part_id]),*destination_front=destination_this->front;
			
			source_this->front=destination_front;			destination_front->back=source_this;
			source_front->back=destination_this;			destination_this->front=source_front;
		}
	}while(true);

	for(int i=0;i<part_number;i++){
		temp *p=&(part_reference[i]);
		if(!(p->has_done_flag)){
			int destination_part_id=p->destination_part_id;
			do{
				if(p->destination_part_id<destination_part_id)
					destination_part_id=p->destination_part_id;
			}while((p=p->back)!=(&(part_reference[i])));

			do{
					p->destination_part_id=destination_part_id;
					p->has_done_flag=true;
			}while((p=p->back)!=(&(part_reference[i])));
		}
	}

	for(int i=0;i<part_number;i++)
		part_merge[i]=part_reference[i].destination_part_id;

	delete []part_reference;

	return;
}