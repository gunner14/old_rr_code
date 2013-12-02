#include "ExpatWrapper.h"
#include <stdlib.h>
#include <algorithm>
#include <cstring>
#include <sstream>
#include <boost/lexical_cast.hpp>

namespace com {
namespace xiaonei {
namespace talk {
namespace util {
using namespace std;

static void StartElement(void *Wrapper, const XML_Char *name, const XML_Char **atts){
	ExpatWrapper* wrapper = (ExpatWrapper*) Wrapper;
	wrapper->StartNode(name, atts);
}

static void EndElement(void *Wrapper, const XML_Char *name){
	ExpatWrapper* wrapper = (ExpatWrapper*) Wrapper;
	wrapper->EndNode(name);
}


static void ValueElement(void *Wrapper, const XML_Char *value, int len){
	ExpatWrapper* wrapper = (ExpatWrapper*) Wrapper;
	wrapper->ValueHandle(value, len);
}


//======================================================================================

ExpatWrapper::ExpatWrapper() {
	parser_ = XML_ParserCreate(NULL);
}

ExpatWrapper::~ExpatWrapper(){
	XML_ParserFree(parser_);
}


int ExpatWrapper::GetSkeleton(const string &xml, map<string,string> &ans) {
	XML_SetUserData(parser_, this);
  XML_SetElementHandler(parser_, StartElement, EndElement);		
  XML_SetCharacterDataHandler(parser_, ValueElement);
  skeleton_.clear();
  if (XML_Parse(parser_, xml.c_str(), xml.size(), 1) == XML_STATUS_ERROR){ 
    //fprintf(stderr,"%s at line %" XML_FMT_INT_MOD "u\n",  XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));     
    return -1;
  }

  GetSkeletonNodeCount();

	ans.swap(skeleton_);
	return 1;
}

void ExpatWrapper::GetSkeletonNodeCount() {
  char buff[20];
  for (map<string, int>::const_iterator it = node_count_.begin();
       it != node_count_.end(); it++) {
    if (it->second > 0) {
      snprintf(buff, sizeof(buff), "%d", it->second + 1);
      skeleton_[it->first + "%"] = buff;
    }
  }
}

int ExpatWrapper::AddNodeCount(const string& key) {
  map<string, int>::iterator it = node_count_.find(key);
  if (it != node_count_.end()) {
    return ++(it->second);

  } else {
    node_count_[key] = 0;
    return 0;
  }
  
}

void ExpatWrapper::PushNode(const string& name) {
  //MCE_DEBUG("ExpatWrapper::PushNode--> bef:" << node_stack_);
  if (!node_stack_.empty())
    node_stack_ += NODE_SEPARATOR;
  node_stack_ += name;

  int cn = AddNodeCount(node_stack_);

  if (cn > 0) {
    char buff[20];
    snprintf(buff, sizeof(buff), "@%d", cn);
    node_stack_ += buff;
  }

  //MCE_DEBUG("ExpatWrapper::PushNode--> end:" << node_stack_);

}

void ExpatWrapper::PopNode() {
  int pos = node_stack_.rfind(NODE_SEPARATOR);
  //MCE_DEBUG("ExpatWrapper::PopNode--> bef:" << node_stack_ << " pos:" << pos);

  if (string::npos == pos) {
    node_stack_.clear();
  } else {
    node_stack_.resize(pos);
  }
  //MCE_DEBUG("ExpatWrapper::PopNode--> end:" << node_stack_);
}



void ExpatWrapper::StartNode(const string& name, const XML_Char **atts){
  PushNode(name);
  const string &current_key = node_stack_;

	for(int i = 0; atts[i] != NULL; i += 2) {
    //MCE_DEBUG("ExpatWrapper::StartNode--> current_key:" << current_key << ATT_SEPARATOR << atts[i] << " value:" << atts[i + 1]);
    skeleton_[current_key + ATT_SEPARATOR + atts[i]] = atts[i + 1];
	}

}

void ExpatWrapper::EndNode(const XML_Char *name){
  const string &current_key = node_stack_;
  map<string, string>::const_iterator it = node_value_.find(current_key);
  if (it != node_value_.end()) {
    skeleton_[current_key] = it->second;
    //MCE_DEBUG("ExpatWrapper::EndNode--> name:" << name << " current_key:" << current_key << " value:" << it->second);
  } else {
    // 跑到这里，说明这个节点下没有数据，没有走到ValueHandle
    // 举例 <node/>  <node atr1="dd"/>
    skeleton_[current_key] = "";
    //MCE_DEBUG("ExpatWrapper::EndNode--> empty node:" << " current_key:" << current_key);
  }

  PopNode();
}

void ExpatWrapper::ValueHandle(const XML_Char *value, int len){
  const string &current_key = node_stack_;

  map<string, string>::iterator it = node_value_.find(current_key);
  if (it != node_value_.end()) {
    it->second.insert(it->second.size(), value, len);

  } else {
    node_value_[current_key] = string(value, len);
  }

}


};
};
};
};

