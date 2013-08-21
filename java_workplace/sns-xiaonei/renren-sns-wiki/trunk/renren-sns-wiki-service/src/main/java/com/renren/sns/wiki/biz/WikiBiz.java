package com.renren.sns.wiki.biz;

import java.util.Collections;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.dao.SequenceDAO;
import com.renren.sns.wiki.dao.WikiDAO;
import com.renren.sns.wiki.enums.WikiStateType;
import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.utils.cache.CacheDelegator;
import com.renren.sns.wiki.utils.cache.CacheNameSpace;
import com.renren.sns.wiki.utils.cache.CachePrefix;
import com.renren.sns.wiki.utils.cache.CacheDelegator.MultiQueryAction;
import com.renren.sns.wiki.utils.cache.CacheDelegator.QueryAction;

/**
 * Wiki Service 的实现
 * 
 * @author yi.li@renren-inc.com since 2012-3-26
 * 
 */
@Service
public class WikiBiz extends BaseBiz {

    @Autowired
    private WikiDAO wikiDAO;

    @Autowired
    private SequenceDAO sequenceDAO;

    /**
     * 根据Wiki Id 获取WIki的信息（带缓存）
     * 
     * @param wikiId wiki ID
     * @return {@link Wiki}
     */
    public Wiki getWikiById(final int wikiId) {
        Wiki wiki = CacheDelegator.get(new QueryAction<Wiki>(CacheNameSpace.WIKI, CachePrefix.WIKI,
                Wiki.class, wikiId) {

            @Override
            public Wiki getFromOther() {
                return wikiDAO.getWikiById(wikiId);
            }
        }, null);

        return wiki;
    }

    /**
     * 根据wikiName得到wikiId的列表
     * 
     * @param wikiName
     * @return
     */
    public List<Integer> getWikiIdListByName(String wikiName) {
        return wikiDAO.getWikiIdListByName(wikiName);
    }

    /**
     * 根据wikiName和wikiType得到wikiId的列表
     * 
     * @param wikiName
     * @return
     */
    public List<Integer> getWikiIdList(String wikiName, int wikiType) {
        return wikiDAO.getWikiIdList(wikiName, wikiType);
    }

    /**
     * 创建Wiki
     * 
     * @param wiki
     * @return wikiId If success
     */
    public int createWiki(Wiki wiki) {
        int wikiId = sequenceDAO.getWikiNextId();
        wiki.setId(wikiId);
        if (null == wiki.getAlias()) {
            wiki.setAlias("");
        }
        wikiDAO.addWiki(wiki);
        return wikiId;
    }

    /**
     * 修改cookie的头像
     * 
     * @param wikiId
     * @param tinyUrl
     * @param headUrl
     * @param mainUrl
     * @param largeUrl
     */
    public boolean updateWikiHead(int wikiId, String tinyUrl, String headUrl, String mainUrl,
            String largeUrl) {
        int updateCount = wikiDAO.updateWikiHead(wikiId, tinyUrl, headUrl, mainUrl, largeUrl);
        if (updateCount > 0) {
            //清空一下wiki的缓存
            CacheDelegator.delete(CacheNameSpace.WIKI, CachePrefix.WIKI, Wiki.class, wikiId);
            return true;
        } else {
            return false;
        }
    }

    /**
     * 修改wiki的评分总分和评分人数
     * 
     * @param wikiId
     * @param totlaRating
     * @param ratingCount
     */
    public int updateValuation(int wikiId, int valuationScore, int valuationCount) {
        int updateCount = wikiDAO.updateValuation(wikiId, valuationScore, valuationCount);
        if (updateCount > 0) {
            //清空一下wiki的缓存
            clearCache(wikiId);
        }
        return updateCount;
    }

    /**
     * 根据差值,更改总分和评分总人数 update wiki set valuation_score=valuation_score,
     * valuation_count=:valuationCount where id=:wikiId
     * 
     * @param wikiId
     */
    public int updateWikiValuationByChange(int wikiId, int changeRating, int changeUserCount) {
        String strChaneRating = (changeRating >= 0 ? "+" : "") + changeRating;
        String strChangeUserCount = (changeUserCount >= 0 ? "+" : "") + changeUserCount;

        int updateCount = wikiDAO.updateValuationByChange(wikiId, strChaneRating,
                strChangeUserCount);
        if (updateCount > 0) {
            //清空一下wiki的缓存
            clearCache(wikiId);
        }
        return updateCount;
    }

    /**
     * 修改wiki的名字
     * 
     * @param wikiId
     * @param name
     */
    public void updateName(int wikiId, String name) {
        wikiDAO.updateWikiName(wikiId, name);
        clearCache(wikiId);
    }

    /**
     * 修改Wiki的状态
     * 
     * @param wikiId wiki id
     * @param wikiState wiki的状态
     */
    public int updateWikiState(int wikiId, WikiStateType wikiState) {
        int updateCount = wikiDAO.updateWikiState(wikiId, wikiState.getState());
        clearCache(wikiId);
        return updateCount;
    }

    /**
     * 清空缓存
     * 
     * @param wikiId
     */
    public void clearCache(int wikiId) {
        CacheDelegator.delete(CacheNameSpace.WIKI, CachePrefix.WIKI, Wiki.class, wikiId);
    }

    /**
     * 根据据wikiName获取wiki列表,此方法直接读取DB,不宜大量使用
     * 
     * @param wikiName
     * @return
     */
    public List<Wiki> getWikiListByName(String wikiName) {
        List<Wiki> wikiList = wikiDAO.getWikiListByName(wikiName);
        if (logger.isDebugEnabled()) {
            logger.debug("getWikiListByName() wikiName=" + wikiName + ", wikiList.size()="
                    + wikiList.size());
        }
        return wikiList;
    }

    /**
     * 根据id获取多个Wiki的信息（带缓存）
     * 
     * @param ids Wiki ID列表
     * @return List<Wiki>
     */
    public List<Wiki> getWikiListFromCache(List<Integer> ids) {
        List<Wiki> result = CacheDelegator.getMultiAsList(new MultiQueryAction<Integer, Wiki>(
                CacheNameSpace.WIKI, CachePrefix.WIKI, Wiki.class, ids) {

            @Override
            public Wiki getFromOther(Integer id) {
                return wikiDAO.getWikiById(id);
            }
        });

        if (logger.isDebugEnabled()) {
            logger.debug("query wiki list from cache wiki list: " + ids.toString() + " result: "
                    + result.toString());
        }

        return result;
    }

    public List<Wiki> getWikiListByCondition(String condition) {
        return wikiDAO.getWikiListByCondition(condition);
    }

    /**
     * 根据起始位置和获取的最大记录条数获取wiki数据
     * 
     * @param begin
     * @param limit
     * @return List<Wiki>
     */
    public List<Wiki> getWikiListByLimit(int begin, int limit) {
        List<Wiki> wikiList = wikiDAO.getWikiList(begin, limit);
        if (null == wikiList) {
            return Collections.emptyList();
        }

        return wikiList;
    }

}
