#include <stdint.h>
#include "rMessageCache.h"
#include "NotifyI.h"
#include "Notify/util/schema_manager.h"
#include "Notify/util/rmessage_util.h"
#include "Notify/util/time_stat.h"

using namespace xce::notify;
using namespace MyUtil;

void UserMessage::AddMessage(const NotifyContentPtr& content){
  MessageBasePtr list = GetList(RMessageUtil::instance().Util_GetRMessageBigtype(content->type));
  if(list){
    list->AddMessage(content);
  }
}

/*
NotifyIndexSeq UserMessage::GetRemindMessages(int begin, int limit) {
  NotifyIndexSeq ans;
  MessageBasePtr msg_list = GetList(REPLY_TYPE);
  if (msg_list) {
    ans = msg_list->GetMessages(begin, limit);
  }
  return ans;
}
*/

//NotifyIndexSeq MessageBase::GetMessages(int begin, int limit) { }

void ReplyMessage::FirstLoad() {
  Statement sql;
  TimeStat time_cost;
  LoadRemindMessageToCache(0, _first_load_count);
  //get the MIN notify id of new messages
  sql << "select min_unread_nid from " << GetInfoTableName(_userid) 
      << " where id = " << _userid;
  mysqlpp::StoreQueryResult res = QueryRunner(GetDbInstance(), CDbRServer, 
                    GetInfoTableName(_userid)).store(sql);
  if(res && res.num_rows() > 0) {
    mysqlpp::Row row = res.at(0);
    _min_unread_id = (Ice::Long)row["min_unread_nid"];
    MCE_INFO("ReplyMessage::FirstLoad --> uid:" << _userid 
        << "  min_id_of_new:" << _min_unread_id);
  } else {
    _min_unread_id = LLONG_MAX;
  }
  _undeal_count = GetUnreadCount();//_undeal_count equal unread_count 
  list<NotifyIndex>::iterator msg_it = _messagelist.begin();
  for (; msg_it != _messagelist.end(); ++msg_it) {
    if (msg_it->id >= _min_unread_id) {
      _unread_count++;
    } 
  }
  MCE_INFO("ReplyMessage::FirstLoad uid:" << _userid << " isload:" << _isload 
      << " bigtype:" << _bigtype << " unread:" << _unread_count 
      <<  " undeal:" << _undeal_count << " cost:" << time_cost.getTime());
}

int ReplyMessage::GetUnreadCount() {
  Statement sql;
  TimeStat time_cost;
  ostringstream os;
  int count = 0;
  sql << "select count(*) as unread_count from " << GetIndexTableName(_userid) 
      << " force index(idx_toid_bigtype_id) where to_id =" << _userid 
      << " and bigtype = " << _bigtype << " and unread = 1"; 
  mysqlpp::StoreQueryResult res = QueryRunner(GetDbInstance(), CDbRServer, 
      GetIndexTableName(_userid)).store(sql);
  if(res && res.num_rows() > 0) {
    mysqlpp::Row row = res.at(0);
    count = (Ice::Long)row["unread_count"];
    MCE_INFO("ReplyMessage::GetUnreadCount --> uid:" << _userid 
        << " unread:" << count);
  } 
  return count;
}

NotifyIndexSeq ReplyMessage::GetSeqByBigtype(int begin, int limit) {
  return GetMessages(begin, limit);
}

NotifyIndexSeq ReplyMessage::GetMessages(int begin, int limit) {
  NotifyIndexSeq remind_msg;
  int cached_messages_size = _messagelist.size();
  if ((begin < 0) || (limit < 0)) {
    MCE_WARN("ReplyMessage::GetMessages --> parameters error uid:" << _userid 
        << "begin:" << begin << " limit:" << limit);
    return remind_msg;
  } 
  // if the cached message num less than half of threshold 
  // then load from db
  if (cached_messages_size < _evict_threshold_value) {
    if (_load_from_db) {
      int limit = _evict_threshold_value - cached_messages_size;
      if (limit < 0 || limit > _evict_threshold_value) {
        limit = _first_load_count;
      }
      LoadRemindMessageToCache(cached_messages_size, limit);
    }
  }
  int get_count = 0;
  get_count = GetRemindMessageFromCache(begin, limit, remind_msg);
  MCE_INFO("ReplyMessage::GetMessages --> uid:" << _userid << " get_count:"
        << get_count << " begin:" << begin << " limit:" << limit);
  return remind_msg;
}

int ReplyMessage::GetRemindMessageFromCache(int begin, int limit,
    NotifyIndexSeq& msgs) {
  typedef map<pair<int, int64_t>, NotifyIndexSeq> MergeMap;
  MergeMap merged_msgs;
  NotifyIndexSeq not_merged_msgs;
  int count = 0;
  list<NotifyIndex>::iterator it = _messagelist.begin();
  for (; it != _messagelist.end(); ++it) {
    if (count > limit) break;
    int stype = RMessageUtil::instance().Util_GetRMessageType(it->type);
    if (SchemaManager::instance().MustMerge(stype, 16)) {
      pair<int, int64_t> stype_source_pair(stype, it->source);
      MergeMap::iterator map_it = merged_msgs.find(stype_source_pair);
      if (map_it != merged_msgs.end()) {
        map_it->second.push_back(*it);
      } else {
        merged_msgs[stype_source_pair].push_back(*it);
        count++;
      }
    } else {
      not_merged_msgs.push_back(*it);
      count++;
    }
  }
  for(MergeMap::iterator it = merged_msgs.begin(); it != merged_msgs.end(); ++it) {
    for(NotifyIndexSeq::iterator index_it = it->second.begin(); 
        index_it != it->second.end(); ++index_it) {
      msgs.push_back(*index_it);
    }
  }
  msgs.insert(msgs.end(), not_merged_msgs.begin(), not_merged_msgs.end());
  if(LLONG_MAX != _min_unread_id ){
    _min_unread_id = LLONG_MAX;
    TaskManager::instance().execute(new UpdateMinUnreadIdTask(_userid, _min_unread_id));
  }
  _unread_count = 0;
  return count;
}

void ReplyMessage::LoadRemindMessageToCache(int begin, int limit) {
  Statement sql;
  TimeStat time_cost;
  sql << "select id, type, source, from_id, is_reply, bigtype, unread from " 
      << GetIndexTableName(_userid) 
      << " force index(idx_toid_bigtype_id) where to_id =" << _userid 
      << " and bigtype = " << _bigtype << " and unread = 1" 
      << " order by id desc limit " << begin << ", " << _first_load_count; 
  mysqlpp::StoreQueryResult res = QueryRunner(GetDbInstance(), CDbRServer, 
      GetIndexTableName(_userid)).store(sql);
  int query_count = 0;
  if (res) {
    query_count = res.num_rows();
    if (query_count > 0) {
      for (size_t i = 0; i < res.num_rows(); ++i) {
        NotifyIndex index;
        mysqlpp::Row row = res.at(i);
        index.id = (Ice::Long) row["id"];
        int bigtype = (int)row["bigtype"];
        int type = (Ice::Int) row["type"];
        type |= (bigtype << 16);
        index.type = type;
        index.source = (Ice::Long) row["source"];
        index.from = (Ice::Int) row["from_id"];
        index.isreply = (Ice::Int) row["is_reply"];
        index.unread = row["unread"];
        _messagelist.push_back(index);
      }
    } else if (query_count == 0) {
      _load_from_db = false;
    }
  }
  MCE_INFO("ReplyMessage::LoadRemindMessageToCache --> uid:" << _userid 
      << " query:" << query_count << " undeal:" << _undeal_count 
      << " bigtype:" << _bigtype << " isload:" << _isload << " load_from_db:"
      << _load_from_db << " cost:" << time_cost.getTime());
}

int UserMessage::GetBigTypeByNotifyId(int64_t notify_id) {
  if (_reply_message->IsExistInMemory(notify_id)) {
    return _reply_message->_bigtype;
  }
  if (_request_message->IsExistInMemory(notify_id)) {
    return _request_message->_bigtype;
  }
  if (_inform_message->IsExistInMemory(notify_id)) {
    return _inform_message->_bigtype;
  }
  return -1;
}

int UserMessage::GetTypeByNotifyId(int64_t notify_id) {
  int type = _reply_message->GetTypeByNotifyId(notify_id); 
  if (type < 0) { //get type from request message
    type = _request_message->GetTypeByNotifyId(notify_id); 
    MCE_DEBUG("request_message::GetTypeByNotifyId::type:" << type);
  }
  if (type < 0) { //get type from inform messages
    type = _inform_message->GetTypeByNotifyId(notify_id); 
    MCE_DEBUG("inform_message::GetTypeByNotifyId::type:" << type);
  }
  return type;
}

int MessageBase::GetUndealCountByBigtype(){
  return _undeal_count; 
}
int MessageBase::GetUndealCountByType(int type){
  int ans = 0;
  map<int, int>::iterator it = _type_undeal_count.find(type);
  if(it != _type_undeal_count.end()){
    ans = it->second;
  }
  return ans;
}

