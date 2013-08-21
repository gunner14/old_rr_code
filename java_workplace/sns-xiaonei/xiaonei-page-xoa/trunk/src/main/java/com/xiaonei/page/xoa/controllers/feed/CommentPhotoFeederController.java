package com.xiaonei.page.xoa.controllers.feed;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.album.model.Album;
import com.xiaonei.album.model.Photo;
import com.xiaonei.album.model.PhotoComment;
import com.xiaonei.album.service.AlbumService;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.AuthService;
import com.xiaonei.page.service.album.AlbumPhotoService;
import com.xiaonei.page.service.feed.FeedManager;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.service.feed.impl.ContextImpl;
import com.xiaonei.page.util.FeedDefUtil;
import com.xiaonei.page.util.MetionEncrytedUtil;
import com.xiaonei.page.xoa.controllers.CheckerUtil;
import com.xiaonei.page.xoa.domain.PageHostPair;
import com.xiaonei.page.xoa.util.PageIdChecker;
import com.xiaonei.platform.core.model.User;

@Path("cpf")
public class CommentPhotoFeederController {

    @Autowired
    private AlbumService albumService;

    @Autowired
    private AlbumPhotoService albumPhotoService;

    @Autowired
    private AuthService authService;

    @Autowired
    private FeedManager feedManager;
    
    private final static Logger logger = Logger.getLogger("PAGEXOA");

    @Get
    @Post
    public String index(@Param("pageId") int pageId, @Param("curHost") int curHost,
            @Param("albumId") long albumId, @Param("albumOwnerId") int albumOwnerId,
            @Param("photoId") long photoId, @Param("commentId") long commentId) {
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
        boolean isAdmin = authService.isPageAdministrator(host, pageId);

        Album album = albumService.getAlbum(albumId, albumOwnerId);
        if (album == null) {
            return "@album-not-exist";
        }
        Photo photo = albumPhotoService.getPhotoWithoutLog(photoId, albumOwnerId);
        if (photo == null) {
            return "@photo-not-exist";
        }
        PhotoComment photoComment = albumPhotoService.getPhotoComment(commentId, albumOwnerId,
                photoId);
        if (photoComment == null) {
            return "@photocomment-not-exist";
        }

        IContext context = new ContextImpl();
        context.put("album", album);
        context.put("photo", photo);
        context.put("content", MetionEncrytedUtil.subHtmlContent(photoComment.getBody(), 35));
        context.put("cid", commentId);
        
        try{
            feedManager.publishForXoa(page, host, FeedDefUtil.getPHOTO_COMMON_Def(page), isAdmin, context);
        }catch (Exception e) {
            logger.error(e.getMessage(), e);
            return "@send-feed-failed";
        }
        

        return "@done";
    }
}
