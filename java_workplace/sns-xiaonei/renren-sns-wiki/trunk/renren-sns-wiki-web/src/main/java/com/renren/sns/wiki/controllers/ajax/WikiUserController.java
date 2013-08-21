package com.renren.sns.wiki.controllers.ajax;

import java.util.Map;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.wiki.constant.WikiConstant;
import com.renren.sns.wiki.exception.WikiExceptionType;
import com.renren.sns.wiki.service.WikiUserService;
import com.renren.sns.wiki.utils.remotelog.RemoteLogType;
import com.renren.sns.wiki.utils.remotelog.annotation.RemoteLog;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;

@Path("user/${wikiId:\\d+}")
public class WikiUserController extends BaseAjaxController {

    @Autowired
    private HostHolder hostHolder;

    @Autowired
    private WikiUserService wikiUserService;

    @Get("/friends/${type:\\w+}")
    @LoginRequired
    public String friends(Invocation inv, @Param("wikiId") int wikiId, @Param("type") String type,
            @Param("p") int page, @Param("frame") int frame, @Param("pageSize") int pageSize) {

        int framePageSize = WikiConstant.FRAME_USER_SIZE;
        if (frame < 0 || frame >= framePageSize - 1) {
            return writeJson(WikiExceptionType.UNKNOWN_EXCEPTION.getCode(), "frame取0到"
                    + (framePageSize - 1) + "之间的值", "");
        }

        if (page < 0) {
            page = 0;
        }
        if (pageSize <= 0) {
            pageSize = WikiConstant.WIKI_USER_PAGE_SIZE;
        }
        if (pageSize > WikiConstant.WIKI_USER_MAX_PAGE_SIZE) {
            pageSize = WikiConstant.WIKI_USER_MAX_PAGE_SIZE;
        }

        User host = hostHolder.getUser();

        int realPage = page * framePageSize + frame;

        Map<String, Object> map = wikiUserService.getFriendWikiUserMap(wikiId, host, realPage,
                pageSize, type);

        return writeJsonData(map);
    }

    @Get("/all/${type:\\w+}")
    @RemoteLog(type = RemoteLogType.DISPLAY_USER_AJAX, extras = { "type", "p", "frame" })
    public String all(Invocation inv, @Param("wikiId") int wikiId, @Param("type") String type,
            @Param("p") int page, @Param("frame") int frame, @Param("pageSize") int pageSize) {
        int framePageSize = WikiConstant.FRAME_USER_SIZE;
        if (frame < 0 || frame >= framePageSize - 1) {
            return writeJson(WikiExceptionType.UNKNOWN_EXCEPTION.getCode(), "frame取0到"
                    + (framePageSize - 1) + "之间的值", "");
        }
        if (page < 0) {
            page = 0;
        }
        if (pageSize <= 0) {
            pageSize = WikiConstant.WIKI_USER_PAGE_SIZE;
        }
        if (pageSize > WikiConstant.WIKI_USER_MAX_PAGE_SIZE) {
            pageSize = WikiConstant.WIKI_USER_MAX_PAGE_SIZE;
        }

        User host = hostHolder.getUser();

        int realPage = page * pageSize + frame;

        Map<String, Object> map = wikiUserService.getAllWikiUserMap(wikiId, host, realPage,
                pageSize, type);

        return writeJsonData(map);
    }
}
