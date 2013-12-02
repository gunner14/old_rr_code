package com.renren.tripod.impl;

import java.util.HashMap;
import java.util.Map;

import com.renren.tripod.TripodProducer;

public class BadooTipsTripodProducer implements TripodProducer {

	@Override
	public Map<String, byte[]> create(String[] keys, boolean isMaster) {
		// TODO Auto-generated method stub
		Map<String, byte[]> res = new HashMap<String, byte[]>();

		for (String key : keys) {
			res.put(key, "0".getBytes());
		}
		return res;
	}

	@Override
	public String[] createKeys(String beginKey, int limit) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public String[] createHotKeys(String beginKey, int limit) {
		// TODO Auto-generated method stub
		return null;
	}

}
