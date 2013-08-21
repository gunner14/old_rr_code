package com.xiaonei.sns.platform.core.opt.ice.impl;

import java.util.List;
import java.util.Map;

import com.xiaonei.sns.platform.core.opt.ice.IUserStateCacheAdapter;
import com.xiaonei.xce.userstatecache.UserStateCache;
import com.xiaonei.xce.userstatecache.UserStateCacheAdapter;

/**
 * The default implementation for UserStateCacheAdapter
 * @author xylz (xylz@live.cn)
 * @sine 2010-8-11
 */
public class WUserStateCacheAdapter extends BaseUsersAdapter<UserStateCacheAdapter> implements IUserStateCacheAdapter {

    @Override
    public Map<Integer, UserStateCache> getUserStateCacheList(List<Integer> userIds) {
        return getAdapter().getUserStateCacheList(userIds);
    }
    @Override
    protected String getServiceName() {
        return "userstatecache";
    }
    @Override
    protected UserStateCacheAdapter makeObject() {
        return UserStateCacheAdapter.getInstance();
    }

}
