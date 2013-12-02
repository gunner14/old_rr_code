#include <boost/lexical_cast.hpp>
#include "NotifyIndexNAdapter.h"
//#include "OceCxxAdapter/src/NotifyContentAdapter.h"
#include "OceCxxAdapter/src/NotifyContentLoaderAdapter.h"
#include "quicklz.h"
#include "Notify/util/rmessage_util.h"
#include "Notify/util/PrintUtil.h"

using namespace xce::notify;

/*
// TODO : code review
NotifyBody::NotifyBody(NotifyContentPtr notify) : id(notify->id), source(notify->source), owner(notify->owner), from(
      notify->from), type(notify->type & 0x7FFFFFFF), time(notify->time), bigtype(GetRMessageBigtype(notify->type)) {
    //important!! props 中key-value的分隔符是 \t
    //解压缩
    string propsStr;
    //MCE_DEBUG("NotifyBody::NotifyBody --> type="<<notify->type << " nid:" << id);
    if ((notify->type & 0x80000000) == 0x80000000) {
      //if (false) {
      //MCE_DEBUG("NotifyBody::NotifyBody --> decompress");
      char* scratch = new char[QLZ_SCRATCH_DECOMPRESS];
      size_t dataLen = qlz_size_decompressed(notify->props.data());
      MCE_DEBUG("dataLen = " << dataLen);
      char* decompressed = new char[dataLen + 2];
      qlz_decompress(notify->props.data(), decompressed, scratch);
      decompressed[dataLen] = '\t';
      decompressed[dataLen + 1] = '\0';
      propsStr = decompressed;
      delete scratch;
      delete decompressed;
    } else {
      MCE_DEBUG("NotifyBody::NotifyBody --> NO decompress");
      propsStr.swap(notify->props);
    }

    //MCE_DEBUG("NotifyBody::NotifyBody --> props="<<propsStr);


    //get real type for rmessage
    type = GetRMessageType(type);

    size_t pos = 0;
    size_t tail = 0;
    string key;
    string val;

    while (pos < propsStr.length()) {
      tail = propsStr.find('\t', pos);
      if (tail == string::npos) {
        break;
      }
      key = propsStr.substr(pos, tail - pos);
      pos = tail + 1;

      tail = propsStr.find('\t', pos);
      if (tail == string::npos) {
        tail = propsStr.length();
      }
      val = propsStr.substr(pos, tail - pos);
      pos = tail + 1;
      props[key] = val;
    }
    props["bigtype"] = boost::lexical_cast<string>(bigtype);
    }
*/

