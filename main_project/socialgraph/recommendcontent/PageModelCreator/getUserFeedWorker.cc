
#include "OceCxxAdapter/src/FeedInterfaceReplicaAdapter.h"
#include "Singleton.h"
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char** argv) {
  int limit = 20;
  vector<int> userid;
  userid.push_back(250137860);
  vector<int> types;
  types.push_back(502);
  types.push_back(701);
  xce::feed::FeedItemSeq seq = xce::feed::FeedInterfaceReplicaAdapter::instance().GetOwnFeeds(userid, 0, 100, types);
  for(xce::feed::FeedItemSeq::const_iterator it = seq.begin() ; it  != seq.end() ;) {
    xce::feed::FeedItem item = (*it++);
    std::cout << item.actor << std::endl;
  } 
  std::cout << "sucess " << std::endl;
}
