#ifndef __EXPATWRAPPER_H__
#define __EXPATWRAPPER_H__
#include <string>
#include <vector>
#include <list>
#include <utility>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include "TalkUtil/src/expat/expat.h"
#include "IceLogger.h"

namespace com {
namespace xiaonei {
namespace talk {
namespace util {

using namespace std;

const string NODE_SEPARATOR = ".";
const string ATT_SEPARATOR = ":";

class ExpatWrapper{
public:
	ExpatWrapper();
	~ExpatWrapper();

	int GetSkeleton(const string &xml, map<string,string> &ans);//获取xml转化后的骨架map

	void StartNode(const string& name, const XML_Char **atts); //回调函数 node开始
	void EndNode(const XML_Char *name); // 回调函数 node结束
	void ValueHandle(const XML_Char *value, int len);	//回调函数 处理node value

  void PushNode(const string& name);
  int AddNodeCount(const string& key);
  void PopNode();
  void GetSkeletonNodeCount();
private:
	map<string, string> skeleton_; //xml 解析后的骨架map
	XML_Parser parser_; //解析器

  map<string, int> node_count_;
  map<string, string> node_value_;
  string node_stack_;

};



};
};
};
};
#endif
//__EXPATWRAPPER_H__
