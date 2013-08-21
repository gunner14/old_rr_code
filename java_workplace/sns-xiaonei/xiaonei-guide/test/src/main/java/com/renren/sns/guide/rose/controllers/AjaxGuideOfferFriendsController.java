package com.renren.sns.guide.rose.controllers;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.portal.Portal;

import com.renren.sns.guide.utils.GuideUtils;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.reg.guide.util.LogTimeUtils;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.sns.platform.core.rose.cache.provider.UserProvider;
import com.xiaonei.xce.offerfriends.DefaultOffer;
import com.xiaonei.xce.offerfriends.OfferFriendsAdapter;

/**
 * @author: wei.xiang
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-5-20 下午02:29:19
 */
public class AjaxGuideOfferFriendsController {

    @Get
    public String index(Invocation inv, Portal portal, CacheManager cm) throws InterruptedException {
        try {
            long begin = LogTimeUtils.logBegin();
            User host = UserProvider.getInstance().get(cm);
            int limit = 49;
            OfferFriendsAdapter<DefaultOffer> adapter = OfferFriendsAdapter.getInstance();
            adapter.rebuild(host.getId());
            Thread.sleep(73);
            List<DefaultOffer> offers = adapter.getBestOffer(host.getId(), limit);
            List<Integer> ids = new ArrayList<Integer>();
            if (offers != null) {
                for (int i = 0; i < offers.size(); i++) {
                    ids.add(offers.get(i).getUserId());
                }
            }
            Map<Integer, WUserCache> results = null;
            try {
                results = SnsAdapterFactory.getUserCacheAdapter().getUserCacheMap(ids);
            } catch (Exception e) {// 超时或者出错？这个服务太重要的，再取一次？
                System.err.println("GET_USER_CACHE_FOR_HOME_ERROR");
                try {
                    results = SnsAdapterFactory.getUserCacheAdapter().getUserCacheMap(ids);
                } catch (Exception e2) {
                    System.err.println("GET_USER_CACHE_FOR_HOME_ERROR AGAIN!!!");
                    e2.printStackTrace();
                }
            }
            if (ids != null && ids.size() > 0) {
                List<WUserCache> guideCommonFriend = new ArrayList<WUserCache>();
                for (Integer id : ids) {
                    WUserCache c = results.get(id);
                    if (c != null) {
                        guideCommonFriend.add(c);
                    }
                }
                String jsonTwoDegreesFriends = GuideUtils.getInstance().getJSONPeopleForDefType(guideCommonFriend, "TwoDegreesFriends", -1, -1);
                return "@" + jsonTwoDegreesFriends;
            }
            LogTimeUtils.logEnd(begin, "AjaxGuideOfferFriends");
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "@";
    }
}
