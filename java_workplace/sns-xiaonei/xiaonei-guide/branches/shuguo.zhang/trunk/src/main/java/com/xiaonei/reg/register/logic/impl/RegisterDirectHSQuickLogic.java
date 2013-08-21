/**
 *
 */
package com.xiaonei.reg.register.logic.impl;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.register.logic.additional.InitializtionCountLogic;
import com.xiaonei.reg.register.logic.additional.RegEmailLogic;
import com.xiaonei.reg.register.logic.additional.RegNoActivateLogic;
import com.xiaonei.reg.register.logic.additional.RegUserConfigLogic;
import com.xiaonei.reg.register.logic.buildperson.RegAccountLogic;
import com.xiaonei.reg.register.pojo.RegAccountPojo;
import com.xiaonei.reg.register.pojo.RegUserConfigPojo;
import com.xiaonei.reg.register.pojo.RegisterPojo;

import java.sql.SQLException;

/**
 * 负责高中用户的quick直接注册
 *
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-9-24 下午09:46:04
 */
public class RegisterDirectHSQuickLogic extends RegisterBaseLogic {
    protected RegisterDirectHSQuickLogic() {
        super();
    }

    @Override
    protected int additional(User user, RegisterPojo regPojo) {
        int returnCode = err_noerr;
        // user config
        try {
            this.setUserConfig(user, regPojo);
        } catch (Exception e) {
            returnCode |= err_user_config;
            e.printStackTrace();
        }

        // init
        try {
            this.initInfo(user);
        } catch (Exception e) {
            returnCode |= err_initialize_info;
            e.printStackTrace();
        }
        // 好友分组
        try {
            this.setBuddyGroup(user);
        } catch (Exception e) {
            System.err.println("Buddy team failure");
            e.printStackTrace();
        }


        return returnCode;
    }

    @Override
    protected User buildPerson(RegisterPojo regPojo) throws Throwable {
        RegAccountPojo account = new RegAccountPojo();

        account.setPwd(regPojo.getPwd());
        account.setStage(Integer.valueOf(regPojo.getStage()));
        account.setAccount(regPojo.getRegEmail());
        account.setIp(regPojo.getIp());
        account.setName(regPojo.getName());
        account.setGender(regPojo.getGender());
        account.setHeadUrl(regPojo.getHeadurl());
        account.setTinyUrl(regPojo.getTinyurl());
        account.setMainUrl(regPojo.getMainurl());
        account.setHighSchoolCode(regPojo.getHighSchoolCode());
        account.setHighSchoolName(regPojo.getHighSchoolName());
        // 添加网络
        int hsNetWorkId = 0;
        account.setNetid(hsNetWorkId);
        // 用户基本信息，关于描述的信息，都放到持久层去吧！
        User oceUser = RegAccountLogic.getInstance().create(account);

        return oceUser;
    }

    /**
     * 建立默认好友分组
     *
     * @param oceUser
     */
    private void setBuddyGroup(User oceUser) {
        com.xiaonei.platform.component.friends.logic.GroupLogic
                .addDefaultGroup(oceUser.getId(), UserAdapter.get(oceUser.getId()).getStage());
    }

    /**
     * 设置user config
     *
     * @param user
     * @param regPojo
     * @author (wang-tai)tai.wang@opi-corp.com 2008-9-28 上午11:50:13
     */
    private void setUserConfig(User user, RegisterPojo regPojo) {
        RegUserConfigPojo uc = new RegUserConfigPojo(user);
        RegUserConfigLogic.getInstance().configUserInfo(uc);
    }

    /**
     * 初始化用户信息
     *
     * @param user
     * @author (wang-tai)tai.wang@opi-corp.com 2008-9-28 下午04:17:13
     */
    private void initInfo(User user) {
        InitializtionCountLogic.getInstance().init(user);
    }

    /**
     * 发送激活信
     *
     * @param user
     * @throws SQLException
     * @author (wang-tai)tai.wang@opi-corp.com 2008-9-28 下午04:18:23
     */
    private void sendActivateEmail(User user) throws SQLException {
        if (StringFormat.isValidEmailAddr(user.getAccount())
                && !RegNoActivateLogic.getInstance().isNoActivatedSendEmail(user)) {   //增加判断是否是直接激活的判断
            RegEmailLogic.getInstance().sendActivateEmail(user);
		}
	}

	@Override
	protected int additionalSync(User oceUser, RegisterPojo regPojo) {
        // send active mail
        try {

            this.sendActivateEmail(oceUser);
        } catch (SQLException e) {
            e.printStackTrace();
        }
		return 0;
	}

}