// TODO : 并行调用service
void NotifyIndexNAdapter::addNotify(const NotifyContentPtr& r,
    const MyUtil::IntSeq& uids) {
  map<int, MyUtil::IntSeq> uidMap;
  for (size_t i = 0; i < uids.size(); ++i) {
    uidMap[uids.at(i) % cluster()].push_back(uids.at(i));
    MCE_DEBUG("NotifyIndexNAdapter::addNotify --> i:" << i << " uid:" << uids.at(i));
  }
  for (map<int, MyUtil::IntSeq>::iterator it = uidMap.begin(); it
      != uidMap.end(); ++it) {
    MCE_DEBUG("NotifyIndexNAdapter::addNotify --> N:" << it->first);
    getManagerOneway(it->first)->addNotify(r, it->second);
  }
}
/*
Ice::Int NotifyIndexNAdapter::getUnreadCount(Ice::Int uid) {
  return getManager(uid)->getUnreadCount(uid);
}

Ice::Int NotifyIndexNAdapter::getUnreadCountByView(Ice::Int uid,int view) {
  return getManager(uid)->getUnreadCountByView(uid,view);
}

void NotifyIndexNAdapter::removeById(Ice::Int uid, Ice::Long notifyId) {
  getManagerOneway(uid)->removeById(uid, notifyId);
}

void NotifyIndexNAdapter::removeByIds(Ice::Int uid, const MyUtil::LongSeq& nids) {
  if (!nids.empty()) {
    getManagerOneway(uid)->removeByIds(uid, nids);
  }
}

void NotifyIndexNAdapter::removeBySource(Ice::Int uid, Ice::Int type,
    Ice::Long source) {
  getManagerOneway(uid)->removeBySource(uid, type, source);
}

void NotifyIndexNAdapter::removeBySourceAndSender(Ice::Int uid, Ice::Int type,
    Ice::Long source,Ice::Int sender) {
  getManagerOneway(uid)->removeBySourceAndSender(uid, type, source,sender);
}


void NotifyIndexNAdapter::conentSeq2BodySeq(const NotifyContentSeq & contentSeq,
    NotifyBodySeq& bodySeq) {
  for (size_t h = 0; h < contentSeq.size(); ++h) {
    bodySeq.push_back(new NotifyBody(contentSeq.at(h)));
  }
}

NotifyBodySeq NotifyIndexNAdapter::getNotifySeq(Ice::Int uid, Ice::Long nid,
    Ice::Int limit) {
  NotifyContentSeq contentSeq = getManager(uid)->getNotifySeq(uid, nid, limit);
  NotifyBodySeq bodySeq;

  conentSeq2BodySeq(contentSeq, bodySeq);

  return bodySeq;
}

NotifyBodySeq NotifyIndexNAdapter::getReplySeq(Ice::Int uid, Ice::Long nid,
    Ice::Int limit) {
  NotifyContentSeq contentSeq = getManager(uid)->getReplySeq(uid, nid, limit);
  NotifyBodySeq bodySeq;
  conentSeq2BodySeq(contentSeq, bodySeq);
  return bodySeq;
}

NotifyBodySeq NotifyIndexNAdapter::getByType(Ice::Int uid,
    const MyUtil::IntSeq& type, Ice::Long nid, Ice::Int limit) {
  NotifyContentSeq contentSeq = getManager(uid)->getByType(uid, type, nid,
      limit);
  NotifyBodySeq bodySeq;
  conentSeq2BodySeq(contentSeq, bodySeq);
  return bodySeq;
}

NotifyBodySeq NotifyIndexNAdapter::getNotifySeq2(Ice::Int uid, Ice::Int begin,
    Ice::Int limit) {
  NotifyContentSeq contentSeq = getManager(uid)->getNotifySeq2(uid, begin,
      limit);
  NotifyBodySeq bodySeq;

  conentSeq2BodySeq(contentSeq, bodySeq);

  return bodySeq;
}

NotifyBodySeq NotifyIndexNAdapter::getReplySeq2(Ice::Int uid, Ice::Int begin,
    Ice::Int limit) {
  NotifyContentSeq contentSeq =
      getManager(uid)->getReplySeq2(uid, begin, limit);
  NotifyBodySeq bodySeq;
  conentSeq2BodySeq(contentSeq, bodySeq);
  return bodySeq;
}
*/

/*NotifyBodySeq NotifyIndexNAdapter::getNotifySeqMerge(Ice::Int uid, Ice::Long nid,
    Ice::Int limit) {
  NotifyContentSeq contentSeq = getManager(uid)->getNotifySeqMerge(uid, nid, limit);
  NotifyBodySeq bodySeq;

  conentSeq2BodySeq(contentSeq, bodySeq);

  return bodySeq;
}

NotifyBodySeq NotifyIndexNAdapter::getReplySeqMerge(Ice::Int uid, Ice::Long nid,
    Ice::Int limit) {
  NotifyContentSeq contentSeq = getManager(uid)->getReplySeqMerge(uid, nid, limit);
  NotifyBodySeq bodySeq;
  conentSeq2BodySeq(contentSeq, bodySeq);
  return bodySeq;
}*/


