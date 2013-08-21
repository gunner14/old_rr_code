package com.xiaonei.reg.guide.action.base;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.action.BasePageAction;
import com.xiaonei.reg.guide.action.a090729.MyTimeCost;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;


/**
 * @author wangtai
 * 
 */
public abstract class GuideFlowBaseAction extends BasePageAction {

	protected abstract String thisUrl();

	protected abstract String nextUrl();
	
	protected abstract int thisStatus();

	/**
	 * 成功跳转
	 * 
	 * @param request
	 * @param mapping
	 * @return
	 */
	protected abstract ActionForward succ(HttpServletRequest request,
			ActionForm form, ActionMapping mapping, HttpServletResponse response);

	/**
	 * 时候应该初始化
	 * 
	 * @param request
	 * @return
	 */
	protected abstract boolean shouldInit(HttpServletRequest request);

	/**
	 * 初始化页面
	 * 
	 * @param request
	 * @param mapping
	 * @return
	 */
	protected abstract ActionForward initPage(HttpServletRequest request,
			ActionMapping mapping);

	protected String nextUrl(HttpServletRequest request){
		return nextUrl();
	}

	@Override
	public ActionForward executePt(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		GuideLogger.writeLine(this,"executePt() 1");
		MyTimeCost cost = MyTimeCost.begin();
		ActionForward af = null;
		cost.endStep("ExPT::Binit");
		String toUrl = shouldIn(request);
		cost.endStep("ExPT::Ainit");
		if (null != toUrl) {
			GuideLogger.writeLine(this,"executePt() 2 to"+toUrl);
			cost.endStep("ExPT::B2");
			af = new ActionForward(toUrl, true);
			cost.endStep("ExPT::A2");
			cost.endFinally();
			return af;
		}
		if (isSkip(request)) {
			GuideLogger.writeLine(this,"executePt() 3");
			cost.endStep("ExPT::B3");
			af = skipThisStep(request, form, mapping, response);
			cost.endStep("ExPT::A3");
			cost.endFinally();
			return af;
		}
		if (shouldInit(request)) {
			GuideLogger.writeLine(this,"executePt() 4");
			cost.endStep("ExPT::B4");
			af = initPage(request, mapping);
			cost.endStep("ExPT::A4");
			cost.endFinally();
			return af;
		} else {
			GuideLogger.writeLine(this,"executePt() 5");
			cost.endStep("ExPT::B5");
			af = succGo(request, form, mapping, response);
			cost.endStep("ExPT::A5");
			cost.endFinally();
			return af;
		}
	}

	private ActionForward skipThisStep(HttpServletRequest request, ActionForm form,
			ActionMapping mapping, HttpServletResponse response) {
		return this.succOk(request, form, mapping, response);
	}

	/**
	 * 是否跳过次步
	 * 
	 * @param request
	 * @return
	 */
	private boolean isSkip(HttpServletRequest request) {
		return "1".equals(request.getParameter("skip")) && !"yes".equals(request.getParameter("input"));
	}

	protected final ActionForward succGo(HttpServletRequest request,
			ActionForm form, ActionMapping mapping, HttpServletResponse response) {
		ActionForward rt = succ(request, form, mapping, response);
		if (null != rt) {
			return rt;
		}
		return succOk(request, form, mapping, response);
	}
	protected final ActionForward succOk(HttpServletRequest request,
			ActionForm form, ActionMapping mapping, HttpServletResponse response) {
		User host = BaseThreadUtil.currentHost(request);
		// 标记完成
		StepStatusLogic.updateStepStatus(host.getId(), thisStatus());
		String nextu = nextUrl(request);
		GuideLogger.writeLine(this,"succOk() - "+nextu);
		if(nextu.indexOf("?")>=0){
			nextu += "&rt=o";
		}
		else{
			nextu += "?rt=o";
		}
		return new ActionForward(nextu, true);
	}
	/**
	 * 判断是否应该进入此步
	 * 
	 * @param request
	 * 
	 * @return
	 */
	protected final String shouldIn(HttpServletRequest request) {
		GuideLogger.writeLine(this,"shouldIn() - request:"+request.getRequestURL());
		String rt= request.getParameter("rt");
		if("o".equals(rt)){
			return null;
		}
		User host = BaseThreadUtil.currentHost(request);
		String toUrl = StepStatusLogic.goWhere(host);
		GuideLogger.writeLine(this, "shouldIn() - toUrl   is:"+toUrl);
		GuideLogger.writeLine(this, "shouldIn() - thisUrl is:"+thisUrl());
		if (!StringUtils.equals(toUrl, thisUrl())) {
			return toUrl;
		}
		return null;
	}


}
