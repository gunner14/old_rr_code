#ifndef _ACCESS_H
#define _ACCESS_H

#include <vector>
#include <map>
#include <string>
#include <fstream>

using namespace std;

typedef enum AccessType{
	ADDFRIEND=0,
	FORBID,
	INFO,
	SHOW
};

inline int GetAccessType(const char* typestr){
	if( strcmp(typestr, "addFriend")==0 )return 0;
	else if( strcmp(typestr, "forbid")==0 )return 1;
	else if( strcmp(typestr, "info")==0 )return 2;
	else if( strcmp(typestr, "show")==0 )return 3;
	else return -1;
}

class UserLog{
	public:
		int uid;
		map<int, int>	userAccess;
		virtual ~UserLog(){userAccess.clear();}
};

class Access:public vector<UserLog>{
	public:
			Access(const char* logfile);
		virtual	~Access(){this->clear();}
	private:
		void	_load(const char* logfile);
};

#endif
