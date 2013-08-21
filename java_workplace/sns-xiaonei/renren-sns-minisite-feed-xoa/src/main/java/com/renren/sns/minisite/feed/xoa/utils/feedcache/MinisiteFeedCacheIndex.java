package com.renren.sns.minisite.feed.xoa.utils.feedcache;

import java.util.concurrent.TimeUnit;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.sns.minisite.feed.xoa.dao.FeedsIndexDAO;
import com.renren.xcache.XCache;

public class MinisiteFeedCacheIndex implements StackIndex {

    private static final String FEEDS_INDEX = "minisite_feeds_index_";

    private static final String FEEDS_INDEX_KEY = "minisite_feeds_";

    private final XCache<Long> cache;

    private FeedsIndexDAO feedsIndexDAO;

    private final int length;

    private final long start = 0l;

    protected Log logger = LogFactory.getLog(this.getClass());

    public MinisiteFeedCacheIndex(final XCache<Long> cache, final int length) {
        this.cache = cache;
        this.length = length;
    }

    @Override
    public String buildIndexKey(final int userId, final long index) {
        return MinisiteFeedCacheIndex.FEEDS_INDEX_KEY + userId + "_" + index;
    }

    /**
     * @author Wang Tai (cn.wang.tai@gmail.com)
     * @date Jun 13, 2011 5:03:51 PM
     * 
     * @return 当前的索引值
     * 
     */
    @Override
    public String getIndex(final int userId) {
        final Long index = this.getIndexFromCache(userId);
        return this.buildIndexKey(userId, this.mod(userId, index));
    }

    /**
     * @author Wang Tai (cn.wang.tai@gmail.com)
     * @date Jun 14, 2011 4:53:09 PM
     * 
     * @param start
     * @param count
     * @return 返回 start，start从0开始 开始共count个索引
     */
    @Override
    public String[] getIndexs(final int userId, final int start, int count) {
        final Long long1 = this.getIndexFromCache(userId);
        if (null == long1) {
            this.logger.error(this.buildIndex(userId) + " in cache is null");
            return new String[0];
        }
        Long index = long1 - start;
        count = (int) (index > count ? count : index);
        count = count > 0 ? count : 0;
        final String[] indexs = new String[count];
        for (int i = 0; i < indexs.length; i++) {
            indexs[i] = this.buildIndexKey(userId, this.mod(userId, index));
            index--;
        }
        return indexs;
    }

    /**
     * @author Wang Tai (cn.wang.tai@gmail.com)
     * @date Jun 13, 2011 6:07:41 PM
     * 
     * @return 获得一个新增的index
     */
    @Override
    public String getNewIndex(final int userId) {
        Long index = this.getIndexFromCache(userId);
        if ((null == index) || (index > Long.MAX_VALUE)) {
            index = 0l;
        } else {
            index++;
        }
        index = (long) this.mod(userId, index);
        return this.buildIndexKey(userId, index);    	
    }

    /**
     * 增加移动索引
     * 
     * @author Wang Tai (cn.wang.tai@gmail.com)
     * @date Jun 13, 2011 5:18:00 PM
     * 
     * @param inc
     */
    @Override
    public void incr(final int userId, final long inc) {
        final long incr = this.cache.incr(this.buildIndex(userId), inc);
        if (this.logger.isDebugEnabled()) {
            this.logger.debug("MinisiteFeedCacheIndex.incr():" + userId + "|" + incr);
        }
        if (-1 == incr) {
            this.initCachedIndex(userId);
        } else {
        	if(this.feedsIndexDAO.updateIndex(userId, incr)!=1) //update first
        		this.feedsIndexDAO.insertIndex(userId, incr);	//if not exist, insert it
        }
    }

    public void setFeedsIndexDAO(final FeedsIndexDAO feedsIndexDAO) {
        this.feedsIndexDAO = feedsIndexDAO;
    }

    private String buildIndex(final int userId) {
        return MinisiteFeedCacheIndex.FEEDS_INDEX + userId;
    }

    private Long getIndexFromCache(final int userId) {
        Long index = this.cache.get(this.buildIndex(userId));
        if (null == index) {
        	//edit by zwang, 2011/07/29
            try {
				index = this.feedsIndexDAO.getIndex(userId);
			} catch (final NullPointerException e) {
				// TODO: handle exception
				this.logger.error("empty set index in feeds_index for "+userId);
			}
			if (null == index) {
                this.initCachedIndex(userId);
            } else {
                this.cache.set(this.buildIndex(userId), index, (int) TimeUnit.DAYS.toSeconds(30));
            }
        }
        return index;
    }

    private boolean initCachedIndex(final int userId) {
        //edit by zwang, 2011/07/29
    	//this.feedsIndexDAO.initIndex(userId, this.start);
        return this.cache.set(this.buildIndex(userId), new Long(this.start),
                (int) TimeUnit.DAYS.toSeconds(30));
    }

    private int mod(final int userId, Long index) {
        if ((index == null) || (index < this.start)) {
            this.initCachedIndex(userId);
            index = 0l;
        } else {
            index = (index - this.start) % this.length;
        }
        return index.intValue();
    }

}
