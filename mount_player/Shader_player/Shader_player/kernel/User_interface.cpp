#include "StdAfx.h"
#include "User_interface.h"

User_interface::User_interface(Whole_system &my_ws,Graphics_system &my_gs,CDC &my_dc,int my_camera_id,Box *my_selection_box,double *mirror_plane)
{
	dc=&my_dc;
	ws=&my_ws;
	swap_flag=false;
	
	CRect rect;
	dc->GetWindow()->GetClientRect(&rect);
	view_width =rect.Width();
	view_height=rect.Height();

	selection_box=my_selection_box;

	my_gs.begin_display(*dc);
	change_camera(my_camera_id,view_width,view_height,mirror_plane);
}

void User_interface::change_camera(int new_camera_id,double new_view_width,double new_view_height,double *mirror_plane)
{
	if((new_camera_id<0)||(new_camera_id>=MAX_CAMERA_NUMBER))
		new_camera_id=camera_id;
	else
		camera_id=new_camera_id;

	if(camera_pointer()==NULL)
		camera_id=0;

	camera_pointer()->caculate_camera(new_view_width/new_view_height,*(ws->parts),mirror_plane);
	
	Graphics_system::set_camera(new_view_width,new_view_height);

	clipper.clear_clip_component();

	if(mirror_plane!=NULL)
		clipper.add_clip_component(mirror_plane[0],mirror_plane[1],mirror_plane[2],mirror_plane[3],false,true);

	for(int i=0;i<(camera_pointer()->clip_plane_number);i++)
		clipper.add_clip_component(camera_pointer()->clip_plane[i].A,camera_pointer()->clip_plane[i].B,camera_pointer()->clip_plane[i].C,camera_pointer()->clip_plane[i].D,true,true);

	if(selection_box==NULL)
		clipper.add_selection_clipper(0,0,new_view_width,new_view_height,camera_pointer()->project_matrix,false);
	else
		clipper.add_selection_clipper(selection_box->p[0].x,selection_box->p[0].y,selection_box->p[1].x,selection_box->p[1].y,camera_pointer()->project_matrix,true);

	clipper.add_near_far_clipper((camera_pointer()->center_point)-(camera_pointer()->eye_point),camera_pointer()->near_point,camera_pointer()->far_point);

	return;
}

void User_interface::create_graphics_system_object(int display_map,Component *comp,double precision_scale)
{
	if(comp==NULL)
		return;
	if(comp->child!=NULL){
		Component *p=comp->child;
		do{
			create_graphics_system_object(display_map,p,precision_scale);
		}while((comp->child)!=(p=p->left_brother));
	}else if(((comp->part_id)>=0)&&((comp->part_id)<(ws->parts->part_number))){
			Part_with_shader *pws=ws->parts->part_array[comp->part_id];
			if(pws!=NULL)
				if(pws->has_load_flag)
					if(pws->part_shader!=NULL){
						double my_view_width=view_width,my_view_height=view_height;
						int my_camera_id=camera_id;

						pws->part_shader->create_graphics_system_object(display_map,comp,this,precision_scale);

						view_width=my_view_width;
						view_height=my_view_height;
						camera_id=my_camera_id;
					}
	}
	return;
}
void User_interface::delete_graphics_system_object()
{
	for(int i=0,part_number=ws->parts->part_number;i<part_number;i++)
		for(Part_with_shader *ps=ws->parts->part_array[i];ps!=NULL;ps=ps->next_part)
			ps->part_shader->delete_graphics_system_object();

	for(int i=0;i<MAX_LIGHT_NUMBER;i++)
		ws->lights->light_array[i].delete_depth_texture_and_frame_buffer();
}

