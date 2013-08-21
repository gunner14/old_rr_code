package com.xiaonei.platform.core.opt.base.chain.impl;

import java.util.HashMap;
import java.util.Iterator;

import com.xiaonei.platform.core.opt.base.chain.BizFilterConfig;

public class BizFilterConfigBase implements BizFilterConfig {

	protected HashMap<String, String> params = new HashMap<String, String>();
	
	protected String filterName;
	
	void setFilterName(String name) {
		filterName = name;
	}
	
	void setParam(String key, String value) {
		params.put(key, value);
	}
	
	@Override
	public String getFilterName() {
		return filterName;
	}

	@Override
	public String getInitParameter(String name) {
		return params.get(name);
	}

	@Override
	public Iterator<String> getInitParameterNames() {
		return params.keySet().iterator();
	}

}
