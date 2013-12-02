#include "expandfeed.h"
#include "logic_action.h"
#include <stdio.h>
#include <boost/lexical_cast.hpp>
#include "boost/shared_ptr.hpp"
#include <boost/algorithm/string.hpp>

#include "ctemplate/template.h"
#include "ctemplate/template_dictionary.h"

#include <sstream>
#include "feed/base_feed/logging.h"
#include "IceLogger.h"
#include "feed/base_feed/hashmap.h"
// #include "feed/base_feed/once.h"
#include "feed/rfeed_adapter.h"
#include "feedxml.h"
#include "feed_hdf_parser.h"
#include "feed/cwf_feed/site.h"
#include "headurl.h"
#include "feed/ilikeit_adapter.h"
#include <algorithm>
#include "ambcache.h"
#include "whitelist.h"

#include "AdEdmLogAdapter.h"

using namespace xce::notify;

namespace xce {

  const std::string FeedXml::tmp = std::string();

template<typename Target, typename Source>
Target lexical_cast(const Source & arg, const Target & default_target = Target()) {
  try {
    return boost::lexical_cast<Target>(arg);
  }
  catch (boost::bad_lexical_cast* e) {}
  return default_target;
}
//
// <#include "1|4|0">
// 1 表示 feed type
// 4 表示业务，如：0 是网站，99 是page
// 0 对应于 xml 中的 v，
//   如果 v=1.0 此处为 1
//   如果 v=2.0 此处为 2
//   如果没有 v 此处为 0
// 

// CAUTION: 为了支持minifeed，改变了Source 的定义，这里需要去除影响
// 故 & WS_FLAG
std::string GetTemplateFilename(int type, WeiredSource source, int version) {
  for (int i=0; FeedTypeTable[i].type; ++i) {
    if (FeedTypeTable[i].type == type && 
      FeedTypeTable[i].source == (source & WS_FLAG)) {
      char str[256];
      sprintf(str, "%s.%d.tpl", FeedTypeTable[i].filename, version);
      return str;
    }
  }
  return std::string();
}

std::string GetTemplateFilename2(int type, WeiredSource source, int version, int uid) {
  for (int i=0; FeedTypeTable[i].type; ++i) {
    if (FeedTypeTable[i].type == type && 
      FeedTypeTable[i].source == (source & WS_FLAG)) {
      char str[256];
      if((uid%50 == 41) && (source & WS_NEWSFEED)) {
        sprintf(str, "%s.%d.tpl3", FeedTypeTable[i].filename, version);
      } else {
        sprintf(str, "%s.%d.tpl2", FeedTypeTable[i].filename, version);
      }
      return str;
    }
  }
  return std::string();
}

void IfElse(FeedXml *fx, ctemplate::TemplateDictionary* dict
        , const char * key, const char * section_value_name = 0) {

  std::string value = fx->find(key);
  if (!value.empty()) {
    if (section_value_name)
      dict->SetValueAndShowSection(section_value_name, value, key);
    else
      dict->ShowSection(key);
  } else {
    std::string not_key(key);
    not_key += "_NOT";
    dict->ShowSection(not_key);
  }
}

void If(FeedXml *fx, ctemplate::TemplateDictionary* dict
        , const char *key, const char * section_value_name = 0) {
  std::string value = fx->find(key);
  if (!value.empty()) {
    if (section_value_name)
      dict->SetValueAndShowSection(section_value_name, value, key);
    else
      dict->ShowSection(key);
  }
}

void IfNot(FeedXml *fx, ctemplate::TemplateDictionary* dict
        , const char *key,  const char * section_value_name = 0) {
  std::string value = fx->find(key);
  if (value.empty()) {
      std::string not_key(key);
      not_key += "_NOT";
      dict->ShowSection(not_key);
  }
}

void IfNotElse(FeedXml *fx, ctemplate::TemplateDictionary* dict
            , const char *key, const char * section_value_name = 0) {
  
  if (!key)
    return;
  std::string value = fx->find(key);
  if (value.empty()) {
    std::string not_key(key);
    not_key += "_NOT";
    dict->ShowSection(not_key);
  } else {
    if (section_value_name)
      dict->SetValueAndShowSection(section_value_name, value, key);
    else
      dict->ShowSection(key);
  }
}

struct DictBuilder {
  // 整个 fill 周期需要共享一些变量
  std::string vType_;
  int reply_limit_;
  unsigned int render_;
  ctemplate::TemplateDictionary * dict_delete_;

  DictBuilder() : reply_limit_(2), dict_delete_(0) {}

  bool BuildDictionary(const feed::FeedData *feed, FeedXml* fx
    , cwf::User * u, WeiredSource source
    , ctemplate::TemplateDictionary* dict, bool isXML = true, std::string logic_str = "", int owner = -1, const ilikeit::LikeInfo* likeinfo = NULL,  const std::string & template_file = "", int index = WS_HOME, bool  isAdmin = false);

  
  typedef bool (DictBuilder::*GenerateProc)(const feed::FeedData *, FeedXml *, ctemplate::TemplateDictionary *);
  typedef bool (DictBuilder::*GenerateProc2)(const feed::FeedData *, FeedXml *, ctemplate::TemplateDictionary *, cwf::User *, WeiredSource, int, const ilikeit::LikeInfo*, const std::string &, int index, bool isAdmin);

  static GenerateProc FindEntry(int type, int source) {
    struct Entry {
      int type;
      int source;
      GenerateProc proc;
      GenerateProc2 proc2;
       
    };

    #include "table.inc"

    for(const Entry *e=arr; e->type; ++e) {
      if (e->type == type && e->source == source) {
          return e->proc;
      }
    }
    return 0;
  }

  static GenerateProc2 FindEntry2(int type, int source) {
    struct Entry {
      int type;
      int source;
      GenerateProc proc;
      GenerateProc2 proc2;
       
    };

    #include "table.inc"

    for(const Entry *e=arr; e->type; ++e) {
      if (e->type == type && e->source == source) {
          return e->proc2;
      }
    }
    return 0;
  }

  #include "body.inc"

