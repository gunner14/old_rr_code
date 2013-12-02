/*
 * SendHandler.cpp
 *
 *  Created on: 2012-2-27
 *      Author: renren
 */
#include "FeedCreatorAdapter.h"
#include "SendHandler.h"
#include "LogHandler.h"
#include "ParametersController.h"
#include <json/json.h>
#include <curl/curl.h>
#include <string>

namespace xce {
namespace ad {

bool SendHandler::handle(MatchHandlerParamter& para) {
  //para可以得到user_id,及edm_id
  //int user_id = para.user_id();
  UserProfile profile = para.userProfile();
  set<SelectedAdGroupPtr> select_groups = para.selected_groups();
  set<SelectedAdGroupPtr>::iterator it;
  for (it = select_groups.begin(); it != select_groups.end(); ++it) {
    string widget = (*it)->widget();
    AdGroupPtr group_ptr = (*it)->group();
    BuildEdmAndSend(profile,widget,group_ptr);  //拼装edm并发送
  }
  return true;
}
/**********************************************************************
 * 将json穿转换成Str2StrMap并且去掉value中的引号和换行符
 **********************************************************************/
bool SendHandler::JsonToMap(const std::string &json_str,MyUtil::Str2StrMap& json_map) {
  Json::Value value;
  Json::Reader reader;
  Json::Value::iterator iter;
  Json::Value::Members members;
  bool parsingSuccessful = reader.parse(json_str,value);
  if (parsingSuccessful) {
    members = value.getMemberNames();
    for(Json::Value::Members::iterator it = members.begin();it != members.end(); ++ it) {
      std::string name = *it;
      std::string value_str;
      /*try {
        value_str = boost::lexical_cast<std::string>(value[name]);
      } catch(std::exception& e) {
        MCE_WARN("SendHandler::BuildEdmAndSend-> boost::lexical_cast error :" << e.what());
      } catch(...) {
        MCE_WARN("SendHandler::BuildEdmAndSend-> boost::lexical_cast error");
      }
      int num = value_str.find("\"");
      value_str.replace(num,1,"");
      num = value_str.rfind("\"");
      value_str.replace(num,1,"");
      num = value_str.rfind("\n");
      value_str.replace(num,1,"");*/
      try {
        value_str = value[name].asString();
      } catch(std::exception& e) {
        MCE_WARN("SendHandler::BuildEdmAndSend->JsonToMap boost::lexical_cast error :" << e.what());
        return false;
      } catch(...) {
        MCE_WARN("SendHandler::BuildEdmAndSend->JsonToMap boost::lexical_cast error");
        return false;
      }
      json_map[name] = value_str;
      //json_map[name] = value_str.substr(1,value_str.length()-3);
      MCE_DEBUG("JsonToMap : key : "<< name << " value : "<< json_map[name]);
    }
  }
  return true;
}

/**********************************************************************
 * 将json字符串中预留的关键字替换成需要的值
 **********************************************************************/
void SendHandler::ProcessJsonStr(string &json_str, string target, string replace) {
  size_t idx = json_str.rfind(target);
  if (idx != string::npos) {   
    json_str.replace(idx, target.length(), replace);
  }
}

/**********************************************************************
 * 使用用户信息、广告信息拼接出新鲜事需要的信息；
 *     其中log是传给新鲜事用于曝光日志的内容
 *          mc是点击串中需要带到点击日志中的内容
 **********************************************************************/
void SendHandler::BuildEdmAndSend(const UserProfile &profile, string &json_str, const AdGroupPtr& group_ptr) { //拼装edm并发送
  //json_str = "{\"key1\":\"value1\",\"key2\":\"value2\",\"key3\":\"value3\"}";   //获取json串 暂时这样写 为了测试
  MCE_DEBUG("BuildEdmAndSend : json_str : "<<json_str);
  time_t send_time = time(NULL);
  int user_id = profile.id();
  Ice::Long group_id = group_ptr->group_id();
  string ip = LogHelper::instance().MakeIp(profile.ip());
  ostringstream log;
  ostringstream mc_info;
  /********************************************************************
   * 曝光日志通过新鲜事数据模板传给新鲜事的字段，格式如下：
   * 服务器名称@ip,user_id,stage,age,gender,grade,home_area_code,current_area_code,user_ip,
   * member_id,member_category,member_industry,creative_id,adgroup_id,campaign_id,price,cost,
   * transtype(消费类型),bidunit_id,adzone_id(暂时使用0),cheat_flag(暂时使用0),
   *  曝光时间,index(曝光时处于新鲜事中的第几条) ------->曝光时间和index由新鲜事在曝光时增加
  *********************************************************************/
  log << EngineConfig::instance().server_name() << "," << profile.id() << "," << profile.stage() << "," << profile.age() << ","
    << profile.gender() << "," << profile.grade() << "," <<  ((profile.home_area().empty()) ? "-" : profile.home_area()) << ","
    << ((profile.current_area().empty()) ? "-" : profile.current_area()) << "," << ip << "," << group_ptr->member_id() << ","
    << group_ptr->member_category() << "," << group_ptr->member_industry() << "," 
    << group_ptr->creative_id() << "," << group_ptr->group_id() << ","
    << group_ptr->campaign_id() << "," << group_ptr->max_price() << "," << "0" << "," << group_ptr->trans_type() << "," << group_ptr->bid_unit_id() << ","
    << "0" << "," << "0" ; 

  /********************************************************************
   * 点击日志需要记录的通过点击跳转url传递，格式如下：
   * 服务器名称@ip,----------->服务器名称在点击时打印
   * user_id,stage,age,gender,grade,home_area_code,current_area_code,user_ip,
   * member_id,member_category,member_industry,creative_id,adgroup_id,campaign_id,price,cost,
   * transtype(消费类型),bidunit_id,adzone_id(暂时使用0),
   * cheat_flag(暂时使用0),点击时间,------->cheat_flag和点击时间在点击时打印
  *********************************************************************/
  mc_info << profile.id() << ","<< profile.stage() << "," << profile.age() << ","
    << profile.gender() << "," << profile.grade() << "," <<  ((profile.home_area().empty()) ? "-" : profile.home_area()) << ","
    << ((profile.current_area().empty()) ? "-" : profile.current_area()) << "," << ip << "," << group_ptr->member_id() << ","
    << group_ptr->member_category() << "," << group_ptr->member_industry() << "," 
    << group_ptr->creative_id() << "," << group_ptr->group_id() << ","
    << group_ptr->campaign_id() << "," << group_ptr->max_price() << "," << "0"<< "," << group_ptr->trans_type() << "," << group_ptr->bid_unit_id() << ","
    << "0" ; 
  
  string mc_str = mc_info.str();
  string mc_str_md5 =LogHelper::instance().GetMD5(mc_str);
  MCE_DEBUG("log md5: GetMD5 str :" << mc_str << " md5: " << mc_str_md5);
  mc_str += "^B";
  mc_str += mc_str_md5;
  LogHelper::instance().ProcessUrl(mc_str);
  ProcessJsonStr(json_str,"$time",boost::lexical_cast<std::string>(send_time) + "000");
  ProcessJsonStr(json_str,"$userId",boost::lexical_cast<std::string>(user_id));
  Ice::Long uid_gid = group_ptr->group_id() + profile.id() * 10000000000;
  //ProcessJsonStr(json_str,"$adgroupid",boost::lexical_cast<std::string>(group_ptr->group_id()));
  ProcessJsonStr(json_str,"$adgroupid",boost::lexical_cast<std::string>(uid_gid));
  ProcessJsonStr(json_str,"$log",log.str());
  ProcessJsonStr(json_str,"$mc",mc_str);
  MyUtil::Str2StrMap json_map;
  bool jsontomapsuccess = false;
  jsontomapsuccess = JsonToMap(json_str,json_map);
  if(!jsontomapsuccess) {
    MCE_WARN("SendHandler::BuildEdmAndSend->JsonToMap not success");
    return ;
  }
  LogHelper::instance().ProcessUrl(json_map["titleLink"]);
  LogHelper::instance().ProcessUrl(json_map["bodyLink"]);
  MCE_DEBUG("titleLink:" << json_map["titleLink"]);
  string edm_type_str = json_map["type"];//TODO
  //新鲜事的分发表达式
  ostringstream expression;
  //expression << "u(229942474)";
  expression << "u(" << profile.id() << ")";
  json_map["expr"] = expression.str();
  MCE_DEBUG("edm_type_str:" << edm_type_str);
  MCE_DEBUG("expr:" << json_map["expr"]);
  int edm_type_number = 8001;
  try {
  edm_type_number = boost::lexical_cast<int>(edm_type_str);
  }catch(std::exception& e) {
    MCE_WARN("SendHandler::BuildEdmAndSend-->"<<edm_type_str<<"-,error:"<<e.what());
  }catch(...){
    MCE_WARN("SendHandler::BuildEdmAndSend->Unknown error");
  }
  MCE_DEBUG("edm_type_number:" << edm_type_number);
  //int edm_type_number = 8002; //新鲜事小类型号 如 8002 从edm信息中获取
  const int edm_version = 1; //版本号 目前就是1
  try {
    //bool edm_send_success = 1;
    bool edm_send_success = xce::feed::FeedCreatorAdapter::instance().CreateEDM(edm_type_number,edm_version,json_map);
    if (!edm_send_success) {
      MCE_WARN("edm send fail : user_id : "<<user_id<<" group_id : "<<group_id);
    } else {
      MCE_DEBUG("edm sended successfully : user_id : "<<user_id<<" group_id : "<<group_id);
    }
  } catch (Ice::Exception& e) {
    MCE_WARN("call FeedCreatorAdapter error -> "<<e);
  } catch (std::exception& ex) {
    MCE_WARN("call FeedCreatorAdapter exception error -> "<<ex.what());
  } catch (...) {
    MCE_WARN("call FeedCreatorAdapter failed");
  }
}

}
}

