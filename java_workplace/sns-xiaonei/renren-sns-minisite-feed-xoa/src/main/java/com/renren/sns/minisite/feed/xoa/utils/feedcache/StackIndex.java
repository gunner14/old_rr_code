package com.renren.sns.minisite.feed.xoa.utils.feedcache;


public interface StackIndex {

    String buildIndexKey(final int userId, long index);

    String getIndex(final int userId);

    String[] getIndexs(final int userId, int start, int count);

    String getNewIndex(final int userId);

    void incr(final int userId, long i);

}
