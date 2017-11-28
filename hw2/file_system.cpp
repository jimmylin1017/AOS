#include "file_system.h"

FileSystem::FileSystem() {

	// group AOS-students & CSE-students
	vector<string> group_aos, group_cse;

	group_user.insert(pair<string, vector<string>>("aos", group_aos));
	group_user.insert(pair<string, vector<string>>("cse", group_cse));

}

FileSystem::~FileSystem() {

}

bool FileSystem::AddUser(string username, string groupname) {

	if(groupname == "aos" || groupname == "cse") {

		if(find(group_user[groupname].begin(), group_user[groupname].end(), username) != group_user[groupname].end()) {

			group_user[groupname].push_back(username);

			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

bool FileSystem::CreateFile(string username, string filename, string rights) {

	// file exist then fail to create file
	ifstream ifile(filename.c_str());
  	
  	if(ifile) {

  		ifile.close();
  		return false;
  	}

  	// add file information
  	file_owner.insert(pair<string, string>(filename, username));
  	file_owner.insert(pair<string, string>(filename, username));

	// for new process for make new file
	pid_t pid = fork();

	if(pid == 0) {

		char op[] = {"touch"};
		char *name = new char[filename.size() + 1];
		strcpy(name, filename.c_str());

		char* const my_argv[] = {op, name, 0};
		execvp(my_argv[0], my_argv);
	}
	else if(pid > 0) {
		return true;
	}
}

bool FileSystem::ReadFile(string filename) {

}



void FileSystem::UpdateFileRight() {


}