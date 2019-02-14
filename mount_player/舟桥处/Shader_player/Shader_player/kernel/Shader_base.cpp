#include "StdAfx.h"
#include "Part.h"

static char *shader_type_string[]={
		"\n#version 150\n#define SHADER_TYPE_NORMAL		\n",
		"\n#version 150\n#define SHADER_TYPE_BOX		\n",
		"\n#version 150\n#define SHADER_TYPE_EDGE		\n",
		"\n#version 150\n#define SHADER_TYPE_POINT		\n",
		"\n#version 150\n#define SHADER_TYPE_FRAME		\n",
};

static int shader_type_id=0;

void Shader_base::set_shader_type(int my_shader_type_id)
{
	shader_type_id=my_shader_type_id;
}

class shader_program{
public:
	CString shader_file_name[MAX_SHADER_NUMBER];
	struct{
		int program_id,shader_id[MAX_SHADER_NUMBER];
		Shader_program_variable_index variable_index;
	}shader_id[sizeof(shader_type_string)/sizeof(shader_type_string[0])];
};

static int shader_program_number=0;
static shader_program shader_program[MAX_SHADER_PROGRAM_NUMBER];
static Shader_base *first_shader_pointer=NULL;

static CString directory_file_name,configure_shader_file_name,uniform_shader_file_name,vertex_shader_file_name;

void Shader_base::set_general_shader_file_name_and_max_draw_instance_number(
					CString my_directory_file_name,CString my_configure_shader_file_name,
					CString my_uniform_shader_file_name,CString my_vertex_shader_file_name)

{
	directory_file_name=my_directory_file_name;
	configure_shader_file_name=my_configure_shader_file_name;
	uniform_shader_file_name=my_uniform_shader_file_name;
	vertex_shader_file_name=my_vertex_shader_file_name;
}
bool Shader_base::is_same_shader(Shader_base *s,Shader_base*t)
{
	if((s==NULL)||(t==NULL))
		return false;
	if((s->get_program_index_id())!=(t->get_program_index_id()))
		return false;
	if((s->class_name)!=(t->class_name))
		return false;
	return true;
}
int Shader_base::get_program_id(Shader_program_variable_index **variable_index)
{
	int program_id;
	
	if((program_id=shader_program[program_index_id].shader_id[shader_type_id].program_id)<0){
		CString display_error_message=_T("");
		for(int i=0;i<MAX_SHADER_NUMBER;i++){
			if(shader_program[program_index_id].shader_id[shader_type_id].shader_id[i]<0){
				for(int j=0;j<shader_program_number;j++){
					if(j!=program_index_id)
						if(shader_program[j].shader_id[shader_type_id].shader_id[i]>0)
							if((shader_program[j].shader_file_name[i])==(shader_program[program_index_id].shader_file_name[i])){
								shader_program[program_index_id].shader_id[shader_type_id].shader_id[i]=shader_program[j].shader_id[shader_type_id].shader_id[i];
								break;
							}
				}
				if(shader_program[program_index_id].shader_id[shader_type_id].shader_id[i]<0){
					File_data program_code((directory_file_name)+(shader_program[program_index_id].shader_file_name[i]));
					if(program_code.data_pointer()!=NULL){
						CString str;

						if(i==0)
							program_code.insert_file_data(directory_file_name+vertex_shader_file_name);
						program_code.insert_file_data(directory_file_name+uniform_shader_file_name);
						program_code.insert_file_data(directory_file_name+configure_shader_file_name);

						Shader_part_information *info=get_part_information();
						str.Format(_T("\n#define MAX_USER_ORDER_NUMBER			%d\n"),(info->max_user_order_number<5)?5:(info->max_user_order_number));			program_code.insert_data(str);
						str.Format(_T("\n#define MAX_USER_PARAMETER_NUMBER		%d\n"),(info->max_user_parameter_number<2)?2:(info->max_user_parameter_number));	program_code.insert_data(str);
						str.Format(_T("\n#define MAX_LIGHT_NUMBER				%d\n"),(MAX_LIGHT_NUMBER<2)?2:MAX_LIGHT_NUMBER);									program_code.insert_data(str);
						str.Format(_T("\n#define MAX_CLIP_PLANE_NUMBER			%d\n"),(MAX_CLIP_PLANE_NUMBER<2)?2:MAX_CLIP_PLANE_NUMBER);							program_code.insert_data(str);

						program_code.insert_data(CString(shader_type_string[shader_type_id]));

						shader_program[program_index_id].shader_id[shader_type_id].shader_id[i]=Graphics_system::compile_shader(display_error_message,program_code.data_pointer(),i);
					}
				}
			}
		}
		program_id=Graphics_system::link_program(display_error_message,
							MAX_SHADER_NUMBER,
							shader_program[program_index_id].shader_id[shader_type_id].shader_id,
							shader_program[program_index_id].shader_file_name);

		shader_program[program_index_id].shader_id[shader_type_id].program_id=program_id;

		shader_program[program_index_id].shader_id[shader_type_id].variable_index.order_parameter		=Graphics_system::get_uniform_index(program_id,"order_parameter");
		shader_program[program_index_id].shader_id[shader_type_id].variable_index.clip_plane_parameter	=Graphics_system::get_uniform_index(program_id,"clip_plane_parameter");

		const char *shadow_texture_name[]={
					"shadow_texture0", "shadow_texture1", "shadow_texture2", "shadow_texture3",
					"shadow_texture4", "shadow_texture5", "shadow_texture6", "shadow_texture7"
		};
		for(int i=0;(i<MAX_LIGHT_NUMBER)&&(i<(sizeof(shadow_texture_name)/sizeof(shadow_texture_name[0])));i++)
			shader_program[program_index_id].shader_id[shader_type_id].variable_index.shadow_texture[i]=Graphics_system::get_uniform_index(program_id,shadow_texture_name[i]);

		if(display_error_message!=_T("")){
			static int error_sequence_id=0;
			CString str;
			str.Format(_T("temp\\compile_and_link_error_%d.txt"),++error_sequence_id);
			str=directory_file_name+str;
			ofstream f(str);
			File_data::output_string(f,display_error_message);
			f.close();
			::ShellExecute(NULL,_T("open"),str,NULL,NULL,SW_SHOWNORMAL);
		}
	}	
	if(variable_index!=NULL)
			*variable_index=&(shader_program[program_index_id].shader_id[shader_type_id].variable_index);
	return program_id;
}

