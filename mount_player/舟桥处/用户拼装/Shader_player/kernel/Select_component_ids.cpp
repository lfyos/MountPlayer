#include "StdAfx.h"
#include "Select_component_ids.h"
#include "Camera.h"

void Select_component_ids::compute_pointer(Part_set *parts)
{
	part_with_shader=NULL;
	part_shader=NULL;
	rude_part=NULL;
	body=NULL;
	face=NULL;
	face_loop=NULL;
	edge=NULL;

	box=Box();
	has_box_flag=false;

	if(comp==NULL){
		body_id=-5;
		face_id=-5;
		vertex_id=-5;
		loop_id=-5;
		edge_id=-5;
		edge_point_id=-5;
		return ;
	}
	if((comp->part_id<0)||((comp->part_id)>=(parts->part_number))){
		body_id=-5;
		face_id=-5;
		vertex_id=-5;
		loop_id=-5;
		edge_id=-5;
		edge_point_id=-5;
		return ;
	}
	if((part_with_shader=parts->part_array[comp->part_id])==NULL){
		body_id=-5;
		face_id=-5;
		vertex_id=-5;
		loop_id=-5;
		edge_id=-5;
		edge_point_id=-5;
		return;
	}
	part_shader=part_with_shader->part_shader;
	if((rude_part=part_with_shader->rude_part)==NULL){
		body_id=-5;
		face_id=-5;
		vertex_id=-5;
		loop_id=-5;
		edge_id=-5;
		edge_point_id=-5;
		return;
	}

	Shader_part_information *info=part_with_shader->part_shader->get_part_information();

	if((body_id<0)||(body_id>=(rude_part->body_number))){
		body_id=-5;
		face_id=-5;
		vertex_id=-5;
		loop_id=-5;
		edge_id=-5;
		edge_point_id=-5;

		if(info->shader_has_box_flag){
			has_box_flag=true;
			box=(comp->absulate_location)*(rude_part->box);
		}
		return;
	}

	body=&(rude_part->body_array[body_id]);
	if((face_id<0)||(face_id>=(body->face_number))){
		face_id=-5;
		vertex_id=-5;
		loop_id=-5;
		edge_id=-5;
		edge_point_id=-5;

		if(info->shader_has_box_flag)
			if(body->has_box_flag){
				has_box_flag=true;
				box=(comp->absulate_location)*(body->box);
			}
		return ;
	}
	face=&(body->face_array[face_id]);
	if((loop_id<0)||(loop_id>=(face->face_curve.face_loop_number))){
		loop_id=-5;
		edge_id=-5;
		edge_point_id=-5;
		
		if(info->shader_has_box_flag)
			if(face->has_box_flag){
				has_box_flag=true;
				box=(comp->absulate_location)*(face->box);
			}
		return ;
	}
	face_loop=&(face->face_curve.face_loop[loop_id]);
	if((edge_id<0)||(edge_id>=face_loop->edge_number)){
		edge_id=-5;
		edge_point_id=-5;

		if(info->shader_has_box_flag)
			if(face_loop->has_box_flag){
				has_box_flag=true;
				box=(comp->absulate_location)*(face_loop->box);
			}
		return;
	}
	edge=&(face_loop->edge[edge_id]);

	if(info->shader_has_box_flag)
		if(face_loop->has_box_flag){
			has_box_flag=true;
			box=(comp->absulate_location)*(edge->box);
		}
	
	start_point=(comp->absulate_location)*(edge->start_point);
	end_point=(comp->absulate_location)*(edge->end_point);

	switch(edge_point_id){
	case 0:
	case 1:
		break;
	case 2:
		switch(edge->curve_type){
		case 1:		
			break;
		case 0:
		case 2:
		default:
			edge_point_id=-5;
			break;
		}
		break;
	default:
			edge_point_id=-5;
	}
	return ;
}
CString Select_component_ids::get_part_description(Part_set *parts)
{
	CString str=_T(""),tmp;

	compute_pointer(parts);

	if(comp==NULL)
		return str;
	if(part_with_shader==NULL)
		return str;
	str=str+part_with_shader->user_name;
	if((rude_part==NULL)||(part_shader==NULL))
		return str+_T(":没有装入");
	if(body==NULL)
		return str;
	tmp.Format(_T(":体[%d/%d]"),body_id,rude_part->body_number);
	str=str+tmp;
	if(face==NULL)
		return str;
	tmp.Format(_T(",面[%d/%d"),face_id,body->face_number);
	str=str+tmp;
	switch(face->face_curve.face_type){
	case 0:
		str=str+_T("平面]");
		break;
	case 1:
		str=str+_T("柱面]");
		break;
	case 2:
		str=str+_T("锥面]");
		break;
	case 3:
		str=str+_T("球面]");
		break;
	case 4:
		str=str+_T("环面]");
		break;
	default:
		str=str+_T("]");
		break;
	}
	if((vertex_id<0)||(vertex_id>=(face->triangles.triangle_number*3)))
		return str;
	tmp.Format(_T(",三角片顶点[%d/%d]"),vertex_id,face->triangles.triangle_number*3);
	str=str+tmp;

	if(face_loop==NULL)
		return str;
	tmp.Format(_T(",回路[%d/%d]"),loop_id,face->face_curve.face_loop_number);
	str=str+tmp;

	if(edge==NULL)
		return str;
	tmp.Format(_T(",边[%d/%d "),edge_id,face_loop->edge_number);
	str=str+tmp;
	
	switch(edge->curve_type){
	case 0:
		{
			switch(edge_point_id){
			case 0:
				tmp.Format(_T("起点(%lf,%lf,%lf)]"),start_point.x,start_point.y,start_point.z);
				break;
			case 1:
				tmp.Format(_T("终点(%lf,%lf,%lf)]"),end_point.x,end_point.y,end_point.z);
				break;
			default:
				tmp.Format(_T("起点(%lf,%lf,%lf) 终点(%lf,%lf,%lf)]"),start_point.x,start_point.y,start_point.z,end_point.x,end_point.y,end_point.z);
				break;
			} 
			str=str+_T("直线:")+tmp;
		}
		break;
	case 1:
		{
			Point center_point=(comp->absulate_location)*Point(edge->curve_parameter[0],edge->curve_parameter[1],edge->curve_parameter[2]);
			Point direction=((comp->absulate_location)*Point(edge->curve_parameter[3],edge->curve_parameter[4],edge->curve_parameter[5]))-(comp->absulate_location*Point(0,0,0));
			tmp.Format(_T("圆心(%lf,%lf,%lf) 方向(%lf,%lf,%lf) 半径%lf]"),center_point.x,center_point.y,center_point.z,direction.x,direction.y,direction.z,edge->curve_parameter[6]);
			str=str+tmp;				
		}
		if(edge->start_effective_flag){
			tmp.Format(_T("起点(%lf,%lf,%lf)]"),start_point.x,start_point.y,start_point.z);
			str=str+tmp;
		}
		if(edge->end_effective_flag){
			tmp.Format(_T("终点(%lf,%lf,%lf)]"),end_point.x,end_point.y,end_point.z);
			str=str+tmp;
		}
		break;
	case 2:
		str=str+_T("椭圆");
		break;
	}
	return str;
}
void Size_tag::add_tag_point(Select_component_ids &ids,Part_set &parts,int my_tag_direction_flag)
{
		if(finish_flag)
			return;
		tag_direction_flag=my_tag_direction_flag;

		ids.compute_pointer(&parts);
		switch(state_id){
		case 0://刚开始,什么也没有输入
			if(ids.edge_id>=0){
				first_tag.copy_from(ids);
				if(ids.edge_point_id<0)
					state_id=1;				//输入了第一个tag,输入的是一条线
				else
					state_id=2;				//输入了第一个tag,输入的是一个点
			}
			break;
		case 1://输入了第一个tag,输入的是一条线
			if(ids.edge_id<0){
				state_id=3;						//对单线的标注
				finish_flag=true;
			}else{
				if(   ((ids.comp)==(first_tag.comp))
					&&((ids.body_id)==(first_tag.body_id))
					&&((ids.face_id)==(first_tag.face_id))
					&&((ids.loop_id)==(first_tag.loop_id))
					&&((ids.edge_id)==(first_tag.edge_id))
					&&(ids.edge_point_id>=0))
				{
					first_tag.copy_from(ids);
					state_id=2;					//输入了第一个tag,输入的是一个点
				}else if(ids.edge_point_id<0){
					second_tag.copy_from(ids);
					state_id=4;					//输入了第二个tag,第一次输入的是线,第二次输入的也是一条线,线线标注
				}else{
					second_tag.copy_from(ids);	
					state_id=5;					//输入了第二个tag,第一次输入的是线,第二次输入的是点,线点标注
				}
			}
			break;
		case 2://输入了第一个tag,输入的是一个点
			if(ids.edge_id>=0){
				if(   ((ids.comp)!=(first_tag.comp))
					||((ids.body_id)!=(first_tag.body_id))||((ids.face_id)!=(first_tag.face_id))
					||((ids.loop_id)!=(first_tag.loop_id))||((ids.edge_id)!=(first_tag.edge_id))
					||((ids.edge_point_id)!=(first_tag.edge_point_id)))
				{
			
					if(ids.edge_point_id<0){
						second_tag.copy_from(ids);
						state_id=6;					//输入了第二个tag,第一次输入的是点,第二次输入的是一条线,点线标注
					}else{
						second_tag.copy_from(ids);
						state_id=7;					//输入了第二个tag,第一次输入的是点,第二次输入的也是一个点,点点标注
					}
				}
			}
			break;
		case 3://对单线的标注
		case 4://线线的标注
		case 5://线点的标注
		case 6://点线的标注
		case 7://点点的标注

		case 8://修改屏幕位置
			finish_flag=true;
			break;
		default:
			reset();
			break;
		}
}


