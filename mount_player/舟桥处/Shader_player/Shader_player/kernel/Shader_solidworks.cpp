#include "StdAfx.h"

#include "..\Shader_player.h"

#include "Shader_solidworks.h"
#include "GL/glew.h"
#include "Texture.h"

#include "Shader_solidworks_material_dialog.h"

static Shader_solidworks a(_T("Soliworks渲染器"));

Shader_solidworks::Shader_solidworks(CString my_class_name,CString my_directory_name,CString my_file_name)
	:Shader_base(my_class_name,my_directory_name,my_file_name)
{
	material_number=0;
	prepare_flag=false;

	for(int i=0;i<MAX_SOLIDWORKS_MATERIAL_NUMBER;i++)
		material[i].texture_object_id=0;
}
Shader_solidworks::~Shader_solidworks()
{
	set_reload_material();
}

void Shader_solidworks::modify_material()
{
	Shader_solidworks_material_dialog (this).DoModal();
}
void Shader_solidworks::save_material()
{
	set_reload_material();

	ofstream f(directory_name+file_name);

	f<<"数目    "<<(material_number)<<"\n";

	for(int i=0;i<material_number;i++){
		f<<"第    "<<i<<"    种材质\n";
		f<<"        材质参数  ";
		
		f<<"        "<<(material[i].red);
		f<<"        "<<(material[i].green);
		f<<"        "<<(material[i].blue);

		f<<"        "<<(material[i].ambient);
		f<<"        "<<(material[i].diffuse);
		f<<"        "<<(material[i].specular);

		f<<"        "<<(material[i].shininess);
		f<<"        "<<(material[i].transparency);
		f<<"        "<<(material[i].emission)<<"\n";
		f<<"        纹理文件名    ";
		File_data::output_string(f,material[i].file_name+_T("\n"));
		f<<"        旋转角度      "<<(material[i].angle)<<"\n";
		f<<"        放大倍数      "<<(material[i].scale)<<"\n";
		f<<"        混合标志      ";
		if(material[i].blend_flag)
			f<<"混合\n\n";
		else
			f<<"不混合\n\n";
	}
	f<<"\n\n";
}

void  Shader_solidworks::set_reload_material()
{
	for(int i=0;i<material_number;i++)
		if(material[i].texture_object_id!=0){
			::glDeleteTextures(1,(GLuint*)&(material[i].texture_object_id));
			material[i].texture_object_id=0;
		}
	prepare_flag=false;
}
void Shader_solidworks::prepare(Component_display_parameter *par,Component_display_flag *flag)
{
	int i,width,height;
	float *data_pointer;

	if(prepare_flag)
		return;

	ifstream f(directory_name+file_name);
	char buffer[MAX_BUFFER_LENGTH];

	for(i=0;i<material_number;i++)
		if(material[i].texture_object_id!=0){
			::glDeleteTextures(1,(GLuint *)&(material[i].texture_object_id));
			material[i].texture_object_id=0;
		}

	f>>buffer>>material_number;
	
	if(material_number>(sizeof(material)/sizeof(material[0])))
		material_number=sizeof(material)/sizeof(material[0]);
	if(material_number<0)
		material_number=0;
	for(i=0;i<material_number;i++){

			f>>buffer>>buffer>>buffer>>buffer;

			f>>(material[i].red);
			f>>(material[i].green);
			f>>(material[i].blue);

			f>>(material[i].ambient);
			f>>(material[i].diffuse);
			f>>(material[i].specular);

			f>>(material[i].shininess);
			f>>(material[i].transparency);
			f>>(material[i].emission);

			f>>buffer>>buffer;	
			material[i].tex.load_from_file(directory_name+CString(buffer));
			material[i].file_name=buffer;

			f>>buffer>>(material[i].angle);				material[i].cos_v=(float)(::cosl(material[i].angle));		material[i].sin_v=(float)(::sinl(material[i].angle));
			f>>buffer>>(material[i].scale);
			f>>buffer>>buffer;
			material[i].blend_flag=(CString(buffer)==_T("混合"))?true:false;

			data_pointer=material[i].tex.get_image_data(&width,&height);
			if(data_pointer==NULL)
				material[i].texture_object_id=-1;
			else{
				::glGenTextures(1,(GLuint *)&(material[i].texture_object_id));
				::glBindTexture(GL_TEXTURE_2D,material[i].texture_object_id);

				::gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,width,height,GL_RGBA,GL_FLOAT,data_pointer);
				::glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
				::glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

				::glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
				::glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
			}
	}
	f.close();
	
	for(i=0;i<material_number;i++){
		parameter[16*i+ 0]=material[i].red;
		parameter[16*i+ 1]=material[i].green;
		parameter[16*i+ 2]=material[i].blue;
		parameter[16*i+ 3]=1.0;

		parameter[16*i+ 4]=material[i].ambient;
		parameter[16*i+ 5]=material[i].diffuse;
		parameter[16*i+ 6]=material[i].specular;
		parameter[16*i+ 7]=1.0;

		parameter[16*i+ 8]=material[i].shininess;
		parameter[16*i+ 9]=material[i].transparency;
		parameter[16*i+10]=material[i].emission;
		parameter[16*i+11]=1.0;

		parameter[16*i+12]=material[i].cos_v;
		parameter[16*i+13]=material[i].sin_v;
		parameter[16*i+14]=material[i].scale;

		if(material[i].texture_object_id<=0)
				parameter[16*i+15]=0.0;
		else if(material[i].blend_flag)
				parameter[16*i+15]=2;
		else
				parameter[16*i+15]=1;
	}

	prepare_flag=true;

}
void Shader_solidworks::set_material(Component_display_parameter *par,Component_display_flag *flag)
{
	prepare(par,flag);

	for(int i=0;i<material_number;i++){
		::glActiveTexture(GL_TEXTURE1+i);
		if(material[i].texture_object_id<=0)
			::glDisable(GL_TEXTURE_2D);
		else{
			const char *var_name[]=	{
				"image0",		"image1",		"image2",		"image3",		
				"image4",		"image5",		"image6",		"image7"
			};
			::glEnable(GL_TEXTURE_2D);
			::glBindTexture(GL_TEXTURE_2D,(GLuint)(material[i].texture_object_id));			
			::glUniform1i(::glGetUniformLocation(get_program_id(),var_name[i]),i+1);	
		}
	}

	::glUniformMatrix4fv(::glGetUniformLocation(get_program_id(),"parameter"),material_number,GL_FALSE,parameter);
	::glUniform1i(::glGetUniformLocation(get_program_id(),"light_number"),par->lights->light_number);

	::glUniform4f(::glGetUniformLocation(get_program_id(),"eye_location"),
		(float)(par->camera->eye_point.x),(float)(par->camera->eye_point.y),(float)(par->camera->eye_point.z),1.0);

}

int Shader_solidworks::test_display(Component *comp,Component_display_parameter *par,Component_display_flag *flag,Component_display_list *display_list)
{
	return 0;
}
Shader_part_information *Shader_solidworks::get_part_information()
{
	info.display_flag=true;
	info.shader_has_box_flag=true;

	return &info;
}