void User_interface::create_depth_texture(int display_map,double precision_scale)
{
	double scale;
	double my_view_width=view_width,my_view_height=view_height;
	int my_camera_id=camera_id;

	for(int i=0;i<MAX_LIGHT_NUMBER;i++){
		Light *lt=&(ws->lights->light_array[i]);
		lt->light_matrix=Location();
		lt->projection_matrix=Location();

		if(i<(ws->lights->light_number))
			if((lt->mount_camera_id>=0)&&(lt->mount_camera_id<MAX_CAMERA_NUMBER)){
				Camera *cam=ws->camera[lt->mount_camera_id];
				if(cam!=NULL){
					lt->light_matrix=cam->eye_component->absulate_location*Location::move(0,0,cam->distance);
					if(lt->shadow_texture_unit_id>=0)
						if((scale=lt->shadow_texture_precision)>0){
							change_camera(lt->mount_camera_id,scale*my_view_width,scale*my_view_height,NULL);
							lt->projection_matrix=camera_pointer()->project_matrix;

							lt->create_depth_texture_and_frame_buffer((int)(scale*my_view_width),(int)(scale*my_view_height));
							Component_collector_for_display a(	precision_scale,false,ws->minnimal_fovy_tanl,
									Component_display_parameter(ws->parts,camera_pointer(),ws->lights,&clipper,NULL,NULL), 
									Component_display_flag(1,display_map,ws->selected_line_width,ws->selected_point_size),
									NULL,NULL);
							Graphics_system::clear_screen();
							a.collect(ws->root_component);
							a.display_components(-1.0,-1.0,ws->line_stipple_factor,ws->line_stipple_pattern);
							lt->get_depth_texture_image((int)(scale*my_view_width),(int)(scale*my_view_height));
						}
				}
			}
	}
	
	view_width=my_view_width;
	view_height=my_view_height;
	camera_id=my_camera_id;
	return;
}

int User_interface::paint(int display_map,double precision_scale)
{
	Light *lt;
	
	Graphics_system::restore_frame_buffer();
	change_camera(camera_id,view_width,view_height,NULL);

	for(int i=0;i<(ws->lights->light_number);i++)
		if((lt=&(ws->lights->light_array[i]))->shadow_texture_unit_id>=0)
			Graphics_system::mount_depth_texture(lt->shadow_texture_unit_id,(lt->shadow_texture_precision<=0.0)?0:(lt->shadow_texture_object_id));

	Component_display_parameter display_parameter(ws->parts,camera_pointer(),ws->lights,&clipper,&(ws->select_component_ids),&(ws->match_component_ids));
	Component_collector_for_display a(	 precision_scale,false,ws->minnimal_fovy_tanl,display_parameter,
										Component_display_flag(0,display_map,ws->selected_line_width,ws->selected_point_size),
										&(ws->select_component_ids),&(ws->match_component_ids));	
	Graphics_system::clear_screen(ws->background_color);
	a.collect(ws->root_component);
	a.display_components(ws->offset_factor,ws->offset_units,ws->line_stipple_factor,ws->line_stipple_pattern);

	camera_pointer()->paint_part_number=a.total_part_number;
	camera_pointer()->paint_triangle_number=a.total_display_triangle_number;
		
	swap_flag=true;

	return a.return_value;
}
int User_interface::do_paint(double precision_scale,CDC &dc,Whole_system &my_ws,Graphics_system &my_gs,int my_camera_id)
{
	double start_time=Platform_information::get_current_time();
	int return_value;
	{
		User_interface a(my_ws,my_gs,dc,my_camera_id);
		a.create_depth_texture(1<<my_camera_id,precision_scale);
		a.create_graphics_system_object(1<<my_camera_id,my_ws.root_component,precision_scale);
		return_value=a.paint(1<<my_camera_id,precision_scale);
	}
	if((my_camera_id>=0)&&(my_camera_id<MAX_CAMERA_NUMBER))
		if(my_ws.camera[my_camera_id]!=NULL)
			my_ws.camera[my_camera_id]->paint_time_length=Platform_information::get_current_time()-start_time;
	return return_value;
}

