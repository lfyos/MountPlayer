#include "StdAfx.h"
#include "graphics_system.h"
#include "..\Shader_player.h"


#include "GL/glew.h"
#include "GL/wglew.h"

#include "GL/glext.h"
#include "GL/wglext.h"

#include "GL/glut.h"

#include "camera.h"

#pragma comment( lib,"glew32.lib")
#pragma comment( lib,"glew32s.lib")
#pragma comment( lib,"glew32mx.lib")
#pragma comment( lib,"glew32mxs.lib")
#pragma comment( lib,"glut32.lib")

static Graphics_system *gs_head=NULL;
static unsigned int parameter_block_buffer_object_id=0;

Graphics_system::Graphics_system(CDC &pdc,int &max_user_parameter_number,CString directory_name)
{
	graphics_system_flag=true;

	if(gs_head==NULL){
		gs_head=this;
		front_gs=this;
		back_gs=this;
	}else{
		front_gs=gs_head;
		back_gs=front_gs->back_gs;

		front_gs->back_gs=this;
		back_gs->front_gs=this;
	}
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
	if(pdf.cDepthBits<64)
		pdf.cDepthBits=64;
		
	int pixelFormat=::ChoosePixelFormat(pdc.m_hDC, &pdf);
	BOOL success=::SetPixelFormat(pdc.m_hDC,pixelFormat,&pdf);

	
	m_hRC=::wglCreateContext(pdc.m_hDC);
	::wglMakeCurrent(pdc.m_hDC,m_hRC);
	
	::glewInit();

	{
		static bool flag=true;
		CString version_string=CString(::glGetString(GL_VERSION));
		if(flag){
			flag=false;
			ofstream f(directory_name+_T("temp\\graphics.lfy"));
			File_data::output_string(f,_T("OpenGL ")+version_string+_T("\n"));
			f.close();
		}
		if(version_string<_T("3.2")){
			::wglMakeCurrent(pdc.m_hDC,NULL);
			CString str;
			str+=_T("运行本系统要求OpenGL3.2或以上版本\n");
			str+=_T("OpenGL当前版本为:")+version_string+_T(",OpenGL版本过低\n");
			str+=_T("请更新显卡或显卡驱动程序以支持高版本OpenGL\n");
			
			graphics_system_flag=false;
			return;
		}
	}
	if(gs_head!=this)
		::wglShareLists(gs_head->m_hRC,m_hRC);
	else{
		int max_length;

		::glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE,&max_length);
		max_length=max_length/sizeof(float);

		int order_length=Component_display_parameter::get_display_data_size();
		int user_length=4*max_user_parameter_number;

		if((order_length+user_length)>=max_length){
			user_length=max_length-order_length;
			max_user_parameter_number=user_length/4;
		}

		::glGenBuffers(1,&parameter_block_buffer_object_id);
		::glBindBuffer(GL_UNIFORM_BUFFER,parameter_block_buffer_object_id);
		::glBufferData(GL_UNIFORM_BUFFER,(order_length+user_length)*sizeof(float),NULL,GL_DYNAMIC_DRAW);
	}		
	::wglMakeCurrent(pdc.m_hDC,NULL);
}

Graphics_system::~Graphics_system()
{
	wglDeleteContext(m_hRC);

	if(front_gs==this){
		gs_head=NULL;
		if(parameter_block_buffer_object_id!=0){
			::glDeleteBuffers(1,&parameter_block_buffer_object_id);
			parameter_block_buffer_object_id=0;
		}
	}else{
		if(gs_head==this)
			gs_head=front_gs;
		front_gs->back_gs=back_gs;
		back_gs->front_gs=front_gs;
	}
	front_gs=NULL;
	back_gs=NULL;
}

void Graphics_system::begin_display(CDC &dc)
{
	::wglMakeCurrent(dc.m_hDC,m_hRC);
	::glDisable(GL_NORMALIZE);
	::glUseProgram(0);
}
void Graphics_system::end_display(CDC &dc,bool swap_flag)
{
	if(swap_flag){
		::glFlush();
		::SwapBuffers(dc.m_hDC);
	}
	::wglMakeCurrent(dc.m_hDC,NULL);
}

