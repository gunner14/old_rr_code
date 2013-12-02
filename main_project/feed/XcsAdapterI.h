/* ------------------------------------------------------------------
 * FileName:      XcsAdapterI.h
 * Desc:          
 * Author:        jin.zhang
 * Email:         jin.zhang@renren-inc.com
 * Create:        2012-09-07 16:46:59
 * LastChange:    2012-09-07 16:46:59
 * History:       
 *------------------------------------------------------------------ */

#ifndef __XCS_ADAPTERI_H__
#define __XCS_ADAPTERI_H__

#include <map>
#include <set>
#include <vector>
#include <string>
#include <boost/bimap.hpp>
#include <boost/foreach.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/lexical_cast.hpp>

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

#include "LogWrapper.h"

//#include "XCS/include/xcs_service_factory.h"
#include "XCS/cpp/xcs-core/include/xcs_service_factory.h"

namespace MyUtil {

using boost::bimap;
using namespace std;
using namespace xcs;

template<class T> class IceProxyConsumer;
template<class T> class SandBoxConsumer;
template<class T> class SandBoxConsumerManager;


// 提供使用 配置中心 并实现HA的Adapter
template<class T> class XcsAdapterI {
public:
    XcsAdapterI(const string& identity, 
            const string& name,
            const string& version, 
            int timeout = 300,
            const string& cfgFilePath = "")
            :consumer_(identity, name, version, timeout, cfgFilePath)
             ,manager_(identity, name, timeout, false, cfgFilePath)
    {
        consumer_.initialize();
    }

    map<int, std::vector<T> > getAvailableProxies() {
        return consumer_.getAvailableProxies();
    }

    map<int, std::vector<T> > getAvailableProxiesOneway() {
        return consumer_.getAvailableProxiesOneway();
    }

    T getProxy(long id, bool enable = false) {
        if (enable) {
            typename SandBoxConsumerManager<T>::SandBoxConsumerPtr consumer = manager_.getConsumer(id);
            if (consumer) {
                T prx = consumer->getProxy(id);
                if (prx) return prx;
            }
        }
        return consumer_.getProxy(id);
    }

    vector<T> getProxySeq(long id, bool enable = false) {
        if (enable) {
            typename SandBoxConsumerManager<T>::SandBoxConsumerPtr consumer = manager_.getConsumer(id);
            if (consumer) {
                vector<T> prxs = consumer->getProxySeq(id);
                if (prxs.size() > 0) return prxs;
            }
        }
        return consumer_.getProxySeq(id);
    }

    vector<T> getValidProxySeq(long id, bool enable = false) {
        if (enable) {
            typename SandBoxConsumerManager<T>::SandBoxConsumerPtr consumer = manager_.getConsumer(id);
            if (consumer) {
                vector<T> prxs = consumer->getValidProxySeq(id);
                if (prxs.size() > 0) return prxs;
            }
        }
        return consumer_.getValidProxySeq(id);
    }

    vector<T> getInvalidProxySeq(long id, bool enable = false) {
        if (enable) {
            typename SandBoxConsumerManager<T>::SandBoxConsumerPtr consumer = manager_.getConsumer(id);
            if (consumer) {
                vector<T> prxs = consumer->getInvalidProxySeq(id);
                if (prxs.size() > 0) return prxs;
            }
        }
        return consumer_.getInvalidProxySeq(id);
    }

    vector<T> getAllProxySeq(int id, bool enable = false) {
        if (enable) {
            typename SandBoxConsumerManager<T>::SandBoxConsumerPtr consumer = manager_.getConsumer(id);
            if (consumer) {
                vector<T> prxs = consumer->getAllProxySeq(id);
                if (prxs.size() > 0) return prxs;
            }
        }
        return consumer_.getAllProxySeq(id);
    }

    vector<T> getAllProxySeqOneway(int id, bool enable = false) {
        if (enable) {
            typename SandBoxConsumerManager<T>::SandBoxConsumerPtr consumer = manager_.getConsumer(id);
            if (consumer) {
                vector<T> prxs = consumer.getAllProxySeqOneway(id);
                if (prxs.size() > 0) return prxs;
            }
        }
        return consumer_.getAllProxySeqOneway(id);
    }

    T getProxyOneway(long id, bool enable = false) {
        if (enable) {
            typename SandBoxConsumerManager<T>::SandBoxConsumerPtr consumer = manager_.getConsumer(id);
            if (consumer) {
                T prx = consumer->getProxyOneway(id);
                if (prx) return prx;
            }
        }
        return consumer_.getProxyOneway(id);
    }

    T getProxyBatchOneway(long id, bool enable = false) {
        if (enable) {
            typename SandBoxConsumerManager<T>::SandBoxConsumerPtr consumer = manager_.getConsumer(id);
            if (consumer) {
                T prx = consumer->getProxyBatchOneway(id);
                if (prx) return prx;
            }
        }
        return consumer_.getProxyBatchOneway(id);
    }

    T getProxyDatagram(long id, bool enable = false) {
        if (enable) {
            typename SandBoxConsumerManager<T>::SandBoxConsumerPtr consumer = manager_.getConsumer(id);
            if (consumer) {
                T prx = consumer->getProxyDatagram(id);
                if (prx) return prx;
            }
        }
        return consumer_.getProxyDatagram(id);
    }

    void enableSandBox() {
        manager_.enable();
    }

    void disableSandBox() {
        manager_.disable();
    }

    int getCluster() {
        return consumer_.getCluster();
    }

private:
    // Attributes
    IceProxyConsumer<T> consumer_;
    SandBoxConsumerManager<T> manager_;
};  // class XcsAdapterI

class XcsConsumer {
public:
    virtual ~XcsConsumer() {}
    // 更新 version 以下所有节点信息
    virtual void update(const map<string, vector<string> >& endpoints) = 0;

