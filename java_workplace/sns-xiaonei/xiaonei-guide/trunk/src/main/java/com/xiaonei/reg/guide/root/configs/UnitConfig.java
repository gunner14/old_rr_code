package com.xiaonei.reg.guide.root.configs;

import java.util.HashMap;
import java.util.Map;

import com.xiaonei.reg.guide.root.models.Unit;
import com.xiaonei.reg.guide.util.GuideLogger;


public class UnitConfig {
	private static final Map<String,Unit> unitMap = new HashMap<String,Unit>();
	
	public static Unit getUnitById(String id){
		Unit u = unitMap.get(id);
		if(u == null){
			GuideLogger.printLog("[No Unit Found!] id:"+id+" return default.",5);
			u = DefaultConfig.getDefaultUnit();
		}
		return u;
	}
	
	public static Unit getEndPointUnit(){
		Unit u = unitMap.get("0");
		if(u == null){
			u =  DefaultConfig.getDefaultEndpointUnit();
		}
		return u;
	}
	static {
		reload();
	}
	
	public static void reload(){
		unitMap.clear();
		
		//reading guide_modules.xml
		Unit m0 = new Unit("0","STEP_COMPLETE_GUIDE", "complete_guide", "/guide.do", "");
		Unit m1 = new Unit("1","STEP_FILLINFO", "FillInfo", "/gfillinfo.do", "");
		Unit m2 = new Unit("2","STEP_PREVIEW", "Preview", "/gpreview.do?h=true", "");
		Unit m10 = new Unit("10","STEP_IMPORT_MSN", "ImportMsn", "/gimport.do", "");
		Unit m3 = new Unit("3","STEP_UPLOAD_HEAD", "Upload", "/gportrait.do", "");
		Unit m4 = new Unit("4","STEP_4399_FILLINFO", "ABC", "/setp4.do", "");
		Unit m5 = new Unit("5","STEP_APP", "DEF", "/step5.do", "");
		
		Unit m6 = new Unit("6",StepStatusDefinition.STR_STEP_FLASH_APP_OPEN, "FlashOpen", "/flash_farm.do", "");
		Unit m7 = new Unit("7",StepStatusDefinition.STR_STEP_FILLINFO, "FlashFillinfo", "/flash_farm.do", "");
		Unit m8 = new Unit("8",StepStatusDefinition.STR_STEP_PREVIEW, "FlashPreview", "/flash_farm.do", "");
		Unit m9 = new Unit("9",StepStatusDefinition.STR_STEP_UPLOAD_HEAD, "FlashUpload", "/flash_farm.do", "");
		
		
		Unit m11 = new Unit("11",StepStatusDefinition.STR_STEP_FILLINFO, "NetbarFillinfo", "finb", "");

		unitMap.put(m0.getId(), m0);
		unitMap.put(m1.getId(), m1);
		unitMap.put(m2.getId(), m2);
		unitMap.put(m3.getId(), m3);
		unitMap.put(m4.getId(), m4);
		unitMap.put(m5.getId(), m5);
		unitMap.put(m6.getId(), m6);
		unitMap.put(m7.getId(), m7);
		unitMap.put(m8.getId(), m8);
		unitMap.put(m9.getId(), m9);
		unitMap.put(m10.getId(), m10);
		unitMap.put(m11.getId(), m11);
	}
	
	public static void printAll(){
		GuideLogger.printLog("All Unit -- start -- contains:"+unitMap.size());
		for(String s: unitMap.keySet()){
			Unit u = unitMap.get(s);
			u.printAll();
		}
		GuideLogger.printLog("All Unit -- end -- contains:"+unitMap.size());
	}
}
