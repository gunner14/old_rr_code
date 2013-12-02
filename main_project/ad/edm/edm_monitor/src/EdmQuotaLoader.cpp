#include <curl/curl.h>
#include <boost/lexical_cast.hpp>
#include "../include/EdmQuotaLoader.h"

USE_NS(std)
USE_NS(xercesc)
USE_NS(xce::edm)

const string AND = "&";
const int RESET = -1; //for loop will add i with 1, the next loop begin i value is 0.
const int ONE_SECOND = 1000;
const int MAX_SEND_ID_COUNT = 1;

EdmQuotaLoader::EdmQuotaLoader(){
    CURLcode result;
    result = curl_global_init( CURL_GLOBAL_ALL ); 
    if ( 0 != result){
        MCE_WARN("EdmQuotaLoader::constructor curl_global_init error, result = " << result);  
    } 
}

EdmQuotaLoader::~EdmQuotaLoader(){
    curl_global_cleanup();
}

void EdmQuotaLoader::GetIDs(const map<Ice::Long, DummyRecordPtr>& dataMap, vector<string>& idVec){
    map<Ice::Long, DummyRecordPtr>::const_iterator mIter = dataMap.begin();
    string element;
    int count = 0;
    for (; mIter != dataMap.end(); ++mIter, ++count){
	if (0 == count){
	    element = boost::lexical_cast<std::string>(mIter->first);
        } else {
	    element += AND + boost::lexical_cast<std::string>(mIter->first);
	}
        
	if (MAX_SEND_ID_COUNT == count){
	    idVec.push_back(element);
	    MCE_INFO("EdmQuotaLoader::GetIDs --> " << element);
	    element.clear();
	    count = RESET;
        }	
    }

    if (0 != element.size()){
	idVec.push_back(element);
    }
}

void EdmQuotaLoader::GetIDs2(const map<Ice::Long, Ice::Long>& dataMap, vector<string>& idVec){
  map<Ice::Long, Ice::Long>::const_iterator mIter = dataMap.begin();
  string element;
  int count = 0;
  for (; mIter != dataMap.end(); ++mIter, ++count){
    if (0 == count){
      element = boost::lexical_cast<std::string>(mIter->first);
    } else {
   	  element += AND + boost::lexical_cast<std::string>(mIter->first);
    }
    if (MAX_SEND_ID_COUNT == count){
      idVec.push_back(element);
      MCE_INFO("EdmQuotaLoader::GetIDs2 --> " << element);
      element.clear();
	  count = RESET;
	}
  }

  if (0 != element.size()){
    idVec.push_back(element);
  }
}

size_t WriteCallback(void *ptr, size_t size, size_t nmemb, void *data) {
    string *buff = (string *) data;
    buff->append((char *) ptr, size * nmemb);
    return size * nmemb;
}


bool EdmQuotaLoader::GetHttpResult(const string& url, string& result){
    MCE_INFO("EdmQuotaLoader::GetHttpResult --> " << url);
    CURL* handle = NULL;
    try{
        handle = curl_easy_init();
        curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(handle, CURLOPT_TIMEOUT_MS, ONE_SECOND);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, &result);
        curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
        CURLcode ret = curl_easy_perform(handle);
        curl_easy_cleanup(handle);

        if ( 0 < ret ){
	    result.clear();
	    MCE_WARN("EdmQuotaLoader::GetHttpResult --> Load Url Error! " << url << " ErrorType: " << ret);
	    return false;
        }
    } catch (...){
	MCE_WARN("EdmQuotaLoader::GetHttpResult --> Http Request Error!");
	curl_easy_cleanup(handle);
	return false;
    }
    return true;
}

bool EdmQuotaLoader::GetNodeValue(DOMNode* current, const string& nodeName, Ice::Long& value){
    if (0 == current){
	return false;
    }

    DOMNode* parent = current->getParentNode();
    string name = XMLString::transcode(parent->getNodeName());
    string valueStr = XMLString::transcode(current->getNodeValue());
    if (name == nodeName){ 
	value = boost::lexical_cast<Ice::Long>(valueStr);
        MCE_INFO("EdmQuotaLoader::GetNodeValue --> name: " << name << " value: " << value);
    	return true;
    }
    return false;
}

bool EdmQuotaLoader::ParseMembers(const string& result, map<Ice::Long, Ice::Long>& records){
  map<Ice::Long, Ice::Long> tempRecords;
  if (0 == result.size()){
 	return false;
  }
  bool ret = false;
  try{
    XMLPlatformUtils::Initialize();
    XercesDOMParser parser;
    char* pointer = new char[result.size() + 1]();
    strcpy(pointer, result.c_str());
    const string bufID = "fake_member_input";
    const XMLByte* p = (const XMLByte*)(pointer);
	MemBufInputSource input(p, strlen(pointer), bufID.c_str(), true);
	parser.parse(input);
	DOMDocument *xmlDoc = parser.getDocument();
	DOMElement* root = xmlDoc->getDocumentElement();
	DOMNodeIterator *iter = xmlDoc->createNodeIterator(root, DOMNodeFilter::SHOW_TEXT, NULL, true);
	Ice::Long key = 0;
	for (DOMNode* current = (DOMNode*)iter->nextNode(); current != 0; current = (DOMNode*)iter->nextNode()){
	  Ice::Long balance;
	  ret = GetNodeValue(current, MEMBER_ID, key);
	  if (!ret){
	    MCE_WARN("EdmQuotaLoader::Parse --> Parse member ID Key Error! " << key);
	    break;
	  }

	  current = (DOMNode*)iter->nextNode();
	  ret = GetNodeValue(current, MEMBER_BALANCE, balance);
	  if (!ret){
	    MCE_WARN("EdmQuotaLoader::Parse --> Parse member balance Value Error! " << balance);
	    break;
	  }

	  tempRecords[key] = balance;
	  MCE_INFO("EdmQuotaLoader Result: Key: " << key << "value:"<< balance);
	}
  } catch (const XMLException& toCatch){
      ret = false;
	  MCE_WARN("EdmQuotaLoader::Parse --> XMLException " );
  } catch (const std::exception& e){
	  ret = false;
	  MCE_WARN("EdmQuotaLoader::Parse --> STD Exception ");
  }
  XMLPlatformUtils::Terminate();

  if (!ret){
    return false;
  }

  {
    IceUtil::RWRecMutex::WLock lock(mutex_);
    map<Ice::Long, Ice::Long>::const_iterator mIter = tempRecords.begin();
    for (; mIter != tempRecords.end(); ++mIter){
      records[mIter->first] = mIter->second;
    }
  }
  return ret;
}
