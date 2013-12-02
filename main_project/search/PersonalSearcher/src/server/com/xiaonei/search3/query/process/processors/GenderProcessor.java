package com.xiaonei.search3.query.process.processors;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.lucene.index.Term;
import org.apache.lucene.search.Query;
import org.apache.lucene.search.TermQuery;

import com.xiaonei.search2.model.UserFieldNames;

/**
 * 
 * @author xuemin.luo xuemin.luo@opi-corp.com
 **/
public class GenderProcessor {
	public static List<Query> process(Map<String, Map<String, String>> map,
			float boost) {
		// 解析成QueryListMap
		List<Query> queryList = parseBasic(map);
		// 添加boost
		for (Query query : queryList) {
			addBoost(query, boost);
		}
		return queryList;
	}

	public static String getType() {
		return "gender";
	}

	private static List<Query> parseBasic(Map<String, Map<String, String>> map) {
		ArrayList<Query> queryList = new ArrayList<Query>();
		for (Entry<String, Map<String, String>> outerEntry : map.entrySet()) {
			// [gender-->[gender-->0]]
			String typeOuter = outerEntry.getKey();
			if (typeOuter.equals("gender")) {
				// [gender-->0]
				for (Entry<String, String> innerEntry : outerEntry.getValue()
						.entrySet()) {
					String typeInner = innerEntry.getKey();
					if (typeInner.equals("gender")) {
						Query query = new TermQuery(new Term(UserFieldNames.GENDER,
								innerEntry.getValue()));
						queryList.add(query);
					}
					break;// 很重要
				}
				break;// 很重要
			}
		}
		return queryList;
	}

	private static Query addBoost(Query query, float boost) {
		query.setBoost(boost);
		return query;
	}

	public static void main(String args[]) {
		Map<String, Map<String, String>> outerMap = new HashMap<String, Map<String, String>>();
		Map<String, String> innerMap = new HashMap<String, String>();
		innerMap.put("gender", "0");
		innerMap.put("gender", "1");
		outerMap.put("gender", innerMap);
		List<Query> queryAll = GenderProcessor.process(outerMap, 2.0f);
		for (Query query : queryAll) {
			System.out.println(query.toString());
		}
	}
}