void Graphics_system::clear_screen(Point clear_color)
{
	::glClearColor(clear_color.x,clear_color.y,clear_color.z,1);
	::glDepthRange(0,1);
	::glClearDepth(1);
	::glClearStencil(0);
	::glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
}
int Graphics_system::compile_shader(CString &display_error_message,char *shader_program,int type_id)
{
	const GLuint shader_type_id[]={
		GL_VERTEX_SHADER,
		GL_FRAGMENT_SHADER,
		GL_GEOMETRY_SHADER_EXT,
		GL_TESS_CONTROL_SHADER,
		GL_TESS_EVALUATION_SHADER,
	};
	int complie_status,shader_id;
	GLchar err_log[MAX_BUFFER_LENGTH];
	GLsizei err_length=0;

	if((shader_id=::glCreateShader(shader_type_id[type_id]))<=0){
		display_error_message+=(_T("\n创建着色器错误\n"));
		return 0;
	}
	char *shader_program_array[]={shader_program};
	::glShaderSource(shader_id,1,(const GLchar **)shader_program_array,NULL);

	::glCompileShader(shader_id);

	::glGetShaderiv(shader_id,GL_OBJECT_COMPILE_STATUS_ARB,&complie_status);
				
	if(complie_status==GL_TRUE)
		return shader_id;

	::glGetInfoLogARB(shader_id,sizeof(err_log)/sizeof(err_log[0]),&err_length,err_log);
	::glDeleteShader(shader_id);

	err_log[err_length]='\0';
	display_error_message+=(_T("\n编译着色器程序出错\n")+CString(err_log))+_T("\n")+CString(shader_program)+_T("\n");

	
	return 0;
}
int Graphics_system::link_program(CString &display_error_message,int shader_number,int shader_id[],CString shader_program_file_name[])

{
	int i,program_id;
	GLint result,length;
	GLchar err_log[MAX_BUFFER_LENGTH];
		
	if((program_id=::glCreateProgram())<=0){
		display_error_message+=_T("\n创建着色器程序错误\n");
		return 0;
	}
	for(i=0;i<shader_number;i++){
		if(shader_id[i]>0)
			::glAttachShader(program_id,shader_id[i]);
	}
	
	{
		::glBindAttribLocation(program_id, 0,"vertex_location");
		::glBindAttribLocation(program_id, 1,"vertex_normal");
		::glBindAttribLocation(program_id, 2,"vertex_attribute_0");
		::glBindAttribLocation(program_id, 3,"vertex_attribute_1");
		
		::glBindAttribLocation(program_id, 4,"vertex_texture0");
		::glBindAttribLocation(program_id, 5,"vertex_texture1");
		::glBindAttribLocation(program_id, 6,"vertex_texture2");
		::glBindAttribLocation(program_id, 7,"vertex_texture3");
		::glBindAttribLocation(program_id, 8,"vertex_texture4");
		::glBindAttribLocation(program_id, 9,"vertex_texture5");
		::glBindAttribLocation(program_id,10,"vertex_texture6");
		::glBindAttribLocation(program_id,11,"vertex_texture7");

	}

	::glLinkProgram (program_id);
	::glGetProgramiv(program_id,GL_OBJECT_LINK_STATUS_ARB,&result);
	
	if(result){
		::glUseProgram(program_id);
		::glBindBuffer(GL_UNIFORM_BUFFER,parameter_block_buffer_object_id);
		::glBindBufferBase(GL_UNIFORM_BUFFER,::glGetUniformBlockIndex(program_id,"parameter_block"),parameter_block_buffer_object_id);
		return program_id;
	}

	::glGetProgramInfoLog(program_id,MAX_BUFFER_LENGTH,&length,err_log);
	err_log[length]='\0';

	CString str=CString(_T("\n连接着色器程序出错\n"))+err_log;
	for(i=0;i<shader_number;i++)
		if(shader_id[i]>0)
			str=str+_T("着色器程序文件: ")+shader_program_file_name[i]+_T("\n");

	for(i=0;i<shader_number;i++)
		if(shader_id[i]>0)
			::glDetachShader(program_id,shader_id[i]);

	display_error_message+=str+_T("\n");

	return 0;
}
void Graphics_system::turn_onoff_line_stipple_and_polygon_offset(
					bool line_stipple_onoff_flag,		int line_stipple_factor,		int line_stipple_pattern,
					bool polygon_offset_onoff_flag,		double polygon_offset_factor,	double polygon_offset_units)
{
	{
		if(line_stipple_onoff_flag){
			::glLineStipple((GLint)line_stipple_factor,(GLushort)line_stipple_pattern);
			::glEnable(GL_LINE_STIPPLE);
		}else
			::glDisable(GL_LINE_STIPPLE);
	}
	{
		::glDisable(GL_POLYGON_OFFSET_POINT);
		::glDisable(GL_POLYGON_OFFSET_LINE);
		if(polygon_offset_onoff_flag){
			::glPolygonOffset((GLfloat)polygon_offset_factor,(GLfloat)polygon_offset_units);
			::glEnable(GL_POLYGON_OFFSET_FILL);
		}else{
			::glDisable(GL_POLYGON_OFFSET_FILL);
		}
	}
}
void Graphics_system::delete_program(int program_id)
{
	::glDeleteProgram((GLuint)program_id);
	
}
void Graphics_system::delete_shader(int shader_id)
{
	::glDeleteShader((GLuint)shader_id);
}

