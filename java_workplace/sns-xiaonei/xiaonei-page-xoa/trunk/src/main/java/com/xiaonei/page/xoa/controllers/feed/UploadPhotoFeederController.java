package com.xiaonei.page.xoa.controllers.feed;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.sf.json.JSONArray;
import net.sf.json.JSONObject;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.page.feed.domain.BiddingHistory;
import com.renren.page.feed.service.BiddingService;
import com.xiaonei.album.exception.AlbumException;
import com.xiaonei.album.home.AlbumHome;
import com.xiaonei.album.model.Album;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.AuthService;
import com.xiaonei.page.service.TypeService;
import com.xiaonei.page.service.feed.FeedManager;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.service.feed.impl.ContextImpl;
import com.xiaonei.page.util.FeedDefUtil;
import com.xiaonei.page.xoa.controllers.CheckerUtil;
import com.xiaonei.page.xoa.domain.PageHostPair;
import com.xiaonei.page.xoa.util.PageIdChecker;
import com.xiaonei.platform.core.model.User;

@Path("upf")
public class UploadPhotoFeederController {

    @Autowired
    private AuthService authService;

    @Autowired
    private TypeService typeService;

    @Autowired
    private FeedManager feedManager;

    @Autowired
    private BiddingService biddingService;

    private final static Logger logger = Logger.getLogger("PAGEXOA");

    @Get
    public String index(@Param("pageId") int pageId, @Param("curHost") int curHost,
            @Param("albumId") long albumId, @Param("feed") String feed) {
//        if (!PageIdChecker.isPageDomainIn(curHost)) {
//            logger.error(String.format("curHost %s is not a page", curHost));
//            return "@done";
//        }
        PageHostPair pageHostPair = CheckerUtil.getInstance().checkPageAndHost(pageId, curHost);
        Page page = pageHostPair.getPage();
        User host = pageHostPair.getHost();
        if (page == null || host == null) {
            return pageHostPair.getMsg();
        }

        boolean isAdmin = authService.isPageAdministrator(host, pageId);
        if (!isAdmin) {
            return "@not-page-admin";
        }

        Album album = AlbumHome.getInstance().getAlbum(albumId, pageId);
        if (album == null) {
            return "@album-not-exist";
        }

        IContext context = new ContextImpl();
        context.put("album", album);
        try {
            JSONObject feeds = JSONObject.fromObject(feed);
            context.put("feed", feeds);

            this.updateDescription(feeds, host, page);
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            return "@feed-format-wrong?";
        }
        try {
            //设置竞价信息
            BiddingHistory bidding = biddingService.buildBidding(page, host,
                    FeedDefUtil.getPHOTO_PUBLISH_Def(page), albumId, null);
            context.setBidding(bidding);
            feedManager.publishForXoa(page, host, FeedDefUtil.getPHOTO_PUBLISH_Def(page), true,
                    context);
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            return "@send-feed-failed";
        }
        return "@done";
    }

    /**
     * 更新描述，添加by who的语言
     * 
     * @param feeds
     * @param curHost
     */
    private void updateDescription(JSONObject feeds, User curHost, Page page) {
        // 是情侣空间就更新一下描述
        if (typeService.isLover(page)) {
            if (feeds == null) {
                logger.warn(String.format("no pic found for update description： pid=%s,user=%s",
                        page.getId(), curHost.getId()));
                return;
            }
            String description = String.format("【by %s】", curHost.getName());
            JSONArray photoList = feeds.getJSONArray("records");
            if (photoList == null) {
                logger.warn(String.format(
                        "no records found for update description： pid=%s,user=%s", page.getId(),
                        curHost.getId()));
                return;
            }
            for (int i = 0; i < photoList.size() && i < 4; i++) {
                JSONObject photo = (JSONObject) photoList.get(i);
                Long id = photo.getLong("id");

                if (logger.isDebugEnabled()) {
                    logger.debug(String.format("updating description : pid=%s,photo=%s,user=%s",
                            page.getId(), id, curHost.getId()));
                }

                try {
                    AlbumHome.getInstance().updatePhotoTitle(id, page.getId(), description);
                } catch (AlbumException e) {
                    logger.error(
                            String.format("updateDescription fail: pid=%s,photo=%s,user=%s",
                                    page.getId(), id, curHost.getId()), e);
                }
            }

        }
    }
}
