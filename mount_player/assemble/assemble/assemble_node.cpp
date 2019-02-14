#include "assemble_node.h"

assemble_node::assemble_node(CComPtr<IComponent2>	my_pComponent)
{
	is_root_flag=false;

	pModelDoc=NULL;
	pComponent=my_pComponent;
	children_number=0;
	children=NULL;

	if(pComponent->IGetModelDoc(&pModelDoc)!=S_OK)
		pModelDoc=NULL;
	create_child();


	return;
}
assemble_node::assemble_node(CComPtr<ISldWorks> sw)
{
	long i,doctype;
	
	HRESULT res;
	CComPtr		<IAssemblyDoc>		pAssemblyDoc=NULL;
	CComPtr		<IConfiguration>	pConfiguration=NULL;	


	is_root_flag=true;

	pModelDoc=NULL;
	pComponent=NULL;
	children_number=0;
	children=NULL;

	res=sw->get_IActiveDoc2(&pModelDoc);
	if(res!=S_OK||pModelDoc==NULL){
		pModelDoc=NULL;
		::MessageBox(NULL,_T("取得活跃文档失败"),_T("取得活跃文档失败"),MB_OK);
		return;
	}

	res=pModelDoc->GetType(&doctype);
	if(res!=S_OK||doctype!=swDocASSEMBLY){
			pModelDoc.Release();
			pModelDoc=NULL;
			::MessageBox(NULL,_T("取得文档类型失败"),_T("取得文档类型失败"),MB_OK);
			return;
	}

	pModelDoc->ShowNamedView2(_T("等轴测"),7);
	pModelDoc->ViewZoomtofit2();
	pModelDoc->GraphicsRedraw2(); 

	res=pModelDoc->QueryInterface(IID_IAssemblyDoc, (LPVOID *)&pAssemblyDoc);
	if(res!=S_OK||pAssemblyDoc==NULL){
		pModelDoc.Release();
		pModelDoc=NULL;
		::MessageBox(NULL,_T("取得装配体失败"),_T("取得装配体失败"),MB_OK);
		return;
	}

	pAssemblyDoc->ResolveAllLightWeightComponents(FALSE,&i);
	pAssemblyDoc.Release();

	pModelDoc->Rebuild(swForceRebuildAll);
	pModelDoc->GraphicsRedraw2(); 

	res=pModelDoc->IGetActiveConfiguration(&pConfiguration);
	if((res!=S_OK)||(pConfiguration==NULL)){
		pModelDoc.Release();
		pModelDoc=NULL;
		::MessageBox(NULL,_T("取得配置失败"),_T("取得配置失败"),MB_OK);
		return;
	}
	res=pConfiguration->IGetRootComponent2(&pComponent);
	if((res!=S_OK)||(pComponent==NULL)){
		pModelDoc.Release();		
		pModelDoc=NULL;
		pComponent=NULL;
	
		pConfiguration.Release();
		::MessageBox(NULL,_T("取得根组件失败"),_T("取得根组件失败"),MB_OK);
		return;
	}
	pConfiguration.Release();

	create_child();

	return;
}
assemble_node::~assemble_node(void)
{
	int i;

	if(pTransForm!=NULL){
		pTransForm.Release();
		pTransForm=NULL;
	}

	if(children_number>0){
		for(i=0;i<children_number;i++)
			delete (children[i]);
		delete []children;
	}
	children=NULL;
	children_number=0;

	if(pModelDoc!=NULL){
		if(is_root_flag){
			pModelDoc->Rebuild(swForceRebuildAll);
			pModelDoc->Save();
			pModelDoc->GraphicsRedraw2(); 
		}
		pModelDoc.Release();
		pModelDoc=NULL;
	}
}
void assemble_node::create_child()
{
	int i;
	long component_type=swDocASSEMBLY;
	CComPtr<IComponent2> *component;

	if(pComponent->get_Transform2(&pTransForm)!=S_OK)
		pTransForm=NULL;
	get_information();
	
	if(children_number>0){
		for(i=0;i<children_number;i++)
			delete (children[i]);
		delete []children;
		
	}
	children=NULL;
	children_number=0;

	if(pModelDoc!=NULL)
		if(pModelDoc->GetType(&component_type)!=S_OK)
			component_type=swDocPART;

	if(component_type!=swDocASSEMBLY)
		return;
	if(pComponent->IGetChildrenCount(&children_number)!=S_OK)
		children_number=0;
	if(children_number<=0)
		return;

	component=new CComPtr <IComponent2> [children_number];
	if(pComponent->IGetChildren(&(component[0]))!=S_OK)		
		children_number=0;
	else{
		children=new assemble_node *[children_number];
		for(i=0;i<children_number;i++){
			children[i]=new assemble_node(component[i]);
			for(int j=i;j>0;j--){
				assemble_node *p=children[j-1],*q=children[j];
				if((p->component_name)<=(q->component_name))
					break;
				children[j-1]=q;
				children[j]=p;
			}
		}
	}
	delete []component;
	return;
}
void assemble_node::get_information(void)
{
	int i;
	double tmp[16]={1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0};
	BSTR str;

	 if(pTransForm!=NULL)
		 if(pTransForm->get_IArrayData(location)==S_OK)
			for(i=0;i<16;i++)
				tmp[i]=location[i];

	for(i=0;i<16;i++)
		location[i]=tmp[i];

	component_name=(pComponent->get_Name2(&str)!=S_OK)?_T("没有组件名称"):str;
	component_name=get_rid_of_space(component_name);

	file_name=(pComponent->GetPathName(&str)!=S_OK)?_T("没有类型名称"):str;
	type_name=get_rid_of_space(file_name);

	return;
}

