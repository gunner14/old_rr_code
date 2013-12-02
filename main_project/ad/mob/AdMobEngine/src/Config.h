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
	void InitializeServer(int server_index); 
	string GetPropertyWithDefault(const string& key, string& value);
	int GetPropertyAsIntWithDefault(const string& key, int value);
	double GetPropertyAsDoubleWithDefault(const string& key, double value);
	void Print();
	size_t Size();
	string GetIp();

private:
	map<string, string> property_dict_;
	map<string, int> property_dict_int_;
	map<string, double> property_dict_double_;
	
	void TotalLoad();
	string GetProperty(const string& key);

	SETGET_DATA( string, server_name )
	SETGET_DATA( int, server_index )
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