NotifyIndexSeq MessageBase::LoadSeqFromDBNowMerge(int wantbegin, int wantsize) {
  if(_load_stat == LOAD_STAT_BUSY || !_isload){
    MCE_DEBUG("MessageBase::LoadSeqFromDBNowMerge load_stat is busy so do not load uid[" 
        << _userid << "]wantbegin[" << wantbegin << "]wantsize[" << wantsize 
        << "]load_stat[" << _load_stat << "]bigtype[" << _bigtype 
        << "]isload[" << _isload << "]");
    return NotifyIndexSeq();
  } 
  _load_stat = LOAD_STAT_BUSY;
  MyUtil::IntSeq types = SchemaManager::instance().GetTypesByBigtype(_bigtype);
  if(types.empty()){
    MCE_WARN("MessageBase::LoadSeqFromDBNowMerge --> GetTypesByBigtype types empty uid:" 
        << _userid << " bigtype:" << _bigtype);
  }
  ostringstream oss;
  for(MyUtil::IntSeq::iterator tit = types.begin(); tit != types.end(); ++tit){
    if(tit != types.begin()){
      oss << ",";
    }
    oss << boost::lexical_cast<string>(*tit);
  }
  typedef map<pair<int, Ice::Long> , NotifyIndexSeq> MergeMap;
  MergeMap merge;
  NotifyIndexSeq ans;//no merge
  NotifyIndexSeq nomerge;//no merge
  Ice::Long min_message_id = GetMinMessageId();
  MCE_INFO("MessageBase::LoadSeqFromDBNowMerge --> uid:" << _userid 
      << " wantbegin:" << wantbegin << " wantsize:" << wantsize 
      << " min_message_id:" << min_message_id 
      << " bigtype:" << _bigtype 
      << " _isload:" << _isload);
  //while (true) {
  for (int i = 0; i < 10; ++i) {
    bool isbreak = false;
    Statement sql;
    list<NotifyIndex> seq;
    sql << "select id, type, source, from_id ,bigtype from " 
        << GetIndexTableName(_userid) 
        << " force index(idx_toid_bigtype_id) where to_id =" << _userid 
        << " and id < " << min_message_id <<" and bigtype = " << _bigtype << " and unread = 1" 
        << " order by id desc limit " <<  _first_load_count; 
    mysqlpp::StoreQueryResult res = QueryRunner(GetDbInstance(), CDbRServer, 
        GetIndexTableName(_userid)).store(sql);
    if(!res){
      isbreak = true;
    }
    if (res && res.num_rows() > 0) {
      for (size_t i = 0; i < res.num_rows(); ++i) {
        NotifyIndex index;
        mysqlpp::Row row = res.at(i);
        index.id = (Ice::Long) row["id"];
        int bigtype = (int)row["bigtype"];
        int type = (int)row["type"];
        type |= (bigtype << 16);
        index.type = type;
        index.source = (Ice::Long) row["source"];
        index.from = (Ice::Int) row["from_id"];
        int stype = RMessageUtil::instance().Util_GetRMessageType(index.type);
        bool ismerge = SchemaManager::instance().MustMerge(stype, 16);
        if(ismerge){
          merge[make_pair<int, Ice::Long>(stype, index.source)].push_back(index);
        } else{
          nomerge.push_back(index);
        }
        int realbeginsize = nomerge.size() + merge.size();
        seq.push_back(index);
        if(realbeginsize > wantsize + wantbegin){
          MCE_DEBUG("MessageBase::LoadSeqFromDBNowMerge BREAK userid:" 
              << _userid << " realbeginsize:" << realbeginsize 
              << " wantsize+wantbegin:" << wantsize + wantbegin);
          isbreak = true;
        } else {
          int realtotalsize = nomerge.size() + merge.size();
          int endpos = wantbegin + wantsize;
          if(!wantbegin){
            endpos += 1;  
          }
          if(realtotalsize >= wantbegin && realtotalsize < endpos){
            ans.push_back(index);
            MCE_DEBUG("MessageBase::LoadSeqFromDBNowMerge insert into ans id:" << index.id);
          }
        }
      }
    }
    if((int)(seq.size()) < _first_load_count){
      //update load state
      MCE_DEBUG("MessageBase::LoadSeqFromDBNowMerge BREAK _userid[" << _userid 
          << "]seq.size[" << seq.size() << "]first_load_count[" 
          << _first_load_count << "]");
      _isload = false;
      isbreak = true;
    }
    AppendMessages(seq, false);
    MCE_DEBUG("MessageBase::LoadSeqFromDBNowMerge Append msg size[" 
        << seq.size() << "]");
    min_message_id = GetMinMessageId();
    if(isbreak){
      break;
    }
  }
  _load_stat = LOAD_STAT_FREE;
  MCE_INFO("MessageBase::LoadSeqFromDBNowMerge --> uid:" << _userid 
      << " ans:" << ans.size() << " merge_size:" << merge.size() 
      << " nomerge_size:" << nomerge.size() << " load_stat:" << _load_stat 
      << " bigtype:" << _bigtype << " isload:" << _isload );
  return ans;
}

NotifyIndexSeq MessageBase::LoadSeqFromDBNow(const set<int>& types, 
    int wantbegin, int wantsize){
  if(_load_stat == LOAD_STAT_BUSY || !_isload){
    MCE_INFO("MessageBase::LoadSeqFromDBNow load_stat busy,do not load,uid["
        << _userid << "]wantbegin[" << wantbegin << "]wantsize[" << wantsize 
        << "]load_stat[" << _load_stat << "]bigtype[" << _bigtype << "]_isload[" 
        << _isload << "]");
    return NotifyIndexSeq();
  } 
  _load_stat = LOAD_STAT_BUSY;
  MyUtil::IntSeq typess = SchemaManager::instance().GetTypesByBigtype(_bigtype);
  if(typess.empty()){
    MCE_WARN("MessageBase::LoadSeqFromDBNow GetTypesByBigtype result empty!"
        << "uid[" << _userid << "]bigtype[" << _bigtype << "]");
  }
  ostringstream oss;
  for(MyUtil::IntSeq::iterator tit = typess.begin(); tit != typess.end(); ++tit){
    if(tit != typess.begin()){
      oss << ",";
    }
    oss << boost::lexical_cast<string>(*tit);
  }
  NotifyIndexSeq ans;
  Ice::Long min_message_id = GetMinMessageId();
  int s = 0;
  MCE_INFO("MessageBase::LoadSeqFromDBNow --> types:" << types.size() 
      << " uid:" << _userid << " wantbegin:" << wantbegin << " wantsize:" 
      << wantsize << " min_messag_id:" << min_message_id << " bigtype:" 
      << _bigtype << " isload:" << _isload);
  //while (true) {
  for(int i = 0; i < 5; ++i) {
    bool isbreak = false;
    Statement sql;
    list<NotifyIndex> seq;
    sql << "select id, type, source, from_id ,bigtype from " << GetIndexTableName(_userid) 
        << " force index(idx_toid_bigtype_id) where to_id =" << _userid 
        << " and id < " << min_message_id << " and bigtype = " << _bigtype << " and unread = 1" 
        << " order by id desc limit " <<  _first_load_count; 
    mysqlpp::StoreQueryResult res = QueryRunner(GetDbInstance(), CDbRServer,
        GetIndexTableName(_userid)).store(sql);
    if(!res){
      isbreak = true;
    }
    if (res && res.num_rows() > 0) {
      for (size_t i = 0; i < res.num_rows(); ++i) {
        NotifyIndex index;
        mysqlpp::Row row = res.at(i);
        index.id = (Ice::Long) row["id"];
        int bigtype = (int)row["bigtype"];
        int type = (int)row["type"];
        type |= (bigtype << 16);
        index.type = type;
        index.source = (Ice::Long) row["source"];
        index.from = (Ice::Int) row["from_id"];
        seq.push_back(index);
        if(types.find(index.type) != types.end() || types.find(-1) != types.end()){
          if((int)(ans.size()) > wantsize + wantbegin){
            isbreak = true;
          }else{
            if(s >= wantbegin){
              ans.push_back(index);
            }
          }
          ++s;
        }
      }
    }
    if((int)(seq.size()) < _first_load_count){
      //update load state
      _isload = false;
      isbreak = true;
    }
    AppendMessages(seq, false);
    MCE_DEBUG("MessageBase::LoadSeqFromDBNow Append msg size[" 
        << seq.size() << "]");
    min_message_id = GetMinMessageId();
    if (isbreak) {
      break;
    }
  }
  _load_stat = LOAD_STAT_FREE; 
  MCE_INFO("MessageBase::LoadSeqFromDBNow --> uid:" << _userid 
      << " ans:" << ans.size() 
      << " load_stat:" << _load_stat 
      << " bigtype:" << _bigtype 
      << " isload:" << _isload);
  return ans;
}

void MessageBase::Evict(){
  bool e = false;
  while(true){
    if((int)(_messagelist.size()) <= _evict_threshold_value){
      break;
    }
    e = true;
    _messagelist.pop_back();
  }
  if(e){
    MCE_INFO("MessageBase::Evict uid[" << _userid << "]msg size[" 
        << _messagelist.size() << "]");
  }
  _isload = true;
}

