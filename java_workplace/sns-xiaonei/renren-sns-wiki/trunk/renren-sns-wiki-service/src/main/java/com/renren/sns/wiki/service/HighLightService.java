package com.renren.sns.wiki.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.biz.HighLightBiz;
import com.renren.sns.wiki.exception.WikiException;
import com.renren.sns.wiki.exception.WikiExceptionType;
import com.renren.sns.wiki.model.HighLightSummary;

/**
 * @author yi.li@renren-inc.com since 2012-5-2
 * 
 */
@Service
public class HighLightService extends BaseService {

    @Autowired
    private HighLightBiz highLightBiz;

    /**
     * 根据wiki id获取wiki照片highlight信息
     * 
     * @param wikiId
     * @return {@link HighLightSummary}
     */
    public HighLightSummary getPhotoHighLightSummary(final int wikiId) {
        try {

            return highLightBiz.getPhotoHighLight(wikiId);
        } catch (Exception e) {
            logger.error("", e);
            throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, e);
        }
    }

    /**
     * 根据wiki id获取wiki视频highlight信息
     * 
     * @param wikiId
     * @return {@link HighLightSummary}
     */
    public HighLightSummary getVideoHighLightSummary(final int wikiId) {
        try {
            return highLightBiz.getVideoHighLight(wikiId);
        } catch (Exception e) {
            logger.error("", e);
            throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, e);
        }
    }

    public void cleanCache(int wikiId) {
        try {
            highLightBiz.cleanHightCache(wikiId);
        } catch (Exception e) {
            logger.error("clean highlight cache error, wikiId-" + wikiId, e);
            throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, e);
        }
    }
}
