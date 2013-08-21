package com.renren.sns.wiki.biz.recommend;

import java.util.Collections;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.biz.BaseBiz;
import com.renren.sns.wiki.dao.recommend.WikiRecommendationDAO;
import com.renren.sns.wiki.utils.cache.CacheDelegator;
import com.renren.sns.wiki.utils.cache.CacheNameSpace;
import com.renren.sns.wiki.utils.cache.CachePrefix;
import com.renren.sns.wiki.utils.cache.CacheDelegator.QueryAction;

/**
 * Wiki推荐服务。主要用于
 * 
 * @author yi.li@renren-inc.com since 2012-5-14
 * 
 */
@Service
public class WikiRecommedationBiz extends BaseBiz {

    @Autowired
    private WikiRecommendationDAO wikiRecommendationDAO;

    /**
     * 根据wiki获取与该wiki相关的wiki列表（带缓存）
     * 
     * @param wikiId wiki ID
     * @return 返回推荐wiki的id列表
     */
    @SuppressWarnings({ "unchecked", "rawtypes" })
    public List<Integer> getRecommendation(final int wikiId) {
        List<Integer> result = (List<Integer>) CacheDelegator.get(new QueryAction<List>(
                CacheNameSpace.WIKI, CachePrefix.WIKI_RECOMMENDATION, List.class, wikiId) {

            @Override
            public List getFromOther() {
                return wikiRecommendationDAO.getRecommendation(wikiId);
            }
        }, Collections.emptyList());

        return result;
    }
}
