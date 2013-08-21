#ifndef _INFOCONFIG_PARSER_H_
#define _INFOCONFIG_PARSER_H_

#include <string>
#include <map>
using std::string;
using std::map;


/**
 * 1.解析类似window的.inf后缀的配置文件
 * [name]
 * key1=value2
 * key2=value2
 * 2.解析结果为
 * map<string,map<string,string> > mapInfConf;
 * 3.支持继承
 * [name:basename]
 * key3=key4
 *
 */

class CInfConfigParser
{
public:
  CInfConfigParser(){m_tLastModifyTime=0;}

  bool ParseInfConf(const string& strConfFile, map<string,map<string,string> >& mapInfConf) const;
private:
  mutable time_t m_tLastModifyTime;

};
#endif
