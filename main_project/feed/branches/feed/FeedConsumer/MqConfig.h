#ifndef MQ_CONFIG_H
#define MQ_CONFIG_H
#include <string>

namespace xce {
namespace feed {
    const std::string ZK_ADDR = "UserZooKeeper1.d.xiaonei.com:2181,UserZooKeeper2.d.xiaonei.com:2181,UserZooKeeper3.d.xiaonei.com:2181,UserZooKeeper4.d.xiaonei.com:2181,UserZooKeeper5.d.xiaonei.com:2181";
const std::string MQ_CLUSTER = "bj_feed_test";

const std::string DISPATCH_Q = "dispatch";
const std::string REPLY_Q = "reply";
const std::string READ_Q = "read";
const std::string REMOVE_Q = "remove";
const std::string OTHER_Q = "other";

//const std::string REMOVE_REPLY_Q = "remove_reply";
//enum ReadOP {READ_NEWS, READ_ALL_NEWS, READ_MINI, READ_ALL_MINI};
//enum RemoveOP {REMOVE_BY_ID, REMOVE_BY_SOURCE, REMOVE_BY_SOURCE_HS, REMOVE_BY_PARENT, REMOVE_FEED, REMOVE_REPLY, REMOVE_REPLY_BY_ID};
};
};
#endif

