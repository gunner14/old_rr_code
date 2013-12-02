#ifndef ZK_CONN_H_
#define ZK_CONN_H_

#include <zookeeper/zookeeper.h>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>

namespace xlog
{
enum KeeperState
{
    /** Unused, this state is never generated by the server */
    Unknown = -1,

    /** The client is in the disconnected state - it is not connected
     * to any server in the ensemble. */
    Disconnected = 0,

    /** Unused, this state is never generated by the server */
    NoSyncConnected = 1,

    /** The client is in the connected state - it is connected
     * to a server in the ensemble (one of the servers specified
     * in the host connection parameter during ZooKeeper client
     * creation). */
    SyncConnected = 3,

    /**
     * Auth failed state
     *
     */
    AuthFailed = 4,

    /** The serving cluster has expired this session. The ZooKeeper
     * client connection (the session) is no longer valid. You must
     * create a new client connection (instantiate a new ZooKeeper
     * instance) if you with to access the ensemble. */
    Expired = -112
};

/**
 * Enumeration of types of events that may occur on the ZooKeeper
 */
enum EventType
{
    None = -1, NodeCreated = 1, NodeDeleted = 2, NodeDataChanged = 3, NodeChildrenChanged = 4
};

struct ZKWatchedEvent
{
    ZKWatchedEvent(EventType et, KeeperState ks, const std::string& pt) :
            eventType(et), keeperState(ks), path(pt)
    {
    }
    EventType eventType;
    KeeperState keeperState;
    std::string path;
};

class ZKWatcher
{
public:
    virtual void process(ZKWatchedEvent event) = 0;
    static void watcher_callback(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx);
    virtual ~ZKWatcher()
    {
    }

};
typedef boost::shared_ptr<ZKWatcher> ZKWatcherPtr;

class ZKConnection
{
public:
    bool init(const std::string& address);
    bool create(const std::string& path, const std::vector<char>& data);
    void remove(const std::string& path, int version);
    void exists(const std::string& path, ZKWatcher* watcher);
    std::vector<char> getData(const std::string& path, ZKWatcher* watcher);
    bool setData(const std::string& path, const std::vector<char>& data, int version);
    std::vector<std::string> getChildren(const std::string& path, ZKWatcher* watcher);
    ~ZKConnection();
private:
    zhandle_t* _zk;

};
typedef boost::shared_ptr<ZKConnection> ZKConnectionPtr;

}

#endif /* ZK_CONN_H_ */