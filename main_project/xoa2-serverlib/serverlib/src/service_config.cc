#include "service_config.h"

// #include <stdio.h>
// #include <string.h>

#include <map>
#include <set>
#include <fstream>
#include <iostream>
#include <iterator>

#include <expat.h>
#include <boost/lexical_cast.hpp>

#include "UtilCxx/src/LogWrapper.h"
namespace xoa2 {

int Xoa2ServiceConfig::Init(const std::string & conf_file) {
  std::string conf;
  {
    std::ifstream fin(conf_file.c_str(), std::ios::binary);
    if (!fin) {
      MCE_ERROR(stderr << "open cs file error");
      return 1;
    }
    printf("open cs file ok\n");
    fin.unsetf(std::ios::skipws);
    std::copy(std::istream_iterator<char>(fin), std::istream_iterator<char>() , std::back_inserter(conf));
  }
  //printf("%s", conf.c_str());

  XML_Parser parser = XML_ParserCreate(NULL);
  if (!parser) {
    return -1;
  }

  XML_SetUserData(parser, this);
  XML_SetElementHandler(parser, SchemaStartTag, SchemaEndTag);
  // XML_SetElementHandler(parser, NULL, SchemaEndTag);
  if (!XML_Parse(parser, conf.c_str(), conf.size(), 1)) {
    MCE_ERROR(stderr << "Parse error at line "
        << XML_GetCurrentLineNumber(parser) << ":\n"
        << XML_ErrorString(XML_GetErrorCode(parser)));
    XML_ParserFree(parser);
    return -2;
  }

  XML_ParserFree(parser);
  return 0;
}

std::string Xoa2ServiceConfig::GetPath(const std::string & key_name) const {
  std::string full_tag;
  for (size_t i = 0; i < tag_path_.size(); ++i) {
    full_tag += tag_path_[i];
    full_tag += '.';
  }
  full_tag += key_name;
  return full_tag;
}

//el是每隔标签的开头，attr[2*n]是属性名，attr[2*n+1]是属性值。
void Xoa2ServiceConfig::SchemaStartTag(void *data, const char *el, const char **attr) {
  Xoa2ServiceConfig * obj = (Xoa2ServiceConfig *)data;
  std::string attr_name;
  std::string attr_value;
  
  std::string path = obj->GetPath(el);
  for (size_t i = 0; attr[i]; i += 2) {
      attr_name = attr[i];
      attr_value = attr[i + 1];
      if (path == "xoa-app.services.service") {
        if (attr_name == "port") {
          obj->listen_port_ = boost::lexical_cast<int>(attr_value);
        } else if (attr_name == "id") {
          obj->service_id_ = attr_value;
        } else if (attr_name == "version") {
          obj->version_ = attr_value; 
        } 
      } else if (path == "xoa-app.services.service.worker-thread-pool") {
        if (attr_name == "core-size") {
          obj->thread_pool_core_size_ = boost::lexical_cast<int>(attr_value);
        } else if (attr_name == "max-size") {
          obj->thread_pool_max_size_ = boost::lexical_cast<int>(attr_value);
        }
      }
      // cout << path << " " << attr_name << " = " << attr_value << endl;
  }
  obj->tag_path_.push_back(el);
}

void Xoa2ServiceConfig::SchemaEndTag(void *data, const char *el) {
  Xoa2ServiceConfig  * obj = (Xoa2ServiceConfig *)data;
  obj->tag_path_.pop_back();
}

}

//int main(int argc, char **argv) {
//  xoa2::Xoa2ServiceConfig conf;
//  conf.Init(std::string("./xoa.xml"));
//  std::cout << conf.thread_pool_core_size() << std::endl;
//  std::cout << conf.thread_pool_max_size() << std::endl;
//  std::cout << conf.service_id() << std::endl;
//  return 0;
//}