/*
NotifyBodySeq NotifyIndexNAdapter::getNotifySeqX(Ice::Int uid, Ice::Int view,Ice::Long nid,
    Ice::Int limit) {
  NotifyContentSeq contentSeq = getManager(uid)->getNotifySeqX(uid,  view,nid, limit);
  NotifyBodySeq bodySeq;

  conentSeq2BodySeq(contentSeq, bodySeq);
  return bodySeq;
}

NotifyBodySeq NotifyIndexNAdapter::getReplySeqX(Ice::Int uid, Ice::Int view,Ice::Long nid,
    Ice::Int limit) {
  NotifyContentSeq contentSeq = getManager(uid)->getReplySeqX(uid,  view,nid, limit);
  NotifyBodySeq bodySeq;
  conentSeq2BodySeq(contentSeq, bodySeq);
  return bodySeq;
}
NotifyBodySeq NotifyIndexNAdapter::getByType2(Ice::Int uid,
    const MyUtil::IntSeq& type, Ice::Int begin, Ice::Int limit) {
  NotifyContentSeq contentSeq = getManager(uid)->getByType2(uid, type, begin,
      limit);
  NotifyBodySeq bodySeq;
  conentSeq2BodySeq(contentSeq, bodySeq);
  return bodySeq;
}
*/
//====================================================RMESSAGE================

bool NotifyIndexNAdapter::reloadSchema(){
  MCE_DEBUG("NotifyIndexNAdapter::reloadSchema. begin");
  for(size_t i = 0; i < cluster(); ++i)
  {
    MCE_DEBUG("NotifyIndexNAdapter::reloadSchema. reload service:" << i);
    getManager(i)->reloadSchema();
  }
  return true;
}

NotifyBodySeq NotifyIndexNAdapter::GetRemindContentByIndex(
    const NotifyIndexSeq& index_seq) {
  MyUtil::LongSeq nid_seq;
  NotifyBodySeq body_seq;
  NotifyContentSeq content_seq;
  NotifyContentDict contentDict;

  for(NotifyIndexSeq::const_iterator it = index_seq.begin(); 
      it != index_seq.end(); ++it){
    NotifyIndex index = *it;
    nid_seq.push_back(index.id);
  }
  try {
    //get content from  memcache 
    contentDict = NotifyContentLoaderAdapter::instance().GetNotifyContentDict(nid_seq);
  } catch (Ice::Exception& e) {
    MCE_WARN("NotifyIndexNAdapter::GetRemindContentByIndex --> " 
        << "GetNotifyContentDict ids:" << PrintHelper::Print(nid_seq) 
        << " exception:" << e);
  }
  NotifyContentDict::iterator content_it;
  for(NotifyIndexSeq::const_iterator it = index_seq.begin();
      it != index_seq.end(); ++it){
    Ice::Long id = it->id;
    content_it = contentDict.find(id);
    if (content_it != contentDict.end()) {
      if(!content_it->second){
        continue;
      }
      content_it->second->unread = it->unread;
      content_seq.push_back(content_it->second);
    }
  }
  for(NotifyContentSeq::iterator it = content_seq.begin(); 
      it != content_seq.end(); ++it) {
    NotifyContentPtr content = *it;
    NotifyBodyPtr body = new NotifyBody(*it);
    body_seq.push_back(body);

    ostringstream os;
    for(map<string, string>::iterator it = body->props.begin(); 
        it != body->props.end(); ++it){
      os << " key:" << it->first << " value:" << it->second << ", ";
    }
    MCE_DEBUG("NotifyIndexNAdapter::GetRemindContentByIndex id:" << body->id 
        << " source:" << body->source << " owner:" << body->owner << " from:" 
        << body->from << " type:" << body->type << " time:" << body->time << " unread:" 
        << body->unread << " props:" << os.str());
  }
  return body_seq;
}

