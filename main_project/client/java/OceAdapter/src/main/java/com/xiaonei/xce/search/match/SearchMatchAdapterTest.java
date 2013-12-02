package com.xiaonei.xce.search.match;

import java.util.Map;
import java.util.Map.Entry;
import java.util.TreeMap;

import com.renren.search.MatchResult;


public class SearchMatchAdapterTest {

	public static void main(String[] args) {
		System.setProperty("isTestSearchChannel", "true");
//		 for (int i = 0; i < 10; i++) {
//		 Thread t = new SearchMatchThread();
//		 t.start();
//		 }
		Map<String, String> queryMap = new TreeMap<String, String>();
		queryMap.put(
				"1111ff",
				"北京大学， Insti著名企业家tuto S马天宇uperi人民网or 改名了改来改去改多了 Politécnico José Antonio Echeverríabeijing清华大学中国矿业大学（北京）Universidad de Cienfuegos农业东北大学，中国农业大学");
		queryMap.put("2222ff", "很喜欢 仙剑奇侠传三 哈哈 ");
		Map<String, MatchResult[]> map = SearchMatchAdapter.getInstance()
				.matchQuery(queryMap);
		
		System.out.println("----------------------------main----------------------------");
		System.out.println(Thread.currentThread().getId() + " size: " + map.get("1111ff").length + " map: " + map);
		System.out.println(Thread.currentThread().getId() + " size: " + map.get("2222ff").length + " map: " + map);
		for (Entry<String, MatchResult[]> entry : map.entrySet()) {
			String id = entry.getKey();
			MatchResult[] matchResults = entry.getValue();
			System.out.println("id: " + id);
			for (MatchResult matchResult : matchResults) {
				System.out.println("matchedString: " + matchResult.matchedString
						+ " wikiid: " + matchResult.wikiId
						+ " start:" + (matchResult.end + 1 - matchResult.matchedString.length())
						+ " end:" + matchResult.end);
			}
		}
		System.exit(0);
	}

}
