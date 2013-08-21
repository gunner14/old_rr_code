#include "memcache.h"

#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>
using namespace std;

//CMemcache g_memcache;
DiskCache gDiskCache;

CMemcache::CMemcache()
{
	m_client=NULL;
}
CMemcache::~CMemcache()
{
	if(m_client)
		memcached_free(m_client);
}
bool CMemcache::Init(const string& strConfFile)
{
	static time_t tLastModifyTime=0;
	struct stat st;
	if(stat(strConfFile.c_str(),&st)!=0){
		cerr<<"file not found:"<<strConfFile<<endl;
		return false;
	}
	if(st.st_mtime<=tLastModifyTime){
		return true;
	}
	tLastModifyTime=st.st_mtime;

	ifstream fin(strConfFile.c_str());
	if(!fin){
		cerr<<"open file fail:"<<strConfFile<<endl;
		return false;
	}
	
	memcached_st* client=memcached_create(NULL);
	memcached_return& rc=m_rc;

	string strLine;
	string strHost;
	uint32_t nPort;
	const std::string kSep(": ");
	const uint32_t kDefaultPort = 11211;
	std::vector<std::string> host_port_vec;
	while(getline(fin,strLine)) {
	    boost::trim(strLine);
		if(strLine.size()<2 || strLine[0]=='#')
			continue;
		
	    boost::split(host_port_vec, strLine, boost::is_any_of(kSep), boost::token_compress_on);
	    if (host_port_vec.size() == 1) {
	      strHost = host_port_vec[0];
	      nPort = kDefaultPort;
	    }
	    else if (host_port_vec.size() == 2) {
	      strHost = host_port_vec[0];
	      nPort = atoi(host_port_vec[1].c_str());
	    }
	    else {
	      // wrong host port configure
	      continue;
	    }
		rc=memcached_server_add(client, strHost.c_str(), nPort);
	}
	
	
	if(m_client)
		memcached_free(m_client);
	m_client=client;
	memcached_behavior_set(m_client, MEMCACHED_BEHAVIOR_DISTRIBUTION, MEMCACHED_DISTRIBUTION_CONSISTENT);	//重要,必须调用此语句使"一致性hash生效"
  return true;
}
bool CMemcache::Set(const string& strKey, const string& strValue, time_t expiration_secs, uint32_t flags)
{
	memcached_return& rc=m_rc;
	rc=memcached_set(m_client, strKey.c_str(), strKey.size(), strValue.c_str(), strValue.size(),  expiration_secs, flags);
	return rc==MEMCACHED_SUCCESS;
}
bool CMemcache::Delete(const string& strKey)
{
	memcached_return& rc=m_rc;
	rc=memcached_delete(m_client, strKey.c_str(), strKey.size(), 0);	//最后一个参数表示多少秒后再删除,0表示立即删除
	return rc==MEMCACHED_SUCCESS;
}
string CMemcache::Get(const string& strKey)
{
	string strValue;
	size_t len=0;
	memcached_return& rc=m_rc;
	uint32_t flags;
	char *value=memcached_get(m_client,strKey.c_str(), strKey.size(),&len,&flags,&rc);
	if(value){
		strValue=string(value,len);
		free(value);
	}
	return strValue;		
}

uint32_t CMemcache::Gets(map<string,string>& mapKey2Value)
{
	uint32_t nExsitCount=0;
	uint32_t num_keys=mapKey2Value.size();
	char **keys=new char*[num_keys];
	size_t* keys_length=new size_t[num_keys];
	size_t i=0;
	for(map<string,string>::iterator itMap=mapKey2Value.begin(); itMap!=mapKey2Value.end(); itMap++){
		const string& strKey=itMap->first;
		keys[i] = (char *)strKey.c_str();
		keys_length[i] = strKey.size();
		i++;
	}

	memcached_return& rc=m_rc;
	rc=memcached_mget(m_client, keys, keys_length, num_keys); 
	char return_key[MEMCACHED_MAX_KEY];
	size_t return_key_length;
	char *return_value;
	size_t return_value_length;
	uint32_t flags;
	do{
		return_value=memcached_fetch(m_client, return_key, &return_key_length, &return_value_length, &flags, &rc);
		if(rc==MEMCACHED_END)
			break;	
		if(return_value){
			string strKey=string(return_key,return_key_length);
			mapKey2Value[strKey]=string(return_value,return_value_length);
			free(return_value);
			nExsitCount++;
		}
	}
	while(true);
	
	return nExsitCount;
}


struct CacheItem {
  std::string key;
  std::string ip;
  int port;
};

CacheItem _cache[10] = {
  {"/fmn056", "10.7.18.26", 11211},
  {"/fmn057", "10.7.18.26", 11211},
  {"/fmn058", "10.7.18.26", 11211},

  {"/fmn059", "10.7.18.27", 11211},
  {"/fmn060", "10.7.18.27", 11211},
  {"/fmn061", "10.7.18.27", 11211},
  {"/fmn062", "10.7.18.27", 11211},

  {"/fmn063", "10.7.18.28", 11211},
  {"/fmn064", "10.7.18.28", 11211},
  {"/fmn065", "10.7.18.28", 11211},
};

std::map<std::string, int> _servermap;


