package com.xiaonei.reg.guide.flows.flashcook.controllers;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.reg.guide.action.a090729.A090729Util;
import com.xiaonei.reg.guide.logic.GuideUserTraceLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;
import com.xiaonei.reg.register.constants.IRegCookieKeyDef;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

/**
 * FlashCookInitController.java
 * @author inshion(xin.yin@opi-corp.com)
 *
 * @date 2010-8-24 兼容老的"acook.do"
 */
@LoginRequired
@Path({"fifc","acook.do"})
public class FlashCookInitController {
	private static final String SUCC = "/views/guide/flowflashcook/flashcook.jsp";

	@Get
	public String get(Invocation inv){
		GuideTimeCost cost = GuideTimeCost.begin();
		HttpServletRequest request = inv.getRequest();
		User host = BaseThreadUtil.currentHost(request);
		cost.endStep();
		try {
			GuideLogger.writeLine(this,"init() - start - host:"+host.getId());
			//餐厅的步骤记录！
			GuideUserTraceLogic.getInstance().asyncWriteTrace(request, host, GuideUserTraceLogic.COOK_STEP001_ARRIVAL_OPENFLOW);
			cost.endStep();
			
		} catch (Exception e) {
			GuideLogger.printLog("host:"+host.getId()+"writeTrace() error! "+e.toString());
		}
		try {
			A090729Util.setInitAttributes(request);
			cost.endStep();
			
		} catch (Exception e) {
			GuideLogger.printLog("host:"+host.getId()+"setInitAttributes() error! "+e.toString());
		}
		
		try {
			UserBasic ub = SnsAdapterFactory.getUserBasicAdapter().get(host.getId());
			cost.endStep();
			String sbirth = ub.getBirthYear()+"-"+ub.getBirthMonth()+"-"+ub.getBirthDay();
			request.setAttribute("f_birth", sbirth);
			cost.endStep();
			
		} catch (Exception e) {
			GuideLogger.printLog("host:"+host.getId()+"UserBasic error! "+e.toString());
		}
		try {
			cost.endStep();
			request.setAttribute("f_gender", host.getGender());
			cost.endStep();
			request.setAttribute("f_name", host.getName());
			cost.endStep();
			request.setAttribute("f_identity", host.getId());
			cost.endStep();
		} catch (Exception e) {
			GuideLogger.printLog("host:"+host.getId()+"setAttribute() error! "+e.toString());
		}
		try {
			String sguester = CookieManager.getInstance().getCookie(request, IRegCookieKeyDef.GUESTERTICKET);
			cost.endStep();
			String shost = CookieManager.getInstance().getCookie(request, IRegCookieKeyDef.HOSTID);
			cost.endStep();
			String stticket = CookieManager.getInstance().getCookie(request, IRegCookieKeyDef.TTICKET);
			cost.endStep();
			request.setAttribute("f_ck_societyguester", sguester);
			cost.endStep();
			request.setAttribute("f_ck_host", shost);
			cost.endStep();
			request.setAttribute("f_ck_ticket", stticket);
			cost.endStep();
			
		} catch (Exception e) {
			GuideLogger.printLog("host:"+host.getId()+"CookieManager error! "+e.toString());
		}
		cost.endFinally();
		return SUCC;
	}
}
