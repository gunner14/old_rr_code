package com.xiaonei.xce.friendrecommend;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.socialgraph.FriendRecommendPrx;
import xce.socialgraph.FriendRecommendPrxHelper;
import xce.socialgraph.RecommendItem;
import xce.util.channel.Channel;

/**
 * FriendRecommendAdapter
 * 
 * @author lei.yang
 * last updated: 2012/12/17
 */

public class FriendRecommendAdapter extends ReplicatedClusterAdapter {

    private static FriendRecommendAdapter instance = null;

    private static final String ctrEndpoints = "ControllerFriendRecommend";

    private static int interval = 150;

    private FriendRecommendAdapter() {
        super(ctrEndpoints, interval, Channel.newSocialGraphChannel(ctrEndpoints));
    }

    public static FriendRecommendAdapter getInstance() {
        if (null == instance) {
            synchronized (FriendRecommendAdapter.class) {
                if (null == instance) {
                    try {
                        instance = new FriendRecommendAdapter();
                    } catch (Exception e) {
                        e.printStackTrace();
                        instance = null;
                    }
                }
            }
        }
        return instance;
    }

    public RecommendItem[] getFriendRecommendation(int userId, int limit) {
        Ice.ObjectPrx prx0 = getValidProxy(userId, 200);
        FriendRecommendPrx proxy = null;
        if (null != prx0) {
            proxy = FriendRecommendPrxHelper.uncheckedCast(prx0.ice_timeout(200)
                    .ice_twoway());
        }
        
        if (proxy != null) {
            return proxy.getFriendRecommendation(userId, limit);
        }
        return null;
    }
    
    public RecommendItem[] getInfoRecommendation(int userId, int limit) {
        Ice.ObjectPrx prx0 = getValidProxy(userId, 200);
        FriendRecommendPrx proxy = null;
        if (null != prx0) {
            proxy = FriendRecommendPrxHelper.uncheckedCast(prx0.ice_timeout(200)
                    .ice_twoway());
        }
        
        if (proxy != null) {
            return proxy.getInfoRecommendation(userId, limit);
        }
        return null;
    }
    
    public static void main(String[] args) {
        int userId = 224357645;
        int limit = 20;
        FriendRecommendAdapter adapter = FriendRecommendAdapter.getInstance();
        if(adapter != null){
            RecommendItem[] result = adapter.getFriendRecommendation(userId, limit);
            if(result != null){
                for(RecommendItem item : result){
                    System.out.println(item.id + ":" + item.weight);
                }
            }else{
                System.out.println("friend result is null.");
            }
            System.out.println("------------------------");
            result = null;
            result = adapter.getInfoRecommendation(userId, limit);
            if(result != null){
                for(RecommendItem item : result){
                    System.out.println(item.id + ":" + item.weight);
                }
            }else{
                System.out.println("info result is null.");
            }
        }else{
            System.out.println("adapter is null.");
        }
        
        System.exit(0);
    }
}
