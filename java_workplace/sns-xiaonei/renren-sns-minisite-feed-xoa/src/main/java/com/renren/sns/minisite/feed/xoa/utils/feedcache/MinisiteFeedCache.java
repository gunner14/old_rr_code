package com.renren.sns.minisite.feed.xoa.utils.feedcache;

import java.util.Map;
import java.util.concurrent.TimeUnit;

import org.apache.commons.lang.ArrayUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.sns.minisite.feed.xoa.dao.FeedsIndexDAO;

public class MinisiteFeedCache implements Stack {

    private final static int NEW_UGCS_COUNT = 1000;

    private final StackIndex cacheIndex;

    protected Log logger = LogFactory.getLog(this.getClass());

    public MinisiteFeedCache() {
        this.cacheIndex = new MinisiteFeedCacheIndex(Resource.INDEX_CACHE,
                MinisiteFeedCache.NEW_UGCS_COUNT);
    }

    /* (non-Javadoc)
     * @see com.renren.sns.minisite.feed.xoa.utils.feedcache.aaa#getCachedUgcIds(int, int)
     */
    @Override
    public long[] getCachedUgcIds(final int userId, final int start, final int count) {
        final String[] indexs = this.cacheIndex.getIndexs(userId, start, count);
        //added by zwang, 2011/07/28
        //System.out.println("*********indexs.length="+indexs.length);
        //added by zwang, 2011/07/28
        //if (!ArrayUtils.isEmpty(indexs))
        	//System.out.println("***********getCachedUgcIds start:" + indexs[0] + ",end:"
            //    + indexs[indexs.length - 1]);
        
        if (this.logger.isDebugEnabled()) {
            if (!ArrayUtils.isEmpty(indexs)) {
                this.logger.debug("getCachedUgcIds start:" + indexs[0] + ",end:"
                        + indexs[indexs.length - 1]);
            } else {
                this.logger.debug("getCachedUgcIds null");
            }
        }
        if (ArrayUtils.isEmpty(indexs)) {
            return new long[0];
        }
        final Map<String, Long> mapUgcIds = Resource.CACHE.getMulti(indexs);
        for (final String index : indexs) {
            if (null == mapUgcIds.get(index)) {
                mapUgcIds.remove(index);
            }
        }
        final long[] ugcIds = new long[mapUgcIds.size()];
        int i = 0;
        for (final String index : indexs) {
            if (null != mapUgcIds.get(index)) {
                ugcIds[i] = mapUgcIds.get(index);
                i++;
            }
        }
        return ugcIds;
    }

    /* (non-Javadoc)
     * @see com.renren.sns.minisite.feed.xoa.utils.feedcache.aaa#pushCachedUgcId(long)
     */
    @Override
    public boolean pushCachedUgcId(final int userId, final long ugcId) {
    	//added by zwang, 2011/08/01
    	//System.out.println("=========come in pushCachedUgcId===");
        final String newIndex = this.cacheIndex.getNewIndex(userId);
        this.logger.debug("pushCachedUgcId:" + newIndex + "," + ugcId);
        //added by zwang, 2011/07/27
        //System.out.println("==============pushCachedUgcId:" + newIndex + "," + ugcId);
        if (Resource.CACHE.set(newIndex, ugcId, (int) TimeUnit.DAYS.toSeconds(1) * 7)) {
            this.cacheIndex.incr(userId, 1);
            return true;
        } else {
            return false;
        }

    }

    /* (non-Javadoc)
     * @see com.renren.sns.minisite.feed.xoa.utils.feedcache.aaa#removeUgcId(int)
     */
    @Override
    public boolean removeUgcId(final int userId, final int index) {
        return Resource.CACHE.delete(this.cacheIndex.buildIndexKey(userId, index));
    }

    public void setFeedsIndexDAO(final FeedsIndexDAO feedsIndexDAO){
        ((MinisiteFeedCacheIndex) this.cacheIndex).setFeedsIndexDAO(feedsIndexDAO);
    }

}
