package com.xiaonei.page.xoa.controllers;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.xoa.commons.annotation.JsonParam;
import com.xiaonei.album.home.AlbumHome;
import com.xiaonei.album.home.PhotoForPageHome;
import com.xiaonei.album.model.Album;
import com.xiaonei.album.model.Photo;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.AdminAuditService;
import com.xiaonei.page.service.PageService;
import com.xiaonei.page.service.feed.FeedManager;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.service.feed.impl.ContextImpl;
import com.xiaonei.page.util.FeedDefUtil;
import com.xiaonei.page.util.HeaderUtil;
import com.xiaonei.page.util.Util;
import com.xiaonei.page.xoa.domain.PageHeadUrls;
import com.xiaonei.page.xoa.domain.PageHostPair;
import com.xiaonei.platform.core.model.User;

@Path("sph")
public class SetPageHeadController {

    @Autowired
    private PageService pageService;

    @Autowired
    private AdminAuditService adminAuditService;

    @Autowired
    private FeedManager feedManager;

    private final static Logger logger = Logger.getLogger("PAGEXOA");

    @Post
    public String setPageHead(@Param("pageId") int pageId, @Param("curHost") int curHost,
            @JsonParam PageHeadUrls urls) {

        PageHostPair pageHostPair = CheckerUtil.getInstance().checkPageAndHost(pageId, curHost);
        Page page = pageHostPair.getPage();
        User host = pageHostPair.getHost();
        if (page == null || host == null) {
            return pageHostPair.getMsg();
        }

        Album album = null;
        try {
            album = AlbumHome.getInstance().getAlbumListByType(pageId, 1).get(0);
        } catch (Exception e) {
            logger.error(String.format("Get album failed! pageId is %s,current host is %s", pageId,
                    curHost), e);
            return "@getAlbumFailed";
        }
        Photo lastPhoto = PhotoForPageHome.getInstance()
                .getLastOne(album.getId(), album.getOwner());
        Photo firstPhoto = PhotoForPageHome.getInstance().getFirstOne(album.getId(),
                album.getOwner());
        int photoCountInAlbum = AlbumHome.getInstance().getPhotoCountInAlbum(album.getOwner(),
                album.getId());

        Photo photo = new Photo();
        photo.setPosition(photoCountInAlbum + 1);
        photo.setAid(album.getId());
        photo.setOwner(pageId);
        photo.setId(PhotoForPageHome.getInstance().getNext());
        logger.info("largeUrl:"+urls.getLargeUrl());
        photo.setLargeUrl(urls.getLargeUrl());
        logger.info("tinyurl:"+urls.getTinyUrl());
        photo.setTinyUrl(urls.getTinyUrl());
        logger.info("headUrl:"+urls.getHeadUrl());
        photo.setHeadUrl(urls.getHeadUrl());
        logger.info("maineUrl:"+urls.getMainUrl());
        photo.setMainUrl(urls.getMainUrl());
        photo.setTitle("");

        photo.setPrevious(photo.getId());
        photo.setNext(photo.getId());
        if (Util.validate(lastPhoto)) {
            photo.setPrevious(lastPhoto.getId());
        }
        if (Util.validate(firstPhoto)) {
            photo.setNext(firstPhoto.getId());

        }
        PhotoForPageHome.getInstance().save(photo);
        if (Util.validate(lastPhoto)) {
            PhotoForPageHome.getInstance().updateNext(lastPhoto.getId(), pageId, photo.getId());
        }
        if (Util.validate(firstPhoto)) {
            PhotoForPageHome.getInstance()
                    .updatePrevious(firstPhoto.getId(), pageId, photo.getId());
        }

        AlbumHome.getInstance().updatePhotoCountAndTime(album.getId(), album.getOwner(),
                album.getPhotoCount() + 1);
        AlbumHome.getInstance().updateCover(album.getId(), album.getOwner(), photo.getMainUrl2(),
                photo.getHeadUrl2(), photo.getTinyUrl2());
        HeaderUtil.updateHeader(pageId, photo.getHeadUrl2(), photo.getMainUrl2(),
                photo.getTinyUrl2(), photo.getLargeUrl2());
        page.setTinyUrl(photo.getTinyUrl2());
        page.setHeadUrl(photo.getHeadUrl2());
        page.setMainUrl(photo.getMainUrl2());
        page.setLargeUrl(photo.getLargeUrl2());
        pageService.update(page);

        if (Util.isLoverPages(page)) {
            //情侣空间修改头像送入审核后台先发后审
            adminAuditService.sentPageHeadToAudit(photo.getId(), pageId, page.getMainUrl());
        }

        try {
            IContext context = new ContextImpl();
            context.put("photoId", photo.getId());
            feedManager.publish(page, host, FeedDefUtil.getHEADPHOTO_UPDATE_Def(page), true,
                    context);
        } catch (Exception e) {
            e.printStackTrace();
        }
        logger.info("new head photo:" + page.getId() + ",operator:" + host.getId() + ",picid:"
                + photo.getId());

        return "@done";
    }
}
