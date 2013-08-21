package com.renren.sns.wiki.controllers.ajax;

import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.wiki.exception.WikiException;
import com.renren.sns.wiki.service.FollowService;
import com.renren.sns.wiki.utils.remotelog.RemoteLogType;
import com.renren.sns.wiki.utils.remotelog.annotation.RemoteLog;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.commons.interceptors.access.annotation.ValidateRequestToken;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.rest.Post;

/**
 * WIKI Follow控制器
 * 
 * @author yi.li@renren-inc.com since 2012-4-5
 * 
 */
@LoginRequired
public class FollowController extends BaseAjaxController {

    @Autowired
    private FollowService followService;

    @Autowired
    private HostHolder hostHolder;

    /**
     * @see ValidateRequestToken
     */
    @Post("follow_wiki")
    @ValidateRequestToken
    @RemoteLog(type = RemoteLogType.FOLLOW)
    public String follow(Invocation inv, @Param("wikiId") int wikiId) {
        try {
            followService.follow(wikiId, hostHolder.getUser().getId());
        } catch (WikiException e) {
            return this.writeJson(e);
        }
        return this.writeJsonData("");
    }

    /**
     * @see ValidateRequestToken
     */
    @Post("unfollow_wiki")
    @ValidateRequestToken
    public String unfollow(Invocation inv, @Param("wikiId") int wikiId) {
        try {
            followService.unfollow(wikiId, hostHolder.getUser().getId());
        } catch (WikiException e) {
            return this.writeJson(e);
        }
        return this.writeJsonData("");
    }
}
