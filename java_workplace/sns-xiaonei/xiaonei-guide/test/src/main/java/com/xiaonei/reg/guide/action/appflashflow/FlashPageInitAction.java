/**
 * 
 */
package com.xiaonei.reg.guide.action.appflashflow;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.guide.action.a090729.A090729Util;
import com.xiaonei.reg.guide.action.a090729.MyTimeCost;
import com.xiaonei.reg.guide.action.base.GuideFlowBaseAction;
import com.xiaonei.reg.guide.logic.GuideUserTraceLogic;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.constants.IRegCookieKeyDef;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

/**
 * 
 * @author inshion - xin.yin@opi-corp.com
 * 
 */
public class FlashPageInitAction extends GuideFlowBaseAction {
	
	private static final String FORWARD_SUCC = "succ";

	/**
	 * 成功后跳转
	 * 
	 * @param request
	 * @param form
	 * @param mapping
	 * @param response
	 * 
	 * @param host
	 * 
	 * @return
	 */
	@Override
	protected ActionForward succ(HttpServletRequest request, ActionForm form,
			ActionMapping mapping, HttpServletResponse response) {
		return null;
	}


	
	@Override
	protected ActionForward initPage(HttpServletRequest request,
			ActionMapping mapping) {
		MyTimeCost cost = MyTimeCost.begin();
		User host = BaseThreadUtil.currentHost(request);
		cost.endStep();
		try {
			GuideLogger.writeLine(this,"init() - start - host:"+host.getId());
			//农场的步骤记录！
			GuideUserTraceLogic.getInstance().asyncWriteTrace(request, host, GuideUserTraceLogic.STEP001_ARRIVAL_OPENFLOW);
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
		return mapping.findForward(FORWARD_SUCC);
	}



	@Override
	protected boolean shouldInit(HttpServletRequest request) {
		return !"post".equalsIgnoreCase(request.getMethod());
	}

	@Override
	protected String thisUrl() {
		return Globals.urlGuide + "/af.do";
	}

	@Override
	protected int thisStatus() {
		return StepStatusLogic.STEP_FLASH_APP_OPEN;
	}

	@Override
	protected String nextUrl() {
		return Globals.urlGuide + "/guide.do";
	}
	
	@Override
	protected String nextUrl(HttpServletRequest request){
		MyTimeCost cost = MyTimeCost.begin();
		cost.endFinally();
		return Globals.urlGuide + "af.do";
	}
	
}
