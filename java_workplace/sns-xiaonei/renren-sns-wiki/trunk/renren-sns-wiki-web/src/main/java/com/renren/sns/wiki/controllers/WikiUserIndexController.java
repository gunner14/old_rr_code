package com.renren.sns.wiki.controllers;

import java.util.Map;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.sf.json.JSONObject;

import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.wiki.constant.WikiConstant;
import com.renren.sns.wiki.enums.InterestType;
import com.renren.sns.wiki.interceptor.WikiHolder;
import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.service.WikiUserService;
import com.renren.sns.wiki.utils.remotelog.RemoteLogType;
import com.renren.sns.wiki.utils.remotelog.annotation.RemoteLog;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.platform.core.model.User;

/**
 * 用户相关的页面,包括follow wish do collect
 * 
 * @author weiwei.wang@renren-inc.com since 2012-7-31
 * 
 */
@Path("/${wikiId:\\d+}/user")
public class WikiUserIndexController extends BaseIndexController {

    @Autowired
    private HostHolder hostHolder;

    @Autowired
    private WikiHolder wikiHolder;

    @Autowired
    private WikiUserService wikiUserService;

    @Get("/${action:\\w+}")
    @RemoteLog(type = RemoteLogType.USER_DISPLAY, extras = { "action", "page" })
    public String wikiUserIndex(Invocation inv, @Param("wikiId") int wikiId,
            @Param("action") String action, @Param("page") int curPage, @Param("size") int pageSize) {
        if (InterestType.getEnumByValue(action) == null && !"follow".equals(action)) {
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
        wrapWikiIndexInvocation(inv, wiki, host, curPage, pageSize, "user");

        //多少人操作,多少好友操作的数据
        Map<String, Object> countInfoMap = wikiUserService.getCountInfoMap(wikiId, host, action);
        inv.addModel("countInfo", JSONObject.fromObject(countInfoMap).toString());

        inv.addModel("wikiFeedType", "hot");
        inv.addModel("action", action);

        return "wiki-index";
    }

}
