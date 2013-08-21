package com.xiaonei.reg.guide.flows.xfive.controllers;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserDecisionLogic;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;

@LoginRequired
@Path("previews")
@IgnoreUserStatusImportedValidation
public class PreviewSubmitController {
	private static final String SUCC = "f:lead"; 
	private static final int thisStep = StepStatusLogic.STEP_PREVIEW;
	private static final int BGLIMIT = 100;
	
	@Get
    @Post
	public String get(Invocation inv){
		HttpServletRequest request = inv.getRequest();
		GuideTimeCost cost = GuideTimeCost.begin();
		User host = BaseThreadUtil.currentHost(request);
		cost.endStep();
		
		if(host!=null && host.getStatus()==10 && host.getDomain().contains("imop")){
            GuideLogger.printLog("returntoregister id:"+host.getId()+" domain:"+host.getDomain()+" status:"+host.getStatus());
            return "r:http://reg.renren.com/sb/fillemail";
        }
		
		GuideLogger.printLog("host:"+host.getId());
		cost.endStep();
		
		boolean isgood = false;
		switch(host.getStage()){
		case Stage.STAGE_UNIV:
			if(getC(host.getId(),request,"U") < BGLIMIT && getC(host.getId(),request,"H") < BGLIMIT){
				isgood = false;
			}
			else{
				isgood = true;
			}
			break;
		case Stage.STAGE_HEIGHSCHOOL:
			int more = 0;
			if(getC(host.getId(),request,"H") >= BGLIMIT) more++;
			if(getC(host.getId(),request,"J") >= BGLIMIT) more++;
			if(getC(host.getId(),request,"E") >= BGLIMIT) more++;
			if(getC(host.getId(),request,"T") >= BGLIMIT) more++;
			if(more >= 1 ){
				isgood = true;
			}
			else{
				isgood = false;
			}
			break;
		case Stage.STAGE_MAJOR:
			if(getC(host.getId(),request,"W") < BGLIMIT && getC(host.getId(),request,"I") < BGLIMIT){
				isgood = false;
			}
			else{
				isgood = true;
			}
			break;
		case Stage.STAGE_NONE:
			if(getC(host.getId(),request,"P") < BGLIMIT){
				isgood = false;
			}
			else{
				//2010-1-18 11:33:39 其它阶段的判断 全改成bad Oscar xiaotang
				isgood = false;
			}
			break;
		default:
			isgood = false;
			break;
		}
		
		//2009-12-14 17:13:37 写决定的时候，不光是看尾号了，还得看NBX5……什么尾号的都有……
		//2009-12-24 17:46:22 又多了面馆……所以干脆不判断了……
		//2010-1-8 11:25:37 6的全写Bad！诗伦的需求
		if("".equals("")/*GuideUtil.needGoToXFiveUser(host) || GuideUtil.isNetBarXFiveUser(host)*/){
			GuideLogger.printLog(" host:"+host.getId()+" sg:"+isgood);
			if(host.getId() % 10 == 6){
				GuideXFiveUserDecisionLogic.getInstance().saveDecision(host.getId(), GuideXFiveUserDecisionLogic.SOCIAL_GRAPH_BAD);
			}
			else{
				GuideXFiveUserDecisionLogic.getInstance().saveDecision(host.getId(), isgood?1:2);
			}
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" is not an X5 user but sg:"+isgood,GuideLogger.WARN);
		}
		
		//改标志
		StepStatusLogic.updateStepStatus(host.getId(), thisStep);
		GuideLogger.printLog("| host:"+host.getId()+" update step:"+thisStep);
		cost.endStep();
		cost.endFinally();

		return SUCC;
	}
	
	private int getC(int userid, HttpServletRequest req ,String type){
		int cnt = 0;
		String ucnt = req.getParameter(type);
		try {
			cnt = Integer.parseInt(ucnt);
			GuideLogger.printLog(" host:"+userid+" type:"+type+" cnt:"+cnt);
		} catch (NumberFormatException e) {
			GuideLogger.printLog(" host:"+userid+" type:"+type+" err:"+e.toString());
			//e.printStackTrace();
		}
		return cnt;
	}
	
	
}
