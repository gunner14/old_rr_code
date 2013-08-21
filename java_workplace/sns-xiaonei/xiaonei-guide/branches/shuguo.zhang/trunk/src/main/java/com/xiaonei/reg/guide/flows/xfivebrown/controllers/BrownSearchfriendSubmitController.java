package com.xiaonei.reg.guide.flows.xfivebrown.controllers;

import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveStepCheckLogic;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.flows.xfivecoral.formbeans.CoralFormInfo;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.root.configs.IStepStatusDefinition;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;
import com.xiaonei.reg.register.logic.IRegExtraMsgLogic;
import com.xiaonei.reg.register.logic.additional.RegExtraMsgLogic;
import com.xiaonei.reg.register.model.RegExtraMsg;

@LoginRequired
@Path("sfbrown-s")
@IgnoreUserStatusImportedValidation
public class BrownSearchfriendSubmitController {
	
	private static final String SUCC = "f:lead"; 
	private final int thisStep = IStepStatusDefinition.STEP_SEARCH_FRIEND;
	
	
	@Get
	public String get(Invocation inv){
		return "r:lead";
	}
	@Post
	public String index(Invocation inv,CoralFormInfo form){
		
		String ret = SUCC;
		
		HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
		HttpServletResponse response = GuideXFiveRequestUtil.getResponse(inv);
		User host = GuideXFiveRequestUtil.getCurrentHost(request);
		
		if(request == null || response == null){
			GuideLogger.printLog(" request or response null "+(request==null)+(response==null),GuideLogger.ERROR);
		}
		else if(host == null){
			GuideLogger.printLog(" host null ",GuideLogger.ERROR);
		}
		else{
			boolean isdone = GuideXFiveStepCheckLogic.getInstance().isTheStepDone(thisStep, host);
			if(isdone){
				ret = SUCC;
			}
			else{
				GuideTimeCost cost = GuideTimeCost.begin();
				
				try {
					//改标志
					StepStatusLogic.updateStepStatus(host.getId(), thisStep);
					GuideLogger.printLog("| host:"+host.getId()+" update step:"+thisStep);
					cost.endStep();
					cost.endFinally();
					ret = SUCC;
				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				
				updateFlag(host);
		        
			}
		}
		return ret;
	}

	@SuppressWarnings("unchecked")
	private void updateFlag(User host) {
		try {
			MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_default);
			String memKey = "mem_cache_reg_extra_msg_" + host.getId();
			Object obj = mem.get(memKey);
			if(obj != null){
				Map<String, RegExtraMsg> map = (Map<String,RegExtraMsg>)obj;
				RegExtraMsg msg = map.get(IRegExtraMsgLogic.key_reg_recommond);
				if(msg != null){
					if(msg.getReg_flag() == 0){
						GuideLogger.printLog(" host:"+host.getId()+" update flag to 1");
						RegExtraMsgLogic.getInstance().asyncUpdateMsgFlagByUidAndInputKey(host, IRegExtraMsgLogic.key_reg_recommond, 1);
					}
				}
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
