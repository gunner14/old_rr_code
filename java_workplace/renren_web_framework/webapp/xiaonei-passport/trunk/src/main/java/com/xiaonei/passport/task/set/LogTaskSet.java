package com.xiaonei.passport.task.set;

import java.util.HashSet;
import java.util.Set;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;


import com.xiaonei.passport.configure.ILoginConfigure;
import com.xiaonei.passport.configure.LoginConfigureFactory;
import com.xiaonei.passport.constants.LoginConfigureKey;
import com.xiaonei.passport.logger.LoginLoggerDef;
import com.xiaonei.passport.logger.PpLogger;
import com.xiaonei.passport.task.followmission.IFollowMissionTask;
import com.xiaonei.passport.task.followmission.IFollowMissionTaskSet;
import com.xiaonei.passport.task.task.ActiveTrackTask;
import com.xiaonei.passport.task.task.FeedTask;
import com.xiaonei.passport.task.task.LastIpTask;
import com.xiaonei.passport.task.task.LastTimeTask;
import com.xiaonei.passport.task.task.UserScoreTask;
import com.xiaonei.passport.util.StatUtil;
import com.xiaonei.wservice.userscore.login.LoginType;

public class LogTaskSet implements IFollowMissionTaskSet {

    protected static final PpLogger _Tstat = LoginLoggerDef.logTask;
    
    protected Log logger = LogFactory.getLog(this.getClass());

    private StatUtil su = new StatUtil(_Tstat);

    private Set<IFollowMissionTask> taskSet = new HashSet<IFollowMissionTask>();
    private ILoginConfigure loginConfigure=LoginConfigureFactory.getLoginConfigureInstance();
  

    public LogTaskSet(int userid, String ip, String requestUrl, int loginRegistrationId) {
    	addTaskSet(new ActiveTrackTask(userid, ip, requestUrl), loginRegistrationId);//记录id，ip， url
    	Integer logType=loginConfigure.getUniqLoginConfigureForInt(loginRegistrationId, LoginConfigureKey.TASK_LAST_IP_TASK_LOG_TYPE);
    	if(logType==null){
    		logType=0;
    	}
    	addTaskSet(new LastIpTask(userid, ip, logType), loginRegistrationId);//在数据记录userId，ip
    	addTaskSet(new LastTimeTask(userid), loginRegistrationId);//登陆天数，莱纳西登陆天数
    	Integer loginType=loginConfigure.getUniqLoginConfigureForInt(loginRegistrationId, LoginConfigureKey.TASK_USER_SCORE_TASK_LOGIN_TYPE);
    	if(loginType==null){
    		loginType=Integer.valueOf(LoginType.TYPE_WEB);
    	}
    	
    	addTaskSet(new UserScoreTask(userid, loginType.byteValue()), loginRegistrationId);//增加登陆天数
    	addTaskSet(new FeedTask(userid, ip), loginRegistrationId);//通知广告准备广告位
    }
    /**
     * 判断是否需要加入此任务
     * @param task
     * @param loginRegistrationId
     */
    private void addTaskSet(IFollowMissionTask task, int loginRegistrationId){
    	Integer addTaskFlag=loginConfigure.getUniqLoginConfigureForInt(loginRegistrationId, task.getName());
    	if(addTaskFlag!=null){
    		 taskSet.add(task);
    	}
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
