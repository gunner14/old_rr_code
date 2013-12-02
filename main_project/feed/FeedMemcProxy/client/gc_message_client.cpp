#include "gc_message_client.h"
#include "GroupChat.pb.h"
#include "GroupChatMessageLoaderAdapter.h"

#include "XmppTools.h"
using namespace std;
using namespace Ice;
using namespace xce::xntalk::groupchat;
using namespace com::xiaonei::talk::util;


long GetSessionId(const string &session_id) {
  if (session_id.substr(0,6) != "GCMSG#")
    return -1;
  Ice::Long id = -1;
  try {
    id = boost::lexical_cast<Ice::Long>(session_id.substr(6, session_id.length()-6));
  } catch(boost::bad_lexical_cast &) {
    return -1;
  }
  return id;
}

string GetSessionKey(const long session_id) {
  ostringstream os;
  if (session_id > 0)
    os << "GCMSG#" << session_id;
  return os.str();  
}

void GetMessageBodyFromMem(const SessionBody &mem_session_body, const MessageRequestPtr request, MessageBodySeq &seq) {
  if (!request) 
    return;
  const int offset = request->offset;
  const int message_size = mem_session_body.msg_body_size();
  for (int i=0; i<message_size; ++i) {
    if (request->msgid>0 && mem_session_body.msg_body(i).message_id()>request->msgid)
      continue;
    if (i < offset)
      continue;
    GroupChatMessageBody mem_msg_body = mem_session_body.msg_body(i);
    MessageBodyPtr msg_body = new MessageBody();
    msg_body->messageId = mem_msg_body.message_id();
    msg_body->sessionId = mem_msg_body.session_id();
    msg_body->content = mem_msg_body.content();
    msg_body->contentInfo = mem_msg_body.content_info();
    msg_body->source = mem_msg_body.source();
    msg_body->sourceType = mem_msg_body.source_type();
    msg_body->link = mem_msg_body.link();
    msg_body->uid = mem_msg_body.uid();
    msg_body->attachFlag = mem_msg_body.attach_flag();
    msg_body->createTime = mem_msg_body.create_time();
    for (int j=0; j<mem_msg_body.attachments_size(); ++j) {
      GroupChatMessageAttachment mem_attch_body = mem_msg_body.attachments(j);
      AttachmentBodyPtr attachment_body = new AttachmentBody();
      attachment_body->attachmentId = mem_attch_body.attachment_id();
      attachment_body->messageId = mem_attch_body.msgid();
      attachment_body->sessionId = mem_attch_body.session_id();
      attachment_body->type = mem_attch_body.type();
      attachment_body->attachKey = mem_attch_body.attach_key();
      attachment_body->info = mem_attch_body.info();
      msg_body->attachments.push_back(attachment_body);
    }
    seq.push_back(msg_body);
    request->msgid = msg_body->messageId;
    request->offset = 0;
    request->limit--; 
    if (request->limit == 0)
      break;
  }
}

