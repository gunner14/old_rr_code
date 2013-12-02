#include "TalkProtoManagerI.h"
#include "XmppTools.h"
#include "ExpatWrapper.h"
#include "AdapterI.h"
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "Notify/util/clear_silver_wrap.h"
#include "pugixml.hpp"
#include "ReceiversHandler.h"
#include "feed/FeedMemcProxy/client/online_cache_client.h"
#include "MessageType.h"
#include "Deliver.h"
#include "OnlineCenterAdapter.h"
#include "TalkFunStateAdapter.h"

using namespace com::xiaonei::talk::util;
using namespace com::xiaonei::talk::proto;
using namespace xce::talk::online;
using namespace MyUtil;
using namespace pugi;
using namespace xce::notify;
using namespace com::xiaonei::talk::fun::state;

void MyUtil::initialize() {
	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&TalkProtoManagerI::instance(), service.createIdentity("M", ""));

	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("Service.Mod");
	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service.Interval", 5);
	xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerTalkProto", &TalkProtoManagerI::instance(), mod, interval, new XceFeedControllerChannel());

  Ice::PropertiesPtr props = service.getCommunicator()->getProperties();
	string configDirPath = props->getPropertyWithDefault("Service.ConfigFilePath", "/data/xce/XNTalk/etc/TalkProtoConf");
  string configFilePath = configDirPath + "/TalkProto.conf";

	TalkProtoManagerI::instance().LoadProtoConfig(configFilePath);
	TalkProtoManagerI::instance().LoadTplConfig(configDirPath);
	MCE_INFO("MyUtil::initialize. mod:" << mod << " interval:" << interval );

	PipePool::instance().initialize(new PipeFactoryI);
  TaskManager::instance().scheduleRepeated(&CounterTimer::instance());
}



bool GetContentFromFile(const string file_path, string &content) {
  bool ret =true;
  ifstream ifile(file_path.c_str()) ; 
  if (!ifile) { 
    MCE_WARN("IMWindowManagerI::GetReceiversFromFile   Fail to open file ,path:" << file_path);
    ret = false;
  }
  if (ret) {
    char buf[2048];
    for(int i=0; !ifile.eof(); i++) {
      if (ifile.getline(buf , sizeof(buf)-1).good()) {
        content += string(buf);
      } else {
        ret = false;
        break;
      }
    }
  }
  return ret;
}



bool TalkProtoManagerI::HasAdapter(const string& adaptername){
	if(adapters_.find(adaptername) == adapters_.end()){
		return false;
	}
	return true;
}


void TalkProtoManagerI::AddAdapter(const string& controllername, const string& channel){
	if(!HasAdapter(controllername)){
    MyUtil::ChannelPtr chn = 0;
    if (channel == "XceFeedControllerChannel") {
      chn = new XceFeedControllerChannel;
    } else if (channel == "TalkChannel") {
      chn = new TalkChannel;
    } else {
      MCE_WARN("TalkProtoManagerI::AddAdapter--> channel not found:" << channel);
      return;
    }
		TalkFunctionAdapterPtr ptr = new TalkFunctionAdapter(controllername, chn);
		adapters_[controllername] = ptr;
	}
}




