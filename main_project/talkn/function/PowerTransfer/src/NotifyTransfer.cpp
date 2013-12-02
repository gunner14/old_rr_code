#include "NotifyTransfer.h"
#include "MessageType.h"
#include "TalkFunStateAdapter.h"
#include <boost/lexical_cast.hpp>
#include "TalkUtil/src/pugixml.hpp"
#include "TalkUtil/src/XmppTools.h"
#include "TalkDeliverAdapter.h"
#include "PowerTransferI.h"

using namespace com::xiaonei::talk::util;
using namespace com::xiaonei::talk::function::transfer;
using namespace com::xiaonei::talk::fun::state;
using namespace pugi;
using namespace mop::hi::svc::adapter;

void deliverNotifyClientTask::handle(){
  TimeStatN ts;
  xml_document doc;
  if(!doc.load(_content.c_str())){
    MCE_WARN("PowerTransfer::deliverNotifyClient --> xml err, " <<_content);
    PowerTransferI::instance().UpCount(100000,0);
    return;
  }

  Ice::Long nid = 0;
  size_t npos = _content.find("\"nid\":\"");
  if(npos != string::npos){
    size_t ipos = _content.find_first_of("\"", npos + 7);
    if(ipos != string::npos){
      string snid = _content.substr(npos+7, ipos - (npos+7));
      try{
        nid = boost::lexical_cast<Ice::Long>(snid);
      } catch(...) {
        MCE_WARN("PowerTransfer::deliverNotifyClient--> boost cast nid error nid = " << nid);
      }
    }
  }
  MessageSeq msgs;
  JidSeqMap jmap;
  try{
    jmap = TalkFunStateAdapter::instance().getUsersJids(_toids,4);
  } catch(Ice::Exception& e) {
    MCE_WARN("PowerTransfer::deliverNotifyClient-->TalkFunStateAdapter::getUserJids-->" << e);
    PowerTransferI::instance().UpCount(100000,0);
    return ;
  }
  MCE_INFO("PowerTransfer::deliverNotifyClient-->jmap:" << jmap.size());
  for(JidSeqMap::iterator jit = jmap.begin();jit != jmap.end();++jit) {
    JidSeq& jseq = (jit->second); 
    for(int j=0; j<(int)jseq.size(); j++){
      if(jseq.at(j)->endpoint[0] == 'T'){
        MessagePtr msg = new Message();
        JidPtr jfrom = new Jid();
        jfrom->userId = 0;
        jfrom->endpoint = "feed.talk.xiaonei.com";
        jfrom->index = -1;
        doc.child("message").append_attribute("to") = jidToString(jseq.at(j)).c_str();
        msg->from = jfrom;
        msg->to = jseq.at(j);
        msg->type = NOTIFY2_MESSAGE;
        msg->msg = doc.xml();
        msgs.push_back(msg);
      }
    }
  }
  if(msgs.empty()){
    //MCE_INFO("PowerTransfer::deliverNotifyClient-->message is empty!");
    return;
  }
  try{
    TalkDeliverAdapter::instance().deliver(msgs);
  } catch(Ice::Exception& e) {
    MCE_WARN("PowerTransfer::deliverNotifyClient-->TalkDeliverAdapter::deliver-->" << e);
    PowerTransferI::instance().UpCount(100000,0);
    return ;
  }
  FunStatManager::instance().Stat("PowerTransfer::deliverNotifyClient", ts.getTime(), false);
  PowerTransferI::instance().UpCount(100000,1,msgs.size());
}