void GetOfflineMessageBodyFromMem(const SessionBody &mem_session_body, const MessageRequestPtr request, MessageBodySeq &seq) {
  if (!request) 
    return;
  const int message_size = mem_session_body.msg_body_size();
  for (int i=0; i<message_size && i<request->limit; ++i) {
    MCE_DEBUG("GroupChatMessageCacheClient::GetOfflineMessagesFromMem   request->msgid:" << request->msgid <<"   memid:" <<mem_session_body.msg_body(i).message_id());
    if (mem_session_body.msg_body(i).message_id() <= request->msgid)
      break;
    GroupChatMessageBody mem_msg_body = mem_session_body.msg_body(i);
    MessageBodyPtr msg_body = new MessageBody();
    msg_body->messageId = mem_msg_body.message_id();
    msg_body->sessionId = mem_msg_body.session_id();
    msg_body->content = mem_msg_body.content();
    msg_body->contentInfo = mem_msg_body.content_info();
    msg_body->source = mem_msg_body.source();
    msg_body->sourceType = mem_msg_body.source_type();
    msg_body->link = mem_msg_body.link();
    msg_body->uid = mem_msg_body.uid();
    msg_body->attachFlag = mem_msg_body.attach_flag();
    msg_body->createTime = mem_msg_body.create_time();
    for (int j=0; j<mem_msg_body.attachments_size(); ++j) {
      GroupChatMessageAttachment mem_attch_body = mem_msg_body.attachments(j);
      AttachmentBodyPtr attachment_body = new AttachmentBody();
      attachment_body->attachmentId = mem_attch_body.attachment_id();
      attachment_body->messageId = mem_attch_body.msgid();
      attachment_body->sessionId = mem_attch_body.session_id();
      attachment_body->type = mem_attch_body.type();
      attachment_body->attachKey = mem_attch_body.attach_key();
      attachment_body->info = mem_attch_body.info();
      msg_body->attachments.push_back(attachment_body);
    }
    seq.push_back(msg_body);
  }
  MCE_DEBUG("GroupChatMessageCacheClient::GetOfflineMessagesFromMem  seq:" << seq.size());
}

void GetMessageBodyFromDB(long session_id, const MessageRequestPtr &request, MessageBodySeq &msg_seq, const bool is_offline) {
  MessageBodySeq temp_seq;
	try{
    GroupChatMessageLoaderAdapter::instance().LoadMessages(session_id, request, temp_seq, is_offline);
	} catch(std::exception& e){
		MCE_WARN("GetMessageBodyFromDB GroupChatMessageLoaderAdapter.instance().LoadMessages: e:" << e.what());
    return;
	} catch(...){
		MCE_WARN("GetMessageBodyFromDB GroupChatMessageLoaderAdapter.instance().LoadMessages");
    return;
	}
  msg_seq.insert(msg_seq.end(), temp_seq.begin(), temp_seq.end());
}

  
void UpdateMessages(const MessageBodySeq &msg_seq, MessageBodySeq  &mem_msg_seq) {
  /*
  int i, j;
  for (i=0; i<msg_seq.size()&&mem_msg_seq.size()<MESSAGES_MEM; ++i) {
    bool insert = true;
    for (j=0; j<mem_msg_seq.size(); ++j) {
      if (msg_seq.at(i)->messageId <= mem_msg_seq.at(j)->messageId)
        insert = false;
      else 
        insert = true;
      if (insert)
        break;
    }
    if (insert)
      mem_msg_seq.insert(mem_msg_seq.begin()+j, msg_seq.at(i));
  }
  */
  /*
  const int insert_size = MESSAGES_MEM - mem_msg_seq.size();
  if (insert_size > 0) 
    mem_msg_seq.insert(mem_msg_seq.end(), msg_seq.begin(), msg_seq.begin()+insert_size);
  */
  mem_msg_seq.insert(mem_msg_seq.begin(), msg_seq.begin(), msg_seq.end());
}

void SetMemFromMessageBody(GroupChatMessageBody *mem_msg_body, const MessageBodyPtr msg_body) {
  mem_msg_body->set_message_id(msg_body->messageId);
  mem_msg_body->set_session_id(msg_body->sessionId);
  mem_msg_body->set_content(msg_body->content);
  mem_msg_body->set_content_info(msg_body->contentInfo);
  mem_msg_body->set_source(msg_body->source);
  mem_msg_body->set_source_type(msg_body->sourceType);
  mem_msg_body->set_link(msg_body->link);
  mem_msg_body->set_uid(msg_body->uid);
  mem_msg_body->set_attach_flag(msg_body->attachFlag);
  mem_msg_body->set_create_time(msg_body->createTime);

  const AttachmentBodySeq &attseq = msg_body->attachments;
  for (AttachmentBodySeq::const_iterator it = attseq.begin(); it != attseq.end(); ++it) {
    GroupChatMessageAttachment *attach = mem_msg_body->add_attachments();
    attach->set_attachment_id((*it)->attachmentId);
    attach->set_msgid((*it)->messageId);
    attach->set_session_id((*it)->sessionId);
    attach->set_type((*it)->type);
    attach->set_attach_key((*it)->attachKey);
    attach->set_info((*it)->info);
  }
}