void User_interface::set_view(int view_id,double time_length)
{
	{
		Component_collector_for_selected_component a(ws->root_component);	
		if(a.box_component_number>0){
			if(view_id<0)
				camera_pointer()->locate_view(true,a.component_box,1.0,time_length,*(ws->parts));
			else
				camera_pointer()->set_view(true,a.component_box,1.0,time_length,*(ws->parts),view_id);
			return;
		}
	}
	{
		class temp:public Component_collector_base
		{
			int display_map;
		public:
			temp(Component *comp,int my_display_map)
			{
				display_map=my_display_map;
				collect(comp);
			}
			virtual bool operate_component(Component *comp,bool my_selected_flag)
			{
				if((comp->display_flag&display_map)==0)
					return false;
				if(comp->child==NULL)
					register_component_box(comp);
				return true;
			}
		};
		temp a(ws->root_component,1<<camera_id);
		if(a.box_component_number>0){
			if(view_id<0)
				camera_pointer()->locate_view(true,a.component_box,1.0,time_length,*(ws->parts));
			else
				camera_pointer()->set_view   (true,a.component_box,1.0,time_length,*(ws->parts),view_id);
			return;
		}
	}
	if(view_id<0)
		camera_pointer()->locate_view(true,ws->root_component->component_box,1.0,time_length,*(ws->parts));
	else
		camera_pointer()->set_view   (true,ws->root_component->component_box,1.0,time_length,*(ws->parts),view_id);

	return;
}
void User_interface::enlarge_selected_area(int mouse_down_x,int mouse_down_y,int mouse_up_x,int mouse_up_y,double time_length)
{
	if((abs(mouse_down_x-mouse_up_x)>=10)&&(abs(mouse_down_y-mouse_up_y)>=10)){
		double px=::abs((double)(mouse_down_x-mouse_up_x)/view_width),py=::abs((double)(mouse_down_y-mouse_up_y)/view_height),depth_value=-1.0;

		if(ws->select_component_ids.comp!=NULL)
			depth_value=Graphics_system::object_to_screen(ws->select_component_ids.selection_point,camera_pointer()->project_matrix).z;
		camera_pointer()->enlarge_selected_area(mouse_down_x, mouse_down_y, mouse_up_x, mouse_up_y,px>py?px:py,depth_value,*(ws->parts));
		camera_pointer()->push_restore_record(*(ws->parts),time_length,true);
	}
}
void User_interface::translate_component(int last_x, int last_y, int this_x, int this_y,Point &world_select_flag,Point &local_select_flag)
{
	Component *p;
	
	if((p=get_most_recent_selected_component(ws->root_component))==NULL)
		return;
	if((last_x==this_x)&&(last_y==this_y))
		return;

	double screen_z=Graphics_system::object_to_screen(p->absulate_location*Point(0,0,0),camera_pointer()->project_matrix).z;

	p->translate_component(last_x,last_y,this_x,this_y,screen_z,world_select_flag,local_select_flag,*(ws->parts),camera_pointer()->project_matrix);

}
void User_interface::rotate_component(int rotate_type,int last_x, int last_y, int this_x, int this_y)
{
	Component *p;
	
	if((p=get_most_recent_selected_component(ws->root_component))==NULL)
		return ;
	if((last_x==this_x)&&(last_y==this_y))
		return ;

	Point p0=camera_pointer()->center_point,p1=camera_pointer()->eye_point;
	double depth=Graphics_system::object_to_screen(p0+(p1-p0).expand(camera_pointer()->distance*(ws->rotate_scale)),camera_pointer()->project_matrix).z;
	p->rotate_component(rotate_type,last_x,last_y,this_x,this_y,*(ws->parts),camera_pointer()->project_matrix,depth);
}
double User_interface::adjust_mouse_distance(double mouse_x,double mouse_y,double scale_value)
{
	camera_pointer()->enlarge_mouse_point(
		mouse_x,mouse_y,scale_value,
		ws->root_component->component_box,
		(ws->select_component_ids.comp==NULL)
			?NULL
			:(&(ws->select_component_ids.selection_point)));
	return camera_pointer()->distance;
}
double User_interface::adjust_fovy(double delta)
{
	camera_pointer()->set_fovy(camera_pointer()->get_fovy()*delta);

	if((camera_pointer()->get_fovy())>150)
		camera_pointer()->set_fovy(150);
	else 
		if((camera_pointer()->get_fovy())<1)
			camera_pointer()->set_fovy(1);
	return camera_pointer()->get_fovy();
}

