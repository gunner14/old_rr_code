/**
 * 
 */
package com.xiaonei.reg.guide.action.a090729;

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
import com.xiaonei.reg.guide.form.f090729.FormDaxue;
import com.xiaonei.reg.guide.logic.FillInfoLogic;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.logic.fillinfo.GuideUserProfileLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.constants.IRegCheckCodeDef;
import com.xiaonei.reg.register.logic.pretreatment.validate.Antispam;

/**
 * 有阶段的fillinfo
 * 
 * @author inshion - xin.yin@opi-corp.com
 * 
 */
public class FillInfoActionDaxue extends GuideFlowBaseAction {

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
		GuideLogger.writeLine(this, "check() - start -- here");
		FormDaxue fform = (FormDaxue) form;
		User host = BaseThreadUtil.currentHost(request);
		String rt = null;
		
//		rt = new CheckUniversity().checkUniversity("1", fform.getUniversitycode(), fform.getUniversityname());
//		
//		if (!IRegCheckCodeDef.OK.equals(rt)	&& !IRegCheckCodeDef.ERR_NO_INPUT_UNIV.equals(rt)) {
//			System.out.println("[inshion] - [new Guide] check() 9 rt="+rt);
//			request.setAttribute("err_msg", rt);
//			return rt;
//		}
		if (fform.getUniversitycode() == 0){
			rt = IRegCheckCodeDef.ERR_NO_INPUT_UNIV;
		} 
		else if (fform.getHighschoolcode() == 0){
			rt = IRegCheckCodeDef.ERR_NO_INPUT_HIGHSCHOOL;
		}
		List<String> antispamList = new ArrayList<String>();		
		antispamList.add(fform.getUniversityname());
		antispamList.add(fform.getHighschoolname());
		for (String antispamStr : antispamList) {
			if (StringUtils.isEmpty(antispamStr))
				continue;
			CheckResult cr = Antispam.checkAndFilterCR(host.getId(),
					antispamStr);
			switch (cr.getFlag()) {
			case CheckResult.SAFE:
				break;
			case CheckResult.PROHIBITED:
				request.setAttribute("err_msg", DodoyoResource.getString("errors.anti.web"));
				return DodoyoResource.getString("errors.anti.web");
			default:
				break;
			}
		}
		request.setAttribute("err_msg", rt);
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
	private String sb(final HttpServletRequest request, ActionForm form,
			ActionMapping mapping, HttpServletResponse response) {
		request.setAttribute("action_form", form);
		MyTimeCost cost = MyTimeCost.begin();
		final FormDaxue fform = (FormDaxue) form;
		final User host = BaseThreadUtil.currentHost(request);
		GuideLogger.printLog(" host:"+host.getId()+MyRequestUtil.getBrowserInfo(request));
		
		cost.endStep();
		try {
			GuideLogger.printLog("sb() - hostid:"+ host.getId()+ " - start - "+fform.getUniversityname()+" | "+fform.getHighschoolname());
			cost.endStep();
			
			/*保存大学的逻辑改为以下新接口操作*/
			GuideUserProfileLogic.getInstance().saveUniversity(host, fform);
			cost.endStep();

			/*保存网络的逻辑改为以下新接口操作*/
			GuideUserProfileLogic.getInstance().asyncSaveUniversityNet(host, fform);
			cost.endStep();
			
			/*保存中学的逻辑改为以下新接口操作*/
			GuideUserProfileLogic.getInstance().asyncSaveHighschoolAndNet(host, fform);
			cost.endStep();

			//写阶段很关键 要同步
			GuideLogger.printLog("saveStage: "+host.getId());
			FillInfoLogic.getInstance().saveStage(host, Stage.STAGE_UNIV);
			cost.endStep();
			
			/*//2010-2-5 14:14:00 Inshion 去掉
			//09新生
			A090729Util.beFansof09(host);
			cost.endStep();*/
			
			/*//2010-2-5 14:14:00 Inshion 去掉
			//新手帮
			A090729Util.asyncBeFansofPage(600002351, host);
			cost.endStep();*/

			GuideLogger.writeLine(this , "sb() - hostid:"+ host.getId()+ " - end - "+fform.getUniversitycode()+" | "+fform.getHighschoolcode());

			cost.endStep();
			cost.endFinally();
			
		} catch (Exception e) {
			GuideLogger.writeLine(this, "sb() error "+host.getId()+" "+e.toString(),1);
		}

		return null;
	}


	@Override
	protected ActionForward initPage(HttpServletRequest request,
			ActionMapping mapping) {
		GuideLogger.writeLine(this,"init()");
		try {
			A090729Util.setInitAttributes(request);
		} catch (Exception e) {
			GuideLogger.printLog("setInitAttributes failed!"+e.toString());
			e.printStackTrace();
		}
		return mapping.findForward(FORWARD_SUCC);
	}

	@Override
	protected boolean shouldInit(HttpServletRequest request) {
		return !"post".equalsIgnoreCase(request.getMethod());
	}

	@Override
	protected String thisUrl() {
		return Globals.urlGuide + "/fi-090729.do";
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
	protected String nextUrl(HttpServletRequest request){
		MyTimeCost cost = MyTimeCost.begin();
		ActionForm form = (ActionForm)request.getAttribute("action_form");
		cost.endStep();
		boolean has = false;
		cost.endStep();
		try {
			FormDaxue fform = (FormDaxue)form;
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
