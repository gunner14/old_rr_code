package com.xiaonei.xce.generalsuggestion.view;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import xce.generalsuggestion.IndexItem;
import xce.generalsuggestion.SuggestionResult;

import com.xiaonei.xce.generalsuggestion.condition.SuggestionType;

public class SecondDegreeFriendsSuggestionDataView {

	private List<SecondDegreeFriendsData> datas = new ArrayList<SecondDegreeFriendsData>();;

	public SecondDegreeFriendsSuggestionDataView() {
	}

	public SecondDegreeFriendsSuggestionDataView(SuggestionResult result) {
		if (result != null) {
			for (IndexItem item : result.results) {
				for (Map<String, String> doc : item.docs) {
					int id = -1;
					String idStr = doc.get(SuggestionType.RESULT_ID);
					if (idStr != null && !idStr.isEmpty()) {
						id = Integer.parseInt(idStr);
					}
					String name = doc.get(SuggestionType.RESULT_NAME);
					String commonNumStr = doc.get(SuggestionType.COMMON_NUM);
					int commonNum = 0;
					if (commonNumStr != null && !commonNumStr.isEmpty()) {
						commonNum = Integer.parseInt(commonNumStr);
					}
					SecondDegreeFriendsData data = new SecondDegreeFriendsData(
							id, name, commonNum);
					datas.add(data);
				}
			}
		}
	}

	public List<SecondDegreeFriendsData> getData() {
		return datas;
	}

	public static class SecondDegreeFriendsData {
		private int id;
		private String name;
		private int commonNum;

		public SecondDegreeFriendsData(int id, String name, int commonNum) {
			this.id = id;
			this.name = name;
			this.commonNum = commonNum;
		}

		public int getId() {
			return this.id;
		}

		public String getName() {
			return this.name;
		}

		@Override
		public String toString() {
			return "ID: " + id + "  Name: " + name + "  CommonNum: "
					+ commonNum;
		}

		public void setCommonNum(int commonNum) {
			this.commonNum = commonNum;
		}

		public int getCommonNum() {
			return commonNum;
		}
	}
}
