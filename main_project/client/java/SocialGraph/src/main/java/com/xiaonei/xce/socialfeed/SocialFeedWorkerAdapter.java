package com.xiaonei.xce.socialfeed;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.socialgraph.SocialFeedRecommendationManagerPrx;
import xce.socialgraph.SocialFeedRecommendationManagerPrxHelper;
import xce.util.channel.Channel;

/**
 * SocialFeedRecManagerAdapter
 * 
 * @author
 * 
 */

public class SocialFeedWorkerAdapter extends ReplicatedClusterAdapter {

    private static SocialFeedWorkerAdapter instance = null;

    //private static final String ctrEndpoints = "ControllerSocialFeedRecommendationManager";
    private static final String ctrEndpoints = "ControllerSocialFeedWorker";

    private static int interval = 120;

    private SocialFeedWorkerAdapter() {
        super(ctrEndpoints, interval, Channel.newSocialGraphChannel(ctrEndpoints));
        //super(ctrEndpoints, interval);
    }

    public static SocialFeedWorkerAdapter getInstance() {
        if (null == instance) {
            synchronized (SocialFeedWorkerAdapter.class) {
                if (null == instance) {
                    try {
                        instance = new SocialFeedWorkerAdapter();
                    } catch (Exception e) {
                        e.printStackTrace();
                        instance = null;
                    }
                }
            }
        }
        return instance;
    }

    /**
     * NotifySocialFeedUserLogin
     * 
     * @param userId
     * @return
     */
    public void notifySocialFeedUserLogin(int userId) {
        SocialFeedRecommendationManagerPrx proxy = getSocialFeedRecManagerPrxOneWay(userId);
        if (proxy != null) {
            proxy.NotifySocialFeedUserLogin(userId);
        } 
    }

    //Get oneway proxy
    private SocialFeedRecommendationManagerPrx getSocialFeedRecManagerPrxOneWay(int userId) {
        Ice.ObjectPrx prx0 = getValidProxy(userId, 300);
        if (null != prx0) {
            return SocialFeedRecommendationManagerPrxHelper.uncheckedCast(prx0.ice_timeout(300)
                    .ice_oneway());
        }
        return null;
    }
}