NotifyBodySeq NotifyIndexNAdapter::GetNotifyBodyByNotifyIndex(const NotifyIndexSeq& indexseq, 
    map<Ice::Long, int>& not_in_db_nids){

  MyUtil::LongSeq indexids;
  map<Ice::Long, int> indexids2type;
  NotifyBodySeq bodyseq;
  NotifyContentSeq contents;
  NotifyContentDict contentDict;

  for(NotifyIndexSeq::const_iterator logit = indexseq.begin(); logit != indexseq.end(); ++logit){
    NotifyIndex index = *logit;
    int type = RMessageUtil::instance().Util_GetRMessageType(index.type);
    indexids2type[index.id] = type;
    indexids.push_back(index.id);
  }
  //GetContent
  try {
    //contentDict = NotifyContentAdapter::instance().getNotifyDict(indexids);
    //replace by the memcached client
    contentDict = NotifyContentLoaderAdapter::instance().GetNotifyContentDict(indexids);
  } catch (Ice::Exception& e) {
    MCE_WARN("NotifyIndexNAdapter::GetNotifyBodyByNotifyIndex --> getNotifyDict nids:" 
        << PrintHelper::Print(indexids) << " exception[" << e << "]");
  }
  NotifyContentPtr reply;
  NotifyContentDict::iterator iter;
  for(map<Ice::Long, int>::iterator idit = indexids2type.begin(); idit != indexids2type.end(); ++idit){
    Ice::Long id = idit->first;
    int type = idit->second;
    iter = contentDict.find(id);
    if (iter != contentDict.end()) {
      if(!iter->second){
        not_in_db_nids[id]= type;
        continue;
      }
      contents.push_back(iter->second);
    } else{
      not_in_db_nids[id]= type;
    }
  }
  for(NotifyContentSeq::iterator logit = contents.begin(); logit != contents.end(); ++logit){
    NotifyContentPtr content = *logit;
    NotifyBodyPtr body = new NotifyBody(*logit);
    bodyseq.push_back(body);

    ostringstream os;
    for(map<string, string>::iterator it = body->props.begin(); it != body->props.end(); ++it){
      os << " key:" << it->first << " value:" << it->second << ", ";
    }
    MCE_DEBUG("NotifyIndexNAdapter::GetNotifyBodyByNotifyIndex id:" << body->id 
        << " source:" << body->source << " owner:" << body->owner << " from :" 
        << body->from << " type:" << body->type << " time:" << body->time <<  " props:" << os.str());
  }
  return bodyseq;
}


NotifyBodySeq NotifyIndexNAdapter::GetNotifyBodyByNotifyIndex(const NotifyIndexSeq& indexseq, 
    vector<Ice::Long>& not_in_db_nids){
  
  MyUtil::LongSeq indexids;
  NotifyBodySeq bodyseq;
  NotifyContentSeq contents;
  NotifyContentDict contentDict;

  for(NotifyIndexSeq::const_iterator logit = indexseq.begin(); logit != indexseq.end(); ++logit){
    NotifyIndex index = *logit;
    indexids.push_back(index.id);
  }
  //GetContent
  try {
    //contentDict = NotifyContentAdapter::instance().getNotifyDict(indexids);
    //replace by the memcached client
    contentDict = NotifyContentLoaderAdapter::instance().GetNotifyContentDict(indexids);
  } catch (Ice::Exception& e) {
    MCE_WARN("NotifyIndexNAdapter::GetNotifyBodyByNotifyIndex --> getNotifyDict nids:" 
        << PrintHelper::Print(indexids) << " exception[" << e << "]");
  }
  NotifyContentPtr reply;
  NotifyContentDict::iterator iter;
  //BOOST_FOREACH(Ice::Long id, indexids){
  for(vector<Ice::Long>::iterator idit = indexids.begin(); idit != indexids.end(); ++idit){
    Ice::Long id = *idit;
    iter = contentDict.find(id);
    if (iter != contentDict.end()) {
      if(!iter->second){
        not_in_db_nids.push_back(id);
        continue;
      }
      contents.push_back(iter->second);
    }
    else{
      not_in_db_nids.push_back(id);
    }
  }
  for(NotifyContentSeq::iterator logit = contents.begin(); logit != contents.end(); ++logit){
    NotifyContentPtr content = *logit;
    NotifyBodyPtr body = new NotifyBody(*logit);
    bodyseq.push_back(body);

    ostringstream os;
    for(map<string, string>::iterator it = body->props.begin(); it != body->props.end(); ++it){
      os << " key:" << it->first << " value:" << it->second << ", ";
    }
    MCE_DEBUG("NotifyIndexNAdapter::GetNotifyBodyByNotifyIndex id:" 
        << body->id << " source:" << body->source << " owner:" << body->owner 
        << " from :" << body->from << " type:" << body->type << " time:" 
        << body->time <<  " props:" << os.str());
  }
  return bodyseq;
}