Component *User_interface::do_screen_selection(int display_map,double sx, double sy)
{
	Part *my_part;
	Body *my_body;
	Face *my_face;
	Face_loop *my_face_loop;
	Face_edge *my_face_edge;

	ws->select_component_ids.reset();

	Select_component_ids *ids=&(ws->select_component_ids);

	for(int method_id=2;method_id<=8;method_id++){
		Component_collector_for_display a(1.0,(ids->comp==NULL)?false:true,ws->minnimal_fovy_tanl,
				Component_display_parameter(ws->parts,camera_pointer(),ws->lights,&clipper,ids,NULL), 
				Component_display_flag(method_id,display_map,ws->selected_line_width,ws->selected_point_size),
				ids,NULL);

		Graphics_system::clear_screen();
		a.collect((ids->comp==NULL)?(ws->root_component):(ids->comp));
		
		a.display_components(ws->offset_factor,ws->offset_units,ws->line_stipple_factor,ws->line_stipple_pattern);

		double depth_value=1.0;
		Point color=Graphics_system::get_screen_color(sx,sy,depth_value);
		int select_id =(int)(64*color.x)+64*(int)(64*color.y)+64*64*(int)(64*color.z)-1;

		switch(method_id){
		case 2:
			ids->selection_point=Graphics_system::screen_to_object(Point(sx,sy,depth_value),camera_pointer()->project_matrix);

			if((ids->comp=ws->root_component->get_node_by_id(select_id))==NULL)
				return NULL;
			if(ids->comp->child!=NULL)
				return NULL;
			if((ids->comp->part_id<0)||((ids->comp->part_id)>=(ws->parts->part_number)))
				return NULL;
			my_part=ws->parts->part_array[ids->comp->part_id]->rude_part;
			break;
		case 3:			
			if((select_id<0)||(select_id>=(my_part->body_number)))
				return ids->comp;
			ids->body_id=select_id;
			my_body=&(my_part->body_array[select_id]);
			break;
		case 4:			
			if((select_id<0)||(select_id>=(my_body->face_number)))
				return ids->comp;
			ids->face_id=select_id;
			my_face=&(my_body->face_array[select_id]);
			break;
		case 5:			
			if((select_id<0)||(select_id>=(my_face->triangles.triangle_number*3)))
				return ids->comp;
			ids->vertex_id=select_id;
			break;
		case 6:
			if((select_id<0)||(select_id>=(my_face->face_curve.face_loop_number)))
				return ids->comp;
			ids->loop_id=select_id;
			my_face_loop=&(my_face->face_curve.face_loop[select_id]);
			break;
		case 7:			
			if((select_id<0)||(select_id>=(my_face_loop->edge_number)))
				return ids->comp;
			ids->edge_id=select_id;
			my_face_edge=&(my_face_loop->edge[select_id]);
			break;
		case 8:			
			ids->edge_point_id=select_id;
			break;
		default:
			break;
		}
	}
	return ids->comp;
}

Component *User_interface::select_component_from_screen(Whole_system &my_ws,Graphics_system &my_gs,CDC & my_dc,int my_camera_id,double sx, double sy)
{
	double selected_line_width=my_ws.selected_line_width;	
	if(sx<selected_line_width)
			sx=selected_line_width;
	if(sy<selected_line_width)
			sy=selected_line_width;
	
	Box select_box(sx-selected_line_width,sy-selected_line_width,0,sx+selected_line_width,sy+selected_line_width,0);
	Box *select_box_pointer=&select_box;
	
	Component *comp=User_interface(my_ws,my_gs,my_dc,my_camera_id,select_box_pointer).do_screen_selection(1<<my_camera_id,sx,sy);
	
	my_ws.select_component_ids.compute_pointer(my_ws.parts);
	
	return comp;
}

