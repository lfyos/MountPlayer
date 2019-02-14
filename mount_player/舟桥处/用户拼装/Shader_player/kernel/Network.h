

class Network_base
{
public:
	int socket_id;
	bool fail_flag;
	struct sockaddr_in network_address;

	void reset();

	Network_base(int port_id,char *hostname);
	~Network_base()
	{
		reset();
	};
};

class Run_program_client:public Network_base
{
public:
	int reply_length;
	char *reply_data;

	Run_program_client(int port_id,char *service_name);
	~Run_program_client();

	int request_and_reply(char *data,int data_length);
};

class Run_program_server:public Network_base
{
	int connect_socket_id;
public:
	int request_length;
	char *request_data;

	Run_program_server(int port_id,char *service_name);
	~Run_program_server();

	int get_request();
	void reply_result(char *data,int data_length);
};