void MessageBase::AppendMessages(const list<NotifyIndex>& seq, bool update_load_stat){
  if(update_load_stat){
    _load_stat = LOAD_STAT_FREE;
  }
  if(seq.empty()){
    MCE_DEBUG("MessageBase::AppendMessage --> msgs is empty uid:" << _userid);
    return;
  }
  for(list<NotifyIndex>::const_iterator it = seq.begin(); it != seq.end(); ++it){
    _messagelist.push_back(*it);
  }
  if((int)(_messagelist.size()) >= _evict_threshold_value){
    NotifyIndexManagerI::instance().UserRMessageEvict(_userid);
    MCE_DEBUG("MessageBase::AppendMessages --> uid:" << _userid 
        << " load stat:" << update_load_stat << " should Evict");
  } 
}

void MessageBase::UpdateUserBigtypeLoadStat(bool isload){
  _isload = isload;
  MCE_DEBUG("MessageBase::UpdateUserBigtypeLoadStat userid[" << _userid 
      << "]bigtye[" << _bigtype << "]load_stat[" << _load_stat << "]isload[" 
      << _isload << "]");
}

int MessageBase::GetUnreadCountByBigtype(){
  MCE_DEBUG("MessageBase::GetUnreadCountByBigtype uid[" << _userid 
      << "]unread[" << _unread_count << "]");
  return _unread_count;
}

void MessageBase::CheckAndLoadFromDB(int leftvalue, 
    const set<Ice::Long>& blacklist_nids){
  MCE_DEBUG("MessageBase::CheckAndLoadFromDB --> uid:" << _userid 
      << " leftvalue:" << leftvalue 
      << " bigtype:" << _bigtype 
      << " load_stat:" << _load_stat 
      << " isload:" << _isload);
  if(_load_stat == LOAD_STAT_FREE && _isload){
    int left_merge_count = 0;
    if(leftvalue) {
      map< pair<int, long>, int > left_merge;
      int c = 0;
      for(list<NotifyIndex>::reverse_iterator it = _messagelist.rbegin(); 
          it != _messagelist.rend(); ++it, ++c){
        if(c >= leftvalue || left_merge_count > _preload_threshold_value){
          break;
        }
        int type = (*it).type;
        Ice::Long source = (*it).source;
        left_merge[make_pair<int, long>(type, source)] += 1;
      }
      left_merge_count = (int)left_merge.size();
    }
    if(left_merge_count <= _preload_threshold_value){
      MCE_DEBUG("MessageBase::CheckAndLoadFromDB --> uid" << _userid 
          << " use preload start LoadMessageTask");
      _load_stat = LOAD_STAT_BUSY;
      TaskManager::instance().execute(new LoadMessageTask(_userid, _bigtype, 
          _first_load_count, GetMinMessageId(), blacklist_nids));
    }
  }
  MCE_INFO("MessageBase::CheckAndLoadFromDB --> uid:" << _userid 
      << " bigtype:" << _bigtype 
      << " left:" << leftvalue
      << " load_stat:" << _load_stat 
      << " isload:" << _isload);
}

NotifyIndexSeq MessageBase::GetSeqBeforLastNid(int view, Ice::Long nid, int limit){
  NotifyIndexSeq ans;
  int count = limit;
  if(limit < 0 || limit >= (int)(_messagelist.size())){
    count = _messagelist.size();
  }
  int s = 0, c = 0;
  for(list<NotifyIndex>::iterator it = _messagelist.begin(); it != _messagelist.end() && c < count; ++it, ++s){
    if((view == 1 || view == 0) && ((*it).id >= nid)){
      ans.push_back((*it));
      c++;
    }
  }
  int endpos = count;
  int messagesize = _messagelist.size();
  CheckAndLoadFromDB(messagesize - endpos, set<Ice::Long>());
  
  if(_min_unread_id != LLONG_MAX){
    _min_unread_id = LLONG_MAX;
    TaskManager::instance().execute(new UpdateMinUnreadIdTask(_userid, _min_unread_id));
  }
  _unread_count = 0;
  MCE_INFO("MessageBase::GetSeqBeforLastNid userid[" << _userid 
      << "]_messagelist size[" << _messagelist.size() << "]_min_unread_id[" 
      << _min_unread_id << "]_unread_count[" << _unread_count << "]ans size[" 
      << ans.size() << "]");
  return ans;
}

NotifyIndexSeq MessageBase::GetSeqBeforMerge(int view, Ice::Long nid, int limit){
  typedef map<pair<int, Ice::Long> , NotifyIndexSeq> MergeMap;
  MergeMap merge;
  NotifyIndexSeq ans;
  if(nid < 0){
    nid = LLONG_MAX;
  }
  int count = 0;
  //int begin = 0;
  if(limit < 0){
    return ans;
  }
  if(limit >= (int)(_messagelist.size())){
    limit = _messagelist.size();
  }
  for(list<NotifyIndex>::iterator it = _messagelist.begin(); it != _messagelist.end(); ++it){
    if((int)(merge.size()) >= limit){
      break;
    }
    NotifyIndex index = *it;
    if(index.id <= nid){
      //merge[make_pair<int, Ice::Long>(index.type, index.source)].push_back(index);
      merge[make_pair<int, Ice::Long>(RMessageUtil::instance().Util_GetRMessageType(index.type), index.source)].push_back(index);
    }
    count++;
  }

  for(MergeMap::iterator it = merge.begin(); it != merge.end(); ++it){
    for(NotifyIndexSeq::iterator vit = it->second.begin(); vit != it->second.end(); ++vit){
      ans.push_back(*vit);
    }
  }
  int endpos =  count;
  int messagesize = _messagelist.size();
  CheckAndLoadFromDB(messagesize - endpos, set<Ice::Long>());
  
  if(LLONG_MAX != _min_unread_id ){
    _min_unread_id = LLONG_MAX;
    TaskManager::instance().execute(new UpdateMinUnreadIdTask(_userid, _min_unread_id));
  }
  _unread_count = 0;
  MCE_INFO("MessageBase::GetSeqBeforMerge-->userid[" << _userid 
      << "]_message size[" << _messagelist.size() << "]_min_unread_id[" 
      << _min_unread_id << "]_unread_count[" << _unread_count 
      << "]NotifyIndexSeq[" << ans.size() << " count[" << count 
      << "]merge size[" << merge.size() << "]");
  return ans;
}

NotifyIndexSeq MessageBase::GetSeqByBigtype(int begin, int limit){
  typedef map<pair<int, Ice::Long> , NotifyIndexSeq> MergeMap;
  MergeMap merge;//需要合并的结果集
  NotifyIndexSeq nomerge;//不需要合并的结果集
  NotifyIndexSeq nomergeans;//不需要合并的结果集
  MergeMap seqs;

  NotifyIndexSeq ans;
  int count = limit;
  if(begin < 0){
    return ans;
  }
  if(limit < 0 || limit >= (int)(_messagelist.size())){
    count = _messagelist.size();
  }
  int realbeginsize = 0;
  int realtotalsize = 0;
  if((int)(_messagelist.size()) >= begin){
    MCE_DEBUG("MessageBase::GetSeqByBigtype uid:" << _userid << " msg_size:" 
        << _messagelist.size() << " begin:" << begin << " limit:" << limit);
    int s = 0;
    for(list<NotifyIndex>::iterator it = _messagelist.begin(); it != _messagelist.end(); ++it, ++s){
      NotifyIndex index = *it;
      int stype = RMessageUtil::instance().Util_GetRMessageType(index.type);
      bool ismerge = SchemaManager::instance().MustMerge(stype, 16);
      if(ismerge){
        seqs[make_pair<int, Ice::Long>(stype, index.source)].push_back(index);
        realbeginsize = seqs.size() + nomerge.size();//get the total size
        if(realbeginsize >= begin){ 
          MergeMap::iterator mit = merge.find(make_pair<int, Ice::Long>(stype, index.source));
          if(mit != merge.end()){
            mit->second.push_back(index);
            MCE_DEBUG("MessageBase::GetSeqByBigtype add one to a merged seq"
                << " uid:" << _userid << " realtotalsize:" << realtotalsize 
                << " nomergeans:" << nomergeans.size() << " merges:" 
                << merge.size() << " nid:" << index.id);
          } else {
            realtotalsize = nomergeans.size() + merge.size();
            if(realtotalsize >= count){
              MCE_DEBUG("MessageBase::GetSeqByBigtype enough uid:" << _userid 
                  << " realtotalsize:" << realtotalsize << " count:" << count);
              break;
            }
            merge[make_pair<int, Ice::Long>(stype, index.source)].push_back(index);
            MCE_DEBUG("MessageBase::GetSeqByBigtype create a new seq uid:" 
                << _userid << " realtotalsize:" << realtotalsize 
                << " nomergeans:" << nomergeans.size() << " merges:" 
                << merge.size() << " nid:" << index.id);
          }
        }
      } else {
        nomerge.push_back(index);
        realbeginsize = seqs.size() + nomerge.size();
        if(realbeginsize >= begin){ 
          realtotalsize = nomergeans.size() + merge.size();
          if(realtotalsize >= count){
            MCE_DEBUG("MessageBase::GetSeqByBigtype enough, uid:" << _userid 
                << " realtotalsize:" << realtotalsize << " count:" 
                << count);
            break;
          }
          nomergeans.push_back(index);
          MCE_DEBUG("MessageBase::GetSeqByBigtype add one to nomerge uid["
              << _userid << " realtotalsize:" << realtotalsize 
              << " nomergeans.size:" << nomergeans.size() << " merges:" 
              << merge.size() << " nid:" << index.id);
        }
      }
    }
  }
  for(MergeMap::iterator it = merge.begin(); it != merge.end(); ++it){
    for(NotifyIndexSeq::iterator vit = it->second.begin(); vit != it->second.end(); ++vit){
      ans.push_back(*vit);
    }
  }
  ans.insert(ans.end(), nomergeans.begin(), nomergeans.end());
  if(LLONG_MAX != _min_unread_id ){
    _min_unread_id = LLONG_MAX;
    TaskManager::instance().execute(new UpdateMinUnreadIdTask(_userid, _min_unread_id));
  }
  _unread_count = 0;
  realtotalsize = merge.size() + nomergeans.size();
  int messagesize = _messagelist.size();
  if(realtotalsize > count || (realtotalsize == count && count)){
    int endpos = begin + count;
    if(messagesize < 600) {
      CheckAndLoadFromDB(messagesize - endpos, set<Ice::Long>());
    }
    MCE_INFO("MessageBase::GetSeqByBigtype uid:" << _userid << " bigtype:" 
      << _bigtype << "]msg size[" << messagesize << "]merge[" << merge.size() 
      << "]nomerge[" << nomerge.size() << "]nomergeans size[" << nomergeans.size() 
      << "]ans size[" << ans.size() << "]unread[" << _unread_count 
      << "]begin[" << begin << "]limit[" << limit << "]");
    return ans;
  } else {
    int wantsize = count - realtotalsize;
    int wantbegin = 0;
    if(realtotalsize){
      wantbegin = 0;
    }else{
      wantbegin = begin;
    }
    if(messagesize < 600) {
      NotifyIndexSeq ans2 = LoadSeqFromDBNowMerge(wantbegin ,wantsize);
      if(!ans2.empty()){
        ans.insert(ans.end(), ans2.begin(), ans2.end());
      }
      CheckAndLoadFromDB(0, set<Ice::Long>());
    }
    MCE_INFO("MessageBase::GetSeqByBigtype after LoadSeqFromDBNowMerge "
        << "uid[" << _userid << "]bigtype[" << _bigtype 
        << "]ans size[" << ans.size() 
        << "]msg_size[" << messagesize 
        << "]wantbegin[" << wantbegin << "]wantsize[" 
        << wantsize << "]");
    return ans;
  }
}