/*
NotifyBodySeq NotifyIndexNAdapter::GetRemindMessage(int uid, int begin, int limit) {
  NotifyIndexSeq index_seq = getManager(uid)->GetRemindMessage(uid, begin, limit);
  NotifyBodySeq notify_body_seq  = GetRemindContentByIndex(index_seq);
  MCE_INFO("NotifyIndexNAdapter::GetRemindMessage --> uid:" << uid 
      << " nid_size:" << index_seq.size() << " body_size:" << notify_body_seq.size());
  return notify_body_seq;
}
*/

NotifyBodySeq NotifyIndexNAdapter::GetRMessageByBigtype(int uid, int begin, int limit, int bigtype){
  NotifyIndexSeq indexids = getManager(uid)->GetRMessageByBigtype(uid, begin, limit, bigtype);
  map<Ice::Long, int> should_remove_indexs;
  NotifyBodySeq ans = GetNotifyBodyByNotifyIndex(indexids, should_remove_indexs);
  MCE_INFO("NotifyIndexNAdapter::GetRMessageByBigtype --> uid:" << uid 
      << " nids:" << indexids.size() << " bodys:" << ans.size()
      <<  " should_remove_indexs:" << should_remove_indexs.size());
//  for(map<Ice::Long, int>::iterator rit = should_remove_indexs.begin(); rit != should_remove_indexs.end(); ++rit){
//    RemoveRMessageById(uid, rit->first, rit->second, 1);
//  }
  //RemoveRMessageByIds(uid, should_remove_indexs, bigtype);
  return ans;
}

