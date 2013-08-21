package com.xiaonei.platform.core.userswitch;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-6-14 下午12:30:38
 */
public class SwitchPool {

	private Map<String, UserSwitch> pool = new ConcurrentHashMap<String, UserSwitch>();
	
	public UserSwitch get(String switchId) {
		
		UserSwitch switcher = pool.get(switchId);
		if (switcher == null) {
			synchronized (switchId.intern()) {
				switcher = pool.get(switchId);
				if (switcher == null) {
					switcher = initSwitch(switchId);
					pool.put(switchId, switcher);
				}
			}
		}
		return switcher;
	}

	private UserSwitch initSwitch(String switchId) {
		DefaultSwitcher s = new DefaultSwitcher();
		s.setId(switchId);
		s.init();
		return s;
	}
	
}
