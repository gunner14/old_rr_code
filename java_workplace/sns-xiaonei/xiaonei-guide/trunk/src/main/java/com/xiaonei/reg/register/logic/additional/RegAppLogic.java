package com.xiaonei.reg.register.logic.additional;

import java.sql.SQLException;

import com.xiaonei.platform.component.application.home.AppOutSiteInvitationHome;
import com.xiaonei.platform.component.application.model.AppOutSiteInvitation;
import com.xiaonei.reg.register.dao.RegAppInviteRelationDAO;
import com.xiaonei.reg.register.dao.RegProcessAppdatasDAO;
import com.xiaonei.reg.register.model.RegAppInviteRelation;
import com.xiaonei.reg.register.model.RegProcessAppdatas;

/**
 * 注册时，负责app关系的处理
 * 
 * @author wang-tai
 */
public class RegAppLogic {
	private static RegAppLogic _instance = new RegAppLogic();

	public static RegAppLogic getInstance() {
		return _instance;
	}

	private RegAppLogic() {
		super();
	}

	/**
	 * app邀请信<br>
	 * 建立 邀请人，新用户和app建立关系
	 * 
	 * @param appid
	 *            app id
	 * @param fromId
	 *            邀请人id
	 * @param fromName
	 *            邀请人姓名
	 * @param userId
	 *            接受人id
	 * @param userName
	 *            接受人姓名
	 * @param appToken
	 *            app token
	 */
	public void createAppRelation(int appid, int fromId, String fromName,
			int userId, String userName, String appToken) {
		if ("".equals(appToken) || null == appToken) {
			appToken = AppOutSiteInvitationHome.getInstance()
					.insertWhenCopyLinkReg(fromId, appid, userId);
		}
		/* 当appid apptoken 参数都不存在时，认为不是app邀请链接不做操作 */
		if (0 == appid && (null == appToken || "".equals(appToken))) {
			return;
		}
		// from target;me owner;
		try {
			if (appid != 0) {
				// 更新app邀请数
				updateAppInvite(fromId, appid, userId, appToken);
				// 更新reg_invite_map表中的appid
				RegFriendShipLogic.getInstance().updateAppId(fromId, userId,
						appid);
				// 邀请统一的接口里面用到的表
				this.createAppInviteRelation(fromId, appid, userId);
			}
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
	}

	/**
	 * 邀请统一的接口里面用到的表
	 * @param inviterId
	 * @param appId
	 * @param userId
	 * @author wei.cheng@opi-corp.com
	 * @version 1.0
	 * @date 创建时间：Mar 22, 2010 1:50:16 PM
	 */
	private void createAppInviteRelation(int inviterId, int appId, int userId){
	    RegAppInviteRelation ri = new RegAppInviteRelation();
	    ri.setInviterId(inviterId);
	    ri.setInviteeId(userId);
	    ri.setAppId(appId);
	    try {
            RegAppInviteRelationDAO.getInstance().save(ri);
        } catch (SQLException e) {
            e.printStackTrace();
        }
	}
	
	/**
	 * 记录app邀请的成功数量，在注册成功后调用. 首先查找user id 和apptoken 是否存在，不存在更新
	 * 
	 * @param userId
	 *            user id
	 * @param apptoken
	 *            uuid
	 * @author wang-tai
	 */
	private void updateAppInvite(int inviterId, int appId, int userId,
			String apptoken) {
		//
		if (null == apptoken) {
			AppOutSiteInvitationHome.getInstance().insert(inviterId, appId);
		} else {
			AppOutSiteInvitation aosi = AppOutSiteInvitationHome.getInstance()
					.get(appId, apptoken);
			if (aosi != null) {
				AppOutSiteInvitationHome.getInstance().updateInviteeUserId(
						apptoken, userId);
			}
		}
	}

	/**
	 * 添加待处理的app的信息，例如：注册进来给邀请者加钱，具体实现是增值组做
	 * 
	 * @param appId
	 * @param inviterId
	 * @param inviteeId
	 */
	public void addToAppBizPool(int appId, int inviterId, int inviteeId) {
		try {
			RegProcessAppdatas regProcessAppdatas = new RegProcessAppdatas();
			regProcessAppdatas.setInviterId(inviterId);
			regProcessAppdatas.setInviteeId(inviteeId);
			regProcessAppdatas.setAppId(appId);
			// 插入
			RegProcessAppdatasDAO.getInstance().save(regProcessAppdatas);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
