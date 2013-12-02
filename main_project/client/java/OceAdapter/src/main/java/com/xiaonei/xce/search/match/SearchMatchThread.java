package com.xiaonei.xce.search.match;

import java.util.Map;
import java.util.TreeMap;
import java.util.Map.Entry;

import com.renren.search.MatchResult;

public class SearchMatchThread extends Thread{

	@Override
	public void run() {
		// TODO Auto-generated method stub
		Map<String, String> queryMap = new TreeMap<String, String>();
		queryMap.put(
				"1111ff",
				"北京大学， Insti著名企业家tuto S马天宇uperi人民网or 改名了改来改去改多了 Politécnico José Antonio Echeverríabeijing清华大学中国矿业大学（北京）Universidad de Cienfuegos农业东北大学，中国农业大学");
		queryMap.put("2222ff", "222清华大学111");
		Map<String, MatchResult[]> map = SearchMatchAdapter.getInstance()
				.matchQuery(queryMap);
		System.out.println(Thread.currentThread().getId() + " size: " + map.get("1111ff").length + " map: " + map);
		System.out.println(Thread.currentThread().getId() + " size: " + map.get("2222ff").length + " map: " + map);
//		for (Entry<Integer, MatchResult[]> entry : map.entrySet()) {
//			int id = entry.getKey();
//			MatchResult[] matchResults = entry.getValue();
//			System.out.println("id: " + id);
//			for (MatchResult matchResult : matchResults) {
//				System.out.println("matchedString: " + matchResult.matchedString
//						+ " wikiid: " + matchResult.wikiId
//						+ " start:" + (matchResult.end + 1 - matchResult.matchedString.length())
//						+ " end:" + matchResult.end);
//			}
//		}
	}
	
}