void TalkProtoManagerI::LoadTplConfig(const string& tplDirPath){
  string tplConfPath = tplDirPath+"/tpl.conf";
	MCE_INFO("TalkProtoManagerI::LoadTplConfig --> tplconfigpath:" << tplConfPath);
  map<Ice::Int, TplInfoPtr> temp_tpls_info;
	xml_document doc;
	if (doc.load_file(tplConfPath.c_str())) {
		xml_node xntalk_node = doc.child("xntalk");
		for (xml_node config_node = xntalk_node.child("config"); config_node; config_node = config_node.next_sibling("config")) {
			int type = 0;
      try {
        type = boost::lexical_cast<int>(config_node.child("config_type").first_child().value().c_str());
      } catch (...) {
        continue;
      }
      TplInfoPtr tpl_info = new TplInfo;
			tpl_info->description_ = config_node.child("description").first_child().value();
      xml_node tpls_node = config_node.child("tpl");
      
      for (xml_node tpl_node=tpls_node.child("item"); tpl_node; tpl_node=tpl_node.next_sibling("item")) {
        string tpl_type = tpl_node.attribute("type").value();
        string tpl_name = tpl_node.attribute("name").value();
        string tpl_client = tpl_node.attribute("client").value();
		    MCE_DEBUG("TalkProtoManagerI::LoadTplConfig-->  tpl_type:" << tpl_type << "   tpl_name:"<<tpl_name<<"   tpl_client:" << tpl_client);
        
        string tpl_content;
        if (!tpl_name.empty()) {
          string tpl_path = tplDirPath+"/templates/"+tpl_name;
          GetContentFromFile(tpl_path, tpl_content);
          MCE_DEBUG("tpl_path:" << tpl_path<<"   conetnt:" <<tpl_content);
        }
        if (!tpl_type.empty() && !tpl_content.empty() && !tpl_client.empty()) {
          const string tpl_key = tpl_type + "." + tpl_client;
          (tpl_info->tpls_)[tpl_key] = tpl_content;
        }
      }
      xml_node important_key_node = config_node.child("key");
      for (xml_node item_node=important_key_node.child("item"); item_node; item_node=item_node.next_sibling("item")) {
        string key = item_node.first_child().value();
        if (!key.empty()){
          (tpl_info->important_keys_).push_back(key);
        }
      }
      temp_tpls_info[type] = tpl_info;
		}
   
	} else {
		MCE_WARN("TalkProtoManagerI::LoadTplConfig--> file not found,  path:" << tplConfPath);
	}

  ostringstream os;
  for (map<Ice::Int, TplInfoPtr>::const_iterator iter=temp_tpls_info.begin(); iter!=temp_tpls_info.end(); ++iter) {
    for (map<string,string>::const_iterator iter1=((iter->second)->tpls_).begin(); iter1!=((iter->second)->tpls_).end(); iter1++) {
      os.str("");
      os << iter->first << "." << iter1->first;
      if (!CSTemplateCache::Instance().AddTemplate(os.str(),iter1->second , Str2StrMap()))
        MCE_WARN("TalkProtoManagerI::LoadTplConfig  add template error.    type:" <<os.str() << "    tpl:" << iter1->second );;
      MCE_INFO("TalkProtoManagerI::LoadTplConfig   tpl_type:"<<os.str() <<"    tpl_content:\"" <<iter1->second <<"\" ");    
    }
  }

  ///////////
  for (map<Ice::Int, TplInfoPtr>::const_iterator iter=temp_tpls_info.begin(); iter!=temp_tpls_info.end(); ++iter) {
    os << "type:" <<iter->first<<"   description:" <<iter->second->description_;
    for (map<string,string>::const_iterator iter1=((iter->second)->tpls_).begin(); iter1!=((iter->second)->tpls_).end(); iter1++) {
      os << "   tpl_type:"<<iter1->first <<"    tpl_content:\"" <<iter1->second <<"\" ";    
    }
    os << "  important keys:";
    for (vector<string>::const_iterator iter2=((iter->second)->important_keys_).begin(); iter2!=((iter->second)->important_keys_).end(); iter2++)
      os << *iter2 << "  " ;
    os << "\n";
  }
  MCE_DEBUG("TalkProtoManagerI::LoadTplConfig   tpls_info_:" << os.str());
  //////////
  
  IceUtil::RWRecMutex::WLock lock(tpls_info_mutex_);
  tpls_info_.swap(temp_tpls_info);
}

