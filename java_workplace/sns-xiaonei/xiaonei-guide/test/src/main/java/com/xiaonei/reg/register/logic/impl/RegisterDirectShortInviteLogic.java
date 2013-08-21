package com.xiaonei.reg.register.logic.impl;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.register.dao.InviteDAO;
import com.xiaonei.reg.register.logic.additional.*;
import com.xiaonei.reg.register.logic.buildperson.RegAccountLogic;
import com.xiaonei.reg.register.pojo.RegAccountPojo;
import com.xiaonei.reg.register.pojo.RegUserConfigPojo;
import com.xiaonei.reg.register.pojo.RegisterPojo;

import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;

public class RegisterDirectShortInviteLogic extends RegisterBaseLogic {

	private RegisterDirectShortInviteLogic() {
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
		// 好友分组
		try {
			this.setBuddyGroup(user);
		} catch (Exception e) {
			System.err.println("Buddy team failure");
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
	 * 如果存在邀请，则在邀请表上增加一条数据
	 * 
	 * @param oceUser
	 * @param regPojo
	 */
	private void addInviterMap(User oceUser, RegisterPojo regPojo) {
		// 如果存在邀请，则在邀请表上增加一条数据
		if (regPojo != null && regPojo.getInviterId() > 0) {
			String groupName = regPojo.getGroupName();
			if ("".equals(groupName) || null == groupName) {
				try {
					groupName = InviteDAO.getInstance().getFriendGroup(
							regPojo.getInviterId(), regPojo.getRegEmail());
				} catch (Exception ex) {
					ex.printStackTrace();
				}
				if (null == groupName) {
					groupName = "";
				}
			}
			RegFriendShipLogic.getInstance().createRelation(
					regPojo.getInviterId(), oceUser.getId(), groupName,
					regPojo.getAppId());
		}
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
		if (user == null) {
			System.err
					.println("RegisterDirectShortInviteLogic sendActivateEmail user is null");
			return;
		}
		if (StringFormat.isValidEmailAddr(user.getAccount())
		        && !RegNoActivateLogic.getInstance().isNoActivatedSendEmail(user)) {
			RegEmailLogic.getInstance().sendActivateEmail(user);
		}
	}

	@Override
	protected int additionalSync(final User user, final RegisterPojo regPojo) {
		final User inviter = UserAdapter.getUser(regPojo.getInviterId());
		Map<String, Thread> tasksMap = new HashMap<String, Thread>();
		// 加好友&&加分组
		tasksMap.put("add friend", new Thread() {
			@Override
			public void run() {
				try {
					if (null == inviter) {
						System.err.println("Inviter user does not exist");
					} else {
						addInviterMap(user, regPojo);
					}
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
		// 建立 邀请人，新用户和app建立关系
		tasksMap.put("app logic", new Thread() {
			@Override
			public void run() {
				try {
					if (null != inviter) {
						RegAppLogic.getInstance().createAppRelation(
								regPojo.getAppId(), regPojo.getInviterId(),
								inviter.getName(), user.getId(),
								user.getName(), regPojo.getAppToken());
					}
				} catch (Exception e) {
					System.err.print("createAppRelation failure");
					e.printStackTrace();
				}
			}
		});
		// send active mail
		tasksMap.put("send mail", new Thread() {
			@Override
			public void run() {
				try {
					sendActivateEmail(user);
				} catch (SQLException e) {
					e.printStackTrace();
				}
			}
		});

		RunnableTaskExecutor.getInstance().runConcurrentTasks(
				"short invite reg logic", tasksMap);
		return 0;
	}

}
