#include "Tester.h"
#include "TalkProtoAdapter.h"
#include "TalkGroupLoaderAdapter.h"

using namespace xce::xntalk::tester;
using namespace com::xiaonei::talk;
using namespace MyUtil;
using namespace com::xiaonei::talk::proto;

void MyUtil::initialize() {
	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&TesterManagerI::instance(), service.createIdentity("M", ""));

	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("Service.Mod");
	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service.Interval", 5);
	MCE_INFO("MyUtil::initialize. mod:" << mod << " interval:" << interval);
}

void TesterManagerI::TestMC() {
  Str2StrMap paras;
  paras["namespace"] = "http://talk.renren.com/gcmessage/set";
  paras["content"] = "haha";
  paras["source"] = "renren";
  paras["source_type"] = "22";
  paras["link"] = "http://www.google.com";
  paras["uid"] = "225521695";
  paras["attach_flag"] = "0";
  paras["create_time"] = "2012-04-05 12:12:12";
//  .notify(paras);
  string chat_xml = "<message from=\"225521695@talk.renren.com\" to=\"dodo@conferece.talk.renren.com\" type=\"groupchat\" >"
    "<body>Harpier cries: 'tis time, 'tis time.</body>"
    "<format xmlns='xn:format' bold=\"0\" face=\"SimSun\" height=\"9\" ink=\"3355443\""
    " italic=\"0\" />"
    "</message>"
    ;
  JidPtr jid = new Jid;
  jid->userId = 225521695;

//  TalkProtoAdapter::instance().Send(jid, chat_xml);
  
  string chat_xml1 = "<message from=\"225521695@talk.renren.com\" to=\"dodo@conferece.talk.renren.com\" type=\"getgroupchat\" >"
    "<body>Harpier cries: 'tis time, 'tis time.</body>"
    "<format xmlns='xn:format' bold=\"0\" face=\"SimSun\" height=\"9\" ink=\"3355443\""
    " italic=\"0\" />"
    "</message>"
    ;
  TalkProtoAdapter::instance().Send(jid, chat_xml1);
  /*
  MessageBodySeq seq;
  MessageBodyPtr mb = new MessageBody;
//  mb->messageId = 1;
//  mb->sessionId = 11;
  mb->content = "haha";
  mb->source = "renren";
  mb->sourceType = 22;
  mb->link = "http://www.google.com";
  mb->uid = 225521695;
  mb->attachFlag = 1;
  mb->createTime = "2012-04-05 12:12:12";
  seq.push_back(mb);

  mb = new MessageBody;
  mb->messageId = 2;
  mb->sessionId = 11;
  mb->content = "haha1";
  mb->source = "renren1";
  mb->sourceType = 23;
  mb->link = "http://www.google1.com";
  mb->uid = 225521695;
  mb->attachFlag = 1;
  mb->createTime = "2012-04-06 12:12:12";
  seq.push_back(mb);
  GroupChatMessageCacheClient::instance().SetMessages(seq); 
  MCE_DEBUG("TestMC  succeed to insert");
  MessageRequestPtr mr = new MessageRequest;
  mr->msgid = 0;
  mr->offset = 0;
  mr->limit = 10;
  MessageRequestMap mrm;
  mrm.insert(pair<int, MessageRequestPtr>(11, mr));
  MessageBodySeqMap msgbody_map = GroupChatMessageCacheClient::instance().GetMessages(mrm); 
  MCE_DEBUG("TestMC  mark");
  for (MessageBodySeqMap::const_iterator iter1=msgbody_map.begin(); iter1!=msgbody_map.end(); ++iter1) {
    MCE_DEBUG("TestMC session_id:" << iter1->first);
    for (MessageBodySeq::const_iterator iter2=(iter1->second).begin(); iter2!=(iter1->second).end(); ++iter2) {
      MCE_DEBUG("TestMC msg_body msgid:" <<(*iter2)->messageId <<"   sessionId:"<< (*iter2)->sessionId <<"  content:"<< (*iter2)->content <<"   source:"<< (*iter2)->source
          << "   sourceType:" <<(*iter2)->sourceType <<"   link:" <<(*iter2)->link<<"   uid:" <<(*iter2)->uid<<"    attachFlag:" << (*iter2)->attachFlag<<"   createTime:"
          << (*iter2)->createTime); 
    }
  }
  */
  

  /*
  TimeStat st;
  for (int i=0; i<100000; ++i) {
     IMIdGeneratorAdapter::instance().getId(21);
  }  
  MCE_INFO("LoginFeedManagerI::TestMC  time cost:" << st.getTime());
  */


}

void TestCreateGroup() {
  JidPtr jid = new Jid;
  jid->userId = 225521695;
 string create_group = 
  "<iq id='1'"
" type='set'>"
 "<query xmlns='http://conference.talk.renren.com/create'>"
"<item name='明星八卦群'  introduction='8 各种明星'  auth='any' fixed='1'/>"
 "</query>"
"</iq>";

  TalkProtoAdapter::instance().Send(jid, create_group);
}

void TestSerachGroup() {
  JidPtr jid = new Jid;
  jid->userId = 225521695;
 string serach_group = 
"<iq id='1'"
  " to='conference.talk.renren.com' "
  "type='get'>"
  "<query xmlns='http://conference.talk.renren.com/search'>"
  "<type>"
  "roomid"
  "</type>"
  "<content>"
  "510025"
  "</content>"
  "</query>"
  "</iq>";

  TalkProtoAdapter::instance().Send(jid, serach_group);
}

void TesterManagerI::Invoke1(const ::Ice::Current&) {
  MCE_DEBUG("TesterManagerI::Invoke1");
//  TestMC();
  TestCreateGroup(); 
// TestSerachGroup(); 
}
void TesterManagerI::Invoke2(const ::Ice::Current&) {
  GroupInfoPtr group_info = new GroupInfo;
  group_info->id = 22112;
  TalkGroupLoaderAdapter::instance().SetGroupInfo(group_info);
}