CMemCacheBase::CMemCacheBase() : client_(NULL), rc_(MEMCACHED_SUCCESS), valid_(false){

}
CMemCacheBase::~CMemCacheBase() {
  if (client_) {
    memcached_free(client_);
  }
}
bool CMemCacheBase::Init(const std::string& server_conf) {
  valid_ = false;
  if (client_) {
    memcached_free(client_);
  }
  client_ = memcached_create(NULL);

  struct stat st;
  if (stat(server_conf.c_str(), &st) != 0) {
    cerr << "file not found: " << server_conf << endl;
    return false;
  }

  ifstream fin(server_conf.c_str());
  if(!fin){
    cerr<<"open file fail:"<<server_conf<<endl;
    return false;
  }

  std::string line, host;
  uint32_t port;
  const std::string kSep(": ");
  const uint32_t kDefaultPort = 11211;
  std::vector < std::string > host_port_vec;
  while (getline(fin, line)) {
    boost::trim(line);
    if (line.size() < 2 || line[0] == '#') {
      continue;
    }

    boost::split(host_port_vec, line, boost::is_any_of(kSep),
        boost::token_compress_on);
    if (host_port_vec.size() == 1) {
      host = host_port_vec[0];
      port = kDefaultPort;
    } else if (host_port_vec.size() == 2) {
      host = host_port_vec[0];
      port = atoi(host_port_vec[1].c_str());
    } else {
      // wrong host port configure
      continue;
    }
    rc_ = memcached_server_add(client_, host.c_str(), port);
    if (rc_ != MEMCACHED_SUCCESS) {
      cerr << "Error: memcached add server failed: host: " << host << " port: "
          << port << endl;
      return false;
    }
    valid_ = true;
  }
  valid_ = valid_ && SetBehavior();
  return valid_;
}
bool CMemCacheBase::Valid() const {
  return valid_;
}
bool CMemCacheBase::Set(const std::string& key, const string& value, time_t expiration, uint32_t flags) {
  if (!client_) return false;
  rc_ = memcached_set(client_, key.c_str(), key.size(), value.c_str(), value.size(),
       expiration, flags);
  return rc_ == MEMCACHED_SUCCESS;
}
bool CMemCacheBase::Get(const std::string& key, std::string* value) {
  if (!client_ || !value)
    return false;
  size_t ret_value_size = 0;
  uint32_t ret_flags = 0;
  char* ret_value = memcached_get(client_, key.c_str(), key.size(),
      &ret_value_size, &ret_flags, &rc_);
  if (rc_ != MEMCACHED_SUCCESS) {
    return false;
  }
  if (ret_value) {
    value->assign(ret_value, ret_value_size);
  } else {
    value->clear();
  }
  free(ret_value);
  return true;
}
memcached_return CMemCacheBase::LastError() const {
  return memcached_last_error(client_);
}

const char* CMemCacheBase::LastErrorMsg() const {
  return memcached_last_error_message(client_);
}

bool DiskCache::SetBehavior() {
  if (!client_) return false;
  // consistent hashing
  rc_ = memcached_behavior_set(client_, MEMCACHED_BEHAVIOR_DISTRIBUTION, MEMCACHED_DISTRIBUTION_CONSISTENT);
  if (rc_ != MEMCACHED_SUCCESS) {
    cerr << "Error: memcached set behavior: MEMCACHED_BEHAVIOR_DISTRIBUTION\n";
    return false;
  }
  // disk cache can only work in binary protocol
  rc_ = memcached_behavior_set(client_, MEMCACHED_BEHAVIOR_BINARY_PROTOCOL, true);
  if (rc_ != MEMCACHED_SUCCESS) {
    cerr << "Error: memcached set behavior: MEMCACHED_BEHAVIOR_BINARY_PROTOCOL\n";
    return false;
  }
  // CAS
  rc_ = memcached_behavior_set(client_, MEMCACHED_BEHAVIOR_SUPPORT_CAS, 1);
  if (rc_ != MEMCACHED_SUCCESS) {
    cerr << "Error: memcached set behavior: MEMCACHED_BEHAVIOR_SUPPORT_CAS\n";
    return false;
  }
  rc_ = memcached_behavior_set(client_, MEMCACHED_BEHAVIOR_DEAD_TIMEOUT, 60);
  if (rc_ != MEMCACHED_SUCCESS) {
    cerr << "Error: memcached set behavior: MEMCACHED_BEHAVIOR_DEAD_TIMEOUT\n";
    return false;
  }
  // send timeout in ms
  rc_ = memcached_behavior_set(client_, MEMCACHED_BEHAVIOR_SND_TIMEOUT, 2000);
  if (rc_ != MEMCACHED_SUCCESS) {
    cerr << "Error: memcached set behavior: MEMCACHED_BEHAVIOR_SND_TIMEOUT\n";
    return false;
  }
  // receive timeout in ms
  rc_ = memcached_behavior_set(client_, MEMCACHED_BEHAVIOR_RCV_TIMEOUT, 2000);
  if (rc_ != MEMCACHED_SUCCESS) {
    cerr << "Error: memcached set behavior: MEMCACHED_BEHAVIOR_RCV_TIMEOUT\n";
    return false;
  }
  // retry time out 1 second
  rc_ = memcached_behavior_set(client_, MEMCACHED_BEHAVIOR_RETRY_TIMEOUT, 1);
  if (rc_ != MEMCACHED_SUCCESS) {
    cerr << "Error: memcached set behavior: MEMCACHED_BEHAVIOR_RETRY_TIMEOUT\n";
    return false;
  }
  rc_ = memcached_behavior_set(client_, MEMCACHED_BEHAVIOR_REMOVE_FAILED_SERVERS, 3);
  if (rc_ != MEMCACHED_SUCCESS) {
    cerr << "Error: memcached set behavior: MEMCACHED_BEHAVIOR_REMOVE_FAILED_SERVERS\n";
    return false;
  }
  return true;
}

static uint32_t my_hash_function(const char *str, size_t length, void *context) {
  if (length <= 7) // sizeof("/fmn055")
    return 0;
  std::map<std::string, int>::iterator it = _servermap.find(std::string(str, 7));
  if (it != _servermap.end())
    return it->second;
  return 0;
}

