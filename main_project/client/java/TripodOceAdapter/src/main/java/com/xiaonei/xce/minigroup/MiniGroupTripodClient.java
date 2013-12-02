package com.xiaonei.xce.minigroup;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import xce.distcache.user2minigroup.User2MiniGroup.User2MiniGroupData;

import com.google.protobuf.InvalidProtocolBufferException;
import com.renren.tripod.client.TripodClient;

public class MiniGroupTripodClient {
  final private String TRIPOD_ZK_ADDRESS =
    "UserZooKeeper1.n.xiaonei.com:2181,UserZooKeeper2.n.xiaonei.com:2181,UserZooKeeper3.n.xiaonei.com"
    + ":2181,UserZooKeeper4.n.xiaonei.com:2181,UserZooKeeper5.n.xiaonei.com:2181/Tripod";
  final private String TRIPOD_ZK_NAMESPACE = "ne1";
	public static MiniGroupTripodClient instance_ = new MiniGroupTripodClient();
	private MiniGroupTripodClient () {}
	public static MiniGroupTripodClient getInstance() {
	  return instance_;
	}
	
	private static class User2MiniGroupInfo{
	  private int userId_ = 0;
	  private List<Long> groupIds_ = new ArrayList<Long>();
	  
	  public User2MiniGroupInfo() {
	  }
	  public List<Long> getMiniGroupIds() {
	    return groupIds_;
	  }
	  
	  public User2MiniGroupInfo(int userId, List<Long> groupIds) {
	    userId_ = userId;
	    groupIds_ = groupIds;
	  }
	  
	  public static User2MiniGroupInfo deSerialize(final byte[] byteArray) {
	    User2MiniGroupInfo results = new User2MiniGroupInfo();
	    User2MiniGroupData user2MiniGroup = null;
      try {
        user2MiniGroup = User2MiniGroupData.parseFrom(byteArray);
        results = new User2MiniGroupInfo(user2MiniGroup.getUserid(), user2MiniGroup.getGroupidsList());
      } catch (InvalidProtocolBufferException e) {
        e.printStackTrace();
      }
	    return results;
	  }
	}
	
	private TripodClient miniGroupTripodClient = 
	    new TripodClient(TRIPOD_ZK_ADDRESS, TRIPOD_ZK_NAMESPACE, "MiniGroup");
	private TripodClient user2MiniGroupIdsTripodClient = 
	    new TripodClient(TRIPOD_ZK_ADDRESS, TRIPOD_ZK_NAMESPACE, "User2MiniGroup");
	
	public List<MiniGroupInfo> getMiniGroupInfoWithIds(final List<Long> miniGroupIds) {
		List<MiniGroupInfo> results = new ArrayList<MiniGroupInfo>();
		List<String> missedKeys = new ArrayList<String>();
		List<String> keys = new ArrayList<String>();
		for(Long longKey : miniGroupIds){
		  keys.add(Long.toString(longKey));
		}
		Map<String, byte[]> dataMap = miniGroupTripodClient.get(keys, missedKeys);
		miniGroupTripodClient.load(missedKeys, 0, false);
		for (String key : keys) {
			if (dataMap.containsKey(key)) {
				MiniGroupInfo miniGroupInfo = MiniGroupInfo.deSerialize(dataMap.get(key));
				results.add(miniGroupInfo);
			}
		}
		return results;
	}
	
	public List<MiniGroupInfo> getJoinedMiniGroupIds(int userId) {
	  List<MiniGroupInfo> results = new ArrayList<MiniGroupInfo>();
	  List<String> missedKeys = new ArrayList<String>();
    List<String> keys = new ArrayList<String>();
    keys.add(Integer.toString(userId));
    
    Map<String, byte[]> dataMap = user2MiniGroupIdsTripodClient.get(keys, missedKeys);
    user2MiniGroupIdsTripodClient.load(missedKeys, 0, false);
    for (String key : keys) {
      if (dataMap.containsKey(key)) {
        User2MiniGroupInfo user2MiniGroupInfo = User2MiniGroupInfo.deSerialize(dataMap.get(key));
        results.addAll(getMiniGroupInfoWithIds(user2MiniGroupInfo.getMiniGroupIds()));
      }
    }

		return results;
	}
}
