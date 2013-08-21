package com.xiaonei.page.xoa.controllers.feed;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.GossipService;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.service.feed.impl.ContextImpl;
import com.xiaonei.page.service.feed.impl.XoaPageFansGossipPublisher;
import com.xiaonei.page.xoa.controllers.CheckerUtil;
import com.xiaonei.page.xoa.domain.PageHostPair;
import com.xiaonei.platform.component.gossip.view.GossipView;
import com.xiaonei.platform.core.model.User;

@Path("fgf")
public class FansGossipFeederController {

    @Autowired
    private GossipService gossipService;
    
    private final static Logger logger = Logger.getLogger("PAGEXOA");
    
    @Get
    @Post
    public String index(@Param("pageId")int pageId,@Param("curHost")int curHost,@Param("gossipId")long gossipId){
        PageHostPair pageHostPair=CheckerUtil.getInstance().checkPageAndHost(pageId, curHost);
        Page page=pageHostPair.getPage();
        User host=pageHostPair.getHost();
        if(page==null||host==null){
            return pageHostPair.getMsg();
        }
        GossipView gv=gossipService.getGossip(pageId, gossipId);
        if(gv==null){
            return "@gossip-not-exist";
        }
        IContext context = new ContextImpl();
        context.put("body", gv.getBody());
        context.put("gossipId", gossipId);
        context.setSendNews(true);

        try {
            XoaPageFansGossipPublisher.getInstance()
                    .publish(page, host, false, context, null, null);
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            return "@send-feed-failed";
        }
        return "@done";
    }
}
