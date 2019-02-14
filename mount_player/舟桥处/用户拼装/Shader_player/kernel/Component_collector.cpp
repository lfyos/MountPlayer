#include "StdAfx.h"
#include "Component_collector.h"
#include "Camera.h"

static Component_collector_base *first_Component_collector=NULL;

Component_collector_base::Component_collector_base(void)
{
	if(first_Component_collector==NULL){
		first_Component_collector=this;
		front=this;
		back=this;
	}else{
		front=first_Component_collector;
		back=first_Component_collector->back;
		front->back=this;
		back->front=this;
	}
	component_number=0;
	component_pointer=NULL;

	box_component_number=0;
}
Component_collector_base::~Component_collector_base(void)
{
	clear();

	if(front==this)
		first_Component_collector=NULL;
	else{
		if(first_Component_collector==this)
			first_Component_collector=front;
		front->back=back;
		back->front=front;
	}
}
void Component_collector_base::clear_all()
{
	Component_collector_base *p;	
	if((p=first_Component_collector)!=NULL)
		do{
			p->clear();
		}while((p=p->back)!=first_Component_collector);
}
bool Part_collector::part_less_than(Component *s,Component *t)
{
	int s_id,t_id;
	if((s_id=s->part_id)<(t_id=t->part_id))
		return true;
	if(s_id>t_id)
		return false;
	if((s_id=s->movement_id)<(t_id=t->movement_id))
		return true;
	if(s_id>t_id)
		return false;
	Point ps=s->absulate_location*Point(0,0,0),pt=t->absulate_location*Point(0,0,0);
	if(::abs(ps.x-pt.x)>MIN_VALUE)
		return ((ps.x)<(pt.x))?true:false;
	if(::abs(ps.y-pt.y)>MIN_VALUE)
		return ((ps.y)<(pt.y))?true:false;
	if(::abs(ps.z-pt.z)>MIN_VALUE)
		return ((ps.z)<(pt.z))?true:false;

	return (((int)s)<((int)t))?true:false;
}
bool Part_collector::operate_component(Component *comp,bool my_selected_flag)
{
	if((comp->child==NULL)&&(comp->part_id>=0))
		if(ps->part_array[comp->part_id]->part_shader->get_part_information()->display_flag){
			register_component(comp);
			register_component_box(comp);
		}
	return true;
}

bool Part_collector::get_begin_and_end_part(int part_id,int &begin_index,int &end_index)
{
	if(component_number<=0)
		return false;
	begin_index=0;
	end_index=component_number-1;

	for(;begin_index<=end_index;){
		int middle_index=(begin_index+end_index)/2;

		if(component_pointer[middle_index]->part_id<part_id)
			begin_index=middle_index+1;
		else if(component_pointer[middle_index]->part_id>part_id)
			end_index=middle_index-1;
		else{
			for(begin_index=middle_index;begin_index>=0;begin_index--)
				if(component_pointer[begin_index]->part_id!=part_id){
					begin_index++;
					break;
				}
			if(begin_index<0)
				begin_index=0;
			for(end_index=middle_index;end_index<component_number;end_index++)
					if(component_pointer[end_index]->part_id!=part_id){
						end_index--;
						break;
					}
			if(end_index>=component_number)
				end_index=component_number-1;
			return true;
		}
	}
	return false;
}
void Part_collector::sort()
{
	int i,j,k;
	Component *p;
	if(component_number<=0)
		return;
	for(i=1;i<component_number;i++){
		for(j=i;j>0;j=k){
			k=(j-1)/2;
			if(part_less_than(component_pointer[j],component_pointer[k]))
				break;			
			p=component_pointer[j];
			component_pointer[j]=component_pointer[k];
			component_pointer[k]=p;
		}
	}	
	for(i=component_number-1;i>0;i--){
		p=component_pointer[0];
		component_pointer[0]=component_pointer[i];
		component_pointer[i]=p;
		for(j=0;;j=k){
			if((k=j+j+1)>=i)
				break;
			if((k+1)<i)
				if(part_less_than(component_pointer[k],component_pointer[k+1]))
					k=k+1;
			if(part_less_than(component_pointer[k],component_pointer[j]))
				break;
			p=component_pointer[j];
			component_pointer[j]=component_pointer[k];
			component_pointer[k]=p;
		}
	}
	for(i=0,j=0,k=0;i<component_number;i=j){
		for(j=i+1;j<component_number;j++)
			if((component_pointer[j])!=(component_pointer[i]))
				break;
		component_pointer[k++]=component_pointer[i];
	}
	component_number=k;
	return;
}


