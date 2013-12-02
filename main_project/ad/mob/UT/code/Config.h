/*
 *  CachePool.h
 *
 *   Created on: 2012-3-31
 *       Author: yongjun.yin
 *  Description: 定义引擎业务中使用的各种配置表
 *               (EngineConfig、PlatformMap、Brand3GMap、ResolutionMap)
 */

#ifndef __AD_MOB_CONFIG_H__
#define __AD_MOB_CONFIG_H__

#include "CachePool.h"

using namespace std;
using namespace boost;
using namespace xce::ad;

namespace xce{
namespace ad{

class EngineConfig : public CachePool, public MyUtil::Singleton<EngineConfig> {
public:
	void Initialize(int server_index) {
		set_server_index(server_index);
		ostringstream oss;
		oss << "AdMob" << server_index << "@" << GetIp();
		set_server_name( oss.str() );
	}

	string GetProperty(string& key);
	string GetPropertyWithDefault(string& key, string& value);
	int GetPropertyAsInt(string& key);
	int GetPropertyAsIntWithDefault(string& key, int value);

	void Print();
	size_t Size();
private:
	//void TotalLoad();
	string GetIp() {
		int result = 0;
		char hostName[100];
		struct hostent* host = NULL;
		result = gethostname(hostName, 100);
		host = gethostbyname(hostName);
		return string(inet_ntoa(*((struct in_addr *)host->h_addr_list[0])));
	}

	SETGET_DATA( string, server_name )
	SETGET_DATA( int, server_index )

	map<string, string> property_dict_;
};

class PlatformMap : public CachePool, public MyUtil::Singleton<PlatformMap> {
public:
  Ice::Long GetPlatform(string OsVersion);
  Ice::Long GetPlatformX(string OsVersion);
  size_t Size();
  string Print(int);
private:
  void TotalLoad();
  map<string, AdPlatformPtr> pool_;
};

class Brand3GMap : public CachePool, public MyUtil::Singleton<Brand3GMap> {
public:
  Ice::Long GetBrand3G(string brand3G);
  size_t Size();
  string Print(int);
private:
  void TotalLoad();
  map<string, Ice::Long> pool_;
};

class AbstractResolutionMap : public CachePool {
public:
  set<short> GetCeilWidths(short width);
  set<short> GetFloorWidths(short width);
  short GetFloorWidth(short width);
  short GetFixedWidth(short width);
  size_t Size();
  string Print(int);
protected:
  //void TotalLoad();
  vector< pair<short, string> > sorted_width_seq_;
};

class ResolutionMap : public AbstractResolutionMap, public MyUtil::Singleton<ResolutionMap> {
private:
	void TotalLoad();
};

class ResolutionMatcher : public AbstractResolutionMap, public MyUtil::Singleton<ResolutionMatcher> {
private:
	void TotalLoad();
};

}
}
#endif
