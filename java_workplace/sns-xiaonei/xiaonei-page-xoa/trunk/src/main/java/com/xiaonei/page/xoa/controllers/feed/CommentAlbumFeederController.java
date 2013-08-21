package com.xiaonei.page.xoa.controllers.feed;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.album.model.Album;
import com.xiaonei.album.model.AlbumComment;
import com.xiaonei.album.service.AlbumService;
import com.xiaonei.page.PageDomainType;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.AuthService;
import com.xiaonei.page.service.album.AlbumPhotoService;
import com.xiaonei.page.util.FeedDefUtil;
import com.xiaonei.page.util.MetionEncrytedUtil;
import com.xiaonei.page.xoa.controllers.CheckerUtil;
import com.xiaonei.page.xoa.domain.PageHostPair;
import com.xiaonei.page.xoa.util.PageIdChecker;
import com.xiaonei.platform.component.feed.page.PageAlbumCommentPublisher;
import com.xiaonei.platform.component.feed.page.model.PageAlbumCommentModel;
import com.xiaonei.platform.core.model.User;

@Path("caf")
public class CommentAlbumFeederController {

    @Autowired
    private AlbumService albumService;

    @Autowired
    private AuthService authService;

    @Autowired
    private AlbumPhotoService albumPhotoService;

    private final static Logger logger = Logger.getLogger("PAGEXOA");

    @Get
    public String index(@Param("pageId") int pageId, @Param("curHost") int curHost,
            @Param("albumId") long albumId, @Param("commentId") long commentId,
            @Param("albumOwnerId") int albumOwnerId) {
        //如果curHost不代表一个page，直接返回。
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
        if(album==null){
            return "@album-not-exist";
        }
        AlbumComment albumComment = albumPhotoService.getAlbumComment(commentId, albumOwnerId,
                albumId);
        if(albumComment==null){
            return "@albumcomment-not-exist";
        }

        String aUrl = String.format("%s/%s/album/%s?", PageDomainType.getLocalDomain()
                .getDomainUrl(), album.getOwner(), album.getId());
        PageAlbumCommentModel feedModel = new PageAlbumCommentModel();
        feedModel.setAdmin(isAdmin);
        feedModel.setAlbumCommentID(commentId);
        feedModel.setAlbumID(album.getId());
        feedModel.setAName(album.getName());
        feedModel.setAUrl(aUrl);
        feedModel.setComm(MetionEncrytedUtil.subHtmlContent(albumComment.getBody(), 35));
        feedModel.setFeedType((int) FeedDefUtil.getALBUM_COMMON_Def(page));
        if (isAdmin) {
            feedModel.setFromID(page.getId());
            feedModel.setFromName(page.getName());
        } else {
            feedModel.setFromID(host.getId());
            feedModel.setFromName(host.getName());
        }
        feedModel.setIdef(page.isChecked());
        feedModel.setPageID(page.getId());
        feedModel.setSrc(album.getHeadUrl());
        feedModel.setUID(album.getOwner());
        feedModel.setUName(page.getName());
        feedModel.setUrl(aUrl);
        //不发mini feed？如果不发，普通粉丝得不到提示。
        if(!isAdmin){
            feedModel.setSendMinifeed(true);
        }else{
            feedModel.setSendMinifeed(false);//管理员不发mini feed
        }
        
        try {
            PageAlbumCommentPublisher.publish(feedModel);
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            return "@send-feed-failed";
        }
        return "@done";
    }

}