Point Graphics_system::screen_to_object(Point pp,Location project_matrix)
{
	double		modelview_matrix_data[16],project_matrix_data[16];
	GLint		viewport[4];	
	
	::glGetIntegerv(GL_VIEWPORT,viewport);
	
	Location().get_location_double_data(modelview_matrix_data);
	project_matrix.get_location_double_data(project_matrix_data);
	
	double x,y,z;
	::gluUnProject(pp.x,(double)(viewport[3])-pp.y,pp.z,
			modelview_matrix_data,project_matrix_data,viewport,&x,&y,&z);
	
	return Point(x,y,z);
}
Point Graphics_system::object_to_screen( Point  pp,Location project_matrix)
{
	double		modelview_matrix_data[16],project_matrix_data[16];
	GLint		viewport[4];
	double x,y,z;
	
	::glGetIntegerv(GL_VIEWPORT, viewport);
	Location().get_location_double_data(modelview_matrix_data);
	project_matrix.get_location_double_data(project_matrix_data);

	::gluProject(pp.x,pp.y,pp.z,modelview_matrix_data,project_matrix_data,viewport,&x,&y,&z);

	return Point(x,y,z);
}

void Graphics_system::set_camera(double view_port_width,double view_port_height)
{
	::glViewport(0,0,view_port_width,view_port_height);

	::glDisable(GL_DITHER);	

	::glEnable(GL_DEPTH_TEST);
	::glDepthFunc(GL_LEQUAL);
	::glDepthRange(0.0,1.0);

	::glDisable(GL_BLEND);
	::glDepthMask(GL_TRUE);


	::glDisable(GL_DEPTH_CLAMP);

	{
		::glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		::glShadeModel(GL_SMOOTH);
		::glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);
	//	::glHint(GL_POLYGON_SMOOTH_HINT,GL_FASTEST);
		::glEnable(GL_SMOOTH);
	//	::glDisable(GL_SMOOTH);


		::glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	//	::glHint(GL_LINE_SMOOTH_HINT,GL_FASTEST);
	//	::glEnable(GL_LINE_SMOOTH);
		::glDisable(GL_LINE_SMOOTH);
	

		::glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
		::glEnable(GL_POINT_SMOOTH);
	}
}

Point Graphics_system::width_and_height()
{
	GLint viewport[4];
	::glGetIntegerv(GL_VIEWPORT, viewport);
	return Point((double)(viewport[2]),(double)(viewport[3]),0);
}
Point Graphics_system::get_screen_color(double sx,double sy,double &depth_value)
{
	GLfloat color[4]={0,0,0,1},my_depth_value[]={0,0,0,1};
	GLint viewport[4];

	::glGetIntegerv(GL_VIEWPORT,viewport);

	::glFinish();

	::glReadPixels((GLint)sx,viewport[3]-(GLint)sy,1,1,GL_RGB,GL_FLOAT,color);	
	::glReadPixels((GLint)sx,viewport[3]-(GLint)sy,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,my_depth_value);

	depth_value=(double)(my_depth_value[0]);

	return Point((double)(color[0]),(double)(color[1]),(double)(color[2]));
}

