package com.xiaonei.reg.guide.action;

import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.platform.core.model.College;
import com.xiaonei.platform.core.model.HighSchool;
import com.xiaonei.platform.core.model.NetworkStatus;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.util.BaseActionUtil;
import com.xiaonei.platform.core.utility.Escape;
import com.xiaonei.reg.guide.dao.HighSchoolDAO;
import com.xiaonei.reg.guide.logic.NetworkUtils;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.logic.fillinfo.DXFillinfoLogic;
import com.xiaonei.reg.guide.logic.fillinfo.HSFillinfoLogic;
import com.xiaonei.reg.guide.logic.fillinfo.HighSchoolCity;
import com.xiaonei.reg.guide.logic.portrait.CommonPortraitLogic;
import com.xiaonei.reg.guide.model.ElementarySchool;
import com.xiaonei.reg.guide.pojo.CommonPortraitPojo;
import com.xiaonei.reg.guide.pojo.HSFillInfoElementary;
import com.xiaonei.reg.guide.pojo.HSFillInfoJuniorPojo;
import com.xiaonei.reg.guide.pojo.ProCity;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.Utils;
import com.xiaonei.reg.register.dao.CollegeDAO;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegOprDefine;
import com.xiaonei.xce.log.TimeCost;
import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.domain.network.CollegeInfo;
import mop.hi.oce.domain.network.HighSchoolInfo;
import mop.hi.oce.domain.network.Network;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.validator.DynaValidatorForm;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.sql.SQLException;
import java.util.List;

/**
 * 技校fillinfo页<br>
 * <p/>
 * HSFillinfoAction.java
 */
public class JXFillinfoAction extends ProtectAction {

	private final static String fail = "jx";
	private final static String forward_jx_info = "jx";
	// form property
	private final static String form_juniorhighschoolyear = "juniorhighschoolyear";
	private final static String form_elementaryschoolyear = "elementaryschoolyear";

