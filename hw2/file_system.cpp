#include "file_system.h"

#define GROUP_AOS "aos"
#define GROUP_CSE "cse"

FileSystem::FileSystem() {

	// group AOS-students & CSE-students
	vector<string> group_aos, group_cse;

	group_user.insert(pair<string, vector<string>>(GROUP_AOS, group_aos));
	group_user.insert(pair<string, vector<string>>(GROUP_CSE, group_cse));

}

FileSystem::~FileSystem() {

}

bool FileSystem::AddUser(string username, string groupname) {

	if(groupname == GROUP_AOS || groupname == GROUP_CSE) {

		if(find(group_user[groupname].begin(), group_user[groupname].end(), username) != group_user[groupname].end()) {

			return false;
		}
		else {

			group_user[groupname].push_back(username);

			return true;
		}
	}
	else {
		return false;
	}
}

void FileSystem::AddFileRight(string filename, string rights) {

	FileRight fr;

  	if(rights[0] != '-'){
  		fr.owner_read = true;
  	}
  	else {
  		fr.owner_read = false;
  	}

  	if(rights[1] != '-'){
  		fr.owner_write = true;
  	}
  	else {
  		fr.owner_write = false;
  	}

  	if(rights[2] != '-'){
  		fr.group_read = true;
  	}
  	else {
  		fr.group_read = false;
  	}

  	if(rights[3] != '-'){
  		fr.group_write = true;
  	}
  	else {
  		fr.group_write = false;
  	}

  	if(rights[4] != '-'){
  		fr.other_read = true;
  	}
  	else {
  		fr.other_read = false;
  	}

  	if(rights[5] != '-'){
  		fr.other_write = true;
  	}
  	else {
  		fr.other_write = false;
  	}

  	file_rights.insert(pair<string, FileRight>(filename, fr));
}

bool FileSystem::CreateFile(string username, string filename, string rights) {

	// file exist then fail to create file
	ifstream ifile(filename.c_str());
  	
  	if(ifile) {

  		ifile.close();
  		return false;
  	}

  	// add file owner information
  	file_owner.insert(pair<string, string>(filename, username));
  	AddFileRight(filename, rights);

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

bool FileSystem::ModifyFile(string username, string filename, string rights) {

	// file exist then fail to create file
	ifstream ifile(filename.c_str());
  	
  	if(!ifile) {

  		ifile.close();
  		return false;
  	}

  	if(file_owner.find(filename) != file_owner.end())
  	{
  		if(username == file_owner[filename])
  		{
  			file_rights.erase(filename);
  			AddFileRight(filename, rights);

  			return true;
  		}
  	}

  	return false;
}

bool FileSystem::ReadFile(string username, string filename) {

	if(user_rights[username][filename].read_right) {
		return true;
	}

	return false;
}

bool FileSystem::WriteFile(string username, string filename) {

	if(user_rights[username][filename].write_right) {
		return true;
	}

	return false;
}

void FileSystem::UpdateFileUserRight() {

	map<string, string>::iterator it;

	vector<string> aos_user = group_user[GROUP_AOS];
	vector<string> cse_user = group_user[GROUP_CSE];

	// file loop
	for(it = file_owner.begin(); it != file_owner.end(); it++) {

		string filename = it->first;
		string ownername = it->second;
		string ownergroup;

		if(find(group_user[GROUP_AOS].begin(), group_user[GROUP_AOS].end(), ownername) != group_user[GROUP_AOS].end()) {
			ownergroup = GROUP_AOS;
		}
		else {
			ownergroup = GROUP_CSE;
		}

		// user in group aos loop
		for(int i=0; i<aos_user.size(); i++) {

			string username = aos_user[i];
			string usergroup = GROUP_AOS;

			// create file user right for user_rights list
			FileUserRight fur;

			if(username == ownername) {
				// owner
				fur.read_right = file_rights[filename].owner_read;
				fur.write_right = file_rights[filename].owner_write;
			}
			else if(usergroup == ownergroup) {
				// same group
				fur.read_right = file_rights[filename].group_read;
				fur.write_right = file_rights[filename].group_write;
			}
			else {
				// other
				fur.read_right = file_rights[filename].other_read;
				fur.write_right = file_rights[filename].other_write;
			}

			if (user_rights.find(username) != user_rights.end()) {
				// find user in map
				/*if (user_rights[username].find(filename) != user_rights[username].end()) {
					// find file in map
					user_rights[username][filename] = fur;
				}
				else {
					// not find file in map
					user_rights[username].insert(pair<string, FileUserRight>(filename, fur));
				}*/
				user_rights[username][filename] = fur;
			}
			else {
				// not find user in map
				//user_rights.insert(pair<string, pair<string, FileUserRight>>(username, pair<string, FileUserRight>(filename, fur)));
				user_rights[username][filename] = fur;
			}
		}

		// user in group cse loop
		for(int i=0; i<cse_user.size(); i++) {

			string username = cse_user[i];
			string usergroup = GROUP_CSE;

			// create file user right for user_rights list
			FileUserRight fur;

			if(username == ownername) {
				// owner
				fur.read_right = file_rights[filename].owner_read;
				fur.write_right = file_rights[filename].owner_write;
			}
			else if(usergroup == ownergroup) {
				// same group
				fur.read_right = file_rights[filename].group_read;
				fur.write_right = file_rights[filename].group_write;
			}
			else {
				// other
				fur.read_right = file_rights[filename].other_read;
				fur.write_right = file_rights[filename].other_write;
			}

			if (user_rights.find(username) != user_rights.end()) {
				// find user in map
				user_rights[username][filename] = fur;
			}
			else {
				// not find user in map
				user_rights[username][filename] = fur;
			}
		}
	}

	PrintFileUserRight();
}

void FileSystem::PrintFileUserRight() {

	map<string, FileUserRight>::iterator it;

	vector<string> aos_user = group_user[GROUP_AOS];
	vector<string> cse_user = group_user[GROUP_CSE];

	cout<<"\nGroup AOS\n"<<endl;
	for(int i=0; i<aos_user.size(); i++) {

		string username = aos_user[i];

		cout<<username<<"\n=========="<<endl;

		for(it = user_rights[username].begin(); it != user_rights[username].end(); it++) {

			string filename = it->first;
			FileUserRight fur = it->second;

			cout<<filename<<": "<<fur.read_right<<" "<<fur.write_right<<endl;
		}

		cout<<endl;
	}

	cout<<"\nGroup CSE\n"<<endl;
	for(int i=0; i<cse_user.size(); i++) {

		string username = cse_user[i];

		cout<<username<<"\n=========="<<endl;

		for(it = user_rights[username].begin(); it != user_rights[username].end(); it++) {

			string filename = it->first;
			FileUserRight fur = it->second;

			cout<<filename<<": "<<fur.read_right<<" "<<fur.write_right<<endl;
		}

		cout<<endl;
	}

}