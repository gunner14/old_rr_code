package com.xiaonei.sns.platform.core.rose.cache.provider;

import com.xiaonei.sns.platform.core.rose.cache.CacheManager;

/**
 * 配合CacheManager取得不同类型的数据，用统一的Provider去取以保证CacheManager可以共享上一次使用过的数据
 * 
 * @author lookis(comic.liu@gmail.com)
 * @author xylz(xylz@live.cn)
 */
public interface IProvider<T extends Object> {

    /**
     * fetch an Object from cache<br />
     * The Object can be reused in one request any where, so putting it in cache.
     * 
     * @param cm Cache对象
     * @param params 取数据的参数，也是Cache的KEY
     * @return 缓存对象或者null
     */
    T get(CacheManager cm, Object... params);
    /**
     * 从缓存中获取一个对象，注意此方法是线程安全的<br />
     * 通常情况下应该使用{@link #get(CacheManager, Object...)}方法，此方法没有加锁，性能稍好
     * @param cm CacheManager
     * @param params 参数
     * @return 缓存对象或者null
     * @see #get(CacheManager, Object...)
     */
    T sychGet(CacheManager cm,Object... params);
}