	@Override
	public ActionForward executePt(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws SQLException {
		TimeCost cost = TimeCost.begin("DispatchViewAction");

		User host = BaseThreadUtil.currentHost(request);
		GuideLogger.printLog(" host:"+host.getId()+" NOHERE! ",GuideLogger.ERROR);
		DynaValidatorForm infoForm = (DynaValidatorForm) form;
		try {
			UserTraceLogic.getInstance().insertRegHis(request, host,
					host.getAccount(), String.valueOf(RegOprDefine.FILL_INFO),
					UserAdapter.get(host).getStage(), null, null, null, null, 0);
			cost.endStep("1");
			// 检查用户是否完善过资料
			String toUrl = StepStatusLogic.goWhere(host);
			if (!Utils.getInstance().validateForString(toUrl)
					&& !toUrl
							.equalsIgnoreCase(Globals.urlGuide+"/fillinfohs.do")) {
				return new ActionForward(toUrl, true);
			}
			cost.endStep("2");
			// 初始化页面
			if (!"post".equalsIgnoreCase(request.getMethod())) {
				try {
					initSchoolInfo(request, cost, host);
				} catch (Exception e) {
					e.printStackTrace();
				}
				return mapping.findForward(forward_jx_info);
			}
			// 处理页面提交数据
			else {
				request.setAttribute("userBasic", DXFillinfoLogic.getInstance()
						.getUserBasic(host));
				cost.endStep("6");
				int schoolType = HSFillinfoLogic.getInstance().getSchoolType(
						host);
				cost.endStep("7");
				HSFillinfoLogic.getInstance().checkSchoolType(schoolType, host);
				cost.endStep("8");
				request.setAttribute("highSchool", HSFillinfoLogic
						.getInstance().getHighSchool(host));
				cost.endStep("9");
				// 更改初中和入学年
				int juniorHighSchoolId = 0;
				String juniorHighSchoolName = Escape
						.stringToHTMLString(infoForm
								.getString("juniorhighschoolname"));
				int juniorHighSchoolYear = 0;
				if (HSFillinfoLogic.getInstance().checkSchoolType(schoolType)) {
					cost.endStep("10");
					request.setAttribute("juniorHighSchoolName",
							juniorHighSchoolName);
					HighSchool highschool = HSFillinfoLogic.getInstance()
							.getHighSchoolByName(juniorHighSchoolName);
					cost.endStep("11");
					if (highschool != null) {
						juniorHighSchoolId = highschool.getId();
					}
				}

				// 更改小学和入学年
				int elementarySchoolId = 0;
				String elementarySchoolName = Escape
						.stringToHTMLString(infoForm
								.getString("elementaryschoolname"));
				int elementarySchoolYear = 0;
				request.setAttribute("elementarySchoolName",
						elementarySchoolName);
				ElementarySchool elementarySchool = HSFillinfoLogic
						.getInstance().getElementarySchoolByName(
								elementarySchoolName);
				cost.endStep("12");
				if (elementarySchool != null) {
					elementarySchoolId = elementarySchool.getId();
				}

				try {
					elementarySchoolYear = Integer.parseInt(Escape
							.stringToHTMLString(infoForm
									.getString(form_elementaryschoolyear)));
					request.setAttribute("elementarySchoolYear",
							elementarySchoolYear);
				} catch (Exception e) {
					BaseActionUtil.addErr(request, "无效的小学入学年");
					return mapping.findForward(fail);
				}
				/* 其次判断初中相关信息 */
				if (HSFillinfoLogic.getInstance().checkSchoolType(schoolType)) {
					cost.endStep("14");
					if (HSFillinfoLogic.getInstance()
							.checkJuniorHighSchoolInfo(juniorHighSchoolName)) {
						cost.endStep("15");
						BaseActionUtil.addErr(request, "请输入初中");
						return mapping.findForward(fail);
					}
					try {
						juniorHighSchoolYear = Integer.parseInt(Escape
								.stringToHTMLString(infoForm
										.getString(form_juniorhighschoolyear)));
						request.setAttribute("juniorhighschoolyear",
								juniorHighSchoolYear);
					} catch (Exception e) {
						BaseActionUtil.addErr(request, "无效的初中入学年");
						return mapping.findForward(fail);
					}
				}
				/* 再次判断小学信息 */
				if (Utils.getInstance().validateForString(elementarySchoolName)) {
					cost.endStep("16");
					BaseActionUtil.addErr(request, "请输入小学");
					return mapping.findForward(fail);
				}
				if (HSFillinfoLogic.getInstance().checkSchoolType(schoolType)) {
					cost.endStep("17");
					if (elementarySchoolYear >= juniorHighSchoolYear) {// 小学入学年份必须小于初中的入学年份
						BaseActionUtil.addErr(request, "请正确输入初中,小学入学年");
						return mapping.findForward(fail);
					}
				}
				/*
				 * 更新用户资料
				 */
				// 初中资料
				if (HSFillinfoLogic.getInstance().checkSchoolType(schoolType)) {
					cost.endStep("19");
					HSFillInfoJuniorPojo hsf = new HSFillInfoJuniorPojo();
					hsf.setHost(host);
					hsf.setJuniorHighSchoolId(juniorHighSchoolId);
					hsf.setJuniorHighSchoolName(juniorHighSchoolName);
					hsf.setJuniorHighSchoolYear(juniorHighSchoolYear);
					HSFillinfoLogic.getInstance().updateJuniorSchoolInfo(hsf);
					cost.endStep("20");
				}
				HSFillInfoElementary hsfi = new HSFillInfoElementary();
				hsfi.setElementarySchoolId(elementarySchoolId);
				hsfi.setElementarySchoolName(elementarySchoolName);
				hsfi.setElementarySchoolYear(elementarySchoolYear);
				hsfi.setUser(host);
				HSFillinfoLogic.getInstance().updateElementarySchoolInfo(hsfi);
				cost.endStep("21");

				/*
				 * 更新用户的照片资料
				 */
				String headpic = Escape.stringToHTMLString(infoForm
						.getString("headUrl"));
				String largepic = Escape.stringToHTMLString(infoForm
						.getString("largeUrl"));
				String mainpic = Escape.stringToHTMLString(infoForm
						.getString("mainUrl"));
				String tinypic = Escape.stringToHTMLString(infoForm
						.getString("tinyUrl"));
				CommonPortraitPojo pp = new CommonPortraitPojo();
				pp.setHead(headpic);
				pp.setLarge(largepic);
				pp.setMain(mainpic);
				pp.setTiny(tinypic);
				CommonPortraitLogic.getInstance().updateHeadUrl(host, pp);
				// 存入加星系统
				CommonPortraitLogic.save(host);

				// 记录用户是否完善过资料
				StepStatusLogic.updateStepStatus(host.getId(),
						StepStatusLogic.STEP_FILLINFO);
				cost.endStep("22");

				return new ActionForward(Globals.urlGuide + "/previewhs.do",
						true);
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			cost.endFinally();
		}
		return null;
	}

	/**
	 * <ul>
	 * <li>初始化学校信息</li>
	 * </ul>
	 * 
	 * @param request
	 * @param cost
	 * @param host
	 * @throws SQLException
	 */
	private void initSchoolInfo(HttpServletRequest request, TimeCost cost,
			User host) throws SQLException {
		List<Network> nw = AdapterFactory.getNetworkAdapter()
				.getNetworkListByStatus(host.getId(),
						NetworkStatus.CURRENT_NETWORK);

		if (null != nw)
			request.setAttribute("test", nw.size());
		if (nw != null && nw.size() > 0) {
			int networkid = nw.get(0).getNetworkId();
			request.setAttribute("networkid", networkid);
			int[] schoolInfo = this.getHighSchoolInfo(host, networkid);
			cost.endStep("3");

			if (null != schoolInfo) {
				String code = null;

				if (NetworkUtils.isCollegeNetwork(networkid)) {// 中专技校
					College co = CollegeDAO.getInstance()
							.getByID(schoolInfo[0]);
					if (co != null) {
						code = co.getCode();
					}
				} else {// 高中
					/* 初始化家乡 */
					code = HighSchoolDAO.getInstance().getHighSchoolCode(
							schoolInfo[0]);
				}
				cost.endStep("4");

				if (code != null) {

					ProCity pc = HighSchoolCity.getInstance()
							.getAreaByHighSchoolId(code);

					cost.endStep("5");
					if (pc != null) {
						request.setAttribute("homeProvince", pc.getProvince());
						request.setAttribute("homeCity", pc.getCity());
					}
				}

				/* 初始化入学年 */
				// 初始化初中入学年
				request.setAttribute("juniorhighschoolyear", schoolInfo[1] - 3);
				// 初始化小学入学年
				request.setAttribute("elementarySchoolYear", schoolInfo[1] - 9);

			}
			if (NetworkUtils.isJuniorNetwork(networkid)) {
				int junYear = AdapterFactory.getNetworkAdapter()
						.getJuniorHighSchoolInfo(host.getId())
						.getJunHighentarySchoolYear();
				request.setAttribute("elementarySchoolYear", junYear - 6);
			}
		}
	}

	/**
	 * 更新高中班级
	 * 
	 * @param host
	 *            用户
	 * @param request
	 *            HttpServletRequest 对象
	 * @param infoForm
	 *            表单对象
	 * @author seraph
	 */
	@SuppressWarnings("unused")
	private void updateHSClassInfo(User host, HttpServletRequest request,
			DynaValidatorForm infoForm) {

		try {
			/* 更改高中班级 */
			String highSchoolClass1 = this.getHighSchoolClass(infoForm,
					"highschoolclass1", request, 1);
			String highSchoolClass2 = this.getHighSchoolClass(infoForm,
					"highschoolclass2", request, 2);
			String highSchoolClass3 = this.getHighSchoolClass(infoForm,
					"highschoolclass3", request, 3);

			/* 获取自定义输入的班级 */
			String custom_class_text_1 = this.getHighSchoolClass(infoForm,
					"custom_class_text_1", request, 1);
			String custom_class_text_2 = this.getHighSchoolClass(infoForm,
					"custom_class_text_2", request, 2);
			String custom_class_text_3 = this.getHighSchoolClass(infoForm,
					"custom_class_text_3", request, 3);
			// 高中
			HighSchoolInfo highSchoolInfo = null;
			List<Network> nw = AdapterFactory.getNetworkAdapter()
					.getNetworkListByStatus(host.getId(),
							NetworkStatus.CURRENT_NETWORK);
			if (nw != null && nw.size() > 0) {
				highSchoolInfo = AdapterFactory.getNetworkAdapter()
						.getHighSchoolInfoBySchoolId(host.getId(),
								nw.get(0).getNetworkId());
			}
			if (highSchoolInfo != null) {
				if (highSchoolClass1.equals("")) {
					highSchoolInfo.setHClass1(custom_class_text_1);

				} else {
					highSchoolInfo.setHClass1(highSchoolClass1);
				}
				if (highSchoolClass2.equals("")) {
					highSchoolInfo.setHClass2(custom_class_text_2);

				} else {
					highSchoolInfo.setHClass2(highSchoolClass2);
				}
				if (highSchoolClass3.equals("")) {
					highSchoolInfo.setHClass3(custom_class_text_3);
				} else {
					highSchoolInfo.setHClass3(highSchoolClass3);
				}
				// 更新高中用户班级信息
				AdapterFactory.getNetworkAdapter().setHighSchoolInfo(
						highSchoolInfo);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * 处理从前台读取到的班级
	 * 
	 * @param infoForm
	 *            表单对象
	 * @param str
	 *            要处理的班级名称
	 * @param request
	 *            会话请求
	 * @param index
	 *            组装班级属性
	 * @return 返回班机名称
	 * @author seraph
	 */
	private String getHighSchoolClass(DynaValidatorForm infoForm, String str,
			HttpServletRequest request, int index) {

		String result = "";
		try {
			result = Escape.stringToHTMLString(infoForm.getString(str));
			if (str.startsWith("highSchoolClass")) {
				request.setAttribute("highSchoolClass" + index, str);
			} else {
				request.setAttribute("custom_class_text_" + index, str);
			}

		} catch (Exception e) {
		}
		return result;
	}

	/**
	 * 获得高中信息
	 * 
	 * @param user
	 * @return [高中／技校编号,入学年月]
	 * @author (wang-tai)tai.wang@opi-corp.com 2008-11-5 下午02:52:31 modified by
	 *         llb
	 */
	private int[] getHighSchoolInfo(User user, int networkid) {
		int[] schoolInfo = null;
		try {
			if (NetworkUtils.isCollegeNetwork(networkid)) {// 中专技校
				CollegeInfo co = AdapterFactory.getNetworkAdapter()
						.getCollegeInfoByCollegeId(user.getId(), networkid);
				if (co != null) {
					schoolInfo = new int[2];
					schoolInfo[0] = co.getCollegeId();
					schoolInfo[1] = co.getEnrollYear();
				}
			} else {
				HighSchoolInfo hi = AdapterFactory.getNetworkAdapter()
						.getHighSchoolInfoBySchoolId(user.getId(), networkid);
				if (hi != null) {
					schoolInfo = new int[2];
					schoolInfo[0] = hi.getHighSchoolId();
					schoolInfo[1] = hi.getEnrollYear();
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return schoolInfo;
	}
}