GroupChatMessageCacheClient::GroupChatMessageCacheClient() { 
  pool_.Init(100, "im"); 
}

GroupChatMessageCacheClient::~GroupChatMessageCacheClient() { 
}

// request. msgid 当前消息id, offset为0， limit 取离线消息的条数
MessageBodySeqMap GroupChatMessageCacheClient::GetOfflineMessages(xce::xntalk::groupchat::MessageRequestMap &requests) {
  MCE_DEBUG("GroupChatMessageCacheClient::GetOfflineMessages  requests.size:" << requests.size());
  MessageBodySeqMap dict;
  if (requests.empty())
    return dict;
  int first_session_id = 0;
  vector<string> session_ids;
  for (MessageRequestMap::const_iterator msg_req_iter=requests.begin(); msg_req_iter!=requests.end(); ++msg_req_iter) {
    char data[64] = {0};
    snprintf(data, 64, "GCMSG#%ld", msg_req_iter->first);
    session_ids.push_back(data);
    if (!first_session_id)
      first_session_id = msg_req_iter->first;
  }
  pair<int, MemcachedClient*> cli = pool_.GetClient(first_session_id);
  if (!cli.second) {
    MCE_WARN("GroupChatMessageCacheClient::GetOfflineMessages  pool_.GetClient error. first_session_id" << first_session_id 
        <<"   requests.size:" << requests.size());
    return dict;
  }
  map<string, string> res;
  map<string, int> flags;
  TimeStatN ts; 
  cli.second->Get(session_ids, res, flags);
  float cost_mem = ts.getTime();
  ts.reset();
  MCE_DEBUG("GroupChatMessageCacheClient::GetOfflineMessages   res:" << res.size());
  for (map<string, string>::iterator res_iter=res.begin(); res_iter!=res.end(); ++res_iter) {
    vector<string>::iterator del_iter = find(session_ids.begin(), session_ids.end(), res_iter->first);
    if (del_iter != session_ids.end()) {
      session_ids.erase(del_iter);
    }
    long session_id = GetSessionId(res_iter->first);
    if (session_id == -1) { 
      MCE_WARN("GroupChatMessageCacheClient::GetOfflineMessages   GetSessionId error.res_iter->first:" << res_iter->first);
      continue;
    }
    MessageBodySeq seq;
    SessionBody mem_session_body;
    if (!mem_session_body.ParseFromString(res_iter->second)) {
      MCE_WARN("GroupChatMessageCacheClient::GetOfflineMessages   ParseFromString error.  res_iter->second:" << res_iter->second);
      continue;
    }
    MessageRequestMap::const_iterator request_iter = requests.find(session_id); 
    if (request_iter == requests.end())
      continue;
    if (!(request_iter->second))
      continue;
    GetOfflineMessageBodyFromMem(mem_session_body, request_iter->second, seq);
    dict.insert(make_pair(session_id, seq));
  }
  if (!session_ids.empty()) {
    for (vector<string>::const_iterator load_iter=session_ids.begin(); load_iter!=session_ids.end(); ++load_iter) {
      const long session_id = GetSessionId(*load_iter);
      if (session_id == -1) { 
        MCE_WARN("GroupChatMessageCacheClient::GetOfflineMessages   GetSessionId error.session_id_key:" << *load_iter);
        continue;
      }
      MessageRequestMap::const_iterator request_iter = requests.find(session_id); 
      if (request_iter == requests.end())
        continue;
      MessageBodySeq seq;
      GetMessageBodyFromDB(session_id, request_iter->second, seq, true);
      dict.insert(make_pair(session_id, seq));
    }
  }
  float cost_decom = ts.getTime();  
  pool_.ReleaseClient(cli.first, cli.second);
  MCE_INFO("GroupChatMessageCacheClient::GetOfflineMessages --> requests:" << requests.size() << " res:" << dict.size()
                      << " cost_mem:" << cost_mem << " cost_par:" << cost_decom << " cost_total:" << (cost_mem+cost_decom));
  return dict;
}

