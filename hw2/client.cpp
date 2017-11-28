#include "client.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 3500

Client::Client(string server_ip, int server_port) {

	this->server_ip = server_ip;
	this->server_port = server_port;
}

Client::~Client() {

	close(server_socket);
}

bool Client::ClientCreate() {

	// create socket fd
	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	// socket fd failed
	if (server_socket == -1) {

		perror("socket fd failed: ");
		return false;
	}

	cout<<"Client Create Success"<<endl;

	return true;
}

bool Client::ClientConnect() {

	// connect to server
    memset(&server_addr, 0, sizeof(server_addr)); // full of zero
    server_addr.sin_family = PF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());
    server_addr.sin_port = htons(server_port);

    int check = connect(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    if(check == -1) {

    	perror("connect failed: ");
		return false;
    }

    cout<<"Server Connect Success"<<endl;

    return true;
}

void Client::WriteString(string message) {

	send(server_socket, message.c_str(), message.length(), 0);

	cout<<"Send: "<<message<<endl;
}

string Client::ReadString() {

	char message[100] = {};

	recv(server_socket, message, sizeof(message), 0);

	return message;
}



int main()
{

	string server_ip = SERVER_IP;
	int server_port = SERVER_PORT;
	string message = "";

	Client cl(server_ip, server_port);
	cl.ClientCreate();
	cl.ClientConnect();

	getline(cin, message);
	cl.WriteString(message);
	message = cl.ReadString();
	cout<<"Receive: "<<message<<endl;

	cl.WriteString("create homework2.c rwr---");
	message = cl.ReadString();
	cout<<"Receive: "<<message<<endl;

	cin>>message;

	return 0;
}