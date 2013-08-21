package com.xiaonei.reg.guide.logic.portrait;

import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserDesc;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;
import com.xiaonei.platform.core.opt.ice.WUserDescAdapter;
import com.xiaonei.platform.core.opt.jdbc.dao.UniversityDAO;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.dao.PendingUserDAO;
import com.xiaonei.reg.guide.model.PendingUser;
import com.xiaonei.reg.guide.pojo.CommonPortraitPojo;
import com.xiaonei.reg.register.logic.additional.RegInviterInviteeLogic;
import com.xiaonei.xce.userstate.UserStateInfo;

import org.apache.struts.upload.MultipartRequestHandler;

import javax.servlet.http.HttpServletRequest;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

public class CommonPortraitLogic {

	public static final String MEM_CACHE_NEW_INVITEE_KEY = "mem_cache_new_invitee_";

	static class SingletonHolder {
		static CommonPortraitLogic instance = new CommonPortraitLogic();
	}

	public static CommonPortraitLogic getInstance() {
		return SingletonHolder.instance;
	}

	public UserDesc getUserDesc(User host) {
		return WUserDescAdapter.getInstance().get(host.getId());
	}

	public boolean maxLengthExceeded(HttpServletRequest request) {
		Boolean maxLengthExceeded = (Boolean) request
				.getAttribute(MultipartRequestHandler.ATTRIBUTE_MAX_LENGTH_EXCEEDED);
		if (maxLengthExceeded == null)
			maxLengthExceeded = false;
		return maxLengthExceeded;
	}

	@SuppressWarnings("unchecked")
	public void setNewInvitee2Mem(int inviteeId) {
		try {
			// 得到邀请者信息
			int inviterId = RegInviterInviteeLogic.getInstance()
					.getInviterFrom(inviteeId);
			if (inviterId <= 0) {
				return;
			}
			// 拼装key
			String key = MEM_CACHE_NEW_INVITEE_KEY + inviterId;
			MemCacheManager mem = MemCacheFactory.getInstance()
					.getManagerByPoolName(MemCacheKeys.pool_user_space);
			// 得到已经存在于memcache中的邀请者信息。
			List<Integer> inviteeIdLst = (List<Integer>) mem.get(key);
			if (inviteeIdLst == null) {
				inviteeIdLst = new ArrayList<Integer>();
			}
			inviteeIdLst.add(Integer.valueOf(inviteeId));
			mem.set(key, inviteeIdLst, (int) (MemCacheKeys.hour * 24 * 7));
		} catch (Exception e) {
			e.printStackTrace();
		}

	}

	/**
	 * 判断图片地址的合法性
	 * 
	 * @param filename
	 *            图片地址
	 * @return 判断结果
	 */
	public boolean isValidImg(String filename) {
		if (!filename.endsWith(".jpg") && !filename.endsWith(".jpeg")
				&& !filename.endsWith(".png") && !filename.endsWith(".bmp")
				&& !filename.endsWith(".gif")) {
			return false;
		}
		return true;
	}

	/**
	 * 更新用户的头像url
	 * 
	 * @param host
	 * @param b
	 *            封装的一些数据
	 */
	public void updateHeadUrl(User host, CommonPortraitPojo b) {
		UserAdapter.updateHeadUrl(host.getId(), b.getHead(), b.getMain(), b
				.getTiny(), b.getLarge());
	}

	/**
	 * 将User对象存入加星系统
	 * 
	 * @param user
	 * @throws SQLException
	 */
	public static void save(User user) throws SQLException {
		save(user, PendingUser.FLAG_DEFAULT);
	}

	public static void save(User user, int type) throws SQLException {
		if (user.getStatus() == User.statusNormal) {
			if (!UserAdapter.get(user).getHeadUrl().equals(
					Globals.userMenHeadUrl)
					&& (!UserAdapter.get(user).getHeadUrl().equals(
							Globals.userWomenHeadUrl))
					&& (!UserAdapter.get(user).getHeadUrl().equals(
							Globals.userHeadUrl))) {
				if (!PendingUserDAO.getInstance().isExists(user.getId())) {
					PendingUser pu = new PendingUser(user.getId());
					pu.setStage(UserAdapter.get(user).getStage());
					PendingUserDAO.getInstance().save(pu, type);
				}
			} else {
				if (UserAdapter.get(user).getSelected() == User.usrSelected) {
					UserStateInfo userStateInfo = new UserStateInfo();
					userStateInfo.setStar(User.usrNotSelected);
					WUserAdapter.getInstance().getStateAdapter()
							.createUserState(user.getId(), userStateInfo);
					UniversityDAO.getInstance().decStarCount(
							UserAdapter.get(user).getUniv());
				}
			}
		}
	}

}
