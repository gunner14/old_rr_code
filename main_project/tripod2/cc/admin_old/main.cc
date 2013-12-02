/*
 * main.cc
 *
 *  Created on: Apr 17, 2012
 *      Author: liuwenju
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <zookeeper/zookeeper.h>
#include <expat.h>

#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include "tripod2/cc/admin/tripod_config.h"
#include "tripod2/cc/core/zk_client.h"

namespace xce {
namespace tripod {
// 用于解析xml文件
static const std::string kTripod = "tripod";
static const std::string kDefaultCache = "cache";
static const std::string kDefaultCachemanager = "cachemanager";
static const std::string kDefaultProducermanager = "producermanager";
static const std::string kNode = "node";
static const std::string kNamespace = "namespace";
// 用于zk中的节点
/*
static const std::string kDefaultZkAddress = "10.2.78.253:2181";
static const std::string kDefaultZNodePrefix = "/tripod2";
static const std::string kZkSeparator = "/";
static const std::string kDefaultZkCacheZNode = kDefaultZNodePrefix +
    kZkSeparator + "cache";
static const std::string kDefaultZkCacheManagerZNode= kDefaultZNodePrefix +
    kZkSeparator + "cachemanager";
static const std::string kDefaultZkProducerManagerZNode = kDefaultZNodePrefix +
    kZkSeparator + "producermanager";
*/
};
};


using namespace xce::tripod;
using namespace boost;
using namespace boost::algorithm;

// 只能本文件使用
static zhandle_t *g_zk;            // g_zk连接器
static TripodConfig g_xml_config;  // 通过读配置文件，得到配置信息
static TripodConfig g_zk_config;   // 通过连接g_zk，得到配置信息
static std::vector<std::string> g_xml_nodes;
static std::string g_xml_namespace;


//////////////////////////////xml文件的解析///////////////////////////
/*
 * xml元素开始处理的函数,内部是递归调用的
 * 每一个节点都会递归的调用一次Start和end方法
 */
