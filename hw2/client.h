#include <iostream>
#include <cstring>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h> //struct sockaddr_in
#include <unistd.h>

using namespace std;

class Client {

public:

	Client(string server_ip, int server_port);
	~Client();
	bool ClientCreate();
	bool ClientConnect();
	void WriteString(string message);
	string ReadString();

private:

	string server_ip;
	int server_port;

	int server_socket;
	struct sockaddr_in server_addr;

	int client_socket;
	struct sockaddr_in client_addr;

};