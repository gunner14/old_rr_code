package com.xiaonei.xce.search.friendcache;

import java.util.Arrays;
import java.util.Map;

public class SearchFriendCacheAdapterTest {
	public static void main(String[] args) {
		/*
		 * int fromUserId0 = 25701953, toUserId0 = 58742, fromUserId1 =
		 * 34355462, toUserId1 = 34343523, fromUserId2 = 44444443, toUserId2 =
		 * 9999999; Operation op = Operation.Add;
		 */
		System.setProperty("isTestSearchChannel", "true");
//		
//		String[] name = {"北京","张华","电脑","王旭"};
//		boolean[] res = SearchFriendCacheAdapter.getInstance().GetSecond(name);
//		System.out.println("res:" + Arrays.toString(res));
//		try {
//			Thread.sleep(100);
//		} catch (Exception e) {
//			e.printStackTrace();
//		}
		
//		Map<Integer,Integer> res= SearchFriendCacheAdapter.getInstance().GetSecond(220678672);
//		System.out.println("res:" + res);
		
		int[] res= SearchFriendCacheAdapter.getInstance().GetSecondFriends(103113664);
		System.out.println("res:" + Arrays.toString(res));

		try {
			Thread.sleep(100);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
