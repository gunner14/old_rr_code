package com.xiaonei.xce.feed;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import xce.feed.FeedData;
import xce.feed.FeedReply;
import xce.feed.LoadType;

public class FeedLogicAdapter {
	private static FeedLogicAdapter instance;

	protected FeedLogicAdapter() {
	};

	public static FeedLogicAdapter getInstance() {
		if (instance == null) {
			instance = new FeedLogicAdapter();
		}
		return instance;
	}

	public void dispatch(Feed feed) throws Exception {
		FeedAssistantAdapter.getInstance().dispatch(feed);
	}
	public void dispatchAgain(long source, int stype , int actor, int toid){
		FeedAssistantAdapter.getInstance().dispatchAgain(source, stype, actor, toid);
	}
	public void dispatch(Feed feed, FeedReply reply) throws Exception {
		FeedAssistantAdapter.getInstance().dispatch(feed, reply);
	}

	public void dispatchEDM(Feed feed) throws Exception {
		FeedAssistantAdapter.getInstance().dispatchEDM(feed);
	}

	public void addFeedReply(long source, int stype, int actor, FeedReply reply) {
		FeedAssistantAdapter.getInstance().addFeedReply(source, stype, actor,
				reply);
	}
        /*
	public void updateFeedReply(long source, int stype, int actor,
			Map<String, String> newReplyMap, Map<String, String> oldReplyMap,
			long replyId, boolean isDel){
		FeedAssistantAdapter.getInstance().updateFeedReply(source, stype, actor, newReplyMap, oldReplyMap, replyId, isDel);
	}
        */
	public void addFeedReply(int stype, long feedId, FeedReply reply) {
		FeedAssistantAdapter.getInstance().addFeedReply(stype, feedId, reply);
	}
	
	public void addFeedReplyWithConfig(long source, int stype, int actor, FeedReply reply, Map<String, String> replyConfig) {
		FeedAssistantAdapter.getInstance().addFeedReplyWithConfig(source, stype, actor, reply, replyConfig);
	}

	public void removeFeedById(long feedId) {
		FeedAssistantAdapter.getInstance().removeFeedById(feedId);
	}

	public void removeFeedBySource(long source, int stype) {
		FeedAssistantAdapter.getInstance().removeFeedBySource(source, stype);
	}

	public void removeFeedBySourceHS(long source, int stype) {
    FeedAssistantAdapter.getInstance().removeFeedBySourceHS(source, stype);
  }

	public void removeFeedByTime(long source, int stype, int actor, int time){
                FeedAssistantAdapter.getInstance().removeFeedByTime(source, stype, actor, time);
        }

	public void removeFeedByParent(long parent, int ptype) {
		FeedAssistantAdapter.getInstance().removeFeedByParent(parent, ptype);
	}

	public void removeFeed(long source, int stype, int actor) {
		FeedAssistantAdapter.getInstance().removeFeed(source, stype, actor);
	}

	public void removeFeedReply(long source, int stype, int actor,
			long removeReplyId, FeedReply reply) {
		FeedAssistantAdapter.getInstance().removeFeedReply(source, stype,
				actor, removeReplyId, reply);
	}

	public void removeFeedReply(int stype, long feedid, long removeReplyId,
			FeedReply reply) {
		FeedAssistantAdapter.getInstance().removeFeedReply(stype, feedid,
				removeReplyId, reply);
	}

	public void readNews(int user, long feedid) {
		FeedAssistantAdapter.getInstance().readNews(user, feedid);
	}

	public void readAllNews(int user) {
		FeedAssistantAdapter.getInstance().readAllNews(user);
	}

	public void readMini(int user, long feedid) {
		FeedAssistantAdapter.getInstance().readMini(user, feedid);
	};

	public void readAllMini(int user) {
		FeedAssistantAdapter.getInstance().readAllMini(user);
	};

	public List<FeedData> getNewsByStype(int uid, int[] stypes, int begin,
			int limit) {
		return FeedItemAdapter.getInstance().getNewsByStype(uid, stypes, begin,
				limit);
	}

