package com.xiaonei.xce.socialfeed;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.socialgraph.SocialFeedLoaderPrx;
import xce.socialgraph.SocialFeedLoaderPrxHelper;
import xce.util.channel.Channel;

/**
 * SocialFeedLoaderAdapter
 * 
 * @author
 * 
 */

public class SocialFeedLoaderAdapter extends ReplicatedClusterAdapter {

    private static SocialFeedLoaderAdapter instance = null;

    private static final String ctrEndpoints = "ControllerSocialFeedLoader";

    private static int interval = 120;

    private SocialFeedLoaderAdapter() {
        super(ctrEndpoints, interval, Channel.newSocialGraphChannel(ctrEndpoints));
    }

    public static SocialFeedLoaderAdapter getInstance() {
        if (null == instance) {
            synchronized (SocialFeedLoaderAdapter.class) {
                if (null == instance) {
                    try {
                        instance = new SocialFeedLoaderAdapter();
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
     * handler
     * 
     * @param userId
     * @return
     */
    public void handler(int userId) {
        SocialFeedLoaderPrx proxy = getSocialFeedLoaderPrxOneWay(userId);
        if (proxy != null) {
            proxy.handler(userId);
        }
    }

    //Get oneway proxy
    private SocialFeedLoaderPrx getSocialFeedLoaderPrxOneWay(int userId) {
        Ice.ObjectPrx prx0 = getValidProxy(userId, 300);
        if (null != prx0) {
            return SocialFeedLoaderPrxHelper.uncheckedCast(prx0.ice_timeout(300).ice_oneway());
        }
        return null;
    }

}