static void Start(void *data, const char* el, const char **attr) {
  // std::cout << std::string(el) << std::endl;
  g_xml_nodes.push_back(std::string(el));
  
  if (g_xml_nodes.at(0) != kTripod) {
    std::cout << "xmlpath指定的xml文件格式错误！！！" << std::endl;
    exit(-1);
  }
  
  if (g_xml_nodes.size() > 1) {
    if (g_xml_nodes.at(1) != kDefaultCache && 
        g_xml_nodes.at(1) != kDefaultCachemanager &&
        g_xml_nodes.at(1) != kDefaultProducermanager) {
      std::cout << "xml解析失败：xmlpath指定的xml文件格式错误！！！" << std::endl;
      exit(-1);
    }
  }

  std::map<std::string, std::string> attr_map;
  for (int i = 0; attr[i]; i += 2) {
    attr_map[std::string(attr[i])] = std::string(attr[i + 1]);
    //  std::cout << "========" << attr[i] << ":" << attr[i + 1] << std::endl;
  }

  if (kNamespace == std::string(el) && kTripod == g_xml_nodes.at(0)) {
    if (attr_map.find(std::string("id")) != attr_map.end() && attr_map.size() == 1) {
      g_xml_namespace = attr_map["id"];
    } else {
      std::cout << "xml解析失败：xmlpath指定的xml文件的namespace节点的格式错误！！！" << std::endl;
      exit(-1);
    }
  }

  if (kNode == std::string(el) && kTripod == g_xml_nodes.at(0) && kDefaultCache
      == g_xml_nodes.at(1)) {
    if (kNamespace != g_xml_nodes.at(2)) {
      std::cout << "xml解析失败:" << g_xml_nodes.at(2) << "语法错误,应该为namespace"<< std::endl;
      exit(-1);
    }
    if (attr_map.find("nickname") != attr_map.end() && attr_map.find("address")
        != attr_map.end() && attr_map.find("port") != attr_map.end() && attr_map.size() == 3) {
      g_xml_config.caches()[g_xml_namespace].insert(XmlNode(
          attr_map["nickname"], attr_map["address"], attr_map["port"]));
    } else {
      std::cout << "xml解析失败：node节点的属性名错误,请检查命名空间：" << g_xml_namespace << std::endl;
      exit(-1);
    }
  } else if (kNode == std::string(el) && kTripod == g_xml_nodes.at(0)
      && kDefaultCachemanager == g_xml_nodes.at(1)) {
    if (kNamespace != g_xml_nodes.at(2)) {
      std::cout << "xml解析失败:" << g_xml_nodes.at(2) << "语法错误,应该为namespace"<< std::endl;
      exit(-1);
    }
    if (attr_map.find("address") != attr_map.end() && attr_map.find("port")
        != attr_map.end() && attr_map.size() == 2) {
      g_xml_config.cachemanagers()[g_xml_namespace].insert(XmlNode(
          attr_map["address"], attr_map["port"]));
    } else {
      std::cout << "xml解析失败：node节点的属性名错误,请检查命名空间：" << g_xml_namespace << std::endl;
      exit(-1);
    }
  } else if (kNode == std::string(el) && kTripod == g_xml_nodes.at(0)
      && kDefaultProducermanager == g_xml_nodes.at(1)) {
    if (kNamespace != g_xml_nodes.at(2)) {
      std::cout << "xml解析失败:" << g_xml_nodes.at(2) << "语法错误,应该为namespace"<< std::endl;
      exit(-1);
    }
    if (attr_map.find("address") != attr_map.end() && attr_map.find("port")
        != attr_map.end() && attr_map.size() == 2) {
      g_xml_config.producermanagers()[g_xml_namespace].insert(XmlNode(
          attr_map["address"], attr_map["port"]));
    } else {
      std::cout << "xml解析失败：node节点的属性名错误,请检查命名空间：" << g_xml_namespace << std::endl;
      exit(-1);
    }
  }
}



/*
 * xml元素结束调用的函数
 */
static void End(void* data, const char* el) {
  g_xml_nodes.pop_back();
}
/*
 * 根据xml文件路径解析xml获得配置信息
 */
void GetXmlConfig(const std::string& xmlPath) {
  const int kBuffSize = 1024 * 8;
  char buff[kBuffSize];
  /*
   * 创建解析对象
   */
  XML_Parser parser = XML_ParserCreate(NULL);
  if (!parser) {
    fprintf(stderr, "Can not get XML_Parser!\n");
    exit(-1);
  }
  /*
   * 设置每个Xml元素出现和结束的处理函数
   */
  XML_SetElementHandler(parser, Start, End);
  // open file
  FILE* file = fopen(xmlPath.c_str(), "rb");
  for (;;) {
    int done;
    int len;
    /*
     * 从文件读取数据到缓冲区
     */
    len = static_cast<int>(fread(buff, 1, kBuffSize, file));
    if (ferror(file)) {
      fprintf(stderr, "Read error\n");
      exit(-1);
    }
    /*
     * 判断是否到文件未
     */
    done = feof(file);
    /*
     * 调用库函数XML_Parse来分析缓冲区buff里的XML数据。
     */
    if (XML_Parse(parser, buff, len, done) == XML_STATUS_ERROR) {
      fprintf(stderr, "Parse error at line %" "l" "u:\n%s\n",
          XML_GetCurrentLineNumber(parser), XML_ErrorString(XML_GetErrorCode(
              parser)));
      exit(-1);
    }
    /*
     * 如果到文件尾结束
     */
    if (done) {
      break;
    }
  }
  /*
   * 别忘了，释放资源
   */
  fclose(file);
  XML_ParserFree(parser);
  return;
}  // end the function
/////////////////////////end xml文件的解析/////////////////////////////////////


