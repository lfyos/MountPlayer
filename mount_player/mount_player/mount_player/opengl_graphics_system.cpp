#include "StdAfx.h"
#include "opengl_graphics_system.h"


#include "GL/glew.h"
#include "GL/wglew.h"

#include "GL/glext.h"
#include "GL/wglext.h"

#include "GL/glut.h"

#include "mount_material_property.h"

#include <math.h>

#pragma comment( lib,"glew32.lib")
#pragma comment( lib,"glut32.lib")


opengl_graphics_system::opengl_graphics_system(CDC *pdc)
{

	PIXELFORMATDESCRIPTOR pdf={
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW
			|PFD_SUPPORT_OPENGL
			|PFD_TYPE_RGBA
			|PFD_DOUBLEBUFFER
			|PFD_STEREO_DONTCARE,
		24,
		0,0,0,0,0,0,
		0,0,0,0,0,0,0,
		0,0,
		PFD_MAIN_PLANE,
		0,
		0,0,0,
	};
	if(pdf.cStencilBits<8)
		pdf.cStencilBits=8;
		
	int pixelFormat=ChoosePixelFormat(pdc->m_hDC, &pdf);
	BOOL success=SetPixelFormat(pdc->m_hDC,pixelFormat,&pdf);
	m_hRC=wglCreateContext(pdc->m_hDC);

	wglMakeCurrent(pdc->m_hDC,m_hRC);

	glewInit();
	
	glDeleteShader(glCreateShader(GL_VERTEX_SHADER));
	glDeleteShader(glCreateShader(GL_FRAGMENT_SHADER));

	CString version_string,extension_string,vendor_string;
	version_string=glGetString(GL_VERSION);
	extension_string=glGetString(GL_EXTENSIONS);
	vendor_string=glGetString(GL_VENDOR);

	int texture_unit_number=-1;
	
	glGetIntegerv(GL_MAX_TEXTURE_UNITS,&texture_unit_number); 
	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,&texture_unit_number); 
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,&texture_unit_number);
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,&texture_unit_number);


	return;
}
opengl_graphics_system::~opengl_graphics_system()
{
	wglDeleteContext(m_hRC);
}
void opengl_graphics_system::begin_display(CDC *pdc)
{
	CRect rect; 
	pdc->GetWindow()->GetClientRect(&rect);

	wglMakeCurrent(pdc->m_hDC,m_hRC);
	glViewport(0,0,rect.Width(),rect.Height());
	glDisable(GL_NORMALIZE);

	return;
}
void opengl_graphics_system::end_display(CDC *pdc,BOOL swap_flag)
{
	if(swap_flag)
		SwapBuffers(pdc->m_hDC);
	wglMakeCurrent(pdc->m_hDC,NULL);
}

void opengl_graphics_system::display_line(
		double red_color,double green_color,double blue_color,
		double x1,double y1,double z1,double x0,double y0,double z0)
{
	glDisable(GL_LIGHTING);

	glColor3f((GLfloat)red_color,(GLfloat)green_color,(GLfloat)blue_color);
	glBegin(GL_LINES);
	glVertex3d(x0,y0,z0);
	glVertex3d(x1,y1,z1);
	glEnd();

	glEnable(GL_LIGHTING);
}

