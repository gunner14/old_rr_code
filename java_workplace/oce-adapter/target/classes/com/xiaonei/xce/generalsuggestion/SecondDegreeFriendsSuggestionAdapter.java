package com.xiaonei.xce.generalsuggestion;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import xce.clusterstate.ClientInterface;
import xce.clusterstate.ReplicatedClusterAdapter;
import xce.generalsuggestion.SearchManagerPrx;
import xce.generalsuggestion.SearchManagerPrxHelper;
import xce.generalsuggestion.SuggestionResult;
import xce.util.channel.Channel;

import com.xiaonei.xce.generalsuggestion.condition.SuggestionCondition;
import com.xiaonei.xce.generalsuggestion.condition.SuggestionType;
import com.xiaonei.xce.generalsuggestion.view.SecondDegreeFriendsSuggestionDataView;
import com.xiaonei.xce.generalsuggestion.view.SuggestionDataView;

public class SecondDegreeFriendsSuggestionAdapter extends
		ReplicatedClusterAdapter {
	private static Log logger = LogFactory
			.getLog(SecondDegreeFriendsSuggestionAdapter.class);
	private static final String ctr_endpoints = "ControllerSecondDegreeFriendSuggestion";

	private static int _interval = 120;
	private static final int TIMEOUT = 10000;

	public static SecondDegreeFriendsSuggestionAdapter instance = new SecondDegreeFriendsSuggestionAdapter();

	public static SecondDegreeFriendsSuggestionAdapter getInstance() {
		return instance;
	}

	private SecondDegreeFriendsSuggestionAdapter() {
		super(ctr_endpoints, _interval, Channel.newGadgetsChannel(ctr_endpoints));
	}

	public ClientInterface getClientInterface() {
		return _csClient;
	}

	private SearchManagerPrx getSearchManager(int owner) {
		Ice.ObjectPrx prx0 = getValidProxy(owner, TIMEOUT);
		if (prx0 != null) {
			SearchManagerPrx prx = SearchManagerPrxHelper.uncheckedCast(prx0
					.ice_timeout(TIMEOUT).ice_twoway());
			return prx;
		}
		System.out.println("prx0 is null");
		return null;
	}

	public SecondDegreeFriendsSuggestionDataView search(
			SuggestionCondition condition) {
		return search(condition.getUserId(), condition.getCondition(),
				condition.getBegin(), condition.getLimit(),
				condition.getFields());
	}

	public SecondDegreeFriendsSuggestionDataView search(int userid,
			String condition, int begin, int limit, String[] types) {
		SearchManagerPrx proxy = getSearchManager(userid);
		SecondDegreeFriendsSuggestionDataView view = null;
		if (proxy != null) {
			SuggestionResult result = proxy.search(userid, condition, begin,
					limit, types);
			view = new SecondDegreeFriendsSuggestionDataView(result);

		} else {
			view = new SecondDegreeFriendsSuggestionDataView();
			logger.info("代理为空");
		}
		return view;
	}

	public void load(int userid) {
		SearchManagerPrx proxy = getSearchManager(userid);
		if (proxy != null) {
			try {
				proxy.load(userid, null, null);
			} catch (Exception e) {
				logger.info("load id=" + userid + "异常");
			}
		}
	}

}
