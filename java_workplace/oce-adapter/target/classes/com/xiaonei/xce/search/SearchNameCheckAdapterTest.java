package com.xiaonei.xce.search;

import java.util.Arrays;

public class SearchNameCheckAdapterTest {
	public static void main(String[] args) {
		/*
		 * int fromUserId0 = 25701953, toUserId0 = 58742, fromUserId1 =
		 * 34355462, toUserId1 = 34343523, fromUserId2 = 44444443, toUserId2 =
		 * 9999999; Operation op = Operation.Add;
		 */
		System.setProperty("isTestSearchChannel", "true");
		
		String[] name = {"北京","张华","电脑","王旭"};
		boolean[] res = SearchNameCheckAdapter.getInstance().CheckBatch(name);
		System.out.println("res:" + Arrays.toString(res));
		try {
			Thread.sleep(100);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
