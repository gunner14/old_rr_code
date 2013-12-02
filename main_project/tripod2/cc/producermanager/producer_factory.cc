#include "tripod2/cc/producermanager/producer_factory.h"
#include "base/once.h"
#include "tripod2/cc/producermanager/example_producer.h"
#include "tripod2/cc/producermanager/simple_counter_producer.h"
#include "tripod2/cc/producermanager/single_table_producer.h"

#include "tripod2/cc/producermanager/user_score_vip_producer.h"
#include "tripod2/cc/producermanager/user_score_login_producer.h"
#include "tripod2/cc/producermanager/home_foot_print_producer.h"
#include "tripod2/cc/producermanager/user_view_count_producer.h"
#include "tripod2/cc/producermanager/menu_producer.h"
#include "tripod2/cc/producermanager/like_producer.h"
//#include "tripod2/cc/producermanager/friend_count_producer.h"
#include "tripod2/cc/producermanager/account_connect_producer.h"
#include "tripod2/cc/producermanager/ticket_producer.h"

#include "OceProto/proto/UserBase.pb.h"
#include "OceProto/proto/UserDesc.pb.h"
#include "OceProto/proto/user_score.pb.h"
#include "OceProto/proto/user_network.pb.h"
#include "OceProto/proto/UserConfig.pb.h"
#include "OceProto/proto/vip_info.pb.h"


namespace xce {
namespace tripod {

static ProducerFactory* instance_;
static void InitProducerFactory() {
  instance_ = new ProducerFactory();
  instance_->Add("test_biz", new ExampleProducer());

  //UserBase
  instance_->Add("user_passport", new SingleTableProducer<xce::tripoddata::userbase::PbUserPassport>());
  instance_->Add("user_name", new SingleTableProducer<xce::tripoddata::userbase::PbUserName>());
  instance_->Add("user_right", new SingleTableProducer<xce::tripoddata::userbase::PbUserRight>());
  instance_->Add("user_state", new SingleTableProducer<xce::tripoddata::userbase::PbUserState>());
  instance_->Add("user_logincount", new SimpleCounterProducer("user_logincount", "user_logincount", "id", "logincount"));
  instance_->Add("user_born", new SingleTableProducer<xce::tripoddata::userbase::PbUserBorn>());
  instance_->Add("user_stage", new SingleTableProducer<xce::tripoddata::userbase::PbUserStage>());
  instance_->Add("user_doing", new SingleTableProducer<xce::tripoddata::userbase::PbUserDoing>());
  instance_->Add("user_url", new SingleTableProducer<xce::tripoddata::userbase::PbUserUrl>());

  instance_->Add("user_config", new SingleTableProducer<xce::tripod::UserConfigData>());
  instance_->Add("user_score", new SingleTableProducer<xce::tripoddata::userscore::PbUserScore>());
  instance_->Add("user_score_login", new UserScoreLoginProducer());
  instance_->Add("user_score_award_count", new SingleTableProducer<xce::tripoddata::userscore::PbScoreAwardCount>());
  instance_->Add("user_score_sign", new SingleTableProducer<xce::tripoddata::userscore::PbScoreSign>());
  instance_->Add("user_score_vip", new UserScoreVipProducer());
//  instance_->Add("friend_count_cache", new FriendCountProducer());
  instance_->Add("account_connect", new AccountConnectProducer());

  //Ticket
  instance_->Add("TTicket", new xce::tripod::TTicketProducer());
  instance_->Add("PTicket", new xce::tripod::PTicketProducer());

  //UserNetwork
  instance_->Add("user_network_network", new SingleTableProducer<xce::tripoddata::usernetwork::PbNetworkHistory>());
  instance_->Add("user_network_high_school", new SingleTableProducer<xce::tripoddata::usernetwork::PbHighSchoolInfo>());
  instance_->Add("user_network_university", new SingleTableProducer<xce::tripoddata::usernetwork::PbUniversityInfo>());
  instance_->Add("user_network_workplace", new SingleTableProducer<xce::tripoddata::usernetwork::PbWorkplaceInfo>());
  instance_->Add("user_network_college", new SingleTableProducer<xce::tripoddata::usernetwork::PbCollegeInfo>());
  instance_->Add("user_network_elementary_school", new SingleTableProducer<xce::tripoddata::usernetwork::PbElementarySchoolInfo>());
  instance_->Add("user_network_junior_high_school", new SingleTableProducer<xce::tripoddata::usernetwork::PbJuniorHighSchoolInfo>());
  instance_->Add("user_network_region", new SingleTableProducer<xce::tripoddata::usernetwork::PbRegionInfo>());

  //UserDesc
  instance_->Add("user_time", new SingleTableProducer<xce::tripoddata::userdesc::PbUserTime>());
  instance_->Add("user_contact", new SingleTableProducer<xce::tripoddata::userdesc::PbUserContact>());

  //App
  instance_->Add("i_like_it", new LikeProducer()); 
  instance_->Add("recent_menu", new MenuProducer(xce::menu::RECENTLY)); 
  instance_->Add("favourite_menu", new MenuProducer(xce::menu::FAVORITE));
  instance_->Add("foot_print_head", new HomeFootprintProducer(xce::footprint::CACHE_SIZE));
  instance_->Add("foot_print_body", new HomeFootprintProducer(xce::footprint::CACHE_BUF_SIZE));
  instance_->Add("user_view_count", new UserViewCountProducer);
  instance_->Add("vip_icon_url", new SingleTableProducer<xce::tripoddata::vipinfo::PbVipIconUrl>());
  instance_->Add("vip_hat_url", new SingleTableProducer<xce::tripoddata::vipinfo::PbVipHatUrl>());
  instance_->Add("vip_member_info", new SingleTableProducer<xce::tripoddata::vipinfo::PbVipMemberInfo>());
}

ProducerFactory& ProducerFactory::GetInstance() {
  static XCE_DECLARE_ONCE(once_guard_); 
  xce::XceOnceInit(&once_guard_, &InitProducerFactory);
  return *instance_;
}


void ProducerFactory::Add(const std::string& biz, Producer* producer) {
  if (!producer) {
    return;
  }
  biz_producers_.insert(std::make_pair(biz, producer));
}

Producer* ProducerFactory::Get(const std::string& biz) {
  BusinessProducerMapType::const_iterator iter = biz_producers_.find(biz);
  if (iter == biz_producers_.end()) {
    return NULL;
  }
  return iter->second;
}

}} // xce::tripod



