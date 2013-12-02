/*
 * Function:  SocialGraph blocking filters with data from htables
 * Developer: XiongJunWu
 * EMail: junwu.xiong@renren-inc.com  
 */

package com.renren.xce.socialgraph.block;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;

import com.renren.xce.socialgraph.common.BlockFetcher;
import com.renren.xce.socialgraph.data.BlockItemsOfFriendList;
import com.renren.xce.socialgraph.data.BlockItemsOfFriendListDeleted;
import com.renren.xce.socialgraph.data.BlockItemsOfRcdItemsDeleted;
import com.xiaonei.xce.buddybyapplicantcache.BuddyByApplicantCacheAdapter;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheAccepterInfo;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheData;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheFactory;

public class BlockFetcherWithHTables implements BlockFetcher {
	public static DefaultBuddyByApplicantCacheFactory factory = new DefaultBuddyByApplicantCacheFactory();
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static BuddyByApplicantCacheAdapter adapter = new BuddyByApplicantCacheAdapter(
			factory);
 
	public List<Long> getBlockList(int hostId) {
		// List<Long> sgBlockList = DbUtil.getInstance().getSgBlockList(hostId);
		List<Long> sgBlockList = BlockItemsOfRcdItemsDeleted.getInstance()
				.getSgBlockList(hostId);
		// List<Integer> friendList =
		// FriendListUtil.getInstance().getFriendList(hostId, -1);
		List<Integer> friendList = BlockItemsOfFriendList.getInstance()
				.getFriendList(hostId, -1);

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
		for (Integer id : friendListDeleted) {
			sgBlockList.add((long) id);
		}

		return sgBlockList;
	}
	
	public HashMap<Integer,Set<Integer>> getBlockSet(List<Integer> hostIdList) {
		// List<Long> sgBlockList = DbUtil.getInstance().getSgBlockList(hostId);
		HashMap<Integer, Set<Integer>> orgBlogListMap = new HashMap<Integer, Set<Integer>>();
		HashMap<Integer,Set<Integer>> sgBlockListMap = BlockItemsOfRcdItemsDeleted.getInstance()
				.getSgBlockList(hostIdList);
		
		mergeMap(orgBlogListMap, sgBlockListMap);
		
		// List<Integer> friendList =
		// FriendListUtil.getInstance().getFriendList(hostId, -1);
		HashMap<Integer,Set<Integer>> friendListMap = BlockItemsOfFriendList.getInstance()
				.getFriendList(hostIdList, -1);

		mergeMap(orgBlogListMap, friendListMap);

		HashMap<Integer,Set<Integer>> friendListDeletedMap = BlockItemsOfFriendListDeleted
		.getInstance().getFriendListDeleted(hostIdList);
		
		mergeMap(orgBlogListMap, friendListDeletedMap);

		// Add friends deleted by host
		// List<Integer> deletedFriendsList =
		// DeletedFriendsListUtil.getInstance().getDeletedFriensList(hostId,
		// -1);
		for(Integer hostId : hostIdList){
			DefaultBuddyByApplicantCacheData data = (DefaultBuddyByApplicantCacheData) adapter
					.getBuddyByApplicantCacheData(hostId, 0, -1);
			List<DefaultBuddyByApplicantCacheAccepterInfo> accepters = data
					.getAccepters();
			HashSet<Integer> accepterSet = new HashSet<Integer>();
			for (Iterator<DefaultBuddyByApplicantCacheAccepterInfo> iterator = accepters
					.iterator(); iterator.hasNext();) {
				DefaultBuddyByApplicantCacheAccepterInfo accepterInfo = iterator
						.next();
				accepterSet.add(accepterInfo.getAccepter());
			}
			orgBlogListMap.put(hostId, accepterSet);
		}


		return orgBlogListMap;
	}
	
	private void mergeMap(HashMap<Integer, Set<Integer>> orgMap, HashMap<Integer, Set<Integer>> newMap){
		if(orgMap == null || newMap == null){
			return;
		}
		Iterator<Integer> iterNew = newMap.keySet().iterator();
		Integer key;
		Set<Integer> orgSet, newSet;
		while(iterNew.hasNext()){
			key = iterNew.next();
			if(!orgMap.containsKey(key)){
				orgMap.put(key, newMap.get(key));
			}else{
				newSet = newMap.get(key);
				orgSet = orgMap.get(key);
				if(orgSet != null){
					orgSet.addAll(newSet);
				}else{
					orgSet = newSet;
				}
				orgMap.put(key, orgSet);
			}
		}
	}
	
	public static void main(String[]args){
		ArrayList<Integer> hostIdList = new ArrayList<Integer>();
		hostIdList.add(256404211);
		hostIdList.add(221177832);
		HashMap<Integer, Set<Integer>> datas;
		try {
			Long s2 = System.currentTimeMillis();
			datas = new BlockFetcherWithHTables().getBlockSet(hostIdList);
			Long s3 = System.currentTimeMillis();
			System.out.println("---------Success-!"
					+ " Time cost: " + (s3 - s2) + "ms");
			for(Integer host : hostIdList){
				Set<Integer> ss = datas.get(host);
				System.out.println("UserId: " + host
						+ " Succeeded result data size: " + ss.size());
				for(Integer i : ss){
					System.out.println( i + "," );
				}
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
}
