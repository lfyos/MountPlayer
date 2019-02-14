#include "StdAfx.h"
#include "Whole_system.h"

void Whole_system::init_part(ifstream &f_part_file,CString merge_file_name)
{
	Shader_base *shader_pointer;
	for(system_max_user_parameter_number=1;;){
		Part_parameter par(plat_info->directory_name,f_part_file);
		if((f_part_file.eof())||(f_part_file.fail()))
			break;
		while(par.load())
			if(File_data::get_file_length((plat_info->directory_name)+(par.mesh_file_name))>=0)
				if((shader_pointer=parts->add_part(par))!=NULL){
					Shader_part_information *info=shader_pointer->get_part_information();
					if((info->max_user_parameter_number)>system_max_user_parameter_number)
						system_max_user_parameter_number=info->max_user_parameter_number;
				}
	}
	parts->merge_same_part(plat_info->directory_name+merge_file_name);
}
void Whole_system::init_component(ifstream &f)
{
	char buffer[MAX_BUFFER_LENGTH];
	Component *p;
	int display_flag;

	f>>buffer;	
	for(root_component=NULL;;){
		CString file_name;
		f>>buffer>>display_flag;
		file_name=plat_info->directory_name+CString(buffer);
		if((f.eof())||(f.fail()))
			break;
		p=new Component	(ifstream(file_name),~display_flag,parts,root_component);
		if(root_component==NULL)
			root_component=p;
	}
	root_component->compute_absulate_location();
	root_component->get_component_box(*parts);
	root_component->selected_time=0;
}
void Whole_system::init_camera()
{
	int i;
	char buffer[MAX_BUFFER_LENGTH];
	ifstream f_camera(plat_info->directory_name+camera_file_name);

	f_camera>>buffer>>selected_line_width;
	f_camera>>buffer>>selected_point_size;
	f_camera>>buffer>>minnimal_fovy_tanl;
	if(minnimal_fovy_tanl<0.0)
		minnimal_fovy_tanl=0.0;
	
	f_camera>>buffer>>rotate_scale>>buffer;
	background_color=Point(f_camera);

	f_camera>>buffer>>line_stipple_factor>>line_stipple_pattern;

	for(i=0;i<MAX_CAMERA_NUMBER;i++){
		double fovy,near_value_ratio,far_value_ratio,display_precision;
		CString attatch_name;
		bool my_locate_mount_flag,my_expand_mount_flag,my_lock_camera_flag,my_rotate_view_flag;
		int view_id;

		f_camera>>buffer>>buffer;
		attatch_name=buffer;
		f_camera>>buffer>>fovy;
		f_camera>>buffer>>near_value_ratio;
		f_camera>>buffer>>far_value_ratio;
		f_camera>>buffer>>display_precision;
		f_camera>>buffer>>buffer;	my_locate_mount_flag=((CString(buffer)==_T("YES"))||(CString(buffer)==_T("yes")))?true:false;
		f_camera>>buffer>>buffer;	my_expand_mount_flag=((CString(buffer)==_T("YES"))||(CString(buffer)==_T("yes")))?true:false;
		f_camera>>buffer>>buffer;	my_lock_camera_flag=((CString(buffer)==_T("YES"))||(CString(buffer)==_T("yes")))?true:false;
		f_camera>>buffer>>buffer;	my_rotate_view_flag=((CString(buffer)==_T("YES"))||(CString(buffer)==_T("yes")))?true:false;
		f_camera>>buffer>>view_id;
		

		if((f_camera.eof())||(f_camera.fail())){
			for(;i<MAX_CAMERA_NUMBER;i++)
				camera[i]=NULL;
			break;
		}
		camera[i]=new Camera(root_component,*parts,attatch_name,fovy,near_value_ratio,far_value_ratio,display_precision,my_locate_mount_flag,my_expand_mount_flag,my_lock_camera_flag,my_rotate_view_flag,view_id);
	}
	f_camera.close();
}

void Whole_system::save_camera()
{
	ofstream f_camera(plat_info->directory_name+camera_file_name+_T(".bak"));
 
	f_camera<<    "选择线宽	"<<selected_line_width;
	f_camera<<  "\n选择点大小	"<<selected_point_size;
	f_camera<<"\n\n最小视角	"<<minnimal_fovy_tanl;	
	f_camera<<"\n\n旋转标尺	"<<rotate_scale;
	f_camera<<"\n\n背景颜色";
	f_camera<<"	"<<(background_color.x);
	f_camera<<"	"<<(background_color.y);
	f_camera<<"	"<<(background_color.z);

	f_camera<<"\n\n点划线参数	"<<line_stipple_factor<<"	"<<line_stipple_pattern<<"\n\n";


	for(int i=0;i<MAX_CAMERA_NUMBER;i++)
		if(camera[i]!=NULL)
			camera[i]->save(f_camera);
	f_camera<<"\n";
	f_camera.close();
}