void deliverNotifyWebpagerTask::handle(){
  TimeStatN ts;
  MessageSeq msgs;
  bool is_feed_actor = false;
  Ice::Long nid = 0; 
  if(_content.find("ugc_content") != string::npos){
    is_feed_actor = true;
  }
  size_t npos = _content.find("\"nid\":\"");
  if(npos != string::npos){
    size_t ipos = _content.find_first_of("\"", npos + 7);
    if(ipos != string::npos){
      string snid = _content.substr(npos+7, ipos - (npos+7));
      try{
        nid = boost::lexical_cast<Ice::Long>(snid);
      }catch(...){
        MCE_WARN("PowerTransfer::deliverNotifyWebpager--> boost cast nid error nid = " << nid);
      }
    }
  } 
  JidSeqMap jmap;
  try{
    jmap = TalkFunStateAdapter::instance().getUsersJids(_toids,2);
  } catch(Ice::Exception& e) {
    MCE_WARN("PowerTransfer::deliverNotifyWebpager-->TalkFunStateAdapter::getUserJids-->" << e);
    PowerTransferI::instance().UpCount(200000,0);
    return ;
  }
  for(JidSeqMap::iterator jit = jmap.begin();jit != jmap.end();++jit) {
    JidSeq& jseq = (jit->second);
    for(int j=0; j<(int)jseq.size(); j++){
      if(jseq.at(j)->endpoint[0] == 'W'){
        MessagePtr msg = new Message();
        JidPtr jfrom = new Jid();
        jfrom->userId = 0;
        jfrom->endpoint = "feed.talk.xiaonei.com";
        jfrom->index = -1;
        ostringstream os;
        os << "<message type='common' from='" << jidToString(jfrom) 
           << "' to='" << jidToString(jseq.at(j)) << "'>" << "</message>";
        xml_document doc;
        if(!doc.load(os.str().c_str())){
          continue;
        }
        xml_node body = doc.child("message").append_child();
        body.set_name("body");
        body.append_child(node_pcdata).set_value(_content.c_str());
        msg->from = jfrom;
        msg->to = jseq.at(j);
        msg->type = NOTIFY2_MESSAGE;
        msg->msg = doc.xml();              
        msgs.push_back(msg);
      }
    }
  }
  if(msgs.empty()){
    PowerTransferI::instance().UpCount(200000,0);
    return;
  }
  ostringstream os;
  for(IntSeq::iterator idit = _toids.begin(); idit != _toids.end(); ++idit){
    os << (*idit) << " ";
  }
  try{
    MCE_INFO("PowerTransfer::deliverNotifyWebpager--> is_feed_actor = " << is_feed_actor << " nid : " << nid  << " toids.size :" << _toids.size() << " toids : " << os.str());
    TalkDeliverAdapter::instance().deliver(msgs);
  }catch(Ice::Exception& e){
    MCE_WARN("PowerTransfer::deliverNotifyWebpager-->TalkDeliverAdapter::deliver--> " << e);
    PowerTransferI::instance().UpCount(200000,0);
    return ;
  }
  FunStatManager::instance().Stat("PowerTransfer::deliverNotifyWebpager", ts.getTime(), false);
  PowerTransferI::instance().UpCount(200000,1,msgs.size());
}

void deliverNotifyPhoneTask::handle(){
  TimeStatN ts;
  xml_document doc;
  if(!doc.load(_content.c_str())){
    MCE_WARN("PowerTransfer::deliverNotifyPhone --> xml err, " <<_content);
    PowerTransferI::instance().UpCount(300000,0);
    return;
  }
  Ice::Long nid = 0;
  size_t npos = _content.find("\"nid\":\"");
  if(npos != string::npos){
    size_t ipos = _content.find_first_of("\"", npos + 7);
    if(ipos != string::npos){
      string snid = _content.substr(npos+7, ipos - (npos+7));
      try{
        nid = boost::lexical_cast<Ice::Long>(snid);
      }catch(...){
        MCE_WARN("PowerTransfer::deliverNotifyClient--> boost cast nid error nid = " << nid);
      }
    }
  }
  MessageSeq msgs;
  JidSeqMap jmap;
  try{
    jmap = TalkFunStateAdapter::instance().getUsersJids(_toids,16);
  }catch(Ice::Exception& e){
    MCE_WARN("PowerTransfer::deliverNotifyPhone-->TalkFunStateAdapter::getUserJids--> " << e);
    PowerTransferI::instance().UpCount(300000,0);
    return ;
  }
  for(JidSeqMap::iterator jit = jmap.begin();jit != jmap.end();++jit) {
    JidSeq& jseq = (jit->second);
    for(int j=0; j<(int)jseq.size(); j++){
      if(jseq.at(j)->endpoint[0] == 'P'){
        MessagePtr msg = new Message();
        JidPtr jfrom = new Jid();
        jfrom->userId = 0;
        jfrom->endpoint = "feed.talk.xiaonei.com";
        jfrom->index = -1;
        doc.child("message").append_attribute("to") = jidToString(jseq.at(j)).c_str();
        msg->from = jfrom;
        msg->to = jseq.at(j);
        msg->type = PHONE_NOTIFY;
        msg->msg = doc.xml();
        msgs.push_back(msg);
      }
    }
  }
  if(msgs.empty()){
    PowerTransferI::instance().UpCount(300000,0);
    return;
  }
  try{
    TalkDeliverAdapter::instance().deliver(msgs);
  }catch(Ice::Exception& e){
    MCE_WARN("PowerTransfer::deliverNotifyPhone-->TalkDeliverAdapter::deliver-->" << e);
    PowerTransferI::instance().UpCount(300000,0);
  }
  FunStatManager::instance().Stat("PowerTransfer::deliverNotifyPhone", ts.getTime(), false);
  PowerTransferI::instance().UpCount(300000,1,msgs.size());
}

