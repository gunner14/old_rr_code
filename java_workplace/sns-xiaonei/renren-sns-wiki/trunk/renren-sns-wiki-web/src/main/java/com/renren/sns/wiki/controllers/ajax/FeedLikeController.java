package com.renren.sns.wiki.controllers.ajax;

import java.util.Map;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.wiki.exception.WikiException;
import com.renren.sns.wiki.exception.WikiExceptionType;
import com.renren.sns.wiki.service.FeedLikeService;
import com.renren.sns.wiki.utils.remotelog.RemoteLogType;
import com.renren.sns.wiki.utils.remotelog.annotation.RemoteLog;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.commons.interceptors.access.annotation.ValidateRequestToken;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;

@Path("like")
public class FeedLikeController extends BaseAjaxController {

    @Autowired
    private HostHolder hostHolder;

    @Autowired
    private FeedLikeService feedLikeService;

    @Post("/add")
    @LoginRequired
    @ValidateRequestToken
    @RemoteLog(type = RemoteLogType.FEED_LIKE, sourceParam = "feedId", extras={"feedType"})
    public String add(Invocation inv, @Param("globalId") String globalId) {
        if (StringUtils.isBlank(globalId)) {
            throw new WikiException(WikiExceptionType.PARAMS_ERROR);
        }
        User host = hostHolder.getUser();
        Map<String, Object> likeInfoMap = feedLikeService.addLike(globalId, host.getId());
        return writeJsonData(likeInfoMap);
    }

    @Post("/remove")
    @LoginRequired
    @ValidateRequestToken
    @RemoteLog(type = RemoteLogType.FEED_UNLIKE, sourceParam = "feedId", extras={"feedType"})
    public String remove(@Param("globalId") String globalId) {
        if (StringUtils.isBlank(globalId)) {
            throw new WikiException(WikiExceptionType.PARAMS_ERROR);
        }
        User host = hostHolder.getUser();
        Map<String, Object> likeInfoMap = feedLikeService.removeLike(globalId, host.getId());
        return writeJsonData(likeInfoMap);
    }

}
