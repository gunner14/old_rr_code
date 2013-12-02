#include "tripod2/cc/producermanager/like_producer.h"
#include "OceProto/proto/Like.pb.h"

namespace xce {
namespace tripod {

bool LikeProducer::Create(const std::string& key, ByteSeq* value, bool use_master) {
  using namespace std;
  using namespace com::xiaonei::xce;
  using namespace xce::tripodlike;
  using namespace xce::ilikeit;
  using namespace MyUtil;
  MCE_INFO("[LikeProducer:create] " << ", key = " << key);
  long gid = boost::lexical_cast<long>(key);
  int table_sufix = gid % TABLE_NUM;
  Statement sql;
  sql << "select " << FIELD_GlobalId << ", " << FIELD_UID << " from " << TABLE_LIKESHIP << boost::lexical_cast<string>(table_sufix)
      << " where " << FIELD_GlobalId << " = " << gid << " order by " << FIELD_GlobalId
      << ", " << FIELD_UID << " limit 1000";
  map<long, LikeDataIPtr> like_data_map;
  BatchLikeshipHandler handler(like_data_map);
  QueryRunner(DB_DESC_ILIKEIT, use_master?CDbWServer:CDbRServer).query(sql, handler);

  if ( like_data_map.size() == 1 ) {
    map<long, LikeDataIPtr>::const_iterator it = like_data_map.begin();
    string db_key = boost::lexical_cast<string>(it->first); 
    if ( db_key != key ) {
      MCE_WARN("[LikeProducer::create] " << ", key = " << key << " fail");
      return false;
    }
    boost::shared_ptr<PbLikeData> message(new PbLikeData);
    message->set_gid(gid);
    vector<int> userids(it->second->getUsers());
    vector<int>::const_iterator user_it = userids.begin();
    while( user_it != userids.end() ) {
      message->add_userid(*user_it++);
    }
    std::string str;
    message->SerializeToString(&str);
    (*value) = ByteSeq(str.begin(), str.end());
    return true;
  } else if ( like_data_map.size() == 0 ) {
    boost::shared_ptr<PbLikeData> message(new PbLikeData);
    message->set_gid(0);
    std::string str;
    message->SerializeToString(&str);
    (*value) = ByteSeq(str.begin(), str.end());
    return true;
  } else {
    MCE_WARN("[LikeProducer::create] " << ", key = " << key << " fail");
    return false;
  }
  map<long, LikeDataIPtr>::const_iterator it = like_data_map.begin();
  string db_key = boost::lexical_cast<string>(it->first); 
  if ( db_key != key ) {
    MCE_WARN("[LikeProducer::create] " << ", key = " << key << " fail");
    return false;
  }
  boost::shared_ptr<PbLikeData> message(new PbLikeData);
  message->set_gid(gid);
  vector<int> userids(it->second->getUsers());
  vector<int>::const_iterator user_it = userids.begin();
  while( user_it != userids.end() ) {
    message->add_userid(*user_it++);
  }
  std::string str;
  message->SerializeToString(&str);
  (*value) = ByteSeq(str.begin(), str.end());
  return true;
}

}}


