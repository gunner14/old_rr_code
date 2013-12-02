package com.xiaonei.xce.generalsuggestion.test;

import com.xiaonei.xce.generalsuggestion.GeneralSuggestionAdapter;
import com.xiaonei.xce.generalsuggestion.SecondDegreeFriendsSuggestionAdapter;
import com.xiaonei.xce.generalsuggestion.view.SecondDegreeFriendsSuggestionDataView;
import com.xiaonei.xce.generalsuggestion.view.SuggestionDataView;
import com.xiaonei.xce.generalsuggestion.view.SecondDegreeFriendsSuggestionDataView.SecondDegreeFriendsData;
import com.xiaonei.xce.generalsuggestion.view.SuggestionDataView.Data;

public class SecondDegreeFriendsSuggestionTest {
	public static void main(String args[]) {
		System.setProperty("isTestSearchChannel", "true");
		int id = 62522936;
		id = 193537801;
		id = 193577421;
		id = 220678672;
//		GeneralSuggestionAdapter.getInstance().load(id);

		try {
			Thread.sleep(100);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}

//		SecondDegreeFriendsSuggestionDataView result = SecondDegreeFriendsSuggestionAdapter.getInstance().search(
//				id, "李", 0, 10,
//				new String[] { "friend_name", "page_name", "app_name", "university_name", "workplace_name"});
		SecondDegreeFriendsSuggestionDataView result = SecondDegreeFriendsSuggestionAdapter.getInstance().search(
				id, "李", 0, 100,
				new String[] {});
		System.out.println("result data size:" + result.getData().size());
		for(SecondDegreeFriendsData data:result.getData()){
			System.out.println("id:" + data.getId() + " name:" + data.getName() + " commonNum:" + data.getCommonNum());
		}
//		System.out.println("Friend Result: " + result.getFriendData().size());
//		for (Data data : result.getFriendData()) {
//			System.out.println("\t" + data.toString());
//		}
//		
//		System.out.println("Page Result: " + result.getPageData().size());
//		for (Data data : result.getPageData()) {
//			System.out.println("\t" + data.toString());
//		}
//		
//		System.out.println("App Result: " + result.getAppData().size());
//		for (Data data : result.getAppData()) {
//			System.out.println("\t" + data.toString());
//		}
//		
//		System.out.println("University Result: " + result.getUniversityData().size());
//		for (Data data : result.getUniversityData()) {
//			System.out.println("\t" + data.toString());
//		}
//		
//		System.out.println("Workplace Result: " + result.getWorkplaceData().size());
//		for (Data data : result.getWorkplaceData()) {
//			System.out.println("\t" + data.toString());
//		}
		System.exit(0);
		// ArrayList prxs = repli.getAllProxy(0);
		// for (int i = 0; i < prxs.size(); ++i) {
		// System.out.println(prxs.get(i).toString());
		// }
		// Map<Integer, ServerStateInfo[]> serverStateInfoMap =
		// repli.getClientInterface().getAvailableObjProxies();
		// Set<Integer> keys = serverStateInfoMap.keySet();
		// Iterator<Integer> it = keys.iterator();
		// while(it.hasNext()) {
		// Integer integer = it.next();
		// System.out.println(integer);
		// ServerStateInfo[] serverInfo = serverStateInfoMap.get(integer);
		// for(int i = 0; i < serverInfo.length; ++i) {
		// System.out.println("name = " + serverInfo[i].servername + " identity = "
		// + serverInfo[i].identity + " cluster = "
		// + serverInfo[i].cluster + " mod = " + serverInfo[i].mod);
		// }
		// System.out.println("\n\n");
		// }
	}
}
