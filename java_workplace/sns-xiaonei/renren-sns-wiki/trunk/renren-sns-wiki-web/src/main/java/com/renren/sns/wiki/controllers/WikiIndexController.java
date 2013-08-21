package com.renren.sns.wiki.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.wiki.constant.WikiConstant;
import com.renren.sns.wiki.enums.ItemType;
import com.renren.sns.wiki.enums.WikiFeedType;
import com.renren.sns.wiki.interceptor.WikiHolder;
import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.service.HighLightService;
import com.renren.sns.wiki.service.WikiFeedService;
import com.renren.sns.wiki.utils.remotelog.RemoteLogType;
import com.renren.sns.wiki.utils.remotelog.annotation.RemoteLog;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.platform.core.model.User;

/**
 * 根目录的入口
 * 
 * @author yi.li@renren-inc.com since 2012-3-23
 * 
 */
@Path("/${wikiId:[a-z0-9\\-_]+}")
public class WikiIndexController extends BaseIndexController {

    @Autowired
    private HostHolder hostHolder;

    @Autowired
    private WikiHolder wikiHolder;

    @Autowired
    private WikiFeedService wikiFeedService;

    @Autowired
    private HighLightService highLightService;

    @Get("")
    @Post("")
    @RemoteLog(type = RemoteLogType.INDEX)
    public String index(Invocation inv, @Param("wikiId") int wikiId) {

        int curPage = 0;
        int pageSize = WikiConstant.DEFAULT_FEED_PAGE_SIZE;
        String wikiFeedType = WikiFeedType.HOT.getValue();
        String itemType = ItemType.ALL.getValue();

        //如果最热的条数少于6,将吐最新的数据
        long[] hotFeedIds = wikiFeedService.getHotFeedIds(wikiId, null, 0, 8);
        if (hotFeedIds.length < 6) {
            wikiFeedType = WikiFeedType.LATEST.getValue();
        }

        Wiki wiki = wikiHolder.getWiki();
        User host = hostHolder.getUser();

        wrapWikiIndexInvocation(inv, wiki, host, curPage, pageSize, "feed");
        wrapWikiFeedInvocation(inv, wiki, host, curPage, pageSize, wikiFeedType, itemType);
        
        //是否是首页
        inv.addModel("isIndex", true);
                
        return "wiki-index";
    }

    @Get("test")
    public String test(Invocation inv, @Param("wikiId") int wikiId) {
        wrapHighLightData(inv, wikiId);
        highLightService.cleanCache(wikiId);
        return "@success";
    }

}
