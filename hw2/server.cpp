#include "server.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 3500


Server::Server(string server_ip, int server_port) {

	this->server_ip = server_ip;
	this->server_port = server_port;
}

Server::~Server() {

	close(client_socket);
	close(server_socket);
}

bool Server::ServerCreate() {
	
	// create socket fd
	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	// socket fd failed
	if (server_socket == -1) {

		perror("socket fd failed: ");
		return false;
	}

	// bind socket fd with ip and port
	memset(&server_addr, 0, sizeof(server_addr)); // full of zero
	server_addr.sin_family = AF_INET; // use IPv4
	server_addr.sin_addr.s_addr = inet_addr(server_ip.c_str()); // ip address
	server_addr.sin_port = htons(server_port); // port number

	// to avoid socket on use
	int reuseaddr = 1;
	socklen_t reuseaddr_len = sizeof(reuseaddr);
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, reuseaddr_len);


	// socket bind failed
	int check = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if(check == -1) {

        perror("socket bind failed: ");
        return false;
    }

    cout<<"Server Create Success"<<endl;

	return true;
}

ClientHandler Server::ServerListen() {

	cout<<"Listening..."<<endl;

	// listen 20 client
	listen(server_socket, 20);

	// receive client information
    socklen_t client_addr_size = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);

    cout<<"Client Connect Success"<<endl;

    ClientHandler client_handler(client_socket, client_addr);

    return client_handler;
}

// =====================================================================
// ========================ClientHandler================================
// =====================================================================

FileSystem ClientHandler::file_manager;

ClientHandler::ClientHandler(int client_socket, struct sockaddr_in client_addr) {

	this->client_socket = client_socket;
	this->client_addr = client_addr;
}

ClientHandler::~ClientHandler() {

}

void ClientHandler::WriteString(string message) {

	send(client_socket, message.c_str(), message.length(), 0);

	cout<<"Send: "<<message<<endl;
}

string ClientHandler::ReadString() {

	char message[100] = {};

	while(recv(client_socket, message, sizeof(message), 0) <= 0);

	return message;
}

void ClientHandler::CheckUser(string command) {

	istringstream ss(command);

	ss>>username>>groupname;

	cout<<username<<"\n"<<groupname<<endl;

	bool check = file_manager.AddUser(username, groupname);

	if(check) {
		WriteString("User " + username + "(" + groupname + ") Success!");
	}
	else {
		WriteString("User " + username + "(" + groupname + ") Failed!");
	}
}

void ClientHandler::ExecuteCommand(string command) {

	istringstream ss(command);

	string operation;
	string filename;
	string rights;

	ss>>operation>>filename>>rights;

	cout<<operation<<"\n"<<filename<<"\n"<<rights<<endl;

	if(operation == "create") {

		bool check = file_manager.CreateFile(username, filename, rights);

		if(check) {
			WriteString("Create File (" + filename + ") Success!");
		}
		else {
			WriteString("Create File (" + filename + ") Failed!");
		}
	}
}

void client_handler_thread(ClientHandler client_handler)
{
	string message = "";
	message = client_handler.ReadString();
	client_handler.CheckUser(message);

	while(1)
	{
		message = client_handler.ReadString();

		cout<<"Reveive: "<<message<<endl;

		client_handler.ExecuteCommand(message);
	}
}

int main()
{

	string server_ip = SERVER_IP;
	int server_port = SERVER_PORT;

	Server sv(server_ip, server_port);

	if(!sv.ServerCreate()) exit(1);

	while(1)
	{
		ClientHandler client_handler = sv.ServerListen();

		thread (client_handler_thread, client_handler).detach();
	}

	return 0;
}
