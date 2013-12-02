#include "Common.h"
#include "XmppTools.h"
#include "TalkProtoAdapter.h"
#include "ReceiversHandler.h"
#include "ExpatWrapper.h"
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

using namespace com::xiaonei::talk::function::transfer;
using namespace com::xiaonei::talk::proto;
using namespace com::xiaonei::talk::util;

int com::xiaonei::talk::function::transfer::Xml2Map(const string& FuncName, const string& xml, Str2StrMap& map) {
  ExpatWrapper w;
  int success = -1;
  success = w.GetSkeleton(xml, map);
  if(success < 0) 
    MCE_WARN(FuncName << "--> xml error :" << xml);

	for(Str2StrMap::iterator iter = map.begin(); iter != map.end(); ++iter) {
		ClientEncode(iter->second);
  }

  return success;
}

void com::xiaonei::talk::function::transfer::ClientEncode(string& body_string){
  //----------------------取离线2次转义特殊字符:-----------------------------------
	try {
		size_t pos = body_string.find("&");
		if(string::npos != pos) {
			body_string = boost::regex_replace(body_string, boost::regex("&"), string("&amp;"));
		}
		pos = body_string.find("<");
		if(string::npos != pos) {
			body_string = boost::regex_replace(body_string, boost::regex("<"), string("&lt;"));
		}
		pos = body_string.find(">");
		if(string::npos != pos) {
			body_string = boost::regex_replace(body_string, boost::regex(">"), string("&gt;"));
		}
		pos = body_string.find("\"");
		if(string::npos != pos) {
			body_string = boost::regex_replace(body_string, boost::regex("\""), string("&apos;"));
		}
	} catch(...) {
		MCE_WARN("PowerTransferI::ClientEncode --> error  body_string = " << body_string);
	}
}

void com::xiaonei::talk::function::transfer::Send2Proto(vector<string>& to_jids, Str2StrMap & msg) {
  if(to_jids.size() > 0) {
    try {
      msg["to_jid"] = ReceiversHandler::ReceiversAssembly(to_jids);
      TalkProtoAdapter::instance().ReceiveExpress(msg);
    } catch(Ice::Exception& e) {
      MCE_WARN("Send2Proto-->TalkProtoAdapter::ReceiveExpress error: " << e.what());
    }
    to_jids.clear();
  }
}