void TalkProtoManagerI::LoadProtoConfig(const string& path){
	MCE_DEBUG("TalkProtoManagerI::LoadProtoConfig --> configpath:" << path);
	xml_document doc;
	if (doc.load_file(path.c_str())) {
		xml_node confignode = doc.child("config");
		for (xml_node n = confignode.child("proto"); n; n = n.next_sibling("proto")) {
			xml_node checkernode = n.child("checker");
			xml_node handlernode = n.child("handler");
			if (checkernode.attribute("key")
          && checkernode.attribute("value")
          && handlernode.attribute("controllername")
          && handlernode.attribute("channel")
          ) {
				string key = checkernode.attribute("key").value();
				string checker_value = checkernode.attribute("value").value();
				string controllername = handlernode.attribute("controllername").value();
				string channel = handlernode.attribute("channel").value();
			
			
				CheckerInfoPtr ci = new CheckerInfo(key, checker_value);
        map<CheckerInfoPtr, vector<string> >::iterator cit = checkers_.find(ci);
        if(cit == checkers_.end()) {
          //新checker
				  checkers_[ci].push_back(controllername);
        } else {
          // checker已存在，检查插controllername是否存在,不存在则插入新controllername
          if(find(cit->second.begin(), cit->second.end(), controllername) == cit->second.end()) 
            cit->second.push_back(controllername);
        }
				AddAdapter(controllername, channel);

				MCE_INFO("TalkProtoManagerI::LoadProtoConfig -->  key:" << key << " value:" << checker_value << " controllername:" << controllername << " channel:" << channel);
			}else{
				MCE_WARN("TalkProtoManagerI::LoadProtoConfig-->config error");
			}

		}

	} else {
		MCE_WARN("TalkProtoManagerI::LoadProtoConfig--> file not found, path:" << path);
	}
	MCE_INFO("TalkProtoManagerI::LoadProtoConfig --> configpath:" << path << " load done checkers.size:" << checkers_.size() << " adapters.size:" << adapters_.size());

}


void TalkProtoManagerI::GetAdapter(vector<string>& controllernames, vector<TalkFunctionAdapterPtr>& ptrs, const Str2StrMap& mp){
	//MCE_INFO("TalkProtoManagerI::GetAdapter--> checker.size:" << checkers_.size() << " adapters.size:" << adapters_.size() << " size:" << TalkProtoManagerI::instance().checkers_.size() << " add1:" << this << " add2:" << &TalkProtoManagerI::instance());
	
	CheckerInfoPtr ci = new CheckerInfo;


	for(Str2StrMap::const_iterator it = mp.begin(); it != mp.end(); ++it){
		ci->checker_key_ = it->first;
		ci->checker_value_ = it->second; // 空白字符一样算在value里面

		map<CheckerInfoPtr, vector<string> >::iterator cit = checkers_.find(ci);
    // 如果找不到，尝试 通用查找方式 * 
    if (cit == checkers_.end()) {
      ci->checker_value_ = "*";
      cit = checkers_.find(ci);
    }
		if(cit != checkers_.end()){
      for(size_t i = 0; i < cit->second.size(); ++i) {
			  map<string, TalkFunctionAdapterPtr>::iterator ait = adapters_.find(cit->second.at(i));
			  if(ait != adapters_.end()){
          ptrs.push_back(ait->second);
          controllernames.push_back(cit->second.at(i));
			  } else {
          MCE_WARN("TalkProtoManagerI::GetAdapter-> can not find adapter:" << it->first << " second:" 
          << it->second << " controllername:" << cit->second.at(i));	
        }
      }
      // Checker调用统计
      cit->first->counter_++;
      break;
		}

    // 多个key决定一个controller仲么版？
	}
}

/* brief: 协议接收接口，接入会把从用户接收到得数据，通过该接口投递到该服务
 * actor: 投递用户jid
 * xml: 数据，目前只有talkproxy，对其进行调用，送过来的都是xml数据
 * return: void
 */
