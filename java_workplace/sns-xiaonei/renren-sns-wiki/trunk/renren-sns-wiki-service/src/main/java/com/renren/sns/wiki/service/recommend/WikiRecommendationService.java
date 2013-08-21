package com.renren.sns.wiki.service.recommend;

import java.util.Collections;
import java.util.List;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.biz.WikiBiz;
import com.renren.sns.wiki.biz.recommend.WikiRecommedationBiz;
import com.renren.sns.wiki.exception.WikiException;
import com.renren.sns.wiki.exception.WikiExceptionType;
import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.service.BaseService;

/**
 * Wiki推荐服务
 * 
 * @author yi.li@renren-inc.com since 2012-5-14
 * 
 */
@Service
public class WikiRecommendationService extends BaseService {

    @Autowired
    private WikiRecommedationBiz wikiRecommendationBiz;

    @Autowired
    private WikiBiz wikiBiz;

    /**
     * 根据Wiki Id获取推荐的wiki列表
     * 
     * @param wikiId
     * @return List<Wiki>如果没有推荐列表，则返回大小为0的List对象
     */
    public List<Wiki> getRecommendedWikiList(int wikiId) {
        try {
            List<Integer> ids = wikiRecommendationBiz.getRecommendation(wikiId);
            if (ids.size() == 0) {
                return Collections.emptyList();
            }

            List<Wiki> result = wikiBiz.getWikiListFromCache(ids);
            if (null == result) {
                return Collections.emptyList();
            }

            return result;
        } catch (Exception e) {
            logger.error("query recommended wiki error, wikiId-" + wikiId, e);
            throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, e);
        }
    }

}