void opengl_graphics_system::display_quadrangle(
				double dx,double dy,double dz,
				double x0,double y0,double z0,
				double x1,double y1,double z1,
				double x2,double y2,double z2,
				double x3,double y3,double z3)
{
	glBegin(GL_QUADS);
	glNormal3d(dx,dy,dz);
	glVertex3d(x0,y0,z0);
	glVertex3d(x1,y1,z1);
	glVertex3d(x2,y2,z2);
	glVertex3d(x3,y3,z3);
	glEnd();
}
void opengl_graphics_system::display_color(double red_color,double green_color,double blue_color)
{
	glColor3f((GLfloat)red_color,(GLfloat)green_color,(GLfloat)blue_color);

}
void opengl_graphics_system::display_material(mount_material_property *material_property,double transparence_ratio)
{
	double p,r,g,b;
	GLfloat Ambient[4],Diffuse[4],Specular[4],Shininess[4],Transparency[4],Emission[4];

	r=material_property->material_value[0];
	g=material_property->material_value[1];
	b=material_property->material_value[2];

	p=material_property->material_value[3];
	Ambient[0]=(GLfloat)(p*r);
	Ambient[1]=(GLfloat)(p*g);
	Ambient[2]=(GLfloat)(p*b);
	Ambient[3]=(GLfloat)transparence_ratio;

	p=material_property->material_value[4];
	Diffuse[0]=(GLfloat)(p*r);
	Diffuse[1]=(GLfloat)(p*g);
	Diffuse[2]=(GLfloat)(p*b);
	Diffuse[3]=(GLfloat)transparence_ratio;

	p=material_property->material_value[5];
	Specular[0]=(GLfloat)(p*r);
	Specular[1]=(GLfloat)(p*g);
	Specular[2]=(GLfloat)(p*b);
	Specular[3]=(GLfloat)transparence_ratio;

	p=material_property->material_value[6];
	Shininess[0]=(GLfloat)(p*r);
	Shininess[1]=(GLfloat)(p*g);
	Shininess[2]=(GLfloat)(p*b);
	Shininess[3]=(GLfloat)transparence_ratio;

	p=material_property->material_value[7];
	Transparency[0]=(GLfloat)(p*r);
	Transparency[1]=(GLfloat)(p*g);
	Transparency[2]=(GLfloat)(p*b);
	Transparency[3]=(GLfloat)transparence_ratio;

	p=material_property->material_value[8];	

	Emission[0]=(GLfloat)(p*r);
	Emission[1]=(GLfloat)(p*g);
	Emission[2]=(GLfloat)(p*b);
	Emission[3]=(GLfloat)transparence_ratio;

	glMaterialfv(GL_FRONT,GL_AMBIENT,Ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,Diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,Specular);
	glMaterialfv(GL_FRONT,GL_SHININESS,Shininess);
	glMaterialfv(GL_FRONT,GL_EMISSION,Emission);

	return;
}
void opengl_graphics_system::specify_face_point(BOOL display_texture_flag,double *tesstriangles,double *tessnorms,double *tesstexture)
{
	glEnableClientState(GL_VERTEX_ARRAY);	
	glVertexPointer(3,GL_DOUBLE,0,tesstriangles);

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_DOUBLE,0,tessnorms);

	if(display_texture_flag){
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2,GL_DOUBLE,0,tesstexture);
	}else
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	return;
}
void opengl_graphics_system::display_face(int tesstrianglecount)
{
	glDrawArrays(GL_TRIANGLES,0,tesstrianglecount*3);
	return;
}
void opengl_graphics_system::display_box(double box_data[])
{
	double x0,y0,z0,x1,y1,z1;
	
	x0=box_data[0];
	y0=box_data[1];
	z0=box_data[2];
	x1=box_data[3];
	y1=box_data[4];
	z1=box_data[5];

	glBegin(GL_QUADS);

	glNormal3d(-1,0,0);
	glVertex3d(x0,y0,z0);
	glVertex3d(x0,y1,z0);
	glVertex3d(x0,y1,z1);
	glVertex3d(x0,y0,z1);

	glNormal3d(1,0,0);
	glVertex3d(x1,y0,z0);
	glVertex3d(x1,y0,z1);	
	glVertex3d(x1,y1,z1);
	glVertex3d(x1,y1,z0);

	glNormal3d(0,-1,0);
	glVertex3d(x0,y0,z0);
	glVertex3d(x0,y0,z1);
	glVertex3d(x1,y0,z1);
	glVertex3d(x1,y0,z0);

	glNormal3d(0,1,0);
	glVertex3d(x0,y1,z0);
	glVertex3d(x1,y1,z0);	
	glVertex3d(x1,y1,z1);
	glVertex3d(x0,y1,z1);
	
	glNormal3d(0,0,1);
	glVertex3d(x0,y0,z0);
	glVertex3d(x1,y0,z0);
	glVertex3d(x1,y1,z0);
	glVertex3d(x0,y1,z0);

	glNormal3d(0,0,-1);
	glVertex3d(x0,y0,z1);
	glVertex3d(x0,y1,z1);
	glVertex3d(x1,y1,z1);
	glVertex3d(x1,y0,z1);

	glEnd();

	return;
}
int opengl_graphics_system::get_max_call_depth()
{
	int max_call_list_depth;
	glGetIntegerv(GL_MAX_LIST_NESTING,&max_call_list_depth);
	return max_call_list_depth;
}
void opengl_graphics_system::begin_create_call_list(int call_list_id)
{
	glDeleteLists(call_list_id,1);
	glNewList(call_list_id,GL_COMPILE);	
}
void opengl_graphics_system::end_create_call_list()
{
	glEndList();
}
void opengl_graphics_system::execute_call_list(int call_list_id)
{
	glCallList(call_list_id);
}
void opengl_graphics_system::push_matrix()
{
	glPushMatrix();
}
void opengl_graphics_system:: pop_matrix()
{
	glPopMatrix();
}
void opengl_graphics_system::load_identity_matrix()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void opengl_graphics_system::mult_matrix(double matrix_data[])
{
	glMultMatrixd(matrix_data);
}
void opengl_graphics_system::get_current_matrix(double matrix_data[])
{
	glGetDoublev(GL_MODELVIEW_MATRIX,matrix_data);
}
void opengl_graphics_system::begin_transparence_display()
{
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
}
void opengl_graphics_system::end_transparence_display()
{
	glBlendFunc(GL_ONE,GL_ZERO);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}
