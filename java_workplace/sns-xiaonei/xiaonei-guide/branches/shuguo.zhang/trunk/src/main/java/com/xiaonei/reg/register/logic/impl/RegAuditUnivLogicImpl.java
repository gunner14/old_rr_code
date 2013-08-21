/**
 *
 */
package com.xiaonei.reg.register.logic.impl;

import com.xiaonei.platform.core.model.University;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.jdbc.dao.UniversityDAO;
import com.xiaonei.platform.core.opt.network.NetworkManager;
import com.xiaonei.reg.common.RegUtil;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.register.dao.IpTableDAO;
import com.xiaonei.reg.register.dao.RegAuditDAO;

import com.xiaonei.reg.register.form.RegCommForm;
import com.xiaonei.reg.register.form.RegForm;
import com.xiaonei.reg.register.form.RegInviteForm;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.logic.IRegAuditLogic;
import com.xiaonei.reg.register.model.RegAppAudit;
import com.xiaonei.reg.register.model.RegAudit;
import com.xiaonei.reg.register.pojo.AuditBasePojo;
import com.xiaonei.reg.register.pojo.AuditUnivPojo;
import com.xiaonei.xce.log.TimeCost;

import java.sql.SQLException;
import java.util.List;

/**
 * RegAuditUnivLogicImpl.java
 * 
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-10-10 上午11:05:01
 */
public class RegAuditUnivLogicImpl implements IRegAuditLogic {
	private static RegAuditUnivLogicImpl _instance = new RegAuditUnivLogicImpl();

