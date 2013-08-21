package com.xiaonei.talk;

import java.util.Vector;

import xce.util.channel.Channel;
import xce.util.service.ServiceAdapter;
import xce.clusterstate.ReplicatedClusterAdapter;

public class TalkRightAdapter extends ReplicatedClusterAdapter {
    private static final String ctr_endpoints = "ControllerTalkRight";
    private static int _interval = 120;
    
    private static TalkRightAdapter instance = new TalkRightAdapter();
 
    public static TalkRightAdapter instance() {
        return instance;
    }
 
    TalkRightAdapter() {
        super(ctr_endpoints, _interval, Channel.newFeedControllerChannel(ctr_endpoints));
    }
         
    public RightManagerPrx getManagerOneway(int id) {
        Ice.ObjectPrx prx0 = getValidProxy(id, 300);
        if (prx0 != null) {
            RightManagerPrx prx = RightManagerPrxHelper.uncheckedCast(prx0.ice_timeout(300).ice_oneway());
            return prx;
        } else {
            return null;
        }
        
    }

    public void kickDesk(int userId){
        getManagerOneway(userId).kickDesk(userId);
    }
 
 }
