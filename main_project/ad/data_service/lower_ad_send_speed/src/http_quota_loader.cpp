#include <boost/lexical_cast.hpp>
#include "../include/http_quota_loader.h"

using namespace std;
using namespace xercesc;
using namespace xce::ad;

const int RESET = -1;
const string AND = "&";
const int MAX_SEND_ID_COUNT = 10;
const int ONE_SECOND = 1000;

HttpQuotaLoader::HttpQuotaLoader(){
  CURLcode result;
  result = curl_global_init( CURL_GLOBAL_ALL );
  if ( 0 != result){
    MCE_WARN("HttpQuotaLoader::constructor curl_global_init error, result = " << result);
  }
}

HttpQuotaLoader::~HttpQuotaLoader(){
  curl_global_cleanup();
}

void HttpQuotaLoader::Load(map<Ice::Long, DummyRecordPtr>& records, vector<std::string> node_name, ComputeRatioPtr computer, const std::string& http){
  vector<string> id_vec;
  GetIDs(records, id_vec);

  vector<string>::iterator v_iter = id_vec.begin();
  for (; v_iter != id_vec.end(); ++v_iter){
    string url = http + *v_iter;
    string result;
    GetHttpResult(url, result);
    //MCE_DEBUG("Http Result: " << result);
    Parse(result, records, node_name, computer);
  } 
} 

void HttpQuotaLoader::GetIDs(const map<Ice::Long, DummyRecordPtr>& data_map, vector<string>& id_vec){
    map<Ice::Long, DummyRecordPtr>::const_iterator m_iter = data_map.begin();
    string element;
    int count = 0;
    for (; m_iter != data_map.end(); ++m_iter, ++count){
        if (0 == count){
            element = boost::lexical_cast<std::string>(m_iter->first);
        } else {
            element += AND + boost::lexical_cast<std::string>(m_iter->first);
        }

        if (MAX_SEND_ID_COUNT == count){
            id_vec.push_back(element);
            //MCE_DEBUG("HttpQuotaLoader::GetIDs --> " << element);
            element.clear();
            count = RESET;
        }
    }

    if (0 != element.size()){
        id_vec.push_back(element);
    }
}

size_t WriteCallback(void *ptr, size_t size, size_t nmemb, void *data) {
    string *buff = (string *) data;
    buff->append((char *) ptr, size * nmemb);
    return size * nmemb;
}


bool HttpQuotaLoader::GetHttpResult(const string& url, string& result){
    //MCE_DEBUG("HttpQuotaLoader::GetHttpResult --> " << url);
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
            MCE_WARN("HttpQuotaLoader::GetHttpResult --> Load Url Error! " << url << " ErrorType: " << ret);
            return false;
        }
    } catch (...){
        MCE_WARN("HttpQuotaLoader::GetHttpResult --> Http Request Error!");
        curl_easy_cleanup(handle);
        return false;
    }
    return true;
}

bool HttpQuotaLoader::GetNodeValue(DOMNode* current, const string& node_name, double& value){
    if (0 == current){
        return false;
    }

    DOMNode* parent = current->getParentNode();
    string name = XMLString::transcode(parent->getNodeName());
    string valueStr = XMLString::transcode(current->getNodeValue());
    if (name == node_name){
        value = boost::lexical_cast<double>(valueStr);
        //MCE_DEBUG("HttpQuotaLoader::GetNodeValue --> name: " << name << " value: " << value);
        return true;
    }
    return false;
}

bool HttpQuotaLoader::Parse(const string& result, map<Ice::Long, DummyRecordPtr>& records, const vector<string>& value_name, ComputeRatioPtr computer){
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
    double key = 0;
    for (DOMNode* current = (DOMNode*)iter->nextNode(); current != 0; current = (DOMNode*)iter->nextNode()){
      vector<string>::const_iterator v_iter = value_name.begin();
      ret = GetNodeValue(current, *v_iter, key);
      if (!ret){
        MCE_WARN("HttpQuotaLoader::Parse --> Parse Member ID Key Error! name: " << *v_iter << " value: "<< key);
        break;
      }
      map<string, double> node_values;
      for (++v_iter; v_iter != value_name.end(); ++v_iter){
	current = (DOMNode*)iter->nextNode();
        node_values[*v_iter] = 0.0;
	ret = GetNodeValue(current, *v_iter, node_values[*v_iter]);
        if (!ret){
	  MCE_WARN("QuotaLoader::Parse --> Parse node error name: " << *v_iter <<  " value: " << node_values[*v_iter]);
	  break;
        } 
      }

      if (!ret){ break; }

      records[(Ice::Long)key]->value_ = computer->Compute(node_values); 
    }
  } catch (const XMLException& toCatch){
    ret = false;
    MCE_WARN("HttpQuotaLoader::Parse --> XMLException " );
  } catch (const std::exception& e){
    ret = false;
    MCE_WARN("HttpQuotaLoader::Parse --> STD Exception ");
  }
  XMLPlatformUtils::Terminate();

  return ret;
}
 