// request. msgid 当前消息id, offset为0， limit 取离线消息的条数
MessageBodySeq GroupChatMessageCacheClient::GetOfflineMessage(const Ice::Long session_id,  const MessageRequestPtr &request) {
  MessageRequestMap requests;
  requests[session_id] = request;
  MessageBodySeqMap msgbody_seq_map = GetOfflineMessages(requests);
  MessageBodySeqMap::const_iterator msg_body_seq_iter = msgbody_seq_map.find(session_id); 
  if (msg_body_seq_iter != msgbody_seq_map.end())
    return msg_body_seq_iter->second;
  return MessageBodySeq();
}

MessageBodySeq GroupChatMessageCacheClient::GetMessage(const Ice::Long session_id, const MessageRequestPtr &request) {
  MessageRequestMap requests;
  requests[session_id] = request;
  MessageBodySeqMap msgbody_seq_map = GetMessages(requests);
  MessageBodySeqMap::const_iterator msg_body_seq_iter = msgbody_seq_map.find(session_id); 
  if (msg_body_seq_iter != msgbody_seq_map.end())
    return msg_body_seq_iter->second;
  return MessageBodySeq();
}

MessageBodySeqMap GroupChatMessageCacheClient::GetMessages(MessageRequestMap &requests, int load_db) {
  MCE_DEBUG("GroupChatMessageCacheClient::GetMessages  requests.size:" << requests.size() << " load_db:" << load_db);
  MessageBodySeqMap dict;
  if (requests.size() == 0)
    return dict;
  int first_session_id = 0;
  vector<string> session_ids;
  for (MessageRequestMap::const_iterator msg_req_iter=requests.begin(); msg_req_iter!=requests.end(); ++msg_req_iter) {
    char data[64] = {0};
    snprintf(data, 64, "GCMSG#%ld", msg_req_iter->first);
    session_ids.push_back(data);
    if (!first_session_id)
      first_session_id = msg_req_iter->first;
  }
  pair<int, MemcachedClient*> cli = pool_.GetClient(first_session_id);
  if (!cli.second) {
    MCE_WARN("GroupChatMessageCacheClient::GetMessages  pool_.GetClient error. first_session_id" << first_session_id);
    return dict;
  }
  map<string, string> res;
  map<string, int> flags;
  TimeStatN ts; 
  cli.second->Get(session_ids, res, flags);
  float cost_mem = ts.getTime();
  ts.reset();
  MCE_DEBUG("GroupChatMessageCacheClient::GetMessages   res:" << res.size());
  for (map<string, string>::iterator res_iter=res.begin(); res_iter!=res.end(); ++res_iter) {
    vector<string>::iterator del_iter = find(session_ids.begin(), session_ids.end(), res_iter->first);
    if (del_iter != session_ids.end()) {
      session_ids.erase(del_iter);
    }
    long session_id = GetSessionId(res_iter->first);
    if (session_id == -1) { 
      MCE_WARN("GroupChatMessageCacheClient::GetSessionId error.  res_iter->first:" << res_iter->first);
      continue;
    }
    MessageBodySeq seq;
    SessionBody mem_session_body;
    if (!mem_session_body.ParseFromString(res_iter->second)) {
      MCE_WARN("GroupChatMessageCacheClient::ParseFromString error.  res_iter->second:" << res_iter->second);
      continue;
    }
//    const MessageRequestPtr temp_request = requests[session_id];
    MessageRequestMap::const_iterator request_iter = requests.find(session_id); 
    if (request_iter == requests.end())
      continue;
    if (!(request_iter->second))
      continue;
    const MessageRequestPtr temp_request = request_iter->second;
    MessageRequestPtr request = new MessageRequest();
    request->msgid = temp_request->msgid;
    request->offset = temp_request->offset;
    request->limit = temp_request->limit;
    if (request->offset <= MESSAGES_MEM) { 
      GetMessageBodyFromMem(mem_session_body, request, seq);
    }
    if (request->limit>0 && load_db) {
      GetMessageBodyFromDB(session_id, request, seq, false);
    }
    dict.insert(make_pair(session_id, seq));
  }
  if (load_db) {
    for (vector<string>::const_iterator load_iter=session_ids.begin(); load_iter!=session_ids.end(); ++load_iter) {
      const long session_id = GetSessionId(*load_iter);
      MessageRequestPtr request = new MessageRequest();
      request->msgid = 0;
      request->offset = 0;
      request->limit = MESSAGES_MEM;
      MessageBodySeq seq;
      GetMessageBodyFromDB(session_id, request, seq, false);
      /*
      string value;
      if (!SerializeMessageBody(seq, value)) {
        MCE_WARN("GroupChatMessageCacheClient:: SerializeMessageBody  seq.size:" << seq.size());
        continue;
      }
      //pair<int, MemcachedClient*> cli = pool_.GetClient(session_id); //  !!!如果尽量不是获取两次释放一次
      string key = GetSessionKey(session_id);
      if (key == "")
        continue;
      bool res = cli.second->Set(key, value, 0);
      */
      dict.insert(make_pair(session_id, seq));
    }
  }
  float cost_decom = ts.getTime();  
  pool_.ReleaseClient(cli.first, cli.second);
  MCE_INFO("GroupChatMessageCacheClient::GetMessages --> requests:" << requests.size() << " res:" << dict.size()
                      << " cost_mem:" << cost_mem << " cost_par:" << cost_decom << " cost_total:" << (cost_mem+cost_decom));
  return dict;
}

