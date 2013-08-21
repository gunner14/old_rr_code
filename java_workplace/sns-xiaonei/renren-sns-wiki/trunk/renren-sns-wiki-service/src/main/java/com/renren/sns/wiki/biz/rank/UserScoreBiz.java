package com.renren.sns.wiki.biz.rank;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.dao.rank.UserScoreDAO;
import com.renren.sns.wiki.model.rank.UserScore;

/**
 * 用户积分的相关的业务
 * 
 * @author yi.li@renren-inc.com since 2012-5-15
 * 
 */
@Service
public class UserScoreBiz {

    @Autowired
    private UserScoreDAO userScoreDAO;

    /**
     * 增加用户在某个wiki上的积分，并返回用户增加积分前的积分（注意：该实现非原子操作）
     * 
     * @param userId 用户ID
     * @param wikiId wiki ID
     * @param incrScore
     * @return 返回用户积分增加前的积分
     */
    public int incrUserScore(int userId, int wikiId, int incrScore) {
        UserScore score = userScoreDAO.getUserScore(userId, wikiId);
        if (null == score) {
            userScoreDAO.insertUserScore(userId, wikiId, incrScore);
            return 0;
        }

        userScoreDAO.updateUserScoreIncrementally(userId, wikiId, incrScore);
        return score.getScore();
    }

    /**
     * 获取用户在对应wiki上的积分数据
     * 
     * @param userId user id
     * @param wikiId wiki id
     * @return {@link UserScore}，如果用户没有积分返回空对象。
     */
    public UserScore getUserSocre(int userId, int wikiId) {
        return userScoreDAO.getUserScore(userId, wikiId);
    }

}
