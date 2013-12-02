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

inline int swap_32(int i32) {
  int temp = (((int)(i32) & 0xff000000) >> 24) | 
    (((int)(i32) & 0x00ff0000) >> 8)  |   
    (((int)(i32) & 0x0000ff00) << 8)  |   
    (((int)(i32) & 0x000000ff) << 24);
  return temp;
}

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
 * 解析widget协议，获得Edm Widget
 **********************************************************************/
bool SendHandler::TraitsEdmWidget(const std::string &widget, std::string &edm_widget) {
  size_t pos = widget.find("\"revolutionType\":\"edm\"");
  if (pos == string::npos){
    edm_widget = widget;
    return true;
  }

  size_t begin_pos = widget.rfind("{", pos);
  size_t end_pos = widget.find("}", pos);
  if ( begin_pos == string::npos || end_pos == string::npos ){
    return false;
  }

  edm_widget = widget.substr(begin_pos, end_pos - begin_pos + 1 );

  return true;
}

/**********************************************************************
 * 使用用户信息、广告信息拼接出新鲜事需要的信息；
 *     其中log是传给新鲜事用于曝光日志的内容
 *          mc是点击串中需要带到点击日志中的内容
 **********************************************************************/
void SendHandler::BuildEdmAndSend(const UserProfile &profile, string &json_str_origin, const AdGroupPtr& group_ptr) { //拼装edm并发送
  string json_str;
  bool res = TraitsEdmWidget(json_str_origin, json_str);
  if ( false == res ) {
    MCE_WARN("SendHandler::BuildEdmAndSend --> TraitsEdmWidget false, widget:" << json_str_origin);
    return;
  }
  //json_str = "{\"key1\":\"value1\",\"key2\":\"value2\",\"key3\":\"value3\"}";   //获取json串 暂时这样写 为了测试
  MCE_DEBUG("BuildEdmAndSend : json_str : "<<json_str);
  time_t send_time = time(NULL);
  int user_id = profile.id();
  Ice::Long group_id = group_ptr->group_id();
  string ip = LogHelper::instance().MakeIp(profile.ip());
  ProcessJsonStr(json_str,"$time",boost::lexical_cast<std::string>(send_time) + "000");
  ProcessJsonStr(json_str,"$userId",boost::lexical_cast<std::string>(user_id));
  ProcessJsonStr(json_str,"$adgroupid",boost::lexical_cast<std::string>(group_ptr->group_id()));
  ProcessJsonStr(json_str, "$creativeId", boost::lexical_cast<std::string>(group_ptr->creative_id()));
  ProcessJsonStr(json_str, "$widgetId", boost::lexical_cast<std::string>(group_ptr->widget_id()));
  ProcessJsonStr(json_str, "$widgetVersion", group_ptr->widget_version());
  ProcessJsonStr(json_str, "$edmID", boost::lexical_cast<std::string>(group_ptr->creative_id()));
  MyUtil::Str2StrMap json_map;
  bool jsontomapsuccess = false;
  jsontomapsuccess = JsonToMap(json_str,json_map);
  if(!jsontomapsuccess) {
    MCE_WARN("SendHandler::BuildEdmAndSend->JsonToMap not success");
    return ;
  }
//nielsen
  if(group_ptr->member_id() == 100000025275) {
    int vuid = swap_32(user_id);
    json_map["titleLink"] = json_map["titleLink"] + boost::lexical_cast<std::string>(vuid);
    json_map["bodyLink"] = json_map["bodyLink"] + boost::lexical_cast<std::string>(vuid);
  }

  LogHelper::instance().ProcessUrl(json_map["titleLink"]);
  LogHelper::instance().ProcessUrl(json_map["bodyLink"]);
  MCE_DEBUG("titleLink:" << json_map["titleLink"]);
  string edm_type_str = json_map["type"];//TODO
  string edm_version_str = "";
  Str2StrMap::const_iterator it = json_map.find("edmVersion");
  if (json_map.end() != it){
      edm_version_str = it->second;//TODO
  }
  //新鲜事的分发表达式
  ostringstream expression;
  //expression << "u(229942474)";
  expression << "u(" << profile.id() << ")";
  json_map["expr"] = expression.str();

  int edm_type_number = 8001;
  int edm_version = 1; //版本号 目前就是1
  try {
    edm_type_number = boost::lexical_cast<int>(edm_type_str);
    if(edm_version_str != "") {
      edm_version = boost::lexical_cast<int>(edm_version_str);
    }
  }catch(std::exception& e) {
    MCE_WARN("SendHandler::BuildEdmAndSend-->"<<edm_type_str<<"-,error:"<<e.what());
  }catch(...){
    MCE_WARN("SendHandler::BuildEdmAndSend->Unknown error");
  }
  MCE_DEBUG("edm_type_number:" << edm_type_number << " edm_version:" << edm_version);
  //int edm_type_number = 8002; //新鲜事小类型号 如 8002 从edm信息中获取

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

