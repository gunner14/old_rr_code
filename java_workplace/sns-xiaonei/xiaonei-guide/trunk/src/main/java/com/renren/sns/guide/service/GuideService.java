package com.renren.sns.guide.service;

import com.renren.app.share.hot.model.HotShare;
import com.renren.app.share.model.Share;
import com.xiaonei.platform.core.model.User;

public interface GuideService {

	public abstract Share parseHotShareToShare(HotShare hotShare);
	
	public abstract void transmitNewsFeed(User host);
	
	public abstract String cutLongTitle(String title);
	
	public abstract String abbreviate(String str, int len);
	
	public abstract String getAlbumUrl(long albumId, int userId);
	
	public abstract long getMergeId(Share share);
	
}
