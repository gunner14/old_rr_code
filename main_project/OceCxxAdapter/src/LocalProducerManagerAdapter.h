#ifndef __COM_RENREN_DATASYNC_LOCALPRODUCERMANAGERADAPTER_H__ 
#define __COM_RENREN_DATASYNC_LOCALPRODUCERMANAGERADAPTER_H__ 

#include <DataSync.h>
#include <AdapterI.h>

#include <DataSync/share/SyncMessageI.hpp>

namespace com {
namespace renren {
namespace datasync {

template <class TKey>
class LocalProducerManagerAdapter : public MyUtil::ReplicatedClusterAdapterI<LocalProducerManagerPrx>,
                                    public IceUtil::Thread {
public : 
  LocalProducerManagerAdapter(const string& source, const string& table, const long expire = 2147483647, const bool isDel = false) :
      MyUtil::ReplicatedClusterAdapterI<LocalProducerManagerPrx>("ControllerLocalProducerManager", 120, 300),
      dbSource_(source), dbTable_(table), expire_(expire) , isDel_(isDel){
    start().detach();
  }

  void add(TKey key) {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(mutex_);
    keys_.push_back(boost::lexical_cast<std::string>(key));
    mutex_.notify();
  }
  
  void add(std::vector<TKey> keys) {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(mutex_);
    for(typename std::vector<TKey>::iterator it = keys.begin(); it != keys.end(); ++it) {
        keys_.push_back(boost::lexical_cast<std::string>(*it));
    }
    mutex_.notify();
  }

protected:

  virtual std::vector<std::string> getMsgs(const std::vector<std::string>& keys) = 0;

private:
  void produce(const std::vector<std::string>& keys) {
    std::vector<std::string> msgs = getMsgs(keys);

    for(std::vector<std::string>::const_iterator it = msgs.begin(); it != msgs.end(); ++it) {
        try {
          getProxy(0)->produce(dbTable_, std::vector<unsigned char>((*it).begin(), (*it).end()));
        }catch(Ice::Exception& e) {
          MCE_WARN("LocalProducerManagerAdapter::produce table:" << dbTable_ 
              << " ,size: " << it->size() << " Ice::Exception " << e.what());
        }catch(...) {
          MCE_WARN("LocalProducerManagerAdapter::produce table:" << dbTable_
              << " ,size: " << it->size() << " UnKnownException ");
        }
    }
  }

  void run() {
    while(true) {
      std::vector<std::string> keys;
      {
        IceUtil::Monitor<IceUtil::Mutex>::Lock lock(mutex_);
        if(keys_.empty()) {
          mutex_.wait();
        }
        keys.swap(keys_);
      }
      produce(keys);
    }
  }

protected:
  std::string dbSource_;
  std::string dbTable_;
  long expire_;
  bool isDel_;
  IceUtil::Monitor<IceUtil::Mutex> mutex_;
  std::vector<std::string> keys_;
};

template <class TKey>
class SingleLocalProducerManagerAdapter : public LocalProducerManagerAdapter<TKey> {
public:
  SingleLocalProducerManagerAdapter(const string& source, const string& table, const long expire = 2147483647, bool isDel = false) : LocalProducerManagerAdapter<TKey>(source, table, expire, isDel) {
  }

protected:
  virtual std::vector<std::string> getMsgs(const std::vector<std::string>& keys) {
    SyncMessageI sm(LocalProducerManagerAdapter<TKey>::dbSource_, LocalProducerManagerAdapter<TKey>::dbTable_, keys, false, "", -1, LocalProducerManagerAdapter<TKey>::expire_, LocalProducerManagerAdapter<TKey>::isDel_);
    std::string smStr;
    sm.SerializeToString(&smStr); 
    std::vector<std::string> res;
    res.push_back(smStr);
    return res;
  }
};

template <class TKey>
class RouterLocalProducerManagerAdapter : public LocalProducerManagerAdapter<TKey> {
public:
  RouterLocalProducerManagerAdapter(const string& source, const string& table, const int cluster, const long expire = 2147483647, bool isDel = false) : LocalProducerManagerAdapter<TKey>(source, table, expire, isDel), dbCluster_(cluster) {
  }

protected:
  virtual std::vector<std::string> getMsgs(const std::vector<std::string>& keys) {
    std::map<int, std::vector<std::string> > tmpRes;
    for(std::vector<std::string>::const_iterator it = keys.begin(); it != keys.end(); ++it) {
        std::string key = *it;
        long iKey = boost::lexical_cast<long>(*it);
        int mod = iKey % dbCluster_;
        std::map<int, vector<std::string> >::iterator findIt = tmpRes.find(mod);
        if(findIt != tmpRes.end()) {
            findIt->second.push_back(key);
        }else {
            std::vector<std::string> tmpVec;
            tmpVec.push_back(key);
            tmpRes.insert(make_pair(mod, tmpVec));
        }
    }

    std::vector<std::string> res;
    for(std::map<int, std::vector<std::string> >::const_iterator it = tmpRes.begin(); it != tmpRes.end(); ++it) {
        std::string dbPattern = LocalProducerManagerAdapter<TKey>::dbTable_ + "_" + boost::lexical_cast<std::string>(it->first);
        SyncMessageI sm(LocalProducerManagerAdapter<TKey>::dbSource_, LocalProducerManagerAdapter<TKey>::dbTable_, it->second, true, dbPattern, -1, LocalProducerManagerAdapter<TKey>::expire_, LocalProducerManagerAdapter<TKey>::isDel_);
        std::string smStr;
        sm.SerializeToString(&smStr); 
        res.push_back(smStr);
    }
    return res;
  }

private:
  int dbCluster_;
};

}}} // namespace com::renren::datasync

#endif
