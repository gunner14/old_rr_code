#ifndef XCE_CLIENT_BUDDY_RELATION_CLIENT_H_
#define XCE_CLIENT_BUDDY_RELATION_CLIENT_H_

namespace xce {
namespace client {

class BuddyRelationClient {
public:
  static bool IsFriend(int host, int guest);

};

}} //namespace xce::client

#endif