	public List<FeedData> getNewsByType(int uid, int[] types, int begin,
			int limit) {
		return FeedItemAdapter.getInstance().getNewsByType(uid, types, begin,
				limit);
	}

	public List<FeedData> getNews(int uid, int begin, int limit) {
		return FeedItemAdapter.getInstance().getNews(uid, begin, limit);
	}

	public List<FeedData> getNews2(int uid, int begin, int limit) {
		return FeedItemAdapter.getInstance().getNews2(uid, begin, limit);
	}

	public List<FeedData> getFeedNewsByIds(long[] feedIds) {
		return FeedItemAdapter.getInstance().getFeedDataByIds(feedIds);
	}

	public List<FeedData> getMiniByStype(int uid, int[] stypes, int begin,
			int limit) {
		return FeedMiniHaAdapter.getInstance().getMiniByStype(uid, stypes, begin,
				limit);
	}

	public List<FeedData> getMiniByType(int uid, int[] types, int begin,
			int limit) {
		return FeedMiniHaAdapter.getInstance().getMiniByType(uid, types, begin,
				limit);
	}

	public List<FeedData> getMini(int uid, int begin, int limit) {
		return FeedMiniHaAdapter.getInstance().getMini(uid, begin, limit);
	}

	public void joinPageNotify(int pageId, int fansId) {
		FeedDispatcherAdapter.getInstance().joinPageNotify(pageId, fansId);
	}

	public void guideBuddyReplyNotify(int newUser, int friend) {
		FeedDispatcherAdapter.getInstance().guideBuddyReplyNotify(newUser,
				friend);
	}

	public void addMiniSiteFeed(int siteid, int userid) {
                FeedDispatcherAdapter.getInstance().addMiniSiteFeed(siteid, userid);
        }

	public void addMiniSiteFeedToGuides(long source, int type, int siteid, int... guides) {
		FeedDispatcherAdapter.getInstance().addMiniSiteFeedToGuides(source, type, siteid, guides);
	}
	
	public boolean hasMiniFeed(int uid){
		return FeedMiniHaAdapter.getInstance().hasFeed(uid);
	}
	public int getNewsFeedCount(int uid){
		return FeedCacheNAdapter.getInstance().getCount(uid);
	}

	
	public List<FeedData> getFocusFeedData(int user, int begin, int limit) {
		return FeedFocusAdapter.getInstance().getFocusFeedData(user, begin, limit);
	}

	/**
	 * 
	 * @param user
	 * @param focuses
	 * @return  -1 throw an ice exception, 0  insert false, 1  insert ok
	 */
	public int addFocuses(int user, int target) {
		return FeedFocusAdapter.getInstance().addFocuses(user, target);
	}

 	//return value. 0. success 1. exceed limit. 2.focused already. 3. not friend. 4.other reason
	public int addFocusWithCheck(int user, int target) {
		return FeedFocusAdapter.getInstance().addFocusWithCheck(user, target);
	}

	public void delFocuses(int user, int target) {
		FeedFocusAdapter.getInstance().delFocuses(user, target);
	}
	
	public List<Integer> getFocuses(int user){
		return FeedFocusAdapter.getInstance().getFocuses(user);
	}
	
	
	public List<String>  getHot(int owner, String ownerName, int view, int begin, int limit){
		return FeedViewHaAdapter.getInstance().getHot(owner, ownerName, view, begin, limit);
	}
	public List<String> getHotByStype(int owner, String ownerName, int view,
			int[] types, int begin, int limit){
		return FeedViewHaAdapter.getInstance().getHotByStype(owner, ownerName, view, types, begin, limit);
	}
	public List<String> getHotByType(int owner, String ownerName, int view,
			int[] types, int begin, int limit){
		return FeedViewHaAdapter.getInstance().getHotByType(owner, ownerName, view, types, begin, limit);
	}
	public List<String>  getLive(int owner, String ownerName, int view, int begin, int limit){
		return FeedViewHaAdapter.getInstance().getLive(owner, ownerName, view, begin, limit);
	}
	public List<String>  getLiveByStype(int owner, String ownerName, int view, int[] stypes, int begin, int limit){
		return FeedViewHaAdapter.getInstance().getLiveByStype(owner, ownerName, view, stypes, begin, limit);
	}
	
