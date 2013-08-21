package com.renren.sns.wiki.biz.rank;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.biz.BaseBiz;
import com.renren.sns.wiki.dao.rank.WikiRankDAO;
import com.renren.sns.wiki.model.rank.WikiRankRecord;

/**
 * wiki积分排名相关的业务
 * 
 * @author yi.li@renren-inc.com since 2012-5-15
 * 
 */
@Service
public class WikiRankBiz extends BaseBiz {

    @Autowired
    private WikiRankDAO userRankDAO;

    /**
     * 更新wiki的排名
     * 
     * @param userId 用户ID
     * @param wikiId wiki ID
     * @param startScore 用户积分变更前的积分
     * @param incrOffset 用户增长的积分
     */
    public void updateUserRank(int userId, int wikiId, int startScore, int incrOffset) {

        updateStartScore(wikiId, startScore, incrOffset);

        updateTargetScore(wikiId, startScore, incrOffset);

        // rank还是放在后面好，targe的结果可能利用到range中的数据，主从库的延迟可能导致target的数据不准
        updateRangeRank(wikiId, startScore, incrOffset);
    }

    private void updateStartScore(int wikiId, int startScore, int incrOffset) {
        WikiRankRecord startRecord = getWikiScore(wikiId, startScore);
        if (null == startRecord) {
            logger.warn("update start score, not rank record found. wikiId-" + wikiId
                    + " startScore-" + startScore + " incrOffset-" + incrOffset);
            return;
        }

        if (startRecord.getCapacity() <= 1) {
            // delete rank record
            userRankDAO.removeRank(wikiId, startScore);
        } else {
            if (incrOffset < 0) {
                userRankDAO.updateRankAndCapacity(wikiId, startScore, 0, -1);
            } else {
                userRankDAO.updateRankAndCapacity(wikiId, startScore, 1, -1);
            }
        }
    }

    // 考虑减分逻辑
    private void updateTargetScore(int wikiId, int startScore, int incrOffset) {
        int score = startScore + incrOffset;

        WikiRankRecord targetRecord = getWikiScore(wikiId, score);
        if (null == targetRecord) {
            initScoreRank(wikiId, startScore, incrOffset);
            return;
        }

        if (incrOffset < 0) { //减分逻辑
            userRankDAO.updateRankAndCapacity(wikiId, score, -1, 1);
        } else {
            userRankDAO.updateRankAndCapacity(wikiId, score, 0, 1);
        }
    }

    private void updateRangeRank(int wikiId, int startScore, int incrOffset) {
        int incrRank = 1;
        if (incrOffset < 0) {
            incrRank = -1;
        }

        int start;
        int end;
        if (incrOffset < 0) {
            start = startScore + incrOffset;
            end = startScore;
        } else {
            start = startScore;
            end = startScore + incrOffset;
        }
        userRankDAO.updateRankByScoreRange(wikiId, start, end, incrRank);
    }

    // 考虑减分逻辑
    private void initScoreRank(int wikiId, int startScore, int incrOffset) {
        logger.info("initial new score rank. wikiId-" + wikiId + " startscore-" + startScore
                + " incrscore-" + incrOffset);

        int score = startScore + incrOffset;
        WikiRankRecord preRankRecord = userRankDAO.getPreRank(wikiId, score);
        if (null == preRankRecord) {
            userRankDAO.insertRank(wikiId, score, 1, 1);
            return;
        }

        // 这个地方之所以要加一层这个逻辑而不是先更新range中的数据然后这个地方采用统一的策略
        // 主要是考虑了主从库延时可能导致影响这里的准确率的问题
        if (incrOffset < 0 && preRankRecord.getScore() != startScore) {
            userRankDAO.insertRank(wikiId, score,
                    preRankRecord.getCapacity() + preRankRecord.getRank() - 1, 1);
        } else {
            userRankDAO.insertRank(wikiId, score,
                    preRankRecord.getCapacity() + preRankRecord.getRank(), 1);
        }
    }

    private WikiRankRecord getWikiScore(int wikiId, int score) {
        return userRankDAO.getRankByScore(wikiId, score);
    }

    /**
     * 根据积分获取该积分在对应wiki上所在的排名
     * 
     * @param wikiId wiki id
     * @param score 积分
     * @return {@link WikiRankRecord}, 如果该积分没有对应的排名则返回null
     */
    public WikiRankRecord getWikiRankByScore(int wikiId, int score) {
        return getWikiScore(wikiId, score);
    }

}
