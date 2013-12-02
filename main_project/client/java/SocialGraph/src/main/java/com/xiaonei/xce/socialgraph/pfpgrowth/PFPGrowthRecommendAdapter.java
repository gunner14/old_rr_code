package com.xiaonei.xce.socialgraph.pfpgrowth;

import java.util.ArrayList;
import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.xce.socialgraph.sortbycluster.SortByClusterAdapter;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.socialgraph.AssociationRulePrx;
import xce.socialgraph.AssociationRulePrxHelper;
import xce.socialgraph.FriendData;
import xce.socialgraph.RcdInfo;
import xce.util.channel.Channel;

public class PFPGrowthRecommendAdapter extends ReplicatedClusterAdapter {
    
    private static final Log LOG = LogFactory.getLog(PFPGrowthRecommendAdapter.class);
    private static final String endpoints = "ControllerAssociationRule";
    private static int INTERVAL = 100;
    private static int TIMEOUT = 300;
    private static final Channel DEFAULT_CHANNEL = Channel.newSocialGraphChannel(endpoints);
    private static PFPGrowthRecommendAdapter instance = null;
    
    private PFPGrowthRecommendAdapter() {
        super(endpoints, INTERVAL, DEFAULT_CHANNEL);
    }
    
    private PFPGrowthRecommendAdapter(Channel channel) {
        super(endpoints, INTERVAL, channel);
    }
    
    public static PFPGrowthRecommendAdapter getInstance(){
    	if (null == instance) {
			synchronized (PFPGrowthRecommendAdapter.class) {
				if (null == instance) {
					try {
						instance = new PFPGrowthRecommendAdapter();
					} catch (Exception e) {
						e.printStackTrace();
						instance = null;
					}
				}
			}
		}
		return instance;
    }

    protected AssociationRulePrx getManager(int userId) {
        Ice.ObjectPrx proxy = getValidProxy(userId, TIMEOUT);
        if (proxy != null) {
            return AssociationRulePrxHelper.uncheckedCast(proxy.ice_timeout(TIMEOUT).ice_twoway());
        }
        return null;
    }

    protected AssociationRulePrx getCacheOneWay(int userId) {
        Ice.ObjectPrx proxy = getValidProxy(userId, TIMEOUT);
        if (proxy != null) {
            return AssociationRulePrxHelper.uncheckedCast(proxy.ice_timeout(TIMEOUT).ice_oneway());
        }
        return null;
    }
    
    public List<FriendData> recommend(int userID, int applyID, int limit){
        AssociationRulePrx prx = null;
        List<FriendData> result = new ArrayList<FriendData>();
        try {
            prx = getManager(userID);
            FriendData[] res = prx.getRecommendByAssociationRule(userID, applyID, limit);
            for(FriendData data : res){
                result.add(data);
            }
        } catch (Exception e) {
            LOG.error(e, e);
        }
        
        return result;
    }
    
    public List<RcdInfo> getRcdInfos(int userID, int applyID, int limit){
        AssociationRulePrx prx = null;
        List<RcdInfo> result = new ArrayList<RcdInfo>();
        try {
            prx = getManager(userID);
            RcdInfo[] res = prx.getRcdByAssociationRule(userID, applyID, limit);
            for(RcdInfo data : res){
                result.add(data);
            }
        } catch (Exception e) {
            LOG.error(e, e);
        }
        return result;
    }
    
    public static void main(String[] args) {
        if (args.length != 2) {
          System.out.println("wrong args");
          System.exit(0);
        }
        int userID = Integer.parseInt(args[0]);
        int applyID = Integer.parseInt(args[1]);
        int limit = 30;
        List<FriendData> result = PFPGrowthRecommendAdapter.getInstance().recommend(userID, applyID, limit);
        for (FriendData rcd : result) {
          System.out.println(rcd.userID + " " + rcd.weight + " " );
        }
        
        List<RcdInfo> resultRcd = PFPGrowthRecommendAdapter.getInstance().getRcdInfos(userID, applyID, limit);
        for (RcdInfo rcd : resultRcd) {
          System.out.println(rcd.userID + " " + rcd.weight + " " + rcd.type);
        }
        System.exit(0);
    }
}
