package com.renren.sns.wiki.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.wiki.constant.WikiConstant;
import com.renren.sns.wiki.enums.WikiFeedType;
import com.renren.sns.wiki.interceptor.WikiHolder;
import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.utils.remotelog.RemoteLogType;
import com.renren.sns.wiki.utils.remotelog.annotation.RemoteLog;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.platform.core.model.User;

/**
 * 新鲜事页面,包含latest hot
 * 
 * @author weiwei.wang@renren-inc.com since 2012-7-31
 * 
 */
@Path("/${wikiId:\\d+}/feed")
public class WikiFeedIndexController extends BaseIndexController {

    @Autowired
    private HostHolder hostHolder;

    @Autowired
    private WikiHolder wikiHolder;

    /**
     * 返回新鲜事 wiki.renren.com/610000011/hot wiki.renren.com/610000011/latest
     * 
     * @param inv
     * @param wikiId
     * @param page
     * @param pageSize
     * @param type
     * @return
     */
    @Get("/${wikiFeedType:\\w+}")
    @RemoteLog(type = RemoteLogType.FEED_DISPLAY, extras = { "wikiFeedType", "type", "page" })
    public String wikiFeedTypePage(Invocation inv, @Param("wikiId") int wikiId,
            @Param("wikiFeedType") String strWikiFeedType, @Param("page") int curPage,
            @Param("size") int pageSize, @Param("type") String itemType) {

        WikiFeedType wikiFeedType = WikiFeedType.getEnum(strWikiFeedType);
        if (wikiFeedType == null) {
            return "e:404";
        }
        if (curPage < 0) {
            curPage = 0;
        }
        if (pageSize < 0 || pageSize > 100) {
            pageSize = WikiConstant.DEFAULT_FEED_PAGE_SIZE;
        }
        Wiki wiki = wikiHolder.getWiki();
        User host = hostHolder.getUser();
        wrapWikiIndexInvocation(inv, wiki, host, curPage, pageSize, "feed");
        wrapWikiFeedInvocation(inv, wiki, host, curPage, pageSize, strWikiFeedType, itemType);

        return "wiki-index";
    }
}
