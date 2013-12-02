#ifndef FEEDCONSUMERI_H_
#define FEEDCONSUMERI_H_

#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>
#include "Singleton.h"
#include "IceLogger.h"
#include "FeedConsumer.h"
#include "message_pipe/src/cpp/Consumer.h"
#include "Feed.pb.h"
#include "MqConfig.h"

using namespace MyUtil;
using namespace std;

namespace xce{
  namespace feed{

//    const std::string ZK_ADDR = "UserZooKeeper1.d.xiaonei.com:2181,UserZooKeeper2.d.xiaonei.com:2181,UserZooKeeper3.d.xiaonei.com:2181,UserZooKeeper4.d.xiaonei.com:2181,UserZooKeeper5.d.xiaonei.com:2181";
//const std::string MQ_CLUSTER = "bj_feed_test";
//const std::string DISPATCH_Q = "dispatch";
//const std::string REPLY_Q = "reply";
//const std::string READ_Q = "read";


template <class ProtoType> class FeedConsumerClient: public Ice::Object, public xce::messagepipe::Consumer {
  public:
    FeedConsumerClient(const std::string& zkAddress, const std::string cluster,
        const std::string& subject): xce::messagepipe::Consumer(zkAddress, cluster, subject) { }

    //virtual void onMessage(const std::vector<xce::messagepipe::MessageDataPtr>& receiveMessages) {}
  protected:


    std::vector<ProtoType> vec2proto(const std::vector<xce::messagepipe::MessageDataPtr>& receiveMessages) {

      std::vector<ProtoType> ret;
      for(std::vector<xce::messagepipe::MessageDataPtr>::const_iterator it = receiveMessages.begin(); it != receiveMessages.end(); it++) {
        std::string subject = (*it)->getSubject();
        std::vector<unsigned char> data = (*it)->getData();
        string str(data.begin(), data.end());
        std::istringstream iss(str);
        //MCE_INFO("sub:" << subject << " str:" << str);
        ProtoType proto_data;
        if(!proto_data.ParseFromIstream(&iss)) {
          MCE_WARN("FeedConsumerClient::vec2proto --> parse error! str:" << str);
        } else {
          ret.push_back(proto_data);
        }   
      }
      return ret;
    }
};

class DispatchConsumer: public FeedConsumerClient<FeedDispatchProto> {
  public:
    DispatchConsumer(const std::string& zkAddress, const std::string cluster,
        const std::string& subject): FeedConsumerClient<FeedDispatchProto>(zkAddress, cluster, subject) { }

  public:

    virtual void onMessage(const std::vector<xce::messagepipe::MessageDataPtr>& receiveMessages);
};

typedef IceUtil::Handle<DispatchConsumer> DispatchConsumerPtr;

class ReplyConsumer: public FeedConsumerClient<FeedReplyProto> {
  public:
    ReplyConsumer(const std::string& zkAddress, const std::string cluster,
        const std::string& subject): FeedConsumerClient<FeedReplyProto>(zkAddress, cluster, subject) { }

  public:

    virtual void onMessage(const std::vector<xce::messagepipe::MessageDataPtr>& receiveMessages);
};

typedef IceUtil::Handle<ReplyConsumer> ReplyConsumerPtr;

class ReadConsumer: public FeedConsumerClient<ReadProto> {
  public:
    ReadConsumer(const std::string& zkAddress, const std::string cluster,
        const std::string& subject): FeedConsumerClient<ReadProto>(zkAddress, cluster, subject) { }

  public:

    virtual void onMessage(const std::vector<xce::messagepipe::MessageDataPtr>& receiveMessages);
};

typedef IceUtil::Handle<ReadConsumer> ReadConsumerPtr;

class RemoveConsumer: public FeedConsumerClient<RemoveFeedProto> {
  public:
    RemoveConsumer(const std::string& zkAddress, const std::string cluster,
        const std::string& subject): FeedConsumerClient<RemoveFeedProto>(zkAddress, cluster, subject) { }

  public:

    virtual void onMessage(const std::vector<xce::messagepipe::MessageDataPtr>& receiveMessages);
};

typedef IceUtil::Handle<RemoveConsumer> RemoveConsumerPtr;


class OtherFeedConsumer: public FeedConsumerClient<OtherFeedProto> {
  public:
    OtherFeedConsumer(const std::string& zkAddress, const std::string cluster,
        const std::string& subject): FeedConsumerClient<OtherFeedProto>(zkAddress, cluster, subject) { }

  public:

    virtual void onMessage(const std::vector<xce::messagepipe::MessageDataPtr>& receiveMessages);
};

typedef IceUtil::Handle<OtherFeedConsumer> OtherFeedConsumerPtr;

class FeedConsumerI : public FeedConsumer,
  public MyUtil::Singleton<FeedConsumerI> {

    public:
      FeedConsumerI() {
        MCE_INFO("FeedConsumerI.");
        //new DispatchConsumer(ZK_ADDR ,MQ_CLUSTER, DISPATCH_Q);

        //new ReplyConsumer(ZK_ADDR ,MQ_CLUSTER, REPLY_Q);

        new ReadConsumer(ZK_ADDR ,MQ_CLUSTER, READ_Q);

        new RemoveConsumer(ZK_ADDR ,MQ_CLUSTER, REMOVE_Q);

        //new OtherFeedConsumer(ZK_ADDR ,MQ_CLUSTER, OTHER_Q);

      }


      int get(const Ice::Current& = Ice::Current());


  };

  }
}

#endif /* FEEDCONSUMERI_H_ */