bool Size_tag::modify_tag_screen_location(int my_tag_direction_flag)
{
		if(finish_flag)
			switch(state_id){
			case 3://对单线的标注
			case 4://线线的标注
			case 5://线点的标注
			case 6://点线的标注
			case 7://点点的标注
			case 8:
				tag_direction_flag=my_tag_direction_flag;
				state_id=8;
				finish_flag=false;
				return true;
			default:		
				break;
			}
		return false;
}

bool  Size_tag::get_tag_point(Point &first_point,Point &second_point)
{
		switch(state_id){
		case 0://刚开始,什么也没有输入
			return false;
		case 1://输入了第一个tag,输入的是一条线
		case 3://对单线的标注
			switch(first_tag.edge->curve_type){
			case 0:
				first_point=first_tag.start_point;
				second_point=first_tag.end_point;
				return true;
			case 1:
				if((first_tag.edge->tessellation_point_number<=0)||(first_tag.edge->tessellation==NULL)){
					reset();
					return false;
				}else{
					first_point=(first_tag.comp->absulate_location)*Point(first_tag.edge->curve_parameter[0],first_tag.edge->curve_parameter[1],first_tag.edge->curve_parameter[2]);
					second_point=(first_tag.comp->absulate_location)*(first_tag.edge->tessellation[first_tag.edge->tessellation_point_number/2]);
					second_point=first_point+(second_point-first_point).expand(first_tag.edge->curve_parameter[6]);
					return true;
				}
			default:
				reset();
				return false;
			}
		case 2://输入了第一个tag,输入的是一个点
			return false;
		case 4://线线标注
			return get_line_line_tag_point(first_point,second_point);
		case 5://线点标注
			return get_line_point_tag_point(first_point,second_point);
		case 6://点线标注
			return get_point_line_tag_point(first_point,second_point);
		case 7://点点标注	
			return get_point_point_tag_point(first_point,second_point);

		case 8://修改屏幕位置
			first_point=tag_point[0];
			second_point=tag_point[1];

			return true;
		default:
			reset();
			return false;
		}
}

