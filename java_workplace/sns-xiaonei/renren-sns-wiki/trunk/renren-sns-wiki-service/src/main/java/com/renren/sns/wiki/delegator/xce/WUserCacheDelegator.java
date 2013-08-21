package com.renren.sns.wiki.delegator.xce;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;

import org.springframework.stereotype.Service;

import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.platform.core.opt.ice.WUserCacheAdapter;

/**
 * @author yi.li@renren-inc.com since 2012-4-10
 * 
 */
@Service
public class WUserCacheDelegator {

    /**
     * 获取多个用户的WUserCache列表，返回的顺序和获取的顺序一致，当某一个id没有获取到对象时，将不会添加到list中去
     * 
     * @param uids
     * @return
     */
    public List<WUserCache> getCacheUsers(List<Integer> uids) {
        if ((null == uids) || (uids.size() == 0)) {
            return Collections.emptyList();
        }

        List<WUserCache> users = new ArrayList<WUserCache>();
        Map<Integer, WUserCache> userMaps = WUserCacheAdapter.getInstance().getUserCacheMap(uids);
        for (Integer id : uids) {
            WUserCache user = userMaps.get(id);
            if (null != user) {
                users.add(user);
            }
        }

        return users;
    }

    /**
     * 获取多个用户的WUserCache列表，返回的顺序和获取的顺序一致，当某一个id没有获取到对象时，将不会添加到list中去
     * 
     * @param uids
     * @return
     */
    public Map<Integer, WUserCache> getUserCacheMap(List<Integer> uids) {
        if ((null == uids) || (uids.size() == 0)) {
            return Collections.emptyMap();
        }

        return WUserCacheAdapter.getInstance().getUserCacheMap(uids);
    }
}
