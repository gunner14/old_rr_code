package com.renren.sns.wiki.service;

import java.text.DecimalFormat;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.biz.UserActionRecordBiz;
import com.renren.sns.wiki.biz.WikiBiz;
import com.renren.sns.wiki.enums.WikiStateType;
import com.renren.sns.wiki.exception.WikiException;
import com.renren.sns.wiki.exception.WikiExceptionType;
import com.renren.sns.wiki.model.Wiki;

/**
 * @author yi.li@renren-inc.com since 2012-3-29
 * 
 */
@Service
public class WikiService extends BaseService {

    @Autowired
    private WikiBiz wikiBiz;

    @Autowired
    private UserActionRecordBiz userActionRecordBiz;

    /**
     * 根据WikiId获取Wiki对象
     * 
     * @param wikiId
     * @return wiki if exists, or return null
     */
    public Wiki getWikiById(final int wikiId) {
        try {
            return wikiBiz.getWikiById(wikiId);
        } catch (Exception e) {
            logger.error("getWiki by Id error", e);
            throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, e);
        }
    }

    /**
     * 根据wikiName得到wikiId的列表
     * 
     * @param wikiName
     * @return
     */
    public List<Integer> getWikiIdListByName(String wikiName) {
        try {
            return wikiBiz.getWikiIdListByName(wikiName);
        } catch (Exception e) {
            logger.error("Get wikiId list by name error", e);
            throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, e);
        }
    }

    /**
     * 更新wiki的名字
     * 
     * @param wikiId
     * @param wikiName
     */
    public void updateWikiName(int wikiId, String wikiName) {
        try {
            wikiBiz.updateName(wikiId, wikiName);
        } catch (Exception e) {
            logger.error("updateWikiName error", e);
            throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, e);
        }
    }

    /**
     * 更新Wiki的状态
     * 
     * @param wikiId
     * @param state
     */
    public int updateWikiState(int wikiId, WikiStateType state) {
        try {
            logger.warn(String.format("Update wiki state wikiId-%d state-%d desc-%s", wikiId, state
                    .getState(), state.getDescription()));
            return wikiBiz.updateWikiState(wikiId, state);
        } catch (Exception e) {
            logger.error("updateWikiState error", e);
            throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, e);
        }
    }

    /**
     * 清空wiki的缓存
     * 
     * @param wikiId
     */
    public void clearCache(int wikiId) {
        wikiBiz.clearCache(wikiId);
    }

    /**
     * 根据wikiName获取wiki列表
     * 
     * @param name
     * @return
     */
    public List<Wiki> getWikiListByName(String wikiName) {
        return wikiBiz.getWikiListByName(wikiName);
    }

    /**
     * 从数据库中分页获取wikiList
     * 
     * @param light 是否高亮
     * @param wikiType 过滤类型
     * @param curPage
     * @param endId
     * @param page
     * @param pageSize
     * @param string
     * @return
     */
    public List<Wiki> getWikiList(boolean light, int type, int curPage, int endId, int page,
            int pageSize, String order) {

        //默认升序
        if (order == null) {
            order = "asc";
        }

        int flag = 1;
        if (page <= curPage) {
            flag *= -1;
        }
        if (order.equals("desc")) {
            flag *= -1;
        }

        //查询条件是否包含等号
        String eq = "";
        if (page <= curPage) {
            eq = "=";
        }
        int begin = 0; //起始值
        int limit = pageSize;
        if (page > curPage) {
            begin = (page - curPage - 1) * pageSize;
        } else {
            begin = (curPage - page) * pageSize;
        }

        StringBuilder sb = new StringBuilder(" where 1=1 ");
        if (light) {
            sb.append(" and d.wiki_id is not null ");
        }
        if (type >= 1 && type <= 4) {
            sb.append(" and w.type = " + type);
        }
        if (endId > 0) {
            if (flag > 0) {
                //where 大于 id>endId order by id asc limit 
                sb.append(" and w.id >").append(eq).append(" ").append(endId);
                sb.append(" order by w.id asc limit ").append(begin).append(",").append(limit);
            } else {
                sb.append(" and w.id <").append(eq).append(" ").append(endId);
                sb.append(" order by w.id desc limit ").append(begin).append(",").append(limit);
            }
        } else {
            sb.append(" order by w.id " + order + " limit ").append(begin).append(",")
                    .append(limit);
        }
        if (logger.isDebugEnabled()) {
            logger.debug("condition:" + sb.toString());
        }
        List<Wiki> wikiList = wikiBiz.getWikiListByCondition(sb.toString());

        if (logger.isDebugEnabled()) {
            if (wikiList != null) {
                logger.debug("wikiWordList.size()=" + wikiList.size());
            } else {
                logger.debug("wikiWordList is null");
            }
        }

        //page<=curPage时需要倒序
        if (endId > 0 && page <= curPage && wikiList != null && wikiList.size() > 0) {
            Collections.reverse(wikiList);
        }
        return wikiList;

    }

    /**
     * 更新下某个wiki的评分更新到最新
     * 
     * @param userActionRecordService TODO
     * @param wikiId
     */
    public void updateWikiValuation(int wikiId) {
        try {
            //评分有所修改,计算平均分
            int totalRating = userActionRecordBiz.getTotalRating(wikiId);
            int ratingCount = userActionRecordBiz.getRatingCount(wikiId);
            if (logger.isDebugEnabled()) {
                logger.debug("wikiId=" + wikiId + ", 总评分=" + totalRating + ", 评分人数=" + ratingCount);
            }
            wikiBiz.updateValuation(wikiId, totalRating, ratingCount);
        } catch (Exception e) {
            logger.error("更新wiki评分异常", e);
            throw new WikiException(WikiExceptionType.WIKI_UPDATE_VALUATION_EXCEPTION, e);
        }
    }

    /**
     * 根据评分的差值更改总分
     * 
     * @param wikiId
     * @param changeRating
     * @param changeUserCount
     */
    public void updateWikiValuationByChange(int wikiId, int changeRating, int changeUserCount) {
        try {
            wikiBiz.updateWikiValuationByChange(wikiId, changeRating, changeUserCount);
            if (logger.isDebugEnabled()) {
                logger.debug("wikiId=" + wikiId + ", 分数差值=" + changeRating + ", 评分人数差值="
                        + changeUserCount);
            }
        } catch (Exception e) {
            logger.error("更新wiki评分异常", e);
            throw new WikiException(WikiExceptionType.WIKI_UPDATE_VALUATION_EXCEPTION, e);
        }

    }

    /**
     * 获取wiki的平均评分,保留一位小数
     * 
     * @param wiki
     * @return
     */
    public String getValuationDisplayChars(int valuationScore, int valuationCount) {
        String result = "0";
        if (valuationScore == 0 || valuationCount == 0) {
            return result;
        }
        double evaluationDisplay = 2.0 * valuationScore / valuationCount;
        DecimalFormat df = new DecimalFormat("#.0");
        return df.format(evaluationDisplay);
    }

    /**
     * 获取wiki的平均评分,保留一位小数
     * 
     * @param wiki
     * @return
     */
    public String getValuationDisplayChars(Wiki wiki) {
        String result = "0";
        if (wiki == null) {
            return result;
        }
        int valuationScore = wiki.getValuationScore();
        int valuationCount = wiki.getValuationCount();
        //分数无效时重新计算
        if (valuationCount * 5 < valuationScore || valuationCount > valuationScore) {
            this.updateWikiValuation(wiki.getId());
        }
        return this.getValuationDisplayChars(valuationScore, valuationCount);
    }

    /**
     * 获取wiki的评分信息的Map对象
     * 
     * @param wiki
     */
    public Map<String, Object> getWikiValuationMap(Wiki wiki) {
        if (wiki == null) {
            logger.warn("getWikiValuationMap(Wiki wiki) param wiki is null");
            throw new WikiException(WikiExceptionType.WIKI_NOT_EXISTS, "wiki对象为空");
        }
        Map<String, Object> map = new HashMap<String, Object>();

        map.put("valuationScore", wiki.getValuationScore());
        map.put("valuationCount", wiki.getValuationCount());
        map.put("valuationDisplayChars", this.getValuationDisplayChars(wiki));

        return map;
    }

    /**
     * wiki的评分信息
     * 
     * @param wikiId
     * @return
     */
    public Map<String, Object> getWikiValuationMap(int wikiId) {
        Wiki wiki = wikiBiz.getWikiById(wikiId);
        return this.getWikiValuationMap(wiki);
    }

    /**
     * 将wiki的基本信息封装成简单的map形式,用于组装json
     * @param wiki
     * @return
     */
    public Map<String, Object> getWikiMap(Wiki wiki) {
        Map<String, Object> map = new HashMap<String, Object>();
        map.put("type", wiki.getType());
        map.put("id", wiki.getId());
        map.put("name", wiki.getName());
        map.put("tinyUrl", wiki.getTinyUrl());
        map.put("headUrl", wiki.getHeadUrl());
        map.put("mainUrl", wiki.getMainUrl());
        map.put("valuation", this.getWikiValuationMap(wiki));
        return map;
    }

}