bool Size_tag::get_point_point_tag_point(Point &first_point,Point &second_point)
{
		switch(first_tag.edge_point_id){
		case 0:
			first_point=first_tag.start_point;
			break;
		case 1:
			first_point=first_tag.end_point;
			break;
		case 2:
			switch(first_tag.edge->curve_type){
			case 1:	
				first_point=(first_tag.comp->absulate_location)*Point(first_tag.edge->curve_parameter[0],first_tag.edge->curve_parameter[1],first_tag.edge->curve_parameter[2]);
				break;
			case 0:
			case 2:
			default:
				reset();
				return false;
			}
			break;
		default:
			reset();
			return false;
		}
		switch(second_tag.edge_point_id){
		case 0:
			second_point=second_tag.start_point;
			break;
		case 1:
			second_point=second_tag.end_point;
			break;
		case 2:
			switch(second_tag.edge->curve_type){
			case 1:	
				second_point=(second_tag.comp->absulate_location)*Point(second_tag.edge->curve_parameter[0],second_tag.edge->curve_parameter[1],second_tag.edge->curve_parameter[2]);
				break;
			case 0:
			case 2:
			default:
				reset();
				return false;
			}
			break;
		default:
			reset();
			return false;
		}
		return true;
}
bool Size_tag::get_point_line_tag_point(Point &first_point,Point &second_point)
{
		switch(first_tag.edge_point_id){
		case 0:
			first_point=first_tag.start_point;
			break;
		case 1:
			first_point=first_tag.end_point;
			break;
		case 2:
			switch(first_tag.edge->curve_type){
			case 1:	
				first_point=(first_tag.comp->absulate_location)*Point(first_tag.edge->curve_parameter[0],first_tag.edge->curve_parameter[1],first_tag.edge->curve_parameter[2]);
				break;
			case 0:
			case 2:
			default:
				reset();
				return false;
			}
			break;
		default:
			reset();
			return false;
		}
	
		switch(second_tag.edge->curve_type){
		case 0:
			second_point=caculate_min_distance_point(second_tag.start_point,second_tag.end_point,first_point);
			break;
		case 1:
			second_point=(second_tag.comp->absulate_location)*Point(second_tag.edge->curve_parameter[0],second_tag.edge->curve_parameter[1],second_tag.edge->curve_parameter[2]);
			break;
		default:
			reset();
			return false;
		}
		return true;
}
bool Size_tag::get_line_point_tag_point(Point &first_point,Point &second_point)
{
		switch(second_tag.edge_point_id){
		case 0:
			second_point=second_tag.start_point;
			break;
		case 1:
			second_point=second_tag.end_point;
			break;
		case 2:
			switch(second_tag.edge->curve_type){
			case 1:	
				second_point=(second_tag.comp->absulate_location)*Point(second_tag.edge->curve_parameter[0],second_tag.edge->curve_parameter[1],second_tag.edge->curve_parameter[2]);
				break;
			case 0:
			case 2:
			default:
				reset();
				return false;
			}
			break;
		default:
			reset();
			return false;
		}

		switch(first_tag.edge->curve_type){
		case 0:
			first_point=caculate_min_distance_point(first_tag.start_point,first_tag.end_point,second_point);
			break;
		case 1:
			first_point=(first_tag.comp->absulate_location)*Point(first_tag.edge->curve_parameter[0],first_tag.edge->curve_parameter[1],first_tag.edge->curve_parameter[2]);
			break;
		default:
			reset();
			return false;
		}
		return true;
}