NotifyIndexSeq MessageBase::GetSeqByTypeAndBigtype(int userid, 
    const MyUtil::IntSeq& types, int begin, int limit, int bigtype){
  NotifyIndexSeq ans;
  set<int> typeset;
  for(MyUtil::IntSeq::const_iterator tit = types.begin(); tit != types.end(); ++tit){
    typeset.insert(*tit);
  }
  if(limit <= 0){
    limit = _messagelist.size();
  }
  if(types.size() == 1){
    int firsttype = (*(types.begin()));
    if(_type_undeal_count.find(firsttype) != _type_undeal_count.end()){
      int undealcount = _type_undeal_count[firsttype];
      if(limit > undealcount){
        limit = undealcount;
      }
    }
  }
  int s = 0;
  for(list<NotifyIndex>::iterator it = _messagelist.begin(); it != _messagelist.end(); ++it){
    if((int)(ans.size()) >= limit){
      break;
    }
    NotifyIndex index = *it;
    int type = RMessageUtil::instance().Util_GetRMessageType(index.type);
    if(typeset.find(type) != typeset.end()){
      if(s >= begin){
        ans.push_back((*it));
        MCE_DEBUG("MessageBase::GetSeqByTypeAndBigtype uid[" << userid 
            << "]count[" << s << "]type[" << type 
            << "]index.type[" << index.type << "]");
      }
      ++s;
    }
  }
  if(LLONG_MAX != _min_unread_id){
    _min_unread_id = LLONG_MAX;
    TaskManager::instance().execute(new UpdateMinUnreadIdTask(_userid, _min_unread_id));
  }
  _unread_count = 0;
  if((int)(ans.size()) >= limit){
    int endpos = begin + limit;
    int messagesize = _messagelist.size();
    CheckAndLoadFromDB(messagesize - endpos, set<Ice::Long>());
    MCE_INFO("MessageBase::GetSeqByTypeAndBigtype --> CheckAndLoadFromDB uid:" 
        << userid << " bigtype:" << bigtype << " msg size:" << _messagelist.size() 
        << " min_unread:" << _min_unread_id << " unread:" << _unread_count 
        << " ret_size:" << ans.size() << " begin:" << begin << " limit:" 
        << limit << " endpos:" << endpos);
    return ans;
  }else{
    int wantsize = limit - ans.size();  
    int wantbegin = 0;
    //if(ans.empty()){
    if(ans.size()){
      wantbegin = 0;
    }else{
      wantbegin = begin;
    }
    size_t msg_size = _messagelist.size();
    if(msg_size < 600) {
      NotifyIndexSeq ans2 = LoadSeqFromDBNow(typeset, wantbegin ,wantsize);
      if(!ans2.empty()){
        ans.insert(ans.end(), ans2.begin(), ans2.end());
      }
    }
    MCE_INFO("MessageBase::GetSeqByTypeAndBigtype --> LoadSeqFromDBNow uid:"
        << userid 
        << " bigtype:" << bigtype 
        << " msg size:" << msg_size 
        << " min_unread:" << _min_unread_id 
        << " unread:" << _unread_count 
        << " wantbegin:" << wantbegin 
        << " wantsize:" << wantsize 
        << " ret size:" << ans.size() 
        << " begin:" << begin 
        << " limit:" << limit);
    return ans;
  }
}

NotifyIndexSeqMap MessageBase::GetSeqByTypeAndBigtypeAll(int userid, 
    const MyUtil::IntSeq& types, int begin, int limit, int bigtype){
  NotifyIndexSeqMap ans;
  return ans;
}


void MessageBase::RemoveBySourceAndSender(int type, Ice::Long source, int sender){
  set<Ice::Long> delnids;
  int delcount = 0;
  for(list<NotifyIndex>::iterator it = _messagelist.begin(); it != _messagelist.end();){
    int realtype = RMessageUtil::instance().Util_GetRMessageType((*it).type);
    if(realtype == type && (*it).source == source && (*it).from == sender){
      UpdateUnreadCount((*it).id);
      delnids.insert((*it).id);
      ++delcount;
      _messagelist.erase(it++);
    }
    else{
      ++it;
    }
  }
  UpdateUndealCount(type, delcount);
  Statement sql;
  GetRemoveBySourceAndSenderSql(sql, type, source, sender);
  //TaskManager::instance().execute(new RemoveDBTask(sql, _userid));
  //TaskPtr t = new RemoveDBTask(sql, _userid);
  //t->handle();
  CheckAndLoadFromDB(_messagelist.size(), delnids);
}

void MessageBase::RemoveBySource(int type, Ice::Long source){
  set<Ice::Long> delnids;
  ostringstream os; 
  os << "MessageBase::RemoveBySource--> userid:" << _userid << " bigtype:" 
      << _bigtype << " type:" << type << " source" << source << " nids:";
  int delcount = 0;
  for(list<NotifyIndex>::iterator it = _messagelist.begin(); it != _messagelist.end();){
    int realtype = RMessageUtil::instance().Util_GetRMessageType((*it).type);
    if(realtype == type && (*it).source == source){
      delnids.insert((*it).id);
      UpdateUnreadCount((*it).id);
      ++delcount;
      _messagelist.erase(it++);
      os << (*it).id << ", ";
    }else{
      ++it;
    }
  }
  UpdateUndealCount(type, delcount);
  Statement sql;
  GetRemoveBySourceSql(sql);
  //TaskManager::instance().execute(new RemoveDBTask(sql, _userid));
  //TaskPtr t = new RemoveDBTask(sql, _userid);
  //t->handle();
  os << " after remove  _messagelis.size:" << _messagelist.size() 
     << " blacklist.size():"<< delnids.size(); 
  MCE_DEBUG(os.str());
  CheckAndLoadFromDB(_messagelist.size(), delnids);
}

void MessageBase::GetSQLRemoveAllByBigtype(Statement& sql) {
  ostringstream sql_oss;
  sql_oss << "DELETE FROM " << GetIndexTableName(_userid) 
    << " WHERE to_id = " << _userid << " AND bigtype = " << _bigtype;
  sql << sql_oss.str();
  MCE_DEBUG(sql_oss.str())
}

void MessageBase::GetSQLToReadAllByBigtype(Statement& sql) {
  ostringstream sql_oss;
  sql_oss << "UPDATE " << GetIndexTableName(_userid) 
    << " SET unread = 0 WHERE to_id = " << _userid << " AND bigtype = " << _bigtype;
  sql << sql_oss.str();
  MCE_DEBUG(sql_oss.str())
}

