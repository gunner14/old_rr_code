/**
 *
 */
package com.xiaonei.reg.register.logic.impl;

import com.xiaonei.platform.core.model.NetworkStatus;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.reg.common.RegUtil;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.register.dao.RegAuditDAO;
import com.xiaonei.reg.register.logic.TimeCost;
import com.xiaonei.reg.register.logic.additional.*;
import com.xiaonei.reg.register.logic.buildperson.RegAccountLogic;
import com.xiaonei.reg.register.pojo.RegAccountPojo;
import com.xiaonei.reg.register.pojo.RegNetworkHSPojo;
import com.xiaonei.reg.register.pojo.RegUserConfigPojo;
import com.xiaonei.reg.register.pojo.RegisterPojo;
import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.domain.network.CollegeInfo;
import mop.hi.oce.domain.network.HighSchoolInfo;
import mop.hi.oce.domain.network.JuniorHighSchoolInfo;

import java.sql.SQLException;

/**
 * 负责高中用户的普通直接注册
 * <p/>
 * RegisterDirectHSCommonLogic.java
 * 
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-9-24 下午09:46:04
 */
public class RegisterDirectHSCommonLogic extends RegisterBaseLogic {

	protected RegisterDirectHSCommonLogic() {
		super();
	}

	@Override
	protected int additional(User user, RegisterPojo regPojo) {
		TimeCost cost = TimeCost.begin();
		int returnCode = err_noerr;
		cost.endStep();
		// user config
		try {
			this.setUserConfig(user, regPojo);
		} catch (Exception e) {
			returnCode |= err_user_config;
			e.printStackTrace();
		}
		cost.endStep();
		// audit hs log
		// save school info to m
		try {
			this.setHSLog(user, regPojo);
		} catch (Exception e) {
			returnCode |= err_hs_log;
			e.printStackTrace();
		}
		cost.endStep();
		// net work
		if (!RegXidLogic.isXid(regPojo.getRegEmail()))
			try {
				this.setNetwork(user, regPojo);
				cost.endStep();
			} catch (Exception e) {
				returnCode |= err_join_network;
				e.printStackTrace();
			}
		// init
		try {
			this.initInfo(user);
			cost.endStep();
		} catch (Exception e) {
			returnCode |= err_initialize_info;
			e.printStackTrace();
		}
		// 好友分组
		try {
			this.setBuddyGroup(user);
			cost.endStep();
		} catch (Exception e) {
			System.err.println("Buddy team failure");
			e.printStackTrace();
		}


		cost.endFinally();
		return returnCode;
	}

	@Override
	protected User buildPerson(RegisterPojo regPojo) throws Throwable {
		RegAccountPojo account = new RegAccountPojo();

		account.setPwd(regPojo.getPwd());
		account.setStage(Integer.valueOf(regPojo.getStage()));
		account.setAccount(regPojo.getRegEmail());
		account.setIp(regPojo.getIp());
		account.setName(regPojo.getName());
		account.setGender(regPojo.getGender());
		account.setHeadUrl(regPojo.getHeadurl());
		account.setTinyUrl(regPojo.getTinyurl());
		account.setMainUrl(regPojo.getMainurl());
		account.setHighSchoolCode(regPojo.getHighSchoolCode());
		account.setHighSchoolName(regPojo.getHighSchoolName());
		// 添加网络
		try {
			int hsNetWorkId = RegUtil.getHsNetWorkId(Integer.valueOf(regPojo
					.getHighSchoolCode()), Integer.valueOf(regPojo
					.getSchoolType()));
			account.setNetid(hsNetWorkId);
		} catch (Exception e) {
			e.printStackTrace();
		}
		// 用户基本信息，关于描述的信息，都放到持久层去吧！
		User oceUser = RegAccountLogic.getInstance().create(account);

		return oceUser;
	}

	/**
	 * 建立默认好友分组
	 * 
	 * @param oceUser
	 */
	private void setBuddyGroup(User oceUser) {
		com.xiaonei.platform.component.friends.logic.GroupLogic
				.addDefaultGroup(oceUser.getId(), UserAdapter.get(oceUser.getId()).getStage());
	}

	/**
	 * 设置user config
	 * 
	 * @param user
	 * @param regPojo
	 * @author (wang-tai)tai.wang@opi-corp.com 2008-9-28 上午11:50:13
	 */
	private void setUserConfig(User user, RegisterPojo regPojo) {
		RegUserConfigPojo uc = new RegUserConfigPojo(user);
		RegUserConfigLogic.getInstance().configUserInfo(uc);
	}

