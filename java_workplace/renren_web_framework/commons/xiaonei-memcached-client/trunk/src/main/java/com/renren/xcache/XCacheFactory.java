package com.renren.xcache;

/**
 * @author Wang Zhiliang
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 */
public interface XCacheFactory {

    /**
     * 根据指定prefix来获取XCache实例
     * 
     * @param <T>
     * @param prefix 
     * @param klass value的类型
     * @param isCounter 是否用作counter
     * @return
     */
    public <T> XCache<T> getCache(String prefix, Class<T> klass, boolean isCounter);
}
