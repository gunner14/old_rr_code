package com.renren.xce.socialgraph.block;

import java.util.Iterator;
import java.util.List;


import xce.socialgraph.recommend.DbUtil;
import xce.socialgraph.recommend.FriendListUtil;

import com.renren.xce.socialgraph.common.BlockFetcher;
import com.xiaonei.xce.buddybyapplicantcache.BuddyByApplicantCacheAdapter;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheAccepterInfo;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheData;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheFactory;

public class BlockFetchWithBuddyApplicant implements BlockFetcher{
	public static DefaultBuddyByApplicantCacheFactory factory = new DefaultBuddyByApplicantCacheFactory();
	public static BuddyByApplicantCacheAdapter adapter = new BuddyByApplicantCacheAdapter(factory);
	public List<Long> getBlockList(int hostId) {
		List<Long> sgBlockList = DbUtil.getInstance().getSgBlockList(hostId);
		List<Integer> friendList = FriendListUtil.getInstance().getFriendList(hostId, -1);
		for(Integer friendId : friendList) {
			sgBlockList.add((long)friendId);
		}
				
		DefaultBuddyByApplicantCacheData data = (DefaultBuddyByApplicantCacheData) adapter.getBuddyByApplicantCacheData(hostId, 0, -1);
		List<DefaultBuddyByApplicantCacheAccepterInfo> accepters = data.getAccepters();
		for(Iterator<DefaultBuddyByApplicantCacheAccepterInfo> iterator = accepters.iterator(); iterator.hasNext();) {
			DefaultBuddyByApplicantCacheAccepterInfo accepterInfo = iterator.next();
			sgBlockList.add((long)accepterInfo.getAccepter());
//			System.out.println("getAccepter: " + accepterInfo.getAccepter());			
		}		
		
		return sgBlockList;
	}	
}


