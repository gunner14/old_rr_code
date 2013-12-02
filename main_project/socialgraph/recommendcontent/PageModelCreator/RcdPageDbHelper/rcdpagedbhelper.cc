#include "rcdpagedbhelper.h"
#include "util/cpp/IntSeqSerialize.h"
#include "socialgraph/socialgraphutil/clogging.h"

using namespace std;
using namespace com::xiaonei::xce;
using namespace xce::socialgraph;


void RcdPageDbHelper::getUserPageFromDb(int user_id, std::set<int>& page_ids) {
  com::xiaonei::xce::ConnectionPoolManager::instance().initialize();
  com::xiaonei::xce::Statement sql;
  std::string page_table("fans_pages_");
  std::string uid;
  std::string last_two_uid;
  try {
    uid = boost::lexical_cast<std::string>(user_id);
  } catch (boost::bad_lexical_cast& e) {
    CLOG(WARN) << "boost::lexical_cast error : " << e.what();
  }
  if (uid.size() > 2) {
    last_two_uid = uid.substr(uid.size() - 2, uid.size());
    int num = boost::lexical_cast<int>(last_two_uid);
    if (num < 10) {
      last_two_uid = last_two_uid.substr(last_two_uid.size() - 1, last_two_uid.size());
    }
  } else {
    return;
  }
  page_table += last_two_uid;
  CLOG(INFO) << "page_table is " << page_table << " last_two_uid " << last_two_uid;
  sql << "SELECT page_id FROM " << page_table << " WHERE user_id = " << user_id << " and state = 0 ";
  mysqlpp::StoreQueryResult result_page =QueryRunner("fans_pages", CDbRServer, page_table).store(sql);
  for(int i = 0 ; i < result_page.num_rows() ; i++) {
    mysqlpp::Row row = result_page.at(i);
    page_ids.insert((int)row["page_id"]);
  }
}