    // 更新一个 shard 的节点信息
    virtual void update(const string& shard, const vector<string>& endpoints) = 0;
};


// 监听子节点增删变化与session expired状态, 当收到 zk server 通知后, 
// accessor 获取相应的变化信息, 交由 consumer 处理
// 所有监听都是持续的 
class XcsAdapterEventHandler : public ServiceEventListener {
public:
    XcsAdapterEventHandler(const ServiceRegistryAccessorPtr& accessor,
            XcsConsumer& consumer)
        :ServiceEventListener(accessor), consumer_(consumer) {
    }

    void HandleChildChange(const string& version, 
                            const string& stat) {
        if (stat == "") {
            // 如果是 verion 子节点有增删变化
            map<string, vector<string> > values;
            values = getAllChildren(true);
            consumer_.update(values);
        } else {
            // 如果 shard 子节点有增删变化
            vector<string> values;
            values = getChildren(stat, true);
            consumer_.update(stat, values);
        }
    }
    void HandleContentChange(const string& version,
                            const string& stat,
                            const string& endpoint) {
    }

    void HandleExpiredEvent() {
        consumer_.update(getAllChildren(true));
    }

    // 1. 对 session expired 进行监听
    // 2. 获取 version 以下所有子节点信息并监听子节点的增删变化
    void initialize() {
        if (accessor_) {
            accessor_->AddListenExpired(shared_from_this());
        }
        consumer_.update(getAllChildren(true));
    }
protected:
    map<string, vector<string> > getAllChildren(bool listen = false) {
        map<string, vector<string> > res;
        vector<string> stats = getChildren("", listen);
        BOOST_FOREACH(string& stat, stats) {
            if (stat == "") {
                continue;
            }
            vector<string> vec = getChildren(stat, listen);
            BOOST_AUTO(it, res.find(stat));
            if (it != res.end()) {
                it->second.insert(it->second.end(), vec.begin(), vec.end());
            } else {
                res.insert(make_pair(stat, vec));
            }
        }
        return res;
    }

    // 1. stat == "" 时取 version 子节点, 否则取 stat 的子节点
    // 2. listen == false 不监听, listen == true 监听
    inline vector<string> getChildren(const string& stat, bool listen = false) {
        if (!accessor_) {
            MCE_WARN("XcsAdapterEventhandler::getChildren. error: accessor is null.");
            return vector<string>();
        }

        vector<string> res;
        int retCode = 0;
        if (listen) {
            retCode = accessor_->ListAndListen(stat, shared_from_this(), false, &res);
        } else {
            ServiceEventListenerPtr nullPtr = ServiceEventListenerPtr((ServiceEventListener*)NULL);
            retCode = accessor_->ListAndListen(stat, nullPtr, false, &res);
        }
        if (retCode) {
            MCE_WARN("XcsAdapterEventhandler::getChildren. error: stat= " << stat << " retCode=" << retCode);
            return vector<string>();
        }
        return res;
    }