  bool Genrate(const feed::FeedData *feed, int index, FeedXml* fx, ctemplate::TemplateDictionary* dict, cwf::User * u, WeiredSource source, int owner, const ilikeit::LikeInfo* likeinfo,  const std::string & template_file, bool isXML, std::string logic_str, bool isAdmin = false) {

    if (isXML) {

      GenerateProc proc  = FindEntry(WeiredType(feed->type).type, index);
      if (proc) {
        return ((*this).*proc)(feed, fx, dict);
      }

      GenerateProc2 proc2  = FindEntry2(WeiredType(feed->type).type, index);
      if (proc2) {
        return ((*this).*proc2)(feed, fx, dict, u, source, owner, likeinfo, template_file, index, isAdmin);
      } 
      return fill_default(feed, fx, dict);

    } else {
      std::map<std::string, boost::shared_ptr<LogicAction> > root_logic_map = LogicAction::ParseLogicStr(logic_str, feed, fx, dict, u, source, owner, likeinfo, template_file, index, isAdmin);
      bool ret = true; 
      for (std::map<std::string, boost::shared_ptr<LogicAction> >::iterator it = root_logic_map.begin(); it != root_logic_map.end(); it++) {
        boost::shared_ptr<LogicAction> logic = it->second;
        if (!logic->BuildDict()) {
          ret = false;
        }
      }
      return ret; 
    }
  }
};

// 专门处理评论类新鲜事包括(2901, 2902, 2906, 2905)  
bool BuildCommentFeedHdf(const feed::FeedData *feed
    , FeedXml* fx , HdfMap* hdf_map) {
  /*
  if (2901 != WeiredType(feed->type).type 
      && 2902 != WeiredType(feed->type).type
      && 2906 != WeiredType(feed->type).type
      && 2905 != WeiredType(feed->type).type) {
    return false;
  }

  // for AVATAR_COMMENT
  // f_from_id, vType, f_from_tinyimg
  // rename to: ID, TYPE, TINYIMG
  {
    ctemplate::TemplateDictionary* sub_dict = dict->AddIncludeDictionary("AVATAR_COMMENT");
    //  xml_version == "1" ? "AVATAR_1" : "AVATAR_1");
    sub_dict->SetFilename("avatar_comment.ftl.1.tpl2");
    sub_dict->SetValue("new_comment_from_id", fx->find("f_comment_from_id"));
    sub_dict->SetValue("new_comment_from_name", fx->find("f_comment_from_name"));

    std::string new_comment_from_tinyimg = fx->find("f_comment_from_tinyimg");
    new_comment_from_tinyimg = HeadUrl::FromPart(new_comment_from_tinyimg);
    sub_dict->SetValue("new_comment_from_tinyimg", new_comment_from_tinyimg);

    sub_dict->SetValue("vType", vType_);
    sub_dict->SetFormattedValue("id", "%llu", feed->feed); 

    sub_dict->SetIntValue("f_type", WeiredType(feed->type).type);
  }

  // for TOOLBAR_COMMENT =  DELETE1 + DELETE2 + focus  评论类新鲜事，目前处理2901, 2902, 2906, 2905
  ctemplate::TemplateDictionary* inc = dict->AddIncludeDictionary("TOOLBAR_COMMENT");
  inc->SetFilename("toolbar_comment.ftl.1.tpl2");

  inc->SetFormattedValue("id", "%llu", feed->feed);
  inc->SetIntValue("f_type", WeiredType(feed->type).type);
  inc->SetValue("new_comment_from_id", fx->find("f_comment_from_id"));
  inc->SetValue("new_comment_from_name", fx->find("f_comment_from_name"));

  unsigned int flag = render_;
  if (flag & WS_MINIFEED) {
    inc->ShowSection("isNewsFeed_NOT");
  } else {
    inc->ShowSection("isNewsFeed");
    if (u && feed->actor != u->id()) {
      inc->ShowSection("SELF_NOT"); //只有toolbar需要show SELF_NOT
    }
  }

  if (u) {
    std::string uid = lexical_cast<std::string>(u->id());
    if (uid == fx->find("f_comment_from_id")) {
      inc->ShowSection("SELF");
    }

    inc->SetIntValue("userID", u->id());
  }
  */
  return true;
}

void BuildAvatar() {
  /*
  ctemplate::TemplateDictionary* sub_dict = dict->AddIncludeDictionary("AVATAR_1");
  //  xml_version == "1" ? "AVATAR_1" : "AVATAR_1");
  sub_dict->SetFilename("avatar.ftl.1.tpl2");
  sub_dict->SetValue("f_from_id", fx->find("f_from_id"));
  sub_dict->SetValue("f_from_name", fx->find("f_from_name"));
  sub_dict->SetValue("f_from_tinyimg", fx->find("f_from_tinyimg"));
  IfElse(fx, sub_dict, "f_from_url", NULL); 
  IfElse(fx, sub_dict, "f_from_fullimg", NULL); 
  sub_dict->SetValue("vType", vType_);
  sub_dict->SetFormattedValue("id", "%llu", feed->feed); 

  sub_dict->SetIntValue("f_type", WeiredType(feed->type).type);

  IfElse(fx, sub_dict, "f_from_fullimg", "f_from_fullimg");
  IfElse(fx, sub_dict, "f_from_url", "f_from_url");
  */
}

void BuildSource() {
  /*
  ctemplate::TemplateDictionary* sub_dict = dict->AddIncludeDictionary(xml_version == "1" ? "SOURCE_1" : "SOURCE");
  sub_dict->SetFilename(xml_version == "1" ? "source.ftl.1.tpl" : "source.ftl.tpl");
  if (fx->find("f_origin_type") == "1") {
    sub_dict->SetValueAndShowSection("vType", vType_, "TYPE_EQUAL_1");
  } else {
    sub_dict->ShowSection("TYPE_EQUAL_1_NOT");
  }
  sub_dict->SetValue("f_origin_url",fx->find("f_origin_url"));
  If(fx, sub_dict, "f_origin_image");
  If(fx, sub_dict, "f_origin_title");
  sub_dict->SetValue("f_origin_title", fx->find("f_origin_title"));
  sub_dict->SetValue("f_origin_image", fx->find("f_origin_image"));
  */
}

void BuildToolbar() {
  /*
  ctemplate::TemplateDictionary* inc = dict->AddIncludeDictionary("TOOLBAR_1");
  inc->SetFilename("toolbar.ftl.1.tpl2");

  ctemplate::TemplateDictionary* inc_nofocus = dict->AddIncludeDictionary("TOOLBAR_NOFOCUS_1");
  inc_nofocus->SetFilename("toolbar_nofocus.ftl.1.tpl2");

  ctemplate::TemplateDictionary* del_inc = 0;

  inc->SetFormattedValue("id", "%llu", feed->feed);
  inc->SetIntValue("f_type", WeiredType(feed->type).type);
  inc->SetValue("f_from_id", fx->find("f_from_id"));
  inc->SetValue("f_from_name", fx->find("f_from_name"));

  inc_nofocus->SetFormattedValue("id", "%llu", feed->feed);
  inc_nofocus->SetIntValue("f_type", WeiredType(feed->type).type);
  inc_nofocus->SetValue("f_from_id", fx->find("f_from_id"));
  inc_nofocus->SetValue("f_from_name", fx->find("f_from_name"));


  std::string merge_count = fx->find("merge_feed_count");
  std::vector<std::string> mergev = fx->find_v("merge_feed_fid");
  std::string fids;

  unsigned int flag = render_;
  if (flag & WS_MINIFEED) {
    inc->ShowSection("isNewsFeed_NOT");
    inc_nofocus->ShowSection("isNewsFeed_NOT");
  } else {
    inc->ShowSection("isNewsFeed");
    inc_nofocus->ShowSection("isNewsFeed");
    if (u && feed->actor != u->id()) {
      inc->ShowSection("SELF_NOT"); //只有toolbar需要show SELF_NOT
    }
  }

  if (u) {
    std::string uid = lexical_cast<std::string>(u->id());
    if ((uid == fx->find("f_from_id")) || isAdmin) {
      inc->ShowSection("SELF");
      inc_nofocus->ShowSection("SELF");

      if (flag & WS_MINIGROUP) {
        inc_nofocus->ShowSection("MINIGROUP");
      } else {
        inc_nofocus->ShowSection("MINIGROUP_NOT");
      }
    }

    inc->SetIntValue("userID", u->id());
    inc_nofocus->SetIntValue("userID", u->id());
  }

  // merge
  if (!merge_count.empty()) {
    std::string isNewVersionVideoFeed = fx->find("newversion"); //识别是视频分享是不是新版

    if (isNewVersionVideoFeed.empty()) {  //视频分享老版本才显示相似新鲜事条数
      del_inc = dict->AddIncludeDictionary("DELETE_1"); //这里的DELETE现在仅仅表示“显示相似的几条新鲜事”
      del_inc->SetFilename("delete.ftl.1.tpl2");

      del_inc->ShowSection("merge_feed_count");
      // 手工拼接 feedIDs
      del_inc->SetValue("merge_feed_count", merge_count); 
      // inc->ShowSection("typeText_NOT");
      fids = boost::join(mergev, ",");
      del_inc->SetValue("feedIDs", fids);

    }
  }

  char first_id[40];
  sprintf(first_id,"%llu",feed->feed);

  fids = "," + fids;
  std::string id_list = first_id  + fids;
  dict->SetValue("id_list", id_list);  //在顶层模板填充id_list，目前只有share_video需要
  
  dict_delete_ = del_inc;
  */
}

bool BuildHdf(const feed::FeedData *feed
    , cwf::User * u
    , WeiredSource source
    , HdfMap * hdf_map) {
/*
  // f_blog_deny_id 支持
  // 在minifeed中，对某些人实施过滤
  if (source & WS_MINIFEED) {
    if (601 == WeiredType(feed->type).type) {
      // BOOST_FOREACH(StringMap& map, fx->body) {
      std::vector<std::string> deny_id = fx->find_v("f_blog_deny_id");
      if (u) {
        BOOST_FOREACH(std::string& id, deny_id) {
        if (u->id() == lexical_cast(id, 0))
          return false;
        }
      }
    }
  }

  StringMap::iterator fromimgit = fx->body.find("f_from_tinyimg");
  if (fromimgit != fx->body.end()) {
    fromimgit->second = HeadUrl::FromPart(fromimgit->second);
  }

  if (8007 == WeiredType(feed->type).type) { // 对8007的一些空缺属性填充
    const std::string & from = fx->find("f_entry_from_tinyimg");
    if (!from.empty())
      fx->body.insert(std::make_pair("f_from_tinyimg",from));
    const std::string & id = fx->find("f_entry_from_id");
    if (!id.empty()) 
      fx->body.insert(std::make_pair("f_from_id",id));
    const std::string & name = fx->find("f_from_name");
    if (!name.empty()) 
      fx->body.insert(std::make_pair("f_from_name",name));    
  }

  // 3 body
  fx->body["f_type"] = lexical_cast<std::string>(WeiredType(feed->type).type);
  for (StringMap::const_iterator i=fx->body.begin(); i!=fx->body.end(); ++i) {
    hdf_map->insert(make_pair(i->first, i->second));
  }

  std::string xml_version = fx->template_version;

  hdf_map->insert(make_pair("id", boost::lexical_cast<string>(feed->feed)));

  BuildAvatar();
  BuildSource();
  BuildToolbar();
  BuildCommentFeedHdf(feed, fx, hdf_map);
*/
  return true;
}

bool DictBuilder::BuildDictionary(const feed::FeedData *feed, FeedXml* fx
     , cwf::User * u, WeiredSource source
     , ctemplate::TemplateDictionary* dict, bool isXML, std::string logic_str, int owner, const ilikeit::LikeInfo* likeinfo,  const std::string & template_file, int index, bool isAdmin) {
  // f_blog_deny_id 支持
  // 在minifeed中，对某些人实施过滤
  if (source & WS_MINIFEED) {
    if (601 == WeiredType(feed->type).type) {
      // BOOST_FOREACH(StringMap& map, fx->body) {
      std::vector<std::string> deny_id = fx->find_v("f_blog_deny_id");
      if (u) {
        BOOST_FOREACH(std::string& id, deny_id) {
        if (u->id() == lexical_cast(id, 0))
          return false;
        }
      }
    }
  }
  
  // 把 f_from_tinyimg 地址补全,暂时先用着
//   VectorMap::iterator vit = fx->ls.find("f_from_tinyimg");
//   if (fx->ls.end() != vit){
//     BOOST_FOREACH(std::string& img, vit->second) {
//       if (!img.empty()) {
//         img = HeadUrl::FromPart(img);
//       }
//     }
//   }
  StringMap::iterator fromimgit = fx->body.find("f_from_tinyimg");
  if (fromimgit != fx->body.end())
    fromimgit->second = HeadUrl::FromPart(fromimgit->second);

  if (8007 == WeiredType(feed->type).type) { // 对8007的一些空缺属性填充
    const std::string & from = fx->find("f_entry_from_tinyimg");
    if (!from.empty())
      fx->body.insert(std::make_pair("f_from_tinyimg",from));
    const std::string & id = fx->find("f_entry_from_id");
    if (!id.empty()) 
      fx->body.insert(std::make_pair("f_from_id",id));
    const std::string & name = fx->find("f_from_name");
    if (!name.empty()) 
      fx->body.insert(std::make_pair("f_from_name",name));    
  }

  // 2 f_entry_attachments_attachment_src[0] 
  // attachments 固然只用在了asp/PhotoPublishMore, 但是对于其他类型来说
  // 需要第一个 attachment
  /*if (!fx->attachment.empty()) {
    const StringMap & map = *(fx->attachment.begin());
    for (StringMap::const_iterator i=map.begin(); i!=map.end(); ++i) {
      //dict->SetValue(i->first, i->second);
      fx->body.front().insert(std::make_pair(i->first,i->second));
    }
  }*/

  // 3 body

  fx->body["f_type"] = lexical_cast<std::string>(WeiredType(feed->type).type);
  for (StringMap::const_iterator i=fx->body.begin(); i!=fx->body.end(); ++i) {
    dict->SetValue(i->first, i->second);
  }

  // 3 photo[0] 
  // photo 固然只用在了PhotoPublishMore, 但是对于其他类型来说
  // 需要第一个 photo
  /*if (!fx->photo.empty()) {
    const StringMap & map = *(fx->photo.begin());
    for (StringMap::const_iterator i=map.begin(); i!=map.end(); ++i) {
      dict->SetValue(i->first, i->second);
    }
  }*/
 
  std::string xml_version = fx->template_version;

  // win32 long 是32位的，不能调用SetIntValue
  dict->SetFormattedValue("id", "%llu", feed->feed);

//   if (u) { // 多余了
//     dict->SetIntValue("userID", u->id());
//     dict->SetValue("userName", u->name());
//   }
  
  // for AVATAR
  // f_from_id, vType, f_from_tinyimg
  // rename to: ID, TYPE, TINYIMG
  {
    ctemplate::TemplateDictionary* sub_dict = dict->AddIncludeDictionary("AVATAR_1");
    //  xml_version == "1" ? "AVATAR_1" : "AVATAR_1");
    sub_dict->SetFilename("avatar.ftl.1.tpl2");
    sub_dict->SetValue("f_from_id", fx->find("f_from_id"));
    sub_dict->SetValue("f_from_name", fx->find("f_from_name"));
    sub_dict->SetValue("itemsource", fx->find("itemsource"));
    if(!(source & WS_MINIFEED)){
      int white_view = xce::feed::Whitelist::instance().IsWhiteUser(u->id());
      if(white_view > 0){
        if(feed->weight & 0x01){
          sub_dict->SetValue("attention","取消");
          sub_dict->SetValue("isfouce","");
          sub_dict->SetValue("mark","0");
          sub_dict->SetValue("tipcontent","TA的新鲜事正在优先展示。点击取消");
        } else {
          sub_dict->SetValue("attention","");
          sub_dict->SetValue("isfouce"," unfouce");
          sub_dict->SetValue("mark","1");
          sub_dict->SetValue("tipcontent","点击这里。优先展示TA的新鲜事");
        }
        sub_dict->ShowSection("unread");
      }
    }
    sub_dict->SetValue("f_from_tinyimg", fx->find("f_from_tinyimg"));
    IfElse(fx, sub_dict, "f_from_url", NULL); 
    IfElse(fx, sub_dict, "f_from_fullimg", NULL); 
    sub_dict->SetValue("vType", vType_);
    sub_dict->SetFormattedValue("id", "%llu", feed->feed); 

    sub_dict->SetIntValue("f_type", WeiredType(feed->type).type);

    IfElse(fx, sub_dict, "f_from_fullimg", "f_from_fullimg");
    IfElse(fx, sub_dict, "f_from_url", "f_from_url");

    std::string strVipHatId; 
    strVipHatId = fx->find("userVipHatId");
    if(!strVipHatId.empty()) {
      sub_dict->SetValue("userVipHatId", strVipHatId);
      sub_dict->ShowSection("userVipHatId");
    }
  }

  // for SOURCE
  // f_origin_type == 1, vType
  // rename to: TYPE_EQUAL_1 TYPE

  // condition: f_origin_type f_status_streamID f_origin_title
  // f_origin_url, f_origin_title
  // rename: FROM, URL, TITLE

  // condition: f_origin_image
  // rename to: WTIHIMG, URL, IMG
  {
    ctemplate::TemplateDictionary* sub_dict = dict->AddIncludeDictionary(
      xml_version == "1" ? "SOURCE_1" : "SOURCE");
    sub_dict->SetFilename(xml_version == "1" ? "source.ftl.1.tpl" : "source.ftl.tpl");
    if (fx->find("f_origin_type") == "1") {
      sub_dict->SetValueAndShowSection("vType", vType_, "TYPE_EQUAL_1");
    } else {
      sub_dict->ShowSection("TYPE_EQUAL_1_NOT");
    }
    sub_dict->SetValue("f_origin_url",fx->find("f_origin_url"));
    If(fx, sub_dict, "f_origin_image");
    If(fx, sub_dict, "f_origin_title");
    sub_dict->SetValue("f_origin_title", fx->find("f_origin_title"));
    sub_dict->SetValue("f_origin_image", fx->find("f_origin_image"));
  }

  // for TOOLBAR_1 =  DELETE1 + DELETE2 + focus
  // TOOLBAR_NOFOCUS_1 =  DELETE1 + DELETE2 
  {
    ctemplate::TemplateDictionary* inc = dict->AddIncludeDictionary("TOOLBAR_1");
    inc->SetFilename("toolbar.ftl.1.tpl2");

    ctemplate::TemplateDictionary* inc_nofocus = dict->AddIncludeDictionary("TOOLBAR_NOFOCUS_1");
    inc_nofocus->SetFilename("toolbar_nofocus.ftl.1.tpl2");

    ctemplate::TemplateDictionary* del_inc = 0;

    inc->SetFormattedValue("id", "%llu", feed->feed);
    inc->SetIntValue("f_type", WeiredType(feed->type).type);
    inc->SetValue("f_from_id", fx->find("f_from_id"));
    inc->SetValue("f_from_name", fx->find("f_from_name"));

    inc_nofocus->SetFormattedValue("id", "%llu", feed->feed);
    inc_nofocus->SetIntValue("f_type", WeiredType(feed->type).type);
    inc_nofocus->SetValue("f_from_id", fx->find("f_from_id"));
    inc_nofocus->SetValue("f_from_name", fx->find("f_from_name"));


    std::string merge_count = fx->find("merge_feed_count");
    std::vector<std::string> mergev = fx->find_v("merge_feed_fid");
    std::string fids;

    unsigned int flag = render_;
    if (flag & WS_MINIFEED) {
      inc->ShowSection("isNewsFeed_NOT");
      inc_nofocus->ShowSection("isNewsFeed_NOT");
    } else {
      inc->ShowSection("isNewsFeed");
      inc_nofocus->ShowSection("isNewsFeed");
      if (u && feed->actor != u->id()) {
        inc->ShowSection("SELF_NOT"); //只有toolbar需要show SELF_NOT
      }
    }

    if (u) {
      std::string uid = lexical_cast<std::string>(u->id());
      if ((uid == fx->find("f_from_id")) || isAdmin) {
        inc->ShowSection("SELF");
        inc_nofocus->ShowSection("SELF");
  
        if (flag & WS_MINIGROUP) {
          inc_nofocus->ShowSection("MINIGROUP");
        } else {
          inc_nofocus->ShowSection("MINIGROUP_NOT");
        }
      }

      inc->SetIntValue("userID", u->id());
      inc_nofocus->SetIntValue("userID", u->id());
    }

    // merge
    if (!merge_count.empty()) {
      std::string isNewVersionVideoFeed = fx->find("newversion"); //识别是视频分享是不是新版

      if (isNewVersionVideoFeed.empty()) {  //视频分享老版本才显示相似新鲜事条数
        del_inc = dict->AddIncludeDictionary("DELETE_1"); //这里的DELETE现在仅仅表示“显示相似的几条新鲜事”
        del_inc->SetFilename("delete.ftl.1.tpl2");

        del_inc->ShowSection("merge_feed_count");
        // 手工拼接 feedIDs
        del_inc->SetValue("merge_feed_count", merge_count); 
        // inc->ShowSection("typeText_NOT");
        fids = boost::join(mergev, ",");
        del_inc->SetValue("feedIDs", fids);

      }
      
    }

    char first_id[40];
    sprintf(first_id,"%llu",feed->feed);

    fids = "," + fids;
    std::string id_list = first_id  + fids;
    dict->SetValue("id_list", id_list);  //在顶层模板填充id_list，目前只有share_video需要
    
    dict_delete_ = del_inc;
  }

  {
    if((u->id() % 50 == 41) && (source & WS_NEWSFEED)) {
      ctemplate::TemplateDictionary* sub_dict = dict->AddIncludeDictionary("REPLY_1");
      sub_dict->SetFilename("reply.ftl.1.tpl3");
      std::string userID = lexical_cast<std::string>(u->id());
      sub_dict->SetValue("userID", userID);
      sub_dict->SetFormattedValue("id", "%llu", feed->feed);
      sub_dict->SetFormattedValue("itemsource", "%llu", feed->source);
      sub_dict->SetValue("f_from_id", fx->find("f_from_id"));
      sub_dict->SetValue("vType", vType_);
      int showEmotionBtn = 1, showAtBtn = 1, showShareBtn = 1;
      int replyTypeId = -1;
      std::string replyTextMaxLen = "140";
      std::string replyType;
      std::string qunUgcType;
      int replyURI = 0;
      switch (WeiredType(feed->type).type) {
        //case 101 : case 102 : case 103 : case 104 : case 105 : case 106 : case 107 : case 108 : case 109 : case 110 : case 111 : case 112 : case 113 : case 114 : case 115 : case 126 : case 130 : case 131 : case 133 : case 134 : case 135 : case 136 : case 3801: case 3802: case 3803: case 3804: case 3805: case 5030:
        //           replyTypeId = 4 ; replyType = "share"  ; break;
        case 121 : case 122 : case 123 :
                   replyTypeId = 5 ; replyType = "share_edm"  ; showShareBtn = 0; break;
        case 8001: case 8002: case 8003: case 8004: case 8012:
                   replyTypeId = 5 ; replyType = "edm"  ; showEmotionBtn = 0; showAtBtn = 0; showShareBtn = 0; break;
        case 213 : replyTypeId = 25; replyType = "share" ; showEmotionBtn = 0; showAtBtn = 0; showShareBtn = 0; break;
        case 502 : replyTypeId = 3 ; replyType = "status" ; replyTextMaxLen = "240"; break;
        case 601 : case 2901:
                   replyTypeId = 0 ; replyType = "blog"   ; break;
        case 701 : case 708 : case 2902: case 2905:
                   replyTypeId = 1 ; replyType = "photo"  ; break;
        case 709 : case 2906:
                   replyTypeId = 2 ; replyType = "album"  ; break;
        case 801 : replyTypeId = 17; replyType = "gift"   ; showShareBtn = 0; break;
        case 802 : replyTypeId = 18; replyType = "gift"   ; showShareBtn = 0; break;
        case 903 : replyTypeId = 14; replyType = "music"  ; showEmotionBtn = 0; showAtBtn = 0; showShareBtn = 0; break;
        case 1601: case 1602 : case 1603:
                   replyTypeId = 4; replyType = "minigroup"; replyURI = 4; qunUgcType = "share"; showAtBtn = 0; showShareBtn = 0; break;
        case 1604: replyTypeId = 0; replyType = "minigroup"; replyURI = 4; qunUgcType = "blog"; showAtBtn = 0; showShareBtn = 0; break;
        case 1605: replyTypeId = 3; replyType = "minigroup"; replyURI = 4; qunUgcType = "status"; showAtBtn = 0; showShareBtn = 0; break;
        case 1606: replyTypeId = 1; replyType = "minigroup"; replyURI = 4; qunUgcType = "photo"; showAtBtn = 0;  showShareBtn = 0; break;
        case 8301: replyTypeId = 20; replyType = "gift"   ; showEmotionBtn = 0; showShareBtn = 0; break;
        case 1101: case 1102:
                   replyTypeId = 11; replyType = "checkin"; showShareBtn = 0; break;
        case 1103: replyTypeId = 13; replyType = "checkin"; showShareBtn = 0; break;
        case 1104: replyTypeId = 15; replyType = "checkin"; showShareBtn = 0; break;
        case 8183: replyTypeId = 10; replyType = "share"  ; break;
        case 8186: replyTypeId = 6 ; replyType = "app"    ; showShareBtn = 0; break;
        case 8190: replyTypeId = 7 ; replyType = "app"    ; showShareBtn = 0; break;
        case 8210: replyTypeId = 19; replyType = "video"  ; showShareBtn = 0; break;
        case 2012:                   replyType = "page"   ; replyURI = 1; showEmotionBtn = 0; showShareBtn = 0; break;
        case 2013:                   replyType = "page"   ; replyURI = 2; showEmotionBtn = 0; showShareBtn = 0; break;
        case 2008: case 2208 :
                   replyTypeId = 3 ; replyType = "page"   ; replyURI = 3; showEmotionBtn = 0; showShareBtn = 0; break;
        default: replyTypeId = 4; replyType = "share"; break;
      }
      if(replyTypeId >= 0) {
        sub_dict->SetIntValue("replyTypeId", replyTypeId);
      }
      sub_dict->SetValue("replyType", replyType);
      sub_dict->SetValue("replyTextMaxLen", replyTextMaxLen);

      if(replyURI == 1) {
        sub_dict->ShowSection("pageBlogURI");
      } else if(replyURI == 2) {
        sub_dict->ShowSection("pageAlbumURI");
      } else if(replyURI == 3) {
        sub_dict->ShowSection("pageDoingURI");
      } else if(replyURI == 4) {
        sub_dict->SetValue("qunUgcType", qunUgcType);
        sub_dict->SetValue("qunId", fx->find("f_minigroup_id"));
        sub_dict->SetValue("commentId", "{{commentId}}");
        sub_dict->ShowSection("qunURI");
      } else {
        sub_dict->ShowSection("commonURI");
      }

      if(showEmotionBtn == 1) {
        sub_dict->ShowSection("showEmotionBtn");
      }
      if(showAtBtn == 1) {
        sub_dict->ShowSection("showAtBtn");
      }
      if(showShareBtn == 1) {
        sub_dict->ShowSection("showShareBtn");
      }

      int count = -1;
      const std::vector<std::string> & reply_v = fx->find_v("f_reply_id");
      BOOST_FOREACH(const std::string& r, reply_v) {
        if (++count == 2) {
          break;
        }
        std::string i = lexical_cast<std::string>(count);
        std::string reply_from_id = fx->find("f_reply_from_id", count);

        sub_dict->SetValue("f_reply_" + i + "_id", r);
        sub_dict->SetValue("f_reply_" + i + "_from_tinyimg", fx->find("f_reply_from_tinyimg", count));
        sub_dict->SetValue("f_reply_" + i + "_body", fx->find("f_reply_body", count));
        sub_dict->SetValue("f_reply_" + i + "_im_body", fx->find("f_reply_from_id", count));
        sub_dict->SetValue("f_reply_" + i + "_from_name", fx->find("f_reply_from_name", count));
        sub_dict->SetValue("f_reply_" + i + "_time", fx->find("f_reply_time", count));
        sub_dict->SetValue("f_reply_" + i + "_type", fx->find("f_reply_type", count));
        sub_dict->SetValue("f_reply_" + i + "_from_id", reply_from_id);

        if(reply_from_id == userID || ((WeiredType(feed->type).type !=121 && WeiredType(feed->type).type != 122 && WeiredType(feed->type).type != 123) && userID == fx->find("f_from_id"))) {
          sub_dict->ShowSection("reply" + i + "DeleteBtn");
        }
        if(reply_from_id != userID && (WeiredType(feed->type).type !=2012 && WeiredType(feed->type).type != 2008 && WeiredType(feed->type).type != 2013)) {
          sub_dict->ShowSection("reply" + i + "ReplyBtn");
        }
        sub_dict->ShowSection("f_reply_" + i + "_id");
      } 

      int replycount = 0;
      std::string strReplyCount = fx->find("reply_count");
      if(!strReplyCount.empty()) {
        replycount = boost::lexical_cast<int>(strReplyCount);
        sub_dict->SetValue("reply_count", strReplyCount);
        if(replycount > 2) {
          std::string strReplyCount2 = lexical_cast<std::string>(replycount - 2);
          sub_dict->SetValue("reply_count2", strReplyCount2);
          sub_dict->ShowSection("reply_count2");
        }
      }

      if (likeinfo) {
        ostringstream os;
        int total = likeinfo->totalCount;
        if (likeinfo->selfCount) {
          total -= 1;
          if (total < 0) {
            MCE_WARN("zan error total :" << total);
          } else if(total > 0) {
            sub_dict->ShowSection("isNotOnlySelfZan");
            os << "我和" << total << "个人很喜欢！" << std::flush;
          } else if(total == 0) {
            os << "我很喜欢！" << std::flush;
          }
          sub_dict->SetValue("selfzancount", lexical_cast<std::string>(likeinfo->selfCount));
        } else if(total > 0) {
          sub_dict->ShowSection("isNotOnlySelfZan");
          os << total << "个人很喜欢！" << std::flush;
        }
        if(total > 0) {
          sub_dict->SetValue("totalzancount", lexical_cast<std::string>(total));
        }
        if(total > 0 || likeinfo->selfCount) {
          sub_dict->SetValue("likeText", os.str());
          sub_dict->ShowSection("likeText");
        }
      }
    }
  }

   // 专门处理评论类新鲜事包括(2901, 2902, 2906, 2905)  
  if (2901 == WeiredType(feed->type).type  || 2902 == WeiredType(feed->type).type || 2906 == WeiredType(feed->type).type || 2905 == WeiredType(feed->type).type) {
     
    // for AVATAR_COMMENT
    // f_from_id, vType, f_from_tinyimg
    // rename to: ID, TYPE, TINYIMG
    {
      ctemplate::TemplateDictionary* sub_dict = dict->AddIncludeDictionary("AVATAR_COMMENT");
      //  xml_version == "1" ? "AVATAR_1" : "AVATAR_1");
      sub_dict->SetFilename("avatar_comment.ftl.1.tpl2");
      sub_dict->SetValue("new_comment_from_id", fx->find("f_comment_from_id"));
      sub_dict->SetValue("new_comment_from_name", fx->find("f_comment_from_name"));

      std::string new_comment_from_tinyimg = fx->find("f_comment_from_tinyimg");
      new_comment_from_tinyimg = HeadUrl::FromPart(new_comment_from_tinyimg);
      sub_dict->SetValue("new_comment_from_tinyimg", new_comment_from_tinyimg);

      sub_dict->SetValue("vType", vType_);
      sub_dict->SetFormattedValue("id", "%llu", feed->feed); 

      sub_dict->SetIntValue("f_type", WeiredType(feed->type).type);

      std::string strVipHatId; 
      strVipHatId = fx->find("userVipHatId");
      if(!strVipHatId.empty()) {
        sub_dict->SetValue("userVipHatId", strVipHatId);
        sub_dict->ShowSection("userVipHatId");
      }
    }

    // for TOOLBAR_COMMENT =  DELETE1 + DELETE2 + focus  评论类新鲜事，目前处理2901, 2902, 2906, 2905
    ctemplate::TemplateDictionary* inc = dict->AddIncludeDictionary("TOOLBAR_COMMENT");
    inc->SetFilename("toolbar_comment.ftl.1.tpl2");

    inc->SetFormattedValue("id", "%llu", feed->feed);
    inc->SetIntValue("f_type", WeiredType(feed->type).type);
    inc->SetValue("new_comment_from_id", fx->find("f_comment_from_id"));
    inc->SetValue("new_comment_from_name", fx->find("f_comment_from_name"));


    unsigned int flag = render_;
    if (flag & WS_MINIFEED) {
      inc->ShowSection("isNewsFeed_NOT");
    } else {
      inc->ShowSection("isNewsFeed");
      if (u && feed->actor != u->id()) {
        inc->ShowSection("SELF_NOT"); //只有toolbar需要show SELF_NOT
      }
    }

    if (u) {
      std::string uid = lexical_cast<std::string>(u->id());
      if (uid == fx->find("f_comment_from_id")) {
        inc->ShowSection("SELF");
      }

      inc->SetIntValue("userID", u->id());
    }
  }

  // TODO: index 是没有必要的
  return Genrate(feed, WS_HOME, fx, dict, u, source, owner, likeinfo, template_file, isXML, logic_str, isAdmin);
} // BuildDictionary

std::string GetCSTplKey(int type, int version, int view) {
  char str[256];
  str[255] = 0;
  snprintf(str, 254, "cs_%d_%d_%d", type, version, view);
  return str;
}

bool FeedTplCache::Reload(int is_test) {
#if 1
  com::xiaonei::xce::Statement sql;
  sql << "select d.stype AS d_stype, d.version AS d_version, t.view AS t_view, t.template AS t_template from data_config as d, tpl_view as t ";

  if (is_test) {
    sql << "WHERE d.test_tpl_id = t.tpl_id";
  } else {
    sql << "WHERE d.using_tpl_id = t.tpl_id AND t.status > 0";
  }

  mysqlpp::StoreQueryResult result;

  try {
    result = com::xiaonei::xce::QueryRunner("feed_admin_db", com::xiaonei::xce::CDbRServer).store(sql);
  } catch(std::exception & e) {
    MCE_WARN("LoadCSTemplates sql query error:" << e.what());
    return false;
  }

  if (!result) {
    MCE_WARN("LoadCSTemplates sql query return NULL.");
    return false;
  }
  HdfMap init_vars;
  init_vars["AVATAR"] = "../etc/feed-cs-tpl/avatar.1.cs";
  init_vars["SOURCE"] = "../etc/feed-cs-tpl/source.1.cs";
  init_vars["TOOLBAR"] = "../etc/feed-cs-tpl/toolbar.1.cs";
  init_vars["SIMILAR"] = "../etc/feed-cs-tpl/similar.1.cs";
  init_vars["REPLY"] = "../etc/feed-cs-tpl/reply.1.cs";
  init_vars["ACTOR"] = "../etc/feed-cs-tpl/actor.1.cs";
  init_vars["REPLY_2"] = "../etc/feed-cs-tpl/reply.2.cs";

  map<string, TplVector*> tpl_cache;
  MCE_INFO("clearsilver template count: " << result.num_rows());
  for (size_t i = 0; i < result.num_rows(); ++i) {
    const mysqlpp::Row & row = result[i];
    string key = GetCSTplKey((int)row["d_stype"], (int)row["d_version"], (int)row["t_view"]);
    
    TplVector * tpl_vec = new TplVector((std::string)row["t_template"], init_vars);
    string tpl_content = (std::string)row["t_template"];

    for (size_t i = 0; i < tpl_content.size(); ++i) {
      if (tpl_content[i] == '\n') {
        tpl_content[i] = ' ';
      }
    }
    if (tpl_vec && tpl_vec->Size() > 0) {
      tpl_cache[key] = tpl_vec;
      MCE_INFO("clearsilver template added : " << key);
    } else {
      delete tpl_vec;
      MCE_WARN("clearsilver template init error : " << key);
    }
  }

  {
    IceUtil::RWRecMutex::WLock lock(rw_mutex_);
    tpl_cache.swap(tpl_cache_);
  }

  MCE_INFO("FeedTplCache loaded size : " << tpl_cache_.size());
#endif
  return true;
}

void UniqueLoopNodes(int stype, map<string, LoopNodeItems> * loop_nodes) {
  for(map<string, LoopNodeItems>::iterator i = loop_nodes->begin(); i != loop_nodes->end(); ++i) {
    if (i->first != "f.from" 
        && i->first != "f.comment.owner" 
        && i->first != "f.follower" 
        && i->first != "f.listener") {
      continue;
    }

    //MCE_INFO(i->first << " node count : " << i->second.size());
    LoopNodeItems uniq_items;
    set<string> id_set;
    bool flag = false;
    for(size_t j = 0; j < i->second.size(); ++j) {
      //MCE_DEBUG("node child count : " << i->second[j].size());

      map<string, string>::iterator k = i->second[j].find(string("id"));
      if (k == i->second[j].end()) {
        MCE_WARN("id not found!");
        continue;
      }

      if (id_set.find(k->second) == id_set.end()) {
        id_set.insert(k->second);
        uniq_items.push_back(i->second[j]);
        //MCE_DEBUG("new node id " << k->second);
      } else {
        flag = true;
        //MCE_DEBUG("duplicate node id " << k->second);
      }
    }

    if (flag) {
      i->second = uniq_items;
    }
  }
}

static bool ClearSilverExpand(const feed::FeedData * data
            , cwf::User * u, int owner
            , WeiredSource render 
            , const ilikeit::LikeInfo* likeinfo
            , int index
            , const vipinfo::VipFeedDataSeq & vipInfo
            , std::string * output
            , std::string & stat_log
            , bool isAdmin
            , int view) {
  const FeedXmlSchema * feed_schema = FeedSchemaCache::Instance().FindFeedSchema(WeiredType(data->type).type, 
      WeiredType(data->type).version);
  if (!feed_schema) {
    MCE_WARN("feed schema not found : uid=" << u->id() << " stype=" << WeiredType(data->type).type << " version=" << WeiredType(data->type).version);
    return false;
  }

  // FeedHdfParser feed_parser(*feed_schema);
  FeedHdfParser * feed_parser = feed_schema->CreateHdfParser();

  if (feed_parser->Parse(data->xml) != 0) {
    MCE_WARN("feed data parse error");
    delete feed_parser;
    return false;
  }

  std::string vType;

  map<string, string> & hdf_map = feed_parser->hdf_map();
  //for(map<string, string>::const_iterator it = hdf_map.begin(); it != hdf_map.end(); ++it) {
  //  MCE_DEBUG(it->first << " : " << it->second);
  //}
  if(!(render & WS_MINIFEED)){
    int white_view = xce::feed::Whitelist::instance().IsWhiteUser(u->id());
    if(white_view > 0){
      hdf_map.insert(std::make_pair("unread", "1"));
      if(data->weight&0x01){
        hdf_map.insert(std::make_pair("tipcontent", "TA的新鲜事正在优先展示。点击取消"));
        hdf_map.insert(std::make_pair("attention", "取消"));
        hdf_map.insert(std::make_pair("isfouce", "unread"));
        hdf_map.insert(std::make_pair("mark", "0"));
      } else{
        hdf_map.insert(std::make_pair("tipcontent", "点击这里。优先展示TA的新鲜事"));
        hdf_map.insert(std::make_pair("attention", ""));
        hdf_map.insert(std::make_pair("isfouce", "unread unfouce"));
        hdf_map.insert(std::make_pair("mark", "1"));
      }
    }
  }

  map<string, LoopNodeItems> & loop_nodes  = feed_parser->loop_nodes();

  UniqueLoopNodes(WeiredType(data->type).type, &loop_nodes);
  //MCE_DEBUG("Loop node count " << loop_nodes.size());
  for(map<string, LoopNodeItems>::iterator i = loop_nodes.begin(); i != loop_nodes.end(); ++i) {
    //MCE_DEBUG("Loop node " << i->first);
    for(size_t j = 0; j < i->second.size(); ++j) {
      //MCE_DEBUG("Node " << i->first << " " << j);
      for(map<string, string>::iterator k = i->second[j].begin(); k != i->second[j].end(); ++k) {
        stringstream key;
        key << i->first << '.' << j;
        if (!k->first.empty()) {
          key << '.' << k->first;
        }
        //MCE_DEBUG("    k=" << k->first << '/' << key.str() << " v=" << k->second);
        hdf_map.insert(std::make_pair(key.str(), k->second));
      }
    }
  }

  hdf_map.insert(std::make_pair("feedIndex", lexical_cast<std::string>(index)));
  hdf_map.insert(std::make_pair("source", lexical_cast<std::string>(data->source)));

  // 依照java做法，塞入fx可能不是最优的做法
  if (owner) {
    hdf_map.insert(std::make_pair("interviewee", lexical_cast<std::string>(owner)));
  }

  // 默认字段: id stype vType isNewsFeed isSelf typeText
  hdf_map.insert(std::make_pair("id", lexical_cast<std::string>(data->feed)));
  if (data->actor == u->id()) {
    hdf_map.insert(std::make_pair("isSelf", "1"));
  }

  hdf_map.insert(std::make_pair("stype", lexical_cast<std::string>(WeiredType(data->type).type)));
  if (u) {
    hdf_map.insert(std::make_pair("userID", lexical_cast<std::string>(u->id())));
    hdf_map.insert(std::make_pair("userName", u->name()));
  }

  if(!vipInfo.empty()) {
    vipinfo::VipFeedDataSeq::const_iterator it = vipInfo.begin();
    std::string strVipIconId, strVipIconLevel;
    if(1 == ((*it)->member)->status && ((*it)->icon)->iconId > 0) {
      if(((*it)->icon)->iconId < 10) {
        strVipIconId = "0" + lexical_cast<std::string>(((*it)->icon)->iconId);
      }
      else {
        strVipIconId = lexical_cast<std::string>(((*it)->icon)->iconId);
      }
      if(((*it)->member)->level < 10) {
        strVipIconLevel = "0" + lexical_cast<std::string>(((*it)->member)->level);
      }
      else {
        strVipIconLevel = lexical_cast<std::string>(((*it)->member)->level);
      }

      hdf_map.insert(std::make_pair("actorVipIconId", strVipIconId));
      hdf_map.insert(std::make_pair("actorVipLevel", strVipIconLevel));
    }

    if(1 == ((*it)->member)->status && ((*it)->hat)->status > 0 && ((*it)->hat)->hatId >= 0) {
      hdf_map.insert(std::make_pair("actorVipHatId", lexical_cast<std::string>(((*it)->hat)->hatId)));
    }
  }

  if (render & WS_HOT) {
    vType = "hot";
  } else if (render & WS_LIVE) {
    vType = "liv";
  } else if (render & WS_FAVORITE) {
    vType = "fav";
  }

  if (render & WS_MINIFEED) {
    vType += "minifeed";
    hdf_map.insert(std::make_pair("visitType", "minifeed"));
    hdf_map.insert(std::make_pair("isMiniFeed", "1"));
  } else if (render & WS_MINIGROUP) {
    vType += "minigroup";
    hdf_map.insert(std::make_pair("visitType", "minigroup"));
    hdf_map.insert(std::make_pair("isMiniGroup", "1"));

  } else {
    vType += "newsfeed";
    hdf_map.insert(std::make_pair("visitType", "newsfeed"));
    hdf_map.insert(std::make_pair("isNewsFeed", "1"));
  }

  vType += "&sfet=";
  vType += lexical_cast<string>(WeiredType(data->type).type);
  vType += "&fin=" + boost::lexical_cast<std::string>(index);
  vType += "&ff_id=" + feed_parser->Find("f.from.id");

  hdf_map.insert(std::make_pair("vType", vType));

  // 处理 "赞"
  if (likeinfo) {
    int total = likeinfo->totalCount;
    if (likeinfo->selfCount) {
      total -= 1;// 去除自己
      if (total < 0) {
        MCE_WARN("zan error total :" << total);
      }
      hdf_map.insert(std::make_pair("selfzancount", lexical_cast<std::string>(likeinfo->selfCount)));
    }
    if (total > 0) {
      hdf_map.insert(std::make_pair("totalzancount", lexical_cast<std::string>(total)));
    }
  }

  int isAmbUser = feed::AmbCache::instance().Get(data->actor);
  if((unsigned int)isAmbUser & 0x1){
    hdf_map.insert(std::make_pair("actorAmbIconId", "1"));
  }
  if((unsigned int)isAmbUser & 0x2){
    hdf_map.insert(std::make_pair("schoolAmbIcon", "1"));
  }

  //BuildHdf(data, u, render, &hdf_map);

  //string tpl_name = GetCSTplKey(WeiredType(data->type).type, WeiredType(data->type).version, render & WS_FLAG);
  string tpl_name;
  //int replyOptimized[] = {102, 103, 104, 107, 110, 213, 215, 216, 701, 708, 709, 801, 808, 2005, 2006, 
  //                      2007, 2008, 2012, 2013, 2032, 2035, 2036, 2038, 3709, 3710, 3711, 3712, 3713,
  //                      3714, 3721, 3722, 3723, 3724, 3725, 3730, 3731, 3732, 3733, 3734, 3735,
  //                      8210, 8211, 8601, 8605, 8607, 9102, 9103, 9104, 9105 };

  if((u->id() % 50 == 41) && (render & WS_NEWSFEED)) {
    tpl_name = GetCSTplKey(WeiredType(data->type).type, WeiredType(data->type).version, 10);
  } else {
    tpl_name = GetCSTplKey(WeiredType(data->type).type, WeiredType(data->type).version, view & WS_FLAG);
  }

  CSTemplate * tpl = FeedTplCache::Instance().GetTemplate(tpl_name);
  if (!tpl) {
    MCE_WARN("CSTemplateCache get template error : " << tpl_name);
    tpl_name = GetCSTplKey(WeiredType(data->type).type, WeiredType(data->type).version, view & WS_FLAG);
    tpl = FeedTplCache::Instance().GetTemplate(tpl_name);
    if(!tpl) {
      MCE_WARN("CSTemplateCache get template error : " << tpl_name);
      delete feed_parser;
      return false;
    }
  }
  *output = tpl->Render(hdf_map);
  //MCE_DEBUG("Render output : " << *output);
  
  std::ostringstream os;  
  os << "stype:" << lexical_cast<std::string>(WeiredType(data->type).type);
  os << " owner:" << owner;
  os << " uid:" << u->id();
  os << " f_id:" << data->feed;
  os << " index:" << index;
  os << " actor:" << data->actor;
  os << " source:" << data->source;
  os << " view:" << view;
  os << " stat_id:" << feed_parser->Find("f.stat.id");
  os << " level:" << feed_parser->Find("f.stat.level");
  os << " origin_url:" << feed_parser->Find("f.origin.url") << std::flush;
  stat_log = os.str();

  if(WeiredType(data->type).type > 8000 && WeiredType(data->type).type < 8005) {
    std::ostringstream os;
    os << feed_parser->Find("f.log");
    time_t now = time(0);
    int err = 0;
    struct tm nowtimeinfo;
    char buf[0x100];
    int len;
    #ifdef WIN32
    err = localtime_s(&nowtimeinfo, (const time_t *)&now);
    if (err) {
      MCE_WARN("ClearSilverExpand --> WRONG TIME:" << now);
    }
    #elif defined(POSIX)
    localtime_r((const time_t *)&now, &nowtimeinfo);
    #endif
    std::string exposeTime;
    if(!err) {
      len = strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &nowtimeinfo);
      exposeTime.assign(buf, len);
    }
    os << "," << exposeTime << "," << index << std::flush;
    MCE_DEBUG("ClearSilverExpand --> edm expose, stype:" << WeiredType(data->type).type << " log:" << os.str());
    ::xce::ad::AdEdmLogAdapter::instance().Expose(os.str());
  }