void TalkProtoManagerI::Send(const JidPtr& actor, const string& xml, const Ice::Current& current){
	//MCE_INFO("TalkProtoManagerI::Send--> xml:" << xml);
	TimeStatN ts;
	ExpatWrapper w;
	map<string, string> mp;
	int success = w.GetSkeleton(xml, mp);
	FunStatManager::instance().Stat("ExpatWrapper.GetSkeleton", ts.getTime(), (ts.getTime() > 2.0));
	if(success < 0){
		MCE_WARN("TalkProtoManagerI::Send--> xml error :" << xml);
	}else{
		mp["actor_jid"] = jidToString(actor);
		mp["actor_uid"] = boost::lexical_cast<string>(actor->userId);
		mp["actor_jid_index"] = boost::lexical_cast<string>(actor->index);
		mp["actor_jid_endpoint"] = actor->endpoint; 
    /*
		MCE_DEBUG("TalkProtoManagerI::Send --> xml:" << xml << " ANS SIZE:" << size)
		for(map<string,string>::iterator it = mp.begin(); it != mp.end(); ++it){
			MCE_DEBUG("TalkProtoManagerI::Send-->key:" << it->first << " value:" << it->second);
		}
    */
	}
	//MCE_INFO("TalkProtoManagerI::Send --> checker.size:" << checkers_.size() << " adapters.size:" << adapters_.size() << " size:" << TalkProtoManagerI::instance().checkers_.size());
	MCE_DEBUG("TalkProtoManagerI::Send --> xml:" << xml);
	vector<TalkFunctionAdapterPtr> aptrs;
	vector<string> controllernames;
	{
		IceUtil::RWRecMutex::RLock lock(rwmutex_);
		GetAdapter(controllernames, aptrs, mp);
	}
  
  if(aptrs.size() == 0) {
    MCE_WARN("TalkProtoManagerI::Send--> could not find the controller xml:");
  } else {
    for(size_t i = 0; i < aptrs.size(); ++i) {
	    if(aptrs.at(i)){
		    try{
			    aptrs.at(i)->Notify(mp);
		    }catch(Ice::Exception& e){
			    MCE_WARN("TalkProtoManagerI::Send-->controllername:" << controllernames.at(i) << " error:" << e);
		    }
      }
    }
	}
	FunStatManager::instance().Stat("TalkProtoManagerI::Send", ts.getTime(), (ts.getTime() > 250.0));
}

bool TalkProtoManagerI::CheckParas(const Str2StrMap &paras, ParasConfig& pc) {
  for (Str2StrMap::const_iterator iter=paras.begin(); iter!=paras.end(); iter++) {
    if (iter->first == "config_type") {
      pc.config_type = iter->second;
    } else if (iter->first == "item_type") {
      pc.item_type = iter->second;
    } else if  (iter->first == "from_jid") {
      pc.from_jid = iter->second;
    } else  if (iter->first == "to_id") {
      pc.to_ids = ReceiversHandler::ReceiversSplit(iter->second);
    } else if (iter->first == "online_type") {
      // to user's online type by jian.wang3 2012-09-12
      try {
        pc.online_type = boost::lexical_cast<int>(iter->second);
      } catch(...) {
        MCE_WARN("TalkProtoManagerI::CheckParas --> online type lexical error"); 
      }
    } else if (iter->first == "to_jid") { 
      pc.to_jid = iter->second;
    } else if (iter->first == "message_type") {
      pc.msg_type = atoi((iter->second).c_str());
    } else if ((iter->first)[0]>='0' && (iter->first)[0]<='9') {
      const size_t first_point_pos = (iter->first).find('.');
      if (first_point_pos != string::npos)
        pc.replaceable_keys.insert((iter->first).substr(first_point_pos+1));
    }
  }
  
  MCE_DEBUG("TalkProtoManagerI::CheckParas  config_type: "<< pc.config_type << " item_type:"
    << pc.item_type <<" to_dis.size:" << pc.to_ids.size() << " to_jid:" << pc.to_jid);
  MCE_DEBUG("TalkProtoManagerI::CheckParas  step1    " );
  if (pc.config_type == "" || pc.item_type== "" || (pc.to_ids.empty() && pc.to_jid.empty()))
    return false;

  MCE_DEBUG("TalkProtoManagerI::CheckParas  step2");
  IceUtil::RWRecMutex::RLock lock(tpls_info_mutex_);
  map<Ice::Int, TplInfoPtr>::const_iterator tpls_info_iter =tpls_info_.find(atoi(pc.config_type.c_str())); 
  if (tpls_info_iter == tpls_info_.end()) {
    return false;
  }
  TplInfoPtr tpl_info = tpls_info_iter->second;
  /*
  for (map<string, string>::const_iterator tpls_iter=(tpl_info->tpls_).begin(); tpls_iter!=(tpl_info->tpls_).end(); tpls_iter++) {
    tpls[config_type_iter->second+"."+tpls_iter->first] = tpls_iter->second;
  }*/

  MCE_DEBUG("TalkProtoManagerI::CheckParas  step3");
  vector<string>::const_iterator keys_iter;
  for (keys_iter=(tpl_info->important_keys_).begin();  keys_iter!=(tpl_info->important_keys_).end(); keys_iter++) {
    Str2StrMap::const_iterator temp_iter = paras.find(*keys_iter);
    if (temp_iter == paras.end()) {
      MCE_INFO("TalkProtoManagerI::CheckParas   key:" <<*keys_iter << "   not found in paras.");
      break;
    }
  }
  MCE_DEBUG("TalkProtoManagerI::CheckParas  step4");
  if (keys_iter != (tpl_info->important_keys_).end()){
    return false;
  }
  MCE_DEBUG("TalkProtoManagerI::CheckParas  step5");
  return true;
}