class  Component_display_state{
public:
	int shader_type_map;
	bool depth_map;
	bool blend_map;
	bool clip_map;
	bool polygon_offset;
	bool line_stipple;

	Component_display_state(
		int my_shader_type_map,
		bool my_depth_map,
		bool my_blend_map,
		bool my_clip_map,
		bool my_polygon_offset,
		bool my_line_stipple)
	{
		shader_type_map	=my_shader_type_map;
		depth_map		=my_depth_map;
		blend_map		=my_blend_map;
		clip_map		=my_clip_map;
		polygon_offset	=my_polygon_offset;
		line_stipple	=my_line_stipple;
	}
};

static Component_display_state display_state[]={
										//shader_type	depth		blend		clip		plygonoffset	line stipple
				Component_display_state(	2,			true,		false,		false,			false,			true),				//0:stipple edge
				Component_display_state(	4,			true,		false,		false,			false,			true),				//1:stipple frame
				Component_display_state(	0,			true,		false,		true,			true,			false),				//2:normal 
				Component_display_state(	1,			true,		false,		true,			true,			false),				//3:box
				Component_display_state(	0,			false,		true,		true,			true,			false),				//4:transpance
				Component_display_state(	0,			true,		true,		true,			true,			false),				//5:depth
				Component_display_state(	2,			true,		false,		false,			false,			false),				//6:selected edge
				Component_display_state(	2,			true,		false,		false,			false,			false),				//7:edge
				Component_display_state(	3,			true,		false,		false,			false,			false),				//8:point
				Component_display_state(	4,			true,		false,		false,			false,			false),				//9:frame
};

void Component_collector_for_display::display_components(double offset_factor,double offset_units,int line_stipple_factor,int line_stipple_pattern,bool create_mirror_picture_flag)
{
	int box_object_type_id=Shader_part_information().object_type_id[display_state[3].shader_type_map];
	double current_time=(double)(::GetTickCount());
	
	Graphics_system::set_display_parameter(par);
			
	for(int matrix_id=0;matrix_id<(sizeof(display_state)/sizeof(display_state[0]));matrix_id++){
		int shader_type_id=display_state[matrix_id].shader_type_map;
		Shader_base::set_shader_type(shader_type_id);

		Graphics_system::turn_onoff_line_stipple_and_polygon_offset(display_state[matrix_id].line_stipple,		line_stipple_factor,	line_stipple_pattern,
																	display_state[matrix_id].polygon_offset,	offset_factor,			offset_units);
		for(int i=0,part_number=par->part_array->part_number;i<part_number;i++){
			for(Part_with_shader *ps=par->part_array->part_array[i];ps!=NULL;ps=ps->next_part){
				int program_id;
				Shader_program_variable_index *variable_index;

				if(ps->paint_component_number[matrix_id]>0)
				if(ps->has_load_flag)
				if(ps->paint_graphics_objects[shader_type_id]!=NULL)
				if((program_id=ps->part_shader->get_program_id(&variable_index))>0){
					ps->access_time=current_time;
					Shader_part_information *info=ps->part_shader->get_part_information();
					bool display_box_flag=false;	
					if(ps->cross_clip_plane_number>0)
						if(info->shader_has_box_flag)
							if(!create_mirror_picture_flag)
								if(display_state[matrix_id].clip_map)
									display_box_flag=true;
					display_box_flag=Graphics_system::mount_program(program_id,display_box_flag,display_state[matrix_id].depth_map,display_state[matrix_id].blend_map,
											(flag->paint_method_id<=1)?1:(flag->selected_line_width),flag->selected_point_size,variable_index,par);						
					
					ps->part_shader->set_material(par,flag);
					
					for(Part_graphics_object *pg=ps->paint_graphics_objects[shader_type_id];pg!=NULL;pg=pg->next_graphics_object_pointer)
						if(pg->triangle_number>0){
							if(pg->create_buffer_object()>0)
								Graphics_system::display_part_graphics_object(matrix_id,info->object_type_id[shader_type_id],pg,
																ps->paint_component_number[matrix_id],ps->paint_component_pointer[matrix_id],variable_index);
							else{
								return_value++;
								Part_with_shader *release_ps=NULL;
								for(int part_id=0,part_number=par->part_array->part_number;part_id<part_number;part_id++)
									for(Part_with_shader *ps=par->part_array->part_array[part_id];ps!=NULL;ps=ps->next_part)
										if(release_ps==NULL)
												release_ps=ps;
										else if((release_ps->access_time)>(ps->access_time))
												release_ps=ps;
								if(release_ps!=NULL)
									ps->part_with_shader_release();
							}
						}
					if(display_box_flag){
						Graphics_system::set_display_part_box_on_clip_plane(par->clipper,par,variable_index);

						for(int j=0;j<par->clipper->clipper_number;j++){
							Clip_component *cp;
							if((cp=par->clipper->clip[j])->display_part_on_plane_flag){
								double x0=clip_box.p[0].x,y0=clip_box.p[0].y,z0=clip_box.p[0].z,x1=clip_box.p[1].x,y1=clip_box.p[1].y,z1=clip_box.p[1].z;

								Part_graphics_object pg(	cp->project_at_plane_location*Point(x0,y0,z0),cp->project_at_plane_location*Point(x0,y0,z1),
															cp->project_at_plane_location*Point(x0,y1,z0),cp->project_at_plane_location*Point(x0,y1,z1),		
															cp->project_at_plane_location*Point(x1,y0,z0),cp->project_at_plane_location*Point(x1,y0,z1),
															cp->project_at_plane_location*Point(x1,y1,z0),cp->project_at_plane_location*Point(x1,y1,z1),NULL);
								pg.set_normal(cp->A,cp->B,cp->C);
								Graphics_system::display_part_graphics_object(matrix_id,box_object_type_id,&pg,1,ps->paint_component_pointer[matrix_id],variable_index,&(Location()));
							}
						}
					}
				}
			}
		}
	}
}

