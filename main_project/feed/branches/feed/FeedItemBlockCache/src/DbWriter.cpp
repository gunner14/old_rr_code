#include "DbWriter.h"

using namespace xce::feed;
using namespace MyUtil;
using namespace com::xiaonei::xce;


//-----------------------------------------------------------------------------
// block type
void DbWriter::addBlockType(Ice::Int userid, Ice::Int type)
{
}

void DbWriter::removeBlockType(Ice::Int userid, Ice::Int type)
{
}

//-----------------------------------------------------------------------------
// block user
void DbWriter::addBlockUser(Ice::Int source_userid, Ice::Int target_userid)
{
    MCE_DEBUG("FeedItemBlockCache DbWriter::addBlockUser(" << source_userid << ", " << target_userid << ")" );
    Statement sql;

    sql << "INSERT INTO relation_feed_low (source, target) VALUES(" << source_userid << ", " << target_userid << ")";

    try {
        QueryRunner("feedwb", CDbWServer).store(sql);
    } catch (mysqlpp::Exception& e) {
        MCE_WARN("FeedItemBlockCache DbWriter::removeBlockUser, userid: " << source_userid << e.what());
    } catch (...) {
        MCE_WARN("FeedItemBlockCache DbWriter::removeBlockUser, userid: " << source_userid << " UNKNOWN EXCEPTION!");
    }
}

void DbWriter::removeBlockUser(Ice::Int source_userid, Ice::Int target_userid)
{
    MCE_DEBUG("FeedItemBlockCache DbWriter::removeBlockUser(" << source_userid << ", " << target_userid << ")" );
    Statement sql;

    sql << "DELETE FROM relation_feed_low"
       << " WHERE source= " << source_userid
       << " AND target=" << target_userid;
    try {
        QueryRunner("feedwb", CDbWServer).store(sql);
    } catch (mysqlpp::Exception& e) {
        MCE_WARN("FeedItemBlockCache DbWriter::removeBlockUser, userid: " << source_userid << e.what());
    } catch (...) {
        MCE_WARN("FeedItemBlockCache DbWriter::removeBlockUser, userid: " << source_userid << " UNKNOWN EXCEPTION!");
    }
}

//-----------------------------------------------------------------------------
// block feed
// add提供成批写策略，remove单个写入，因为add的并发量很大，remove几乎不会被使用
void DbWriter::addBlockFeed(Ice::Int userid, Ice::Long feedid)
{

    MCE_DEBUG("FeedItemBlockCache DbWriter: insert into "<< getBlockFeedTab(userid)<< ", userid: " << userid << ", feedid: " << feedid);
    Statement sql;
    sql << "INSERT INTO " << getBlockFeedTab(userid)
        << "(userid, feedid, time)"
        << "VALUES(" << userid << ", " << feedid << ", " << (int)time(NULL) << ")"
        << " ON DUPLICATE KEY UPDATE " << "time = " << (int)time(NULL);

    /*
    try {
        QueryRunner("user_block_feed", CDbWServer, getBlockFeedTab(userid)).store(sql);
    } catch (mysqlpp::Exception& e) {
        MCE_WARN("FeedItemBlockCache DbWriter::addBlockFeed, userid: " << userid << ", feedid: " << feedid << e.what());
    } catch (...) {
        MCE_WARN("FeedItemBlockCache DbWriter::addBlockFeed, userid: " << userid << ", feedid: " << feedid << " UNKNOWN EXCEPTION");
    }
    */
    QueryRunner("user_block_feed", CDbWServer, getBlockFeedTab(userid)).schedule(sql);
}

void DbWriter::addBlockFeedUseBatch(Ice::Int userid, Ice::Long feedid)
{
    BlockFeedSaverPtr saver = getBlockFeedSaver(userid);
    if (saver) {
        saver->put(userid, feedid);
        MCE_DEBUG("FeedItemBlockCache DbWriter::addBlockFeedUseBatch, userid: "
                << userid << ", feedid: " << feedid);
    }
    else {
        MCE_DEBUG("FeedItemBlockCache DbWriter::addBlockFeedUseBatch, userid: "
                << userid << ", feedid: " << feedid << ", null pointer exception");
    }
}