    inline boost::shared_ptr<XcsAdapterEventHandler> shared_from_this() {
        return boost::dynamic_pointer_cast<XcsAdapterEventHandler>(
                xcs::ServiceEventListener::shared_from_this());
    }
private:
    XcsConsumer& consumer_;
};
typedef boost::shared_ptr<XcsAdapterEventHandler> XcsAdapterEventHandlerPtr;

class IceProxyFactory : virtual public Ice::Object {
public:
    IceProxyFactory(const std::string& identity, const Ice::CommunicatorPtr ic) :
        identity_(identity), ic_(ic) {
        ;
    }
    ~IceProxyFactory() {
    }

    Ice::ObjectPrx create(const std::string& endpoint) {
        if(ic_) {
            std::string id = identity_ + ":" + endpoint;
            Ice::ObjectPrx prx = ic_->stringToProxy(id);
            return prx;
        }
        return NULL;
    }

private:
    std::string identity_;
    Ice::CommunicatorPtr ic_;
};
typedef IceUtil::Handle<IceProxyFactory> IceProxyFactoryPtr;

class ValidProxyMap : public IceUtil::RWRecMutex {
public:
    ValidProxyMap() {
        srand(unsigned(time(NULL)));
    }
    ~ValidProxyMap() {
        mvprx_.clear();
    }

    void insert(int shard, vector<Ice::ObjectPrx> vprx) {
        IceUtil::RWRecMutex::WLock lock(*this);
        mvprx_.insert(make_pair(shard, vprx));
    }
    void swap(map<int, vector<Ice::ObjectPrx> > mvprx) {
        IceUtil::RWRecMutex::WLock lock(*this);
        mvprx_.swap(mvprx);
    }
    void swap(int shard, vector<Ice::ObjectPrx> vprx) {
        IceUtil::RWRecMutex::WLock lock(*this);
        BOOST_AUTO(it, mvprx_.find(shard));
        if (it != mvprx_.end()) {
            it->second.swap(vprx);
        }
    }
    void copy(map<int, vector<Ice::ObjectPrx> >& mvprx) {
        IceUtil::RWRecMutex::RLock lock(*this);
        mvprx = mvprx_;
    }
    void clear() {
        IceUtil::RWRecMutex::RLock lock(*this);
        mvprx_.clear();
    }

    size_t size() {
        IceUtil::RWRecMutex::RLock lock(*this);
        return mvprx_.size();
    }
    size_t size(int shard) {
        IceUtil::RWRecMutex::RLock lock(*this);
        BOOST_AUTO(it, mvprx_.find(shard));
        if (it != mvprx_.end())
            return it->second.size();
        return 0;
    }

    Ice::ObjectPrx getOne(int shard) {
        IceUtil::RWRecMutex::RLock lock(*this);
        BOOST_AUTO(it, mvprx_.find(shard));
        if (it != mvprx_.end() && it->second.size() > 0) {
            return it->second.at(getRandom(it->second.size()));
        }
        return NULL;
    }
    vector<Ice::ObjectPrx> getList(int shard) {
        IceUtil::RWRecMutex::RLock lock(*this);
        vector<Ice::ObjectPrx> vecPrx;
        BOOST_AUTO(it, mvprx_.find(shard));
        if (it != mvprx_.end())
            vecPrx = it->second;
        return vecPrx;
    }

private:
    inline int getRandom(int total) {
        return total > 0 ? (rand() % total) : 0;
    }

