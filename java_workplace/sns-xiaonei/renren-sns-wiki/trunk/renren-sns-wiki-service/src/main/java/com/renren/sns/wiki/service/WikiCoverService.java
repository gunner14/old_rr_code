package com.renren.sns.wiki.service;

import java.util.List;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.biz.WikiCoverBiz;
import com.renren.sns.wiki.exception.WikiException;
import com.renren.sns.wiki.exception.WikiExceptionType;
import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.model.WikiCover;

/**
 * 
 * @author weiwei.wang@renren-inc.com since 2012-4-10
 * 
 */
@Service
public class WikiCoverService extends BaseService {

    @Autowired
    private WikiCoverBiz wikiCoverBiz;

    /**
     * 根据WikiId获取WikiCover对象
     * 
     * @param wikiId
     * @return
     */
    public WikiCover getWikiCover(Wiki wiki) {
        WikiCover wikiCover = wikiCoverBiz.getWikiCoverById(wiki.getId());
        if (wikiCover == null) {
            int wikiType = wiki.getType();
            wikiCover = getDefaultWikiCover(wikiType);
        }
        return wikiCover;
    }

    public WikiCover getDefaultWikiCover(int wikiType) {
        String largeUrl = wikiCoverBiz.getDefaultLargeUrlByType(wikiType);
        WikiCover wikiCover = new WikiCover();
        wikiCover.setExtraOption("default");
        wikiCover.setLargeUrl(largeUrl);
        return wikiCover;
    }

    public Map<Integer, WikiCover> getWikiCoverMap(List<Integer> wikiIdList) {
        return wikiCoverBiz.getWikiCoverMap(wikiIdList);
    }

    /**
     * 修改wiki头图
     * 
     * @param wikiId
     * @param xlargeUrl
     * @param largeUrl
     * @param mainUrl
     * @param headUrl
     * @param tinyUrl
     * @param xlargeUrlWidth
     * @param xlargeUrlHeight
     * @return
     */
    public boolean updateWikiCover(int wikiId, String xlargeUrl, String largeUrl, String mainUrl,
            String headUrl, String tinyUrl, int xlargeUrlWidth, int xlargeUrlHeight) {
        WikiCover wikiCover = new WikiCover();

        wikiCover.setWikiId(wikiId);
        wikiCover.setXlargeUrl(xlargeUrl);
        wikiCover.setLargeUrl(largeUrl);
        wikiCover.setMainUrl(mainUrl);
        wikiCover.setHeadUrl(headUrl);
        wikiCover.setTinyUrl(tinyUrl);
        wikiCover.setXlargeUrlWidth(xlargeUrlWidth);
        wikiCover.setXlargeUrlHeight(xlargeUrlHeight);

        try {
            return wikiCoverBiz.saveOrUpdateWikiCover(wikiCover);
        } catch (Exception e) {
            logger.error("updateWikiCover error", e);
            throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, e);
        }
    }

    /**
     * 清空头图的缓存
     * 
     * @param wikiId
     */
    public void clearCache(int wikiId) {
        wikiCoverBiz.clearCache(wikiId);
    }

    /**
     * 删除头图,等于设置默认头图
     * 
     * @param wikiId
     * @return
     */
    public boolean deleteWikiCover(int wikiId) {
        boolean result = false;
        int updateCount = wikiCoverBiz.deleteWikiCover(wikiId);
        if (updateCount > 0) {
            clearCache(wikiId);
            result = true;
        }
        return result;
    }

}