bool Size_tag::get_line_line_tag_point(Point &first_point,Point &second_point)
{
	if(first_tag.edge->curve_type==1){
		first_point=(first_tag.comp->absulate_location)*Point(first_tag.edge->curve_parameter[0],first_tag.edge->curve_parameter[1],first_tag.edge->curve_parameter[2]);
		if(second_tag.edge->curve_type==1){
			second_point=(second_tag.comp->absulate_location)*Point(second_tag.edge->curve_parameter[0],second_tag.edge->curve_parameter[1],second_tag.edge->curve_parameter[2]);
			return true;
		}else if(second_tag.edge->curve_type==0){
			second_point=caculate_min_distance_point(second_tag.start_point,second_tag.end_point,first_point);
			return true;
		}else{
			reset();
			return false;
		}
	}else if(first_tag.edge->curve_type==0){
		if(second_tag.edge->curve_type==1){
			second_point=(second_tag.comp->absulate_location)*Point(second_tag.edge->curve_parameter[0],second_tag.edge->curve_parameter[1],second_tag.edge->curve_parameter[2]);
			first_point=caculate_min_distance_point(first_tag.start_point,first_tag.end_point,second_point);
			return true;
		}else if(second_tag.edge->curve_type==0){
			Point first_direction=first_tag.end_point-first_tag.start_point;
			Point second_direction=second_tag.end_point-second_tag.start_point;
			Point cross_direction=first_direction.cross(second_direction);
			if(cross_direction.distance2()<MIN_VALUE){	//平行线
				second_point=(second_tag.start_point+second_tag.end_point)*0.5;
				first_point=caculate_min_distance_point(first_tag.start_point,first_tag.end_point,second_point);
			}else{//相交线
				{
					Point plane_direction=second_direction.cross(cross_direction);
					double D=(-(plane_direction.dot(second_tag.start_point)));
					double t=(plane_direction.dot(first_tag.start_point)+D)/plane_direction.dot(first_direction);
					first_point=first_tag.start_point+(first_tag.start_point-first_tag.end_point)*t;
				}	
				{
					Point plane_direction=first_direction.cross(cross_direction);
					double D=(-(plane_direction.dot(first_tag.start_point)));
					double t=(plane_direction.dot(second_tag.start_point)+D)/plane_direction.dot(second_direction);
					second_point=second_tag.start_point+(second_tag.start_point-second_tag.end_point)*t;
				}
			}
		}else{
			reset();
			return false;
		}
	}else{
		reset();
		return false;
	}
	return true;
}
bool Size_tag::get_tag_point(double sx,double sy,Point eye_position,Location pro_loca)
{
	if(finish_flag)
		return true;

	if(!(get_tag_point(tag_point[0],tag_point[1])))
		return false;
	
	Point mouse_position(sx,sy,Graphics_system::object_to_screen((tag_point[0]+tag_point[1])*0.5,pro_loca).z);
	mouse_position=Graphics_system::screen_to_object(mouse_position,pro_loca);
	
	Point up_direction=(tag_point[0]-eye_position).cross(tag_point[1]-eye_position);
	if(up_direction.distance2()<MIN_VALUE){
		reset();
		return false;
	}

	Point plane_direction=(tag_point[1]-tag_point[0]).cross(mouse_position-eye_position);
	double t=plane_direction.dot(up_direction),D=plane_direction.dot(eye_position);

	if(::abs(t)<MIN_VALUE){
		reset();
		return false;
	}

	tag_point[2]=tag_point[1]-up_direction*((plane_direction.dot(tag_point[1])-D)/t);
	tag_point[3]=tag_point[0]-up_direction*((plane_direction.dot(tag_point[0])-D)/t);

	return true;
}