int Graphics_system::get_uniform_index(int program_id,const char *var_name)
{
	return ::glGetUniformLocation(program_id,var_name);
}

int Graphics_system::create_buffer_object(float *data_pointer,int size)
{
	GLuint id;

	::glGenBuffers(1,&id);
	::glBindBuffer(GL_ARRAY_BUFFER,id);
	::glBufferData(GL_ARRAY_BUFFER,size*sizeof(float),data_pointer,GL_STATIC_DRAW);
	if(::glGetError()!=GL_OUT_OF_MEMORY)
		return id;
	::glDeleteBuffers(1,&id);

	static bool flag=true;
	if(flag){
		flag=false;
		AfxMessageBox(_T("显卡存储容量不足,请购买高档显卡"));
	}
	return 0;
}

void Graphics_system::delete_buffer_object(int buffer_object_id)
{
	GLuint id=buffer_object_id;
	::glDeleteBuffers(1,&id);
}
void Graphics_system::set_user_parameter(int parameter_number,void *parameter_data_pointer,int base_id)
{
	int base=Component_display_parameter::get_display_data_size()+4*base_id;
	::glBindBuffer(GL_UNIFORM_BUFFER,parameter_block_buffer_object_id);
	::glBufferSubData(GL_UNIFORM_BUFFER,base*sizeof(float),4*parameter_number*sizeof(float),parameter_data_pointer);
}
void Graphics_system::set_display_parameter(Component_display_parameter *par)
{
	int length=Component_display_parameter::get_display_data_size();
	::glBindBuffer(GL_UNIFORM_BUFFER,parameter_block_buffer_object_id);
	::glBufferSubData(GL_UNIFORM_BUFFER,0,length*sizeof(float),par->get_display_parameter());
}
void Graphics_system::display_part_graphics_object(	int matrix_id,int graphics_object_type_id,Part_graphics_object *pg,int display_number,Component **component_pointer,
												Shader_program_variable_index *variable_index,Location *loca)
{
	int stride=pg->get_stride()*sizeof(float);
	float *vertex_data=(pg->part_buffer_object_id!=0)?(float *)0:(pg->get_vertex_data());
		
	::glBindBuffer(GL_ARRAY_BUFFER,pg->part_buffer_object_id);

	for(int i=0;i<8;i++){
		if(i>=(pg->texture_number))
			::glDisableVertexAttribArray(i+4);
		else{
			::glEnableVertexAttribArray(i+4);
			::glVertexAttribPointer(i+4,4,GL_FLOAT,false,stride, vertex_data+16+4*i);
		}
	}
	for(int i=0;i<=3;i++){
		::glEnableVertexAttribArray(i);
		::glVertexAttribPointer(i,4,GL_FLOAT,false,stride, vertex_data+4*i);
	}

	for(int parameter_length,instance_number,i=0;i<display_number;i++){
		Component *comp;
		Component_private_data_storage *private_data;
		float *order_parameter_data;

		if(component_pointer!=NULL)
		if((comp=component_pointer[i])!=NULL)
		if((private_data=comp->private_data_storage_pointer)!=NULL)
		if((parameter_length=private_data->get_order_parameter(matrix_id,loca,instance_number,order_parameter_data))>0)
		if(instance_number>0){
			::glUniform4fv(variable_index->order_parameter,parameter_length,order_parameter_data);
			::glDrawArraysInstanced(graphics_object_type_id,0,3*(pg->triangle_number),instance_number);
		}
	}
	for(int i=0;i<(pg->texture_number);i++)
		::glDisableVertexAttribArray(i+4);
}