bool GroupChatMessageCacheClient::SerializeMessageBody(const MessageBodySeq &msgbody_seq, string &value) {
  size_t size = msgbody_seq.size()>MESSAGES_MEM ? MESSAGES_MEM : msgbody_seq.size();
  MCE_DEBUG("GroupChatMessageCacheClient::SetMessages   step3.5    size:" <<size );
  SessionBody mem_session_body;
  for (size_t i=0; size>0; ++i) {
    GroupChatMessageBody *mem_msg_body = mem_session_body.add_msg_body();
    SetMemFromMessageBody(mem_msg_body, msgbody_seq.at(i)); 
    --size;
  }
  mem_session_body.SerializeToString(&value);
  return true;
}

bool GroupChatMessageCacheClient::SetMessages(const MessageBodySeq &msg_seq) {
  MCE_DEBUG("GroupChatMessageCacheClient::SetMessages   msg_seq.sizea:" << msg_seq.size());
  if(msg_seq.empty()) 
    return false;
  const long begin_session_id = msg_seq.at(0)->sessionId;
  string key = GetSessionKey(begin_session_id);
  if (key == "")
    return false;
  MessageRequestMap requests;
  MessageRequestPtr request = new MessageRequest();
  request->offset = 0;
  request->limit = MESSAGES_MEM;
  requests[msg_seq.at(0)->sessionId] = request;
  MCE_DEBUG("GroupChatMessageCacheClient::SetMessages   step1" );
  MessageBodySeqMap msgbody_map = GetMessages(requests, 0);
  MessageBodySeq mem_msg_seq = msgbody_map[begin_session_id];
  MCE_DEBUG("GroupChatMessageCacheClient::SetMessages   step2" );
  UpdateMessages(msg_seq, mem_msg_seq);
  MCE_DEBUG("GroupChatMessageCacheClient::SetMessages   step3" );

  string value;
  if (!SerializeMessageBody(mem_msg_seq, value))
    return false;
  MCE_DEBUG("GroupChatMessageCacheClient::SetMessages   step4" );
  pair<int, MemcachedClient*> cli = pool_.GetClient(begin_session_id);
  bool res = cli.second->Set(key, value, 0);
  pool_.ReleaseClient(cli.first, cli.second);
  return res;
}
