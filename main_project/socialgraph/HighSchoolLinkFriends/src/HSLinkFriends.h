#ifndef  _BASICREADER_H_
#define  _BASICREADER_H_
#include "QueryRunner.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <map>
#include <vector>
#include <string.h>
#include "stdlib.h"
#include <iconv.h>
#include <errno.h>
#include <IceUtil/IceUtil.h>
#include <ext/hash_map>

struct HSData
{
	int userid;
	int high_school_id;
	int h_class;
	int enroll_year;

	bool has_friends;
	std::string friend_list;
};

class HSLinkFriends
{
public:
	HSLinkFriends(string);
	virtual ~HSLinkFriends();
	void collectHSData();
private:
	std::map<int, std::map<int,HSData> > linkFriends(std::map<int, std::map<int,HSData> >& );
	void writeFile(std::map<int, std::map<int,HSData> >& );
private:
	bool _succ;
	std::vector<std::ofstream*> _ofss;
};

class HSDataByIDHandler : public com::xiaonei::xce::ResultHandler
{
public:
	HSDataByIDHandler(int& last_id, std::map<int, std::map<int,HSData> >& results):
		_last_id(last_id), _results(results){
	}
	virtual bool handle(mysqlpp::Row& row) const;
private:
	int& _last_id;
	std::map<int, std::map<int,HSData> >& _results;
};

class FriendsHandler : public com::xiaonei::xce::ResultHandler
{
public:
	FriendsHandler(std::map<int,std::string>& results):
		_results(results){
	}
	virtual bool handle(mysqlpp::Row& row) const;
private:
	std::map<int,std::string>& _results;
};

#endif
