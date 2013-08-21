/**
 * 
 */
package com.xiaonei.sns.platform.core.opt.ice;

import java.util.List;
import java.util.Map;

import com.xiaonei.platform.core.model.WUserCache;

/**
 * A wrapper for 'WUserCache' service
 * 
 * @author lookis (comic.liu@gmail.com)
 * @author xylz (xylz@live.cn)
 */
public interface IUserCacheAdapter extends IRegisterable {

    /**
     * This method will be removed after Feb 2010.
     * 
     * @deprecated
     * @see #getUserCacheMap(List)
     */
    List<WUserCache> getUsersCacheList(List<Integer> userIds);

    /**
     * fetch 'WUserCache' with given user ids
     * 
     * @param userIds the user ids
     * @return a Map with all or some 'ids'
     */
    Map<Integer, WUserCache> getUserCacheMap(List<Integer> userIds);

}
