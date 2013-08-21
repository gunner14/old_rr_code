package com.xiaonei.reg.register.logic.additional;

import java.sql.SQLException;
import java.util.Date;
import java.util.concurrent.ConcurrentHashMap;

import org.apache.commons.lang.StringUtils;
import org.perf4j.aop.Profiled;

import com.dodoyo.datum.config.BuildSchedule;
import com.xiaonei.platform.component.application.facade.AppOutSiteInvitationFacade;
import com.xiaonei.platform.component.application.model.AppOutSiteInvitation;
import com.xiaonei.platform.component.exp.model.DailyExp;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;
import com.xiaonei.platform.core.opt.ice.WUserDescAdapter;
import com.xiaonei.platform.core.utility.CharCode;
import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.common.proxy.ProxyMethodLog;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.logic.RegNotifyLogic;
import com.xiaonei.reg.register.dao.RegInviteMapDAO;
import com.xiaonei.reg.register.logic.TimeCost;
import com.xiaonei.reg.register.model.RegInviteMap;
import com.xiaonei.sns.platform.core.opt.ice.IOfferFriendsAdapter;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.xce.buddygroup.BuddyGroupAdapter;
import com.xiaonei.xce.userpassport.UserPassportInfo;
import com.xiaonei.xce.usertime.UserTimeInfo;

public class ActivateLogic {

    private static AppOutSiteInvitationFacade appOutSiteInvitationFacade = AppOutSiteInvitationFacade.getInstance();

    private static RegInviteMapDAO regInviteMapDao = RegInviteMapDAO.getInstance();

    private static RegAppLogic regAppInstance = RegAppLogic.getInstance();

    private static RegFriendShipLogic regFriendShipInstance = RegFriendShipLogic.getInstance();

    private static IOfferFriendsAdapter iOfferFriendsAdapter = SnsAdapterFactory.getOfferFriendsAdapter();

    private static RegInviterInviteeLogic regInviterInvieeInstance = RegInviterInviteeLogic.getInstance();

    private static BuddyGroupAdapter buddyGourpInstance = BuddyGroupAdapter.getInstance();

    @SuppressWarnings("unused")
    private static RegNotifyLogic notifyLogic = RegNotifyLogic.getInstance();

    public ActivateLogic() {
        super();
    }

    private static ActivateLogic _instance = ProxyMethodLog.getLogProxy(ActivateLogic.class);

    public static ActivateLogic getInstance() {
        return _instance;
    }

    /**
     * 判断用户是否激活
     * 
     * @param email
     * @return 激活返回 User
     * @author wang-tai(tai.wang@opi-corp.com) 2009-4-9 - 下午12:33:30
     */
    @Profiled
    public User getActivatedUser(String email) {
        User user = null;
        try {
            user = UserAdapter.getUser(email, Globals.domain.toString());
        } catch (Exception e) {
            e.printStackTrace();
        }
        if (null != user && user.getStatus() == User.statusNormal) {
            return user;
        }
        return null;
    }

    /**
     * 激活用户
     * 
     * @param user
     * @param remoteIp
     */
    public void activateUser(final User user) {
        TimeCost cost = TimeCost.begin();
        // 激活用户
        this.activateMain(user);
        cost.endStep();

        friend(user);
        cost.endStep();

        Runnable task = new Runnable() {

            public void run() {
                if (!StringFormat.isValidEmailAddr(user.getAccount())) {
                    activateAdditional(user, true);
                } else {
                    activateAdditional(user, false);
                }
            }
        };
        RunnableTaskExecutor.getInstance().runTask(task);
        cost.endFinally();
    }

    public void activateUserNoActivatedForward(final User user) {
        TimeCost cost = TimeCost.begin();
        // 激活用户
        this.activateMain(user);
        cost.endStep();

        friend(user);
        cost.endStep();

        Runnable task = new Runnable() {

            public void run() {
                activateAdditional(user, false);
            }
        };
        RunnableTaskExecutor.getInstance().runTask(task);
        cost.endFinally();
    }

    /**
     * 
     * @param user
     * @param isNoActivatedForward 判断用户是否走的是 激活后置 流程
     * @author wei.cheng@opi-corp.com
     * @version 1.0
     * @date 创建时间：Apr 2, 2010 12:53:25 PM
     */
    private void activateAdditional(User user, boolean isSendAdminNotify) {

        TimeCost cost = TimeCost.begin();
        incExp(user);
        cost.endStep();

        // 邀请注册发站内信
        try {
            RegMsgLogic.sendInviteMessage(user);
        } catch (Exception e1) {
            e1.printStackTrace();
        }
        cost.endStep();

        // 头像审核系统
        try {
            PendingUserLogic.save(user);
        } catch (SQLException e1) {
            e1.printStackTrace();
        }
        cost.endStep();

        // 资料完整度
        builSchedule(user);
        cost.endStep();

        // 防止guide页面第一次去不到好友,所以提前预处理一下
        iOfferFriendsAdapter.getBestOffer(user.getId(), 9);
        cost.endStep();

        /* 2010.5.20 notify不需要发送了，在前面都发完了
         * try {
            notifyLogic.sendNotify(user, isSendAdminNotify);
            cost.endStep();
        } catch (Exception e) {
            e.printStackTrace();
        }*/
        cost.endFinally();
    }