Component_collector_for_display::Component_collector_for_display(double my_precision_scale,bool my_not_do_lod_flag,double my_minnimal_fovy_tanl,
				Component_display_parameter &my_par,Component_display_flag &my_flag,Select_component_ids *my_select_ids,Select_component_ids *my_match_ids)
{
	precision_scale2=my_precision_scale*my_precision_scale;
	not_do_lod_flag=my_not_do_lod_flag;
	select_ids=my_select_ids;
	
	minnimal_fovy_tanl2=my_minnimal_fovy_tanl*my_minnimal_fovy_tanl;
	
	total_part_number=0;
	total_display_triangle_number=0;
	
	par=&my_par;
	flag=&my_flag;

	return_value=0;
	if(my_match_ids!=NULL)
		if(my_match_ids->comp!=NULL)
			return_value++;

	for(int i=0,part_number=par->part_array->part_number;i<part_number;i++)
		for(Part_with_shader *ps=par->part_array->part_array[i];ps!=NULL;ps=ps->next_part)
			for(int j=0;j<(sizeof(ps->paint_component_number)/sizeof(ps->paint_component_number[0]));j++){
				ps->paint_component_number[j]=0;
				ps->cross_clip_plane_number=0;
			}
	
	Component *p=par->camera->eye_component;
	while(p->parent!=NULL)
		p=p->parent;
	clip_box=p->component_box;

	Coordinate_management::reset();
}

