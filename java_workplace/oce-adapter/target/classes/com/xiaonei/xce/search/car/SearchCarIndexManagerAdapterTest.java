package com.xiaonei.xce.search.car;

import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

import com.xiaonei.search2.model.IndexCondition;
import com.xiaonei.search2.model.IndexResult;
import com.xiaonei.search2.model.IndexType;

public class SearchCarIndexManagerAdapterTest {
	public static void main(String[] args) {
		/*
		 * int fromUserId0 = 25701953, toUserId0 = 58742, fromUserId1 =
		 * 34355462, toUserId1 = 34343523, fromUserId2 = 44444443, toUserId2 =
		 * 9999999; Operation op = Operation.Add;
		 */
//		System.setProperty("isTestSearchChannel", "true");
//		
//		String[] name = {"北京","张华","电脑","王旭"};
//		boolean[] res = SearchCarIndexManagerAdapter.getInstance().GetSecond(name);
//		System.out.println("res:" + Arrays.toString(res));
//		try {
//			Thread.sleep(100);
//		} catch (Exception e) {
//			e.printStackTrace();
//		}
		
//		Map<Integer,Integer> res= SearchCarIndexManagerAdapter.getInstance().GetSecond(220678672);
//		System.out.println("res:" + res);
		IndexCondition condition = new IndexCondition();
		condition.opUserCondition = new HashMap<String,String>();
		condition.opUserCondition.put("query","车问");
		IndexResult res= SearchCarIndexManagerAdapter.getInstance().searchIndexWithRestriction(0, condition, 0, 2, "hoho");
		System.out.println("res:" + res);

		try {
			Thread.sleep(100);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