string TalkProtoManagerI::GetTplByJid(const JidPtr &jid,const string &config_type, const string &item_type) {
  string ret;
  ostringstream os;
  os << config_type << "." << item_type << ".";
  int ot = MemcGetOnlineType(jid->endpoint);
  if (ot & 4)
    os << "1";
  else if (ot & 2)
    os << "2";
  else if (ot & 8)
    os << "3";
  else 
    os << "0"; 
  // tpl调用统计
  tpl_counter_[os.str()].var_++;
  return os.str();
}

/* brief: Receive批量接口，减少rpc次数
 */
void TalkProtoManagerI::BatchReceive(const Str2StrMapSeq& parasSeq, const Ice::Current& current) {
  for (Str2StrMapSeq::const_iterator iter=parasSeq.begin(); iter!=parasSeq.end(); iter++ )
    Receive(*iter, current);
}

/* brief: 协议发送接口，业务逻辑服务，会通过该接口将协议投递给指定的用户
 * 该接口会根据用户给定的数据，进行模板拼接操作
 * paras: 模板拼接使用的数据，类似与模板需要填充数据段的变量
 * return: void
 */
void TalkProtoManagerI::Receive(const Str2StrMap& paras, const Ice::Current& current){
  for (Str2StrMap::const_iterator iter=paras.begin(); iter!=paras.end(); iter++) {
    MCE_DEBUG("TalkProtoManagerI::Receive   first:" << iter->first << "   second:" <<iter->second);
  }
	TimeStatN ts;
  if (paras.size() > 3) {
    ParasConfig pc;
    // to user 在线方式,默认14为(WP 2|IM 4|PHONE 8)
    pc.online_type = 2|4|16;

    if (CheckParas(paras, pc)) {
      Str2StrMap temp;
      MessageSeq msgs; 
      temp.insert(paras.begin(), paras.end());
/*
      if(pc.to_ids.empty()) {
        MCE_WARN("TalkProtoManagerI::Receive-->error: to_ids is empty");
        return;
      }
*/

      if(!pc.to_jid.empty()) {
        // to_jid不为空 进入express模式, 不检测是否在线直接给to_jid, 不处理其他to_id
        JidPtr to_jid_ptr = stringToJid(pc.to_jid);
        string tpl = GetTplByJid(to_jid_ptr, pc.config_type, pc.item_type);
        temp["to"] = pc.to_jid;
        temp["from"] = pc.from_jid;
        MCE_DEBUG("TalkProtoManagerI::Receive  tpl:" << tpl );
        CSTemplate *t = CSTemplateCache::Instance().GetTemplate(tpl);
        if (t) {
          MessagePtr msg = new Message;
          MessageSeq msg_express;
          msg->from = stringToJid(pc.from_jid);
          msg->to = to_jid_ptr;
          msg->msg = t->Render(temp);
          msg->type = pc.msg_type;          
          msgs.push_back(msg);

          ostringstream os;
          os << "  to:"<< jidToString(msg->to) <<" type:" << msg->type << "  msg:" << msg->msg;
          MCE_DEBUG("TalkProtoManagerI::Receive express message:" << os.str());
        } else {
          MCE_WARN("TalkProtoManagerI::Receive express t empty");
        }
        if (!msgs.empty())
          TalkDeliver::instance().deliver(msgs);
	      FunStatManager::instance().Stat("TalkProtoManagerI::Receive", ts.getTime(), (ts.getTime() > 250.0));
        return;
      } 

      // 检查online type
      if (pc.online_type < 0) {
        MCE_WARN("TalkProtoManagerI::Receive-->illegal online type:" << pc.online_type );
        pc.online_type = 0;
      }
      JidSeqMap jids_map;
      try {
        jids_map = TalkFunStateAdapter::instance().getUsersJids(pc.to_ids, pc.online_type);
      } catch (Ice::Exception &e) {
        MCE_WARN("TalkProtoManagerI::Receive-->TalkFunStateAdapter:getUsersJids to_ids.size:" << pc.to_ids.size()
            << "   online_type:" << pc.online_type <<"  error:" << e);
      }
      MCE_DEBUG("TalkProtoManagerI::Receive-->online type : " << pc.online_type);
      for (JidSeqMap::const_iterator to_iter=jids_map.begin(); to_iter!=jids_map.end(); ++to_iter) {
        Str2StrMap::const_iterator temp_iter;
        for (set<string>::const_iterator key_iter=pc.replaceable_keys.begin(); 
          key_iter!=pc.replaceable_keys.end(); key_iter++) {
          temp_iter = paras.find(str(boost::format("%d.%s") % (to_iter->first) % (*key_iter)));
          if (temp_iter == paras.end())
            temp[*key_iter] = temp_iter->second;
        }
        for (JidSeq::const_iterator jid_iter=(to_iter->second).begin(); jid_iter!=(to_iter->second).end(); jid_iter++) {
          string tpl = GetTplByJid(*jid_iter, pc.config_type, pc.item_type);
          temp["to"] = jidToString((*jid_iter));
          temp["from"] = pc.from_jid;
          MCE_DEBUG("TalkProtoManagerI::Receive  tpl:" << tpl );
          CSTemplate *t = CSTemplateCache::Instance().GetTemplate(tpl);
          if (!t) {
            MCE_DEBUG("TalkProtoManagerI::Receive  t empty");
            continue;
          }
          string result = t->Render(temp);
          MessagePtr msg = new Message;
          msg->from = stringToJid(pc.from_jid);
          msg->to = (*jid_iter);
          msg->msg = result;
          msg->type = pc.msg_type;          
          msgs.push_back(msg);

          //////
          ostringstream os;
          os << "  to:"<<jidToString(msg->to) <<"   type:" << msg->type << "    msg:" << msg->msg;
          MCE_DEBUG("TalkProtoManagerI::Receive  message:"<< os.str());
        }
      }
      if (!msgs.empty())
        TalkDeliver::instance().deliver(msgs);
    }
  }
	FunStatManager::instance().Stat("TalkProtoManagerI::Receive", ts.getTime(), (ts.getTime() > 250.0));
}
/* brief: 协议发送接口，业务逻辑服务，会通过该接口将协议投递给指定的用户，该接口业务层一般指定了
 * 发送对象信息，不许要TalkProto服务再次获取一遍在线信息
 * paras: 模板拼接使用的数据，类似与模板需要填充数据段的变量
 * return: void
 */
