#ifndef XCE_FEED_FEEDVIEW_I_H__
#define XCE_FEED_FEEDVIEW_I_H__

// #include "feed/feedview/feedview.h"
#include "FeedView.h"
// #include "RFeed.h"
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include "ServiceI.h"
#include "TaskManager.h"
#include "Singleton.h"

//#include "Singleton.h"
//#include "ServiceI.h"
namespace xce {

class FeedViewI : public xce::feed::FeedView , public MyUtil::Singleton<FeedViewI>{
//, public MyUtil::Singleton<FeedViewI> {
public:
  virtual feed::HtmlResult Get(Ice::Int uid, const std::string& name, Ice::Int render
    , Ice::Int from, Ice::Int count, const Ice::Current& current= Ice::Current());

  virtual feed::HtmlResult GetByType(Ice::Int uid, const std::string& name
    , Ice::Int render, const ::xce::feed::TypeSeq& types
    , Ice::Int from, Ice::Int count, const Ice::Current& = Ice::Current());

  virtual feed::HtmlResult GetByStype(Ice::Int uid, const std::string& name
    , Ice::Int render, const ::xce::feed::TypeSeq& stypes
    , Ice::Int from, Ice::Int count, const Ice::Current& = Ice::Current());

  virtual feed::HtmlResult GetByIds(Ice::Int uid, const std::string& name
    , const feed::FeedIdSeq & fids, const Ice::Current& = Ice::Current());
  virtual feed::HtmlResult GetByIds2(Ice::Int uid, const std::string& name, Ice::Int render
    , const feed::FeedIdSeq & fids, const Ice::Current& = Ice::Current());


  virtual feed::HtmlResult GetByIdsAndStype(Ice::Int uid, const std::string& name
    , const feed::FeedIdSeq & fids, int stype, long firstID, const Ice::Current& = Ice::Current());
  virtual feed::HtmlResult GetByIdsAndStype2(Ice::Int uid, const std::string& name, Ice::Int render
    , const feed::FeedIdSeq & fids, int stype, long firstID, const Ice::Current& = Ice::Current());

  virtual feed::HtmlResult GetMini(Ice::Int owner, Ice::Int uid, const std::string& name, Ice::Int render
    , Ice::Int from, Ice::Int count, const Ice::Current& current= Ice::Current());

  virtual feed::HtmlResult GetMiniByType(Ice::Int owner, Ice::Int uid, const std::string& name
    , Ice::Int render, const ::xce::feed::TypeSeq& types
    , Ice::Int from, Ice::Int count, const Ice::Current& = Ice::Current());

  virtual feed::HtmlResult GetMiniByStype(Ice::Int owner, Ice::Int uid, const std::string& name
    , Ice::Int render, const ::xce::feed::TypeSeq& stypes
    , Ice::Int from, Ice::Int count, const Ice::Current& = Ice::Current());

  virtual feed::HtmlResult GetFavorite(Ice::Int uid, const std::string& name
    , Ice::Int render, Ice::Int from, Ice::Int count, const Ice::Current& = Ice::Current());

  virtual feed::HtmlResult GetFriendOriginal(Ice::Int uid, const std::string& name
    , Ice::Int render, Ice::Int from, Ice::Int count, const Ice::Current& = Ice::Current());
  virtual feed::HtmlResult GetGuideFeed(Ice::Int uid, const std::string& name
    , Ice::Int render, Ice::Int from, Ice::Int count, const Ice::Current& = Ice::Current());
  virtual feed::HtmlResult GetHot(Ice::Int uid, const std::string& name
    , Ice::Int render, Ice::Int from, Ice::Int count, const Ice::Current& = Ice::Current());
  virtual feed::HtmlResult GetHotByStype(Ice::Int uid, const std::string& name
    , Ice::Int render, const ::xce::feed::TypeSeq& types, Ice::Int from, Ice::Int count, const Ice::Current& = Ice::Current());
  virtual feed::HtmlResult GetHotByType(Ice::Int uid, const std::string& name
    , Ice::Int render, const ::xce::feed::TypeSeq& types, Ice::Int from, Ice::Int count, const Ice::Current& = Ice::Current());

