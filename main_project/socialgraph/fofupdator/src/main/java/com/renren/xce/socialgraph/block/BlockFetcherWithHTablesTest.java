/*
 * Function:  SocialGraph blocking filters with data from htables
 * Developer: XiongJunWu
 * EMail: junwu.xiong@renren-inc.com  
 */

package com.renren.xce.socialgraph.block;

import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Set;

import xce.socialgraph.util.Clogging;

import com.renren.xce.socialgraph.common.BlockFetcher;
import com.renren.xce.socialgraph.data.BlockItemsOfFriendList;
import com.renren.xce.socialgraph.data.BlockItemsOfFriendListDeleted;
import com.renren.xce.socialgraph.data.BlockItemsOfRcdItemsDeleted;
import com.xiaonei.xce.buddybyapplicantcache.BuddyByApplicantCacheAdapter;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheAccepterInfo;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheData;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheFactory;

public class BlockFetcherWithHTablesTest implements BlockFetcher {
	
	public static DefaultBuddyByApplicantCacheFactory factory = new DefaultBuddyByApplicantCacheFactory();
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static BuddyByApplicantCacheAdapter adapter = new BuddyByApplicantCacheAdapter(
			factory);
	public List<Long> getBlockList(int hostId) {
		Clogging.Debug("[BlockFetcherWithHTables] getBlockList");
		// List<Long> sgBlockList = DbUtil.getInstance().getSgBlockList(hostId);
		List<Long> sgBlockList = BlockItemsOfRcdItemsDeleted.getInstance()
				.getSgBlockList(hostId);
		Clogging.Debug("[BlockFetcherWithHTables] BlockItemsOfRcdItemsDeleted over");
		// List<Integer> friendList =
		// FriendListUtil.getInstance().getFriendList(hostId, -1);
		List<Integer> friendList = BlockItemsOfFriendList.getInstance()
				.getFriendList(hostId, -1);
		Clogging.Debug("[BlockFetcherWithHTables] BlockItemsOfFriendList over");
		
		for (Integer friendId : friendList) {
			sgBlockList.add((long) friendId);
		}

		DefaultBuddyByApplicantCacheData data = (DefaultBuddyByApplicantCacheData) adapter
				.getBuddyByApplicantCacheData(hostId, 0, -1);
		List<DefaultBuddyByApplicantCacheAccepterInfo> accepters = data
				.getAccepters();
		for (Iterator<DefaultBuddyByApplicantCacheAccepterInfo> iterator = accepters
				.iterator(); iterator.hasNext();) {
			DefaultBuddyByApplicantCacheAccepterInfo accepterInfo = iterator
					.next();
			sgBlockList.add((long) accepterInfo.getAccepter());
		}

		// Add friends deleted by host
		// List<Integer> deletedFriendsList =
		// DeletedFriendsListUtil.getInstance().getDeletedFriensList(hostId,
		// -1);
		List<Integer> friendListDeleted = BlockItemsOfFriendListDeleted
				.getInstance().getFriendListDeleted(hostId);
		Clogging.Debug("[BlockFetcherWithHTables] BlockItemsOfFriendListDeleted over");

		for (Integer id : friendListDeleted) {
			sgBlockList.add((long) id);
		}

		return sgBlockList;
	}
	public HashMap<Integer, Set<Integer>> getBlockSet(List<Integer> hostIdList) {
		// TODO Auto-generated method stub
		return null;
	}
}
