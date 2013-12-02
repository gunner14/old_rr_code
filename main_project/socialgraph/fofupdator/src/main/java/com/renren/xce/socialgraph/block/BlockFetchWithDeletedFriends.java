package com.renren.xce.socialgraph.block;

import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Set;

import xce.socialgraph.recommend.DbUtil;
import xce.socialgraph.recommend.DeletedFriendsListUtil;
import xce.socialgraph.recommend.FriendListUtil;

import com.renren.xce.socialgraph.common.BlockFetcher;
import com.xiaonei.xce.buddybyapplicantcache.BuddyByApplicantCacheAdapter;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheAccepterInfo;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheData;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheFactory;

public class BlockFetchWithDeletedFriends implements BlockFetcher {
	public static DefaultBuddyByApplicantCacheFactory factory = new DefaultBuddyByApplicantCacheFactory();
	@SuppressWarnings({ "unchecked", "rawtypes" })
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
		}		
		
		//Add friends deleted by host
		List<Integer> deletedFriendsList = DeletedFriendsListUtil.getInstance().getDeletedFriensList(hostId, -1);
		for (Integer id : deletedFriendsList) {
			sgBlockList.add((long)id);
		}
		
		return sgBlockList;
	}

	public HashMap<Integer, Set<Integer>> getBlockSet(List<Integer> hostIdList) {
		// TODO Auto-generated method stub
		return null;
	}

}