  virtual feed::HtmlResult GetLive(Ice::Int uid, const std::string& name
    , Ice::Int render, Ice::Int from, Ice::Int count, const Ice::Current& = Ice::Current());
  virtual feed::HtmlResult GetLiveByType(Ice::Int uid, const std::string& name
    , Ice::Int render, const ::xce::feed::TypeSeq& types, Ice::Int from, Ice::Int count, const Ice::Current& = Ice::Current());
  virtual feed::HtmlResult GetLiveByStype(Ice::Int uid, const std::string& name
    , Ice::Int render, const ::xce::feed::TypeSeq& stypes, Ice::Int from, Ice::Int count, const Ice::Current& = Ice::Current());

  virtual ::xce::feed::HtmlResult GetMiniGroupFeedData(::Ice::Int, const ::std::string&, ::Ice::Int, ::Ice::Int, ::Ice::Int, ::Ice::Int, const ::Ice::Current& = ::Ice::Current());
  virtual ::xce::feed::HtmlResult GetMiniGroupFeedDataByType(::Ice::Int, const ::std::string&, ::Ice::Int, ::Ice::Int, const ::xce::feed::IntSeq&, ::Ice::Int, ::Ice::Int, const ::Ice::Current& = ::Ice::Current()); 
  virtual ::xce::feed::HtmlResult GetMiniGroupFeedDataByStype(::Ice::Int, const ::std::string&, ::Ice::Int, ::Ice::Int, const ::xce::feed::IntSeq&, ::Ice::Int, ::Ice::Int, const ::Ice::Current& = ::Ice::Current());

  virtual ::xce::feed::HtmlResult GetMiniGroupFeedData2(::Ice::Int, const ::std::string&, ::Ice::Int, ::Ice::Int, ::Ice::Int, ::Ice::Int, bool,  const ::Ice::Current& = ::Ice::Current());
  virtual ::xce::feed::HtmlResult GetMiniGroupFeedData2CS(::Ice::Int, const ::std::string&, ::Ice::Int, ::Ice::Int, ::Ice::Int, ::Ice::Int, bool, Ice::Int view,  const ::Ice::Current& = ::Ice::Current());
  virtual ::xce::feed::HtmlResult GetMiniGroupFeedDataByType2(::Ice::Int, const ::std::string&, ::Ice::Int, ::Ice::Int, const ::xce::feed::IntSeq&, ::Ice::Int, ::Ice::Int, bool,  const ::Ice::Current& = ::Ice::Current()); 
  virtual ::xce::feed::HtmlResult GetMiniGroupFeedDataByStype2(::Ice::Int, const ::std::string&, ::Ice::Int, ::Ice::Int, const ::xce::feed::IntSeq&, ::Ice::Int, ::Ice::Int, bool, const ::Ice::Current& = ::Ice::Current());

  virtual void Test(Ice::Int uid, const std::string& name
    , Ice::Int render, Ice::Int from, Ice::Int count, const Ice::Current& = Ice::Current());
  virtual bool ReloadCSTemplate(const Ice::Current& = ::Ice::Current());
  virtual bool ReloadDataSchema(const Ice::Current& = ::Ice::Current());
  virtual void ReloadWhitelist(const Ice::Current& = ::Ice::Current());

  virtual feed::CSTplCheckRes CheckCSTemplate(const std::string& tpl, const Ice::Current& = ::Ice::Current());

  virtual feed::FeedDataResPtr GetHotDataByStype(::Ice::Int uid, const std::string& name, ::Ice::Int render,
      const ::xce::feed::TypeSeq& stypes, 
      ::Ice::Int begin, ::Ice::Int limit, 
      const ::Ice::Current&  = ::Ice::Current());

  virtual feed::FeedDataResPtr GetLiveDataByStype(::Ice::Int uid, const std::string& name, ::Ice::Int render,
      const ::xce::feed::TypeSeq& stypes, 
      ::Ice::Int begin, ::Ice::Int limit, 
      const ::Ice::Current&  = ::Ice::Current());

  virtual feed::FeedDataSeq GetMiniDataByStype(::Ice::Int uid, const std::string& name, ::Ice::Int render,
      const ::xce::feed::TypeSeq& stypes, 
      ::Ice::Int begin, ::Ice::Int limit, 
      const ::Ice::Current&  = ::Ice::Current());

  virtual feed::FeedDataResPtr GetHotDataByStype2(::Ice::Int uid, const std::string& name, ::Ice::Int render,
      const ::xce::feed::TypeSeq& stypes, 
      ::Ice::Int begin, ::Ice::Int limit, ::Ice::Int view,
      const ::Ice::Current&  = ::Ice::Current());