  delete feed_parser;
  return true;
}

static bool CtemplateExpand(const feed::FeedData * feed, FeedXml * fx
            , cwf::User * u, int owner, WeiredSource source
            , const ilikeit::LikeInfo* likeinfo
            , const std::string & template_file
            , int index
            , const vipinfo::VipFeedDataSeq & vipInfo
            , std::string * output
            , bool isXML
            , std::string logic_str
            , bool isAdmin) {
  
  ctemplate::TemplateDictionary dict("");

  std::string vType;
  unsigned int flag = source;

  dict.SetIntValue("feedIndex", index);
  
  // 依照java做法，塞入fx可能不是最优的做法
  StringMap & m = fx->body;
  m.insert(std::make_pair("itemsource", lexical_cast<std::string>(feed->source)));
  if (owner)
    m.insert(std::make_pair("interviewee", lexical_cast<std::string>(owner)));

  bool isVip = false;  

  if (u) {
    m.insert(std::make_pair("userID", lexical_cast<std::string>(u->id())));
    m.insert(std::make_pair("userName", u->name()));
  }

  if(!vipInfo.empty()) {   
      vipinfo::VipFeedDataSeq::const_iterator it = vipInfo.begin();       
      std::string strVipIconId, strVipIconLevel;
      if(1 == ((*it)->member)->status && ((*it)->icon)->iconId > 0) {
        if(((*it)->icon)->iconId < 10) {
          strVipIconId = "0" + lexical_cast<std::string>(((*it)->icon)->iconId);
        }
        else {
          strVipIconId = lexical_cast<std::string>(((*it)->icon)->iconId);
        }
        if(((*it)->member)->level < 10) {
          strVipIconLevel = "0" + lexical_cast<std::string>(((*it)->member)->level);
        }
        else {
          strVipIconLevel = lexical_cast<std::string>(((*it)->member)->level);
        }

        m.insert(std::make_pair("userVipIconId", strVipIconId));
        m.insert(std::make_pair("userVipIconLevel", strVipIconLevel));
        isVip = true;
    }
      
    if(1 == ((*it)->member)->status && ((*it)->hat)->status > 0 && ((*it)->hat)->hatId >= 0) {
      m.insert(std::make_pair("userVipHatId", lexical_cast<std::string>(((*it)->hat)->hatId)));
    }
  }

  if (flag & WS_HOT) {
    vType = "hot";
  } else if (flag & WS_LIVE) {
    vType = "liv";
  } else if (flag & WS_FAVORITE) {
    vType = "fav";
  }

  if (flag & WS_MINIFEED) {
    vType += "minifeed";
    m.insert(std::make_pair("visitType", "minifeed"));
  } else {
    vType += "newsfeed";
    m.insert(std::make_pair("visitType", "newsfeed"));
    m.insert(std::make_pair("isNewsFeed", "1"));
  }
  vType += "&sfet=";
  vType += lexical_cast<std::string>(WeiredType(feed->type).type);
  vType += "&fin=" + lexical_cast<std::string>(index);
  vType += "&ff_id=" + fx->find("f_from_id");

  m.insert(std::make_pair("vType", vType));


  // 处理 "赞"
  if (likeinfo) {
    int total = likeinfo->totalCount;
    if (likeinfo->selfCount) {
      total -= 1;// 去除自己
      if (total < 0) {
        //LOG_F(WARNING) << "zan error total :" << total;
        MCE_WARN("zan error total :" << total);
      }
      //dict.ShowSection("selfzancount");
      m.insert(std::make_pair("selfzancount", lexical_cast<std::string>(likeinfo->selfCount)));
    }
    //else 
    //  dict.ShowSection("selfzancount_NOT");
    if (total > 0) {
      m.insert(std::make_pair("totalzancount", lexical_cast<std::string>(total)));
    }
    //dict.SetIntValue("selfzancount", likeinfo->selfCount);
    //dict.SetIntValue("totalzancount", total);
  }

  DictBuilder g;
  g.vType_ = vType;
  g.render_ = source;
  fx->template_file = template_file; // 703类型会改动文件模版


  if (!g.BuildDictionary(feed, fx, u, source, &dict, isXML, logic_str,  owner, likeinfo, template_file, index, isAdmin))
    return false;

  int outFeedTypes[] = {  102, 103, 202, 401, 802,1001,1002,1003,1005,1006,1302,1801,1802,
                         1804,2002,2003,2004,2017,2202,2203,2204,2205,2206,
                         2207,2217,2222,2303,2401,2402,2403,2503,2601,2602,2701,
                         2901,2902,2905,2906,4101,4203,5033,8007,8012,8501,8601};  //这些类型新鲜事需要在body.inc中特别处理
  if(isVip && /*(source == WS_HOME) && */ 
     std::binary_search(outFeedTypes, outFeedTypes+sizeof(outFeedTypes)/sizeof(int), WeiredType(feed->type).type)) {
    dict.ShowSection("userVipIconId");
  }

  int isAmbUser = feed::AmbCache::instance().Get(feed->actor);
  if((unsigned int)isAmbUser & 0x1){
    dict.ShowSection("userAmbIconId");
  }
  if((unsigned int)isAmbUser & 0x2){
    dict.ShowSection("schoolAmbIcon");
  }
  
  ctemplate::Template* tpl = ctemplate::Template::GetTemplate(
    fx->template_file
    , ctemplate::DO_NOT_STRIP);

  if (!tpl) {
    //LOG(LS_ERROR) << "tpl not found: " << template_file;
    MCE_WARN("tpl not found: " << template_file);
    return false;
  }

  return tpl->Expand(output, &dict);
}