void TalkProtoManagerI::ReceiveExpress(const Str2StrMap& paras, const Ice::Current& current) {
	TimeStatN ts;
  // 快递方式发送消息,调用者提供Jids,不调用TalkFunState取在线状态

  MessageSeq msgs; 
  if (paras.size() > 3) {
    ParasConfig pc;

    if (CheckParas(paras, pc)) {
      Str2StrMap temp;
      temp.insert(paras.begin(), paras.end());

      if(pc.to_jid.empty()) {
        MCE_WARN("TalkProtoManagerI::ReceiveExpress-->error: to_jids is empty");
        return;
      }
      
      vector<string> to_jids;
      ReceiversHandler::ReceiversSplit(pc.to_jid, to_jids);
      for(size_t i = 0; i < to_jids.size(); ++i) {
        JidPtr to_jid_ptr = stringToJid(to_jids.at(i));
        string tpl = GetTplByJid(to_jid_ptr, pc.config_type, pc.item_type);
        temp["to"] = to_jids.at(i);
        temp["from"] = pc.from_jid;
        CSTemplate *t = CSTemplateCache::Instance().GetTemplate(tpl);
        if (t) {
          MessagePtr msg = new Message;
          MessageSeq msg_express;
          msg->from = stringToJid(pc.from_jid);
          msg->to = to_jid_ptr;
          msg->msg = t->Render(temp);
          msg->type = pc.msg_type;          
          msgs.push_back(msg);

          ostringstream os;
          os << "  to:"<< jidToString(msg->to) <<" type:" << msg->type << "  msg:" << msg->msg;
          MCE_DEBUG("TalkProtoManagerI::ReceiveExpress message:" << os.str());
        } else {
          MCE_WARN("TalkProtoManagerI::ReceiveExpress t empty");
        }
      }
    }
  }
  if (!msgs.empty())
    TalkDeliver::instance().deliver(msgs);

	FunStatManager::instance().Stat("TalkProtoManagerI::ReceiveExpress", ts.getTime(), (ts.getTime() > 250.0));
}
/* brief: ReceiveExpress的批量接口
 */
