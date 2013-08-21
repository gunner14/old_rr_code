package com.xiaonei.reg.guide.root.configs;

import java.util.HashMap;
import java.util.Map;

import com.xiaonei.reg.guide.root.models.Flow;
import com.xiaonei.reg.guide.root.models.Mapping;
import com.xiaonei.reg.guide.util.GuideLogger;

public class MappingConfig {
	public static final Map<String,Mapping> map = new HashMap<String,Mapping>();
	public static void reloadConfig(){
		map.clear();
		
		Mapping m = new Mapping();
		m.setFromsource("S_IDREG;");
		m.setFlowname("common_flow");
		
		Mapping m1 = new Mapping();
		m1.setFromsource("S_FARMREG");
		m1.setFlowname("app_farm_flash_flow");
		
		
		map.put(m.getFromsource(), m);
		map.put(m1.getFromsource(), m1);
	}
	
	static {
		reloadConfig();
	}
	
	public static Flow getFlowByFromsource(String fromsource){
		if(fromsource == null || "".equals(fromsource)){
			//TODO
			FlowConfig.getFlowByName(null);
			return null;//FlowConfig.
		}
		Mapping mapping = map.get(fromsource);
		if(mapping == null){
			//TODO
			FlowConfig.getFlowByName(null);
			return null;//FlowConfig.
		}
		String flowname = mapping.getFlowname();
		
		return FlowConfig.getFlowByName(flowname);
	}
	
	public static Mapping getMappingByFromsource(String fromsource){
		return map.get(fromsource);
	}
	
	public static void printAll(){
		GuideLogger.printLog("mapping - contains "+map.size()+"");
		for(String key : map.keySet()){
			GuideLogger.printLog("<"+key+","+map.get(key).getFlowname()+">");
			
		}
		GuideLogger.printLog("mapping - end");
	}
}
