package com.xiaonei.sns.platform.core.rose.cache.provider;

import com.xiaonei.sns.platform.core.rose.cache.CacheManager;

/**
 * an base implement for {@link IProvider}
 * @author xylz(xylz@live.cn)
 *
 * @param <T> the Object to be cached
 */
public abstract class AbstractProvider<T> implements IProvider<T> {


    @Override
    public synchronized T sychGet(CacheManager cm, Object... params) {
        return get(cm, params);
    }

}
