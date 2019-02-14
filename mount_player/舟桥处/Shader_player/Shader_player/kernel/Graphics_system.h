#pragma once

#include "Edge.h"

class Shader_program_variable_index;

class Graphics_system
{
	HGLRC m_hRC;
	Graphics_system *front_gs,*back_gs;
public:
	bool graphics_system_flag;
	static void display_part_graphics_object(	int matrix_id,int graphics_object_type_id,Part_graphics_object *pg,int display_number,Component **component_pointer,
												Shader_program_variable_index *variable_index,Location *loca=NULL);
	
	Graphics_system(CDC &pdc,int &max_user_parameter_number,CString directory_name);
	virtual ~Graphics_system(void);

	void begin_display(CDC &dc);
	static void end_display(CDC &dc,bool swap_flag=false);
	
	static void clear_screen(Point clear_color=Point(0.0,0.0,0.0));

	static void set_camera(double view_port_width,double view_port_height);
	
	static void turn_onoff_line_stipple_and_polygon_offset(
					bool line_stipple_onoff_flag,		int line_stipple_factor,		int line_stipple_pattern,
					bool polygon_offset_onoff_flag,		double polygon_offset_factor,	double polygon_offset_units);

	static void set_user_parameter(int parameter_number,void *parameter_data_pointer,int base_id);
	static void set_display_parameter(Component_display_parameter *par);
	static void set_display_part_box_on_clip_plane(Clipper *clipper,Component_display_parameter *par,Shader_program_variable_index *variable_index);

	static bool mount_program(int program_id,bool stencil_on_off_flag,bool depth_state,bool blend_state,double line_width,double point_size,Shader_program_variable_index *variable_index,Component_display_parameter *par);
			
	static void get_default_object_type_id(int *shader_type_id);

	static Point width_and_height();
	static Point screen_to_object( Point pp,Location project_matrix);
	static Point object_to_screen( Point pp,Location project_matrix);
	static Point get_screen_color(double sx,double sy,double &depth_value);

	static int compile_shader(CString &display_error_message,char *shader_program,int type_id);
	static int link_program(CString &display_error_message,int shader_number,int shader_id[],CString shader_program_file_name[]);

	static void delete_program(int program_id);
	static void delete_shader(int shader_id);

	static int get_uniform_index(int program_id,const char *var_name);
			
	static int create_buffer_object(float *data_pointer,int size);
	static void delete_buffer_object(int buffer_object_id);

	static void create_depth_texture_and_frame_buffer(int *frame_buffer_id,int *render_buffer_id,int *texture_id,int texture_width,int texture_height);
	static void create_mirror_texture_and_frame_buffer(int *frame_buffer_id,int *render_buffer_id,int *texture_id,int texture_width,int texture_height);
	static void restore_frame_buffer();

	static void get_depth_texture_image(int texture_id,int texture_width,int texture_height);
	static void get_mirror_texture_image(int texture_id,int texture_width,int texture_height);

	
	static void delete_framebuffer_renderbuffer_and_texture(int *frame_buffer_id,int *render_buffer_id,int *texture_id);
	
	static void mount_depth_texture(int texture_unit_id,int texture_object_id);
};