///////////////////////////读取zookeeper的配置信息/////////////////////////////
/*
 *获得g_zk的部分信息
 */
static void GetPartZkConfig(std::string path) {
  struct String_vector namespaces;
  int rc = zoo_get_children(g_zk, path.c_str(), 0, &namespaces);
  if (rc != ZOK) {
    std::cout << "Can not get children for path :" << path << std::endl;
    exit(-1);
  }
  for (int i = 0; i < namespaces.count; ++i) {
    std::string ns = std::string(namespaces.data[i]);
    std::ostringstream oss;
    oss << path.c_str() << kZkSeparator << ns;
    struct String_vector nodes;
    rc = zoo_get_children(g_zk, oss.str().c_str(), 0, &nodes);
    if (rc != ZOK) {
      std::cout << "Can not get children for path :" << oss.str() << std::endl;
      exit(-1);
    }

    for (int j = 0; j < nodes.count; ++j) {
      if (kDefaultZkCacheZNode == path) {
        g_zk_config.caches()[ns].insert(XmlNode(std::string(nodes.data[j])));
      } else if (kDefaultZkCacheManagerZNode == path) {
        g_zk_config.cachemanagers()[ns].insert(XmlNode(std::string(nodes.data[j])));
      } else if (kDefaultZkProducerManagerZNode == path) {
        g_zk_config.producermanagers()[ns].insert(XmlNode(std::string(
            nodes.data[j])));
      }
    }
    // 防止命名空间下面没有节点，但是命名空间存在
    if (nodes.count == 0) {
      if (kDefaultZkCacheZNode == path) {
        g_zk_config.caches()[ns].insert(XmlNode("tmp-", "tmp-", "tmp-"));
        g_zk_config.caches()[ns].clear();
      } else if (kDefaultZkCacheManagerZNode == path) {
        g_zk_config.cachemanagers()[ns].insert(XmlNode("tmp-", "tmp-", "tmp-"));
        g_zk_config.cachemanagers()[ns].clear();
      } else if (kDefaultZkProducerManagerZNode == path) {
        g_zk_config.producermanagers()[ns].insert(XmlNode("tmp-", "tmp-", "tmp-"));
        g_zk_config.producermanagers()[ns].clear();
      }
    }
  }
}
/*
 * 根据g_zk路径获得配置信息
 * 获得tripod2的g_zk信息
 */
void GetZooKeeperConfig() {
  if (g_zk == NULL) {
    std::cout << "Can not get zk in GetZooKeeperConfig!" << std::endl;
    exit(-1);
  }
  // cache
  GetPartZkConfig(kDefaultZkCacheZNode);
  // cachemanager
  GetPartZkConfig(kDefaultZkCacheManagerZNode);
  // producermanager
  GetPartZkConfig(kDefaultZkProducerManagerZNode);
}
/////////////////////////////end///////////////////////////////////////////////


///////////////////////////////导出以前的g_zk信息，用于rollback///////////////////
// flag用来标示是否有nickname
static void DoNodePartExport(int flag, const std::set<XmlNode>& s, FILE* file) {
  std::set<XmlNode>::iterator it = s.begin();
  for (; it != s.end(); it++) {
    if (flag == 1) {
      fprintf(file,
          "\t\t\t<%s nickname=\"%s\" address=\"%s\" port=\"%s\"/>\n",
          kNode.c_str(), it->nickname().c_str(), it->ip().c_str(),
          it->port().c_str());
    } else {
      fprintf(file, "\t\t\t<%s address=\"%s\" port=\"%s\"/>\n", kNode.c_str(),
          it->ip().c_str(), it->port().c_str());
    }
  }
}

static void DoNamespacePartExport(int flag, std::map<std::string,
    std::set<XmlNode> >& cf, FILE* file) {
  std::map<std::string, std::set<XmlNode> >::iterator it = cf.begin();
  for (; it != cf.end(); it++) {
    fprintf(file, "\t\t<%s id=\"%s\">\n", kNamespace.c_str(),
        (it->first).c_str());
    DoNodePartExport(flag, it->second, file);
    fprintf(file, "\t\t</%s>\n", kNamespace.c_str());
  }
}

