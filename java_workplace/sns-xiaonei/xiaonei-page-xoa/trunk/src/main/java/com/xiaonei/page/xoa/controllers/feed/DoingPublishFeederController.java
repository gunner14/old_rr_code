package com.xiaonei.page.xoa.controllers.feed;

import java.util.HashMap;
import java.util.Map;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.DoingService;
import com.xiaonei.page.service.feed.FeedManager;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.service.feed.impl.ContextImpl;
import com.xiaonei.page.util.FeedDefUtil;
import com.xiaonei.page.xoa.controllers.CheckerUtil;
import com.xiaonei.page.xoa.domain.PageHostPair;
import com.xiaonei.page.xoa.util.PageIdChecker;
import com.xiaonei.platform.component.doing.model.Doing;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.platform.core.opt.ice.WUserBasicAdapter;

@Path("dpf")
public class DoingPublishFeederController {

    @Autowired
    private DoingService doingService;

    @Autowired
    private FeedManager feedManager;

    public static final String PAGE_ID_KEY = "pageId";

    private final static Logger logger = Logger.getLogger("PAGEXOA");

    @Get
    @Post
    public String index(@Param("pageId") int pageId, @Param("curHost") int curHost,
            @Param("doingId") long dongId, @Param("wap") boolean wap) {
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

        Doing doing = doingService.getDoingById(dongId, pageId, null);
        if (doing == null) {
            return "@doing-not-exist";
        }
        UserBasic ub = WUserBasicAdapter.getInstance().get(pageId);
        Map<String, String> feedParamMap = new HashMap<String, String>();
        feedParamMap.put(PAGE_ID_KEY, String.valueOf(page.getId()));
        IContext context = new ContextImpl();
        context.setWap(wap);
        context.put("ub", ub);
        context.put("doing", doing);
        context.put("feedParamMap", feedParamMap);

        try {
            feedManager.publishForXoa(page, host, FeedDefUtil.getSTATUS_UPDATE_Def(page), true, context);
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            return "@send-feed-failed";
        }

        return "@done";
    }
}