void assemble_node::output_data(ofstream * f,int cut_length,Location *parent_location,int space_number)
{
	int i,map[16]={0,1,2,13,3,4,5,14,6,7,8,15,9,10,11,12};
	double data[16],output_data[16];
	Location  loc;
	CString str;

	(*f)<<"\n";
	output_space(f,space_number);	
	output_string(f,_T("名称         ")+component_name);
	output_space(f,space_number);
	str=type_name.Right(type_name.GetLength()-cut_length);
	if(str.IsEmpty())
		str=_T("默认类型");
	output_string(f,_T("类型         ")+str);
	output_space(f,space_number);
	output_string(f,_T("位置         "));

	for(i=0;i<16;i++){
		data[i]=location[map[i]];
		output_data[i]=data[i];
	}

//###########################################################################################################################################################
//	*f>>location[ 0]0;	*f>>location[ 1]1;	*f>>location[ 2]2;
//	*f>>location[ 4]3;	*f>>location[ 5]4;	*f>>location[ 6]5;
//	*f>>location[ 8]6;	*f>>location[ 9]7;	*f>>location[10]8;
//
//	*f>>location[12]9;	*f>>location[13]10;	*f>>location[14]11;
//	*f>>location[15]12;

//	*f>>location[ 3]13;	*f>>location[ 7]14;	*f>>location[11]15;


//###########################################################################################################################################################

	if(parent_location!=NULL){
		loc=(*parent_location);
		loc=(-loc);
	}else{
		loc.load_identity_matrix();
	}
	loc=loc*Location(data);

	loc.get_data(output_data);

	for(i=0;i<16;i++)
		(*f)<<(output_data[i])<<"    ";


	output_space(f,space_number);
	output_string(f,_T("子节点数     "));
	(*f)<<children_number;

	
	for(i=0;i<children_number;i++){
		Location my_location(data);
		children[i]->output_data(f,cut_length,&my_location,8+space_number);
	}
	return;
}

CString assemble_node::get_directory_name()
{
	CString str=type_name;
	if(children_number<=0)
		return str;
	for(int i=0;i<children_number;i++)
		str=get_min_string(str,children[i]->get_directory_name());
	return str;
}

CString assemble_node::get_min_string(CString &str1 , CString &str2)
{
	int i,length1,length2;

	if((length1=str1.GetLength())<=0)
		return str2;
	if((length2=str2.GetLength())<=0)
		return str1;
	for(i=1;(i<length1)&&(i<length2);i++)
		if(str1.Left(i)!=str2.Left(i))
			return str1.Left(i-1);

	return (length1<length2)?str1:str2;
}
