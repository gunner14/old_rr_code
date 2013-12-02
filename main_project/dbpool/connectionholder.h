#ifndef __ConnectionHolder_h__
#define __ConnectionHolder_h__

#include <iostream>
#include <mysql++/mysql++.h>
#include <mysql++/ssqls.h>
#include "dbpool/connectionpool.h"
#include "dbpool/connectionpoolmanager.h"

namespace com {
namespace xiaonei {
namespace xce {
using namespace std;

class ConnectionHolder {
public:
	ConnectionHolder(const string& name, int wrflag = CDbRServer,
			const string& pattern = " ");
	virtual ~ConnectionHolder();

	mysqlpp::Query query() {
		return _entry->query();
	}

	int errnum() {
		return _entry->errnum();
	}
	const char* error() {
		return _entry->error();
	}

private:
	string _instance;
	string _pattern;
	ConnectionEntryPtr _entry;
};

}
;
}
;
}
;

#endif