void Whole_system::reload_component(CString my_caption_name,CString part_list_file_name,CString configure_file_name)
{
	char buffer[MAX_BUFFER_LENGTH];

	Component_collector_base::clear_all();

	for(int i=0;i<MAX_CAMERA_NUMBER;i++)
		if(camera[i]!=NULL){
			delete (camera[i]);
			camera[i]=NULL;
		}
	if(lights!=NULL){
		delete lights;
		lights=NULL;
	}
	if(root_component!=NULL){
		delete root_component;
		root_component=NULL;
	}
	if(part_list_root_component!=NULL){
		delete part_list_root_component;
		part_list_root_component=NULL;
	}

	part_list_root_component=new Component(ifstream(plat_info->directory_name+part_list_file_name),~0,parts);

	CString light_file_name,mirror_file_name;
	ifstream f(plat_info->directory_name+configure_file_name);
	
	f>>buffer>>buffer;		camera_file_name=buffer;
	f>>buffer>>buffer;		light_file_name=buffer;
	init_component(f);
	f.close();

	init_camera();
	lights=new Light_set(plat_info->directory_name+light_file_name);

	allocate_component_model_maxtrix_memory();

	caption_name=my_caption_name;

	{
		ofstream f_component(plat_info->directory_name+_T("temp\\not_find_component.lfy"));
		ofstream f_part_simple(plat_info->directory_name+_T("temp\\part_list_simple.lfy"));
		ofstream f_part_complete(plat_info->directory_name+_T("temp\\part_list_complete.lfy"));

		list_not_find_component(root_component,f_component);
		list_not_find_component(part_list_root_component,f_component);
	
		parts->list_all_part(f_part_simple,f_part_complete);

		f_component.close();
		f_part_simple.close();
		f_part_complete.close();
	}
}

Whole_system::Whole_system(CString sub_directory_name,CString my_part_file_name,CString my_merge_file_name,CString my_configure_file_name,CString my_caption_file_name)
{
	plat_info=new Platform_information(sub_directory_name);
	parts=new Part_set;
	{
		ifstream f_part_file(plat_info->directory_name+my_part_file_name);
		{
			CString configure_shader_file_name,uniform_shader_file_name,vertex_shader_file_name;
			{
				char buffer[MAX_BUFFER_LENGTH];
				f_part_file>>buffer>>buffer;		configure_shader_file_name	=buffer;
				f_part_file>>buffer>>buffer;		uniform_shader_file_name	=buffer;
				f_part_file>>buffer>>buffer;		vertex_shader_file_name		=buffer;
				
				f_part_file>>buffer>>release_memory_flag_map;
				
				f_part_file>>buffer>>offset_factor>>offset_units;
				
			}
			Shader_base::set_general_shader_file_name_and_max_draw_instance_number(plat_info->directory_name,
				configure_shader_file_name,uniform_shader_file_name,vertex_shader_file_name);
		}
		init_part(f_part_file,my_merge_file_name);	
		f_part_file.close();
	}

	for(int i=0;i<MAX_CAMERA_NUMBER;i++)
		if(camera[i]!=NULL)			
			camera[i]=NULL;
	lights=NULL;
	part_list_root_component=NULL;
	root_component=NULL;

	assemble_type=new Assemble_type(plat_info->directory_name,my_configure_file_name);
	{
		caption_file_name=my_caption_file_name;
		ifstream f_caption_file(plat_info->directory_name+caption_file_name);
		char buffer[MAX_BUFFER_LENGTH];
		f_caption_file>>buffer;
		caption_name=buffer;
		f_caption_file.close();
	}
	if((current_assemble_type=assemble_type->get_assemble_type_by_caption_name(caption_name))==NULL)
		current_assemble_type=assemble_type->get_assemble_type_by_caption_name(_T(""));
	if(current_assemble_type!=NULL)
		reload_component(current_assemble_type->caption_name,current_assemble_type->part_name,current_assemble_type->assemble_name);
	return;
}

void Whole_system::allocate_component_model_maxtrix_memory()
{
	class temp:public Component_collector_base
	{
	public:
		int *counter,part_number,total_part_number;

		virtual bool operate_component(Component *comp,bool my_selected_flag)
		{
			total_part_number++;
			if(comp->child==NULL)
				if(comp->part_id>=0)
					if(comp->part_id<part_number)
						(counter[comp->part_id])++;			
			return true;
		}

		temp(Component *root_component,Part_set *parts)
		{
			part_number=parts->part_number;
			counter=new int[part_number];
			for(int i=0,*p=counter;i<part_number;i++,p++)
				*p=0;
			total_part_number=0;

			collect(root_component);

			for(int i=0;i<part_number;i++){
				for(Part_with_shader *ps=parts->part_array[i];ps!=NULL;ps=ps->next_part){
					for(int j=0;j<(sizeof(ps->paint_component_number)/sizeof(ps->paint_component_number[0]));j++){
						if(ps->paint_component_pointer[j]!=NULL){
							delete [](ps->paint_component_pointer[j]);
							ps->paint_component_pointer[j]=NULL;
						}
						ps->paint_component_number[j]=0;
						if(counter[i]>0)
							ps->paint_component_pointer[j]=new Component*[counter[i]];
					}
				}
			}
			delete []counter;
		}
	};
	temp a(root_component,parts);
}
