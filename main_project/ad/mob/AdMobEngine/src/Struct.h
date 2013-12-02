/*
 * Struct.h
 *
 *  Created on: 2012-04-05
 *      Author: yongjun.yin
 * Description: 定义数据对象
 */

#ifndef __AD_MOB_STRUCT_H__
#define __AD_MOB_STRUCT_H__

#include <Date.h>
#include <set>
#include <string>
#include <IceUtil/Handle.h>
#include <IceUtil/Shared.h>

#include "Util.h"
#include "AdMob.h"
#include "FeedMemcProxy/client/user_profile_client.h"

using namespace std;
using namespace MyUtil;

namespace xce {
namespace ad {

class AbstractStruct : public IceUtil::Shared {
public:
  virtual string ToString() const  {
		return "AbstractStruct : no info";
  }
};
typedef IceUtil::Handle<AbstractStruct> AbstractStructPtr;

class AdMember : public AbstractStruct {
public:
  bool HaveMoney();
  string ToString() const ;
private:
  SETGET_DATA( Ice::Long, member_id )
  SETGET_DATA( int, have_money )
  SETGET_DATA( int, category )
  SETGET_DATA( int, industry )
};
typedef IceUtil::Handle<AdMember> AdMemberPtr;

class AdCampaign : public AbstractStruct {
public:
  string ToString() const ;
private:
  SETGET_DATA( long, campaign_id )
  SETGET_DATA( long, member_id )
  SETGET_DATA( time_t, begin_time )
  SETGET_DATA( time_t, end_time )
};
typedef IceUtil::Handle<AdCampaign> AdCampaignPtr;

#define ALL 32767
class AdBidUnit : public AbstractStruct {
public:
  string ToString() const ;
private:
  SETGET_DATA( long, group_id )
  SETGET_DATA( long, bid_unit_id )
  SETGET_DATA( long, member_id )
  SETGET_DATA( short, gender )
  SETGET_DATA( short, stage )
  SETGET_DATA( string, grade )
  SETGET_DATA( string, age )
  SETGET_DATA( string, school )
  SETGET_DATA( string, area )
  SETGET_DATA( string, platform )
  SETGET_DATA( string, network )
  SETGET_DATA( string, brand )
  SETGET_DATA( string, resolution )
  SETGET_DATA( long, lbs_flag )
  SETGET_DATA( string, lbs_pos )
  SETGET_DATA( double, lbs_radius )
  SETGET_DATA( int, member_category )
};
typedef IceUtil::Handle<AdBidUnit> AdBidUnitPtr;

class Rectangle : public AbstractStruct {
public:
  static const double per_long = 0.01130;   //1公里的经度折算
  static const double per_lati = 0.00897;   //1公里的纬度折算
  
  explicit Rectangle(double x_min, double y_min, double x_max, double y_max):
	  x_min_(x_min), y_min_(y_min), x_max_(x_max), y_max_(y_max) {
  }
  
