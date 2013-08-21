package com.xiaonei.login.task.set;

import java.util.HashSet;
import java.util.Set;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.login.constants.LoginLoggerDef;
import com.xiaonei.login.logic.logger.PpLogger;
import com.xiaonei.login.task.followmission.IFollowMissionTask;
import com.xiaonei.login.task.followmission.IFollowMissionTaskSet;
import com.xiaonei.login.task.task.ActiveTrackTask;
import com.xiaonei.login.task.task.FeedTask;
import com.xiaonei.login.task.task.LastIpTask;
import com.xiaonei.login.task.task.LastTimeTask;
import com.xiaonei.login.task.task.UserScoreTask;
import com.xiaonei.login.util.StatUtil;
import com.xiaonei.wservice.userscore.login.LoginType;

public class LogTaskSet implements IFollowMissionTaskSet {

    protected static final PpLogger _Tstat = LoginLoggerDef.logTask;
    
    protected Log logger = LogFactory.getLog(this.getClass());

    private StatUtil su = new StatUtil(_Tstat);

    private Set<IFollowMissionTask> taskSet = new HashSet<IFollowMissionTask>();

    public LogTaskSet(int userid, String ip, int logType, String requestUrl, byte loginType) {
        taskSet.add(new ActiveTrackTask(userid, ip, requestUrl));
        taskSet.add(new LastIpTask(userid, ip, logType));
        taskSet.add(new LastTimeTask(userid));
        taskSet.add(new UserScoreTask(userid, loginType));
        if (logType == 0 || loginType == LoginType.TYPE_WEB) {
            taskSet.add(new FeedTask(userid, ip));
        }
    }

    public LogTaskSet(int userid, String ip, String requestUrl) {
        taskSet.add(new ActiveTrackTask(userid, ip, requestUrl));
        taskSet.add(new LastIpTask(userid, ip));
        taskSet.add(new LastTimeTask(userid));
        taskSet.add(new UserScoreTask(userid));
        taskSet.add(new FeedTask(userid, ip));
    }

    @Override
    public String getName() {
        return "LogTaskSet";
    }

    @Override
    public void run() {
        int i = 0;
        for (IFollowMissionTask task : taskSet) {
            if (logger.isDebugEnabled()) {
                logger.debug(task.getName() + " start, "+task.toString());
            }
            task.run();
            i++;
            su.stepTimeCost(i + ":" + this.getName() + ":" + task.getName());
        }
        su.getWholeTime("LogTaskSuccFinish");
    }

}
