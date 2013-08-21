package com.renren.sns.wiki.service.rank;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.biz.rank.UserScoreBiz;
import com.renren.sns.wiki.biz.rank.WikiRankBiz;
import com.renren.sns.wiki.exception.WikiException;
import com.renren.sns.wiki.exception.WikiExceptionType;
import com.renren.sns.wiki.model.rank.UserScore;
import com.renren.sns.wiki.model.rank.WikiRankRecord;
import com.renren.sns.wiki.service.BaseService;

/**
 * 用户积分服务
 * 
 * @author yi.li@renren-inc.com since 2012-5-21
 * 
 */
@Service
public class UserRankService extends BaseService {

    @Autowired
    private UserScoreBiz userScoreBiz;

    @Autowired
    private WikiRankBiz wikiRankBiz;

    /**
     * 增加用户积分
     * 
     * @param userId
     * @param wikiId
     * @param incrScore
     */
    public void incrUserScore(int userId, int wikiId, int incrScore) {
        try {
            // TODO 是否需要加限制，一个用户的每日积分增长应该存在上限
            logger.info(String.format("Increase user's score. userId-%d wikiId-%d incrScore-%d",
                    userId, wikiId, incrScore));
            int originScore = userScoreBiz.incrUserScore(userId, wikiId, incrScore);
            wikiRankBiz.updateUserRank(userId, wikiId, originScore, incrScore);
        } catch (Exception e) {
            logger.error("update user score error. ", e);
            throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION);
        }
    }

    /**
     * 获取用户在对应wiki上的积分排名。当没有找到积分所在的排名时抛出WikiException异常
     * 
     * @param wikiId wiki id
     * @param userId user id
     * @return 返回用户所在的排名，如果用户在对应的wiki上没有积分那么返回-1， 如果用户有积分但是却查询不到排名会抛出异常
     * @throws WikiException
     */
    public int getUserRankByWiki(int wikiId, int userId) throws WikiException {
        UserScore score = userScoreBiz.getUserSocre(userId, wikiId);
        if (null == score) {
            return -1;
        }

        WikiRankRecord rank = wikiRankBiz.getWikiRankByScore(wikiId, score.getScore());
        if (null == rank) {
            logger.error(String.format(
                    "Query wiki rank. Does not found userId-%s score-%s wikiId-%s", userId,
                    score.getScore(), wikiId));
            throw new WikiException(WikiExceptionType.WIKI_USERRANK_NOT_FOUND);
        }
        return rank.getRank();
    }
}