	/**
	 * 高中记录学校log（业务不清）
	 * 
	 * @param user
	 * @param regPojo
	 * @throws NumberFormatException
	 * @throws SQLException
	 * @author (wang-tai)tai.wang@opi-corp.com 2008-9-28 下午03:59:05
	 */
	private void setHSLog(User user, RegisterPojo regPojo)
			throws NumberFormatException, SQLException {
		RegAuditDAO.getInstance().saveSchoolType(user.getId(),
				Integer.valueOf(regPojo.getSchoolType()));

		if (Integer.valueOf(regPojo.getSchoolType()) == 1) {
			JuniorHighSchoolInfo info = new JuniorHighSchoolInfo();
			info.setJunHighentarySchoolId(Integer.valueOf(regPojo
					.getHighSchoolCode()));
			info.setJunHighentarySchoolName(regPojo.getHighSchoolName());
			info.setJunHighentarySchoolYear(regPojo.getEnrollYear());
			info.setUserId(user.getId());
			info = AdapterFactory.getNetworkAdapter().addJuniorHighSchoolInfo(
					user.getId(), info);
		} else if (Integer.valueOf(regPojo.getSchoolType()) == 2) {
			CollegeInfo info = new CollegeInfo();
			info.setCollegeId(Integer.valueOf(regPojo.getHighSchoolCode()));
			info.setCollegeName(regPojo.getHighSchoolName());
			info.setEnrollYear(regPojo.getEnrollYear());
			info.setUserId(user.getId());
			info = AdapterFactory.getNetworkAdapter().addCollegeInfo(
					user.getId(), info);
		} else /* high school 0 */{
			HighSchoolInfo info = new HighSchoolInfo();
			info.setHighSchoolId(Integer.valueOf(regPojo.getHighSchoolCode()));
			info.setHighSchoolName(regPojo.getHighSchoolName());
			info.setEnrollYear(regPojo.getEnrollYear());
			info.setUserId(user.getId());
			info.setHClass1("" + 0);
			info.setHClass2("" + 0);
			info.setHClass3("" + 0);
			info = AdapterFactory.getNetworkAdapter().addHighSchoolInfo(info);
		}
	}

	/**
	 * 设置高中网路
	 * 
	 * @param user
	 * @param regPojo
	 * @throws SQLException
	 * @throws NumberFormatException
	 * @author (wang-tai)tai.wang@opi-corp.com 2008-9-28 下午04:16:31
	 */
	private void setNetwork(User user, RegisterPojo regPojo)
			throws NumberFormatException, SQLException {
		int infoId = -1;// 不明白为什么是这个奶奶样
		// int juniorHighSchoolId =
		// Integer.valueOf(regPojo.getHighSchoolCode());
		// if (Integer.valueOf(regPojo.getSchoolType()) == 1) {
		// int jhsid =
		// RegAuditDAO.getInstance().getJuniorSchoolIdByHighSchoolId(Integer.valueOf(regPojo.getHighSchoolCode()));
		// if (jhsid > 0)
		// juniorHighSchoolId = jhsid;
		// }
		int hsNetWorkId = RegUtil
				.getHsNetWorkId(Integer.valueOf(regPojo.getHighSchoolCode()),
						Integer.valueOf(regPojo.getSchoolType()));
		RegNetworkHSPojo net = new RegNetworkHSPojo(Stage.STAGE_HEIGHSCHOOL,
				user.getId(), infoId, hsNetWorkId, regPojo.getHighSchoolName());
		RegNetworkLogic.getInstance().setHSNetwork(net,
				NetworkStatus.CURRENT_NETWORK);
	}

	/**
	 * 初始化用户信息
	 * 
	 * @param user
	 * @author (wang-tai)tai.wang@opi-corp.com 2008-9-28 下午04:17:13
	 */
	private void initInfo(User user) {
		InitializtionCountLogic.getInstance().init(user);
	}

	/**
	 * 发送激活信
	 * 
	 * @param user
	 * @throws SQLException
	 * @author (wang-tai)tai.wang@opi-corp.com 2008-9-28 下午04:18:23
	 */
	private void sendActivateEmail(User user) throws SQLException {
	    if (StringFormat.isValidEmailAddr(user.getAccount())
                && !RegNoActivateLogic.getInstance().isNoActivatedSendEmail(user)) {   //增加判断是否是直接激活的判断
			RegEmailLogic.getInstance().sendActivateEmail(user);
		}
	}

	@Override
	protected int additionalSync(User oceUser, RegisterPojo regPojo) {
		// send active mail
		try {

			this.sendActivateEmail(oceUser);
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return 0;
	}

}
