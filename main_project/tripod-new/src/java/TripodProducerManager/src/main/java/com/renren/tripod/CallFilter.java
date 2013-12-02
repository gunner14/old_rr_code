package com.renren.tripod;

import java.util.HashMap;

import com.xiaonei.xce.log.Tripod;

public class CallFilter {

	private class CountPair {
		private int _curCount = 0;
		private int _maxCount;

		CountPair(int maxCount) {
			_maxCount = maxCount;
		}

		synchronized boolean inc() {
			return ++_curCount <= _maxCount;
		}

		synchronized void dec() {
			--_curCount;
		}
	}

	private class FilterHolder {
		private HashMap<String, CountPair> _filterMap = new HashMap<String, CountPair>();

		public FilterHolder() {
			// add business config here
			_filterMap.put("BADOO", new CountPair(10));
			_filterMap.put("VIP_RECALL_ACTIVE_USER", new CountPair(10));
			_filterMap.put("VIP_RECALL_LOGIN_USER", new CountPair(10));
			_filterMap.put("newbie_helper", new CountPair(10));

		}

		public CountPair getCounter(String businessId) {
			return _filterMap.get(businessId);
		}
	}

	FilterHolder _filterHolder = new FilterHolder();

	public boolean filter(String businessId) throws TripodProducerNotSupportedException {
		CountPair cp = _filterHolder.getCounter(businessId);
		if (cp != null) {
			if (cp.inc()) {
				return true;
			} else {
				Tripod.getLogger().error(
						"CallFilter.filter too many call for business "
								+ businessId + " max is " + cp._maxCount);
			}
		} else {
			throw new TripodProducerNotSupportedException();
		}
		return false;
	}

	public void exitFilter(String businessId) {
		CountPair cp = _filterHolder.getCounter(businessId);
		if (cp != null) {
			cp.dec();
		}
	}

}
