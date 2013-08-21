package com.xiaonei.reg.register.logic.additional;

import java.util.List;
import java.util.concurrent.ConcurrentHashMap;

import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.domain.buddy.BuddyApplyWithInfo;

import com.xiaonei.platform.component.friends.home.FriendsHome;
import com.xiaonei.platform.component.friends.model.FriendApply;
import com.xiaonei.platform.component.friends.model.FriendApplyResult;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.common.proxy.ProxyMethodLog;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.register.dao.InviteDAO;
import com.xiaonei.reg.register.dao.RegInviteMapDAO;
import com.xiaonei.reg.register.model.InviteInfo;
import com.xiaonei.reg.register.model.RegInviteMap;

/**
 * @author wang-tai
 */
public class RegFriendShipLogic {

	private static final RegFriendShipLogic _instance = ProxyMethodLog
			.getLogProxy(RegFriendShipLogic.class);

	public static RegFriendShipLogic getInstance() {
		return _instance;
	}

	/**
	 * 建立好友关系
	 * 
	 * @param applicant
	 * @param accepter
	 * @throws InterruptedException
	 */
	public void createFriendShip(int applicant, int accepter) {
		this.createFriendShip(UserAdapter.getUser(applicant), UserAdapter
				.getUser(accepter));
	}

	/**
	 * 建立好友关系
	 * 
	 * @param applicant
	 * @param accepter
	 */
    public void createFriendShip(User applicant, User accepter) {
		if (null == applicant || null == accepter) {
			System.err.println("User obj was null");
			return;
		}
		// this.requestBuddy(applicant.getId(), accepter.getId());
		// this.acceptBuddy(applicant.getId(), accepter.getId());
		// 自动互加好友(使用20080821白兔提供的新接口)
		RegLogger.getLoggerWithType(this.getClass()).info(applicant.getId() + " " + accepter.getId());
		try {
			//预先获取下关系防止获取不到的情况发生
			AdapterFactory.getBuddyCoreAdapter().getRelation(applicant.getId(), accepter.getId());
			Thread.sleep(2 * 1000L);
			AdapterFactory.getBuddyCoreAdapter().addFriendDirectly(applicant.getId(),accepter.getId());
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * 邀请人对此用户发加好友请求
	 * 
	 * @param uId
	 * @param exceptId
	 *            例外者id
	 */
	public void sendFriendsTo(int uId, int exceptId) {
		this.sendFriendsTo(UserAdapter.getUser(uId), exceptId);
	}

	/**
	 * 邀请人对此用户发加好友请求
	 * 
	 * @param host
	 * @param exceptId
	 *            例外者id
	 */
	public void sendFriendsTo(User host, int exceptId) {
		if (null == host) {
			System.err.println("User obj was null");
			return;
		}
		int buddyApplyCount = 0;
		List<InviteInfo> ls = null;
		InviteInfo ii = null;
		String email = host.getAccount();
		try {
			ls = InviteDAO.getInstance().getInviteUsers(email);
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
		if (ls != null && ls.size() > 0) {
			buddyApplyCount = ls.size();
			for (int i = 0; i < buddyApplyCount; i++) {
				ii = ls.get(i);
				if (ii != null) {
					// 发好友请求
					User friend = UserAdapter.getUser(ii.getOwner_id());
					if (friend != null && exceptId != friend.getId()) {
						this.requestBuddy(friend.getId(), host.getId());
					}
				}
			}
		}
	}

	/**
	 * 接受好友请求
	 * 
	 * @param applicantId
	 *            申请人
	 * @param accepterId
	 *            接受人
	 */
	public void acceptBuddy(int applicantId, int accepterId) {
		try {
			BuddyApplyWithInfo request = new BuddyApplyWithInfo(applicantId,
					accepterId);
			AdapterFactory.getBuddyCoreAdapter().acceptApply(request);
		} catch (Exception e) {
		}
	}

	/**
	 * 邀请者，被邀请者存入表中
	 * 
	 * @param inviterId
	 * @param inviteeId
	 * @param inviterGroupName
	 * @param appId
	 */
	public void createRelation(int inviterId, int inviteeId,
			String inviterGroupName, int appId) {
		this.invite.createRelation(inviterId, inviteeId, inviterGroupName,
				appId);
	}

	/**
	 * 得到被某人邀请过玩某个app的所有好友
	 * 
	 * @param inviterId
	 * @param appId
	 * @return
	 */
	public List<?> getFriendIdList(int inviterId, int appId) {
		return this.invite.getFriendIdList(inviterId, appId);
	}

	/**
	 * 得到谁邀请我的
	 * 
	 * @param inviteeId
	 * @return
	 */
	public int getInviterId(int inviteeId) {
		return this.invite.getInviterId(inviteeId);
	}

	/**
	 * 得到某人邀请过玩某个app的人数
	 * 
	 * @param ownerId
	 * @param appId
	 * @return
	 */
	public int getUserInviteCountByAppId(int ownerId, int appId) {
		return this.invite.getUserInviteCountByAppId(ownerId, appId);
	}

	/**
	 * 得到某人邀请过玩某个app的所有人的id
	 * 
	 * @param inviterId
	 * @param appId
	 * @return
	 */
	public List<RegInviteMap> getUserInviteByAppId(int inviterId, int appId) {
		List<RegInviteMap> rtLst = null;
		try {
			rtLst = RegInviteMapDAO.getInstance().getByInviterId(inviterId,
					appId);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return rtLst;
	}

	/**
	 * 添加app信息
	 * 
	 * @param inviterId
	 * @param inviteeId
	 * @param appId
	 */
	public void updateAppId(int inviterId, int inviteeId, int appId) {
		this.invite.updateAppId(inviterId, inviteeId, appId);
	}

	/**
	 * 加好友请求
	 * 
	 * @param applicantId
	 *            申请人
	 * @param accepterId
	 *            接受人
	 */
	private void requestBuddy(int applicantId, int accepterId) {
		try {
			User applicant = UserAdapter.getUser(applicantId);
			final String currentDomain = OpiConstants.domain.toString();			
            FriendApply fa=new FriendApply(applicant, UserAdapter.getUser(accepterId),currentDomain);
            fa.setContent("");
            try {
            	AdapterFactory.getBuddyCoreAdapter().getRelation(accepterId, applicantId);
                FriendApplyResult friendApplyResult = FriendsHome.getInstance().requestFriendApply(fa);
                RegLogger.getLoggerWithType(this.getClass()).debug("result:" + friendApplyResult.getResult() + "|" + friendApplyResult.getMessage());
            } catch (Exception e) {
                e.printStackTrace();
            }              
			RegLogger.getLoggerWithType(this.getClass()).debug(applicantId + " " + accepterId + " ,domain=" + currentDomain);
		} catch (Throwable e) {
		}
	}

	/**
	 * 加好友来了
	 * @param applicant
	 * @param accepter
	 */
	public void requestBuddy(User applicant, User accepter) {
		try {
			final String currentDomain = OpiConstants.domain.toString();			
            FriendApply fa=new FriendApply(applicant, accepter, currentDomain);
            fa.setContent("");
            try {
            	AdapterFactory.getBuddyCoreAdapter().getRelation(accepter.getId(), applicant.getId());
            	RegLogger.getLoggerWithType(this.getClass()).debug(fa== null ? "fa null" : "fa is not null");
                FriendApplyResult friendApplyResult = FriendsHome.getInstance().requestFriendApply(fa);
                RegLogger.getLoggerWithType(this.getClass()).debug("result:" + friendApplyResult.getResult() + "|" + friendApplyResult.getMessage());
            } catch (Exception e) {
                e.printStackTrace();
            }              
			RegLogger.getLoggerWithType(this.getClass()).debug(applicant.getId() + " " + accepter.getId() + " ,domain=" + currentDomain);
		} catch (Throwable e) {
		}
	}
	
	/**
	 * 发送加好友的申请
	 * @param user
	 * @author wei.cheng@opi-corp.com
	 * @version 1.0
	 * @date 创建时间：Mar 31, 2010 10:47:49 AM
	 */
	@SuppressWarnings("unused")
	private void friend(final User user) {
        // 得到邀请者的信息，互加好友
        final RegInviteMap inviter = RegInviterInviteeLogic
                .getInstance().getRegInviteMapByInvitee(user.getId());
        if (inviter != null) {
            // 加好友
            ConcurrentHashMap<String, Thread> tasks = new ConcurrentHashMap<String, Thread>();
            tasks.put("send create friend", new Thread() {

                @Override
                public void run() {
                    requestBuddy(user.getId(),inviter.getInviterId());
                }
            });
            RunnableTaskExecutor.getInstance().runConcurrentTasks("send create friend", tasks);
        }
    }
	
	private RegInviterInviteeLogic invite = RegInviterInviteeLogic
			.getInstance();
}