void  Size_tag::get_bitmap(Camera *camera)
{
		int i,j,tag_direction=-1;
		double distance;

		switch(tag_direction_flag){
		default:
			tag_direction_flag=0;
		case 0:		
			distance=(tag_point[1]-tag_point[0]).distance();
			tag_direction=-10;
			break;
		case 1:
			distance=::abs((tag_point[1]-tag_point[0]).x);
			tag_direction=11;
			break;
		case 2:
			distance=::abs((tag_point[1]-tag_point[0]).y);
			tag_direction=12;
			break;
		case 3:
			distance=::abs((tag_point[1]-tag_point[0]).z);
			tag_direction=13;
			break;
		case 4:
			{
				Point p=(camera->up_point)-(camera->center_point);
				Point q=(camera->eye_point)-(camera->center_point);
				distance=::abs((tag_point[1]-tag_point[0]).dot(p.cross(q).expand(1)));
				tag_direction=14;
			}
			break;
		case 5:
			{
				Point p=(camera->up_point)-(camera->center_point);
				distance=::abs((tag_point[1]-tag_point[0]).dot(p.expand(1)));
				tag_direction=15;
			}			
			break;
		case 6:
			{
				Point p=(camera->eye_point)-(camera->center_point);
				Point q=tag_point[1]-tag_point[0];
				distance=p.expand(1).dot(q);
				distance=::sqrtl(q.distance2()-distance*distance);
				
				tag_direction=16;
			}
			break;
		}
		CString str;
		str.Format(_T("%lf"),1000.0*distance);
		for(i=str.GetLength()-1;i>=0;i--){
			if(str.GetAt(i)!='0'){
				if(str.GetAt(i)=='.')
					str=str.Left(i+1)+_T("0");
				else
					str=str.Left(i+1);
				break;
			}
		}
		for(i=0;i<str.GetLength();i++){
			if(str.GetAt(i)!=' '){
				str=str.Right(str.GetLength()-i);
				break;
			}
		}
		if((str.GetLength()+1)>(sizeof(bitmap)/sizeof(bitmap[0])))
			str=str.Left(sizeof(bitmap)/sizeof(bitmap[0])-1);
		for(i=0;i<(sizeof(bitmap)/sizeof(bitmap[0]));i++)
			bitmap[i]=-10;
		j=(sizeof(bitmap)/sizeof(bitmap[0]))-(str.GetLength())-1;
		j=j/2;
		bitmap[j++]=tag_direction+100*tag_direction_flag;
		for(i=0;i<(str.GetLength());i++,j++){
			switch(str.GetAt(i)){
			default:
				if((str.GetAt(i)>='0')&&(str.GetAt(i)<='9'))
					bitmap[j]=(int)(str.GetAt(i))-(int)'0';
				break;
			case '.':
				bitmap[j]=10;
				break;
			}
			bitmap[j]+=100*tag_direction_flag;
		}
}