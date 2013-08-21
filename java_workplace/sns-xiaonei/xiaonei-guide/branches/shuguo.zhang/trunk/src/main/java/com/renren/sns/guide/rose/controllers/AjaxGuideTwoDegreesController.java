package com.renren.sns.guide.rose.controllers;

import java.util.ArrayList;
import java.util.List;

import mop.hi.oce.domain.model.FriendCommon;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.rest.Post;
import net.paoding.rose.web.portal.Portal;

import com.renren.sns.guide.utils.GuideUtils;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.platform.core.opt.ice.WUserCacheAdapter;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.sns.platform.core.rose.cache.provider.UserProvider;

/**
 * @author: wei.xiang
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-8-19 下午04:45:29
 */
public class AjaxGuideTwoDegreesController {

    @Post
    public String index(Invocation inv, Portal portal, CacheManager cm) throws InterruptedException {
        User host = UserProvider.getInstance().get(cm);
        // 二度好友
        List<FriendCommon> fcList = SnsAdapterFactory.getFriendFinderGuideReplicaAdapter().getCommonFriendsRandom(host.getId(), 45);
        List<Integer> ids = new ArrayList<Integer>();
        if (fcList != null) {
            for (int i = 0; i < fcList.size(); i++) {
                ids.add(fcList.get(i).getUserId());
            }
        }
        List<WUserCache> commonFriendList = WUserCacheAdapter.getInstance().getUsersCacheList(ids);
        String jsonTwoDegreesFriends = GuideUtils.getInstance().getJSONPeopleForDefType(commonFriendList, "TwoDegreesFriends" , -1, -1);
        inv.addModel("ajaxTwoDegreesFriends", jsonTwoDegreesFriends);
        return "/views/guide/xf/common/ajax/guide_two_degrees.jsp";
    }
}