static bool IsClearSilver(const string & xml) {
  size_t pos = xml.find_first_of('>');
  return xml.substr(0, pos).find("tpl=\"cs\"") != string::npos;
}

bool ExpandFeedData(const feed::FeedData * feed, cwf::User * u
            , int owner
            , WeiredSource source
            , const ilikeit::LikeInfo* likeinfo
            , int index
            , const vipinfo::VipFeedDataSeq & vipInfo
            , std::string * output
            , std::string & stat_log
            , bool isAdmin
            , int view) {
  // MCE_INFO("xml content : " << feed->xml);

  if (IsClearSilver(feed->xml)) {
    //MCE_INFO("IsClearSilver true");
    return ClearSilverExpand(feed, u, owner, source, likeinfo, index, vipInfo, output, stat_log, isAdmin, view);
  }

  bool isXML = true;
  std::string packed_xml("<?xml version=\"1.0\" encoding=\"UTF-8\"?><feeds>");
  FeedXml fx;

  std::string logic_str;
 
  packed_xml += feed->xml;
  packed_xml += "</feeds>";
  
  int ret = ParseCtemplateXml(packed_xml, &fx);

  if (0 != ret) {
    MCE_WARN("xml parse error: " << ret
      << " id: " << feed->feed);
    MCE_WARN("xml:" << packed_xml);
    return false;
  }

  if (fx.body.empty()) {
    MCE_WARN("empty body, fid:" << feed->feed);
    return false;
  }

  if (fx.template_type == "cs") {
    return false;
  }

  int version  = 0;
  {
    if (!fx.template_version.empty())
      version = atoi(fx.template_version.c_str());
  }

  // <== TODO: feed->type, TypeConfigTable, fx->version
  std::string filename;
  if (source & (WS_HOT | WS_LIVE | WS_FAVORITE)) {
    filename = GetTemplateFilename2(WeiredType(feed->type).type, source, version, u->id());
  } else {
    if (true || u->id() % 1000 == 708) {
      // filename = GetTemplateFilename2(WeiredType(feed->type).type, source, version);
      filename = GetTemplateFilename2(feed->type & 0xFFFF, source, version, u->id());
    } else {
      filename = GetTemplateFilename(WeiredType(feed->type).type, source, version);
    }
  }

  if (filename.empty()) {
    //MCE_WARN("tpl not support type: " << WeiredType(1342177782).type);
    MCE_WARN("tpl not support type v: " << (WeiredType(feed->type).type)
      << " 0xFFFF " << (feed->type & 0xFFFF)
      << " (" << feed->type
      << ") version: " << version
      << " fid: " << feed->feed);
    return false;
  }

  std::ostringstream os; 
  os << "stype:" << lexical_cast<std::string>(WeiredType(feed->type).type);
  os << " owner:" << owner;
  os << " uid:" << u->id();
  os << " f_id:" << feed->feed;
  os << " index:" << index;
  os << " actor:" << feed->actor;
  os << " source:" << feed->source;
  os << " view:" << view;
  os << " stat_id:" << fx.find("f_stat_id");
  os << " level:" << fx.find("f_stat_level");
  os << " origin_url:" << fx.find("f_origin_url") << std::flush;

  stat_log = os.str();
  return CtemplateExpand(feed, &fx, u, owner, source, likeinfo, filename, index, vipInfo, output, isXML, logic_str, isAdmin);
}

}

