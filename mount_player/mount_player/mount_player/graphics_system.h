#pragma once

class mount_material_property;

struct graphics_system_light{
		BOOL effective_flag,absolute_or_relative_flag;
		double x,y,z;
		double R, G, B;
		double Ambient, Diffuse, Specular, Shininess, Transparency, Emission;
};

class graphics_system  
{
public:

//method for clear background 
	virtual void clear_background(double red_color,double green_color,double blue_color)=NULL;
	virtual void draw_background (double x,double y,double z,int width,int height,void *background)=NULL;

//method for set up light
	virtual void graphics_setup_light(int max_light_number,struct graphics_system_light light_data[],double environment_red_color,double environment_green_color,double environment_blue_color,BOOL separate_specular_color_flag)=NULL;

//method for draw line
	virtual void display_line(double red_color,double green_color,double blue_color,
		double x1,double y1,double z1,double x0=0,double y0=0,double z0=0)=NULL;

//method for display
	virtual void display_color(double red_color,double green_color,double blue_color)=NULL;
	virtual void display_material(mount_material_property *material_property,double transparence_ratio=1)=NULL;
	virtual void specify_face_point(BOOL display_texture_flag,double *tesstriangles,double *tessnorms,double *tesstexture)=NULL;
	virtual void display_face(int tesstrianglecount)=NULL;

	virtual void display_box(double box_data[])=NULL;
	virtual void display_quadrangle(
				double dx,double dy,double dz,
				double x0,double y0,double z0,
				double x1,double y1,double z1,
				double x2,double y2,double z2,
				double x3,double y3,double z3)=NULL;


// method for manipulate matrix
	virtual void push_matrix()=NULL;
	virtual void pop_matrix()=NULL;
	virtual void load_identity_matrix()=NULL;
	virtual void mult_matrix(double matrix_data[])=NULL;
	virtual void get_current_matrix(double matrix_data[])=NULL;

//method for view-to-world
	virtual BOOL get_xy(double sx, double sy, double sz,double *x, double *y, double *z)=NULL;
	virtual BOOL get_screen_xy(double x, double y, double z,double *sx, double *sy)=NULL;
	virtual void get_view_size(double *sx, double *sy)=NULL;
	virtual void setup_view(double view_depth,double eye_x,double eye_y,double eye_z,double center_x,double center_y,double center_z,double up_x,double up_y,double up_z)=NULL;

// method for get screen color(RBG)
	virtual BOOL get_screen_color(double sx,double sy,double *color_buffer)=NULL;

//  method for manipulate call list
	virtual void begin_create_call_list(int call_list_id)=NULL;
	virtual void end_create_call_list()=NULL;
	virtual void execute_call_list(int call_list_id)=NULL;
	virtual int get_max_call_depth()=NULL;

//method for transparence display
	virtual void begin_transparence_display()=NULL;
	virtual void end_transparence_display()=NULL;

//method for control clip plane
	virtual void turn_on_clip_plane(double *clip_plane_data)=NULL;
	virtual void begin_display_box_on_clip_plane()=NULL;
	virtual void end_display_box_on_clip_plane()=NULL;

//method for begin and end display
	virtual void begin_display(CDC *pdc)=NULL;
	virtual void end_display(CDC *pdc,BOOL swap_flag)=NULL;

//method for begin and end texture display
	virtual int register_texure_object(int width,int height,float *texture_data,BOOL mipmap_flag)=NULL;
	virtual void unregister_texure_object(int object_id)=NULL;
	virtual void begin_texure_display(int object_id,mount_material_property *texture_material,mount_material_property *color_material,double transparence_ratio=1)=NULL;
	virtual void end_texure_display()=NULL;
	virtual void replace_texure_object(int object_id,int width,int height,float *texture_data)=NULL;


	graphics_system();
	virtual ~graphics_system();
};

