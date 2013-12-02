#ifndef  PUSH_FEED_TASK_H_
#define  PUSH_FEED_TASK_H_
#include "SocialFeedRecommendationManager.h"
#include "feed_recommend_builder.h"

const  static int TASK_LEVEL_FEED_PUSH = 8;

class  FeedPushTask: public MyUtil::Task {

public:
       FeedPushTask(int userid, int limit = 2, ::Ice::Int level = TASK_LEVEL_FEED_PUSH):
                    MyUtil::Task(TASK_LEVEL_FEED_PUSH), userid_(userid),limit_(limit) {}
      
       //call the bulider method to push the feed
       virtual  void  handle(){
           //get the result
           xce::socialgraph::RecommendItemSeq  feedseq = xce::socialgraph::FeedRecommendBuilder::instance().GetRecommendData(userid_, limit_);  
           //exchage the result to stand format and push to feed group
           PushFeed(feedseq);
       }

private:
       //push feed to user
       void  PushFeed(xce::socialgraph::RecommendItemSeq& feedseq);

private:
       int   userid_;
       int   limit_; 
};
#endif
