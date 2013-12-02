#include "BlockRelationAdapter.h"
#include "LogWrapper.h"
#include <boost/lexical_cast.hpp>

using namespace xce::blockrelation::adapter;
using namespace com::xiaonei::xce;

//int main(int argc, char* argv[]) {
//
//	if (argc < 4) {
//		std::cout << "Usage: " << argv[0] << " userId begin limit" << std::endl;
//		exit(-1);
//	}
//
//	int userId = boost::lexical_cast<int>(argv[1]);
//	int begin = boost::lexical_cast<int>(argv[2]);
//	int limit = boost::lexical_cast<int>(argv[3]);
//
//	MyUtil::IntSeq blockerList =
//			BlockRelationAdapter::instance().getBlockerList(userId, begin,
//					limit);
//
//	std::cout << userId << "'s blocker list ----------------" << std::endl;
//
//	for (MyUtil::IntSeq::iterator it = blockerList.begin(); it
//			!= blockerList.end(); ++it) {
//		std::cout << *it << std::endl;
//	}
//
//	return 0;
//}

MyUtil::IntSeq BlockRelationAdapter::getBlockerList(int userId, int begin,
		int limit) {
	MyUtil::IntSeq blockerList;

	if (begin < 0 || limit < 0) {
		MCE_ERROR(
				"BlockRelationAdapter::getBlockerList Invalid parameters. begin:"
						<< begin << " limit:" << limit);
		return blockerList;
	}

	const std::string tableName = "relation_block";
	Statement sql;
	sql << "SELECT host" << " FROM " << tableName << " WHERE guest = "
			<< userId << " ORDER BY host ASC LIMIT " << begin << "," << limit;

	std::cout << "SELECT host" << " FROM " << tableName << " WHERE guest = "
				<< userId << " ORDER BY host ASC LIMIT " << begin << "," << limit << std::endl;


	ostringstream pattern;
	pattern << tableName;
	QueryRunner(tableName, CDbRServer, pattern.str()).query(sql,
			IdListResultHandler(blockerList, "host"));

	return blockerList;
}

bool IdListResultHandler::handle(mysqlpp::Row& res) const {
	_data.push_back((int) res[_field.c_str()]);
	return true;
}

