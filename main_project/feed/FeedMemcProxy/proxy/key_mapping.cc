#include "key_mapping.h"

#include "consist_hash.h"

#include "ServiceI.h"
#include "LogWrapper.h"

using namespace std;
using namespace boost::asio;
using namespace MyUtil;

namespace xce {
namespace feed {

static map<string, Continuum *> g_clusters;
                  
									//"10.3.24.165:11211 2400;"\
                                //"10.3.17.150:11211 2400;"\

static string memcached_nodes = "10.3.16.100:11211 2400;"\
                                "10.3.23.235:11211 2400;"\
                                "10.3.23.238:11211 2400;"\
                                "10.3.19.228:11211 2400;"\
                                "10.3.17.155:11211 2400;"\
                                "10.3.17.156:11211 2400;"\
                                "10.3.17.149:11211 2400;"\
                                "10.3.23.236:11211 3600;"\
                                "10.3.17.158:11211 2400;"\
                                "10.3.22.42:11211 2400;"\
                                "10.3.22.43:11211 2400;"\
                                "10.3.22.119:11211 2400;"\
                                "10.3.24.166:11211 2400;"\
                                "10.3.22.121:11211 2400;"\
                                "10.3.22.122:11211 2400;"\
                                "10.3.22.123:11211 2400;"\
                                "10.3.22.44:11211 2400;"\
                                "10.3.22.125:11211 2400;"\
                                "10.3.22.126:11211 2400;"\
                                "10.3.19.71:11211 2400;";


static string talk_memcached_nodes =  "10.3.22.122:11212 500;"\
                                      "10.3.22.123:11212 500;"\
                                      "10.3.22.44:11212 500;"\
                                      "10.3.22.125:11212 500;"\
                                      "10.3.22.126:11212 500;";

static string interaction_memcached_nodes = "10.3.22.122:11213 200;"\
                                            "10.3.22.123:11213 200;"\
                                            "10.3.22.44:11213 200;"\
                                            "10.3.22.125:11213 200;"\
                                            "10.3.22.126:11213 200;";

static string photo_memcached_nodes = "10.3.22.121:11212 2400;"\
                                "10.3.22.119:11214 2400;";


static string feedlist_memcached_nodes = "10.3.20.45:11214 2400;";

static string feed_user_property_memcached_nodes =  "10.3.22.120:11215 400;"\
                                                    "10.3.22.124:11215 400;";

static string feed_index_memcached_nodes =  "10.3.22.43:11216 400;"\
                                                    "10.3.22.119:11216 400;";

static string notify_content_memcached_nodes =  "10.3.22.42:11217 800;"\
                                            "10.3.22.43:11217 800;"\
                                            "10.3.22.121:11217 800;"\
                                            "10.3.22.124:11217 800;";

static string online_cache_memcached_nodes = "10.3.22.43:11219 800;"\
																						 "10.3.22.120:11219 800;"\
																						 "10.3.22.122:11219 800;"\
																						 "10.3.22.124:11219 800;"\
                                             "10.3.22.126:11219 800;";

static string online_cache_backup_memcached_nodes = "10.3.22.42:11219 800;"\
                                                    "10.3.22.119:11219 800;"\
																						 			  "10.3.22.121:11219 800;"\
																						 			  "10.3.22.123:11219 800;"\
																						 			  "10.3.22.125:11219 800;";

static string im_recent_contact_memcached_nodes = "10.3.22.42:11218 800;"\
																									 "10.3.22.43:11218 800;"\
																									 "10.3.22.119:11218 800;"\
																									 "10.3.22.120:11218 800;";

static string talk_group_view_nodes = "10.3.19.52:11211 800;";
static string talk_group_message_nodes = "10.3.19.52:11211 800;";
  static string talk_group_info_nodes = "10.3.19.52:11211 800;";
static string talk_group_member_nodes = "10.3.19.52:11211 800;";
bool InitCacheClusters() {



  {
    string ns = "TALK_GROUP_INFO";
    Continuum * c = new Continuum;
    if (c->SetCacheNodes(talk_group_info_nodes)) {
      MCE_INFO("加载 Continuum " << ns << ":" << talk_group_info_nodes);
      g_clusters.insert(make_pair(ns, c));
    } else {
      MCE_WARN("加载 Continuum 失败 : " << ns << "-" << talk_group_info_nodes);
      delete c;
    }
  }




  {
    string ns = "GCMSG";
    Continuum * c = new Continuum;
    if (c->SetCacheNodes(talk_group_message_nodes)) {
      MCE_INFO("加载 Continuum " << ns << ":" << talk_group_message_nodes);
      g_clusters.insert(make_pair(ns, c));
    } else {
      MCE_WARN("加载 Continuum 失败 : " << ns << "-" << talk_group_message_nodes);
      delete c;
    }
  }


  {
    string ns = "TALKGROUPVIEW";
    Continuum * c = new Continuum;
    if (c->SetCacheNodes(talk_group_view_nodes)) {
      MCE_INFO("加载 Continuum " << ns << ":" << talk_group_view_nodes);
      g_clusters.insert(make_pair(ns, c));
    } else {
      MCE_WARN("加载 Continuum 失败 : " << ns << "-" << talk_group_view_nodes);
      delete c;
    }
  }


  {
    string ns = "TALK_GROUP_MEMBER";
    Continuum * c = new Continuum;
    if (c->SetCacheNodes(talk_group_member_nodes)) {
      MCE_INFO("加载 Continuum " << ns << ":" << talk_group_member_nodes);
      g_clusters.insert(make_pair(ns, c));
    } else {
      MCE_WARN("加载 Continuum 失败 : " << ns << "-" << talk_group_member_nodes);
      delete c;
    }
  }



  {
    string ns = "DEFAULT";
    Continuum * c = new Continuum;
    if (c->SetCacheNodes(memcached_nodes)) {
      MCE_INFO("加载 Continuum " << ns << ":" << memcached_nodes);
      g_clusters.insert(make_pair(ns, c));
    } else {
      MCE_WARN("加载 Continuum 失败 : " << ns << "-" << memcached_nodes);
      delete c;
    }
  }
	{
    string ns = "IMONLINEBACKUP";
    Continuum * c = new Continuum;
    if (c->SetCacheNodes(online_cache_backup_memcached_nodes)) {
      MCE_INFO("加载 Continuum " << ns << ":" << online_cache_backup_memcached_nodes);
      g_clusters.insert(make_pair(ns, c));
    } else {
      MCE_WARN("加载 Continuum 失败 : " << ns << "-" << online_cache_backup_memcached_nodes);
      delete c;
    }
  }
	{
    string ns = "IMONLINE";
    Continuum * c = new Continuum;
    if (c->SetCacheNodes(online_cache_memcached_nodes)) {
      MCE_INFO("加载 Continuum " << ns << ":" << online_cache_memcached_nodes);
      g_clusters.insert(make_pair(ns, c));
    } else {
      MCE_WARN("加载 Continuum 失败 : " << ns << "-" << online_cache_memcached_nodes);
      delete c;
    }
  }
	{
    string ns = "IMRECENT";
    Continuum * c = new Continuum;
    if (c->SetCacheNodes(im_recent_contact_memcached_nodes)) {
      MCE_INFO("加载 Continuum " << ns << ":" << im_recent_contact_memcached_nodes);
      g_clusters.insert(make_pair(ns, c));
    } else {
      MCE_WARN("加载 Continuum 失败 : " << ns << "-" << im_recent_contact_memcached_nodes);
      delete c;
    }
  }
  {
    string ns = "FEED";
    Continuum * c = new Continuum;
    if (c->SetCacheNodes(memcached_nodes)) {
      MCE_INFO("加载 Continuum " << ns << ":" << memcached_nodes);
      g_clusters.insert(make_pair(ns, c));
    } else {
      MCE_WARN("加载 Continuum 失败 : " << ns << "-" << memcached_nodes);
      delete c;
    }
  }

  {
    string ns = "ADUP";
    Continuum * c = new Continuum;
    if (c->SetCacheNodes(memcached_nodes)) {
      MCE_INFO("加载 Continuum " << ns << ":" << memcached_nodes);
      g_clusters.insert(make_pair(ns, c));
    } else {
      MCE_WARN("加载 Continuum 失败 : " << ns << "-" << memcached_nodes);
      delete c;
    }
  }

  {
    string ns = "HOTM";
    Continuum * hot_continuum = new Continuum;
    if (hot_continuum->SetCacheNodes(memcached_nodes)) {
      MCE_INFO("加载 Continuum " << ns << ":" << memcached_nodes);
      g_clusters.insert(make_pair(ns, hot_continuum));
    } else {
      MCE_WARN("加载 Continuum 失败 : " << ns << "-" << memcached_nodes);
      delete hot_continuum;
    }
  }

  {
    string ns = "TUSR";
    Continuum * talk_cache_continuum = new Continuum;
    if (talk_cache_continuum->SetCacheNodes(talk_memcached_nodes)) {
      MCE_INFO("加载 Continuum " << ns << ":" << talk_memcached_nodes);
      g_clusters.insert(make_pair(ns, talk_cache_continuum));
    } else {
      MCE_WARN("加载 Continuum 失败 : " << ns << "-" << talk_memcached_nodes);
      delete talk_cache_continuum;
    }
  }

  {
    string ns = "INTERACTION";
    Continuum * interaction_cache_continuum = new Continuum;
    if (interaction_cache_continuum->SetCacheNodes(interaction_memcached_nodes)) {
      MCE_INFO("加载 Continuum " << ns << ":" << interaction_memcached_nodes);
      g_clusters.insert(make_pair(ns, interaction_cache_continuum));
    } else {
      MCE_WARN("加载 Continuum 失败 : " << ns << "-" << interaction_memcached_nodes);
      delete interaction_cache_continuum;
    }
  }

  {
    string ns = "FUP";
    Continuum * fup_cache_continuum = new Continuum;
    if (fup_cache_continuum->SetCacheNodes(feed_user_property_memcached_nodes)) {
      MCE_INFO("加载 Continuum " << ns << ":" << feed_user_property_memcached_nodes);
      g_clusters.insert(make_pair(ns, fup_cache_continuum));
    } else {
      MCE_WARN("加载 Continuum 失败 : " << ns << "-" << feed_user_property_memcached_nodes);
      delete fup_cache_continuum;
    }
  }

  {
    string ns = "NOTIFY_CONT";
    Continuum * notifycont_cache_continuum = new Continuum;
    if (notifycont_cache_continuum->SetCacheNodes(notify_content_memcached_nodes)) {
      MCE_INFO("加载 Continuum " << ns << ":" << notify_content_memcached_nodes);
      g_clusters.insert(make_pair(ns, notifycont_cache_continuum));
    } else {
      MCE_WARN("加载 Continuum 失败 : " << ns << "-" << notify_content_memcached_nodes);
      delete notifycont_cache_continuum;
    }
  }

  {
    string ns = "FINDEX";
    Continuum * findex_cache_continuum = new Continuum;
    if (findex_cache_continuum->SetCacheNodes(feed_index_memcached_nodes)) {
      MCE_INFO("加载 Continuum " << ns << ":" << feed_index_memcached_nodes);
      g_clusters.insert(make_pair(ns, findex_cache_continuum));
    } else {
      MCE_WARN("加载 Continuum 失败 : " << ns << "-" << feed_index_memcached_nodes);
      delete findex_cache_continuum;
    }
  }


  {
    string ns = "FEEDLIST";
    Continuum * feedlist_cache_continuum = new Continuum;
    if (feedlist_cache_continuum->SetCacheNodes(feedlist_memcached_nodes)) {
      MCE_INFO("加载 Continuum " << ns << ":" << feedlist_memcached_nodes);
      g_clusters.insert(make_pair(ns, feedlist_cache_continuum));
    } else {
      MCE_WARN("加载 Continuum 失败 : " << ns << "-" << feedlist_memcached_nodes);
      delete feedlist_cache_continuum;
    }
  }

  {
      string ns = "PHOTO_SEQ";
      Continuum * c = new Continuum;
      if (c->SetCacheNodes(photo_memcached_nodes)) {
        MCE_INFO("加载 Continuum " << ns << ":" << photo_memcached_nodes<<"line: "<<__LINE__);
        g_clusters.insert(make_pair(ns, c));
      } else {
        MCE_WARN("加载 Continuum 失败 : " << ns << "-" << photo_memcached_nodes);
        delete c;
      }
    }

    {
        string ns = "PHOTO_CONTENT";
        Continuum * c = new Continuum;
        if (c->SetCacheNodes(photo_memcached_nodes)) {
          MCE_INFO("加载 Continuum " << ns << ":" << photo_memcached_nodes<<"line: "<<__LINE__);
          g_clusters.insert(make_pair(ns, c));
        } else {
          MCE_WARN("加载 Continuum 失败 : " << ns << "-" << photo_memcached_nodes);
          delete c;
        }
      }

  return true;
}

ip::tcp::endpoint GetEndpointByKey(const char * key, size_t len) {
  size_t delim_pos = 0;
  for (; delim_pos < len; ++ delim_pos) {
    if (key[delim_pos] == '#') {
      break;
    }
  }

  Continuum * c = 0;

  if (delim_pos != len) {
    string tmp(key, delim_pos);
    map<string, Continuum *>::const_iterator it = g_clusters.find(tmp);
    if (it != g_clusters.end()) {
      c = it->second;
    }
  } else {
    delim_pos = 0;
  }

  if (!c) {
    c = g_clusters["DEFAULT"];
  }
  
  ip::tcp::endpoint ep = c->LocateCacheNode(key, len);

  //MCE_DEBUG("key=" << string(key, len) << " cache_node=" << ep);

  return ep;
}

}
}

