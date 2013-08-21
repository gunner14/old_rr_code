package com.xiaonei.reg.register.logic.additional;

import java.util.List;
import java.util.concurrent.ConcurrentHashMap;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.logic.RegNotifyLogic;
import com.xiaonei.reg.register.dao.InviteDAO;
import com.xiaonei.reg.register.model.InviteInfo;
import com.xiaonei.reg.register.model.RegInviteMap;

/**
 * 邀请重构新方法
 * @author wei.cheng@opi-corp.com
 * 2010.5.18 邀请重构
 */
public class RegInviteFriendBusinessLogic {

	private static RegInviteFriendBusinessLogic _instance = new RegInviteFriendBusinessLogic();

    public static RegInviteFriendBusinessLogic getInstance() {
        return _instance;
    }

    private RegInviteFriendBusinessLogic() {
        super();
    }
    
    /**
     * B通过A的邀请链接进行注册，如果<发现自己被其他人C邀请过>
     *     否，自动和A成为好友，AB收到成为好友的通知
     *	       是，自动和A成为好友，AB收到成为好友的通知,C收到B刚刚注册的通知，B收到C的好友申请
     * B普通注册，如果<发现自己被其他人C邀请过>
     *     否，直接去激活页面
     *     是，C受到B刚注册的通知，B收到C的好友申请
     * @param user	注册用户
     */
    public void doFriendsBusiness(final User user){
    	
    	ConcurrentHashMap<String, Thread> tasks = new ConcurrentHashMap<String, Thread>();
    	final RegInviteMap inviterReg = RegInviterInviteeLogic
        		.getInstance().getRegInviteMapByInvitee(user.getId());
    	if (inviterReg != null) {
            // 加好友
    		final User sender = UserAdapter.getUser(inviterReg.getInviterId());
            tasks.put("[new invite flow]send create friend", new Thread() {

                @Override
                public void run() {
                	//auto make friends
                    RegFriendShipLogic.getInstance().createFriendShip(user, sender);
                }
            });
            tasks.put("[new invite flow]send friend notify", new Thread() {

                @Override
                public void run() {
                    //send make friends notify
                    RegNotifyLogic.getInstance().send2InviteeNotify(user, sender, false); //发给好友
                    RegNotifyLogic.getInstance().send2InviteeNotify(sender, user, true);  //发给自己
                }
            });
        }
		tasks.put("[new invite flow]who invite me notify", new Thread() {

			@Override
			public void run() {
				// for other inviter
				List<InviteInfo> whoInviteMe = null;
				try {
					whoInviteMe = InviteDAO.getInstance().invitedGetInvitoer(
							user.getAccount());
					if (whoInviteMe != null && whoInviteMe.size() > 0) {
						for (InviteInfo inv : whoInviteMe) {
							int send = inv.getOwner_id(); // 发送邀请的人
							if (inviterReg == null || (inviterReg != null && send != inviterReg.getInviterId())) { // 排除掉邀请注册的人
								User cursender = UserAdapter.getUser(send);
								// begin send register notify
								RegNotifyLogic.getInstance()
										.sendToInviterNotify(user, cursender);
								// owner then get the inviter's notify
								RegFriendShipLogic.getInstance().requestBuddy(
										cursender, user);
							}
						}
					}
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
		RunnableTaskExecutor.getInstance().runConcurrentTasks("send create friend", tasks);
    }
    
    
	/**
	 * @param args
	 */
	public static void main(String[] args) {
	
	}

}
