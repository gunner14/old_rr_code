package com.xiaonei.reg.register.logic.additional;

import com.xiaonei.reg.register.dao.InviteDAO;
import com.xiaonei.reg.register.model.InviteInfo;
import com.xiaonei.xce.log.TimeCost;

import java.sql.SQLException;

/**
 * 负责管理邀请注册用户的注册类型
 * 
 * @author wang-tai
 */
public class RegInviteTypeLogic {
	private static RegInviteTypeLogic _instance = new RegInviteTypeLogic();

	public static RegInviteTypeLogic getInstance() {
		return _instance;
	}

	private RegInviteTypeLogic() {
	}

	// 沒有被邀請
	public static final int REG_INVITE_NO = 90;
	// 通過邀請鏈接，改變了email
	/**
	 * @author tai.wang@opi-cor.com
	 * @deprecated 因为审核表中现在没有invitee email 字段，所以该属性判断结果有问题
	 */
	public static final int REG_INVITE_CHANGEMAIL = 91;
	// 沒有點邀請鏈接, 但被邀請
	public static final int REG_INVITE_NOTLINK = 92;
	// 通過邀請鏈接
	public static final int REG_INVITE_LINK = 93;

	/**
	 * 判断MSN邀请的类型
	 * 
	 * @param request
	 * @param inviteeEmail
	 * @param email
	 * @return
	 * @author tai.wang@opi-corp.com
	 */
	public int checkInviteType(int inviterId, String inviteeEmail,
			String regEmail) {
		TimeCost cost = TimeCost.begin("RegInviteTypeLogic.checkInviteType");

		int inviteStatus = REG_INVITE_NO;
		try {
			InviteInfo inviteInfo = null;
			try {
				inviteInfo = InviteDAO.getInstance().getInviteInfo(inviterId,
						inviteeEmail);
				cost.endStep("1");
			} catch (SQLException e1) {
				e1.printStackTrace();
			}
			// 注意！此处得到的是inviteInfo 中的自增id
			int inviteId = 0;
			if (null == inviteInfo) {
				return inviteStatus;
			}
			inviteId = inviteInfo.getId();

			if (inviteId > 0) { // from invite link
				// check invite status is ok
				if (inviteInfo != null) {
					if (inviteInfo.getAccount().equalsIgnoreCase(regEmail)) {
						inviteStatus = REG_INVITE_LINK;
					} else {
						inviteStatus = REG_INVITE_CHANGEMAIL;
					}
				}
			} else { // invited, but not from invite link
				int count = 0;
				try {
					count = InviteDAO.getInstance().getInviteCount(regEmail);
					cost.endStep("2");
				} catch (Exception e) {
					e.printStackTrace(System.err);
				}
				if (count > 0) {
					inviteStatus = REG_INVITE_NOTLINK;
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			cost.endFinally();
		}

		return inviteStatus;
	}

}