	explicit Rectangle(double longitude, double latitude, double radius) {
		x_min_ = longitude - per_long * radius;
		x_max_ = longitude + per_long * radius;
		y_min_ = latitude - per_lati * radius;
		y_max_ = latitude + per_lati * radius;
  }
  bool operator<( const Rectangle& r ) const;
  bool operator==( const Rectangle& r ) const;
  bool IsInRec( const Rectangle& r ) const;
  string ToString() const ;
  friend ostream& operator<< (ostream&, const Rectangle &rec);
private:
  SETGET_DATA ( double, x_min )
  SETGET_DATA ( double, y_min )
  SETGET_DATA ( double, x_max )
  SETGET_DATA ( double, y_max )
};

typedef IceUtil::Handle<Rectangle> RectanglePtr;
const Rectangle LBS_ALL(1,1,1,1);

class AdTargetUnit : public AbstractStruct {
public:
  explicit AdTargetUnit(const AdBidUnit& bid_unit);
  bool Match(const UserProfile& userProfile, const AdMobReqPara& request);
private:
  enum MatchScope { Area, SchoolArea };
  static bool SchoolAreaMatch(uint64_t area_key, const vector<uint64_t>& area);
  static bool AreaMatch(uint64_t area_key, const vector<uint64_t>& area);
  static bool LbsMatch(Rectangle rec_key, const vector<Rectangle>& rec_list);
  short gender_;
  short stage_;
  vector<short> grade_;
  vector<short> age_;
  vector<uint64_t> school_;
  vector<uint64_t> area_;
  vector<uint64_t> school_area_;
  vector<uint64_t> platform_;
  vector<short> network_;
  vector<uint64_t> brand_;
  vector<short> resolution_;
  vector<Rectangle> lbs_;
};
typedef IceUtil::Handle<AdTargetUnit> AdTargetUnitPtr;

class AdCreative : public AbstractStruct {
public:
  explicit AdCreative(Ice::Long creative_id, int widget_id, string& widget):
					creative_id_(creative_id), widget_id_(widget_id), widget_(widget) {
    ParseWidget(widget);
  }
  bool operator<( const AdCreative& r ) const;
  string GetValue(const string& key);
private:
  map<string, string> widgetMap;
  bool ParseWidget(const string& json_str);
  SETGET_DATA( Ice::Long, creative_id )
  SETGET_DATA( Ice::Long, group_id )
  SETGET_DATA( int, widget_id )
  SETGET_DATA( int, update_time )
  SETGET_DATA( string, widget )
};
typedef IceUtil::Handle<AdCreative> AdCreativePtr;
typedef map<Ice::Long, AdCreative> AdCreativeMap;

class AdGroup : public AbstractStruct {
public:
  void add_adzone_id(Ice::Long adzone_id);
  set<Ice::Long> adzone_ids();
  bool Available();
  string ToString() const ;
  bool operator<( const AdGroup& r ) const;
  AdTargetUnitPtr target_unit();
  void set_target_unit(AdTargetUnitPtr target_unit);
  //样式相关
  void add_creative(AdCreativePtr creative);
  bool have_creative();
	Ice::Long creative_id();
  AdCreativePtr creative();
  AdCreativePtr creative(Ice::Long creative_id);
  //竞价相关
  double cost();
  //double weight();
private:
  SETGET_DATA( long, group_id )
  SETGET_DATA( long, bid_unit_id )
  SETGET_DATA( long, member_id )
  SETGET_DATA( long, campaign_id )
  SETGET_DATA( int, display_type )
  SETGET_DATA( int, trans_type )
  SETGET_DATA( int, max_price )
  SETGET_DATA( bool, campaign_status )
  SETGET_DATA( bool, member_table_status )
  SETGET_DATA( int, member_category )
  SETGET_DATA( int, member_industry )
  SETGET_DATA( bool, point_valid )
  SETGET_DATA( int, rotation_number )
private:
  AdTargetUnitPtr target_unit_;
  set<Ice::Long> adzone_id_set_;
  vector<AdCreativePtr> creative_seq_;
};
typedef IceUtil::Handle<AdGroup> AdGroupPtr;

class AdZone : public AbstractStruct {
public:
  bool AddGroupToRotateList(int rotate_index, const AdGroupPtr& group);
  AdGroupPtr GetRotateGroups(int rotate_index);
  void GetAllRotateGroups(map<AdGroupPtr, int>& result);
  void ResizeRotateList(int size);
  void RandomRotateList();
  int GetRotateSize() const;
  string ToString() const ;
private:
  SETGET_DATA( long, zone_id )
  SETGET_DATA( int, member_type )
  SETGET_DATA( int, brand_count )
  SETGET_DATA( int, priority )
  SETGET_DATA( string, rotation_number )
  SETGET_DATA( int, brand_rotation_number )
  SETGET_DATA( int, self_rotation_number )
  SETGET_DATA( int, rotation_size )
  SETGET_DATA( double, min_price )
private:
  vector<AdGroupPtr> rotate_list_;
};
typedef IceUtil::Handle<AdZone> AdZonePtr;
typedef map<Ice::Long, AdZonePtr> AdZoneMap;

class AdUserBind : public AbstractStruct {
public:
  bool operator >(const AdUserBind& r) const;
  bool operator ==(const AdUserBind& r) const;
  bool operator <(const AdUserBind& r) const;
private:
  SETGET_DATA( long, zone_id )
  SETGET_DATA( int, user_id )
  SETGET_DATA( long, creative_id )
};
typedef IceUtil::Handle<AdUserBind> AdUserBindPtr;

class AdPlatform : public AbstractStruct {
private:
  SETGET_DATA( long, code )
  SETGET_DATA( string, name )
  SETGET_DATA( string, version )
};
typedef IceUtil::Handle<AdPlatform> AdPlatformPtr;


}
}
#endif
