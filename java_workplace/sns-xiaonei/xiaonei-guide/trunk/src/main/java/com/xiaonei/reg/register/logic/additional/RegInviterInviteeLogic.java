/**
 *
 */
package com.xiaonei.reg.register.logic.additional;

import com.xiaonei.reg.register.dao.RegInviteMapDAO;
import com.xiaonei.reg.register.logic.TimeCost;
import com.xiaonei.reg.register.model.RegInviteMap;

import java.sql.SQLException;
import java.util.List;

/**
 * @author wang-tai
 */
public class RegInviterInviteeLogic {

	private RegInviterInviteeLogic() {
		super();
	}

	private static RegInviterInviteeLogic _instance = new RegInviterInviteeLogic();

	public static RegInviterInviteeLogic getInstance() {
		return _instance;
	}

	/**
	 * 如果是app邀请，则把该用户的appid更新到对应记录上去
	 * 
	 * @param inviterId
	 * @param inviteeId
	 * @param appId
	 */
	public void updateAppId(int inviterId, int inviteeId, int appId) {
		RegInviteMap ri = new RegInviteMap();
		ri.setOwnerId(inviteeId);
		ri.setInviterId(inviterId);
		ri.setAppId(appId);
		try {
			RegInviteMapDAO.getInstance().updateAppId(ri);
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}

	/**
	 * 获取用户app的邀请总数
	 * 
	 * @param ownerId
	 * @param appId
	 * @return
	 */
	public int getUserInviteCountByAppId(int ownerId, int appId) {
		int count = 0;
		try {
			count = RegInviteMapDAO.getInstance().getUserAppCount(appId,
					ownerId);
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return count;
	}

	/**
	 * 获取邀请人的id
	 * 
	 * @param inviteeId
	 * @return
	 */
	public int getInviterId(int inviteeId) {
		int id = 0;
		try {
			id = RegInviteMapDAO.getInstance().getInviterId(inviteeId);
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return id;
	}

	/**
	 * @param inviterId
	 * @param appId
	 * @return
	 */
	public List<RegInviteMap> getFriendIdList(int inviterId, int appId) {

		List<RegInviteMap> friendList = null;
		try {
			friendList = RegInviteMapDAO.getInstance().getByInviterId(
					inviterId, appId);
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return friendList;
	}

	/**
	 * 建立邀请者与被邀请者关系
	 * 
	 * @param inviterId
	 * @param inviteeId
	 * @param inviterGroupName
	 * @param appId
	 */
	public void createRelation(int inviterId, int inviteeId,
			String inviterGroupName, int appId) {
		RegInviteMap ri = new RegInviteMap();
		ri.setOwnerId(inviteeId);
		ri.setInviterId(inviterId);
		ri.setInviterGroupName(inviterGroupName);
		ri.setAppId(appId);
		try {
			RegInviteMapDAO.getInstance().save(ri);
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}

	/**
	 * 得到被邀请者信息
	 * 
	 * @param invitee
	 * @return
	 */
	public int getInviteeFrom(int inviter) {
		int inviteeid = 0;
		try {
			List<RegInviteMap> invitee = RegInviteMapDAO.getInstance()
					.getByInviterId(inviter);
			if (invitee != null) {
				if (0 != invitee.size()) {
					inviteeid = invitee.get(0).getOwnerId();
				}
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return inviteeid;
	}

	/**
	 * 得到邀请者信息
	 * 
	 * @param inviter
	 * @return 没有返回0
	 */
	public int getInviterFrom(int inviter) {
		int inviteeid = 0;
		try {
			RegInviteMap invitee = RegInviteMapDAO.getInstance().getByOwnerId(
					inviter);
			if (invitee != null) {
				inviteeid = invitee.getInviterId();
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return inviteeid;
	}

	/**
	 * 根据被邀请者得到邀请映射表的信息
	 * 
	 * @param invitee
	 * @return 没有返回null
	 */
	public RegInviteMap getRegInviteMapByInvitee(int invitee) {
		RegInviteMap inviter = null;
		TimeCost cost = TimeCost.begin();
		try {
			inviter = RegInviteMapDAO.getInstance().getByOwnerId(invitee);
			cost.endStep();
		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			cost.endFinally();
		}
		return inviter;
	}
}
