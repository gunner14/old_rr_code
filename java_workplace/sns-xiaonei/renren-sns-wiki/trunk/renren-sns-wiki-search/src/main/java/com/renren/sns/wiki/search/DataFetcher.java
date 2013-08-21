package com.renren.sns.wiki.search;

import java.util.List;

/**
 * 该类用来获取数据创建索引
 * 
 * @author yi.li@renren-inc.com since Aug 13, 2012
 * 
 * @param <T>
 */
public interface DataFetcher<T> {

    /**
     * 是否还有数据可以获取
     * 
     * @return true if has more data to fetch, otherwise false
     */
    public boolean hasMore();

    /**
     * 获取数据
     * 
     * @return List<T> 如果没有取到数据返回大小为空的List
     */
    public List<T> fetchData();

    /**
     * 重置获取器的指针，下一次调用{@link #fetchData()}重头开始获取
     */
    public void reset();
}
