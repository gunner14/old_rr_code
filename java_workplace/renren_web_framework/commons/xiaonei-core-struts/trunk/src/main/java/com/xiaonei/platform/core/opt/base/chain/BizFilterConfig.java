package com.xiaonei.platform.core.opt.base.chain;

import java.util.Iterator;


public interface BizFilterConfig {

	public String getFilterName();
	
	public String getInitParameter(String name);
	
	public Iterator<String> getInitParameterNames();
}
