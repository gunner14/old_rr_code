package com.renren.sns.wiki.stat.service.fingerprint;

import java.util.Map;

/**
 * 去重算法
 * 
 * @author yi.li@renren-inc.com since 2012-7-19
 * 
 */
public interface IdentityStrategy {

    /**
     * 支持的新鲜事类型
     * 
     * @param feedType feedType
     * @return ture if support otherwise false
     */
    public boolean supportFeedType(int feedType);

    /**
     * 判断是否是已经存在的内容
     * 
     * @param wikiId wiki id即验证的范围
     * @param feedType feed type
     * @param feed 需要验证的文本内容
     * @return true if does exist otherwise false
     */
    public boolean isDuplicate(int wikiId, int feedType, Map<String, Object> feed);

    /**
     * @return 返回策略名
     */
    public String getStrategyName();
}
