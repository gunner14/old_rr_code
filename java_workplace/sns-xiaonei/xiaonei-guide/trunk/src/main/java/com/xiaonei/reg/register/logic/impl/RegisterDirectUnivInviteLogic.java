/**
 *
 */
package com.xiaonei.reg.register.logic.impl;

import com.xiaonei.platform.core.model.NetworkStatus;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.Workplace;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;
import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.reg.common.RegUtil;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.register.dao.InviteDAO;
import com.xiaonei.reg.register.logic.additional.*;
import com.xiaonei.reg.register.logic.buildperson.RegAccountLogic;
import com.xiaonei.reg.register.model.InviteInfo;
import com.xiaonei.reg.register.pojo.RegAccountPojo;
import com.xiaonei.reg.register.pojo.RegNetworkUnivPojo;
import com.xiaonei.reg.register.pojo.RegUserConfigPojo;
import com.xiaonei.reg.register.pojo.RegisterPojo;
import com.xiaonei.xce.log.TimeCost;

import java.sql.SQLException;

/**
 * RegisterDirectUnivInviteLogic
 *
 * @author wang-tai
 */
public class RegisterDirectUnivInviteLogic extends RegisterBaseLogic {

    private RegisterDirectUnivInviteLogic() {
        super();
    }

    /*
      * (non-Javadoc)
      *
      * @see
      * com.xiaonei.reg.register.logic.RegisterDirectBaseLogic#buildPerson(com
      * .xiaonei.reg.register.form.RegForm,
      * javax.servlet.http.HttpServletRequest)
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
        account.setNetid(Integer.valueOf(regPojo.getUniversityid()));
        account.setHeadUrl(regPojo.getHeadurl());
        account.setMainUrl(regPojo.getMainurl());
        account.setTinyUrl(regPojo.getTinyurl());
        // 用户基本信息，关于描述的信息，都放到持久层去吧！
        User oceUser = RegAccountLogic.getInstance().create(account);

        return oceUser;
    }

    @Override
    protected int additional(User oceUser, RegisterPojo regPojo) {
        TimeCost cost = TimeCost
                .begin("RegisterDirectUnivInviteLogic.setBuddyGroup");
        int code = 0;
        try {
            try {
                this.userConfig(oceUser, regPojo);
                cost.endStep("1");
            } catch (Exception e) {
                code |= err_user_config;
                e.printStackTrace();
            }
            try {
                this.joinNetwork(oceUser, regPojo);
                cost.endStep("2");
            } catch (NumberFormatException e) {
                code |= err_join_network;
                e.printStackTrace();
            } catch (Exception e) {
                code |= err_join_network;
                e.printStackTrace();
            }

            // group
            try {
                this.setBuddyGroup(oceUser);
                cost.endStep("4");
            } catch (Exception e) {
                System.err.println("Buddy team failure");
                e.printStackTrace();
            }


            cost.endStep("10");
            // 邀请链接失效
            try {
                this.invalidateInviteLink(regPojo);
                cost.endStep("11");
            } catch (SQLException e) {
                code |= err_invalidate_invite_link_sql;
                e.printStackTrace();
            } catch (Exception e) {
                code |= err_invalidate_invite_link;
                e.printStackTrace();
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            cost.endFinally();
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
        TimeCost cost = TimeCost
                .begin("RegisterDirectUnivInviteLogic.setBuddyGroup");
        try {
            com.xiaonei.platform.component.friends.logic.GroupLogic
                    .addDefaultGroup(oceUser.getId(), UserAdapter.get(oceUser.getId()).getStage());
            cost.endStep("1");
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            cost.endFinally();
        }
    }

    /**
     * 加入一个网络，自己玩多没意思
     *
     * @param oceUser
     * @param regForm
     * @throws NumberFormatException
     * @author (wang-tai)tai.wang@opi-corp.com
     * @date 2008-9-23 下午08:15:03
     */
    private void joinNetwork(User oceUser, RegisterPojo regForm)
            throws NumberFormatException {
        RegNetworkUnivPojo univPojo = null;
        univPojo = new RegNetworkUnivPojo(UserAdapter.get(oceUser.getId()).getStage(), oceUser.getId(),
                regForm.getTypeOfCourse(), regForm.getUniversityname(), Integer
                        .valueOf(regForm.getUniversityid()));
        RegNetworkLogic.getInstance().setUnivStageNetwork(univPojo,
                NetworkStatus.CURRENT_NETWORK);
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
     * 发送激活信
     *
     * @param user
     * @throws SQLException
     * @author (wang-tai)tai.wang@opi-corp.com 2008-9-23 下午09:05:14
     */
    private void sendActivateEmail(User user) throws SQLException {
        if (StringFormat.isValidEmailAddr(user.getAccount())
                && !RegNoActivateLogic.getInstance().isNoActivatedSendEmail(user)) {   //增加判断是否是直接激活的判断
            RegEmailLogic.getInstance().sendActivateEmail(user);
        }
    }

    /**
     * 邀请链接作废<br>
     * 只能用一次
     *
     * @throws SQLException
     */
    private void invalidateInviteLink(RegisterPojo form) throws SQLException {
        TimeCost cost = TimeCost
                .begin("RegisterDirectUnivInviteLogic.invalidateInviteLink");
        try {
            InviteInfo info = InviteDAO.getInstance().getInviteInfo(
                    form.getInviterId(), form.getInviteeEmail());
            cost.endStep("1");
            if (info == null)
                return;
            else {
                int inviteId = info.getId();
                InviteDAO.getInstance().updateInviteStatus(inviteId, 2);
                cost.endStep("2");
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			cost.endFinally();
		}
	}

	@Override
	protected int additionalSync(User oceUser, RegisterPojo regPojo) {
		// 加好友&&加分组
        User inviter = null;
        try {
            inviter = WUserAdapter.getInstance()
                    .get(regPojo.getInviterId());
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
                RegAppLogic.getInstance().createAppRelation(
                        regPojo.getAppId(), regPojo.getInviterId(),
                        inviter.getName(), oceUser.getId(),
                        oceUser.getName(), regPojo.getAppToken());
            }
        } catch (Exception e) {
            System.err.print("createAppRelation failure");
            e.printStackTrace();
        }
        // news feed
        try {
            if (null != inviter) {
                RegNewsFeedLogic.getInstance().sendNewsFeed2Inviter(
                        inviter, oceUser);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        try {
            this.sendActivateEmail(oceUser);
        } catch (SQLException e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        }
		return 0;
	}
}