void User_interface::select_parent_component()
{
	int n=0;
	Component_collector_for_selected_component a(ws->root_component);

	for(int i=0;i<a.component_number;i++)
		if(a.component_pointer[i]->selected_flag){
			n++;
			if(a.component_pointer[i]->parent!=NULL)
				a.component_pointer[i]->parent->set_component_selected();
		}
	if(n<=0)
		get_most_recent_selected_component(ws->root_component)->set_component_selected();
}
void User_interface::select_child_component(Component *component_pointer)
{
	Component *p,*select_child;

	if((select_child=component_pointer->child)==NULL)
		select_child=component_pointer;
	else{
		component_pointer->clear_select_flag();
		for(p=select_child->left_brother;p!=(component_pointer->child);p=p->left_brother)
			if(p->selected_time>select_child->selected_time)
				select_child=p;
	}
	select_child->set_component_selected();
}
void User_interface::select_child_component()
{
	int i,n=0;

	Component_collector_for_selected_component a(ws->root_component);
	for(i=0;i<a.component_number;i++)
		if(a.component_pointer[a.component_number-i-1]->selected_flag){
			select_child_component(a.component_pointer[a.component_number-i-1]);
			n++;
		}
	if(n<=0)
		get_most_recent_selected_component(ws->root_component)->set_component_selected();
	return;
}
Component *User_interface::get_most_recent_selected_component(Component *p)
{
	class temp:public Component_collector_base
	{
	public:
		Component *pp;
		virtual bool operate_component(Component *comp,bool my_selected_flag)
		{
			if((comp->selected_time)>(pp->selected_time))
				pp=comp;
			return true;
		}	
		temp(Component *my_p)
		{
			pp=my_p;
			collect(pp);
		}
	};
	return temp(p).pp;
}

double User_interface::set_component_transparence(double scale,bool transparence_flag)
{
	class temp:public Component_collector_base
	{
	public:
		double scale_value,new_scale_value;
		bool transparence_flag;
		virtual bool operate_component(Component *comp,bool my_selected_flag)
		{
			if(comp->child==NULL){				
				if(transparence_flag)
					new_scale_value=scale_value;
				else
					new_scale_value=comp->transparence+scale_value;
				new_scale_value=(new_scale_value<0.01)?0.01:(new_scale_value>0.99)?0.99:new_scale_value;
				comp->transparence=new_scale_value;
			}
			return true;
		}
		temp(double my_scale,bool my_transparence_flag)
		{
			scale_value=my_scale;
			transparence_flag=my_transparence_flag;
			new_scale_value=0.0;
		}
	};
	temp a(scale,transparence_flag);	

	Component_collector_for_selected_component selected_components(ws->root_component);
	if(selected_components.component_number>0){
		for(int i=0;i<selected_components.component_number;i++){
			selected_components.component_pointer[i]->clear_select_flag();
			a.collect(selected_components.component_pointer[i]);
		}
	}else{
		Component *comp=get_most_recent_selected_component(ws->root_component);
		if(comp!=NULL)
			a.collect(comp);
	}
	return a.new_scale_value;
}
void User_interface::clear_component_transparence()
{
	class temp:public Component_collector_base
	{
	public:
		virtual bool operate_component(Component *comp,bool my_selected_flag)
		{
			if(comp->child==NULL)
				comp->transparence=1.0;
			return true;
		}
	};
	temp a;
	Component_collector_for_selected_component selected_components(ws->root_component);
	if(selected_components.component_number<=0)
		a.collect(ws->root_component);
	else
		for(int i=0;i<selected_components.component_number;i++){
			selected_components.component_pointer[i]->clear_select_flag();
			a.collect(selected_components.component_pointer[i]);
		}
	return;
}
void User_interface::reset_component()
{
	Component_collector_for_selected_component a(ws->root_component);
	if(a.component_number>0){
		for(int i=0;i<a.component_number;i++)
			if(a.component_pointer[i]->selected_flag)
				a.component_pointer[i]->reset_component(*(ws->parts));
	}else{
		Location old_camera_location[MAX_CAMERA_NUMBER];
		for(int i=0;i<MAX_CAMERA_NUMBER;i++){
			old_camera_location[i]=Location();
			if(ws->camera[i]!=NULL)
				if(ws->camera[i]->eye_component!=NULL)
					old_camera_location[i]=ws->camera[i]->eye_component->absulate_location;
		}
		ws->root_component->reset_component(*(ws->parts));
		for(int i=0;i<MAX_CAMERA_NUMBER;i++){
			if(ws->camera[i]!=NULL)
				if(ws->camera[i]->eye_component!=NULL){
					Location new_camera_location=ws->camera[i]->eye_component->absulate_location;
					Location modify_location=(-new_camera_location)*(old_camera_location[i]);
					ws->camera[i]->eye_component->modify_move_relative_location(modify_location,*(ws->parts));
				}
		}
	}
	ws->root_component->get_component_box(*(ws->parts));
}
void User_interface::area_select_component(int my_display_map,double win_x0, double win_y0, double win_x1, double win_y1)
{
	if((abs(win_x0-win_x1)<10)||(abs(win_y0-win_y1)<10))
		return;
	if(win_x0>win_x1){
		double x=win_x0;
		win_x0=win_x1;
		win_x1=x;
	}
	if(win_y0>win_y1){
		double y=win_y0;
		win_y0=win_y1;
		win_y1=y;
	}
	class temp:public Component_collector_base
	{
		int clip_display_map;
	public:	
		Clipper clip;

		temp(int my_display_map,Point &eye_point,double win_x0, double win_y0, double win_x1, double win_y1,Location &project_matrix)
		{
			clip_display_map=my_display_map;
			clip.clear_clip_component();
			clip.add_selection_clipper(win_x0,win_y0,win_x1,win_y1,project_matrix,false);			
		}
		bool operate_component(Component *comp,bool my_selected_flag)
		{
			if(((comp->display_flag)&(clip_display_map))==0)
				return false;
			if(clip.clipper_test(*comp,false))
				return false;
			if(comp->child!=NULL)
				return true;
			if(comp->need_do_clip_test_flag)
				return false;
			register_component(comp);
			register_component_box(comp);
			return false;
		}
	};
	temp t(my_display_map,camera_pointer()->eye_point,win_x0,win_y0,win_x1,win_y1,camera_pointer()->project_matrix);
	t.collect(ws->root_component);

	for(int i=0;i<t.component_number;i++)
		t.component_pointer[i]->set_component_selected();
	ws->root_component->collect_selected_flag();
	return;
}

