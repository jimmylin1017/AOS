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

	lock_guard<mutex> m_locks(add_user_mutex);

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

	lock_guard<mutex> m_locks(create_file_mutex);

	// file exist then fail to create file
	/*ifstream ifile(filename.c_str());
  	
  	if(ifile) {

  		ifile.close();
  		return false;
  	}*/

	if(file_owner.find(filename) != file_owner.end())
	{
		return false;
	}

  	// add file owner information
  	file_owner.insert(pair<string, string>(filename, username));
  	AddFileRight(filename, rights);

  	vector<string> rv;
  	vector<string> wv;
  	file_is_read.insert(pair<string, vector<string>>(filename, rv));
  	file_is_write.insert(pair<string, vector<string>>(filename, wv));

  	return true;

	// for new process for make new file
	/*pid_t pid = fork();

	if(pid == 0) {

		char op[] = {"touch"};
		char *name = new char[filename.size() + 1];
		strcpy(name, filename.c_str());

		char* const my_argv[] = {op, name, 0};
		execvp(my_argv[0], my_argv);
	}
	else if(pid > 0) {
		return true;
	}*/
}

bool FileSystem::ModifyFile(string username, string filename, string rights) {

	lock_guard<mutex> m_locks(modify_file_mutex);

	// file exist then fail to create file
	/*ifstream ifile(filename.c_str());
  	
  	if(!ifile) {

  		ifile.close();
  		return false;
  	}*/

  	if(!(file_owner.find(filename) != file_owner.end()))
	{
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

		// already reading the file
		if(find(file_is_read[filename].begin(), file_is_read[filename].end(), username) != file_is_read[filename].end()) {

			return false;
		}

		// reading the file
		file_is_read[filename].push_back(username);
		return true;
	}

	return false;
}

bool FileSystem::WriteFile(string username, string filename) {

	if(user_rights[username][filename].write_right) {

		// already writing the file
		if(find(file_is_write[filename].begin(), file_is_write[filename].end(), username) != file_is_write[filename].end()) {

			return false;
		}

		// writing the file
		file_is_write[filename].push_back(username);
		return true;
	}

	return false;
}

bool FileSystem::CloseFile(string username, string filename) {

	vector<string>::iterator it;

	it = find(file_is_read[filename].begin(), file_is_read[filename].end(), username);

	if(it != file_is_read[filename].end()) {
		file_is_read[filename].erase(it);
		return true;
	}

	it = find(file_is_write[filename].begin(), file_is_write[filename].end(), username);

	if(it != file_is_write[filename].end()) {
		file_is_write[filename].erase(it);
		return true;
	}

	return false;
}

void FileSystem::UpdateFileUserRight() {

	lock_guard<mutex> m_locks(update_file_user_right_mutex);

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

			// check file is read
			if(!file_is_read[filename].empty()) {
				fur.write_right = false;
			}

			// check file is write
			if(!file_is_write[filename].empty()) {
				fur.write_right = false;
				fur.read_right = false;
			}

			if (user_rights.find(username) != user_rights.end()) {
				// find user in map
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

			// check file is read
			if(!file_is_read[filename].empty()) {
				fur.write_right = false;
			}

			// check file is write
			if(!file_is_write[filename].empty()) {
				fur.write_right = false;
				fur.read_right = false;
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
	map<string, vector<string>>::iterator itv;

	vector<string> aos_user = group_user[GROUP_AOS];
	vector<string> cse_user = group_user[GROUP_CSE];

	cout<<"\n==========\nGroup AOS\n=========="<<endl;
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

	cout<<"\n==========\nGroup CSE\n=========="<<endl;
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

	cout<<"\n==================\nFile Reading List\n=================="<<endl;
	for(itv = file_is_read.begin(); itv != file_is_read.end(); itv++) {

		string filename = itv->first;
		vector<string> rv = itv->second;

		cout<<filename<<": ";

		for(int i=0; i<rv.size(); i++)
		{
			cout<<rv[i]<<" ";
		}

		cout<<endl;
	}

	cout<<"\n==================\nFile Writing User\n=================="<<endl;
	for(itv = file_is_write.begin(); itv != file_is_write.end(); itv++) {

		string filename = itv->first;
		vector<string> rv = itv->second;

		cout<<filename<<": ";

		if(rv.size()) {
			cout<<rv[0]<<" ";
		}

		cout<<endl;
	}

}