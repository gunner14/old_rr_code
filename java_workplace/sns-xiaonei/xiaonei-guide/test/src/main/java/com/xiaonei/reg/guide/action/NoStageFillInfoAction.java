/**
 * 
 */
package com.xiaonei.reg.guide.action;

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
import com.xiaonei.admin.biz.model.school.SchoolType;
import com.xiaonei.antispam.model.CheckResult;
import com.xiaonei.platform.core.model.NetworkStatus;
import com.xiaonei.platform.core.model.University;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.util.BaseActionUtil;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.guide.action.base.GuideFlowBaseAction;
import com.xiaonei.reg.guide.form.NoStageFillInfoForm;
import com.xiaonei.reg.guide.logic.FillInfoLogic;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.constants.IRegCheckCodeDef;
import com.xiaonei.reg.register.dao.IpTableDAO;
import com.xiaonei.reg.register.logic.additional.BirthdayLogic;
import com.xiaonei.reg.register.logic.pretreatment.validate.Antispam;
import com.xiaonei.reg.register.logic.pretreatment.validate.CheckUniversity;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

/**
 * 无阶段的fillinfo
 * 
 * @author wangtai
 * 
 */
public class NoStageFillInfoAction extends GuideFlowBaseAction {
	// public class NoStageFillInfoAction extends BaseAnonymousAction {

	private static final String FORWARD_SUCC = "succ";
	private static final int CurrentNet = NetworkStatus.CURRENT_NETWORK;
	private static final int PassNet = NetworkStatus.PASS_NETWORK;
	private static final int NotNet = 9999999;
	private static final FillInfoLogic userinfoLogic = FillInfoLogic
			.getInstance();

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
		NoStageFillInfoForm fform = (NoStageFillInfoForm) form;
		User host = BaseThreadUtil.currentHost(request);
		String rt = null;
		// switch (fform.getSchoolType()) {
		// case 0:
		// if (StringUtils.isEmpty(fform.getHighSchoolName())) {
		// return "请填写中学";
		// }
		// break;
		// case 1:
		// if (StringUtils.isEmpty(fform.getHighSchoolName())) {
		// return "请填写中学";
		// }
		// break;
		// case 2:
		// if (StringUtils.isEmpty(fform.getHighSchoolName())) {
		// return "请填写中学";
		// }
		// break;
		// default:
		// return "请填写中学";
		// }
		// if (0 == fform.getHighschoolyear()) {
		// return "清填写中学年份";
		// }
		// if (!less17age(request)) {
		// rt = new CheckHomecity().checkHomecity(String.valueOf(fform
		// .getHomecityCode()), fform.getHomecityName(), fform
		// .getHomeprovince());
		// if (!IRegCheckCodeDef.OK.equals(rt)) {
		// return rt;
		// }
		// }
		rt = new CheckUniversity().checkUniversity("1",
				fform.getUniversityId(), fform.getUniversityname());
		if (!IRegCheckCodeDef.OK.equals(rt)
				&& !IRegCheckCodeDef.ERR_NO_INPUT_UNIV.equals(rt)) {
			return rt;
		}
		// if ((fform.getHighschoolyear() >= fform.getUniversityyear())
		// || (fform.getHighschoolyear() <= 0 || fform.getUniversityyear() <=
		// 0)) {
		// return "日期不合法";
		// }
		// antispam
		List<String> antispamList = new ArrayList<String>();
		antispamList.add(fform.getCompany());
		antispamList.add(fform.getDepartment());
		antispamList.add(fform.getHighSchoolName());
		antispamList.add(fform.getHomecityName());
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
		return null;
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
		int[] netSign = { NotNet, NotNet, NotNet };
		NoStageFillInfoForm fform = (NoStageFillInfoForm) form;
		if (!"".equals(fform.getUniversityname())) {
			netSign[0] = checkUnivNet(fform, request) ? CurrentNet : PassNet;
		}
		if (checkCityNet(fform)) {
			netSign[1] = (netSign[0] == NotNet) ? CurrentNet : PassNet;
		}
		if (checkMsNet(fform)) {
			netSign[2] = (netSign[1] == NotNet && netSign[0] == NotNet) ? CurrentNet
					: PassNet;
		}
		System.out.println("netSign " + netSign[0] + ", " + netSign[1] + ", "
				+ netSign[2] + ", ");
		User host = BaseThreadUtil.currentHost(request);
		GuideLogger.printLog(" host:"+host.getId()+" NOHERE! ",GuideLogger.ERROR);
		if (netSign[0] != NotNet)
			saveUniv(fform, netSign[0], host);
		if (netSign[1] != NotNet)
			saveCity(fform, netSign[1], host);
		if (netSign[2] != NotNet)
			saveMs(fform, netSign[2], host);
		saveWork(fform,host);