static void PartExport(const std::string node, TripodConfig& config,
    FILE* file) {
  if (node == kDefaultCache) {
    fprintf(file, "\t<%s>\n", kDefaultCache.c_str());
    DoNamespacePartExport(1, config.caches(), file);
    fprintf(file, "\t</%s>\n", kDefaultCache.c_str());
  } else if (node == kDefaultCachemanager) {
    fprintf(file, "\t<%s>\n", kDefaultCachemanager.c_str());
    DoNamespacePartExport(0, config.cachemanagers(), file);
    fprintf(file, "\t</%s>\n", kDefaultCachemanager.c_str());
  } else if (node == kDefaultProducermanager) {
    fprintf(file, "\t<%s>\n", kDefaultProducermanager.c_str());
    DoNamespacePartExport(0, config.producermanagers(), file);
    fprintf(file, "\t</%s>\n", kDefaultProducermanager.c_str());
  }
}
/*
 * 外部使用接口
 */
void ExportZkToXml(const std::string& xmlname, const std::string& out_path) {
  time_t t = time(NULL);
  tm *lt = localtime(&t);
  char buff[100];
  snprintf(buff, sizeof(buff), "%s__%d_%d_%d_%d_%d_%d.xml", xmlname.c_str(), 
    lt->tm_year + 1900, lt->tm_mon+1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
  std::string path = out_path + kZkSeparator + buff;
  
  FILE* file = fopen(path.c_str(), "w");
  fprintf(file, "<%s>\n", kTripod.c_str());
  PartExport(kDefaultCache, g_zk_config, file);
  PartExport(kDefaultCachemanager, g_zk_config, file);
  PartExport(kDefaultProducermanager, g_zk_config, file);
  fprintf(file, "</%s>\n", kTripod.c_str());

  fclose(file);
}
////////////////////////////////////////end rollback/////////////////////////


////////////////////////////////////将从xml解析的node更新到g_zk中去/////////////
// 求差集
//flag = 0 表示删除
//flag = 1 表示插入
static std::set<XmlNode> DifferenceSet(const std::set<XmlNode>& a,
    const std::set<XmlNode>& b, int flag) {
  std::set<XmlNode> results;
  for (std::set<XmlNode>::iterator it = a.begin(); it != a.end(); ++it) {
    std::set<XmlNode>::iterator its; 
    for(its = b.begin(); its != b.end(); ++its) {
      if ((it->nickname() == its->nickname()) && (it->toString() != its->toString())) {
        results.insert(*it);
      }
    }
    if (flag == 1) {
      for(its = b.begin(); its != b.end(); ++its) {
        if (it->nickname() == its->nickname()) {
          break;
        }
      }
      if (its == b.end()) {
        results.insert(*it);
      }
    }
  }
  return results;
}
// 删除xml中没有但是g_zk中有的节点
static void DoDelete(std::map<std::string, std::set<XmlNode> >& g_zkmap,
    std::map<std::string, std::set<XmlNode> > xmlmap, int flag) {
  std::string base_path;
  if (flag == 0) {
    base_path = kDefaultZkCacheZNode + kZkSeparator;
  } else if (flag == 1) {
    base_path = kDefaultZkCacheManagerZNode + kZkSeparator;
  } else {
    base_path = kDefaultZkProducerManagerZNode + kZkSeparator;
  }
  std::map<std::string, std::set<XmlNode> > diff;
  std::map<std::string, std::set<XmlNode> >::iterator it;
  for (it = g_zkmap.begin(); it != g_zkmap.end(); it++) {
    if (xmlmap.find(it->first) != xmlmap.end()) {
      diff[it->first] = DifferenceSet(it->second, xmlmap[it->first], 0);
    }
  }

  int rc = 0;
  if (!diff.empty()) {
    for (it = diff.begin(); it != diff.end(); ++it) {
      if (!(it->second.empty())) {
        for (std::set<XmlNode>::iterator i = it->second.begin(); i
            != it->second.end(); ++i) {
          rc = zoo_delete(g_zk, (base_path + it->first + kZkSeparator
              + i->toString()).c_str(), -1);
          if (rc != ZOK) {
            std::cout << "Can not delete path : " << base_path << it->first
                << kZkSeparator << i->toString() << std::endl;
            exit(-1);
          }
        }
        // 通知监听的g_zk，注意只有修改过g_zk才需要
        char buff[20];
        memset(buff, 0, sizeof(buff));
        snprintf(buff, sizeof(buff), "%ld", time(NULL));
        std::string tmp = it->first + buff;
        rc = zoo_set(g_zk, (base_path + it->first).c_str(), tmp.c_str(), strlen(tmp.c_str()), -1);
        if (rc != ZOK) {
          std::cout << "Can not set value path : " << base_path << it->first
              << std::endl;
          exit(-1);
        }
      }
    }
  }
}
// 删除xml中没有但是g_zk中有的节点
void Delete() {
  DoDelete(g_zk_config.caches(), g_xml_config.caches(), 0);
  DoDelete(g_zk_config.cachemanagers(), g_xml_config.cachemanagers(), 1);
  DoDelete(g_zk_config.producermanagers(), g_xml_config.producermanagers(), 2);
}

// 插入xml中有但是g_zk中没有的节点
static void DoInsert(std::map<std::string, std::set<XmlNode> >& g_zkmap,
    std::map<std::string, std::set<XmlNode> > xmlmap, int flag) {
  std::string base_path;
  if (flag == 0) {
    base_path = kDefaultZkCacheZNode + kZkSeparator;
  } else if (flag == 1) {
    base_path = kDefaultZkCacheManagerZNode + kZkSeparator;
  } else {
    base_path = kDefaultZkProducerManagerZNode + kZkSeparator;
  }
  std::map<std::string, std::set<XmlNode> > diff;
  std::map<std::string, std::set<XmlNode> >::iterator it;
  for (it = xmlmap.begin(); it != xmlmap.end(); it++) {
    if (g_zkmap.find(it->first) != g_zkmap.end()) {
      diff[it->first] = DifferenceSet(it->second, g_zkmap[it->first], 1);
    } else {
      diff[it->first] = it->second;
      
    }
  }
  int rc = 0;
  if (!diff.empty()) {
    for (it = diff.begin(); it != diff.end(); ++it) {
      // 看namespace是否需要创建
      rc = zoo_exists(g_zk, (base_path + it->first).c_str(), 0, 0);
      if (rc != ZOK) {
        // 不存在，就创建
        rc = zoo_create(g_zk, (base_path + it->first).c_str(), "0", 1,
            &ZOO_OPEN_ACL_UNSAFE, 0, 0, 0);
        if (rc != ZOK) {
          std::cout << "Can not create for path " << base_path << it->first
              << std::endl;
          exit(-1);
        }

        char buff[20];
        memset(buff, 0, sizeof(buff));
        snprintf(buff, sizeof(buff), "%ld", time(NULL));
        std::string tmp = it->first + buff;
        rc = zoo_set(g_zk, (base_path + it->first).c_str(), tmp.c_str(), strlen(tmp.c_str()), -1);
        if (rc != ZOK) {
          std::cout << "Can not set value path : " << base_path << it->first
              << std::endl;
          exit(-1);
        }
      }
      // 创建node
      if (!(it->second.empty())) {
        for (std::set<XmlNode>::iterator iIt = it->second.begin(); iIt
            != it->second.end(); ++iIt) {
          rc = zoo_create(g_zk, (base_path + it->first + kZkSeparator
              + iIt->toString()).c_str(), "", 0, &ZOO_OPEN_ACL_UNSAFE, 0, 0, 0);
          if (rc != ZOK) {
            std::cout << "Can not create for path :" << base_path << it->first
                << kZkSeparator << iIt->toString() << std::endl;
            exit(-1);
          }
        }
        // 通知g_zk的监听者，注意只有修改过g_zk才需要
        char buff[20];
        memset(buff, 0, sizeof(buff));
        snprintf(buff, sizeof(buff), "%ld", time(NULL));
        std::string tmp = it->first + buff;
        rc = zoo_set(g_zk, (base_path + it->first).c_str(), tmp.c_str(), strlen(tmp.c_str()), -1);
        if (rc != ZOK) {
          std::cout << "Can not set value path : " << base_path << it->first << std::endl;
          exit(-1);
        }
      }
    }
  }
}

//
bool InitPartPath(const std::string& path) {
  int rc = zoo_exists(g_zk, path.c_str(), 0, 0);
  if (rc != ZOK) {
    // 不存在，就创建
    rc = zoo_create(g_zk, path.c_str(), "0", 1,
            &ZOO_OPEN_ACL_UNSAFE, 0, 0, 0);
    if (rc != ZOK) {
      std::cout << "Can not create for path " << path.c_str() << std::endl;
      return false;
    } else {
      return true;
    }
  }
  return true;
}
//
bool InitTripodPath() {
  int rc = zoo_exists(g_zk, kDefaultZNodePrefix.c_str(), 0, 0);
  if (rc != ZOK) {
    // 不存在，就创建
    rc = zoo_create(g_zk, kDefaultZNodePrefix.c_str(), "0", 1,
            &ZOO_OPEN_ACL_UNSAFE, 0, 0, 0);
    if (rc != ZOK) {
      std::cout << "Can not create for path " << kDefaultZNodePrefix.c_str() << std::endl;
      return false;
    } else {
      return (InitPartPath(kDefaultZkCacheZNode) && InitPartPath(kDefaultZkCacheManagerZNode) && InitPartPath(kDefaultZkProducerManagerZNode)); 
    }
  }
  return true;
}



// 插入xml中有但是g_zk中没有的节点
void Insert() {
  DoInsert(g_zk_config.caches(), g_xml_config.caches(), 0);
  DoInsert(g_zk_config.cachemanagers(), g_xml_config.cachemanagers(), 1);
  DoInsert(g_zk_config.producermanagers(), g_xml_config.producermanagers(), 2);
}
//////////////////////////////////////end////////////////////////////////////


////////////////////////////////显示结果///////////////////////////////////////
/*
 * 显示结果
 */
void PrintConfig(TripodConfig& config) {
  std::map<std::string, std::set<XmlNode> >::iterator it;
  std::set<XmlNode>::iterator its;
  for (it = config.caches().begin(); it != config.caches().end(); it++) {
    std::cout << it->first << std::endl;
    for (its = it->second.begin(); its != it->second.end(); its++) {
      std::cout << its->toString() << std::endl;
    }
  }
  /////////////////////
  for (it = config.cachemanagers().begin(); it != config.cachemanagers().end(); it++) {
    std::cout << it->first << std::endl;
    for (its = it->second.begin(); its != it->second.end(); its++) {
      std::cout << its->toString() << std::endl;
    }
  }
  ///////////////////
  for (it = config.producermanagers().begin(); it
      != config.producermanagers().end(); it++) {
    std::cout << it->first << std::endl;
    for (its = it->second.begin(); its != it->second.end(); its++) {
      std::cout << its->toString() << std::endl;
    }
  }
}

////////////////////////////////////////////////////////////////////////////


////////////////////////////////初始化g_zk//////////////////////////////////
bool InitZk(const std::string& zk_address) {
  g_zk = zookeeper_init(zk_address.c_str(), 0, 10000, 0, 0, 0);
  int rc = zoo_create(g_zk, "/mutex", NULL, 0, &ZOO_OPEN_ACL_UNSAFE,
      ZOO_EPHEMERAL, NULL, 0);
  if (rc != ZOK) {
    std::cout << "Can not get lock!Someone else is using admin!" << std::endl;
    return false;
  }
  return true;
}
/////////////////////////////////end///////////////////////////////////////////
std::string GetFileName(const std::string path) {
  std::string xmlname;
  int first = path.rfind('/');
  int last = path.rfind('.');
  if (first >= 0 && last >= 0) {
    xmlname = path.substr(first + 1, last - first - 1);
  } else if (first < 0 && last >= 0) {
    xmlname = path.substr(0, last);
  }
  return xmlname;
}


bool ArgvCheck(int argc, char* argv[], std::string* address,
    std::string* xmlpath, std::string* outdir) {
  *address = argv[1];
  *xmlpath = argv[2];
  //分析zkaddress
  /*
  if (address->find(":") == -1) {
    std::cout << "===============zkaddress缺少端口，使用默认端口2181==============" 
      << std::endl;
    *address = *address + ":2181";
  }
*/
  
  std::vector<std::string> parts;
  split(parts, *address, is_any_of(","));
  std::string tmp = "";
  for(std::vector<std::string>::iterator it = parts.begin(); it != parts.end(); ++it) {
    if (it->find(":") == std::string::npos) {
      std::cout << "zkaddress缺少端口,程序将使用默认端口2181." << std::endl;
      tmp += (*it) + ":2181,";
    } else {
      tmp += (*it) + ",";
    }
  }
  *address = tmp.substr(0, tmp.size()-1);
  //std::cout << *address << std::endl;
  
  //分析xmlpath
  if (access(argv[2], R_OK) != 0) {
    std::cout << "==============xmlpath ：" << argv[2] << "不可用==============" 
      << std::endl;
    return false;
  }
  //分析outdir
  if (argc == 3) {
    *outdir = ".";
  } else {
    if (access(argv[3], W_OK) == 0) {
      *outdir = argv[3];
    } else {
      *outdir = ".";
      std::cout << "==============outdir : " << argv[3] << "不可用,使用当前目录====" 
        << std::endl;
    }
  }
  return true;
}


/*
 * 主函数
 */
int main(int argc, char* argv[]) {
  if (argc < 3 || argc > 4) {
    std::cout << "run error,correct way like is : ./exec zkaddress[ip:port] xmlpath[输入xml路径] outdir[导出的zk路径]" << std::endl;
    return 0;
  }
  std::string zk_address;
  std::string xml_path;
  std::string outdir = ".";
  std::string xmlname;

  if (!ArgvCheck(argc, argv, &zk_address, &xml_path, &outdir)) {
    return 0;
  }

  //根据路径名获得文件名字
  xmlname = GetFileName(xml_path);

  // zk的初始化
  if (!InitZk(zk_address)) {
    exit(-1);
  }
  // init base path : "/tripod2"
  if (InitTripodPath()) {
    std::cout << std::endl;
  }
  std::cout << "==============更新开始=============" << std::endl;  
  // 获得xml和zk的配置信息
  GetXmlConfig(xml_path);
  GetZooKeeperConfig();
  ExportZkToXml(xmlname, outdir);
  //update zk
  Delete();
  Insert();

  std::cout << "+++++++++++++zk更新成功++++++++++++" << std::endl;
  if (outdir == ".") {
    std::cout << "如果想察看zk配置，请到当前目录下的日期(如xmlname__2012_4_23_5_4_23).xml文件中察看" << std::endl;
  } else {
    std::cout << "如果想察看zk配置，请到目录:'" << outdir << "'的日期(如xmlname__2012_4_23_5_4_23).xml文件中察看" << std::endl;
  }
  zookeeper_close(g_zk);
  std::cout << "==============更新结束=============" << std::endl;

  return 0;
}