void MessageBase::GetSQLRemoveRepeatedByBigtype(Statement& sql) {
  ostringstream sql_oss;
  string table_name = GetIndexTableName(_userid);
  sql_oss << "DELETE FROM a using " << table_name
      << " as a, " << table_name << " as b WHERE a.to_id = " 
      << _userid  << " and a.bigtype = " << _bigtype
      << " and a.id < b.id "
      << " and a.type = b.type " 
      << " and a.source = b.source " 
      << " and a.to_id = b.to_id "
      << " and a.from_id = b.from_id "
      << " and a.bigtype = b.bigtype ";
  sql << sql_oss.str();
  MCE_DEBUG(sql_oss.str())
}

void MessageBase::GetSQLToProcessMsg(Statement& sql, int64_t nid) {
  ostringstream sql_oss;
  string table_name = GetIndexTableName(_userid);
  sql_oss << "UPDATE " << table_name
      << " set unread =0  WHERE to_id = " 
      << _userid  << " and bigtype = " << _bigtype
      << " and id = " << nid;
  sql << sql_oss.str();
  MCE_DEBUG(sql_oss.str())
}

void MessageBase::RemoveAllByBigtype(int bigtype){
  if (_bigtype != bigtype) {
    MCE_WARN("MessageBase::RemoveAllByBigtype parameter error _bigtype " 
      << _bigtype << " bigtype " << bigtype);
  }
  MCE_INFO("MessageBase::RemoveAllByBigtype uid " << _userid 
     << " bigtype " << _bigtype);
  _messagelist.clear();
  _unread_count = 0;
  _undeal_count = 0;
  Statement sql;
  GetSQLRemoveAllByBigtype(sql);
  TaskManager::instance().execute(new RemoveDBTask(sql, _userid));
}

void MessageBase::ReadAllByBigtype(int bigtype){
  if (_bigtype != bigtype) {
    MCE_WARN("MessageBase::ReadAllByBigtype parameter error _bigtype " 
      << _bigtype << " bigtype " << bigtype);
  }
  Statement sql;
  GetSQLToReadAllByBigtype(sql);
  TaskManager::instance().execute(new ReadAllMsgDbTask(sql, _userid));
  _messagelist.clear();
  _unread_count = 0;
  _undeal_count = 0;
  MCE_INFO("MessageBase::ReadAllByBigtype uid " << _userid 
     << " bigtype " << _bigtype << " undeal:" << _undeal_count);
}

void MessageBase::RemoveRepeatedByBigtype(int bigtype){
  if (_bigtype != bigtype) {
    MCE_WARN("MessageBase::RemoveRepeatedByBigtype --> parameter error uid:" 
      << _userid << " _bigtype:" << _bigtype << " bigtype:" << bigtype);
  }
  TimeStat time_cost;
  set<int64_t> keys;
  typedef multimap<int64_t, NotifyIndex> IndexMultiMap;
  IndexMultiMap repeated_indexes;
  list<NotifyIndex>::reverse_iterator list_it = _messagelist.rbegin(); 
  for (; list_it != _messagelist.rend(); ++list_it) {
    int64_t sum = list_it->source + list_it-> from + list_it->type;
    repeated_indexes.insert(pair<int64_t, NotifyIndex>(sum, *list_it));
    keys.insert(sum);
  }
  typedef IndexMultiMap::iterator multimap_iter;
  pair<multimap_iter, multimap_iter> values;
  set<int64_t>::iterator key_it = keys.begin();
  int total_count = 0;
  for (; key_it != keys.end(); ++key_it) {
    int count = repeated_indexes.count(*key_it);
    if (count > 1) {
      total_count += count - 1;
      values = repeated_indexes.equal_range(*key_it);
      for (multimap_iter it = values.first; (it != values.second) && (count > 1);
          ++it, count--) {
        UpdateUnreadCount(it->second.id);
        UpdateUndealCount(it->second.type, 1);
        _messagelist.remove(it->second);
        MCE_DEBUG("MessageBase::RemoveRepeatedByBigtype --> uid:" << _userid 
           << " bigtype:" << _bigtype 
           << " nid:" << it->second.id
           << " type:" << it->second.type 
           << " source:" << it->second.source 
           << " from:" << it->second.from);
      }
    }
  }
  Statement sql;
  GetSQLRemoveRepeatedByBigtype(sql);
  TaskManager::instance().execute(new RemoveDBTask(sql, _userid));
  MCE_INFO("MessageBase::RemoveRepeatedByBigtype --> uid:" << _userid 
      << " bigtype:" << bigtype << " count:" << total_count 
      << " cost:" << time_cost.getTime());
}

void MessageBase::RemoveByIds(const MyUtil::LongSeq& nids, int type){
  ostringstream os; 
  int delcount = 0;
  os << "MessageBase::RemoveByIds uid:" << _userid << " bigtype:" << _bigtype << " nids:";
  for(MyUtil::LongSeq::const_iterator nit = nids.begin(); nit != nids.end(); ++nit){
    MCE_DEBUG("MessageBase::RemoveByIds WANT DELETE IDS:" << (*nit));
  }
  for(list<NotifyIndex>::iterator it = _messagelist.begin(); it != _messagelist.end(); ++it){
    MCE_DEBUG("MessageBase::RemoveByIds NOW HAVE[" << (*it).id);
  }
  for(MyUtil::LongSeq::const_iterator nit = nids.begin(); nit != nids.end(); ++nit){
    Ice::Long nid = *nit;
    for(list<NotifyIndex>::iterator it = _messagelist.begin(); it != _messagelist.end(); ){
      if((*it).id == nid){
        os << nid << ", ";
        UpdateUnreadCount((*it).id);
        _messagelist.erase(it++);
        break;
      } else {
        ++it;
      }
    }
    ++delcount;
  }
  UpdateUndealCount(type, delcount);
  MCE_INFO(os.str());
  Statement sql;
  GetRemoveByIdsSql(sql, nids);
  //TaskManager::instance().execute(new RemoveDBTask(sql, _userid));
  //TaskPtr t = new RemoveDBTask(sql, _userid);
  //t->handle();
  set<Ice::Long> blacklist_nids;
  blacklist_nids.insert(nids.begin(), nids.end());
  CheckAndLoadFromDB(_messagelist.size(), blacklist_nids);
}

void MessageBase::ProcessMessageById(int64_t nid, int type){
  for(list<NotifyIndex>::iterator it = _messagelist.begin(); 
      it != _messagelist.end(); ++it) {
    if(it->id == nid) {
      _messagelist.erase(it);
      MCE_DEBUG("MessageBase::ProcessMessageById --> uid:" << _userid 
          << " nid:" << nid << " type:" << type << " bigtype:" << _bigtype
          << " undeal:" << _undeal_count);
      break;
    }
  }
  --_undeal_count;
  if (_undeal_count < 0) { 
    _undeal_count = 0;
  }
  Statement sql;
  GetSQLToProcessMsg(sql, nid);
  TaskManager::instance().execute(new UpdateUnreadOfIndexInDbTask(sql, _userid));
  //CheckAndLoadFromDB(_messagelist.size());
  MCE_INFO("MessageBase::ProcessMessageById --> uid:" << _userid 
      << " nid:" << nid << " type:" << type << " bigtype:" << _bigtype);  
}

void MessageBase::ProcessMessageByIds(const MyUtil::LongSeq& nids, int type) {
  for (MyUtil::LongSeq::const_iterator processed_it = nids.begin(); 
      processed_it != nids.end(); ++processed_it) {
    for (list<NotifyIndex>::iterator cache_it = _messagelist.begin(); 
      cache_it != _messagelist.end();) {
      if(cache_it->id == *processed_it) {
        _messagelist.erase(cache_it++);
        break;
      } else {
        ++cache_it;
      }
    }
    --_undeal_count;
    if (_undeal_count < 0) { 
      _undeal_count = 0;
    }
    Statement sql;
    GetSQLToProcessMsg(sql, *processed_it);
    TaskManager::instance().execute(new UpdateUnreadOfIndexInDbTask(sql, _userid));
    MCE_DEBUG("MessageBase::ProcessMessageByIds --> uid:" << _userid 
      << " nid:" << *processed_it << " type:" << type << " bigtype:" 
      << _bigtype << " undeal:" << _undeal_count);
  }
  //CheckAndLoadFromDB(_messagelist.size());
  MCE_INFO("MessageBase::ProcessMessageByIds --> uid:" << _userid << " nids:" 
      << nids.size() << " bigtype:" << _bigtype << " undeal:" 
      << _undeal_count);
  return;
}

void MessageBase::RemoveById(Ice::Long nid, int type){
  set<Ice::Long> delnids;
  for(list<NotifyIndex>::iterator it = _messagelist.begin(); it != _messagelist.end(); ++it){
    if((*it).id == nid){
      _messagelist.erase(it);
      delnids.insert((*it).id);
      UpdateUnreadCount((*it).id);
      MCE_INFO("MessageBase::RemoveById uid[" << _userid << "]nid[" << nid 
          << "]bigtype[" << _bigtype << "]");
      break;
    }
  }
  UpdateUndealCount(type, 1);
  Statement sql;
  GetRemoveByIdSql(sql, nid);
  CheckAndLoadFromDB(_messagelist.size(), delnids);
}