		return null;
	}

	private void saveWork(NoStageFillInfoForm fform, User host) {
		if ("".equals(StringUtils.trimToEmpty(fform.getCompany()))) {
			return;
		}
		userinfoLogic.saveCompany(host, fform.getCompany());
	}

	/**r
	 * 保存中学网络
	 * 
	 * @param fform
	 * @param netSign
	 */
	private void saveMs(NoStageFillInfoForm fform, int netSign, User host) {
		switch (fform.getSchoolType()) {
		case SchoolType.TYPE_HIGHSCHOOL:
			userinfoLogic.saveHighSchool(host, fform.getHighSchoolCode(), fform
					.getHighschoolyear(), fform.getHighSchoolName(),
					netSign == CurrentNet, netSign);
			break;
		case SchoolType.TYPE_JUNIORSCHOOL:
			userinfoLogic.saveJuniorSchool(host, fform.getJuniorHighSchoolId(),
					fform.getHighschoolyear(), fform.getJuniorHighSchoolName(),
					netSign == CurrentNet, netSign);
			break;
		case SchoolType.TYPE_COLLEGESCHOOL:
			userinfoLogic.saveTechSchool(host, fform.getTechHighSchoolId(),
					fform.getHighschoolyear(), fform.getTechHighSchoolName(),
					netSign == CurrentNet, netSign);
			break;
		}
	}

	/**
	 * 保存地域网络
	 * 
	 * @param fform
	 * @param netSign
	 * @param host
	 */
	private void saveCity(NoStageFillInfoForm fform, int netSign, User host) {
		userinfoLogic.saveLocation(host, fform.getHomeprovince(), fform
				.getHomecityName(), fform.getHomecityCode(), netSign);
	}

	/**
	 * 保存大学网络
	 * 
	 * @param fform
	 * @param netSign
	 * @param host
	 */
	private void saveUniv(NoStageFillInfoForm fform, int netSign, User host) {
		userinfoLogic.saveUniv(host, fform.getUniversityyear(), fform
				.getUniversityId(), fform.getUniversityname(), "", fform
				.getDepartment(), netSign);
	}

	/**
	 * 判断时候可以加入大学网络
	 * 
	 * @param fform
	 * @param request
	 * @return
	 */
	private boolean checkUnivNet(NoStageFillInfoForm fform,
			HttpServletRequest request) {
		List<University> univList = null;
		try {
			String ip = BaseActionUtil.getClientIP(request);
			univList = IpTableDAO.getInstance().getUnivByIp(ip);
		} catch (SQLException e) {
			e.printStackTrace();
			return false;
		}
		// 搞ip
		if (univList != null && univList.size() > 0) {
			for (int i = 0; i < univList.size(); i++) {
				if (fform.getUniversityId() == univList.get(i).getId()) {
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * 判断时候可以加入地域网络
	 * 
	 * @param fform
	 * @return
	 */
	private boolean checkCityNet(NoStageFillInfoForm fform) {
		if (StringUtils.isNotEmpty(fform.getHomecityName())
				|| StringUtils.isNotEmpty(fform.getHomeprovince())) {
			return true;
		}
		return false;
	}

	/**
	 * 判断时候可以加入中学网络
	 * 
	 * @param fform
	 * @return
	 */
	private boolean checkMsNet(NoStageFillInfoForm fform) {
		switch (fform.getSchoolType()) {
		case SchoolType.TYPE_HIGHSCHOOL:
			if (StringUtils.isNotEmpty(fform.getHighSchoolName()))
				return true;
			break;
		case SchoolType.TYPE_JUNIORSCHOOL:
			if (StringUtils.isNotEmpty(fform.getJuniorHighSchoolName()))
				return true;
			break;
		case SchoolType.TYPE_COLLEGESCHOOL:
			if (StringUtils.isNotEmpty(fform.getTechHighSchoolName()))
				return true;
			break;
		}
		return false;
	}

	/**
	 * 用户年龄是否小于17
	 * 
	 * @param request
	 * @return
	 */
	private boolean less17age(HttpServletRequest request) {
		return BirthdayLogic.getInstance().less17age(request);
	}

	@Override
	protected ActionForward initPage(HttpServletRequest request,
			ActionMapping mapping) {
		User host = BaseThreadUtil.currentHost(request);
		GuideLogger.printLog(" host:"+host.getId()+" NOHERE! ",GuideLogger.ERROR);
		UserBasic ub = SnsAdapterFactory.getUserBasicAdapter().get(host.getId());
		request.setAttribute("age_less_17", less17age(request));
		request.setAttribute("user_age", ub.getBirthYear());
		return mapping.findForward(FORWARD_SUCC);
	}

	@Override
	protected boolean shouldInit(HttpServletRequest request) {
		return !"post".equalsIgnoreCase(request.getMethod());
	}

	@Override
	protected String thisUrl() {
		return Globals.urlGuide + "/fill-info-ns.do";
	}

	@Override
	protected int thisStatus() {
		return StepStatusLogic.STEP_FILLINFO;
	}

	@Override
	protected String nextUrl() {
		return Globals.urlGuide + "/preview-ns.do";
	}
}
