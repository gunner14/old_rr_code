#include "DataProviderDB.h"
#include "QueryRunner.h"
#include "LogWrapper.h"

using namespace std;
using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace xce::dataprovider;

bool DataProviderDB::GetHotShare(Ice::Long &max_db_id,  std::vector<HotShareInfo> &hotshare_vector, size_t size) {
  HotShareInfo hs;
  Ice::Long temp_max_db_id = max_db_id;
  Statement sql;  
  sql << "SELECT id, share_id, user_id,  title, url, summary, creation_date, release_date, user_name, thumb_url, catagory FROM "
    << "hotshare" <<" WHERE id>" << (max_db_id+1) <<" limit " << size;
  try {
    mysqlpp::StoreQueryResult res = QueryRunner("im", CDbRServer, "hotshare").store(sql);
    for (size_t i=0; i<res.num_rows(); i++) {
      mysqlpp::Row row = res.at(i);
      hs.id = static_cast<Ice::Long>(row["id"]) - 1;
      hs.share_id = static_cast<Ice::Long>(row["share_id"]);
      hs.user_id = static_cast<size_t>(row["user_id"]);
      row["title"].to_string(hs.title);
      row["url"].to_string(hs.url);
      row["summary"].to_string(hs.summary);
      row["creation_date"].to_string(hs.creation_date);
      row["release_date"].to_string(hs.release_date);
      row["user_name"].to_string(hs.user_name);
      row["thumb_url"].to_string(hs.thumb_url);
      row["catagory"].to_string(hs.catagory);

      hotshare_vector.push_back(hs);
      if (hs.id > temp_max_db_id)
        temp_max_db_id = hs.id;
     }
    max_db_id = temp_max_db_id;
  }
  catch (std::exception &e) {
    MCE_WARN("DataProviderDB::GetHotShare: "<< e.what());
    return false;
  }
  catch (...) {
    MCE_WARN("DataProviderDB::GetHotShare: exception");
    return false;
  }  
  return true;
}

bool DataProviderDB::GetOnlineVideo(std::vector<OnlineVideoProgramDBInfo> &online_video_program_db_info, const std::string &program_id) {
  string sort_column_name;
  if (program_id.substr(0,4) == "0002")
    sort_column_name = "release_date";
  else if (program_id.substr(0,4) == "0003")
    sort_column_name = "episode_number";
  else
    return false;

  Statement sql;  
  sql << "SELECT title, thumb_url, swf_url, page_url, creation_date, release_date FROM " << "hotshare_video_test " 
    << "WHERE catagory='" <<program_id << "' ORDER BY " << sort_column_name << " DESC";
  try {
    mysqlpp::StoreQueryResult res = QueryRunner("im", CDbRServer, "hotshare_video_test").store(sql);
    for (size_t i=0; i<res.num_rows(); i++) {
      mysqlpp::Row row = res.at(i);

      OnlineVideoProgramDBInfo info;
      row["title"].to_string(info.program_name);
      row["page_url"].to_string(info.page_url);
      row["swf_url"].to_string(info.swf_url);
      row["release_date"].to_string(info.release_date);
      row["creation_date"].to_string(info.creation_date);
      row["thumb_url"].to_string(info.thumb_url);
      info.catagory = program_id;
      online_video_program_db_info.push_back(info);
     }
  }
  catch (std::exception &e) {
    MCE_WARN("DataProviderDB::GetOnlineVideo: "<< e.what());
    return false;
  }
  catch (...) {
    MCE_WARN("DataProviderDB::GetOnlineVideo: exception");
    return false;
  }  
  return true;
}
