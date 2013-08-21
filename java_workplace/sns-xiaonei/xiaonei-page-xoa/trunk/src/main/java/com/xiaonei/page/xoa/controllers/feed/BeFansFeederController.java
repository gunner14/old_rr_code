package com.xiaonei.page.xoa.controllers.feed;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.log4j.Logger;

import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.FeedService;
import com.xiaonei.page.xoa.controllers.CheckerUtil;
import com.xiaonei.page.xoa.domain.PageHostPair;
import com.xiaonei.platform.core.model.User;

@Path("bf")
public class BeFansFeederController {
    
    private final static Logger logger = Logger.getLogger("PAGEXOA");
        
    @Get
    @Post
    public String index(@Param("pageId")int pageId,@Param("curHost")int curHost){        
        PageHostPair pageHostPair = CheckerUtil.getInstance().checkPageAndHost(pageId, curHost);
        Page page = pageHostPair.getPage();
        User host = pageHostPair.getHost();
        if (page == null || host == null) {
            return pageHostPair.getMsg();
        }
        try{
            //不带人人啸应的参数
            FeedService.getInstance().publishBeFansFeed(page, host, "", "");
        }catch (Exception e) {
            logger.error(e.getMessage(), e);
            return "@send-feed-failed";
        } 
        return "@done";
    }
}
