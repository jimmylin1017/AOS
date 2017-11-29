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

struct FileRight {

	bool owner_read;
	bool owner_write;
	bool group_read;
	bool group_write;
	bool other_read;
	bool other_write;
};

struct FileUserRight {

	bool read_right;
	bool write_right;
};

class FileSystem {

public:

	FileSystem();
	~FileSystem();
	bool AddUser(string username, string groupname);
	void AddFileRight(string filename, string rights);
	bool CreateFile(string username, string filename, string rights);
	bool ReadFile(string username, string filename);
	bool WriteFile(string username, string filename);
	bool ModifyFile(string username, string filename, string rights);
	void UpdateFileUserRight();
	void PrintFileUserRight();
	void ManageFile(string filename);

private:

	map<string, string> file_owner; // file -> owner
	map<string, FileRight> file_rights; // file -> rights

	map<string, vector<string>> group_user; // group -> user
	map<string, map<string, FileUserRight>> user_rights; // user -> (file -> rights)

};