    map<int, vector<Ice::ObjectPrx> > mvprx_;
};

// IceProxyConsumer
// 1. 从 zk server 获取服务的地址信息, 并创建代理
// 2. 实现 HA, 建立挂掉服务与冷备的映射关系(保证各个client最终一致)
template<class T> class IceProxyConsumer : public XcsConsumer {
public:
    IceProxyConsumer(const string& identity,
            const string& name, 
            const string& version = "1", 
            int timeout = 300,
            const string& cfgFilePath = ""):
        identity_(identity),
        serviceName_(name),
        version_(version),
        timeout_(timeout),
        configPath_(cfgFilePath)
    {}

    ~IceProxyConsumer() {
        prxUsing_.clear();
        proxies_.clear();
        backupProxies_.clear();
        if (ic_) ic_->destroy();
        if (accessor_) accessor_.reset();
        if (handler_) handler_.reset();
    }

    void initialize() {
        Ice::StringSeq args;
        Ice::InitializationData initData;
        ic_ = Ice::initialize(args, initData);
        factory_ = new IceProxyFactory(identity_, ic_);

        if (configPath_.size() <= 0) {
            configPath_ = "../etc/templates/zk_client_default.cfg";
        }
        PropertiesConfiguration config(configPath_);
        XcsServiceFactory accessorFactory;
        accessor_ = accessorFactory.CreateServiceRegistryAccessor(config, 500);
        if (accessor_) {
            accessor_->Connect(serviceName_, version_);
        }

        handler_ = XcsAdapterEventHandlerPtr(new XcsAdapterEventHandler(accessor_, *this));
        if (handler_) {
            handler_->initialize();
        }
    }
    
    // update 
    // 更新服务状态, 并创建服务代理
    // endpoints 服务的物理地址信息
    virtual void update(const map<string, vector<string> >& endpoints) {
        if (!factory_) {
            return;
        }

        map<int, vector<Ice::ObjectPrx> > prxtmp;
        map<int, vector<Ice::ObjectPrx> > baktmp;

        for (map<string, vector<string> >::const_iterator it = endpoints.begin(); 
                it != endpoints.end(); ++it) {
            int shard = 0;
            if (!typeParser(it->first, shard)) {
                continue;
            }
            vector<Ice::ObjectPrx> vecPrx;
            for (vector<string>::const_iterator itr = it->second.begin(); 
                    itr != it->second.end(); ++itr) {
                Ice::ObjectPrx prx = factory_->create(*itr);
                if (prx) {
                    vecPrx.push_back(prx);
                }
            }

            if (shard < 0) {
                baktmp.insert(make_pair(shard, vecPrx));
            } else {
                prxtmp.insert(make_pair(shard, vecPrx));
            }
        }

        {
            // update, 一起加锁, 保证一致
            IceUtil::RWRecMutex::WLock lock(used_mutex_);
            proxies_.swap(prxtmp);
            backupProxies_.swap(baktmp);
            setBackup();
        }
    }
    virtual void update(const string& shard, const vector<string>& endpoints) {
        if (!factory_) {
            return;
        }
        int intShard;
        if (!typeParser(shard, intShard)) {
            return;
        }
        
        vector<Ice::ObjectPrx> vecPrx;
        for (vector<string>::const_iterator it = endpoints.begin(); 
                it != endpoints.end(); ++it) {
            Ice::ObjectPrx prx = factory_->create(*it);
            if (prx) {
                vecPrx.push_back(prx);
            }
        }

        {
            // update, 一起加锁, 保证一致
            IceUtil::RWRecMutex::WLock lock(used_mutex_);
            if (intShard < 0) {
                backupProxies_.swap(intShard, vecPrx);
            } else {
                proxies_.swap(intShard, vecPrx);
            }
            setBackup();
        }
    }

    //-------------------------------------------------------------------------
    std::vector<Ice::ObjectPrx> getObjectPrxList(int shard) {
        return prxUsing_.getList(shard);
    }
    
    std::vector<Ice::ObjectPrx> getValidObjectPrxList(int shard){
        return proxies_.getList(shard);
    }

    std::vector<Ice::ObjectPrx> getInvalidObjectPrxList(int shard){
        int backupShard = 0;
        {
            IceUtil::RWRecMutex::RLock lock(used_mutex_);
            BOOST_AUTO(it, used_.left.find(shard));
            if (it != used_.left.end()) {
                backupShard = it->second;
            }
        }
        if (backupShard < 0 ) {
            return backupProxies_.getList(backupShard);
        }
        return std::vector<Ice::ObjectPrx>();
    }

    Ice::ObjectPrx getObjectPrx(int shard) {
        return prxUsing_.getOne(shard);
    }
    //-------------------------------------------------------------------------


    T getProxy(long id) { 
        Ice::ObjectPrx proxy = getObjectPrx(getShard(id));
        if (proxy) {
            return T::uncheckedCast(proxy->ice_timeout(timeout_)->ice_twoway());
        }
        return NULL;
    }

    vector<T> getProxySeq(long id) {
        vector<T> res;
        vector<Ice::ObjectPrx> tmp = getObjectPrxList(getShard(id));
        for (vector<Ice::ObjectPrx>::iterator it = tmp.begin(); 
                it != tmp.end(); ++it) {
            res.push_back(T::uncheckedCast((*it)->ice_timeout(timeout_)->ice_twoway()));
        }
        return res;
    }

    vector<T> getValidProxySeq(long id) {
        vector<T> res;
        vector<Ice::ObjectPrx> tmp = getValidObjectPrxList(getShard(id));
        for (vector<Ice::ObjectPrx>::iterator it = tmp.begin(); 
                it != tmp.end(); ++it) {
            res.push_back(T::uncheckedCast((*it)->ice_timeout(timeout_)->ice_twoway()));
        }
        return res;
    }

    vector<T> getInvalidProxySeq(long id) {
        vector<T> res;
        vector<Ice::ObjectPrx> tmp = getInvalidObjectPrxList(getShard(id));
        for (vector<Ice::ObjectPrx>::iterator it = tmp.begin(); 
                it != tmp.end(); ++it) {
            res.push_back(T::uncheckedCast((*it)->ice_timeout(timeout_)->ice_twoway()));
        }
        return res;
    }
    
    vector<T> getAllProxySeq(long id) {
        vector<T> vecPrx = getValidProxySeq(id);
        vector<T> vecPrxBackup = getInvalidProxySeq(id);
        vecPrx.insert(vecPrx.end(), vecPrxBackup.begin(), vecPrxBackup.end());
        return vecPrx;
    }

    vector<T> getValidProxySeqOneway(long id) {
        vector<T> res;
        vector<Ice::ObjectPrx> tmp = getValidObjectPrxList(getShard(id));
        for (vector<Ice::ObjectPrx>::iterator it = tmp.begin(); 
                it != tmp.end(); ++it) {
            res.push_back(T::uncheckedCast((*it)->ice_timeout(timeout_)->ice_oneway()));
        }
        return res;
    }

    vector<T> getInvalidProxySeqOneway(long id) {
        vector<T> res;
        vector<Ice::ObjectPrx> tmp = getInvalidObjectPrxList(getShard(id));
        for (vector<Ice::ObjectPrx>::iterator it = tmp.begin(); 
                it != tmp.end(); ++it) {
            res.push_back(T::uncheckedCast((*it)->ice_timeout(timeout_)->ice_oneway()));
        }
        return res;
    }

    vector<T> getAllProxySeqOneway(long id) {
        vector<T> vecPrx = getValidProxySeqOneway(id);
        vector<T> vecPrxBackup = getInvalidProxySeqOneway(id);
        vecPrx.insert(vecPrx.end(), vecPrxBackup.begin(), vecPrxBackup.end());
        return vecPrx;
    }

    T getProxyOneway(long id) {
        Ice::ObjectPrx proxy = getObjectPrx(getShard(id));
        if (proxy) {
            return T::uncheckedCast(proxy->ice_timeout(timeout_)->ice_oneway());
        }
        return NULL;
    }

    T getProxyBatchOneway(long id) { 
        Ice::ObjectPrx proxy = getObjectPrx(getShard(id));
        if (proxy) {
            return T::uncheckedCast(proxy->ice_timeout(timeout_)->ice_batchOneway());
        }
        return NULL;
    }

    T getProxyDatagram(long id) {
        Ice::ObjectPrx proxy = getObjectPrx(getShard(id));
        if (proxy) {
            return T::uncheckedCast(proxy->ice_timeout(timeout_)->ice_datagram());
        }
        return NULL;
    }


    //-------------------------------------------------------------------------
    std::map<int, std::vector<T> > getAvailableProxies(){
        map<int, vector<T> > resPrx;
        map<int, vector<Ice::ObjectPrx> > prxtmp;
        prxUsing_.copy(prxtmp);
        for (map<int, vector<Ice::ObjectPrx> >::iterator it = prxtmp.begin();
                it != prxtmp.end(); ++it) {
            vector<T> vec;
            for (vector<Ice::ObjectPrx>::iterator it1 = it->second.begin();
                    it1 != it->second.end(); ++it1) {
                vec.push_back(T::uncheckedCast((*it1)->ice_timeout(timeout_)));
            }
            if (!vec.empty()) {
                resPrx.insert(make_pair(it->first, vec));
            }
        }
        return resPrx;
    }

    std::map<int, std::vector<T> > getAvailableProxiesOneway() {
        map<int, vector<T> > resPrx;
        map<int, vector<Ice::ObjectPrx> > prxtmp;
        prxUsing_.copy(prxtmp);
        for (map<int, vector<Ice::ObjectPrx> >::iterator it = prxtmp.begin();
                it != prxtmp.end(); ++it) {
            vector<T> vec;
            for (vector<Ice::ObjectPrx>::iterator it1 = it->second.begin();
                    it1 != it->second.end(); ++it1) {
                vec.push_back(T::uncheckedCast((*it1)->ice_timeout(timeout_)->ice_oneway()));
            }
            if (!vec.empty()) {
                resPrx.insert(make_pair(it->first, vec));
            }
        }
        return resPrx;
    }
    //-------------------------------------------------------------------------

    int getCluster() {
        return proxies_.size();
    }

protected:
    inline int getShard(long shard) {
        int size = proxies_.size();
        return size > 0 ? (shard % size) : 0;
    }

    template<class From, class To>
    inline bool typeParser(const From& fro, To& to) {
        try {
            to = boost::lexical_cast<To>(fro);
            return true;
        } catch (...) {
            return false;
        }
    }

private:
    // setBackup 结合 update 实现 HA
    // 升序选择冷备, 重建一一映射
    void setBackup() {
        map<int, vector<Ice::ObjectPrx> > prx;
        map<int, vector<Ice::ObjectPrx> > bakprx;

        proxies_.copy(prx);
        backupProxies_.copy(bakprx);

        bimap<int, int> use;
        map<int, vector<Ice::ObjectPrx> >::iterator pit = prx.begin();
        map<int, vector<Ice::ObjectPrx> >::iterator bit = bakprx.begin();

        for (; pit != prx.end() && bit != bakprx.end(); ++pit) {
            // 服务正常, 无需冷备
            if (pit->second.size() > 0) continue;

            for (; bit != bakprx.end(); ++bit) {
                // 该冷备异常, 选择其它冷备
                if (bit->second.size() <= 0) continue;

                // 选择该冷备, 并建立映射关系
                BOOST_AUTO(it, use.right.find(bit->first));
                if (it == use.right.end()) {
                    use.insert(bimap<int, int>::value_type(pit->first, bit->first));
                    pit->second.swap(bit->second);
                    break;
                }
            }
        }

        // 写锁在函数外部加
        prxUsing_.swap(prx);
        used_.swap(use);
    }

    //-------------------------------------------------------------------------
    // Attributes
    string identity_;
    string serviceName_;
    string version_;
    int timeout_;
    string configPath_;

    Ice::CommunicatorPtr ic_;
    IceProxyFactoryPtr factory_;            // 用于创建 proxy
    ServiceRegistryAccessorPtr accessor_;   // 读zk, 获取服务的地址信息
    XcsAdapterEventHandlerPtr handler_;     // 监听znode并处理 zk server 通知

    ValidProxyMap prxUsing_;        // 正在使用的代理
    ValidProxyMap proxies_;         // 正常服务的代理
    ValidProxyMap backupProxies_;   // 冷备服务的代理
    bimap<int, int> used_;          // 挂掉的服务 <--映射--> 冷备服务
    IceUtil::RWRecMutex used_mutex_;
}; // class IceProxyCosumer


//-----------------------------------------------------------------------------
class WhiteListStrategy : virtual public ::Ice::Object {
public:
    virtual ~WhiteListStrategy() {};
    virtual void updateWhiteList(const std::map<std::string, std::string>& whitelist) = 0;
    virtual bool inWhiteList(long id) = 0;
};
typedef ::IceUtil::Handle<WhiteListStrategy> WhiteListStrategyPtr;

class DefaultWhiteListStrategy : public WhiteListStrategy {
public:
    ~DefaultWhiteListStrategy() {};