bool MessageBase::IsExistInMemory(int64_t notify_id) {
  for (list<NotifyIndex>::iterator it = _messagelist.begin(); 
    it != _messagelist.end(); ++it) {
    if (notify_id == it->id) {
      return true;
    }
  }
  return false;
}

int MessageBase::GetTypeByNotifyId(int64_t notify_id) {
  int type = -1;
  for (list<NotifyIndex>::iterator it = _messagelist.begin(); 
    it != _messagelist.end(); ++it) {
    if (notify_id == it->id) {
      type = it->type;
    }
  }
  return type;
}

void MessageBase::AddMessage(const NotifyContentPtr& content){
  //add notify index
  NotifyIndex index;
  index.id = content->id;
  index.source = content->source;
  index.type = content->type;
  index.isreply = content->isreply;
  index.from = content->from;
  index.unread = 1;
  _messagelist.push_front(index);

  //add from front, new id bigger than old, so evic from back
  int messages_size = _messagelist.size();
  if (messages_size >= _evict_threshold_value) {
    _messagelist.pop_back();
    _isload = true;
    _load_from_db = true;
    MCE_INFO("MessageBase::AddMessage --> msg list is full then pop one, uid:" 
        << _userid << " bigtype:" << _bigtype << " threshold:" 
        << _evict_threshold_value);
  }
  if(index.id < _min_unread_id){
    _min_unread_id = index.id;
    MCE_DEBUG("MessageBase::AddMessage update min unread id, uid:" 
        << _userid << " bigtype:" << _bigtype << " min_unread_id:" 
        << _min_unread_id);
    TaskManager::instance().execute(new UpdateMinUnreadIdTask(_userid, _min_unread_id));
  }
  //update count
  ++_unread_count;
  ++_undeal_count;
  int stype = RMessageUtil::instance().Util_GetRMessageType(index.type);
  map<int, int>::iterator it = _type_undeal_count.find(stype);  
  if(it == _type_undeal_count.end()){
    _type_undeal_count.insert(make_pair<int,int>(stype, 0));
  }
  ++_type_undeal_count[stype];
  MCE_INFO("MessageBase::AddMessage --> uid:" << _userid 
      << " undeal:" << _undeal_count 
      << " unread:" << _unread_count 
      << " min:" << _min_unread_id 
      << " msgs:" << _messagelist.size() 
      << " nid:" << index.id 
      << " type:" << index.type  
      << " from:" << index.from  
      << " bigtype:" << _bigtype 
      << " msgsize:" << messages_size 
      << " loadstat:" << _load_stat 
      << " loadfromdb:" << _load_from_db 
      << " isload:" << _isload);
} 

void MessageBase::FirstLoad(){
  Statement sql;
  TimeStat time_cost;
  sql << "select id, type, source, from_id, is_reply, bigtype, unread from " << GetIndexTableName(_userid) 
      << " force index(idx_toid_bigtype_id) where to_id =" << _userid 
      << " and id < " << GetMinMessageId() <<" and bigtype = " << _bigtype 
      << " order by id desc limit " <<  _first_load_count; 
  mysqlpp::StoreQueryResult res = QueryRunner(GetDbInstance(), CDbRServer, 
      GetIndexTableName(_userid)).store(sql);
  if (res && res.num_rows() > 0) {
    if(res.num_rows() < (unsigned int)_first_load_count){
      _isload = false;
    }
    for (size_t i = 0; i < res.num_rows(); ++i) {
      NotifyIndex index;
      mysqlpp::Row row = res.at(i);
      index.id = (Ice::Long) row["id"];
      int bigtype = (int)row["bigtype"];
      int type = (Ice::Int) row["type"];
      type |= (bigtype << 16);
      index.type = type;
      index.source = (Ice::Long) row["source"];
      index.from = (Ice::Int) row["from_id"];
      index.isreply = (Ice::Int) row["is_reply"];
      index.unread = row["unread"];
      _messagelist.push_back(index);
    }
  }
  sql.clear();
  //get the MIN notify id of new messages
  sql << "select min_unread_nid from " << GetInfoTableName(_userid) 
      << " where id = " << _userid;
  res = QueryRunner(GetDbInstance(), CDbRServer, 
                    GetInfoTableName(_userid)).store(sql);
  if(res && res.num_rows() > 0){
    mysqlpp::Row row = res.at(0);
    _min_unread_id = (Ice::Long)row["min_unread_nid"];
    MCE_INFO("MessageBase::FirstLoad -->uid:" << _userid 
        << "  min_id_of_new:" << _min_unread_id);
  } else {
    _min_unread_id = LLONG_MAX;
  }
  int querycount = 0;
  Ice::Long min_message_id = LLONG_MAX; 
  while (true) {
    sql.clear();
    ostringstream os;
    sql << "select id, type, source, from_id, bigtype, unread from " 
        << GetIndexTableName(_userid) 
        << " force index(idx_toid_bigtype_id) where to_id =" << _userid 
        << " and id < " << min_message_id <<" and bigtype = " << _bigtype 
        << " order by id desc limit " <<  _first_load_count; 
    res = QueryRunner(GetDbInstance(), CDbRServer, GetIndexTableName(_userid)).store(sql);
    if (res && res.num_rows() > 0) {
      for (size_t i = 0; i < res.num_rows(); ++i) {
        NotifyIndex index;
        mysqlpp::Row row = res.at(i);
        index.id = (Ice::Long) row["id"];
        int bigtype = (int)row["bigtype"];
        int type = (Ice::Int) row["type"];
        index.type = type | (bigtype << 16);
        index.source = (Ice::Long) row["source"];
        index.from = (Ice::Int) row["from_id"];
        index.unread = row["unread"];
        ++_undeal_count;
        int stype = RMessageUtil::instance().Util_GetRMessageType(type);
        if(_type_undeal_count.find(stype) == _type_undeal_count.end()){
          _type_undeal_count.insert(make_pair<int, int>(stype, 0));
        }
        ++_type_undeal_count[stype];
        if(index.id < min_message_id){
          min_message_id = index.id;
          //MCE_DEBUG("MessageBase::FirstLoad uid[" << _userid 
          //    << "] update min_message_id[" << min_message_id << "]bigtype["
          //    << _bigtype << "]query_count[" << querycount << "]");
        }
        if(index.id >= _min_unread_id){
          _unread_count++;
          //MCE_DEBUG("MessageBase::FirstLoad update uid[" << _userid 
          //    << "]unread[" << _unread_count << "]nid[" << index.id 
          //    << "]min unread nid[" << _min_unread_id << "]bigtype[" << _bigtype 
          //    << "]query count[" << querycount << "]");
        }
        querycount++;
      }
    }
    if((int)(res.num_rows()) < _first_load_count || querycount >= 600){
      break;
    }
    if(!res){
      break;
    }
  }
  int msg_size = _messagelist.size();
  if(msg_size >= _evict_threshold_value){
    NotifyIndexManagerI::instance().UserRMessageEvict(_userid);
    MCE_INFO("MessageBase::FirstLoad Evict uid:" << _userid << " isload:" 
        << _isload << " bigtype:" << _bigtype << " unread:" << _unread_count 
        <<  " undeal:" << _undeal_count << " msg_size:" << msg_size << " cost:"
        << time_cost.getTime());
  } else {
    MCE_INFO("MessageBase::FirstLoad uid:" << _userid << " isload:" << _isload 
        << " bigtype:" << _bigtype << " unread:" << _unread_count 
        <<  " undeal:" << _undeal_count << " msg_size:" << msg_size << " cost:"
        << time_cost.getTime());
  }
}

void MessageBase::PreLoad(){
}

void UserMessagePool::AddUserMessage(const UserMessagePtr& usermessage){
  std::pair<UserMessageCache::iterator, bool> p = _cache.push_front(usermessage);
  if(!p.second){
    MCE_INFO("UserMessagePool::AddUserMessage add err same userid exist"
        << " and replace and reloate, uid[" << usermessage->_userid << "]");
    _cache.replace(p.first, usermessage);
    _cache.relocate(_cache.begin(), p.first);
  }else if((int)(_cache.size()) > CACHESIZE) {
    _cache.pop_back();
    MCE_INFO("UserMessagePool::AddUserMessage pop_back a user, uid[" 
      << usermessage->_userid << "]cache_size[" << CACHESIZE << "]");
  }
}


UserMessagePtr UserMessagePool::FindUserMessage(int userid){
  UserMessagePtr ans;
  IceUtil::Mutex::Lock lock(_cachemutex);
  UserIdIndex& id_index = _cache.get<1>();
  UserIdIndex::iterator idit = id_index.find(userid);
  if(idit != id_index.end()){
    ans = (*idit);
    SequencedIndex& seq_index = _cache.get<0>();
    _cache.relocate(seq_index.begin(), _cache.project<0>(idit));
  }
  return ans;
}