NotifyBodySeq NotifyIndexNAdapter::GetRMessageBeforLastNid(int userid, int view, Ice::Long nid, int limit, int bigtype){
  NotifyIndexSeq indexids = getManager(userid)->GetRMessageBeforLastNid(userid, view, nid, limit,bigtype);  
  map<Ice::Long, int> should_remove_indexs;
  NotifyBodySeq ans = GetNotifyBodyByNotifyIndex(indexids, should_remove_indexs);
//  for(map<Ice::Long, int>::iterator rit = should_remove_indexs.begin(); rit != should_remove_indexs.end(); ++rit){
//    RemoveRMessageById(userid, rit->first, rit->second, 1);
//  }
  //RemoveRMessageByIds(userid, should_remove_indexs, bigtype);
  return ans;
}
NotifyBodySeq NotifyIndexNAdapter::GetRMessageMerge(int userid, int view, Ice::Long nid, int limit, int bigtype){
  NotifyIndexSeq indexids = getManager(userid)->GetRMessageMerge(userid, view, nid, limit, bigtype);  
  //vector<Ice::Long> should_remove_indexs;
  map<Ice::Long, int> should_remove_indexs;
  NotifyBodySeq ans = GetNotifyBodyByNotifyIndex(indexids, should_remove_indexs);
//  for(map<Ice::Long, int>::iterator rit = should_remove_indexs.begin(); rit != should_remove_indexs.end(); ++rit){
//    RemoveRMessageById(userid, rit->first, rit->second, 1);
//  }
  //RemoveRMessageByIds(userid, should_remove_indexs, bigtype);
  return ans;
}
NotifyBodySeq NotifyIndexNAdapter::GetRMessageByType(int userid, const MyUtil::IntSeq& types, int begin, int limit, int bigtype){
  NotifyIndexSeq indexids = getManager(userid)->GetRMessageByType(userid, types, begin, limit, bigtype);
  //vector<Ice::Long> should_remove_indexs;
  map<Ice::Long, int> should_remove_indexs;
  NotifyBodySeq ans = GetNotifyBodyByNotifyIndex(indexids, should_remove_indexs);
//  for(map<Ice::Long, int>::iterator rit = should_remove_indexs.begin(); rit != should_remove_indexs.end(); ++rit){
//    RemoveRMessageById(userid, rit->first, rit->second, 1);
//  }
  //RemoveRMessageByIds(userid, should_remove_indexs, bigtype);
  return ans;
}
NotifyBodySeqMap NotifyIndexNAdapter::GetRMessageByTypeAll(int userid, const MyUtil::IntSeq& types, int begin, int limit){
  NotifyBodySeqMap ans;

  NotifyIndexSeqMap indexids = getManager(userid)->GetRMessageByTypeAll(userid, types, begin, limit);
  //vector<Ice::Long> should_remove_indexs;
  map<Ice::Long, int> should_remove_indexs;
  for(NotifyIndexSeqMap::iterator it = indexids.begin(); it != indexids.end(); ++it){
    NotifyBodySeq oneans = GetNotifyBodyByNotifyIndex(it->second, should_remove_indexs);
    ans[it->first] = oneans;
  }
  //RemoveRMessageByIds(userid, should_remove_indexs, bigtype);
//  for(map<Ice::Long, int>::iterator rit = should_remove_indexs.begin(); rit != should_remove_indexs.end(); ++rit){
//    RemoveRMessageById(userid, rit->first, rit->second, 1);
//  }
  return ans;
}
int NotifyIndexNAdapter::GetUndealCountByBigtype(int uid, int bigtype){
  return getManager(uid)->GetUndealCountByBigtype(uid, bigtype);  
}
int NotifyIndexNAdapter::GetUndealCountByType(int uid, int type){
  return getManager(uid)->GetUndealCountByType(uid, type);  
}
MyUtil::Int2IntMap NotifyIndexNAdapter::GetUndealCountByTypes(int uid, const MyUtil::IntSeq& types){
  return getManager(uid)->GetUndealCountByTypes(uid, types);
}

void NotifyIndexNAdapter::TestGet1(int userid, int begin, int limit, int type, int view, int bigtype){
  if(userid % 10 == 2){
    bigtype = rand() % 3 + 1; 
    begin = rand() % 20 + 1;
    limit = rand() % 5 + 1;
    //if(userid % 100 == 1){
      MCE_INFO("NotifyIndexNAdapter::TestGet1 --> uid:" << userid << " begin:" 
          << begin << " limit:" << limit << " bigtype:" << bigtype);
      for(int i = 0; i < 5; ++i){
        getManager(2)->GetRMessageByBigtype(userid, begin, limit, bigtype);
        MyUtil::IntSeq types;
        types.push_back(14);
        types.push_back(15);
        getManager(2)->GetRMessageByType(userid, types, begin, limit, bigtype);
        getManager(2)->GetRMessageByTypeAll(userid, types, begin, limit);
        getManager(2)->GetUnreadCountByBigtype(userid, bigtype);
        MyUtil::IntSeq bigtypes;
        bigtypes.push_back(1);
        bigtypes.push_back(2);
        bigtypes.push_back(3);
        getManager(2)->GetUnreadCountByBigtypes(userid, bigtypes);
        getManager(2)->GetUndealCountByBigtype(userid, bigtype);
        getManager(2)->GetUndealCountByType(userid, bigtype);
        getManager(2)->GetUndealCountByTypes(userid, types);
      }
    //}
  }
}