    virtual void updateWhiteList(const std::map<std::string, std::string>& whitelist) {
    }
    virtual bool inWhiteList(long id) {
        // IceUtil::RWRecMutex::RLock lock(*this);
        return ids_.find(id) != ids_.end() ||
            mod_ > 0 && mods_.find(id % mod_) != mods_.end();
    }

private:
    std::set<long> ids_;
    std::set<int> mods_;
    int mod_;
};
typedef ::IceUtil::Handle<DefaultWhiteListStrategy> DefaultWhiteListStrategyPtr;

class ExtendWhiteListStrategyFactory : virtual public ::Ice::Object{
public:
    virtual ~ExtendWhiteListStrategyFactory() {};
    virtual WhiteListStrategyPtr create() = 0;
};
typedef ::IceUtil::Handle<ExtendWhiteListStrategyFactory> ExtendWhiteListStrategyFactoryPtr;


template <class T> class SandBoxConsumer : public IceProxyConsumer<T> {
public:
    SandBoxConsumer(const string& identity,
            const string& name, 
            const string& version = "-1", 
            int timeout = 300,
            const string& cfgFilePath = ""):
        IceProxyConsumer<T>(identity, name, version, timeout, cfgFilePath),
        versionPath_("/"+name+"/"+version)
    {}

    ~SandBoxConsumer() {
        versionDataListener_.reset();
    }

    void initialize(WhiteListStrategyPtr extended = NULL) {
        IceProxyConsumer<T>::initialize();
        ids_.clear();
        mods_.clear();
        mod_ = 0;
        
        default_strategy_ = new DefaultWhiteListStrategy;
        extended_strategy_ = extended;

        xcs::ZookeeperClient& zkclient = 
            xcs::ZookeeperClient::Instance(xcs::PropertiesConfiguration(""), 5000, "client");
        std::string whitelist = "";
        zkclient.GetValue(versionPath_.c_str(), &whitelist);
        updateWhiteList(whitelist);
        versionDataListener_ = VersionDataListenerPtr(new VersionDataListener(*this));
        zkclient.AddNodeListener(versionPath_.c_str(), versionDataListener_->shared_from_this());
    }

    void updateWhiteList(const string& whitelist) {
        std::map<string, string> m;
        std::vector<string> items = split(whitelist, ';');
        for (size_t i = 0; i < items.size(); ++i) {
            std::string::size_type p = items[i].find(':');
            if (p == std::string::npos)
                continue;
            m.insert(make_pair(items[i].substr(0, p), items[i].substr(p+1)));
        }

        std::set<long> ids;
        std::set<int> mods;
        int mod = 0;

        std::map<string, string>::iterator it;
        if ((it=m.find("id")) != m.end()) {
            std::vector<string> idseq = split(it->second, ',');
            for (size_t i = 0; i < idseq.size(); ++i) {
                long id;
                if (IceProxyConsumer<T>::typeParser(idseq[i], id)) {
                    ids.insert(id);
                }
            }
        }

        if ((it = m.find("mod")) != m.end()) {
            std::vector<string> strs = split(it->second, '.');
            if (strs.size() == 2 && IceProxyConsumer<T>::typeParser(strs[0], mod) && mod > 0) {
                std::vector<string> modseq = split(strs[1], ',');
                for (size_t i = 0; i < modseq.size(); ++i) {
                    int mod;
                    if (IceProxyConsumer<T>::typeParser(modseq[i], mod)) {
                        mods.insert(mod);
                    }
                }
            } else {
                mod = 0;
            }
        }


        ids_.swap(ids);
        mods_.swap(mods);
        mod_ = mod;
        dump();

        default_strategy_->updateWhiteList(m);
        if (extended_strategy_)
            extended_strategy_->updateWhiteList(m);
    }
    inline bool inWhiteList(long id) {
        return ids_.find(id) != ids_.end() ||
            mod_ > 0 && mods_.find(id % mod_) != mods_.end() || 
            (extended_strategy_ && extended_strategy_->inWhiteList(id));
    }

protected:
    inline std::vector<string> split(const string& source, const char del) {
        std::vector<string> result;
        std::string::size_type start = 0;
        std::string::size_type pos = 0;
        while ((pos=source.find(del, start))!=std::string::npos){
            string item = source.substr(start, pos-start);
            start = pos + 1;
            result.push_back(item);
        }
        if (start < source.size()) {
            result.push_back(source.substr(start));
        }
        return result;
    }

    void dump() {
        std::ostringstream oss;
        oss << "id: ";
        BOOST_FOREACH(long id, ids_) {
            oss << id << " ";
        }
        oss << "; ";
        oss << "mod " << mod_ << ": ";
        BOOST_FOREACH(int mod, mods_) {
            oss << mod << " ";
        }
        MCE_DEBUG(oss.str());
        std::cout << oss.str() << endl;
    }

    class VersionDataListener : public xcs::ZkEventListener {
    public:
        VersionDataListener(SandBoxConsumer<T>& consumer): consumer_(consumer) {}
        virtual ~VersionDataListener() {}
        virtual void HandleNodeEvent(const char* path, const std::string& value) {
            consumer_.updateWhiteList(value);
        }
        virtual void HandleChildEvent(const char* path, 
                const std::vector<std::string>& value) {}
        virtual void HandleExpiredEvent() {}
    private:
        SandBoxConsumer<T>& consumer_;
    };
    typedef boost::shared_ptr<VersionDataListener> VersionDataListenerPtr;
private:
    std::set<long> ids_;
    std::set<int> mods_;
    int mod_;

    std::string versionPath_;
    VersionDataListenerPtr versionDataListener_;
    DefaultWhiteListStrategyPtr default_strategy_;
    WhiteListStrategyPtr        extended_strategy_;
};


template <class T> class SandBoxConsumerManager {

public:
    typedef boost::shared_ptr<SandBoxConsumer<T> > SandBoxConsumerPtr;
    // typedef SandBoxConsumer<T>* SandBoxConsumerPtr;
protected:
    typedef std::map<std::string, SandBoxConsumerPtr> VersionConsumerMap;

    class ServiceChildListener : public xcs::ZkEventListener {
    public:
        ServiceChildListener(SandBoxConsumerManager<T>& manager) : manager_(manager){}
        virtual ~ServiceChildListener(){}
        virtual void HandleNodeEvent(const char* path, const std::string& value) {}
        virtual void HandleChildEvent(const char* path, 
                const std::vector<std::string>& value) {
            manager_.updateVersions(value);
        }
        virtual void HandleExpiredEvent() {}
    private:
        SandBoxConsumerManager<T>& manager_;
    };
    typedef boost::shared_ptr<ServiceChildListener> ServiceChildListenerPtr;

public:
    SandBoxConsumerManager(const std::string identity, 
            const std::string name,
            const int timeout = 300,
            const bool enable = false,
            const std::string cfgFile = "") :
        identity_(identity), name_(name), timeout_(timeout),
        enable_(enable), cfgFile_(cfgFile), inited_(false)
    {
    }
    ~SandBoxConsumerManager() {
        consumers_.clear();
        listener_.reset();
    }

    void initialize(ExtendWhiteListStrategyFactoryPtr factory = NULL) {
        if (inited_) return;

        // for extend whitelist strategy
        factory_ = factory;

        xcs::ZookeeperClient& zkclient = 
            xcs::ZookeeperClient::Instance(xcs::PropertiesConfiguration(cfgFile_), 5000, "client");

        std::string path = "/" + name_;
        std::vector<std::string> versions;
        zkclient.GetChildren(path.c_str(), &versions);
        // update versions
        updateVersions(versions);
        listener_ = ServiceChildListenerPtr(new ServiceChildListener(*this));
        zkclient.AddChildListener(("/"+name_).c_str(), listener_->shared_from_this());

        inited_ = true;
    }

    // travers, check and return a non NULL SandBoxConsumerPtr if id is in white list
    SandBoxConsumerPtr getConsumer(long id) {
        if (!enable_ || !inited_) return SandBoxConsumerPtr((SandBoxConsumer<T>*)NULL);
        BOOST_FOREACH(typename VersionConsumerMap::value_type& v, consumers_) {
            if (v.second && v.second->inWhiteList(id))
                return v.second;
        }
        return SandBoxConsumerPtr((SandBoxConsumer<T>*)NULL);
    }

    // update consumers_
    void updateVersions(const std::vector<std::string>& versions) {
        VersionConsumerMap consumers;
        VersionConsumerMap oldConsumers(consumers_);

        BOOST_FOREACH(const std::string& v, versions) {
            // ignore the nonnegative and empty versions
            if (v.size() <= 0 || (v.size() > 0 && v[0]!='-')) continue;

            if (consumers.find(v) == consumers.end()) {
                // reuse the old consumer
                BOOST_AUTO(it, oldConsumers.find(v));
                if (it != oldConsumers.end()) {
                    consumers.insert(make_pair(v, it->second));
                    continue;
                }

                // create new consumer
                SandBoxConsumerPtr consumer = 
                    SandBoxConsumerPtr(new SandBoxConsumer<T>(identity_, name_, v, timeout_, cfgFile_));
                if (factory_) {
                    consumer->initialize(factory_->create());
                } else {
                    consumer->initialize();
                }
                consumers.insert(make_pair(v, consumer));
            }
        }
        // update
        consumers_.swap(consumers);
    }

    void enable(ExtendWhiteListStrategyFactoryPtr factory = NULL) {
        if (!inited_) {
            initialize(factory);
        }
        enable_ = true;
    }
    void disable() {
        enable_ = false;
        if (inited_) {
            // delete consumers_
            consumers_.clear();
            listener_.reset();
            factory_ = NULL;
            inited_ = false;
        }
    }
private:
    std::string identity_;
    std::string name_;
    int         timeout_;
    bool        enable_;
    std::string cfgFile_;
    bool        inited_;

    VersionConsumerMap consumers_;;
    ServiceChildListenerPtr listener_;
    ExtendWhiteListStrategyFactoryPtr factory_;
};


}   // namespace MyUtil

#endif  // __XCS_ADAPTERI_H__

