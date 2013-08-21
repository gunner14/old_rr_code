package com.renren.sns.wiki.controllers.adminwiki.ajax;

import org.springframework.beans.factory.annotation.Autowired;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.renren.sns.wiki.controllers.ajax.BaseAjaxController;
import com.renren.sns.wiki.service.feed.WikiHotFeedService;

/**
 * 用来运营的操作，主要用来人工维护wiki页的内容
 * 
 * @author yi.li@renren-inc.com since Jul 24, 2012
 * 
 */
@Path("feedop")
public class FeedOpController extends BaseAjaxController {

    @Autowired
    private WikiHotFeedService wikiHotFeedService;

    /**
     * 将内容置顶
     * 
     * @param wikiId wiki id
     * @param feedId feed id
     */
    @Post("${wikiId:\\d+}/${feedId:\\d+}/top")
    public String top(@Param("wikiId") int wikiId, @Param("feedId") long feedId) {
        wikiHotFeedService.topFeed(wikiId, feedId);
        return this.writeJsonData(null);
    }

    /**
     * 根据wiki id重新载入最热内容
     * 
     * @param wikiId wiki id
     */
    @Post("${wikiId:\\d+}/reloadhot")
    @Get("${wikiId:\\d+}/reloadhot")
    public String reloadHotFeed(@Param("wikiId") int wikiId) {
        wikiHotFeedService.reloadWikiHotFeed(wikiId);
        return this.writeJsonData(null);
    }

    /**
     * 删除某个wiki中的feed
     * 
     * @param wikiId wiki id
     * @param feedId feed id
     */
    @Post("${wikiId:\\d+}/${feedId:\\d+}/delete")
    public String deleteFeed(@Param("wikiId") int wikiId, @Param("feedId") long feedId) {
        wikiHotFeedService.deleteHotFeedRecord(wikiId, feedId);
        wikiHotFeedService.deleteNewsFeed(wikiId, feedId);
        return this.writeJsonData(null);
    }
}