void Graphics_system::set_display_part_box_on_clip_plane(Clipper *clipper,Component_display_parameter *par,Shader_program_variable_index *variable_index)
{
	Clip_component *cp;
	for(int i=0,do_clip_number=0;i<(clipper->clipper_number);i++){
		if((cp=clipper->clip[i])->do_clip_flag){
			if(cp->display_part_on_plane_flag)
				::glDisable(GL_CLIP_PLANE0+do_clip_number);
			do_clip_number++;
		}
	}
	::glUniform4fv(variable_index->clip_plane_parameter,Component_display_parameter::get_clip_data_size(),par->get_no_clip_parameter());	

	::glStencilFunc(GL_NOTEQUAL,0x00,0x01);
	::glStencilOp(GL_KEEP,GL_ZERO,GL_ZERO);
}

void Graphics_system::get_default_object_type_id(int *default_object_type_id)
{
	const int object_type_id[MAX_OBJECT_TYPE_ID]={
				GL_TRIANGLES,
				GL_TRIANGLES,
				GL_LINES,
				GL_POINTS,
				GL_LINES,
	};

	for(int i=0;(i<(sizeof(object_type_id)/sizeof(object_type_id[0])))&&(i<MAX_OBJECT_TYPE_ID);i++)
		default_object_type_id[i]=object_type_id[i];
}

bool Graphics_system::mount_program(int program_id,bool stencil_on_off_flag,bool depth_state,bool blend_state,double line_width,double point_size,Shader_program_variable_index *variable_index,Component_display_parameter *par)
{
	int display_box_number=0;

	::glUseProgram(program_id);
	
	{
		int do_clip_number=0;
		for(int i=0;i<(par->clipper->clipper_number);i++){
			Clip_component *cp=par->clipper->clip[i];
			if(cp->do_clip_flag)
				::glEnable(GL_CLIP_PLANE0+(do_clip_number++));
			if(cp->display_part_on_plane_flag)
				display_box_number++;
		}
		
		for(int i=do_clip_number;i<MAX_CLIP_PLANE_NUMBER;i++)
			::glDisable(GL_CLIP_PLANE0+i);

		for(int i=0;i<(par->lights->light_number);i++)
			if((par->lights->light_array[i].shadow_texture_unit_id>=0)&&(par->lights->light_array[i].shadow_texture_precision>0.0))
				::glUniform1i(variable_index->shadow_texture[i],par->lights->light_array[i].shadow_texture_unit_id);
		
		::glUniform4fv(variable_index->clip_plane_parameter,Component_display_parameter::get_clip_data_size(),par->get_clip_parameter());
	}

	::glLineWidth((GLfloat)line_width);
	::glPointSize((GLfloat)point_size);
		
	if(display_box_number==0)
		stencil_on_off_flag=false;

	if(stencil_on_off_flag){
		::glEnable(GL_STENCIL_TEST);
		::glStencilFunc(GL_ALWAYS,0x01,0x01);
		::glStencilOp(GL_KEEP,GL_INCR,GL_INCR);
	}else{
		::glDisable(GL_STENCIL_TEST);
		::glStencilFunc(GL_NEVER,0x01,0x01);
		::glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
	}

	::glDepthMask(depth_state?GL_TRUE:GL_FALSE);

	if(blend_state){
		::glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		::glEnable(GL_BLEND);
	}else{
		::glDisable(GL_BLEND);
	}

	return stencil_on_off_flag;
}

void Graphics_system::create_depth_texture_and_frame_buffer(int *frame_buffer_id,int *render_buffer_id,int *texture_id,int texture_width,int texture_height)
{
	unsigned int id;

	::glFlush();

	if((*texture_id)==0){
		::glGenTextures(1,&id);
		*texture_id=id;

		::glBindTexture(GL_TEXTURE_2D,*texture_id);
		::glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		::glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		::glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
		::glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);

		::glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,texture_width,texture_height,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
		::glBindTexture(GL_TEXTURE_2D,0);
	}
	if((*render_buffer_id)==0){
		::glGenRenderbuffers(1,&id);
		*render_buffer_id=id;

		::glBindRenderbuffer(GL_RENDERBUFFER,*render_buffer_id);
		::glRenderbufferStorage(GL_RENDERBUFFER,GL_RGBA,texture_width,texture_height);
		::glBindRenderbuffer(GL_RENDERBUFFER,0);

	}
	if((*frame_buffer_id)!=0)
		::glBindFramebuffer(GL_DRAW_FRAMEBUFFER,*frame_buffer_id);
	else{
		::glGenFramebuffers(1,&id);
		*frame_buffer_id=id;
		::glBindFramebuffer(GL_DRAW_FRAMEBUFFER,*frame_buffer_id);
		::glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_RENDERBUFFER,*render_buffer_id);
		::glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,*texture_id,0);
	}
}

