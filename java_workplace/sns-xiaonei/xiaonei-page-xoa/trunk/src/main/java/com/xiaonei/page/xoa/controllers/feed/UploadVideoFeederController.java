package com.xiaonei.page.xoa.controllers.feed;

import java.sql.SQLException;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.page.dao.PageVideoAlbumDAO;
import com.xiaonei.page.dao.PageVideoDAO;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.domain.PageVideo;
import com.xiaonei.page.domain.PageVideoAlbum;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.service.feed.impl.ContextImpl;
import com.xiaonei.page.service.feed.impl.PageVideoPublisher;
import com.xiaonei.page.xoa.controllers.CheckerUtil;
import com.xiaonei.page.xoa.domain.PageHostPair;
import com.xiaonei.page.xoa.util.PageIdChecker;
import com.xiaonei.platform.core.model.User;

@Path("uvf")
public class UploadVideoFeederController {

    @Autowired
    private PageVideoDAO pva;

    @Autowired
    private PageVideoAlbumDAO pvad;

    private final static Logger logger = Logger.getLogger("PAGEXOA");
    
    @Get
    @Post
    public String index(@Param("pageId") int pageId, @Param("curHost") int curHost,
            @Param("videoAlbumId") int videoAlbumId, @Param("videoId") int videoId) {
//        目前尚不需要
//        if(!PageIdChecker.isPageDomainIn(curHost)){
//            logger.error(String.format("curHost %s is not a page", curHost));
//            return "@done";
//        }
        PageHostPair pageHostPair = CheckerUtil.getInstance().checkPageAndHost(pageId, curHost);
        Page page = pageHostPair.getPage();
        User host = pageHostPair.getHost();
        if (page == null || host == null) {
            return pageHostPair.getMsg();
        }

        PageVideoAlbum album = null;
        try {
            album = pvad.findById(videoAlbumId);
        } catch (SQLException e) {
            logger.error(e.getMessage(), e);
            return "@db-error";
        }
        PageVideo video = null;
        try {
            video = pva.findById(videoId);
        } catch (SQLException e) {
            logger.error(e.getMessage(),e);
            return "@db-error";
        }
        if (album == null) {
            return "@videoalbum-not-exsit";
        }
        if (video == null) {
            return "@video-not-exsit";
        }
        
        IContext context = new ContextImpl();
        context.put("videoAlbum", album);
        context.put("video", video);
        context.setSendMini(true);
        context.setSendNews(true);
        
        PageVideoPublisher publisher = new PageVideoPublisher();
        try {
            publisher.publish(page, host, true, context, null, null);
        } catch (Exception e) {
            logger.error(e.getMessage(),e);
            return "@send-feed-failed";
        }
        
        return "@done";
    }
}