UserMessagePtr UserMessagePool::LocateUserMessage(int userid){
  UserMessagePtr ans;
  {
    IceUtil::Mutex::Lock lock(_cachemutex);
    UserIdIndex& id_index = _cache.get<1>();
    UserIdIndex::iterator idit = id_index.find(userid);
    if(idit != id_index.end()){
      ans = (*idit);
      SequencedIndex& seq_index = _cache.get<0>();
      _cache.relocate(seq_index.begin(), _cache.project<0>(idit));
    }
  }
  if(!ans){
    ans = new UserMessage(userid);
    if(ans){
      ans->FirstLoad();
      IceUtil::Mutex::Lock lock(_cachemutex);
      UserIdIndex& id_index = _cache.get<1>();
      UserIdIndex::iterator idit = id_index.find(userid);
      if(idit == id_index.end()){
        AddUserMessage(ans);
      } else{
        ans = (*idit);
      }
    }
  }
  return ans;
}
//==================================================================================================================

void UserMessagePool::UpdateUserBigtypeLoadStat(int userid, int bigtype, bool isload){
  UserMessagePtr usermessage = FindUserMessage(userid);
  if(usermessage){
    IceUtil::RWRecMutex::WLock lock(_rwmutex);
    usermessage->UpdateUserBigtypeLoadStat(bigtype, isload);
  }
}

void UserMessagePool::AppendMessages(const list<NotifyIndex>& seq, int userid, int bigtype, bool update_load_stat){
  UserMessagePtr usermessage = FindUserMessage(userid);
  if(usermessage){
    IceUtil::RWRecMutex::WLock lock(_rwmutex);
    usermessage->AppendMessages(seq, bigtype, update_load_stat);
  }
}

int UserMessagePool::GetBigTypeById(int user_id, int64_t notify_id) {
  UserMessagePtr user_msg_ptr = FindUserMessage(user_id);
  int big_type = -1;
  if (user_msg_ptr) {
      IceUtil::RWRecMutex::RLock lock(_rwmutex);
      big_type = user_msg_ptr->GetBigTypeByNotifyId(notify_id);
      MCE_DEBUG("UserMessagePool::GetBigTypeById get bigtype:" << big_type
          << "notify_id:" << notify_id);
      if (big_type < 0) {
        MCE_WARN("GetBigTypeById:Not found in memory. notify_id:" << notify_id)
      }
  }
  return big_type;
}

int UserMessagePool::GetTypeById(int user_id, int64_t notify_id) {
  UserMessagePtr user_msg_ptr = FindUserMessage(user_id);
  int type = -1;
  if (user_msg_ptr) {
    IceUtil::RWRecMutex::RLock lock(_rwmutex);
    type = user_msg_ptr->GetTypeByNotifyId(notify_id);
    MCE_DEBUG("UserMessagePool::GetTypeById get type:" << type
      << "notify_id:" << notify_id);
    if (type < 0) {
      MCE_WARN("GetTypeById:Not found in memory. notify_id:" << notify_id);
    }
  }
  return type;
}

void UserMessagePool::RemoveByIdNoType(int user_id, int64_t notify_id) {
  UserMessagePtr user_msg_ptr = FindUserMessage(user_id);
  if (user_msg_ptr) {
    IceUtil::RWRecMutex::WLock lock(_rwmutex);
    int type = user_msg_ptr->GetTypeByNotifyId(notify_id);
    if (type < 0) {
      MCE_WARN("UserMessagePool::GetTypeById:Not found noitfy in cache. "
          << " uid=" << user_id << " nid=" << notify_id);
      return;
    }
    int big_type = RMessageUtil::instance().Util_GetRMessageBigtype(type);
    int small_type = RMessageUtil::instance().Util_GetRMessageType(type);
    MCE_INFO("UserMessagePool::RemoveByIdNoType uid=" << user_id << " nid=" 
        << notify_id << " big_type=" << big_type << " small_type=" << small_type);
    user_msg_ptr->RemoveById(notify_id, small_type, big_type);
  } else {
    MCE_DEBUG("UserMessagePool::RemoveByIdNoType Not found user in cache. uid=" 
        << user_id <<  " nid=" << notify_id);
  }
}

void UserMessagePool::AddMessage(const NotifyContentPtr& content, const MyUtil::IntSeq& target){
  MCE_DEBUG("UserMessagePool::AddMessage nid[" << content->id << "]type[" 
    << content->type << "]");
  for(MyUtil::IntSeq::const_iterator it = target.begin(); it != target.end(); ++it){
    UserMessagePtr usermessage = LocateUserMessage(*it);
    if(usermessage){
      IceUtil::RWRecMutex::WLock lock(_rwmutex);
      usermessage->AddMessage(content);
    }
  }
}

void UserMessagePool::ProcessMessageById(int uid, int64_t nid, 
    int type, int bigtype) {
  UserMessagePtr usermessage = FindUserMessage(uid);
  if (usermessage) {
    IceUtil::RWRecMutex::WLock lock(_rwmutex);
    usermessage->ProcessMessageById(nid, type, bigtype);
    MCE_INFO("UserMessagePool::ProcessMessageById uid:" << uid 
        << " nid:" << nid << " type:" << type << " bigtype:" 
        << bigtype);
  }
}

void UserMessagePool::ProcessMessageByIds(int uid, 
    const MyUtil::LongSeq& nids, int type, int bigtype) {
  UserMessagePtr usermessage = FindUserMessage(uid);
  if (usermessage) {
    IceUtil::RWRecMutex::WLock lock(_rwmutex);
    MCE_INFO("UserMessagePool::ProcessMessageByIds uid:" << uid 
        << " nids:" << nids.size()  << " type:" << type 
        << " bigtype:" << bigtype);
    usermessage->ProcessMessageByIds(nids, type, bigtype);
  }
}

void UserMessagePool::RemoveById(int userid, Ice::Long notifyid, int type, int b){
  int bigtype = SchemaManager::instance().GetBigtypeBytype(type);
  UserMessagePtr usermessage = FindUserMessage(userid);
  if(usermessage){
    IceUtil::RWRecMutex::WLock lock(_rwmutex);
    MCE_INFO("UserMessagePool::RemoveById uid[" << userid << "]nid[" 
        << notifyid << "]bigtype[" << bigtype << "]type[" << type << "]");
    usermessage->RemoveById(notifyid, type, bigtype);
  }
}

void UserMessagePool::RemoveByIds(int userid, const MyUtil::LongSeq& nids, int type, int b){
  int bigtype = SchemaManager::instance().GetBigtypeBytype(type);
  UserMessagePtr usermessage = FindUserMessage(userid);
  if(usermessage){
    IceUtil::RWRecMutex::WLock lock(_rwmutex);
    usermessage->RemoveByIds(nids, type, bigtype);
    MCE_DEBUG("UserMessagePool::RemoveByIds uid[" << userid << "]");
  }
}

void UserMessagePool::RemoveBySource(Ice::Int userid, Ice::Int type, Ice::Long source, int b){
  int bigtype = SchemaManager::instance().GetBigtypeBytype(type);
  UserMessagePtr usermessage = FindUserMessage(userid);
  if(usermessage){
    IceUtil::RWRecMutex::WLock lock(_rwmutex);
    usermessage->RemoveBySource(type, source, bigtype);
  }
}

void UserMessagePool::RemoveAllByBigtype(Ice::Int uid, Ice::Int bigtype){
  UserMessagePtr user_message = FindUserMessage(uid);
  if(user_message){
    IceUtil::RWRecMutex::WLock lock(_rwmutex);
    user_message->RemoveAllByBigtype(bigtype);
  }
}

void UserMessagePool::ReadAllByBigtype(Ice::Int uid, Ice::Int bigtype) {
  UserMessagePtr user_message = FindUserMessage(uid);
  if(user_message){
    IceUtil::RWRecMutex::WLock lock(_rwmutex);
    user_message->ReadAllByBigtype(bigtype);
  }
}

void UserMessagePool::RemoveRepeatedByBigtype(Ice::Int uid, Ice::Int bigtype){
  UserMessagePtr user_message = FindUserMessage(uid);
  if (user_message) {
    IceUtil::RWRecMutex::WLock lock(_rwmutex);
    user_message->RemoveRepeatedByBigtype(bigtype);
  } else {
    MCE_WARN("UserMessagePool::RemoveRepeatedByBigtype --> not found user uid:" 
      << uid << " bigtype:" << bigtype);
  }
}

void UserMessagePool::RemoveBySourceAndSender(Ice::Int userid, Ice::Int type, Ice::Long source, Ice::Int sender, int b){
  int bigtype = SchemaManager::instance().GetBigtypeBytype(type);
  UserMessagePtr usermessage = FindUserMessage(userid);
  if(usermessage){
    IceUtil::RWRecMutex::WLock lock(_rwmutex);
    usermessage->RemoveBySourceAndSender(type, source, sender, bigtype);
  }
}

/*
NotifyIndexSeq UserMessagePool::GetRemindMessages(int uid, 
    int begin, int limit) { 
  NotifyIndexSeq ans;
  UserMessagePtr user_message = LocateUserMessage(uid);
  if (user_message) {
    IceUtil::RWRecMutex::RLock lock(_rwmutex);
    ans = user_message->GetRemindMessages(begin, limit);
  }
  return ans;
}
*/

