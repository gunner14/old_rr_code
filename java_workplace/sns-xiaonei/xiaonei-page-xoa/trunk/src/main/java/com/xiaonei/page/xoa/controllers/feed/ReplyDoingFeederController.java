package com.xiaonei.page.xoa.controllers.feed;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.AuthService;
import com.xiaonei.page.service.DoingService;
import com.xiaonei.page.xoa.controllers.CheckerUtil;
import com.xiaonei.page.xoa.domain.PageHostPair;
import com.xiaonei.page.xoa.util.PageIdChecker;
import com.xiaonei.platform.component.doing.model.Doing;
import com.xiaonei.platform.component.doing.model.ReplyDoing;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

@Path("rdf")
public class ReplyDoingFeederController {

    @Autowired
    private DoingService doingService;
    
    @Autowired
    private AuthService authService;
    
    private final static Logger logger = Logger.getLogger("PAGEXOA");

    @Get
    @Post
    public String index(@Param("pageId") int pageId, @Param("curHost") int curHost,
            @Param("doingOwner") int doingOwner, @Param("doingId") long doingId,
            @Param("replyId") long replyId) {
        //如果curHost不是page，直接不发
        if(!PageIdChecker.isPageDomainIn(curHost)){
            return "@done";
        }
        PageHostPair pageHostPair = CheckerUtil.getInstance().checkPageAndHost(pageId, curHost);
        Page page = pageHostPair.getPage();
        User host = pageHostPair.getHost();
        if (page == null || host == null) {
            return pageHostPair.getMsg();
        }
        
        try{
            User doingUser = WUserAdapter.getInstance().get(doingOwner);
            Doing doing=doingService.getDoingById(doingId, doingOwner, null);
            if(doing==null){
                return "@doing-not-exist";
            }
            ReplyDoing replyDoing=doingService.getDoingReply(replyId, doingOwner);
            if(replyDoing==null){
                return "doingreply-not-exist";
            }            
            boolean isAdmin=authService.isPageAdministrator(host, pageId);
            doingService.publishfeed(page, doingUser, host, doing, replyDoing, isAdmin);
        }catch (Exception e) {
            logger.error(e.getMessage(), e);
            return "@send-feed-failed";
        }
        return "@done";
    }
}
