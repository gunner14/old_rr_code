package com.xiaonei.login.task.task;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.login.constants.LoginLoggerDef;
import com.xiaonei.login.logic.LoginLogicFactory;
import com.xiaonei.login.logic.logger.PpLogger;
import com.xiaonei.login.task.followmission.IFollowMissionTask;
import com.xiaonei.platform.core.model.UserState;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

/**
 * FilteAdsByIPTask <br>
 * 海外ip去广告
 * 
 * @author tai.wang@opi-corp.com Aug 23, 2010 - 8:31:42 PM
 */
public class FilteAdsByIPTask implements IFollowMissionTask {

    protected PpLogger remoteLogger = LoginLoggerDef.login;

    protected Log logger = LogFactory.getLog(this.getClass());

    private int userId;

    private String ip;

    public FilteAdsByIPTask(int userId, String ip) {
        this.userId = userId;
        this.ip = ip;
    }

    @Override
    public String getName() {
        return "FilteAdsByIPTask";
    }

    @Override
    public void run() {
        if (logger.isDebugEnabled()) {
            logger.debug("userId:" + userId + ", ip:" + ip);
        }
        try {
            UserState beforeUserState = SnsAdapterFactory.getUserStateAdapter()
                    .getUserState(userId);
            if (logger.isDebugEnabled()) {
                logger.debug("beforeUserState.isHideAds:" + beforeUserState.isHideAds());
            }
            boolean baboradIP = LoginLogicFactory.getBabordIPsManager().isBaboradIP(ip);
            if (logger.isDebugEnabled()) {
                logger.debug("baboradIP:" + baboradIP);
            }
            if (baboradIP) {
                if (!beforeUserState.isHideAds()) {
                    SnsAdapterFactory.getUserStateAdapter().setHideAdsOn(userId);
                }
            } else {
                if (beforeUserState.isHideAds() && ! beforeUserState.isVipMember()) {
                    SnsAdapterFactory.getUserStateAdapter().setHideAdsOff(userId);
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            logger.error("", e);

        }
    }
}