NotifyIndexSeq UserMessagePool::GetSeqByBigtype(int userid, int begin, int limit, int bigtype){
  NotifyIndexSeq ans;
  UserMessagePtr usermessage = LocateUserMessage(userid);
  if(usermessage){
    IceUtil::RWRecMutex::RLock lock(_rwmutex);
    ans = usermessage->GetSeqByBigtype(begin, limit, bigtype);
  }
  return ans;
}

NotifyIndexSeq UserMessagePool::GetSeqByTypeAndBigtype(int userid, const MyUtil::IntSeq& types, int begin, int limit, int bigtype){
  NotifyIndexSeq ans;
  UserMessagePtr usermessage = LocateUserMessage(userid);
  if(usermessage){
    IceUtil::RWRecMutex::RLock lock(_rwmutex);
    ans = usermessage->GetSeqByTypeAndBigtype(userid, types, begin, limit, bigtype);
  }
  ostringstream os;
  os << "]para types[";
  for(MyUtil::IntSeq::const_iterator tit = types.begin(); tit != types.end(); ++tit){
    os << (*tit) << ", ";
  }
  MCE_INFO("UserMessagePool::GetSeqByTypeAndBigtype uid[" << userid 
      << "]types size[" << types.size()<< "]bigtype[" << bigtype 
      << "]ret size[" << ans.size() << os.str() << "]");
  return ans;
}

NotifyIndexSeqMap UserMessagePool::GetSeqByTypeAndBigtypeAll(int userid, const MyUtil::IntSeq& types,int begin, int limit, int bigtype){
  NotifyIndexSeqMap ans;
  return ans;
}

NotifyIndexSeq UserMessagePool::GetSeqBeforLastNid(int userid, int view, Ice::Long nid, int limit, int bigtype){
  NotifyIndexSeq ans;
  UserMessagePtr usermessage = LocateUserMessage(userid);
  if(usermessage){
    IceUtil::RWRecMutex::RLock lock(_rwmutex);
    ans = usermessage->GetSeqBeforLastNid(view, nid, limit, bigtype);
  }
  MCE_INFO("UserMessagePool::GetSeqBeforLastNid --> uid:" << userid 
      << " view:" << view 
      << " limit:" << limit 
      << " bigtype:" << bigtype 
      << " ans.size:" 
      << ans.size());
  return ans;
}

NotifyIndexSeq UserMessagePool::GetSeqBeforMerge(int userid, int view, Ice::Long nid, int limit, int bigtype){
  NotifyIndexSeq ans;
  UserMessagePtr usermessage = LocateUserMessage(userid);
  if(usermessage){
    IceUtil::RWRecMutex::RLock lock(_rwmutex);
    ans = usermessage->GetSeqBeforMerge(view, nid, limit, bigtype);
  }
  MCE_INFO("UserMessagePool::GetSeqBeforMerge --> uid:" << userid 
      << " view:" << view 
      << " nid:" << nid 
      << " limit:" << limit 
      << " bigtype:" << bigtype 
      << " ans_size:" << ans.size());
  return ans;
}


int UserMessagePool::GetUnreadCountByBigtype(int userid, int bigtype){
  int ans = 0;
  UserMessagePtr usermessage = LocateUserMessage(userid);
  if(usermessage){
    IceUtil::RWRecMutex::RLock lock(_rwmutex);
    return usermessage->GetUnreadCountByBigtype(bigtype);
  }
  return ans;

}

int UserMessagePool::GetUndealCountByBigtype(int userid, int bigtype){
  int ans = 0;
  UserMessagePtr usermessage = LocateUserMessage(userid);
  if(usermessage){
    IceUtil::RWRecMutex::RLock lock(_rwmutex);
    return usermessage->GetUndealCountByBigtype(bigtype);
  }
  return ans;
}
int UserMessagePool::GetUndealCountByType(int userid, int type, int bigtype){
  int ans = 0;
  UserMessagePtr usermessage = LocateUserMessage(userid);
  if(usermessage){
    IceUtil::RWRecMutex::RLock lock(_rwmutex);
    return usermessage->GetUndealCountByType(type, bigtype);
  }
  return ans;
}

//=========================================================================================

void UserMessagePool::UserMessageEvict(int userid){
  IceUtil::Mutex::Lock lock(_cachemutex);
  map<int, int>::iterator it = _user_time.find(userid);
  if(it == _user_time.end()){
    _user_time.insert(make_pair<int, int>(userid, time(NULL))); 
  }else{
    it->second = time(NULL);
  }
  MCE_INFO("UserMessagePool::UserMessageEvict --> uid:" << userid);
}

void UserMessagePool::DoEvict(){
  int userssize = 0, evictcount = 0, leftcount = 0;
  map<int,int> user_time;
  {
    IceUtil::Mutex::Lock lock(_cachemutex);
    user_time.swap(_user_time);
  }
  //if(user_time.empty()){
  //  return;
  //}
  userssize = user_time.size();
  for(map<int, int>::iterator it = user_time.begin(); it != user_time.end();){
    //if(time(NULL) - it->second >= 10 * 60){
    if(time(NULL) - it->second >= 60 * 60){
      int userid = it->first;
      UserMessagePtr usermessage = FindUserMessage(userid);
      if(usermessage){
        IceUtil::RWRecMutex::RLock lock(_rwmutex);
        usermessage->Evict();
        evictcount++;
      }
      user_time.erase(it++);
    }else{
      ++it;
    }
  }
  {
    IceUtil::Mutex::Lock lock(_cachemutex);
    _user_time.swap(user_time);
    leftcount = user_time.size();
  }
  //MCE_INFO("UserMessagePool::DoEvict-->evictusers.size:" << userssize << " evictcount:" << evictcount << " leftcount:" << leftcount);
}

void LoadMessageTask::handle(){
  Statement sql;
  list<NotifyIndex> seq;
  MCE_INFO("LoadMessageTask::handle uid:" << _userid << " bigtype:" 
      << _bigtype << " min_message_id:" << _min_message_id << " loadcount:" 
      << _loadcount);
  ostringstream oss;
  map< pair<int, long>, int > merge;
  while(true){
    sql.clear();
    oss.str("");
    oss << "select id, type, source, from_id, is_reply, bigtype from " 
        << GetIndexTableName(_userid) << " force index(idx_toid_bigtype_id) where to_id =" 
        << _userid << " and id < " << _min_message_id << " and bigtype = " 
        << _bigtype << " and unread = 1 order by id desc limit " <<  _loadcount; 
    //MCE_INFO("LoadMessageTask: " << oss.str());
    //sql << "select id, type, source, from_id ,bigtype from " << GetIndexTableName(_userid) << " where to_id =" << _userid << " and id < " << _min_message_id <<" and type in ( " << oss.str() << ") and ( id < 13903218553 or id > 13915001511 ) order by id desc limit " <<  _loadcount; 
    sql << oss.str();
    mysqlpp::StoreQueryResult res = QueryRunner(GetDbInstance(), CDbRServer, GetIndexTableName(_userid)).store(sql);
    if (res && res.num_rows() > 0) {
      for (size_t i = 0; i < res.num_rows(); ++i) {
        NotifyIndex index; 
        mysqlpp::Row row = res.at(i);
        index.id = (Ice::Long) row["id"];
        if(_min_message_id > index.id){
          _min_message_id = index.id;
        }
        if(_blacklist_nids.find(index.id) != _blacklist_nids.end()){
          MCE_INFO("LoadMessageTask id[" << index.id << "] in blacklist!");
          continue;
        }
        int bigtype = (int)row["bigtype"];
        int type = (int)row["type"];
        type |= (bigtype << 16);
        index.type = type;
        index.source = (Ice::Long) row["source"];
        index.from = (Ice::Int) row["from_id"];
        index.isreply = (Ice::Int) row["is_reply"];
        merge[make_pair<int, long>((int)row["type"], index.source)] += 1;
        seq.push_back(index);
      } 
    }
    if(!res){
      break;
    }
    if(res && res.num_rows() < (unsigned int)_loadcount){
      NotifyIndexManagerI::instance().UpdateUserBigtypeLoadStat(_userid, _bigtype, false);
      break;
    }
    if((int)seq.size() >= _loadcount){
      break;
    }
  }
  NotifyIndexManagerI::instance().AppendUserRMessages(_userid, _bigtype, seq, true);
}

void MessageEvictTimer::handle(){
  NotifyIndexManagerI::instance().UserRMessageDoEvict();
}

void UpdateMinUnreadIdTask::handle(){
  try {
    Statement sql;
    sql << "INSERT INTO " << GetInfoTableName(_userid) 
        << "(id, min_unread_nid)  VALUES (" << _userid << ", " 
        << _min_unread_id << ") ON DUPLICATE KEY UPDATE min_unread_nid=" << _min_unread_id;
    mysqlpp::StoreQueryResult res = QueryRunner(GetDbInstance(), CDbWServer, GetInfoTableName(_userid)).store(sql);
  } catch (std::exception& e) {
    MCE_WARN("UpdateMinUnreadIdTask::handle --> uid:" << _userid 
        << " exception:" << e.what());
  } catch (...) {
    MCE_WARN("UpdateMinUnreadIdTask::handle --> uid:" << _userid);
  }
}

