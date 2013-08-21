package com.renren.sns.minisite.feed.xoa.utils.feedcache;


public interface Stack {


    long[] getCachedUgcIds(int userId, final int start, final int count);

    /**
     * @author Wang Tai (cn.wang.tai@gmail.com)
     * @date Jun 14, 2011 4:35:17 PM
     * 
     * @param userId
     * @param ugcId
     * @return 写入缓存
     */
    boolean pushCachedUgcId(int userId, final long ugcId);

    /**
     * @author Wang Tai (cn.wang.tai@gmail.com)
     * @date Jun 14, 2011 4:27:19 PM
     * 
     * @param userId
     * @param index
     * @return 删除成功返回true
     */
    boolean removeUgcId(int userId, final int index);

}
