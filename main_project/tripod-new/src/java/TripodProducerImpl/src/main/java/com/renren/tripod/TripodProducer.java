package com.renren.tripod;

import java.util.Map;

public interface TripodProducer {
	public Map<String, byte[]> create(String[] keys, boolean isMaster);
	public String[] createKeys(String beginKey, int limit);
	public String[] createHotKeys(String beginKey, int limit);
}
