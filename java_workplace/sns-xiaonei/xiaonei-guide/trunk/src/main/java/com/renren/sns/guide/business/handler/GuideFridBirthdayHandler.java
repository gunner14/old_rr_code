package com.renren.sns.guide.business.handler;

import java.util.ArrayList;
import java.util.List;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.portal.Window;

import com.renren.sns.guide.business.GuideWindowHandler;
import com.renren.sns.guide.model.WUserBirthdayView;
import com.renren.sns.guide.view.UserTinyView;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.sns.platform.core.rose.cache.provider.UserProvider;
import com.xiaonei.wService.recentBirthday.FriendBirthdayList;
import com.xiaonei.wService.recentBirthday.RecentBirthdayAdapter;
import com.xiaonei.wService.recentBirthday.UserBirthdayView;

/**
 * @author: wei.xiang
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-5-26 上午11:23:47
 */
public class GuideFridBirthdayHandler implements GuideWindowHandler {

    public static final String GUIDE_FRIDBIRTHDAY_MEMCACHE_KEY = "guidefriendbirthday_";

    public static final String GUIDE_FRIDBIRTHDAY_KEY = "FriendBirthdayId";

    public static final String GUIDE_FRIDBIRTHDAY_REQUESTKEY = "friendBirthdayList";

    @Override
    public void execute(Invocation inv, Window window, CacheManager cm) {
        User host = UserProvider.getInstance().get(cm);
        int hostId = host.getId();
        if (OpiConstants.isXiaonei()) {
            try {// 先走memcache
                // 服务更新后只要保留此句即可
                FriendBirthdayList flist = RecentBirthdayAdapter.getInstance().getFriendBirthdayList(hostId);
                if (flist != null && flist.friendBirthday != null) {
                    List<WUserBirthdayView> ubvl = new ArrayList<WUserBirthdayView>(flist.friendBirthday.size());
                    for (UserBirthdayView ub : flist.friendBirthday) {
                        ubvl.add(new WUserBirthdayView(ub.id, ub.daymonth));
                    }
                    inv.addModel(GUIDE_FRIDBIRTHDAY_KEY, ubvl);
                }
            } catch (Exception e1) {
                e1.printStackTrace();
                inv.addModel(GUIDE_FRIDBIRTHDAY_REQUESTKEY, new ArrayList<UserTinyView>());
            }
        }
    }
}
