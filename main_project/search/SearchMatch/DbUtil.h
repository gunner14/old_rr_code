/*
 * DbUtil.h
 *
 *  Created on: 2011-10-27
 *      Author: benjamin
 */

#ifndef DBUTIL_H_
#define DBUTIL_H_

#include <string>
#include <vector>
#include "DbCxxPool/src/QueryRunner.h"
#include <IceUtil/IceUtil.h>

struct Wiki
{
	int id;
	std::string name;
	int state;
};

class DbUtil
{
public:
	DbUtil();
	virtual ~DbUtil();
	std::vector<Wiki> getWikiListById(int host, int wiki);
};

class WikiResultHandler : public com::xiaonei::xce::ResultHandler
{
public:
	WikiResultHandler(std::vector<Wiki> &wiki_list) :
		wiki_list_(wiki_list)
	{
	}
	;
	virtual bool handle(mysqlpp::Row &row) const;
private:
	std::vector<Wiki> &wiki_list_;
};

#endif /* DBUTIL_H_ */
