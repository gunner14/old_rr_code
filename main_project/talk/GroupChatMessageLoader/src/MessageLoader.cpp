#include "MessageLoader.h"
#include "QueryRunner.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "DbOpe.h"
#include "XmppTools.h"
using namespace com::xiaonei::talk::util;
using namespace xce::xntalk::groupchat;
using namespace com::xiaonei::xce;

void MyUtil::initialize() {
	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&MessageLoaderManagerI::instance(), service.createIdentity("M", ""));

	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("Service.Mod");
	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service.Interval", 5);
	xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerGroupChatMessageLoader", &MessageLoaderManagerI::instance(), mod, interval, new XceFeedControllerChannel());
	MCE_INFO("MyUtil::initialize. mod:" << mod << " interval:" << interval);
}

bool MessageLoaderManagerI::LocateMessage(const Ice::Long sessionId, const MessageRequestPtr &request, MessageBodySeq &msg_seq, const bool is_offline) {
  //  create table message_body (msgid bigint(20) unsigned primary key, session_id bigint(20) unsigned not null, content text , source varchar(256), source_type int(10) unsigned, link varchar(256) , uid int(10) unsigned, attachment_flag int(10) unsigned, create_time timestamp default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP);
  vector<Ice::Long> msgids_attachment;
  ostringstream msg_sql;
  msg_sql << "select msgid, session_id, content, cotent_info, source, source_type, link, uid, attachment_flag, create_time from "
          << DbOpe::GetDbTable(DB_PREF_MSG, TABLE_COUNT, sessionId) <<"  where ";
  if (is_offline) {
    msg_sql << " msgid>" << request->msgid << " and ";
  } else if (request->msgid > 0) 
    msg_sql << " msgid<" << request->msgid << " and ";
  msg_sql << " session_id=" << sessionId << " order by msgid desc limit " << (request->limit+request->offset);


    mysqlpp::StoreQueryResult res;
    if (!DbOpe::SqlRead(DB_INSTANCE, DbOpe::GetDbRoute(DB_ROUTE_MSG, TABLE_COUNT, sessionId).c_str(), msg_sql.str().c_str(), res)) {
      MCE_WARN("MessageLoaderManagerI::LocateMessage --> load message error. session_id:" << sessionId 
               << "  msgid:" <<request->msgid <<"  size:"<<(request->limit+request->offset));
      return false;
    }

    for (int i=0; i<res.num_rows(); ++i) {
      if (i < request->offset)
        continue;
      mysqlpp::Row row = res.at(i);
      MessageBodyPtr msg_body = new MessageBody();
      msg_body->messageId = (Ice::Long)row["msgid"];
      msg_body->sessionId = (Ice::Long)row["session_id"];
      msg_body->content = (string)row["content"];
      msg_body->contentInfo = (string)row["content_info"];
      msg_body->source = (string)row["source"];
      msg_body->sourceType = (Ice::Long)row["source_type"];
      msg_body->link = (string)row["link"];
      msg_body->uid = (Ice::Long)row["uid"];
      msg_body->attachFlag = (Ice::Long)row["attachment_flag"];
      msg_body->createTime = (string)row["create_time"];
      msg_seq.push_back(msg_body);
      if (msg_body->attachFlag)
        msgids_attachment.push_back(msg_body->messageId);
    }


  if (msgids_attachment.size() > 0) {
    AttachmentBodySeq attach_seq;
    ostringstream attachment_sql;
    attachment_sql << "select msgid,session_id, type, attach_key, info from "
                   << DbOpe::GetDbTable(DB_PREF_ATTACH, TABLE_COUNT, sessionId)
                   << " where session_id=" << sessionId <<" and msgid in (";
    for (int j=0; j<msgids_attachment.size(); ++j) {
      attachment_sql << msgids_attachment.at(j);
      if (j != msgids_attachment.size()-1) 
        attachment_sql << ",";
      else
        attachment_sql << ")";
    } 
    if (!DbOpe::SqlRead(DB_INSTANCE, DbOpe::GetDbRoute(DB_ROUTE_ATTACH, TABLE_COUNT, sessionId).c_str(), attachment_sql.str().c_str(), res)) {
        MCE_WARN("MessageLoaderManagerI::LocateMessage --> load message error. session_id:" << sessionId
            << "  msgid:" <<request->msgid <<"  size:"<<(request->limit+request->offset) );

        return false;
      }

      for (int i=0; i<res.num_rows(); ++i) {
        mysqlpp::Row row = res.at(i);
        AttachmentBodyPtr attach_body = new AttachmentBody();
        attach_body->messageId = (Ice::Long)row["msgid"];
        attach_body->sessionId = (Ice::Long)row["session_id"];
        attach_body->type = (Ice::Long)row["type"];
        attach_body->attachKey = (Ice::Long)row["attach_key"];
        attach_body->info = (string)row["info"];
        attach_seq.push_back(attach_body);
      }

    for (AttachmentBodySeq::iterator attach_iter=attach_seq.begin(); attach_iter!=attach_seq.end(); ++attach_iter) {
      for (MessageBodySeq::iterator msg_iter=msg_seq.begin(); msg_iter!=msg_seq.end(); ++msg_iter) {
        if ((*attach_iter)->messageId == (*msg_iter)->messageId)
          ((*msg_iter)->attachments).push_back(*attach_iter);
      }
    }
  }
  if (!is_offline && msg_seq.size()>0 && request->msgid==0) //如果session在memcache中不存在，载入到memcache中；否则不需要载入 
    GroupChatMessageCacheClient::instance().SetMessages(msg_seq);
  return true;
}

