package com.renren.sns.guide.dao;

import java.util.ArrayList;
import java.util.List;

import com.renren.sns.guide.score.AwardDetail;
import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.userscore.reward.RewardDetail;
import com.xiaonei.userscore.reward.RewardLogic;

/**
 * @author wei.xiang(wei.xiang@opi-corp.com)
 * @since 2010-3-21
 */
public class AwardDao {

    public enum AwardCountType {
        LEVEL_AWARD, //等级奖励
        CONTINUE_DAYS_AWARD, //连续登录奖励
        HISTORY_DAYS_AWARD
        //历史登录天数奖励
    }

    final static MemCacheFactory factory = MemCacheFactory.getInstance();

    final static MemCacheKeys key = MemCacheKeys.pool_userscore_value;

    static final String user_award_count = "user_score_award";

    static final String user_score_award = "user_score_award";

    static final String AWARD_COUNT_KEY = "aw";

    static RewardLogic rewardLogic = RewardLogic.getInstance();

    public static void clearAwardCount(final AwardCountType type, final int userId,
            final int realCount) {
    }

    /**
     * 从memcache中删除奖励计数，同时从db中减少一个奖励计数，并且置状态为为opened=1
     * 
     * @param userId 用户id
     * @param type 类型
     * @param id 奖励记录的id
     * @param awardKey 奖励验证码
     * @param cancel 用户是否主动取消领取
     * @return 是否成功
     */
    public static long decrCountInMemcacheAndDb(final int userId, final int typeId, final int id,
            final String awardKey, final boolean cancel) {
        return rewardLogic.consumeAward(userId, id, typeId, awardKey, cancel);
    }

    public static List<AwardDetail> getAwardDetails(final int userId, final boolean opened) {
        List<RewardDetail> list = rewardLogic.getAwardDetails(userId, opened);
        if (list == null) return null;
        List<AwardDetail> ret = new ArrayList<AwardDetail>(list.size());
        for (RewardDetail rd : list) {
            AwardDetail ad = new AwardDetail();
            ad.setId(rd.getId());
            ad.setUserId(rd.getUserId());
            ad.setLogDetail(rd.getLogDetail());
            ad.setOpened(rd.isOpened());
            ad.setType(rd.getType());
            ad.setUpdateTime(rd.getUpdateTime());
            ad.setAwardKey(rd.getAwardKey());
            ret.add(ad);
        }
        return ret;
    }
}
