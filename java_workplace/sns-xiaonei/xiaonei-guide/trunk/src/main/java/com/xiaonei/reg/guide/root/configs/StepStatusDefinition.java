package com.xiaonei.reg.guide.root.configs;

import java.util.HashMap;
import java.util.Map;

public class StepStatusDefinition implements IStepStatusDefinition{
	
	private static final Map<String,Integer>  map = new HashMap<String,Integer>(); 
	static{
		map.put("STEP_FILLINFO", STEP_FILLINFO);
		map.put("STEP_UPLOAD_HEAD", STEP_UPLOAD_HEAD);
		map.put("STEP_IMPORT_MSN", STEP_IMPORT_MSN);
		map.put("STEP_PREVIEW", STEP_PREVIEW);
		map.put("STEP_GUIDE", STEP_GUIDE);
		map.put("STEP_FILLINFO_MOBILE", STEP_FILLINFO_MOBILE);
		map.put("STEP_VERIFY_MOBILE", STEP_VERIFY_MOBILE);
		map.put("STEP_COMPLETE_GUIDE", STEP_COMPLETE_GUIDE);
		map.put("STEP_4399_FILLINFO", STEP_4399_FILLINFO);
		map.put("STEP_FILLINFO_STAGE_MOBILE", STEP_FILLINFO_STAGE_MOBILE);
		map.put("STEP_APP", STEP_APP);
		map.put("STEP_FLASH_APP_OPEN", STEP_FLASH_APP_OPEN);
	}
	
	public static int getBinaryFlag(String stepflagname) {
		return map.get(stepflagname);
	}

}