void opengl_graphics_system::turn_on_clip_plane(double *clip_plane_data)
{
	if(clip_plane_data==NULL){		
		glDisable(GL_CLIP_PLANE0);
		glDisable(GL_STENCIL_TEST);
	}else{
		glClipPlane(GL_CLIP_PLANE0,clip_plane_data);

		glEnable(GL_CLIP_PLANE0);
		glEnable(GL_STENCIL_TEST);
		glClearStencil(0);
		glClear(GL_STENCIL_BUFFER_BIT);
		glStencilFunc(GL_ALWAYS,0x01,0x01);
		glStencilOp(GL_KEEP,GL_INCR,GL_INCR);
	}
}
void opengl_graphics_system::begin_display_box_on_clip_plane()
{
	glDisable(GL_CLIP_PLANE0);
	glStencilFunc(GL_NOTEQUAL,0x00,0x01);
	glStencilOp(GL_KEEP,GL_ZERO,GL_ZERO);

	return;
}
void opengl_graphics_system::end_display_box_on_clip_plane()
{	
	glEnable(GL_CLIP_PLANE0);
	glStencilFunc(GL_ALWAYS,0x01,0x01);
	glStencilOp(GL_KEEP,GL_INCR,GL_INCR);

	return;
}


BOOL opengl_graphics_system::get_xy(double sx, double sy, double sz,double *x, double *y, double *z)
{
	GLdouble	modelview[16];
	GLdouble	projection[16];
	GLint		viewport[4];

	glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
	glGetDoublev(GL_PROJECTION_MATRIX,projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	if(gluUnProject((GLdouble)sx,
	   (GLdouble)((float)(viewport[3])-(float)sy),(GLdouble)sz,
	   modelview,projection, viewport,x,y,z)!=GL_TRUE)
		return FALSE;
	return TRUE;
}
BOOL opengl_graphics_system::get_screen_xy(double x, double y, double z,double *sx, double *sy)
{
	GLdouble	modelview[16];
	GLdouble	projection[16];
	GLint		viewport[4];
	double sz;

	glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
	glGetDoublev(GL_PROJECTION_MATRIX,projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	return (gluProject(x,y,z,modelview,projection,viewport,sx,sy,&sz)==GL_TRUE)?TRUE:FALSE;
}
void opengl_graphics_system::get_view_size(double *sx, double *sy)
{
	GLint	viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	*sx=viewport[2];
	*sy=viewport[3];
	return ;
}
void opengl_graphics_system::setup_view(double view_depth,
	double eye_x,double eye_y,double eye_z,
	double center_x,double center_y,double center_z,
	double up_x,double up_y,double up_z)
{
	double width,height;
	double sx,sy;

	view_depth*=2;

	width =(center_x-eye_x)*(center_x-eye_x);
	width+=(center_y-eye_y)*(center_y-eye_y);
	width+=(center_z-eye_z)*(center_z-eye_z);
	width=sqrtl(width);

	get_view_size(&sx,&sy);
	height=width*sy/sx;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-width,width,-height,height,width-view_depth,width+view_depth);
	
//	glDepthRange(width-view_depth,width+view_depth);
//	glClearDepth(width+view_depth);

	glDepthRange(0,1);
	glClearDepth(1);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glPolygonMode(GL_FRONT,GL_FILL);

	glShadeModel(GL_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);

	glBlendFunc(GL_ONE,GL_ZERO);
	glDisable(GL_BLEND);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eye_x,eye_y,eye_z,center_x,center_y,center_z,up_x-eye_x,up_y-eye_y,up_z-eye_z);
	
	return;
}