void TalkProtoManagerI::BatchReceiveExpress(const Str2StrMapSeq& paraSeq, const Ice::Current& current){
	TimeStatN ts;
  // 快递方式发送批量消息,调用者提供Jids,不调用TalkFunState取在线状态

  MessageSeq msgs; 
  for(Str2StrMapSeq::const_iterator pit = paraSeq.begin(); pit != paraSeq.end(); ++pit) {
    if (pit->size() > 3) {
      ParasConfig pc;

      if (CheckParas(*pit, pc)) {
        Str2StrMap temp;
        temp.insert(pit->begin(), pit->end());

        if(pc.to_jid.empty()) {
          MCE_WARN("TalkProtoManagerI::ReceiveExpress-->error: to_jids is empty");
          return;
        }
      
        vector<string> to_jids;
        ReceiversHandler::ReceiversSplit(pc.to_jid, to_jids);
        for(size_t i = 0; i < to_jids.size(); ++i) {
          JidPtr to_jid_ptr = stringToJid(to_jids.at(i));
          string tpl = GetTplByJid(to_jid_ptr, pc.config_type, pc.item_type);
          temp["to"] = to_jids.at(i);
          temp["from"] = pc.from_jid;
          CSTemplate *t = CSTemplateCache::Instance().GetTemplate(tpl);
          if (t) {
            MessagePtr msg = new Message;
            MessageSeq msg_express;
            msg->from = stringToJid(pc.from_jid);
            msg->to = to_jid_ptr;
            msg->msg = t->Render(temp);
            msg->type = pc.msg_type;          
            msgs.push_back(msg);

            ostringstream os;
            os << "  to:"<< jidToString(msg->to) <<" type:" << msg->type << "  msg:" << msg->msg;
            MCE_DEBUG("TalkProtoManagerI::BatchReceiveExpress message:" << os.str());
          } else {
            MCE_WARN("TalkProtoManagerI::BatchReceiveExpress t empty");
          }
        }
      }
    }
  }
  if (!msgs.empty())
    TalkDeliver::instance().deliver(msgs);

	FunStatManager::instance().Stat("TalkProtoManagerI::BatchReceiveExpress", ts.getTime(), (ts.getTime() > 250.0));
}

void TalkProtoManagerI::PrintCounter() {
  for(map<CheckerInfoPtr, vector<string> >::iterator i = checkers_.begin(); i != checkers_.end(); ++i) {
    MCE_INFO("FUNCTIONSTAT:funname-->CheckerCounter::" << i->first->checker_key_ 
      << "|" << i->first->checker_value_ << "-->call_times-->" << i->first->counter_ 
      <<"-->total_usetimes-->0-->timeout_times-->0-->ave_usetime-->0-->timeout_rate-->0");
    i->first->counter_ = 0;
  }

  for(hash_map<string, vola_type>::iterator i = tpl_counter_.begin(); i != tpl_counter_.end(); ++i) {
    MCE_INFO("FUNCTIONSTAT:funname-->TplCounter::" << i->first << "-->call_times-->" 
      << i->second <<"-->total_usetimes-->0-->timeout_times-->0-->ave_usetime-->0-->timeout_rate-->0");
    i->second = 0;
  }
}

void CounterTimer::handle() {
  TalkProtoManagerI::instance().PrintCounter();
}