int NotifyIndexNAdapter::GetUnreadCountByBigtype(int userid, int bigtype){
  return getManager(userid)->GetUnreadCountByBigtype(userid, bigtype);  
}

MyUtil::Int2IntMap NotifyIndexNAdapter::GetUnreadCountByBigtypes(int userid, const MyUtil::IntSeq& types){
  return getManager(userid)->GetUnreadCountByBigtypes(userid, types);
}
void NotifyIndexNAdapter::RemoveRMessageById(int uid, Ice::Long notifyId, int type, int bigtype){
  return getManagerOneway(uid)->RemoveRMessageById(uid, notifyId, type, bigtype);
}
void NotifyIndexNAdapter::RemoveRMessageByIds(int uid, 
    const MyUtil::LongSeq& notifyIds, 
    int type, int bigtype) {
  if(notifyIds.empty()){
    return;
  }
  MCE_DEBUG("NotifyIndexNAdapter::RemoveRMessageByIds uid[" << uid  << "]type[" 
      << type << "]bigtype[" << bigtype << "]");
  return getManagerOneway(uid)->RemoveRMessageByIds(uid, notifyIds, type, bigtype);
}

void NotifyIndexNAdapter::RemoveRMessageBySource(int uid, int type, Ice::Long source, int bigtype){
  return getManagerOneway(uid)->RemoveRMessageBySource(uid, type, source, bigtype);
}

void NotifyIndexNAdapter::RemoveRMessageByBigtype(int uid, int bigtype) {
  MCE_DEBUG("NotifyIndexNAdapter::RemoveRMessageByBigtype uid " << uid << " bigtype " << bigtype);
  return getManagerOneway(uid)->RemoveRMessageByBigtype(uid, bigtype);
}

void NotifyIndexNAdapter::RemoveRepeatedRMsgByBigtype(int uid, int bigtype) {
  MCE_DEBUG("NotifyIndexNAdapter::RemoveRepeatedRMsgByBigtyp --> uid:" << uid << " bigtype:" << bigtype)
  return getManagerOneway(uid)->RemoveRepeatedRMsgByBigtype(uid, bigtype);
}

void NotifyIndexNAdapter::RemoveRMessageBySourceSeq(int uid, int type, const MyUtil::LongSeq& sources, int bigtype){
  return getManagerOneway(uid)->RemoveRMessageBySourceSeq(uid, type, sources, bigtype);
}
void NotifyIndexNAdapter::RemoveRMessageBySourceAndSender(int uid, int type, Ice::Long source, int sender, int bigtype){
  return getManagerOneway(uid)->RemoveRMessageBySourceAndSender(uid, type, source, sender,bigtype);
}
//====================================================RMESSAGE================

int NotifyIndexNAdapter::GetBigTypeById(int userId, Ice::Long notifyId) {
  return getManagerOneway(userId)->GetBigTypeById(userId, notifyId);
}

void NotifyIndexNAdapter::RemoveRMessageByIdNoType(int userId, Ice::Long notifyId) {
  return getManagerOneway(userId)->RemoveRMessageByIdNoType(userId, notifyId);
}

void NotifyIndexNAdapter::ProcessMessageById(int uid, Ice::Long nid, int type, int bigtype){
  return getManagerOneway(uid)->ProcessMessageById(uid, nid, type, bigtype);
}

void NotifyIndexNAdapter::ProcessMessageByIds(int uid,
    const MyUtil::LongSeq& nids, int type, int bigtype) {
  return getManagerOneway(uid)->ProcessMessageByIds(uid, nids, type, bigtype);
}

void NotifyIndexNAdapter::ReadAllMessageByBigtype(int uid, int bigtype) {
  MCE_DEBUG("NotifyIndexNAdapter::ReadAllMessageByBigtype uid " << uid 
      << " bigtype " << bigtype);
  return getManagerOneway(uid)->ReadAllMessageByBigtype(uid, bigtype);
}
