/**
 * 
 */
package com.xiaonei.reg.guide.action.a090821;

import java.util.ArrayList;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.dodoyo.opt.DodoyoResource;
import com.xiaonei.antispam.model.CheckResult;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.util.BaseActionUtil;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.guide.action.base.GuideFlowBaseAction;
import com.xiaonei.reg.guide.form.f090729.FormBailing;
import com.xiaonei.reg.guide.logic.FillInfoLogic;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.constants.IRegCheckCodeDef;
import com.xiaonei.reg.register.logic.TimeCost;
import com.xiaonei.reg.register.logic.pretreatment.validate.Antispam;
import com.xiaonei.reg.register.logic.pretreatment.validate.CheckUniversity;

/**
 * 无阶段的fillinfo
 * 
 * @author wangtai
 * 
 */
public class FillInfoActionBailing extends GuideFlowBaseAction {
	// public class NoStageFillInfoAction extends BaseAnonymousAction {

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
		String rt = check(form, request);
		if (rt != null) {
			return err(rt, request, mapping, response);
		}
		rt = sb(request, form, mapping, response);
		if (rt != null) {
			return err(rt, request, mapping, response);
		}
		return null;
	}

	/**
	 * 出错流程
	 * 
	 * @param msg
	 * @param request
	 * @param mapping
	 * @param response
	 * @return
	 */
	private ActionForward err(String msg, HttpServletRequest request,
			ActionMapping mapping, HttpServletResponse response) {
		BaseActionUtil.addErr(request, msg);
		return initPage(request, mapping);
	}

	/**
	 * 检查用户提交数据
	 * 
	 * @param form
	 * @param request
	 * 
	 * @return
	 */
	private String check(ActionForm form, HttpServletRequest request) {
		GuideLogger.writeLine(this, "check() - start");
		FormBailing fform = (FormBailing) form;
		User host = BaseThreadUtil.currentHost(request);
		String rt = null;
		if(StringUtils.isEmpty(fform.getCompanyname())){
			rt = IRegCheckCodeDef.ERR_WORKPLACENAME_NOT_EMPTY;
		}
		if(fform.getHomecitycode()==0){
			rt = "请选择所在地";
		}

		List<String> antispamList = new ArrayList<String>();
		antispamList.add(fform.getCompanyname());
		antispamList.add(fform.getHomeprovince());
		antispamList.add(fform.getUniversityname());
		for (String antispamStr : antispamList) {
			if (StringUtils.isEmpty(antispamStr))
				continue;
			CheckResult cr = Antispam.checkAndFilterCR(host.getId(),
					antispamStr);
			switch (cr.getFlag()) {
			case CheckResult.SAFE:
				break;
			case CheckResult.PROHIBITED:
				return DodoyoResource.getString("errors.anti.web");
			default:
				break;
			}
		}
		request.setAttribute("err_msg", rt);
		GuideLogger.writeLine(this, "check() - end"+rt);
		return rt;
	}

	/**
	 * 处理用户提交数据
	 * 
	 * @param response
	 * @param mapping
	 * @param request
	 * @param form
	 * 
	 * @return
	 */
	private String sb(HttpServletRequest request, ActionForm form,
			ActionMapping mapping, HttpServletResponse response) {
		request.setAttribute("action_form", form);
		FormBailing fform = (FormBailing) form;
		User host = BaseThreadUtil.currentHost(request);
		try {
			GuideLogger.writeLine(this , "sb() - hostid:"+ host.getId()+ " - savework - com:"+fform.getCompanyname());
			//工作
			//saveWork(fform,host);
			UserProfileUtil.saveWork(host, fform);
			
		} catch (Exception e) {
			GuideLogger.writeLine(this, "sb() saveWork() error "+host.getId()+" "+e.toString(),1);
			e.printStackTrace();
		}
		
		try {
			GuideLogger.writeLine(this , "sb() - hostid:"+ host.getId()+ " - saveuniv - univ:"+fform.getUniversityname());
			//大学
			String chk = new CheckUniversity().checkUniversity("1", fform.getUniversitycode(), fform.getUniversityname());
			if(StringUtils.equals(chk, "OK") || StringUtils.equals(chk, "请填写大学（选填）")){
				//saveUniv(fform, host, false);
				UserProfileUtil.saveUniversity(host, fform);
			}
		} catch (Exception e) {
			GuideLogger.writeLine(this, "sb() saveUniv() error "+host.getId()+" "+e.toString(),1);
		}
		
		try {
			GuideLogger.writeLine(this , "sb() - hostid:"+ host.getId()+ " - savecity - city:"+fform.getHomecityname());
			//地理
			if(checkCityNet(fform)){ 
//				saveCity(fform, CurrentNet, host);
//				A090821Util.saveRegion(host, fform);
				UserProfileUtil.saveRegionAndNet(host, fform);
			}
		} catch (Exception e) {
			GuideLogger.writeLine(this, "sb() saveCity() error "+host.getId()+" "+e.toString(),1);
		}
		
		try {
			GuideLogger.writeLine(this , "sb() - hostid:"+ host.getId()+ " - savestage - stage:"+Stage.STAGE_MAJOR);
			//阶段
			FillInfoLogic.getInstance().saveStage(host, Stage.STAGE_MAJOR);
			
		} catch (Exception e) {
			GuideLogger.writeLine(this, "sb() saveStage() error "+host.getId()+" "+e.toString(),1);
		}

		return null;
	}

	
	/**
	 * 判断时候可以加入地域网络
	 * 
	 * @param fform
	 * @return
	 */
	private boolean checkCityNet(FormBailing fform) {
		GuideLogger.writeLine(this,"checkCityNet() -- start");
		if (StringUtils.isNotEmpty(fform.getHomecityname())
				|| StringUtils.isNotEmpty(fform.getHomeprovince())) {
			GuideLogger.writeLine(this,"checkCityNet() -- end true");
			return true;
		}
		GuideLogger.writeLine(this,"checkCityNet() -- end false");
		return false;
	}

	
	@Override
	protected ActionForward initPage(HttpServletRequest request,
			ActionMapping mapping) {
		return mapping.findForward(FORWARD_SUCC);
	}

	@Override
	protected boolean shouldInit(HttpServletRequest request) {
		return !"post".equalsIgnoreCase(request.getMethod());
	}

	@Override
	protected String thisUrl() {
		return Globals.urlGuide + "/fi.do";
	}

	@Override
	protected int thisStatus() {
		return StepStatusLogic.STEP_FILLINFO;
	}

	@Override
	protected String nextUrl() {
		return Globals.urlGuide + "/pv-090729.do";
	}
	
	@Override
	protected String nextUrl(HttpServletRequest request) {
		TimeCost cost = TimeCost.begin();
		ActionForm form = (ActionForm)request.getAttribute("action_form");
		cost.endStep();
		boolean has = false;
		cost.endStep();
		try {
			FormBailing fform = (FormBailing)form;
			cost.endStep();
			if(fform.getUniversitycode()!=0){
				has = true;
				cost.endStep();
			}

			cost.endStep();
		} catch (Exception e) {
			GuideLogger.writeLine(this,"nextUrl()",1);
		}
		cost.endFinally();
		String h = "false";
		if(has) h = "true";
		return Globals.urlGuide + "/pv-090729.do?h="+h;
	}
}
