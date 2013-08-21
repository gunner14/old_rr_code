package com.xiaonei.reg.guide.action;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.action.DynaActionForm;
import org.apache.struts.validator.DynaValidatorForm;

import com.xiaonei.platform.core.model.HighSchool;
import com.xiaonei.platform.core.model.University;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.util.BaseActionUtil;
import com.xiaonei.platform.core.opt.network.NetworkManager;
import com.xiaonei.platform.core.utility.Escape;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.logic.fillinfo.DXFillinfoLogic;
import com.xiaonei.reg.guide.pojo.DXFillinfoPojo;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.Utils;
import com.xiaonei.reg.register.logic.additional.RegPortraitLogic;
import com.xiaonei.reg.register.pojo.Upload;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegOprDefine;

/**
 * DXFillinfoAction
 * 
 * @author (rebuilder)wang-tai(tai.wang@opi-corp.com) 2009-2-21 - 上午10:47:31
 */
public class DXFillinfoAction extends ProtectAction {

	private static final String FWD_INIT = "succ";
	private static final String PAR_department = "department";
	private static final String PAR_otherdepartment = "otherdepartment";
	private static final String PAR_highSchoolCode = "highSchoolCode";
	private static final String PAR_universityyear = "universityyear";
	private static final String PAR_highschoolyear = "highschoolyear";
	private static final String PAR_porId = "porid";

	@Override
	public ActionForward executePt(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		DynaValidatorForm infoForm = (DynaValidatorForm) form;
		User host = BaseThreadUtil.currentHost(request);
		GuideLogger.printLog(" host:" + host.getId() + " NOHERE! ",GuideLogger.ERROR);
		// 检查用户是否完善过资料
		String toUrl = StepStatusLogic.goWhere(host);
		if (!StringUtils.equals(toUrl, (Globals.urlGuide + "/fillinfodx.do")) && StringUtils.isNotEmpty(toUrl)) {
			return new ActionForward(toUrl, true);
		}
		University university = DXFillinfoLogic.getInstance().getUniv(host);
		request.setAttribute("userBasic", DXFillinfoLogic.getInstance().getUserBasic(host));
		request.setAttribute("university", university);
		if (!isPost(request)) {
			return init(request, mapping);
		} else {
			return submit(infoForm, request, host, mapping);
		}
	}

	/**
	 * submit
	 * 
	 * @param infoForm
	 * @param request
	 * @param host
	 * @param mapping
	 * @return
	 */
	private ActionForward submit(DynaActionForm infoForm,
			HttpServletRequest request, User host, ActionMapping mapping) {
		/*
		 * 大学院系
		 */
		String department = Escape.stringToHTMLString(infoForm.getString(PAR_department));
		String otherDepartment = null;
		if (StringUtils.equalsIgnoreCase(department, "其他院系")) {
			otherDepartment = Escape.stringToHTMLString(infoForm.getString(PAR_otherdepartment));
			otherDepartment = otherDepartment.replaceAll("\\s+", "");// 替换掉任何位置处的空格
			department = otherDepartment;
		}
		/*
		 * 大学入学年
		 */
		int universityYear = 0;
		try {
			universityYear = Integer.parseInt(Escape.stringToHTMLString(infoForm.getString(PAR_universityyear)));
		} catch (Exception e) {
			BaseActionUtil.addErr(request, "无效的大学入学年");
			return mapping.findForward(FWD_INIT);
		}
		/*
		 * 高中
		 */
		int highSchoolId;
		String highSchoolName;
		try {
			highSchoolId = Integer.parseInt(Escape.stringToHTMLString(infoForm.getString(PAR_highSchoolCode)));
		} catch (Exception e) {
			BaseActionUtil.addErr(request, "请选择高中");
			return mapping.findForward(FWD_INIT);
		}
		HighSchool highSchool = NetworkManager.getInstance().getHighSchoolById(highSchoolId);
		if (highSchool == null) {
			BaseActionUtil.addErr(request, "没有这个高中");
			return mapping.findForward(FWD_INIT);
		} else {
			highSchoolName = highSchool.getName();
		}

		/* 高中入学年的合法性判断 */
		int highschoolYear = 0;
		try {
			highschoolYear = Integer.parseInt(infoForm.getString(PAR_highschoolyear));
		} catch (Exception e) {
			BaseActionUtil.addErr(request, "无效的高中入学年");
			return mapping.findForward(FWD_INIT);
		}
		/* 院系的合法性判断 */
		if (Utils.getInstance().validateForString(department)) {
			BaseActionUtil.addErr(request, "请选择院系");
			return mapping.findForward(FWD_INIT);
		}
		/* 获得头像 & 合法性判断 */
		if (!RegPortraitLogic.hasUploaded(host)) {
			String porId = request.getParameter(PAR_porId);
			final Upload up = RegPortraitLogic.getPortrait(porId);
			if (StringUtils.trimToNull(porId) == null) {
				BaseActionUtil.addErr(request, "请上传头像");
				return mapping.findForward(FWD_INIT);
			}
			// 保存头像
			RegPortraitLogic.setPortrait(host, up);
		}
		// set attribute
		infoForm.set(PAR_department, department);
		request.setAttribute("highschool", highSchool);
		// 保存数据
		DXFillinfoPojo c = new DXFillinfoPojo();
		c.setHost(host);
		c.setUnivYear(universityYear);
		c.setDepartment(department);
		c.setOtherDepartment(otherDepartment);
		c.setHighSchoolId(highSchoolId);
		c.setHighSchoolName(highSchoolName);
		c.setHighSchoolYear(highschoolYear);
		DXFillinfoLogic.getInstance().saveDataByUniv(c);
		// 用户行为日志
		UserTraceLogic.getInstance().insertRegHis(request, host,host.getAccount(), String.valueOf(RegOprDefine.FILL_INFO),
				UserAdapter.get(host).getStage(), null, null, null, null, 0);
		return new ActionForward(Globals.urlGuide + "/previewdx.do?skip=1",true);
	}

	/**
	 * init
	 * 
	 * @param request
	 * @param mapping
	 * @return
	 */
	private ActionForward init(HttpServletRequest request, ActionMapping mapping) {
		User user = BaseThreadUtil.currentHost(request);
		request.setAttribute("tsc", RegPortraitLogic.getTsc());
		request.setAttribute("hasUpload", RegPortraitLogic.hasUploaded(user));
		return mapping.findForward(FWD_INIT);
	}

}
