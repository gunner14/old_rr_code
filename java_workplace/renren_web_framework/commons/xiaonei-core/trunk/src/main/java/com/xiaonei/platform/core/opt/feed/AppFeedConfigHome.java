package com.xiaonei.platform.core.opt.feed;

import com.xiaonei.platform.core.opt.feed.dao.FeedRecvConfigDAO;
import com.xiaonei.platform.core.opt.feed.dao.FeedSendConfigDAO;

/**
 * App 设置Feed的Home
 * 
 */
public class AppFeedConfigHome {
	private static AppFeedConfigHome instance = new AppFeedConfigHome();

	public static AppFeedConfigHome getInstance() {
		if (instance == null) {
			synchronized (AppFeedConfigHome.class) {
				if (instance == null)
					instance = new AppFeedConfigHome();
			}
		}
		return instance;
	}

	private AppFeedConfigHome() {
		super();
	}

	/**
	 * 拒绝收到我的朋友的行为展现在我的News-Feed上
	 * 
	 * @param userId
	 * @param appId
	 */
	public void setRefuseReceiveNewsFeed(int userId, int appId) {
		try {
			FeedRecvConfigDAO.getInstance().save(userId, appId);
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
	}

	/**
	 * 在我的News-Feed接收朋友所产生的行为
	 * 
	 * @param userId
	 * @param appId
	 */
	public void setReceiveNewsFeed(int userId, int appId) {
		try {
			FeedRecvConfigDAO.getInstance().delete(userId, appId);
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
	}

	/**
	 * 拒绝将行为发送到Mini-Feed，以及拒绝将行为发送到朋友的News-Feed
	 * 
	 * @param userId
	 * @param appId
	 */
	public void setRefuseSendMiniFeed(int userId, int appId) {
		try {
			FeedSendConfigDAO.getInstance().save(userId, appId);
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
	}

	/**
	 * 将自身的行为发送到Mini-Feed，以及将行为发送到朋友的News-Feed
	 * 
	 * @param userId
	 * @param appId
	 */
	public void setSendMiniFeed(int userId, int appId) {
		try {
			FeedSendConfigDAO.getInstance().delete(userId, appId);
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
	}
	
}
