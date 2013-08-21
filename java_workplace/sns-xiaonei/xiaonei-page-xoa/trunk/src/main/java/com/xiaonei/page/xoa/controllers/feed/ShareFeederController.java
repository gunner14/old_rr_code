package com.xiaonei.page.xoa.controllers.feed;

import java.util.HashMap;
import java.util.Map;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.app.share.model.Share;
import com.renren.app.share.xoa.api.ShareService;
import com.renren.xoa.lite.ServiceFactories;
import com.renren.xoa.lite.ServiceFuture;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.AuthService;
import com.xiaonei.page.service.feed.FeedManager;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.service.feed.impl.ContextImpl;
import com.xiaonei.page.xoa.controllers.CheckerUtil;
import com.xiaonei.page.xoa.domain.PageHostPair;
import com.xiaonei.page.xoa.util.PageIdChecker;
import com.xiaonei.platform.component.xfeed.helper.FeedDefinition;
import com.xiaonei.platform.core.model.User;

@Path("sf")
public class ShareFeederController {

    private ShareService shareApi = ServiceFactories.getFactory().getService(ShareService.class);

    @Autowired
    private FeedManager feedManager;

    @Autowired
    private AuthService authService;

    private final static Logger logger = Logger.getLogger("PAGEXOA");

    private static final Map<Integer, Integer> shareFeedTypeMapping = new HashMap<Integer, Integer>();

    //ugc分享的对应关系，与Page这边的不一致
    static {
        //ugc原始的对应关系
        shareFeedTypeMapping.put(6, FeedDefinition.PAGE_SHARE_LINK);
        shareFeedTypeMapping.put(10, FeedDefinition.PAGE_SHARE_VIDEO);
        shareFeedTypeMapping.put(11, FeedDefinition.PAGE_SHARE_MUSIC);

        shareFeedTypeMapping.put(19, FeedDefinition.PAGE_SHARE_MUSIC);
        shareFeedTypeMapping.put(21, FeedDefinition.PAGE_SHARE_LINK);
        shareFeedTypeMapping.put(23, FeedDefinition.PAGE_SHARE_VIDEO);

        shareFeedTypeMapping.put(26, FeedDefinition.CLUB_SHARE_MUSIC);
        shareFeedTypeMapping.put(28, FeedDefinition.CLUB_SHARE_LINK);
        shareFeedTypeMapping.put(30, FeedDefinition.CLUB_SHARE_VIDEO);
    }

    @Get
    @Post
    public String index(@Param("pageId") int pageId, @Param("curHost") int curHost,
            @Param("shareId") long shareId, @Param("ownerId") int ownerId) {
//        //如果curHost不是page，直接不发
//        if(!PageIdChecker.isPageDomainIn(curHost)){
//            return "@done";
//        }
//        PageHostPair pageHostPair = CheckerUtil.getInstance().checkPageAndHost(pageId, curHost);
//        Page page = pageHostPair.getPage();
//        User host = pageHostPair.getHost();
//        if (page == null || host == null) {
//            return pageHostPair.getMsg();
//        }
//        boolean isAdmin = authService.isPageAdministrator(host, pageId);
//
//        ServiceFuture<Share> sf = shareApi.getShare(ownerId, shareId);
//        sf.submit();
//        if (sf.awaitUninterruptibly(3000)) {
//            if (sf.isSuccess()) {
//                Share share = sf.getContent();
//                IContext context = new ContextImpl();
//                context.put("share", share);
//                
//                Integer feedType = shareFeedTypeMapping.get(share.getType());
//
//                if (feedType == null) {
//                    logger.error("sharefeedmanager-not-exist");
//                    return "@sharefeedmanager-not-exist";
//                }
//                feedManager.publishForXoa(page, host, feedType, isAdmin, context);
//            } else {
//                logger.error("get-share-failed");
//                return "@get-share-failed";
//            }
//        } else {
//            logger.error("get-share-failed(timeout)");
//            return "@get-share-failed(timeout)";
//        }
        return "@done";
    }

}
