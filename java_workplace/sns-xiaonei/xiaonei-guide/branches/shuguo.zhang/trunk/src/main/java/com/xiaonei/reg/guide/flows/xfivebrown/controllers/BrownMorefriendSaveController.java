package com.xiaonei.reg.guide.flows.xfivebrown.controllers;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveStepCheckLogic;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveSupportFriendsUtil;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.root.configs.IStepStatusDefinition;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;

@LoginRequired
@Path("mfbrown-s")
@IgnoreUserStatusImportedValidation
public class BrownMorefriendSaveController {
	
	private static final String SUCC = "f:lead"; 
	private final static int thisStep = IStepStatusDefinition.STEP_MOREFRIEND;
	
	
	@Post
	@Get
	public String index(Invocation inv){
		String ret = SUCC;
		HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
		HttpServletResponse response = GuideXFiveRequestUtil.getResponse(inv);
		User host = GuideXFiveRequestUtil.getCurrentHost(request);		
		if(request == null || response == null){
			GuideLogger.printLog(" request or response null "+(request==null)+(response==null),GuideLogger.ERROR);
		}
		else if(host == null){
			GuideLogger.printLog(" host null ",GuideLogger.ERROR);
			return "@";
		}
		else{	
			boolean isdone = GuideXFiveStepCheckLogic.getInstance().isTheStepDone(thisStep, host);
			if(isdone){
				ret = SUCC;
			}
			else{
				try {
					String skip = GuideXFiveRequestUtil.getParamString(request, "skip", host.getId());
					if(MyStringUtils.equals("1", skip)){
						GuideLogger.printLog(" host:"+host.getId()+" skipped step:"+thisStep);
					}
					else{
						String user_id = request.getParameter("friends");
						
						String[] user_ids = { "" };
						if (user_id != null && !"".equalsIgnoreCase(user_id)) {
							user_ids = user_id.split(",");
						}
						final String[] ids = user_ids;
						
						final String currentDomain = OpiConstants.domain.toString();		
						String flag = GuideXFiveRequestUtil.getParamString(request, "from", host.getId());
						if(MyStringUtils.isNullOrEmpty(flag)){
							flag = "mfbrown_default";
						}
						GuideXFiveSupportFriendsUtil.beFriendsAsync(host, ids, currentDomain, flag);
						if(ids != null){
							GuideLogger.printLog(" host:"+host.getId()+" ids:"+ids.length);
						}
					}
					
					StepStatusLogic.updateStepStatus(host.getId(), thisStep);
					GuideLogger.printLog(" host:"+host.getId()+" update step:"+thisStep);
				} catch (Exception e) {
					GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
					e.printStackTrace();
				}
			}
		}
		String tflag = inv.getParameter("tflag");
		if("_AAJ".equals(tflag)){
			ret = "r:lead";
		}
		GuideLogger.printLog(" host:"+host.getId()+" lead_type_flag:"+tflag);
		return ret;
	}

}
