/*
 * Function: Add new friends-of-friends recommendation strategy according to 
 * Facebook's people you may model  
 * Developer: xiongjunwu
 * Email: Junwu.xiong@renren-inc.com
 * Date: 2012-03-27
 */

package com.renren.xce.socialgraph.strategy;

import java.util.ArrayList;
import java.util.BitSet;
import java.util.List;

import xce.socialgraph.util.Clogging;

import com.renren.xce.socialgraph.common.Category;
import com.renren.xce.socialgraph.common.Strategy;
import com.renren.xce.socialgraph.updator.CreateRecommendDataThread;
import com.xiaonei.xce.friendcountcache.FriendCountCacheAdapter;
import com.xiaonei.xce.scorecache.DefaultScoreCache;
import com.xiaonei.xce.scorecache.DefaultScoreCacheFactory;
import com.xiaonei.xce.scorecache.ScoreCacheAdapter;
import com.xiaonei.xce.socialgraph.userapply.UserApplyStatsAdapter;

public class RcdFoFStrategy implements Strategy{
	public List<Category> getStrategy(int userId) {
		List<Category> categoryList = new ArrayList<Category>();
		
		BitSet applyStats = new BitSet(31);
		int friendsCount = 0;
		int loginCount = 0;
		
		try {
			applyStats = getApplyStats(userId);		//user's status of applying
			friendsCount = getFriendsCount(userId);		//number of user's friends
			loginCount = getLoginCount(userId);		//number of user's login
		} catch (Exception e) {
			e.printStackTrace();
		}

		if (applyStats.cardinality() == 0 && loginCount > 15		//no applying in a month, login > 15, friendsCount > 100
				&& friendsCount > 100) {
			Clogging.Debug("UserApplyStrategy use no applying strategy userId:" + userId);
			
			CommonCategory fofList = new CommonCategory(CreateRecommendDataThread.RFOF, 18, 10);
			categoryList.add(fofList);
			
			CommonCategory page = new CommonCategory(CreateRecommendDataThread.PAGE, 4, 10);
			categoryList.add(page);
			
			CommonCategory video = new CommonCategory(CreateRecommendDataThread.RVIDEO, 9, 10);
			categoryList.add(video);
			
			CommonCategory blog = new CommonCategory(CreateRecommendDataThread.RBLOG, 9, 10);
			categoryList.add(blog);
			
			//CommonCategory site = new CommonCategory(CreateRecommendDataThread.RSITE, 2, 10);
			//categoryList.add(site);
			
			CommonCategory app = new CommonCategory(CreateRecommendDataThread.RAPP, 2, 10);
			categoryList.add(app);
		
			CommonCategory forum = new CommonCategory(CreateRecommendDataThread.RFORUM, 2, 10);
			categoryList.add(forum);
			
			CommonCategory desk = new CommonCategory(CreateRecommendDataThread.RDESK, 1, 10);
			categoryList.add(desk);
		} else {
			Clogging.Debug("UserApplyStrategy use common strategy userId:" + userId);
			
			CommonCategory fofList = new CommonCategory(CreateRecommendDataThread.RFOF, 30, 10);
			categoryList.add(fofList);
			
			CommonCategory page = new CommonCategory(CreateRecommendDataThread.PAGE, 4, 10);
			categoryList.add(page);
			
			CommonCategory video = new CommonCategory(CreateRecommendDataThread.RVIDEO, 3, 10);
			categoryList.add(video);
			
			CommonCategory blog = new CommonCategory(CreateRecommendDataThread.RBLOG, 3, 10);
			categoryList.add(blog);
			
			//CommonCategory site = new CommonCategory(CreateRecommendDataThread.RSITE, 2, 10);
			//categoryList.add(site);
			
			CommonCategory app = new CommonCategory(CreateRecommendDataThread.RAPP, 2, 10);
			categoryList.add(app);
			
			CommonCategory forum = new CommonCategory(CreateRecommendDataThread.RFORUM, 2, 10);
			categoryList.add(forum);
			
			CommonCategory desk = new CommonCategory(CreateRecommendDataThread.RDESK, 1, 10);
			categoryList.add(desk);
		}
		
		return categoryList;
	}
	
	/**
	 * Get apply status of user
	 * @param userId
	 * @return
	 */
	private BitSet getApplyStats(int userId) {
		return UserApplyStatsAdapter.getInstance().getUserApplyStats(userId);
	}
	
	/**
	 * Get count of user's friends
	 * @param userId
	 * @return
	 */
	private int getFriendsCount(int userId) {
		return FriendCountCacheAdapter.getInstance().getFriendCount(userId);
	}
	
	/**
	 * Get count of login in month
	 * @param userId
	 * @return
	 */
	private int getLoginCount(int userId) {
		int count = 0;
		ScoreCacheAdapter<DefaultScoreCache> adapter = new ScoreCacheAdapter<DefaultScoreCache>(
				new DefaultScoreCacheFactory());
		DefaultScoreCache data = adapter.getScoreData(userId);
		List<Boolean> activeDaysList = data.getActiveDays();
		for (boolean each : activeDaysList) {
			if (each) {
				++count;
			}
		}
		return count;
	}
}
