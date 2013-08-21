package com.renren.sns.wiki.controllers.share;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.sf.json.JSONObject;

import org.springframework.beans.factory.annotation.Autowired;

import com.renren.app.share.model.Share;
import com.renren.app.share.model.ShareComment;
import com.renren.sns.wiki.controllers.ajax.BaseAjaxController;
import com.renren.sns.wiki.delegator.xoa.ShareXoaDelegator;
import com.renren.sns.wiki.exception.WikiExceptionType;
import com.renren.sns.wiki.service.ugc.ShareService;
import com.renren.sns.wiki.utils.UserUtils;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;

/**
 * 
 * @author weiwei.wang@renren-inc.com since 2012-7-2
 * 
 */
@Path("")
public class ShareLayerController extends BaseAjaxController {

    @Autowired
    private HostHolder hostHolder;

    @Autowired
    private ShareXoaDelegator shareXoaDelegator;

    @Autowired
    private ShareService shareService;

    @Get("{ownerId:[0-9]+}/{shareId:[0-9]+}/layer")
    public String getShareLayer(Invocation inv, @Param("ownerId") int ownerId,
            @Param("shareId") long shareId) {

        Map<String, Object> map = new HashMap<String, Object>();

        User guest = hostHolder.getUser();
        int code = 0;
        if (guest != null) {
            map.put("guest", UserUtils.convertToSimpleMap(guest));
            map.put("guest_tinyFullUrl", guest.getTinyFullUrl());
        } else {
            code = WikiExceptionType.LOGIN_REQUIRED.getCode();
        }

        User owner = WUserAdapter.getInstance().get(ownerId);
        map.put("owner", UserUtils.convertToSimpleMap(owner));
        map.put("owner_tinyFullUrl", owner.getTinyFullUrl());

        Share share = shareXoaDelegator.getShare(ownerId, shareId);

        int shareType = share.getType(); //视频分享10
        if (shareType == 10) {
            Map<?, ?> shareMap = JSONObject.fromObject(share.getMeta());
            map.put("share", shareMap);
        }

        return writeJson(code, "", map);
    }

    /**
     * 每页40条
     * 
     * @param inv
     * @param ownerId
     * @param shareId
     * @return
     */
    @Get("{ownerId:[0-9]+}/{shareId:[0-9]+}/comments")
    @LoginRequired
    public String getShareComments(Invocation inv, @Param("ownerId") int ownerId,
            @Param("shareId") long shareId, @Param("page") int page) {
        Map<String, Object> map = new HashMap<String, Object>();
        User host = hostHolder.getUser();
        Integer commentsCount = shareXoaDelegator.getCommentCount(ownerId, shareId, host.getId());
        ShareComment[] shareComments = shareXoaDelegator.getComments(ownerId, shareId, 0, 40, host
                .getId());

        //将单个ShareComment转化为Map
        List<Map<String, Object>> commentsList = new ArrayList<Map<String, Object>>();
        if (shareComments != null && shareComments.length > 0) {
            for (ShareComment shareComment : shareComments) {
                Map<String, Object> shareCommentMap = shareService
                        .convertShareCommentToMap(shareComment);
                commentsList.add(shareCommentMap);
            }
        }
        map.put("commentsCount", commentsCount);
        map.put("comments", commentsList);

        //hashMore
        return writeJsonData(map);
    }

}
