/*
 * =====================================================================================
 *
 *       Filename:  EdmQuotaLoader.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年04月26日 11时10分04秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lei.yu1 
 *        Company:  renren
 *
 * =====================================================================================
 */

#ifndef __XCE_EDM_QUOTA_LOADER_H_
#define __XCE_EDM_QUOTA_LOADER_H_

#include <map>
#include <string>
#include <vector>
#include "LogWrapper.h"
#include "AdEdmMonitor.h"
#include <IceUtil/Shared.h>
#include <IceUtil/Handle.h>
#include <IceUtil/RWRecMutex.h>
#include <IceUtil/Mutex.h>
#include "../common/ns_common.h"
#include "../common/edm_monitor_struct.h"
#include "../common/edm_monitor_constant.h"
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <cstring>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMNodeIterator.hpp>

NS_BEGIN(xce, edm)

const string HTTP = "http://snipe.jebe.renren.com/snipe/account/adgroup/";

const string HTTP4MEM = "http://snipe.jebe.renren.com/snipe/account/";

/*struct DummyRecord : public IceUtil::Shared{
};

typedef IceUtil::Handle<DummyRecord> DummyRecordPtr;*/

class EdmQuotaLoader : public IceUtil::Shared{
public:
    EdmQuotaLoader();
    virtual ~EdmQuotaLoader();

    virtual void UpdateData() = 0;
    virtual void Clear() = 0;
    virtual bool Parse(const std::string& result, std::map<Ice::Long, DummyRecordPtr>& records) = 0;

    virtual DummyRecordPtr GetQuota(const EdmInfo&) = 0; 
	virtual std::map<Ice::Long ,DummyRecordPtr> GetAdGroupRecords() = 0;

    void GetIDs(const std::map<Ice::Long, DummyRecordPtr>&, std::vector<std::string>&);
    void GetIDs2(const std::map<Ice::Long, Ice::Long>&, std::vector<std::string>&);
	bool ParseMembers(const string& result, map<Ice::Long, Ice::Long>& records);
protected:
    bool GetNodeValue(xercesc::DOMNode* current, const std::string&, Ice::Long& value);
    bool GetHttpResult(const std::string& url, std::string& result);

protected:
    IceUtil::RWRecMutex mutex_;    
};

typedef IceUtil::Handle<EdmQuotaLoader> EdmQuotaLoaderPtr;

NS_END

#endif
