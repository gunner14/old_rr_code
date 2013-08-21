package com.xiaonei.sns.platform.core.opt.ice.impl;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.platform.core.model.factory.WUserCacheFactory;
import com.xiaonei.sns.platform.core.opt.ice.IUserCacheAdapter;
import com.xiaonei.xce.usercache.UserCacheAdapter;

/**
 * The default implementation for 'WUserCache' service.
 * 
 * @author xylz (xylz@live.cn)
 */
public class WUserCacheAdapter extends BaseUsersAdapter<UserCacheAdapter<WUserCache>> implements
        IUserCacheAdapter {

    protected static final Log log = LogFactory.getLog(IUserCacheAdapter.class);

    /**
     * @deprecated
     * @see {@linkplain SnsAdapterFactory#getUserCacheAdapter()}
     */
    public static IUserCacheAdapter getInstance() {
        return SnsAdapterFactory.getUserCacheAdapter();
    }

    protected WUserCacheAdapter() {
    }

    @Override
    protected String getServiceName() {
        return "usercache";
    }

    @Override
    protected UserCacheAdapter<WUserCache> makeObject() {
        return new UserCacheAdapter<WUserCache>(new WUserCacheFactory());
    }

    public List<WUserCache> getUsersCacheList(List<Integer> userIds) {
        if (userIds != null && userIds.size() > 0) {
            Map<Integer, WUserCache> map = getUserCacheMap(userIds);
            if (map != null && map.size() > 0) {
                return new ArrayList<WUserCache>(map.values());
            }
        }
        return null;
    }

    public Map<Integer, WUserCache> getUserCacheMap(List<Integer> userIds) {
        Map<Integer, WUserCache> map = getAdapter().getUserCacheMap(userIds);
        if (log.isDebugEnabled()) {
            log.debug("getUserCacheMap (" + (userIds) + ") return:" + (map));
        }
        return map;
    }

}
