/*
 * =====================================================================================
 *
 *       Filename:  http_loader.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年08月23日
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lei.yu1 
 *        Company:  renren
 *
 * =====================================================================================
 */

#ifndef __XCE_AD_HTTP_LOADER__
#define __XCE_AD_HTTP_LOADER__

#include <map>
#include <vector>
#include <cstring>
#include "LogWrapper.h"
#include <curl/curl.h>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMNodeIterator.hpp>
#include "../common/struct.h"
#include "compute_ratio.h"
#include "Singleton.h"

namespace xce{
namespace ad{

class HttpQuotaLoader : public MyUtil::Singleton<HttpQuotaLoader>{
public:
  HttpQuotaLoader();
  ~HttpQuotaLoader();

  void Load(map<Ice::Long, DummyRecordPtr>& records, vector<std::string> node_name, ComputeRatioPtr computer, const std::string& url);

protected:
  void GetIDs(const map<Ice::Long, DummyRecordPtr>& id_map, vector<string>& id_vec);
  bool GetHttpResult(const string& url, string& result);
  bool GetNodeValue(xercesc::DOMNode* current, const std::string& node_name, double& value);

  bool Parse(const string& result, std::map<Ice::Long, DummyRecordPtr>& records, const std::vector<std::string>& node_name, ComputeRatioPtr computer);
};//end of class HttpLoader 

}//end of namespace ad
}//end of namespace xce

#endif
