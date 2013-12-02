#include "socialgraph/recommendcontent/PageModelCreator/RcdPageCacheHelper/pagecachehelper.h"


using namespace xce::socialgraph;

void RcdPageCacheHelper::filterRcdPage(int user_id, std::set<int>& seq) {
  std::set<int>user_page_list;
  xce::socialgraph::RcdPageDbHelper::getUserPageFromDb(user_id, user_page_list);
  for (std::set<int>::iterator it = user_page_list.begin() ; it != user_page_list.end() ; it++) {
    if (seq.count(*it) > 0) {
      CLOG(INFO) << "user_id : " << user_id << " filter_id : " << *it;
      seq.erase(*it);
    }
  }  
};
bool RcdPageCacheHelper::setRcdPageIntoCache(int user_id, std::set<int>& seq) {
  filterRcdPage(user_id, seq);
  std::vector<int> user_friend;
  std::map<int, std::set<int> > friend_page;
  //getUserFriendPageList(user_id, user_friend, friend_page); //获取用户的每个好友添加Page的列表
  CLOG(INFO) << "finish get user friend page list " ;
  std::vector<int> mutual_friend;
  xce::socialgraph::Items items;
  int rcd_id;
  std::map<int, std::set<int> >::iterator iter;
  for (std::set<int>::iterator it = seq.begin() ; it != seq.end() ; it++) {
    rcd_id = *it;
    //for (int i = 0 ; i < user_friend.size() ; i++) {
    //  iter = friend_page.find(user_friend[i]);
    //  if (iter != friend_page.end()) {
    //    if (iter->second.count(rcd_id) > 0) {
    //      mutual_friend.push_back(user_friend[i]);
    //    }
    //  }
    //}
    CLOG(INFO) << "user_id : " << user_id << "rcd_id : " << rcd_id << " mutual_friend size : " << mutual_friend.size() 
      << "recommendSeq Size : " << seq.size();
    xce::socialgraph::Item* item = items.add_items();
    item->set_id(rcd_id);
    //item->set_field(mutual_friend.size());
    item->set_message("PAGECF");
    //int common_count = mutual_friend.size();
    //int field_loop = std::min(common_count, 10);
    //for (int i = 0 ; i < field_loop ; i++) {
    //  item->add_fields(mutual_friend[i]);
    //}
    items.set_timestamp(0); //离线将数据加载到Cache时，时间戳为0
  }
  const std::string tckey = xce::Comb("RCDPAGECF_", user_id);
  CLOG(INFO) << "tckey : " << user_id << " items.ByteSize() : " << items.ByteSize();
  int byte_size = items.ByteSize();
  char ch[byte_size];
  items.SerializeToArray(ch, byte_size);
  std::string data;
  data.reserve(byte_size);
  data.assign(ch, ch + byte_size);
  return xce::socialgraph::SocialGraphRcdPageCfTripodAdapter::instance().Set(tckey, data, 60*60*24*7);
};

void RcdPageCacheHelper::getUserFriendPageList(int user_id, std::vector<int>& user_friend, std::map<int, std::set<int> >& friend_page) {
  try {
    user_friend = xce::buddy::adapter::BuddyByIdCacheAdapter::instance().getFriendListAsc(user_id, 2000);
  } catch (Ice::Exception& e) {
    CLOG(ERROR) << "BuddyByIdCacheAdapter::instance().getFriendListAsc exception " << e.what() << " user_id : " << user_id;
  } catch (...) {
    CLOG(ERROR) << "BuddByIdCacheAdapter::instance().getFriendListAsc other exception user_id : " << user_id;
  }
  if (user_friend.empty()) {
    return;
  }
  std::set<int> page_list;
  for (size_t i = 0 ; i < user_friend.size() ; i++) {
    RcdPageDbHelper::getUserPageFromDb(user_friend[i], page_list);
    friend_page.insert(pair<int, std::set<int> >(user_friend[i], page_list));
    page_list.clear(); 
  }
};

bool RcdPageCacheHelper::getRcdPageFromCache(int user_id, std::vector<int>& seq) {
  bool if_sucessed = true;
  const std::string tckey = xce::Comb("RCDPAGECF_", user_id);
  std::string value;
  try {
    if_sucessed = xce::socialgraph::SocialGraphRcdPageCfTripodAdapter::instance().Get(tckey, &value);
  } catch(Ice::Exception& e) {
    CLOG(ERROR) << "SocialGraphRcdPageCfTripodAdapter::instance().Get() exception " << e.what() << "user_id : " << user_id;
  }
  if (!value.empty()) {
    std::vector<unsigned char> page_data(value.begin(), value.end());
    Items page_items;
    if (page_items.ParseFromArray(page_data.data(), page_data.size())) {
      BOOST_FOREACH(const Item& it, page_items.items()) {
        if (it.message() == "PAGECF") {            //new fof friends
          seq.push_back(it.id());
        }
      }
    }
    CLOG(INFO) << " the recommend seq size : " << seq.size();
  }
  return if_sucessed;
};

