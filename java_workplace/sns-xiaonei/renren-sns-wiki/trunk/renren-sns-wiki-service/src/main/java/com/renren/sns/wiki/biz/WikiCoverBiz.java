package com.renren.sns.wiki.biz;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.dao.WikiCoverDAO;
import com.renren.sns.wiki.enums.WikiType;
import com.renren.sns.wiki.exception.WikiException;
import com.renren.sns.wiki.exception.WikiExceptionType;
import com.renren.sns.wiki.model.WikiCover;
import com.renren.sns.wiki.model.wrapper.WikiCoverWrapper;
import com.renren.sns.wiki.utils.cache.CacheDelegator;
import com.renren.sns.wiki.utils.cache.CacheNameSpace;
import com.renren.sns.wiki.utils.cache.CachePrefix;
import com.renren.sns.wiki.utils.cache.CacheDelegator.QueryAction;

/**
 * 
 * @author weiwei.wang@renren-inc.com since 2012-3-31
 * 
 */
@Service
public class WikiCoverBiz extends BaseBiz {

    /**
     * 将类型对应的默认cover头图保存到map,可以快速获取
     */
    private static Map<Integer, String> defaultCoverImageMap = new HashMap<Integer, String>();
    static {
        WikiType[] wikiTypes = WikiType.values();
        for (WikiType wikiType : wikiTypes) {
            defaultCoverImageMap.put(wikiType.getWikiType(), wikiType.getDefaultCoverImage());
        }
    }

    @Autowired
    private WikiCoverDAO wikiCoverDAO;

    /**
     * 根据Wiki Id 获取WIkiCover的信息
     * 
     * @param wikiId wiki ID
     * @return {@link WikiCover}
     */
    public WikiCover getWikiCoverById(final int wikiId) {

        WikiCoverWrapper wikiCoverWrapper = CacheDelegator.get(new QueryAction<WikiCoverWrapper>(
                CacheNameSpace.WIKI, CachePrefix.WIKI_COVER, WikiCoverWrapper.class, wikiId) {

            @Override
            public WikiCoverWrapper getFromOther() {
                WikiCover wikiCover = wikiCoverDAO.getWikiCoverById(wikiId);
                WikiCoverWrapper wikiCoverWrapper = new WikiCoverWrapper();
                wikiCoverWrapper.setWikiCover(wikiCover);
                if (wikiCover == null) {
                    wikiCoverWrapper.setHaveCover(false);
                } else {
                    wikiCoverWrapper.setHaveCover(true);
                }
                return wikiCoverWrapper;
            }
        }, null);

        if (wikiCoverWrapper == null) {
            logger.warn("No wiki found for wikiId " + wikiId);
            throw new WikiException(WikiExceptionType.WIKI_NOT_EXISTS);
        }

        return wikiCoverWrapper.getWikiCover();
    }

    /**
     * 更具wikiId列表获取wikiCoverMap
     * 
     * @param ids
     * @return
     */
    public Map<Integer, WikiCover> getWikiCoverMap(List<Integer> wikiIdList) {

        Map<Integer, WikiCover> wikiCoverMap = new HashMap<Integer, WikiCover>();

        if (wikiIdList == null || wikiIdList.size() == 0) {
            return wikiCoverMap;
        }
        Map<String, WikiCoverWrapper> map = CacheDelegator.getMulti(CacheNameSpace.WIKI,
                CachePrefix.WIKI_COVER, WikiCoverWrapper.class, wikiIdList);
        for (Integer wikiId : wikiIdList) {
            String strWikiId = wikiId.toString();
            WikiCoverWrapper wikiCoverWrapper = map.get(strWikiId);
            if (wikiCoverWrapper == null) {
                WikiCover wikiCover = getWikiCoverById(wikiId);
                wikiCoverMap.put(wikiId, wikiCover);
            } else {
                wikiCoverMap.put(wikiId, wikiCoverWrapper.getWikiCover());
            }
        }
        return wikiCoverMap;
    }

    /**
     * 根据wikiType得到默认wiki图的头图
     * 
     * @param wikiType
     * @return
     */
    public String getDefaultLargeUrlByType(int wikiType) {
        return defaultCoverImageMap.get(wikiType);
    }

    /**
     * 修改wikiCover
     * 
     * @param wikiCover
     * @return
     */
    public boolean saveOrUpdateWikiCover(WikiCover wikiCover) {
        Integer queryWikiId = wikiCoverDAO.getWikiIdByWikiId(wikiCover.getWikiId());
        int updateCount = 0;
        if (queryWikiId == null) {
            updateCount = wikiCoverDAO.addWikiCover(wikiCover);
        } else {
            updateCount = wikiCoverDAO.updateWikiCover(wikiCover);
        }
        if (updateCount > 0) {
            //清空一下wikiCover的缓存
            clearCache(wikiCover.getWikiId());
            return true;
        } else {
            return false;
        }
    }

    /**
     * 清空缓存
     * 
     * @param wikiId
     */
    public void clearCache(int wikiId) {
        CacheDelegator.delete(CacheNameSpace.WIKI, CachePrefix.WIKI_COVER, WikiCoverWrapper.class,
                wikiId);
    }

    /**
     * 删除头图
     * 
     * @param wikiId
     * @return
     */
    public int deleteWikiCover(int wikiId) {
        return wikiCoverDAO.deleteWikiCoverByWikiId(wikiId);
    }

}