CString *Shader_base::get_program_file_name()
{
	return &(shader_program[program_index_id].shader_file_name[0]);
}

void Shader_base::delete_all_program_object()
{
	for(int j=0;j<(sizeof(shader_type_string)/sizeof(shader_type_string[0]));j++){
		for(int i=0;i<shader_program_number;i++){
			if(shader_program[i].shader_id[j].program_id>0)
				Graphics_system::delete_program(shader_program[i].shader_id[j].program_id);				
			shader_program[i].shader_id[j].program_id=-1;

			for(int k=0;k<MAX_SHADER_NUMBER;k++)
				if(shader_program[i].shader_id[j].shader_id[k]>0){
					Graphics_system::delete_shader(shader_program[i].shader_id[j].shader_id[k]);			
					shader_program[i].shader_id[j].shader_id[k]=-1;
				}
		}
	}
}

Shader_base *Shader_base::system_clone_shader(Part_parameter &par)
{
	Shader_base *sp,*new_sp,*bak_first_shader_pointer;

	if(shader_program_number>=MAX_SHADER_PROGRAM_NUMBER)
		return NULL;
	for(sp=first_shader_pointer;sp!=NULL;sp=sp->next_shader)
		if(sp->class_name==par.class_name){
			bak_first_shader_pointer=first_shader_pointer;
			new_sp=sp->clone_shader(par.directory_name,par.material_file_name);
			new_sp->class_shader=sp;			
			new_sp->next_shader=NULL;
			first_shader_pointer=bak_first_shader_pointer;
		
			for(int i=0;i<shader_program_number;i++){
					bool match_flag=true;
					for(int j=0;j<MAX_SHADER_NUMBER;j++){
						if(shader_program[i].shader_file_name[j]!=(par.shader_file_name[j])){
							match_flag=false;
							break;
						}
					}
					if(match_flag){
						new_sp->program_index_id=i;
						return new_sp;
					}
			}
			new_sp->program_index_id=shader_program_number;

			for(int i=0;i<MAX_SHADER_NUMBER;i++)
				shader_program[shader_program_number].shader_file_name[i]=par.shader_file_name[i];
			
			for(int i=0;i<(sizeof(shader_type_string)/sizeof(shader_type_string[0]));i++){
				for(int j=0;j<MAX_SHADER_NUMBER;j++)
					shader_program[shader_program_number].shader_id[i].shader_id[j]=-1;
				shader_program[shader_program_number].shader_id[i].program_id=-1;
			}
			shader_program_number++;
			return new_sp;
		}
	return NULL;
}

Shader_base::Shader_base(CString my_class_name,CString my_directory_name,CString my_file_name)
{
	class_name=my_class_name;
	directory_name=my_directory_name;
	file_name=my_file_name;

	program_index_id=-1;
	class_shader=this;
	next_shader=first_shader_pointer;
	first_shader_pointer=this;
}
Shader_base::~Shader_base()
{

}
