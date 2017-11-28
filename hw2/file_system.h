#include <iostream>
#include <cstring>

#include <sstream>
#include <fstream>

#include <algorithm>
#include <map>
#include <vector>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h> //struct sockaddr_in

#include <thread>
#include <unistd.h>

using namespace std;

class FileSystem {

public:

	FileSystem();
	~FileSystem();
	bool AddUser(string username, string groupname);
	bool CreateFile(string username, string filename, string rights);
	bool ReadFile(string filename);
	bool WriteFile(string filename);
	void UpdateFileRight();
	void ManageFile(string filename);

private:

	map<string, string> file_owner; // file -> owner
	map<string, string> file_rights; // file -> rights

	map<string, vector<string>> group_user; // group -> user
	map<string, map<string, string>> user_rights; // user -> (file -> rights)

};