#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string.h>
#include "stdlib.h"

using namespace std;

//从文件里读id列表
vector<string> test_readfile(const char* filename)
{
	cout << "opening file" << endl;
	vector<string> userlist;
	ifstream ifs(filename);
	if( !ifs.is_open())
	{
		cout << "Error opening file" << endl;
		return userlist;
	}
	string buffer;
	while (getline(ifs,buffer)){
		try{
			string userId=boost::lexical_cast<string>(buffer);
			cout << "read userId: " << userId << endl;
			userlist.push_back(userId);
		}catch(...){
			cout << "skipped bad ID: " << buffer << endl;
		}
	}
	ifs.close();
	return userlist;
}

