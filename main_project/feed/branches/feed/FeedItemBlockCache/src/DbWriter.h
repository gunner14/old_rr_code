#ifndef _DBWRITER_H_
#define _DBWRITER_H_

#include "QueryRunner.h"
#include "Singleton.h"
#include <utility>
#include <vector>

namespace xce {
namespace feed{
using namespace xce::feed;
using namespace std;
using namespace MyUtil;

class DbWriter: public MyUtil::Singleton<DbWriter> 
{
public:
    void addBlockType(Ice::Int userid, Ice::Int type);
    void removeBlockType(Ice::Int userid, Ice::Int type);

    void addBlockUser(Ice::Int source_userid, Ice::Int target_userid);
    void removeBlockUser(Ice::Int source_userid, Ice::Int target_userid);

    void addBlockFeed(Ice::Int userid, Ice::Long feedid);
    void addBlockFeedUseBatch(Ice::Int userid, Ice::Long feedid);
    void removeBlockFeed(Ice::Int userid, Ice::Long feedid);

    void updateMaxReadFeed(Ice::Int userid, Ice::Long feedid);
    
private:
    class BlockFeedSaver;
    typedef IceUtil::Handle<BlockFeedSaver> BlockFeedSaverPtr;

    static const int BlockFeedTableCount = 100;
    //用于执行block feed数据库写任务的线程以及互斥锁
    //这里并不一定会启动100个线程(最多)，由userid散的情况决定
    BlockFeedSaverPtr feedSavers_[BlockFeedTableCount];
    IceUtil::Mutex mutexs_[BlockFeedTableCount];

    string getBlockFeedTab(Ice::Int userid) {
        ostringstream tab;
        tab << "user_block_feed_" << (userid % BlockFeedTableCount);
        return tab.str();
    }

    //成批存储block feed
    class BlockFeedSaver : public ::IceUtil::Thread {
    public:
        BlockFeedSaver(string table, size_t batchSize, size_t batchTime):
            table_(table), batchSize_(batchSize), batchTime_(batchTime) {
            MCE_DEBUG("BlockFeedSaver batchSize_: " << batchSize_
                    << ", batchTime_: " << batchTime_);
            ;
        }
        void put(Ice::Int userid, Ice::Long feedid);

        virtual void run();

    private:
        typedef vector<pair<Ice::Int, Ice::Long> > BlockFeedSeq;
        BlockFeedSeq bfeeds_;
        
        IceUtil::Monitor<IceUtil::Mutex> mutex_;
        string table_;
        size_t batchSize_;
        size_t batchTime_;
    };

    BlockFeedSaverPtr getBlockFeedSaver(Ice::Int userid) {
        int index = userid % BlockFeedTableCount;
        IceUtil::Mutex::Lock lock(mutexs_[index]);
        if (!feedSavers_[index]) {
            feedSavers_[index] = new BlockFeedSaver(getBlockFeedTab(userid), 
                    1000, 1);
            if (feedSavers_[index]) {
                feedSavers_[index]->start().detach();
                MCE_DEBUG("BlockFeedSaver thread: " << index << " start...");
            }
        }
        return feedSavers_[index];
    }

};
}
}
#endif
