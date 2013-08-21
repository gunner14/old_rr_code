package com.xiaonei.reg.register.logic.additional;

import com.xiaonei.platform.component.feed.helper.FeedDefinition;
import com.xiaonei.platform.component.feed.publish.FeedBuilder;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.xce.log.TimeCost;

/**
 * 负责注册时的news feed的分发
 *
 * @author wang-tai
 */
public class RegNewsFeedLogic {
    private static RegNewsFeedLogic _instance = new RegNewsFeedLogic();

    public static RegNewsFeedLogic getInstance() {
        return _instance;
    }

    private RegNewsFeedLogic() {
    }

    /**
     * 给邀请者发送newsfeed
     *
     * @param inviter 邀请人
     * @param invitee 被邀请人（注册人）
     */
    public void sendNewsFeed2Inviter(User inviter, User invitee) {
        TimeCost cost = TimeCost.begin("RegNewsFeedLogic.sendNewsFeed2Inviter");

        // 告诉大家我来了
        try {
            FeedBuilder builder = FeedBuilder.getBuilder(FeedDefinition.FRIEND_INVITE_REG, inviter.getId(), inviter.getName());
            cost.endStep("1");
            builder.put("uName", invitee.getName());// 被加的好友
            builder.put("uID", invitee.getId());
            builder.publish();
            cost.endStep("2");
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            cost.endFinally();
		}
	}
}
