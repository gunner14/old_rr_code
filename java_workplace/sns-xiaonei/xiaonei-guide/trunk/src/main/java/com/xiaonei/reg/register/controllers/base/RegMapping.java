package com.xiaonei.reg.register.controllers.base;

import java.util.HashMap;
import java.util.Map;

public class RegMapping {
	private Map<String, RegForward> mapping = new HashMap<String, RegForward>();

	public RegMapping(Map<String, RegForward> mapping){
		set(mapping);
	}
	public RegMapping set(Map<String, RegForward> mapping) {
		if(mapping == null){
			return this;
		}
		this.mapping.putAll(mapping);
		return this;
	}

	public Map<String, RegForward> get() {
		return mapping;
	}

	public RegForward findForward(String name) {
		return mapping.get(name);
	}
}