void User_interface::hide_component(int display_map)
{
	class temp:public Component_collector_base
	{
	public:
		int display_map;
		virtual bool operate_component(Component *comp,bool my_selected_flag)
		{
			if((comp->child==NULL)&&my_selected_flag)
				comp->display_flag&=display_map;
			return true;
		}
		temp(int my_display_map)
		{
			display_map=(~my_display_map);
		}
	};
	temp(display_map).collect(ws->root_component);
	return;
}

void User_interface::show_component(int display_map)
{
	class temp:public Component_collector_base
	{
	public:
		int display_map;
		virtual bool operate_component(Component *comp,bool my_selected_flag)
		{
			if(comp->child==NULL)
				comp->display_flag|=display_map;
			return true;
		}
		temp(int my_display_map)
		{
			display_map=my_display_map;
		}
	};	
	temp (display_map).collect(ws->root_component);
	
	return;
}

void User_interface::set_component_hide_line_type(int hide_line_type_id)
{
	class temp:public Component_collector_base
	{
	public:
		int hide_line_type_id;

		virtual bool operate_component(Component *comp,bool my_selected_flag)
		{
			if(comp->child==NULL)
				comp->hide_line_type_id=hide_line_type_id;
			return true;
		}
		temp(int my_hide_line_type_id)
		{
			hide_line_type_id=my_hide_line_type_id;
		}
	};
	Component_collector_for_selected_component a(ws->root_component);
	temp b(hide_line_type_id);
	
	if(a.component_number<=0)
		b.collect(ws->root_component);
	else
		for(int i=0;i<(a.component_number);i++)
			b.collect(a.component_pointer[i]);
	ws->root_component->clear_select_flag();
	return;
}
void User_interface::set_component_frame_show_detail(bool frame_display_detail_flag)
{
	class temp:public Component_collector_base
	{
	public:
		bool frame_display_detail_flag;

		virtual bool operate_component(Component *comp,bool my_selected_flag)
		{
			if(comp->child==NULL)
				comp->frame_display_detail_flag=frame_display_detail_flag;
			return true;
		}
		temp(bool my_frame_display_detail_flag)
		{
			frame_display_detail_flag=my_frame_display_detail_flag;
		}
	};
	Component_collector_for_selected_component a(ws->root_component);
	temp b(frame_display_detail_flag);
	
	if(a.component_number<=0)
		b.collect(ws->root_component);
	else
		for(int i=0;i<(a.component_number);i++)
			b.collect(a.component_pointer[i]);
	ws->root_component->clear_select_flag();
	return;
}
void User_interface::set_component_frame_show(bool only_display_frame_flag)
{
	class temp:public Component_collector_base
	{
	public:
		bool only_display_frame_flag;

		virtual bool operate_component(Component *comp,bool my_selected_flag)
		{
			if(comp->child==NULL){
				comp->frame_display_flag=true;
				if(only_display_frame_flag)
					comp->transparence=0.0;
			}
			return true;
		}
		temp(bool my_only_display_frame_flag)
		{
			only_display_frame_flag=my_only_display_frame_flag;
		}
	};
	Component_collector_for_selected_component a(ws->root_component);
	temp b(only_display_frame_flag);
	
	if(a.component_number<=0)
		b.collect(ws->root_component);
	else
		for(int i=0;i<(a.component_number);i++)
			b.collect(a.component_pointer[i]);
	ws->root_component->clear_select_flag();
	return;
}
void User_interface::clear_component_frame_show()
{
	class temp:public Component_collector_base
	{
	public:
		virtual bool operate_component(Component *comp,bool my_selected_flag)
		{
			if(comp->child==NULL){
				comp->frame_display_flag=false;
				if(comp->transparence<=0.0)
					comp->transparence=1.0;
			}
			return true;
		}
		temp()
		{
		}
	};
	Component_collector_for_selected_component a(ws->root_component);
	temp b;

	if(a.component_number<=0)
			b.collect(ws->root_component);
	else
		for(int i=0;i<(a.component_number);i++)
			b.collect(a.component_pointer[i]);
	ws->root_component->clear_select_flag();
	return;
}

