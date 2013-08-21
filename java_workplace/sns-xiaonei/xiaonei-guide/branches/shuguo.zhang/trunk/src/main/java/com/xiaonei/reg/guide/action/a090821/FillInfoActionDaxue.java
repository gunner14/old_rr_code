/**
 * 
 */
package com.xiaonei.reg.guide.action.a090821;

import java.sql.SQLException;
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
import com.xiaonei.platform.core.model.University;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.util.BaseActionUtil;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.guide.action.base.GuideFlowBaseAction;
import com.xiaonei.reg.guide.form.f090729.FormDaxue;
import com.xiaonei.reg.guide.logic.FillInfoLogic;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.constants.IRegCheckCodeDef;
import com.xiaonei.reg.register.dao.IpTableDAO;
import com.xiaonei.reg.register.logic.TimeCost;
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
		TimeCost cost = TimeCost.begin();
		final FormDaxue fform = (FormDaxue) form;
		final User host = BaseThreadUtil.currentHost(request);
		try {
			cost.endStep();
			GuideLogger.writeLine(this , "sb() - hostid:"+ host.getId()+ " - start - "+fform.getUniversityname()+" | "+fform.getHighschoolname());
			cost.endStep();
			GuideLogger.writeLine(this,"sb() -- save 1");
			
			UserProfileUtil.saveUniversity(host, fform);

			Runnable taskSave_NET = new Runnable() {
				public void run() {
					TimeCost cost = TimeCost.begin();
					boolean cansave = checkUnivNet(fform, request);
					GuideLogger.writeLine(this,"sb() -- save 3");
					cost.endStep();
					if(cansave){
						UserProfileUtil.saveUniversityNet(host, fform);
					}
					cost.endStep();
					cost.endFinally();
				}
			};
			RunnableTaskExecutor.getInstance().runTask(taskSave_NET);
			
			Runnable taskSaveMS = new Runnable() {
				public void run() {
					TimeCost cost = TimeCost.begin();
					boolean cansavems = checkMsNet(fform);
					cost.endStep();
					if(cansavems){
						UserProfileUtil.saveHighschool(host, fform);
						cost.endStep();
						UserProfileUtil.saveHighschoolNet(host, fform);
						cost.endStep();
					}
					cost.endStep();
					GuideLogger.writeLine(this, "saveMs: "+host.getId());
					cost.endFinally();
				}
			};
			RunnableTaskExecutor.getInstance().runTask(taskSaveMS);

			//写阶段很关键 要同步
			FillInfoLogic.getInstance().saveStage(host, Stage.STAGE_UNIV);
			GuideLogger.writeLine(this, "saveStage: "+host.getId());
			cost.endStep();
			
			/*GuideLogger.writeLine(this,"sb() -- save OK");
			//2010-2-5 14:14:00 Inshion 去掉
			A090729Util.beFansof09(host);*/
			GuideLogger.writeLine(this , "sb() - hostid:"+ host.getId()+ " - end - "+fform.getUniversitycode()+" | "+fform.getHighschoolcode());
			cost.endFinally();
			
		} catch (Exception e) {
			GuideLogger.writeLine(this, "sb() error "+host.getId()+" "+e.toString(),1);
		}

		return null;
	}



	/**
	 * 判断时候可以加入大学网络
	 * 
	 * @param fform
	 * @param request
	 * @return
	 */
	private boolean checkUnivNet(FormDaxue fform,
			HttpServletRequest request) {
		TimeCost cost = TimeCost.begin();
		List<University> univList = null;
		cost.endStep();
		try {
			String ip = BaseActionUtil.getClientIP(request);
			cost.endStep();
			univList = IpTableDAO.getInstance().getUnivByIp(ip);
			cost.endStep();
		} catch (SQLException e) {
			e.printStackTrace();
			return false;
		}
		cost.endStep();
		// 搞ip
		if (univList != null && univList.size() > 0) {
			for (int i = 0; i < univList.size(); i++) {
				if (fform.getUniversitycode() == univList.get(i).getId()) {
					return true;
				}
			}
		}
		cost.endFinally();
		return false;
	}


	/**
	 * 判断时候可以加入中学网络
	 * 
	 * @param fform
	 * @return
	 */
	private boolean checkMsNet(FormDaxue fform) {
		if(StringUtils.isNotEmpty(fform.getHighschoolname()))
				return true;
		return false;
	}

	@Override
	protected ActionForward initPage(HttpServletRequest request,
			ActionMapping mapping) {
		GuideLogger.writeLine(this,"init()");
		UserProfileUtil.setInitAttributes(request);
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
	protected String nextUrl(HttpServletRequest request){
		TimeCost cost = TimeCost.begin();
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
