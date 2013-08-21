package com.xiaonei.reg.guide.flows.xfivebrown.controllers;

import java.util.ArrayList;
import java.util.List;

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
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveCommonCacheLogic;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveStepCheckLogic;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserDecisionLogic;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveSupportFriendsUtil;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;

@LoginRequired
@Path("pvbrown-s")
@IgnoreUserStatusImportedValidation
public class BrownPreviewSaveController {
	
	private static final String SUCC = "f:lead"; 
	//private final static String INIT = "f:pvbrown";
	private final static int thisStep = StepStatusLogic.STEP_PREVIEW;
	
	private static final int BGLIMIT = 100;
	
	@Get
	public String get(Invocation inv){
		return "r:lead";
	}
	
	@Post
	public String index(Invocation inv){
		String ret = SUCC;
		HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
		HttpServletResponse response = GuideXFiveRequestUtil.getResponse(inv);
		User host = GuideXFiveRequestUtil.getCurrentHost(request);		
		if(request == null || response == null){
			GuideLogger.printLog(" request or response null "+(request==null)+(response==null),GuideLogger.ERROR);
			return "@";
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
					GuideTimeCost cost = GuideTimeCost.begin();
					String skip = GuideXFiveRequestUtil.getParamString(request, "skip", host.getId());
					if(MyStringUtils.equals("1", skip)){
						GuideLogger.printLog(" host:"+host.getId()+" skipped step:"+thisStep);
						cost.endStep();
					}
					else{
						String user_id = request.getParameter("friends");
						
						String[] user_ids = { "" };
						if (user_id != null && !"".equalsIgnoreCase(user_id)) {
							user_ids = user_id.split(",");
						}
						final String[] ids = user_ids;
						
						final String currentDomain = OpiConstants.domain.toString();
						cost.endStep();
						String flag = GuideXFiveRequestUtil.getParamString(request, "from", host.getId());
						cost.endStep();
						//2010-5-7 18:37:18 加验证码
						/*if(ids.length >= 50){ //验证
							
						}*/
						if(MyStringUtils.isNullOrEmpty(flag)){
							flag = "pvbrown_default";
						}
						GuideXFiveSupportFriendsUtil.beFriendsAsync(host, ids, currentDomain, flag);
						cost.endStep();
						List<Integer> list = new ArrayList<Integer>();
						for (String id : ids) {
							try {
								list.add(Integer.parseInt(id));
							} 
							catch (NumberFormatException e){
								GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(), GuideLogger.ERROR);
							}
							catch (Exception e) {
								// TODO: handle exception
								e.printStackTrace();
							}
						}
						GuideXFiveCommonCacheLogic.getInstance().saveConfirmList(host, list);
						if(ids != null){
							GuideLogger.printLog(" host:"+host.getId()+" ids:"+ids.length);
						}
					}
					
					cost.endStep();
					saveUserDecision(host, request);
					cost.endStep();
					StepStatusLogic.updateStepStatus(host.getId(), thisStep);
					cost.endStep();
					cost.endFinally();
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

	private void saveUserDecision(User host, HttpServletRequest request) {
		boolean isgood = false;
		int cnt = GuideXFiveRequestUtil.getParamInteger(request, "fcount", host.getId());
		if(cnt >= BGLIMIT){
			isgood = true;
		}
		GuideLogger.printLog(" host:"+host.getId()+" sg:"+isgood);
		GuideXFiveUserDecisionLogic.getInstance().saveDecision(host.getId(), isgood?1:2);
		
	}



}
