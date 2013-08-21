package com.xiaonei.reg.register.logic.impl;

import java.sql.SQLException;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.reg.register.logic.additional.InitializtionCountLogic;
import com.xiaonei.reg.register.logic.additional.RegEmailLogic;
import com.xiaonei.reg.register.logic.additional.RegNoActivateLogic;
import com.xiaonei.reg.register.logic.additional.RegUserConfigLogic;
import com.xiaonei.reg.register.logic.buildperson.RegAccountLogic;
import com.xiaonei.reg.register.pojo.RegAccountPojo;
import com.xiaonei.reg.register.pojo.RegUserConfigPojo;
import com.xiaonei.reg.register.pojo.RegisterPojo;

public class RegisterDirectShortCommonLogic extends RegisterBaseLogic {

	protected RegisterDirectShortCommonLogic() {
		super();
	}

	@Override
	protected int additional(User user, RegisterPojo regPojo) {
		int returnCode = err_noerr;
		// user config
		try {
			this.setUserConfig(user, regPojo);
		} catch (Exception e) {
			returnCode |= err_user_config;
			e.printStackTrace();
		}
		// init
		try {
			this.initInfo(user);
		} catch (Exception e) {
			returnCode |= err_initialize_info;
			e.printStackTrace();
		}
		return returnCode;
	}

	@Override
	protected User buildPerson(RegisterPojo regPojo) throws Throwable {
		RegAccountPojo account = new RegAccountPojo();

		account.setPwd(regPojo.getPwd());
		account.setAccount(regPojo.getRegEmail());
		account.setIp(regPojo.getIp());
		account.setName(regPojo.getName());
		account.setGender(regPojo.getGender());
		account.setHeadUrl(regPojo.getHeadurl());
		account.setTinyUrl(regPojo.getTinyurl());
		account.setMainUrl(regPojo.getMainurl());
        account.setBirthYear(regPojo.getBirth_year());
        account.setBirthMonth(regPojo.getBirth_month());
        account.setBirthDay(regPojo.getBirth_day());
		// 用户基本信息，关于描述的信息，都放到持久层去吧！
		User oceUser = RegAccountLogic.getInstance().create(account);

		return oceUser;
	}

	private void setUserConfig(User user, RegisterPojo regPojo) {
		RegUserConfigPojo uc = new RegUserConfigPojo(user);
		RegUserConfigLogic.getInstance().configUserInfo(uc);
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
	private void sendActivateEmail(User user, RegisterPojo regPojo) throws SQLException {
		if(user == null){
			System.err.println("sendActivateEmail user is null");
			return;
		}
		if (StringFormat.isValidEmailAddr(user.getAccount())
		        && !RegNoActivateLogic.getInstance().isNoActivatedSendEmail(user)) {   //增加判断是否是直接激活的判断
			RegEmailLogic.getInstance().sendActivateEmail(user, regPojo);
		}
	}

	@Override
	protected int additionalSync(User oceUser, RegisterPojo regPojo) {
		int returnCode = 0;
		// send active mail
		try {

			this.sendActivateEmail(oceUser, regPojo);
		} catch (SQLException e) {
			returnCode |= err_send_activate_email;
			e.printStackTrace();
		}
		return returnCode ;
	}
}