void opengl_graphics_system::clear_background(double red_color,double green_color,double blue_color)
{
	glClearColor((GLfloat)red_color,(GLfloat)green_color,(GLfloat)blue_color,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
}
void opengl_graphics_system::draw_background (double x,double y,double z,int width,int height,void *background)
{
	glRasterPos3d(x,y,z);
	glDrawPixels(width,height,GL_RGB,GL_FLOAT,(GLfloat *)background);
	glClear(GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
}
void opengl_graphics_system::graphics_setup_light(int max_light_number,struct graphics_system_light light_data[],double environment_red_color,double environment_green_color,double environment_blue_color,BOOL separate_specular_color_flag)
{
	int i;
	GLfloat data[4];
	static const GLenum light_id[8]={
		GL_LIGHT0,		GL_LIGHT1,		GL_LIGHT2,		GL_LIGHT3,
		GL_LIGHT4,		GL_LIGHT5,		GL_LIGHT6,		GL_LIGHT7
	};	
	
	if(max_light_number<=0){
		glDisable(GL_LIGHTING);
		return;
	}
	for(i=0;(i<max_light_number)&&(i<sizeof(light_id)/sizeof(light_id[0]));i++){
		if(!(light_data[i].effective_flag))
			glDisable(light_id[i]);
		else{
			data[0]=(GLfloat)(light_data[i].x);
			data[1]=(GLfloat)(light_data[i].y);
			data[2]=(GLfloat)(light_data[i].z);
			data[3]=1.0;
			glLightfv(light_id[i],GL_POSITION,data);

			data[0]=(GLfloat)((light_data[i].R)*(light_data[i].Ambient));
			data[1]=(GLfloat)((light_data[i].G)*(light_data[i].Ambient));
			data[2]=(GLfloat)((light_data[i].B)*(light_data[i].Ambient));
			data[3]=1.0;
			glLightfv(light_id[i],GL_AMBIENT,data);

			data[0]=(GLfloat)((light_data[i].R)*(light_data[i].Diffuse));
			data[1]=(GLfloat)((light_data[i].G)*(light_data[i].Diffuse));
			data[2]=(GLfloat)((light_data[i].B)*(light_data[i].Diffuse));
			data[3]=1.0;
			glLightfv(light_id[i],GL_DIFFUSE,data);

			data[0]=(GLfloat)((light_data[i].R)*(light_data[i].Specular));
			data[1]=(GLfloat)((light_data[i].G)*(light_data[i].Specular));
			data[2]=(GLfloat)((light_data[i].B)*(light_data[i].Specular));
			data[3]=1.0;
			glLightfv(light_id[i],GL_SPECULAR,data);

			data[0]=(GLfloat)((light_data[i].R)*(light_data[i].Emission));
			data[1]=(GLfloat)((light_data[i].G)*(light_data[i].Emission));
			data[2]=(GLfloat)((light_data[i].B)*(light_data[i].Emission));
			data[3]=1.0;
			glLightfv(light_id[i],GL_EMISSION,data);

			data[0]=(GLfloat)((light_data[i].R)*(light_data[i].Shininess));
			data[1]=(GLfloat)((light_data[i].G)*(light_data[i].Shininess));
			data[2]=(GLfloat)((light_data[i].B)*(light_data[i].Shininess));
			data[3]=1.0;
			glLightfv(light_id[i],GL_SHININESS,data);
	
			glEnable(light_id[i]);
		}
	}
	{
		GLfloat ambient_color[4]={(GLfloat)environment_red_color,(GLfloat)environment_green_color,(GLfloat)environment_blue_color,1.0};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambient_color);
	}
	glLightModeli (GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
	glLightModeli (GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);
	glLightModeli (GL_LIGHT_MODEL_COLOR_CONTROL,separate_specular_color_flag?GL_SEPARATE_SPECULAR_COLOR:GL_SINGLE_COLOR);

	glEnable(GL_LIGHTING);

	return;
}

BOOL opengl_graphics_system::get_screen_color(double sx,double sy,double *color_buffer)
{
	GLfloat color[3];
	GLint	viewport[4];

	glFinish();
	glGetIntegerv(GL_VIEWPORT,viewport);
	glReadPixels((GLint)sx,viewport[3]-(GLint)sy,1,1,GL_RGB,GL_FLOAT,color);
	
	color_buffer[0]=(double)(color[0]);
	color_buffer[1]=(double)(color[1]);
	color_buffer[2]=(double)(color[2]);

	return TRUE;
}
int opengl_graphics_system::register_texure_object(int width,int height,float *texture_data,BOOL mipmap_flag)
{
	GLuint textname;

	glGenTextures(1,&textname);
	glBindTexture(GL_TEXTURE_2D,textname);

	if(mipmap_flag)
		gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,width,height,GL_RGBA,GL_FLOAT,texture_data);
	else
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_FLOAT,texture_data);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	if(mipmap_flag)
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	else
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	return textname;
}
void opengl_graphics_system::replace_texure_object(int object_id,int width,int height,float *texture_data)
{
	glBindTexture(GL_TEXTURE_2D,(GLuint)object_id);
	glTexSubImage2D(GL_TEXTURE_2D,0,0,0,width,height,GL_RGBA,GL_FLOAT,texture_data);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
}
void opengl_graphics_system::unregister_texure_object(int object_id)
{
	GLuint textname=object_id;
	glDeleteTextures(1,&textname);
	return;
}
void opengl_graphics_system::begin_texure_display(
	int object_id,mount_material_property *texture_material,mount_material_property *color_material,double transparence_ratio)
{
	glBindTexture(GL_TEXTURE_2D,(GLuint)object_id);
	switch(texture_material->color_operation){
	default:
	case 1:
		glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
		break;
	case 2:
		GLfloat p[4]={
			(GLfloat)(color_material->material_value[0]),
			(GLfloat)(color_material->material_value[1]),
			(GLfloat)(color_material->material_value[2]),
			1};
		display_material(color_material,transparence_ratio);
		glTexEnvfv(GL_TEXTURE_ENV,GL_TEXTURE_ENV_COLOR,p);
		glTexEnvf (GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_BLEND);
		break;
	}
	glEnable(GL_TEXTURE_2D);

	return;
}

void opengl_graphics_system::end_texure_display()
{
	glDisable(GL_TEXTURE_2D);
}
