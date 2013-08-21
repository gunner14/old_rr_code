package com.xiaonei.prometheus.storage;

import java.util.Map;

public interface Store {
	public Map get(String key);
	public boolean put(String key,Map cols);
	
}