MessageBodySeq MessageLoaderManagerI::LoadMessages(Ice::Long sessionId, const MessageRequestPtr &request,const bool is_offline, const ::Ice::Current& current) {
	TimeStatN ts;
  MessageBodySeq msg_seq; 
  bool rv = LocateMessage(sessionId, request, msg_seq, is_offline);
	FunStatManager::instance().StatPlus(current, "MessageLoaderManagerI::LoadMessages", ts.getTime(), (ts.getTime() > 200));
  if (rv) {
    return msg_seq;
  } else {
    MCE_WARN("MessageLoaderManagerI::LoadMessages  error to load msg.  sessionid:" << sessionId 
             << "  msgid:" <<request->msgid <<"  size:"<<(request->limit+request->offset));
    return MessageBodySeq();
  }
}

bool MessageLoaderManagerI::SetMessages_(const MessageBodySeq &msg_seq) {
  map<long, MessageBodySeq> mods;
  for (MessageBodySeq::const_iterator it = msg_seq.begin(); it != msg_seq.end(); ++it) {
    long m = (*it)->sessionId % TABLE_COUNT;
    if (mods.find(m) == mods.end()) {
      mods[m] = MessageBodySeq();
    }
    mods[m].push_back(*it);
  }
  MCE_INFO("MessageLoaderManagerI::SetMessages --> size:" << msg_seq.size() << " mod size:" << mods.size());
  for (map<long, MessageBodySeq>::const_iterator it = mods.begin(); it != mods.end(); ++it) {
    if (!SetMessagesOneMod(it->first, it->second)) {
      return false;
    }
  }

  return true;

}



