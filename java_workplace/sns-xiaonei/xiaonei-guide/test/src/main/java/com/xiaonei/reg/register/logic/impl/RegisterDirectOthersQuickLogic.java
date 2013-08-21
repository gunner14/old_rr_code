/**
 *
 */
package com.xiaonei.reg.register.logic.impl;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.reg.common.RegUtil;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.register.logic.additional.RegEmailLogic;
import com.xiaonei.reg.register.logic.additional.RegNoActivateLogic;
import com.xiaonei.reg.register.logic.additional.RegUserConfigLogic;
import com.xiaonei.reg.register.logic.buildperson.RegAccountLogic;
import com.xiaonei.reg.register.pojo.RegAccountPojo;
import com.xiaonei.reg.register.pojo.RegUserConfigPojo;
import com.xiaonei.reg.register.pojo.RegisterPojo;

import java.sql.SQLException;

/**
 * 负责以上都不是阶段用户的quick注册
 * 
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-9-24 下午04:12:36
 */
public class RegisterDirectOthersQuickLogic extends RegisterBaseLogic {

	private RegisterDirectOthersQuickLogic() {
		super();
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.xiaonei.reg.register.logic.RegisterBaseLogic
	 * #additional(com.xiaonei.platform.core.model.User,
	 * com.xiaonei.reg.register.form.RegForm)
	 */
	@Override
	protected int additional(User oceUser, RegisterPojo regForm) {
		int code = err_noerr;
		try {
			this.userConfig(oceUser, regForm);
		} catch (Exception e) {
			code |= err_user_config;
			e.printStackTrace();
		}

		// group
		try {
			this.setBuddyGroup(oceUser);
		} catch (Exception e) {
			System.err.println("Buddy team failure");
			e.printStackTrace();
		}

		return code;
	}

	/**
	 * 建立默认好友分组
	 * 
	 * @param oceUser
	 */
	private void setBuddyGroup(User oceUser) {
		com.xiaonei.platform.component.friends.logic.GroupLogic
				.addDefaultGroup(oceUser.getId(), UserAdapter.get(
						oceUser.getId()).getStage());
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * com.xiaonei.reg.register.logic.RegisterDirectBaseLogic#buildPerson(com
	 * .xiaonei.reg.register.form.RegisterPojo,
	 * javax.servlet.http.HttpServletRequest)
	 */
	@Override
	protected User buildPerson(RegisterPojo regPojo) throws Throwable {
		RegAccountPojo account = new RegAccountPojo();
		account.setAccount(regPojo.getRegEmail().trim());
		account.setGender(regPojo.getGender().trim());
		account.setIp(regPojo.getIp());
		account.setName(regPojo.getName());
		account.setNetid(0);
		account.setPwd(regPojo.getPwd().trim());
		account.setStage(Integer.parseInt(regPojo.getStage().trim()));
		if (RegUtil.isMSNEmail(regPojo.getRegEmail().trim())) {
			account.setMsn(regPojo.getRegEmail().trim());
		}
		// 用户基本信息，关于描述的信息，都放到持久层去吧！
		User oceUser = RegAccountLogic.getInstance().create(account);

		return oceUser;
	}

	/**
	 * 配置user　config 信息<br>
	 * 菜单页面啥的
	 * 
	 * @param oceUser
	 * @param form
	 */
	private void userConfig(User oceUser, RegisterPojo form) {
		RegUserConfigPojo ucp = new RegUserConfigPojo(oceUser);
		RegUserConfigLogic.getInstance().configUserInfo(ucp);
	}

	/**
	 * 发送激活信
	 * 
	 * @param user
	 * @throws SQLException
	 * @author (wang-tai)tai.wang@opi-corp.com 2008-9-24 下午03:58:17
	 */
	private void sendActivateEmail(User user) throws SQLException {
	    if (StringFormat.isValidEmailAddr(user.getAccount())
                && !RegNoActivateLogic.getInstance().isNoActivatedSendEmail(user)) {   //增加判断是否是直接激活的判断
	        RegEmailLogic.getInstance().sendActivateEmail(user);
	    }
	}

	@Override
	protected int additionalSync(User oceUser, RegisterPojo regPojo) {
		try {
			this.sendActivateEmail(oceUser);
		} catch (SQLException e) {
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return 0;
	}
}