  virtual feed::FeedDataResPtr GetLiveDataByStype2(::Ice::Int uid, const std::string& name, ::Ice::Int render,
      const ::xce::feed::TypeSeq& stypes, 
      ::Ice::Int begin, ::Ice::Int limit, ::Ice::Int view,
      const ::Ice::Current&  = ::Ice::Current());

  virtual feed::FeedDataByteResPtr GetLiveDataByStypeCompressed(::Ice::Int uid, const std::string& name, ::Ice::Int render,
      const ::xce::feed::TypeSeq& stypes,
      ::Ice::Int begin, ::Ice::Int limit, ::Ice::Int view,
      const ::Ice::Current& = ::Ice::Current());

  virtual feed::FeedDataSeq GetMiniDataByStype2(::Ice::Int uid, const std::string& name, ::Ice::Int render,
      const ::xce::feed::TypeSeq& stypes, 
      ::Ice::Int begin, ::Ice::Int limit, ::Ice::Int view, 
      const ::Ice::Current&  = ::Ice::Current());

  virtual feed::FeedDataByteSeq GetMiniDataByStypeCompressed(::Ice::Int uid, const std::string& name, ::Ice::Int render,
      const ::xce::feed::TypeSeq& stypes,
      ::Ice::Int begin, ::Ice::Int limit, ::Ice::Int view,
      const ::Ice::Current& = ::Ice::Current());

  virtual feed::FeedDataSeq GetFavorite2(::Ice::Int uid, const std::string& name, 
      ::Ice::Int render, ::Ice::Int begin, ::Ice::Int limit, ::Ice::Int view, const Ice::Current& = Ice::Current());

  virtual feed::FeedDataSeq GetFavorite2ByStype(::Ice::Int uid, const std::string& name, 
      ::Ice::Int render, const ::xce::feed::TypeSeq& stypes, ::Ice::Int begin, ::Ice::Int limit, ::Ice::Int view, 
      const Ice::Current& = Ice::Current());

  virtual feed::FeedDataSeq GetByIds2View(Ice::Int uid, const std::string& name, Ice::Int render
      , const feed::FeedIdSeq & fids, Ice::Int view, const Ice::Current& = Ice::Current());

  virtual feed::FeedDataSeq GetGreaterThanFeedidWithUseridByType(::Ice::Int appid, ::Ice::Long feedid, ::Ice::Int limit,
      ::Ice::Int uid, const ::xce::feed::TypeSeq& types, ::Ice::Int render, Ice::Int view, const Ice::Current& = Ice::Current());

  virtual feed::FeedDataByteSeq GetUnlimitedDataByStype(::Ice::Int uid, ::Ice::Int begin, ::Ice::Int limit,
      const ::xce::feed::TypeSeq& stypes,::Ice::Int render, ::Ice::Int view,
      const ::Ice::Current& = ::Ice::Current());

  virtual feed::HtmlResult GetBySourceStypeAndActor(::Ice::Int uid, const std::string& name, ::Ice::Long source, ::Ice::Int stype, ::Ice::Int actor, ::Ice::Int render, ::Ice::Int view, const Ice::Current& = Ice::Current());
  virtual feed::timeline::UidYear GetTimelineYearMonth(::Ice::Int uid, const Ice::Current& = Ice::Current());
  virtual feed::HtmlResult GetTimelineDataWithAdmin(::Ice::Int owner, ::Ice::Int uid, const std::string& name, ::Ice::Int year, ::Ice::Int month, ::Ice::Int render,
      ::Ice::Int begin, ::Ice::Int limit, bool isAdmin, const Ice::Current& = Ice::Current());
  virtual feed::HtmlResult GetTimelineDataWithAdminByStypes(::Ice::Int owner, ::Ice::Int uid, const std::string& name, ::Ice::Int year, ::Ice::Int month, ::Ice::Int render,
      ::Ice::Int begin, ::Ice::Int limit, bool isAdmin, const ::xce::feed::TypeSeq& stypes, const Ice::Current& = Ice::Current());
  virtual feed::HtmlResult GetTimelineData(::Ice::Int owner, ::Ice::Int uid, const std::string& name, ::Ice::Int year, ::Ice::Int month, ::Ice::Int render,
      ::Ice::Int begin, ::Ice::Int limit, const Ice::Current& = Ice::Current());

private:
  void CheckLoad(const vector<Ice::Long> & fids, feed::FeedContentDict & dict, int uid = 0);
  void GetDataByIds(const vector<Ice::Long> & fids, feed::FeedDataSeq & feed_seq, int uid = 0);
};

}
#endif // XCE_FEED_FEEDVIEW_I_H__