	public List<String> getLiveByType(int owner, String ownerName, int view,
			int[] types, int begin, int limit){
		return FeedViewHaAdapter.getInstance().getLiveByType(owner, ownerName, view, types, begin, limit);
	}
	
	public void reloadDispatchExpr(LoadType  type , int id){
		FeedDispatcherAdapter.getInstance().reload(type, id);
	}
	public void reloadDispatchExpr2(LoadType  type , long id){
                FeedDispatcherAdapter.getInstance().reload2(type, id);
        }
	
	public void ReplaceXML(long source, int stype, int actor,String xml){
		FeedAssistantAdapter.getInstance().ReplaceXML(source,stype,actor,xml);
	};
	
	public void DirectedSend(long source, int stype, int actor,int[] targets){
		FeedAssistantAdapter.getInstance().DirectedSend(source,stype,actor,targets);
	};

	public long getFeedId(long source, int stype, int actor){
                return FeedAssistantNHaAdapter.getInstance().getFeedId(source, stype, actor);
        }

	public long[] getFeedidFromSocialWikiHotByWikiid(int wikiid, int begin, int limit){
                return SocialWikiHotByWikiidHaAdapter.getInstance().get(wikiid, begin, limit);
        }

	public long[] getFeedidByTypesFromSocialWikiHotByWikiid(int wikiid, int begin, int limit, int... types){
                return SocialWikiHotByWikiidHaAdapter.getInstance().getByTypes(wikiid, begin, limit, types);
        }
	
  public void removeFromSocialWikiNewsByWikiid(int wikiid, long feedid){
		SocialWikiNewsByWikiidHaAdapter.getInstance().remove(wikiid, feedid);
	}

	public void reloadFromSocialWikiNewsByWikiid(int wikiid){
		SocialWikiNewsByWikiidHaAdapter.getInstance().reload(wikiid);
	}

	public void removeFromSocialWikiHotByWikiid(int wikiid, long feedid){
		SocialWikiHotByWikiidHaAdapter.getInstance().remove(wikiid, feedid);
	}

	public void reloadFromSocialWikiHotByWikiid(int wikiid){
		SocialWikiHotByWikiidHaAdapter.getInstance().reload(wikiid);
	}

	public long[] getFeedidFromSocialWikiNewsByWikiid(int wikiid, int begin, int limit){
                return SocialWikiNewsByWikiidHaAdapter.getInstance().get(wikiid, begin, limit);
        }

        public long[] getFeedidByTypesFromSocialWikiNewsByWikiid(int wikiid, int begin, int limit, int... types){
                return SocialWikiNewsByWikiidHaAdapter.getInstance().getByTypes(wikiid, begin, limit, types);
        }

        public long[] getFeedidFromSocialWikiByUserid(int[] userids, int wikiid, int begin, int limit){
                return SocialWikiByUseridHaAdapter.getInstance().gets(userids, wikiid, begin, limit);
        }

        public long[] getFeedidByTypesFromSocialWikiByUserid(int[] userids, int wikiid, int[] types, int begin, int limit){
                return SocialWikiByUseridHaAdapter.getInstance().getsWithType(userids, wikiid, types, begin, limit);
        }
        
	public void changeWikiFeedPrivacy(int userid, long source, int stype, int privacy){
                FeedItemFacadeHaAdapter.getInstance().changeWikiFeedPrivacy(userid, source, stype, privacy);
        }

	public void setTimelineHighlight(int uid, int year, long feedid, int highlight){
                FeedAssistantAdapter.getInstance().setTimelineHighlight(uid, year, feedid, highlight);
        }

	public void setTimelineReadById(int uid, int year, long feedid){
                FeedAssistantAdapter.getInstance().setTimelineReadById(uid, year, feedid);
        }

        public void addLow(int userid, int blockUserid){
                FeedAssistantAdapter.getInstance().addLow(userid, blockUserid);
        }

        public void delLow(int userid, int blockUserid){
                FeedAssistantAdapter.getInstance().delLow(userid, blockUserid);
        }
}
