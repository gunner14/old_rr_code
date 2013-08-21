package com.renren.sns.minisite.feed.xoa.components.activeuser;

import java.util.ArrayList;
import java.util.Date;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.concurrent.TimeUnit;

import org.apache.commons.collections.CollectionUtils;
import org.apache.commons.collections.ListUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.renren.sns.minisite.feed.xoa.components.activeuser.model.UserAct;
import com.renren.sns.minisite.feed.xoa.dao.MinisiteFollowerDAO;
import com.xiaonei.platform.core.model.UserTime;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

@Component("xceCacheActiveUserService")
public class XceCacheActiveUserService implements ActiveUserService {

    @Autowired
    private MinisiteFollowerDAO followerDAO;

    protected Log logger = LogFactory.getLog(this.getClass());

    @Override
    public UserAct getActiveFriends(final int siteId) {
        if (this.logger.isDebugEnabled()) {
            this.logger.debug("XceCacheActiveUserService.getActiveFriends():" + siteId);
        }
        final List<Integer> followerids = this.followerDAO.getFollowerIdsInSite(siteId);
        //edit by zwang, 2011/08/03
        final List<Integer> siteOwnerids = this.followerDAO.getSiteOwnerIdsBySiteId(siteId);
        followerids.removeAll(siteOwnerids);
        followerids.addAll(siteOwnerids);
        
        if (this.logger.isDebugEnabled()) {
        	//edit by zwang,2011/07/27
       		this.logger.debug("XceCacheActiveUserService.getActiveFriends():"
                    + (CollectionUtils.isNotEmpty(followerids)? followerids:"no follower") + "|" + siteId);
        }
        if (CollectionUtils.isEmpty(followerids)) {
            return new UserAct();
        }
        final List<Integer> recentIds = this.recentLoginIds(followerids);
        @SuppressWarnings("unchecked")
        final List<Integer> coldids = ListUtils.removeAll(followerids, recentIds);
        final UserAct userAct = new UserAct();
        userAct.setActiveIds(recentIds);
        userAct.setUnActiveIds(coldids);
        return userAct;
    }

    @Override
    public List<Integer> recentLoginIds(final List<Integer> userIds) {
        final Date lastMonth = new Date(System.currentTimeMillis() - TimeUnit.DAYS.toMillis(30));
        final List<Integer> recentIds = new ArrayList<Integer>();
        for (final Integer userId : userIds) {
            final Date lastLoginTime = this.getLoginTime(userId);
            if ((lastLoginTime != null) && lastLoginTime.after(lastMonth)) {
                recentIds.add(userId);
            }
        }
        if (this.logger.isDebugEnabled()) {
            this.logger.debug("XceCacheActiveUserService.recentLoginIds():"
                    + CollectionUtils.isNotEmpty(recentIds));
        }
        return recentIds;
    }

    private Date getLoginTime(final int userId) {
        Date lastLoginTime;
        final Long ltime = Resource.CACHE.get(Resource.KEY + userId);
        if (ltime != null) {
            if (this.logger.isTraceEnabled()) {
                this.logger.trace("XceCacheActiveUserService.getLoginTime():time in cache");
            }
            lastLoginTime = new Date(ltime);
        } else {
            final UserTime usertime = SnsAdapterFactory.getUserTimeAdapter().getUserTime(userId);
            Resource.CACHE.set(Resource.KEY + userId, usertime.getLastLoginTime().getTime(),
                    (int) TimeUnit.DAYS.toSeconds(1));
            lastLoginTime = usertime.getLastLoginTime();
        }
        if (this.logger.isTraceEnabled()) {
            this.logger.trace("XceCacheActiveUserService.getLoginTime():" + lastLoginTime + "|"
                    + userId);
        }
        return lastLoginTime;
    }

}
