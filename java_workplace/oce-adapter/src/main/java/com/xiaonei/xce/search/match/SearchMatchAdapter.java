package com.xiaonei.xce.search.match;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Random;
import java.util.Set;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.util.channel.Channel;

import com.renren.search.MatchManagerPrx;
import com.renren.search.MatchManagerPrxHelper;
import com.renren.search.MatchResult;

public class SearchMatchAdapter extends ReplicatedClusterAdapter {

	public static final int THREAD_SIZE = 8;

	private static final String sil_endpoints = "ControllerMatchManager";

	private static int _interval = 120;

	private static SearchMatchAdapter instance = new SearchMatchAdapter();
	
	private static Random random = new Random();

	private SearchMatchAdapter() {
		super(sil_endpoints, _interval, Channel
				.newGadgetsChannel(sil_endpoints));
	}

	public static SearchMatchAdapter getInstance() {
		return instance;
	}

	public MatchManagerPrx getMatchManagerTwoway() {
		Ice.ObjectPrx prx0 = getValidProxy(random.nextInt(2), 3000);
		if (prx0 != null) {
			MatchManagerPrx prx = MatchManagerPrxHelper.uncheckedCast(prx0
					.ice_timeout(300));
			return prx;
		}
		System.out.println("prx0 is null");
		return null;
	}

	/**
	 * map里的 key value分别代表
	 * key 唯一标识
	 * value 需要匹配的字符串
	 * @param queryMap
	 * @return Map<String, MatchResult[]> key 代表唯一标识，value 代表匹配到的结果
	 */
	public Map<String, MatchResult[]> matchQuery(Map<String, String> queryMap) {

		MatchManagerPrx matchManagerPrx = SearchMatchAdapter.getInstance()
				.getMatchManagerTwoway();
		Map<String, MatchResult[]> resultMap = matchManagerPrx
				.searchMatch(queryMap);
		
		//下面是对匹配到的结果去重
		Set<Entry<String, MatchResult[]>> entrySet = resultMap.entrySet();
		for (Entry<String, MatchResult[]> resultEntry : entrySet) {
			MatchResult[] results = resultEntry.getValue();
			List<MatchResult> list = new ArrayList<MatchResult>();
			for (MatchResult outer_result : results) {
				int matchedStringLength = outer_result.matchedString.length();
				int end = outer_result.end;
				int begin = end - matchedStringLength;
				boolean add_to = true;

				for (MatchResult inner_result : results) {
					int inner_matchStringLength = inner_result.matchedString
							.length();
					if (outer_result.wikiId != inner_result.wikiId
							&& inner_result.end - inner_matchStringLength <= begin
							&& inner_result.end >= end
							&& inner_matchStringLength > matchedStringLength) {
						add_to = false;
						break;
					}
//					if (outer_result.wikiId != inner_result.wikiId
//							&& inner_result.end - inner_matchStringLength <= begin
//							&& inner_result.end >= end
//							&& inner_matchStringLength > matchedStringLength) {
//						add_to = false;
//						break;
//					}
				}
				if (add_to) {
					list.add(outer_result);
				}
			}
			resultEntry.setValue(list.toArray(new MatchResult[list.size()]));
		}

		return resultMap;
	}
}
