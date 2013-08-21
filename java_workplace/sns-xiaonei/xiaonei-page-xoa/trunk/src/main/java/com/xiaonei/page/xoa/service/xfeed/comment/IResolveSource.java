package com.xiaonei.page.xoa.service.xfeed.comment;

import java.util.Map;

/**
 * @author yi.li@renren-inc.com since 2011-10-14
 * 
 */
public interface IResolveSource {

    /**
     * 支持的新鲜事类型
     * 
     * @return
     */
    int supportedFeedType();

    /**
     * 将UGC实体替转换成数据
     * 
     * @param sourceId
     * @return
     * @throws Exception
     */
    Map<String, String> resolveSource(long sourceId, int ownerId) throws Exception;
}
