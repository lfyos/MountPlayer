#include "StdAfx.h"
#include "Network.h"
#include "const.h"

void Network_base::reset()
{
	if(!fail_flag)
		::closesocket(socket_id);
	fail_flag=true;
	socket_id=-1;
}

Network_base::Network_base(int port_id,char *hostname)
{
 	struct hostent *host_info;
	
	fail_flag=true;

	if((socket_id=::socket(AF_INET,SOCK_STREAM,0))<0){
		socket_id=-1;
		return;
	}	
	if((host_info=::gethostbyname(hostname))==NULL){
		::closesocket(socket_id);
		socket_id=-1;
		return;
	}
	network_address.sin_family = AF_INET;
	network_address.sin_port = ::htons(port_id);
	network_address.sin_addr = *((struct in_addr *)host_info->h_addr);
	::memset(&(network_address.sin_zero),'\0',8);
	fail_flag=false;
};

Run_program_client::Run_program_client(int port_id,char *service_name)
	:Network_base(port_id,service_name)
{
	reply_length=0;
	reply_data=NULL;
	if(fail_flag)
		return;
	if(::connect(socket_id,(struct sockaddr *)(&network_address),sizeof(network_address))<0)
		reset();
}

Run_program_client::~Run_program_client()
{
	if(reply_data!=NULL){
		delete []reply_data;
		reply_data=NULL;
	}
	reply_length=0;
}

int Run_program_client::request_and_reply(char *data,int data_length)
{
	if(fail_flag||(data_length<=0)||(data==NULL))
		return -1;
	if(reply_data!=NULL){
		delete []reply_data;
		reply_data=NULL;
	}
	reply_length=0;
	
	if(::send(socket_id,data,data_length,0)!=data_length)
		return -1;

	char *data_buffer=new char[MAX_NETWORK_BUFFER_LENGTH];
	
	if((reply_length=::recv(socket_id,data_buffer,MAX_NETWORK_BUFFER_LENGTH,0))>0){
		reply_data=new char[reply_length+4];
		::memcpy(reply_data,data_buffer,reply_length);
		reply_data[reply_length+0]='\0';
		reply_data[reply_length+1]='\0';
		reply_data[reply_length+2]='\0';
	}
	delete []data_buffer;

	return reply_length;
}

Run_program_server::Run_program_server(int port_id,char *service_name)
	:Network_base(port_id,service_name)
{
	request_length=0;
	request_data=NULL;
	connect_socket_id=-1;
	if(!fail_flag)
		if(::bind(socket_id,(struct sockaddr *)(&network_address),sizeof(network_address))>=0)
			if(::listen(socket_id,5)>=0)
				return;
	reset();
}

Run_program_server::~Run_program_server()
{
	if(request_data!=NULL)
		delete []request_data;
	request_length=0;
	request_data=NULL;
	connect_socket_id=-1;
}

int Run_program_server::get_request()
{
	if(fail_flag)
		return -1;

	if(connect_socket_id>=0){
		::closesocket(connect_socket_id);
		connect_socket_id=-1;
	}
	if(request_data!=NULL)
		delete []request_data;
	request_length=0;

	if((connect_socket_id=::accept(socket_id,NULL,NULL))<0)
		return -1;

	char *data_buffer=new char[MAX_NETWORK_BUFFER_LENGTH];
	if((request_length=::recv(connect_socket_id,data_buffer,MAX_NETWORK_BUFFER_LENGTH,0))>0){
		request_data=new char[request_length+4];
		::memcpy(request_data,data_buffer,request_length);
		request_data[request_length+0]='\0';
		request_data[request_length+1]='\0';
		request_data[request_length+2]='\0';
	}
	delete []data_buffer;
	return request_length;
}

void Run_program_server::reply_result(char *data,int data_length)
{
	if(fail_flag)
		return ;
	if((connect_socket_id>=0)&&(socket_id>=0))
		::send(connect_socket_id,data,data_length,0);
	::closesocket(connect_socket_id);
	connect_socket_id=-1;
}