void DbWriter::removeBlockFeed(Ice::Int userid, Ice::Long feedid)
{
    MCE_DEBUG("FeedItemBlockCache DbWriter: delete from "<< getBlockFeedTab(userid) << ", userid: " << userid << ", feedid: " << feedid);
    Statement sql;
    sql << "DELETE FROM " << getBlockFeedTab(userid)
        << " WHERE userid=" << userid
        << " AND feedid=" << feedid;

    try {
        QueryRunner("user_block_feed", CDbWServer, getBlockFeedTab(userid)).store(sql);
    } catch (mysqlpp::Exception& e) {
        MCE_WARN("FeedItemBlockCache DbWriter::removeBlockFeed, userid: " << userid << ", feedid: " << feedid << e.what());
    } catch (...) {
        MCE_WARN("FeedItemBlockCache DbWriter::removeBlockFeed, userid: " << userid << ", feedid: " << feedid << " UNKNOWN EXCEPTION");
    }
}


//-----------------------------------------------------------------------------
// max read feed
void DbWriter::updateMaxReadFeed(Ice::Int userid, Ice::Long feedid)
{
    if (feedid <= 0) {
        MCE_DEBUG("FeedItemBlockCache DbWriter::updateMaxReadFeed, table: "<< getBlockFeedTab(userid) 
                << ", userid: " << userid << ", feedid: " << feedid << ", feedid<=0 error!");
        return;
    }
    
    MCE_DEBUG("FeedItemBlockCache DbWriter::updateMaxReadFeed, table: "<< getBlockFeedTab(userid)<< ", userid: " << userid << ", feedid: " << feedid);
    Statement sql1, sql2;

    //删除db中<feedid的记录
    sql1 << "DELETE FROM " << getBlockFeedTab(userid)
        //<< " WHERE userid=" << userid << " AND feedid<=0";
        << " WHERE userid= " << userid << " AND feedid<= " << feedid;
    sql2 << "INSERT INTO " << getBlockFeedTab(userid)
        << " (userid, feedid, time)"
        //<< " VALUES(" << userid << ", " << (feedid < 0? feedid : -feedid) << ", " << (int)time(NULL) << ");";
        << " VALUES(" << userid << ", " << (-feedid) << ", " << (int)time(NULL) << ");";

    try {
        QueryRunner("user_block_feed", CDbWServer, getBlockFeedTab(userid)).schedule(sql1);
        QueryRunner("user_block_feed", CDbWServer, getBlockFeedTab(userid)).schedule(sql2);
    } catch (mysqlpp::Exception& e) {
        MCE_WARN("FeedItemBlockCache DbWriter::updateMaxReadFeed, userid: " << userid << ", feedid: " << feedid << endl << e.what());
    } catch (...) {
        MCE_WARN("FeedItemBlockCache DbWriter::updateMaxReadFeed, userid: " << userid << ", feedid: " << feedid << " UNKNOWN EXCEPTION");
    }
}



//-----------------------------------------------------------------------------
// BlockFeedSaver

void DbWriter::BlockFeedSaver::put(Ice::Int userid, Ice::Long feedid)
{
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(mutex_);
    bfeeds_.push_back(make_pair(userid, feedid));
    MCE_DEBUG("BlockFeedSaver::put, table: " << table_ << ", size: " << bfeeds_.size());
    
    if (bfeeds_.size() >= batchSize_)
        mutex_.notify();
}

void DbWriter::BlockFeedSaver::run()
{
    while (true) {
        try {
            BlockFeedSeq tmp;
            {
                IceUtil::Monitor<IceUtil::Mutex>::Lock lock(mutex_);
                if (bfeeds_.size() < batchSize_)
                    mutex_.timedWait(IceUtil::Time::seconds(batchTime_)); 
                bfeeds_.swap(tmp);
            }

            if (!tmp.empty()) {
                MCE_DEBUG("BlockFeedSaver::run, table: " << table_<< ", save size: " << tmp.size());
                Statement sql;
                sql << "INSERT INTO " << table_ << " (userid, feedid, time) VALUES ";
                for (BlockFeedSeq::iterator it = tmp.begin(); it != tmp.end(); ++it) {
                    if (it != tmp.begin())
                        sql << ", ";

                    sql << "( " << (it->first) << ", " << (it->second) << ", " << (int)time(NULL) << " )";
                }

                sql << " ON DUPLICATE KEY UPDATE " << "time = " << (int)time(NULL);
                QueryRunner("user_block_feed", CDbWServer, table_).schedule(sql);
            }
        } catch(...) {
            MCE_WARN("BlockFeedSaver has exception!");
        }
    }
}



