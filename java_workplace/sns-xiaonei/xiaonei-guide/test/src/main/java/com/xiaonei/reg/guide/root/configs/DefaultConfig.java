package com.xiaonei.reg.guide.root.configs;

import com.xiaonei.reg.guide.root.models.Flow;
import com.xiaonei.reg.guide.root.models.Module;
import com.xiaonei.reg.guide.root.models.Unit;

public class DefaultConfig {
	/** 默认的一个模块*/
	private static final Unit defaultUnit = new Unit("hc_99", "STEP_GUIDE", "guide.do", "/guide.do", "this is the default unit!"); 
	private static final Unit defaultEndpoint = new Unit("hc_0", "STEP_COMPLETE_GUIDE", "end to guide.do", "/guide.do", "this is the default endpoint unit!");
	private static final Flow defaultFlow = new Flow();
	private static final Module defaultModule = new Module(defaultUnit,true,true);
	
	static {
		defaultFlow.add(defaultModule);
		defaultFlow.setName("hc_default_flow");
		defaultFlow.setId("hc_0");
	}
	public static Unit getDefaultUnit() {
		return defaultUnit;
	}
	public static Flow getDefaultFlow() {
		return defaultFlow;
	}
	public static Module getDefaultModule() {
		return defaultModule;
	}
	public static Unit getDefaultEndpointUnit(){
		return defaultEndpoint;
	}
	
	
}