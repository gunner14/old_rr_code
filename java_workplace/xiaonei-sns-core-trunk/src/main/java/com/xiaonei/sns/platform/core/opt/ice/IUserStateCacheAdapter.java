package com.xiaonei.sns.platform.core.opt.ice;

import java.util.List;
import java.util.Map;

import com.xiaonei.xce.userstatecache.UserStateCache;

/**
 * a wrapper for UserStateCache
 * 
 * @author xylz (xylz@live.cn)
 * @sine 2010-8-11
 */
public interface IUserStateCacheAdapter extends IRegisterable {

    Map<Integer, UserStateCache> getUserStateCacheList(List<Integer> userIds);
}
