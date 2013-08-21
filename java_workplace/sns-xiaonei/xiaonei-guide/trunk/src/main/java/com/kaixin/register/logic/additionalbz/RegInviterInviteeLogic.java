package com.kaixin.register.logic.additionalbz;

import java.sql.SQLException;
import java.util.List;

import javax.servlet.http.HttpServletRequest;

import com.kaixin.register.dao.RegInviteMapDAO;
import com.kaixin.register.model.RegInviteMap;

/**
 * 处理邀请人与被邀请人的关系
 * @author wang-tai
 *
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
	 * update reg_invite_map's params field
	 * @param inviterId
	 * @param inviteeId
	 * @param request
	 */
	public void updateRegInviteMapParams(int inviterId, int inviteeId, HttpServletRequest request){
		//目前暂时无用,代码直接返回，不再执行。2009.2.24 by weige
		if(true) return;
		/*String dId = request.getParameter("d_id");
		String dType = request.getParameter("d_t");
		
		String id = request.getParameter("id");
		String tsc = request.getParameter("tsc");
		
		if (dId != null && dType != null && !"".equals(dId) && !"".equals(dType))
		{
			RegInviteMap ri = new RegInviteMap();
			ri.setOwnerId(inviteeId);
			ri.setInviterId(inviterId);
			ri.setParams("d_id:" + dId + ";" + "d_t:" + dType);
			try {
				RegInviteMapDAO.getInstance().updateParams(ri);
			} catch (SQLException e) {
				e.printStackTrace();
			}
		}
		else if (id != null && tsc != null && !"".equals(id) && !"".equals(tsc))
		{
			RegInviteMap ri = new RegInviteMap();
			ri.setOwnerId(inviteeId);
			ri.setInviterId(inviterId);
			ri.setParams("id:" + id + ";" + "tsc:" + tsc);
			try {
				RegInviteMapDAO.getInstance().updateParams(ri);
			} catch (SQLException e) {
				e.printStackTrace();
			}
		}
		
		return;*/
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
			count = RegInviteMapDAO.getInstance().getUserAppCount(appId, ownerId);
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
			friendList = RegInviteMapDAO.getInstance().getByInviterId(inviterId, appId);
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
	 */
	public void createRelation(int inviterId, int inviteeId, String inviterGroupName, int appId) {
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
			List<RegInviteMap> invitee = RegInviteMapDAO.getInstance().getByInviterId(inviter);
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
	 * @param invitee
	 * @return 没有返回0
	 */
	public int getInviterFrom(int invitee) {
		int inviteeid = 0;
		try {
			RegInviteMap inviter = RegInviteMapDAO.getInstance().getByOwnerId(invitee);
			if (inviter != null) {
				inviteeid = inviter.getInviterId();
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
		try {
			inviter = RegInviteMapDAO.getInstance().getByOwnerId(invitee);
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return inviter;
	}
}