	public static RegAuditUnivLogicImpl getInstance() {
		return _instance;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * com.xiaonei.reg.register.logic.RegAuditLogic#audit(com.xiaonei.reg.register
	 * .pojo.AuditUnivPojo)
	 */
	public int audit(AuditBasePojo regp) {
		if (!(regp instanceof AuditUnivPojo)) {
			return 0;
		}
		AuditUnivPojo reg = (AuditUnivPojo) regp;
		String department = reg.getDepartment();
		if (department == null || department.length() == 0) {
			return err_no_department;
		}
		String dormitory = reg.getDormitory();
		if (dormitory == null || dormitory.length() == 0) {
			return err_no_dormitory;
		}
		String studentnumber = "0";

		if (regp.getLargeUrl() == null || "".equals(regp.getLargeUrl())
				|| regp.getMainUrl() == null || "".equals(regp.getMainUrl())
				|| regp.getTinyUrl() == null || "".equals(regp.getTinyUrl())
				|| regp.getHeadUrl() == null || "".equals(regp.getHeadUrl())) {
			return err_no_portrait;
		}

		int returnId = 0;
		try {
			/* 如果是邀请注册需要传入库中的数据 */
			String strAppId = "" + reg.getAppId();
			String appToken = reg.getAppToken();
			String fromType = reg.getFromType();
			int strFromId = reg.getInviterId();

			BaseThreadUtil.beginJDBCTranx();

			RegAudit ra = new RegAudit();
			ra.setEmail(reg.getEmail());
			ra.setPassword(reg.getPwd());
			ra.setName(reg.getName());
			ra.setUniversityId(Integer.valueOf(reg.getUniversityId()));
			ra.setUniversityName(reg.getUniversityName());
			ra.setDepartment(department);
			ra.setDormitory(dormitory);
			ra.setStudentNumber(studentnumber);

			ra.setLargeurl(reg.getLargeUrl());
			ra.setMainurl(reg.getMainUrl());
			ra.setHeadurl(reg.getMainUrl());
			ra.setTinyurl(reg.getTinyUrl());

			ra.setIp(reg.getIp());
			ra.setGender(reg.getGender());
			ra.setDegree(reg.getDegree());
			ra.setInviterId(strFromId);
			ra.setGroupName(reg.getGroupName());

			int id = RegAuditDAO.getInstance().save(ra);
			ra.setId(id);
			returnId = -id;

			if ((null != strAppId && !"".equals(strAppId))
					&& !"0".equals(strAppId)) {
				int appId = (Integer.parseInt(strAppId));
				User fromUser = UserAdapter.getUser(strFromId);
				RegAppAudit raa = new RegAppAudit();
				raa.setId(id);
				raa.setApp_id(appId);
				raa.setApp_token(appToken);
				raa.setFrom_id(fromUser.getId());
				raa.setFrom_name(fromUser.getName());
				raa.setInvite_type(fromType);
				RegAuditDAO.getInstance().insertRegAppAudit(raa);
			}
		} catch (RuntimeException e) {
			e.printStackTrace();
			return err_system;
		} catch (SQLException e) {
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return returnId;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * com.xiaonei.reg.register.logic.RegAuditLogic#shouldBeAudit(com.xiaonei
	 * .reg.register.form.RegForm, java.lang.String)
	 */
	public int shouldBeAudit(BaseRegForm regbForm, String ip) {
		int ruid = 0;
		String remail = "";
		String ric = "";
		if (regbForm instanceof RegForm) {
			RegForm regForm = (RegForm) regbForm;
			ruid = regForm.getUniversityid();
			remail = regForm.getRegEmail();
			ric = regForm.getIc();
		} else if (regbForm instanceof RegInviteForm) {
			RegInviteForm regForm = (RegInviteForm) regbForm;
			ruid = regForm.getUniversityid();
			remail = regForm.getRegEmail();
			ric = regForm.getIc();
		} else if (regbForm instanceof RegCommForm) {
			RegCommForm regForm = (RegCommForm) regbForm;
			ruid = regForm.getUniversityid();
			remail = regForm.getRegEmail();
			ric = regForm.getIc();
		}
		TimeCost cost = TimeCost.begin("RegAuditUnivLogicImpl.shouldBeAudit");
		// isReg isAudit
		// 数据校验
		int univId = 0;
		try {
			univId = ruid;
		} catch (Exception e) {
			// "请选择大学"
			return err_no_choice_univ;
		}
		University university = NetworkManager.getInstance()
				.getUnivById(univId);
		cost.endStep("1");
		if (university == null) {
			// "没有这个大学"
			return err_no_such_univ;
		}
		if (university.getCountryId() == 0) { // 国内大学
			// 判断IP和电子邮箱
			boolean validIP = false;
			boolean validEmail = false;
			List<University> univList = null;
			try {
				univList = IpTableDAO.getInstance().getUnivByIp(ip);
				cost.endStep("2");
			} catch (SQLException e) {
				e.printStackTrace();
				return exception_sql;
			}
			// 搞ip
			if (univList != null && univList.size() > 0) {
				for (int i = 0; i < univList.size(); i++) {
					if (univId == univList.get(i).getId()) {
						validIP = true;
						break;
					}
				}
			}
			cost.endStep("3");
			// ip不行，搞邮件
			if (!validIP) {
				List<String> emailList = null;
				try {
					emailList = UniversityDAO.getInstance()
							.getEmailList(univId);
					cost.endStep("4");
				} catch (SQLException e) {
					e.printStackTrace();
					return exception_sql;
				}
				for (int i = 0; i < emailList.size(); i++) {
					if (remail.endsWith(emailList.get(i))) {
						validEmail = true;
						break;
					}
				}
				cost.endStep("5");
			}
			boolean isValidInvite = RegUtil.isValidInvite(ruid, ric);
			cost.endStep("6");
			if (!isValidInvite && !validIP && !validEmail) { // 转到审核注册
				return isAudit;
			}
			return isReg;
		} else { // 国外大学
			/* 判断ip */
			int count;
			try {
				count = IpTableDAO.getInstance().getCountryByIp(university
						.getCountryId(), ip);
				cost.endStep("7");
			} catch (SQLException e) {
				return exception_sql;
			}
			if (count > 0) {
				return isReg;
			}

			/* 判断电子邮箱 */
			List<String> emailList;
			// 指定大学的email list
			try {
				emailList = UniversityDAO.getInstance().getEmailList(univId);
				cost.endStep("8");
			} catch (SQLException e) {
				// return exception_sql ;
				return isAudit;
			}
			// 后台沒有定以 email list
			if (emailList == null || 0 == emailList.size()) {
				return isAudit;
			}
			// 注册邮箱存在于 email list
			boolean validEmail = false;
			for (int i = 0; emailList != null && i < emailList.size(); i++) {
				if (remail.endsWith(emailList.get(i))) {
					validEmail = true;
					return isReg;
				}
			}
			cost.endStep("9");
			if (!validEmail) {
				return err_invalid_email;
			}

			return isReg;
		}
	}
}
