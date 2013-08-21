package com.renren.sns.minisite.feed.xoa.components.activeuser;

import java.util.List;

import com.renren.sns.minisite.feed.xoa.components.activeuser.model.UserAct;


public interface ActiveUserService {

    public UserAct getActiveFriends(int siteId);

    public List<Integer> recentLoginIds(List<Integer> userIds);
}
