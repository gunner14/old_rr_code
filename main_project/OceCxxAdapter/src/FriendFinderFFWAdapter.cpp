#include "FriendFinderFFWAdapter.h"

#include <vector>
#include <boost/foreach.hpp>

#include "socialgraph/socialgraphutil/algostuff.h"
#include "socialgraph/socialgraphutil/mcclient.h"
#include "item.pb.h"

namespace xce {
namespace socialgraph {

CommonFriendSeq FriendFinderFFWAdapter::GetCommonfriends(int userid, int limit) {
  vector<unsigned char> data;
  std::string key = Comb2("FFW", userid);
  MemcachedClient::GetInstance().GetByDistCache(key, data);
  ::xce::socialgraph::Items puk_friends;
  puk_friends.ParseFromArray(data.data(), data.size());
	
  ::google::protobuf::RepeatedPtrField< ::xce::socialgraph::Item >* seq
    = puk_friends.mutable_items();
  int size = seq->size();

	CommonFriendSeq comm_seq;
  for (int index = 0; index < size && index < limit; ++index) {
		::xce::socialgraph::Item* tmp_item = seq->Mutable(index);
		CommonFriend comm;
		comm.userId = tmp_item->id();			//userid
		comm.sharesCount = tmp_item->field();		//sharesCount
		BOOST_FOREACH (int share, tmp_item->fields()) {
			comm.shares.push_back(share);
		}
		comm_seq.push_back(comm);
  }
	return comm_seq;
}

void FriendFinderFFWAdapter::RemoveCommonfriends(int userid, int friendid) {
  vector<unsigned char> data;
  std::string key = Comb2("FFW", userid);
  MemcachedClient::GetInstance().GetByDistCache(key, data);
  ::xce::socialgraph::Items puk_friends;
  puk_friends.ParseFromArray(data.data(), data.size());
	
	xce::socialgraph::Items result;
  ::google::protobuf::RepeatedPtrField< ::xce::socialgraph::Item >* seq
    = puk_friends.mutable_items();
  int size = seq->size();

	for (int index = 0; index < size; ++index) {
  	::xce::socialgraph::Item* tmp_item = seq->Mutable(index);
  	if (friendid == tmp_item->id()) {
  	  continue;
  	}
  	::xce::socialgraph::Item* result_item = result.add_items();
  	result_item->Swap(tmp_item);
  }

  result.set_timestamp(time(NULL));
  vector<unsigned char> binary_data;
  int byte_size = result.ByteSize();
  binary_data.resize(byte_size);
  result.SerializeToArray(binary_data.data(), byte_size);

  std::string r_key = Comb2("FFW", userid);
  MemcachedClient::GetInstance().SetByDistCache(r_key, binary_data);
}

}}