void User_interface::release_component_mesh()
{
	class temp:public Component_collector_base
	{
	public:
		virtual bool operate_component(Component *comp,bool my_selected_flag)
		{
			if(comp->child==NULL)
				register_component(comp);
			return true;
		}
		temp(Whole_system *ws)
		{
			Component_collector_for_selected_component a(ws->root_component);

			if(a.component_number<=0)
				collect(ws->root_component);
			else
				for(int i=0;i<(a.component_number);i++)
					collect(a.component_pointer[i]);

			for(int i=0;i<component_number;i++){
				int part_id=component_pointer[i]->part_id;
				if((part_id>=0)&&(part_id<(ws->parts->part_number)))
					for(Part_with_shader *ps=ws->parts->part_array[part_id];ps!=NULL;ps=ps->next_part)
						ps->part_with_shader_release();
			}
			ws->root_component->clear_select_flag();
		}
	};
	temp a(ws);
}
void User_interface::edit_part_material(Whole_system *my_ws)
{
	class temp:public Component_collector_base
	{
	public:
		virtual bool operate_component(Component *comp,bool my_selected_flag)
		{
			if(comp->child==NULL)
				register_component(comp);
			return true;
		}
		temp(Whole_system *ws)
		{
			Component_collector_for_selected_component a(ws->root_component);
			for(int i=0;i<(a.component_number);i++)
				collect(a.component_pointer[i]);
			ws->root_component->clear_select_flag();

			if(component_number<=0)
				::AfxMessageBox(_T("您没有选中杆件，请选中杆件后再执行该功能"));
			else if(component_number>1)
				::AfxMessageBox(_T("您选中多个杆件，执行该功能请选中一个杆件"));
			else{
				int part_id=component_pointer[0]->part_id;
				if((part_id>=0)&&(part_id<(ws->parts->part_number)))
					for(Part_with_shader *ps=ws->parts->part_array[part_id];ps!=NULL;ps=ps->next_part){
						bool has_not_done_flag=true;
						for(Part_with_shader *has_done_ps=ws->parts->part_array[part_id];ps!=has_done_ps;has_done_ps=has_done_ps->next_part){
							CString str1((ps->part_shader->directory_name)+(ps->part_shader->file_name));
							CString str2((has_done_ps->part_shader->directory_name)+(has_done_ps->part_shader->file_name));
							if(str1==str2){
								has_not_done_flag=false;
								break;
							}
						}
						if(has_not_done_flag)
							ps->part_shader->modify_material();
						else
							ps->part_shader->set_reload_material();
					}
			}
		}
	};
	temp a(my_ws);
}
void User_interface::set_all_material_reload()
{
	for(int i=0;i<(ws->parts->part_number);i++)
		for(Part_with_shader *ps=ws->parts->part_array[i];ps!=NULL;ps=ps->next_part)
			ps->part_shader->set_reload_material();
}
void User_interface::create_assemble_part(CString assemble_part_file_name,bool material_id_increase_flag)
{
	int part_id,body_number;
	Part_with_shader *ps;
	Component *comp;
	
	if((comp=get_most_recent_selected_component(ws->root_component))==NULL)
		return;

	Part_collector collector(ws->parts);

	{
		Part_collector original_collector(comp,ws->parts);
		for(int i=0;i<(original_collector.component_number);i++)
			if((part_id=original_collector.component_pointer[i]->part_id)>=0)
				if(part_id<(ws->parts->part_number))
					if((ps=ws->parts->part_array[part_id])!=NULL){
						if(ps->rude_part==NULL){
							::AfxMessageBox(_T("发现没有形状数据的杆件，无法创建统一装配体"));
							return;
						}
						if(ps->information_has_been_released_flag){
							::AfxMessageBox(_T("发现已经释放了形状数据的杆件，无法创建统一装配体"));
							return;
						}
						if(ps->has_load_flag)
							collector.collect(original_collector.component_pointer[i]);
						else{
							::AfxMessageBox(_T("发现没有装入的杆件，无法创建统一装配体"));
							return;
						}
					}
	}
	if(collector.component_number<=0)
		return;
	collector.sort();

	part_id=collector.component_pointer[0]->part_id;
	ps=ws->parts->part_array[part_id];

	body_number=0;
	for(int i=0;i<(collector.component_number);i++){
		int current_part_id=collector.component_pointer[i]->part_id;
		Part_with_shader *current_ps=ws->parts->part_array[current_part_id];
		if(Shader_base::is_same_shader(ps->part_shader,current_ps->part_shader))
			body_number+=(current_ps->rude_part->body_number);
		else{
			::AfxMessageBox(_T("不是相同的着色器，无法创建统一装配体"));
			return;
		}
	}
	ofstream f(assemble_part_file_name);

	f<<"形体数          "<<body_number<<"\n";

	for(int i=0,j=0,body_id_base=0,material_id_base=0;i<(collector.component_number);i=j){
		part_id=collector.component_pointer[i]->part_id;
		Part_with_shader *ps=ws->parts->part_array[part_id];
		Part *rude_part=ps->rude_part;
		rude_part->part_get_material_id_range();

		for(j=i;j<(collector.component_number);j++){
			int current_part_id=collector.component_pointer[j]->part_id;
			if(part_id!=current_part_id)
				break;
			ps=ws->parts->part_array[current_part_id];
			rude_part=ps->rude_part;
			Location loca=(-(comp->absulate_location))*(collector.component_pointer[j]->absulate_location);
			if(rude_part->flush_for_create_assemble_part(f,body_id_base,loca,material_id_base-(rude_part->min_material_id))){
				f.close();
				return;
			}
		}
		if(material_id_increase_flag)
			material_id_base+=1+(rude_part->max_material_id)-(rude_part->min_material_id);
	}
	f.close();
}