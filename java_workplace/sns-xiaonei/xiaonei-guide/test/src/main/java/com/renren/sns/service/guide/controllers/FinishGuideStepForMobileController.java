package com.renren.sns.service.guide.controllers;

import com.xiaonei.commons.interceptors.access.annotation.IgnorePassportValidation;
import com.xiaonei.platform.core.utility.Encoder;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import net.sf.json.JSONObject;


@Path("finishGuideStepForMobile")
@IgnorePassportValidation
public class FinishGuideStepForMobileController {

	@Get
	@Post
	public String index(Invocation inv ,@Param("userId") int userId,@Param("token") String token){
		
		//验证参数
		JSONObject rtnJson = new JSONObject();
		
		if(!Encoder.encode(Encoder.encode(String.valueOf(userId))+"ADFPkdfoeSDF832#&(MKWDFLkd)*$%la").equals(token)){
			rtnJson.put("code", "1");
    		rtnJson.put("message", "Token非法");
    		GuideLogger.printLog("Token非法");
    		return"@"+rtnJson.toString();
		}
		
		StepStatusLogic.updateStepStatus(userId, StepStatusLogic.STEP_COMPLETE_GUIDE);
		rtnJson.put("code", "0");
		rtnJson.put("message", "sucess");
		return"@"+rtnJson.toString();
	}
	
	
}
