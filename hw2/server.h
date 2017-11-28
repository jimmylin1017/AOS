#include "file_system.h"

class ClientHandler {

public:

	ClientHandler(int client_socket, struct sockaddr_in client_addr);
	~ClientHandler();
	void WriteString(string message);
	string ReadString();
	void CheckUser(string command);
	void ExecuteCommand(string command);

private:

	int client_socket;
	struct sockaddr_in client_addr;

	string username;
	string groupname;

	static FileSystem file_manager;
};



class Server {

public:

	Server(string server_ip, int server_port);
	~Server();
	bool ServerCreate();
	ClientHandler ServerListen();
	/*void WriteString(string message);
	string ReadString();
	void CheckUser(string command);
	void ExecuteCommand(string command);*/

private:

	string server_ip;
	int server_port;

	int server_socket;
	struct sockaddr_in server_addr;

	int client_socket;
	struct sockaddr_in client_addr;

	//FileSystem file_manager;
};