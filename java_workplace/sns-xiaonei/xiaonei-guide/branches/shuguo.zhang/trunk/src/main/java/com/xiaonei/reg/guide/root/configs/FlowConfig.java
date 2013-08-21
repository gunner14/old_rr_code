package com.xiaonei.reg.guide.root.configs;

import java.util.HashMap;
import java.util.Map;

import com.xiaonei.reg.guide.root.models.Flow;
import com.xiaonei.reg.guide.root.models.Module;
import com.xiaonei.reg.guide.util.GuideLogger;

/**
 * 保存流程配置的类
 * 
 * 约定：
 * 
 * @author inshion
 *
 */
public class FlowConfig {
	private static final Map<String,Flow> flowMap = new HashMap<String,Flow>();
	private static final Map<String,String> flowIdMap = new HashMap<String, String>();
	
	public static final String DEFAULT_FLOW_ID = "0";
	public static final String DEFAULT_FLOW_NAME = "default";
	
	public static Flow getFlowByName(String name){;
		Flow flow = null; 
		if(name == null){
			GuideLogger.printLog("[name is null!] name:null"+" return default.",5);
			flow = DefaultConfig.getDefaultFlow();
		}
		else {
			flow = flowMap.get(name);
			if(flow == null){
				GuideLogger.printLog("[No Flow Found!] name:"+name+" return default.",5);
				flow = DefaultConfig.getDefaultFlow();
			}
		}
		return flow;
	}
	
	public static Flow getFlowById(String id){
		String name = null;
		if(id == null){
			GuideLogger.printLog("[id is null!] id:"+"null"+" return default.",5);
		}
		name = flowIdMap.get(id);
		GuideLogger.printLog("id="+id+" name="+name);
		return getFlowByName(name);
		
	}
	
	public static Flow getConfiguredDefaultFlow(){
		Flow ret = flowMap.get(DEFAULT_FLOW_NAME);
		if(ret == null){
			//TODO
			ret = DefaultConfig.getDefaultFlow();
		}
		return ret;
	}

	public static void printAll(){
		for(String key : flowMap.keySet()){
			Flow f = flowMap.get(key);
			GuideLogger.printLog("{"+key+" contains "+f.getModules().size()+"}");
			f.printAll();
			GuideLogger.printLog("{"+key+" end "+"}");
		}
	}
	public static void reloadConfig() {
		flowMap.clear();
		
		//reading guide_flow.xml
		
		Module fm1 = new Module(UnitConfig.getUnitById("1"), false, true);
		
		Module fm2 = new Module(UnitConfig.getUnitById("2"), false, true); //推人
		
		Module fm3 = new Module(UnitConfig.getUnitById("3"), false, true);
		
		Module fm4 = new Module(UnitConfig.getUnitById("4"), false, true);
		
		Module fm5 = new Module(UnitConfig.getUnitById("5"), false, true);
		Module fm6 = new Module(UnitConfig.getUnitById("6"), false, true);
		Module fm7 = new Module(UnitConfig.getUnitById("7"), false, true);
		Module fm8 = new Module(UnitConfig.getUnitById("8"), false, true);
		Module fm9 = new Module(UnitConfig.getUnitById("9"), false, true);
		Module fm10 = new Module(UnitConfig.getUnitById("10"), false, true);
		Module fm11 = new Module(UnitConfig.getUnitById("11"), false, true);
		
		Module fm_end = new Module(UnitConfig.getEndPointUnit(), true, true);
		
		Flow f0 = new Flow();
		f0.setId(DEFAULT_FLOW_ID);
		f0.setName(DEFAULT_FLOW_NAME);
		f0.add(fm1);
		f0.add(fm_end);
		
		Flow f1 = new Flow();
		f1.setId("1");
		f1.setName("common_flow");
		f1.add(fm1);
		f1.add(fm2);
		f1.add(fm10);
		f1.add(fm3);
		f1.add(fm_end);
		
		Flow f2 = new Flow();
		f2.setId("2");
		f2.setName("app_flow");
		f2.add(fm1);
		f2.add(fm2);
		f2.add(fm5);
		f2.add(fm3);
		f2.add(fm4);
		
		Flow f3 = new Flow();
		f3.setId("3");
		f3.setName("app_farm_flash_flow");
		f3.add(fm6);
		f3.add(fm7);
		f3.add(fm8);
		f3.add(fm9);
		f3.add(fm_end);
		
		Flow f4 = new Flow();
		f4.setId("4");
		f4.setName("");
		f4.add(fm11);
		f4.add(fm_end);
		
		flowMap.put(f0.getName(), f0);
		flowMap.put(f1.getName(), f1);
		flowMap.put(f2.getName(), f2);
		flowMap.put(f3.getName(), f3);
		
		flowIdMap.put(f0.getId(), f0.getName());
		flowIdMap.put(f1.getId(), f1.getName());
		flowIdMap.put(f2.getId(), f2.getName());
		flowIdMap.put(f3.getId(), f3.getName());
		flowIdMap.put(f4.getId(), f4.getName());
		
		
	}
	static {
		reloadConfig();
	}
	
	
}


