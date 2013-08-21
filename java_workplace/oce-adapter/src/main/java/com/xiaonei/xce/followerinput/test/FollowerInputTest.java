package com.xiaonei.xce.followerinput.test;

import com.xiaonei.xce.followerinput.FollowerInputAdapter;

public class FollowerInputTest {
	public static void main(String args[]) {
		System.setProperty("isTestSearchChannel", "true");
		int id = 67067835;

		int[] x = FollowerInputAdapter.getInstance().getFollowerInput(id,
				"wanghe", 10);
		System.out.println(x.length);
		for (int u : x)
			System.out.println(u);

		System.exit(0);
	}
}