void Component_collector_for_display::register_display_component(Part_with_shader *ps,Component *comp,bool my_selected_flag,int matrix_id)
{
	if(comp->need_do_clip_test_flag)
		ps->cross_clip_plane_number++;
	ps->paint_component_pointer[matrix_id][ps->paint_component_number[matrix_id]++]=comp;
	total_part_number++;
	for(Part_graphics_object *pg=ps->paint_graphics_objects[display_state[matrix_id].shader_type_map];pg!=NULL;pg=pg->next_graphics_object_pointer)
		total_display_triangle_number+=pg->triangle_number;
}
bool Component_collector_for_display::operate_component(Component *comp,bool my_selected_flag)
{
	int ret_val;
	double precision2=0;
	Part_with_shader *ps,*last_ps;

	if(((comp->display_flag)&(flag->display_map))==0)
		return false;

	if(par->clipper->clipper_test(*comp,true))
		return false;
	
	if(!not_do_lod_flag){
		if((precision2=par->camera->caculate_precision2(comp))<minnimal_fovy_tanl2)
			return false;
		precision2*=precision_scale2;
	}
	if((comp->selected_flag)&&(flag->paint_method_id==0))
		Coordinate_management::register_coordinate(comp->absulate_location,comp->component_box.distance());

	if(comp->child!=NULL)
		return true;
	if((comp->part_id<0)||(comp->part_id>=par->part_array->part_number))
		return false;
	if((ps=par->part_array->part_array[comp->part_id])==NULL)
		return false;

	Component_display_list list(flag->paint_method_id,comp,my_selected_flag,select_ids);
		
	if((ret_val=ps->part_shader->test_display(comp,par,flag,&list))<0)
		return false;

	if(comp->private_data_storage_pointer!=NULL)
		if((comp->private_data_storage_pointer->paint_method_id=flag->paint_method_id)==0)
			if(my_selected_flag)
				comp->private_data_storage_pointer->paint_method_id=1;

	for(return_value+=ret_val,last_ps=ps;ps!=NULL;ps=ps->next_part){
		last_ps=ps;
		if(not_do_lod_flag||(precision2>=((ps->precision)*(ps->precision)))){
			if(ps->has_load_flag){
				if(list.hide_edge_stipple_display)
					register_display_component(ps,comp,my_selected_flag,0);
				if(list.hide_frame_stipple_display)
					register_display_component(ps,comp,my_selected_flag,1);
				if(list.color_and_depth_display)
					register_display_component(ps,comp,my_selected_flag,2);
				if(list.transpancy_display)
					register_display_component(ps,comp,my_selected_flag,4);
				if(list.depth_only_display)
					register_display_component(ps,comp,my_selected_flag,5);	

				if(list.selected_edge_display)
					register_display_component(ps,comp,my_selected_flag,6);
				if(list.edge_display)
					register_display_component(ps,comp,my_selected_flag,7);
				if(list.point_display)
					register_display_component(ps,comp,my_selected_flag,8);
				if(list.frame_display)
					register_display_component(ps,comp,my_selected_flag,9);
			}else{
				ps->need_load_flag=true;
				return_value++;
			}
			return false;
		}
	}

	if((comp->transparence>MIN_VALUE)&&(comp->transparence<(1.0-MIN_VALUE)))
		return false;

	if(!(last_ps->has_load_flag)){
		last_ps->need_load_flag=true;
		return_value++;
		return false;	
	}
	if(!(last_ps->part_shader->get_part_information()->shader_has_box_flag))
		return false;

	if((!(comp->frame_display_flag))||(comp->transparence>=(1.0-MIN_VALUE)))
		register_display_component(last_ps,comp,my_selected_flag,3);		//register display box
	else
		register_display_component(last_ps,comp,my_selected_flag,7);		//register edge
	return false;
}

CString Part_collector::compare_part_consistency(Part_collector *s)
{
	CString str,str1,str2;

	if(s==NULL)
		return _T("没有选中杆件");

	if(s->component_number<=0)
		return _T("没有选中杆件");

	sort();
	s->sort();

	for(int i=0,j=0;i<(s->component_number);i=j){
		int part_component_number=0,component_match_number=0,part_id=s->component_pointer[i]->part_id;
		for(j=0;j<component_number;j++)
			if(component_pointer[j]->part_id==part_id)
				component_match_number++;
		for(j=i+1;j<(s->component_number);j++)
			if(s->component_pointer[j]->part_id!=part_id)
				break;
		part_component_number=j-i;

		if(component_match_number<=0)
					str1+=_T("\n选中的杆件  ")+(ps->part_array[part_id]->user_name)+_T("  不是拼装杆件");
		else if(component_match_number!=part_component_number){
					str.Format(_T("选中%d件，拼装%d件,杆件件数错误"),part_component_number,component_match_number);
					str2+=_T("\n")+(ps->part_array[part_id]->user_name)+str;
		}
	}

	for(int i=0,j=0;i<(component_number);i=j){
		int part_component_number=0,component_match_number=0,part_id=component_pointer[i]->part_id;
		for(j=0;j<(s->component_number);j++)
			if(s->component_pointer[j]->part_id==part_id)
				component_match_number++;
		for(j=i+1;j<(component_number);j++)
			if(component_pointer[j]->part_id!=part_id)
				break;
		part_component_number=j-i;

		if(component_match_number<=0)
					str1+=_T("\n拼装杆件  ")+(ps->part_array[part_id]->user_name)+_T("  没有选中");
		else if(component_match_number!=part_component_number){
					str.Format(_T("拼装%d件,选中%d件，杆件件数错误"),part_component_number,component_match_number);
					str2+=_T("\n")+(ps->part_array[part_id]->user_name)+str;
		}
	}
	return str1;			
}