    /**
     * 好友逻辑
     * 
     * @param user
     * @throws InterruptedException
     */
    public void friend(final User user) {
        // 得到邀请者的信息，互加好友
        final RegInviteMap inviter = regInviterInvieeInstance
                .getRegInviteMapByInvitee(user.getId());
        if (inviter != null) {
            // 加好友
            ConcurrentHashMap<String, Thread> tasks = new ConcurrentHashMap<String, Thread>();
            tasks.put("create friend ship", new Thread() {

                @Override
                public void run() {
                    regFriendShipInstance.createFriendShip(user.getId(), inviter.getInviterId());
                }
            });
            // 添加好友至邀请分组
            tasks.put("add buddy group", new Thread() {

                @Override
                public void run() {
                    String groupName = getGroupName(user);
                    if (StringUtils.isNotEmpty(groupName)) {
                        buddyGourpInstance.addBuddyToGroup(inviter.getInviterId(),
                                groupName.trim(), user.getId());
                    }
                }
            });

            // 取得appId
            tasks.put("appid", new Thread() {

                @Override
                public void run() {
                    appInviteAudit(user, inviter, getAppId(user, inviter));
                }
            });
            RunnableTaskExecutor.getInstance().runConcurrentTasks("activate friend", tasks);
        }

        // 非核心业务
        /*
         * Runnable taskSendAddFriReq = new Runnable() {

            @Override
            public void run() {
                // 所有邀请过他的人给他发加好友请求，除去邀请他的人(inviterId)
                int inviterId = null != inviter ? inviter.getInviterId() : -1;
                regFriendShipInstance.sendFriendsTo(user.getId(), inviterId);
            }
        };
        RunnableTaskExecutor.getInstance().runTask(taskSendAddFriReq);
         */
    }

    private void appInviteAudit(User user, RegInviteMap inviter, int appId) {
        if (appId == 0) {
            return;
        }
        // 给邀请者在app中加钱或其它和app有关的业务，并非实时，由增值的webservise统一批处理
        regAppInstance.addToAppBizPool(appId, inviter.getInviterId(), user.getId());
    }

    /**
     * 得到用户的邀请分组
     * 
     * @param user
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2009-5-5 - 下午03:31:45
     */
    private String getGroupName(User user) {
        String groupName = null;
        try {
            groupName = regInviteMapDao.getInviterGroupName(user.getId());
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return groupName;
    }

    /**
     * 获得appid
     * 
     * @param user
     * @param inviter
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2009-5-5 - 下午03:29:03
     */
    private int getAppId(User user, RegInviteMap inviter) {
        int appId = inviter.getAppId();
        if (appId == 0) {
            AppOutSiteInvitation aosi = appOutSiteInvitationFacade.get(user.getId());
            if (aosi != null) {
                appId = aosi.getAppId();
            }
        }
        return appId;
    }

    /**
     * 资料完整度
     * 
     * @param user
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-27 - 下午04:22:07
     */
    private void builSchedule(User user) {
        try {
            BuildSchedule build = new BuildSchedule(user);
            build.refreshScheduleForReg();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 增加积分
     * 
     * @param user
     */
    private void incExp(User user) {
        // 增加积分
        int exp = DailyExp.expActv;
        if (CharCode.isQualifiedName(user.getName())) {
            exp += DailyExp.expName;
        }
        if (!UserAdapter.get(user).getHeadUrl().equals(Globals.userMenHeadUrl)
                && !UserAdapter.get(user).getHeadUrl().equals(Globals.userWomenHeadUrl)) {
            exp += DailyExp.expPic;
        }
    }

    /**
     * 激活用户主方法
     * 
     * @param user
     */
    private void activateMain(final User user) {
        final int status = User.statusNormal;

        ConcurrentHashMap<String, Thread> tasksMap = new ConcurrentHashMap<String, Thread>();

        // 更新USERS表STATUS
        tasksMap.put("update passport status", new Thread() {

            @Override
            public void run() {
                try {
                    UserAdapter.get(user).setStatus(status);
                    UserPassportInfo userPsssportInfo = new UserPassportInfo();
                    userPsssportInfo.setId(user.getId());
                    userPsssportInfo.setStatus(status);
                    WUserAdapter.getInstance().getPassportAdapter().setUserPassport(user.getId(),
                            userPsssportInfo);
                } catch (Exception e) {
                    e.printStackTrace(System.err);
                }
            }
        });

        // 更新USERDESC表激活时间
        tasksMap.put("update usertime status", new Thread() {

            @Override
            public void run() {
                try {
                    UserTimeInfo userTimeInfo = new UserTimeInfo();
                    Date dt = new Date();
                    userTimeInfo.setId(user.getId());
                    userTimeInfo.setActivateTime(dt);
                    WUserDescAdapter.getInstance().getTimeAdapter().setUserTime(user.getId(),
                            userTimeInfo);
                } catch (Exception e) {
                    e.printStackTrace(System.err);
                }
            }
        });

        RunnableTaskExecutor.getInstance().runConcurrentTasks("activate user main", tasksMap);
    }

}