bool MessageLoaderManagerI::SetMessagesOneMod(long mod, const MessageBodySeq &msg_seq) {
  MCE_INFO("MessageLoaderManagerI::SetMessagesOneMod --> mod:" << mod << " size:" << msg_seq.size());
  if (msg_seq.size() == 0) {
    return true;
  }
  MCE_DEBUG("MessageLoaderManagerI::SetMessages    msg_seq.size:" << msg_seq.size());
  AttachmentBodySeq attachments_seq;
  ostringstream msg_sql;
  msg_sql << "INSERT INTO "<< DbOpe::GetDbTable(DB_PREF_MSG, TABLE_COUNT, mod) <<" (msgid, session_id, content,content_info, source, source_type, link, uid, attachment_flag, create_time) VALUES ";
  size_t count = 0;
  for (MessageBodySeq::const_iterator mb_iter=msg_seq.begin(); mb_iter!=msg_seq.end(); ++mb_iter, ++count) {
    msg_sql << "("
            << (*mb_iter)->messageId
            << "," << (*mb_iter)->sessionId
            <<",'" << EscapeString((*mb_iter)->content) << "'," 
            <<"'" << EscapeString((*mb_iter)->contentInfo) << "','" 
            << EscapeString((*mb_iter)->source) << "',"
            << (*mb_iter)->sourceType
            <<",'" <<EscapeString((*mb_iter)->link) << "' ,"
            << (*mb_iter)->uid
            << "," << (*mb_iter)->attachFlag
            << ",'" << (*mb_iter)->createTime << "')"; 
    if (count < msg_seq.size() - 1) {
      msg_sql << ",";
    }

    attachments_seq.insert(attachments_seq.end(), (*mb_iter)->attachments.begin(), (*mb_iter)->attachments.end());
  }

  if (!DbOpe::SqlWrite(DB_INSTANCE, DbOpe::GetDbRoute(DB_ROUTE_MSG, TABLE_COUNT, mod).c_str(), msg_sql.str().c_str())) {
    MCE_WARN("MessageLoaderManagerI::SetMessages --> save msg dosql error");
    return false;
  }

  MCE_DEBUG("MessageLoaderManagerI::SetMessages    attachments_seq.size():" << attachments_seq.size());

  if (attachments_seq.size() > 0) {
    ostringstream attachment_sql;
    attachment_sql << "INSERT INTO " << DbOpe::GetDbTable(DB_PREF_ATTACH, TABLE_COUNT, mod) << "  (msgid, session_id, type, attach_key, info) VALUES ";
    count = 0;
    for (AttachmentBodySeq::const_iterator ab_iter=attachments_seq.begin(); ab_iter!=attachments_seq.end(); ++ab_iter, ++count) {
      attachment_sql << "(" << (*ab_iter)->messageId << "," << (*ab_iter)->sessionId << "," << (*ab_iter)->type <<"," << (*ab_iter)->attachKey 
                     <<"," << " \"" << (*ab_iter)->info << "\"" << ")";

      if (count < attachments_seq.size() - 1) {
        attachment_sql << ",";
      }

    }

    if (!DbOpe::SqlWrite(DB_INSTANCE, DbOpe::GetDbRoute(DB_ROUTE_ATTACH, TABLE_COUNT, mod).c_str(), attachment_sql.str().c_str())) {
      MCE_WARN("MessageLoaderManagerI::SetMessages --> save attach dosql error");
      return false;
    }

  }

  GroupChatMessageCacheClient::instance().SetMessages(msg_seq);

  return true;
}

string MessageLoaderManagerI::EscapeString(string &raw_string) {
  MCE_DEBUG("MessageLoaderManagerI::EscapeString   raw_stirng:" << raw_string);
  ostringstream os;
  for (size_t i=0; i<raw_string.length(); ++i) {
    if (raw_string[i]=='\'' ||raw_string[i]=='_' ||raw_string[i]=='%' ||raw_string[i]=='\"' ) {
      os << '\\';
    }
    os << raw_string[i];
  }
  MCE_DEBUG("MessageLoaderManagerI::EscapeString   raw_stirng_ret:" << os.str());
  return os.str();
}

bool MessageLoaderManagerI::SetMessages(const MessageBodySeq &msg_seq,  const ::Ice::Current& current) {
	TimeStatN ts;
  bool rv = SetMessages_(msg_seq);
	FunStatManager::instance().StatPlus(current, "MessageLoaderManagerI::SetMessages", ts.getTime(), (ts.getTime() > 200));
  return rv;
}

void MessageLoaderManagerI::SetMessagesAsyn(const  xce::xntalk::groupchat::MessageBodySeq &msg_seq,  const ::Ice::Current& current) {
	TimeStatN ts;
  SetMessages_(msg_seq);
	FunStatManager::instance().StatPlus(current, "MessageLoaderManagerI::SetMessagesAsyn", ts.getTime(), (ts.getTime() > 200));
}
