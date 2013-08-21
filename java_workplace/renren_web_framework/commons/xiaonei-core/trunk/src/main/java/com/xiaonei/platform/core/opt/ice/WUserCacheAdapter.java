package com.xiaonei.platform.core.opt.ice;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.platform.core.model.factory.WUserCacheFactory;
import com.xiaonei.xce.usercache.UserCacheAdapter;

/**
 * 
 * @author
 * @version 0.1 对ice user cache对象的封装
 * 
 */
public class WUserCacheAdapter {
	private static WUserCacheAdapter instance = null;

	public static WUserCacheAdapter getInstance() {
		if (WUserCacheAdapter.instance == null) {
			synchronized (WUserCacheAdapter.class) {
				if (WUserCacheAdapter.instance == null) {
					WUserCacheAdapter.instance = new WUserCacheAdapter();
				}

			}
		}
		return WUserCacheAdapter.instance;
	}

	private final UserCacheAdapter<WUserCache> adapter = new UserCacheAdapter<WUserCache>(
			new WUserCacheFactory());

	private WUserCacheAdapter() {
	}


	public List<WUserCache> getUsersCacheList(List<Integer> userIds) {
		if ((userIds != null) && !userIds.isEmpty()) {
			try {
				return new ArrayList<WUserCache>(adapter.getUserCacheMap(
						userIds).values());
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return null;
	}

	public Map<Integer, WUserCache> getUserCacheMap(List<Integer> userIds) {
		return adapter.getUserCacheMap(userIds);
	}

}