void Graphics_system::create_mirror_texture_and_frame_buffer(int *frame_buffer_id,int *render_buffer_id,int *texture_id,int texture_width,int texture_height)
{
	unsigned int id;

	::glFlush();

	if((*texture_id)==0){
		::glGenTextures(1,&id);
		*texture_id=id;
		::glBindTexture(GL_TEXTURE_2D,*texture_id);

		::glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		::glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		::glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
		::glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
		
		::glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,texture_width,texture_height,0,GL_RGBA,GL_FLOAT,NULL);
		::glBindTexture(GL_TEXTURE_2D,0);
	}
	if((*render_buffer_id)==0){
		::glGenRenderbuffers(1,&id);
		*render_buffer_id=id;

		::glBindRenderbuffer(GL_RENDERBUFFER,*render_buffer_id);
		::glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,texture_width,texture_height);
		::glBindRenderbuffer(GL_RENDERBUFFER,0);
	}
	if((*frame_buffer_id)!=0)
		::glBindFramebuffer(GL_DRAW_FRAMEBUFFER,*frame_buffer_id);
	else{
		::glGenFramebuffers(1,&id);
		*frame_buffer_id=id;
	
		::glBindFramebuffer(GL_DRAW_FRAMEBUFFER,*frame_buffer_id);
		::glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,*render_buffer_id);
		::glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,*texture_id,0);
	}	
}

void Graphics_system::restore_frame_buffer()
{
	::glFlush();
	::glBindFramebuffer(GL_DRAW_FRAMEBUFFER,NULL);
}

void Graphics_system::get_depth_texture_image(int texture_id,int texture_width,int texture_height)
{
	::glFlush();
//	::glActiveTexture(GL_TEXTURE0);
//	::glBindTexture(GL_TEXTURE_2D,texture_id);
//	::glCopyTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,0,0,texture_width,texture_height,0);
}
void Graphics_system::get_mirror_texture_image(int texture_id,int texture_width,int texture_height)
{
	::glFlush();
//	::glActiveTexture(GL_TEXTURE0);
//	::glBindTexture(GL_TEXTURE_2D,texture_id);
//	::glCopyTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,0,0,texture_width,texture_height,0);
}

void Graphics_system::mount_depth_texture(int texture_unit_id,int texture_object_id)
{
	if(texture_unit_id>=0){
		::glActiveTexture(texture_unit_id+GL_TEXTURE0);
		if(texture_object_id>0){
			::glBindTexture(GL_TEXTURE_2D,texture_object_id);
			::glEnable(GL_TEXTURE_2D);
		}else{
			::glBindTexture(GL_TEXTURE_2D,0);
			::glDisable(GL_TEXTURE_2D);
		}
	}
}
void Graphics_system::delete_framebuffer_renderbuffer_and_texture(int *frame_buffer_id,int *render_buffer_id,int *texture_id)
{
	unsigned int id;
	if(texture_id!=NULL)
		if((id=(*texture_id))!=0){
			*texture_id=0;
			::glDeleteTextures(1,&id);
		}
	if(render_buffer_id!=NULL)
		if((id=(*render_buffer_id))!=0){
			*render_buffer_id=0;
			::glDeleteRenderbuffers(1,&id);
		}
	if(frame_buffer_id!=NULL)
		if((id=(*frame_buffer_id))!=0){
			*frame_buffer_id=0;
			::glDeleteFramebuffers(1,&id);
		}
}