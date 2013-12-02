#include<iostream>
#include "FeedGuideAdapter.h"
//#include "FeedMarkReplicaAdapter.h"
//#include "FeedItemBlockCacheReplicaAdapter.h"
//#include "FeedAssistantAdapter.h"
#include "FeedGuide.h"

using namespace std;
using namespace xce::feed; 

int main() {

  int uid, fid;
  bool mark;
  while(1) {
    cout << "input uid:" << endl;
    cin >> uid ;
    //FeedMarkReplicaAdapter::instance().MarkFeed(uid, -1, -1, -1, fid, mark);
    //FeedAssistantAdapter::instance().readAllNews(4444);
    FeedDataResPtr res = FeedGuideAdapter::instance().GetGuideFeedData(uid, 0, 20);
  
    cout << "go!!\n";
    cout << "Size:   " << res->data.size() << endl;
    //FeedAssistantAdapter::instance().markNews(uid, -1, -1, -1, fid, mark);
  }

}
