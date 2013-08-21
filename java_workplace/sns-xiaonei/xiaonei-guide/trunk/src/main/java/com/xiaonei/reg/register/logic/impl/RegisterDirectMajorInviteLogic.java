/**
 *
 */
package com.xiaonei.reg.register.logic.impl;

import com.xiaonei.platform.core.model.NetworkStatus;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.Workplace;
import com.xiaonei.reg.common.RegUtil;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.register.dao.InviteDAO;
import com.xiaonei.reg.register.logic.additional.*;
import com.xiaonei.reg.register.logic.buildperson.RegAccountLogic;
import com.xiaonei.reg.register.model.InviteInfo;
import com.xiaonei.reg.register.pojo.*;

import java.sql.SQLException;

/**
 * RegisterDirectMajorInviteLogic.java
 * 
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-10-10 下午05:34:27
 */
public class RegisterDirectMajorInviteLogic extends RegisterBaseLogic {

	private RegisterDirectMajorInviteLogic() {
		super();
	}

	/**
	 * 如果返回代码没有err_activate <br>
	 * LoginLogic.doLoginR(oceUser, request, response, 0, false);<br>
	 * return new ActionForward(Constants.urlWww + "/Home.do", true);
	 */
	@Override
	protected int additional(User oceUser, RegisterPojo regPojo) {
		int code = err_noerr;

		try {
			this.userConfig(oceUser, regPojo);
		} catch (Exception e) {
			code |= err_user_config;
			e.printStackTrace();
		}
		try {
			this.joinNetwork(oceUser, regPojo);
		} catch (Exception e) {
			code |= err_join_network;
			e.printStackTrace();
		}
		try {
			this.initializeInfo(oceUser);
		} catch (Exception e) {
			code |= err_initialize_info;
			e.printStackTrace();
		}

		// group
		try {
			this.setBuddyGroup(oceUser);
		} catch (Exception e) {
			System.err.println("Buddy team failure");
			e.printStackTrace();
		}

		// 邀请链接失效
		try {
			this.invalidateInviteLink(regPojo);
		} catch (SQLException e) {
			code |= err_invalidate_invite_link_sql;
			e.printStackTrace();
		} catch (Exception e) {
			code |= err_invalidate_invite_link;
			e.printStackTrace();
		}
		return code;
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

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * com.xiaonei.reg.register.logic.impl.RegisterBaseLogic#buildPerson(com
	 * .xiaonei.reg.register.pojo.RegisterPojo)
	 */
	@Override
	protected User buildPerson(RegisterPojo regPojo) throws Throwable {
		RegAccountPojo account = new RegAccountPojo();
		account.setAccount(regPojo.getRegEmail().trim());
		account.setGender(regPojo.getGender().trim());
		account.setIp(regPojo.getIp());
		account.setName(regPojo.getName());
		Workplace wpu = RegUtil.getWorkplace(regPojo.getRegEmail().trim());
		if (wpu != null) {
			account.setNetid(wpu.getId());
		}
		account.setPwd(regPojo.getPwd().trim());
		account.setStage(Integer.parseInt(regPojo.getStage().trim()));
		if (RegUtil.isMSNEmail(regPojo.getRegEmail().trim())) {
			account.setMsn(regPojo.getRegEmail().trim());
		}
		account.setHeadUrl(regPojo.getHeadurl());
		account.setMainUrl(regPojo.getMainurl());
		account.setTinyUrl(regPojo.getTinyurl());
		account.setLargeUrl(regPojo.getLargeurl());
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
	 * 加入一个网络，自己玩多没意思
	 * 
	 * @param oceUser
	 * @param form
	 */
	private void joinNetwork(User oceUser, RegisterPojo form) {
		Workplace wp = RegUtil.getWorkplace(form.getRegEmail().trim());
		RegNetworkMajorPojo majorPojo = null;
		if (null != wp) {
			majorPojo = new RegNetworkMajorPojo(UserAdapter
					.get(oceUser.getId()).getStage(), oceUser.getId(), form
					.getRegEmail().trim(), wp.getId(), wp.getName(), form
					.getWorkplacename());
		} else {
			majorPojo = new RegNetworkMajorPojo(UserAdapter
					.get(oceUser.getId()).getStage(), oceUser.getId(), form
					.getRegEmail().trim(), 0, "", form.getWorkplacename());
		}
		RegNetworkLogic.getInstance().setWorkplaceStageNetwork(majorPojo,
				NetworkStatus.CURRENT_NETWORK);
		RegNetworkReginPojo reginPojo = null;
		reginPojo = new RegNetworkReginPojo(UserAdapter.get(oceUser.getId()).getStage(),
				oceUser.getId(), form.getRegEmail().trim(), Integer
						.valueOf(form.getHomecitycode()), form
						.getHomecityname(), form.getHomeprovince());
		RegNetworkLogic.getInstance().setReginStageNetwork(reginPojo,
				NetworkStatus.PASS_NETWORK);
	}

	/**
	 * 初始化各种信息<br>
	 * ViewCountDAO<br>
	 * GossipCountDAO<br>
	 * AlbumInfoDAO<br>
	 * DailyExpDAO<br>
	 * UserPersonalInfoDAO<br>
	 * 
	 * @param oceUser
	 */
	private void initializeInfo(User oceUser) {
		InitializtionCountLogic.getInstance().init(oceUser);
	}

	/**
	 * 邀请链接作废<br>
	 * 只能用一次
	 * 
	 * @throws SQLException
	 */
	private void invalidateInviteLink(RegisterPojo form) throws SQLException {
		InviteInfo info = InviteDAO.getInstance().getInviteInfo(
				form.getInviterId(), form.getInviteeEmail());
		if (info == null)
			return;
		else {
			int inviteId = info.getId();
			InviteDAO.getInstance().updateInviteStatus(inviteId, 2);
		}
	}

	@Override
	protected int additionalSync(User oceUser, RegisterPojo regPojo) {
		// 加好友&&加分组
		User inviter = null;
		try {
			inviter = UserAdapter.getUser(regPojo.getInviterId());
			if (null == inviter) {
				System.err.println("Inviter user does not exist");
			} else {
				this.addInviterMap(oceUser, regPojo);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		// 建立 邀请人，新用户和app建立关系
		try {
			if (null != inviter) {
				RegAppLogic.getInstance().createAppRelation(regPojo.getAppId(),
						regPojo.getInviterId(), inviter.getName(),
						oceUser.getId(), oceUser.getName(),
						regPojo.getAppToken());
			}
		} catch (Exception e) {
			System.err.print("createAppRelation failure");
			e.printStackTrace();
		}
		// news feed
		try {
			if (null != inviter) {
				RegNewsFeedLogic.getInstance().sendNewsFeed2Inviter(inviter,
						oceUser);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return 0;
	}
}
