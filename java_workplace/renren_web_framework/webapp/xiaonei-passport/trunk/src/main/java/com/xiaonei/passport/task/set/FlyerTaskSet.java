package com.xiaonei.passport.task.set;

import java.util.HashSet;
import java.util.Set;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.passport.configure.ILoginConfigure;
import com.xiaonei.passport.configure.LoginConfigureFactory;
import com.xiaonei.passport.task.followmission.IFollowMissionTask;
import com.xiaonei.passport.task.followmission.IFollowMissionTaskSet;
import com.xiaonei.passport.task.task.PageSpreadTask;
import com.xiaonei.passport.task.task.RRDogTask;
import com.xiaonei.passport.task.task.RRFarmTask;


/**
 * FlyerTaskSet <br>
 * 发一些奖励相关的把
 * 
 * @author tai.wang@opi-corp.com Mar 10, 2010 - 11:08:47 AM
 */
public class FlyerTaskSet implements IFollowMissionTaskSet {

    protected Log logger = LogFactory.getLog(this.getClass());
    private ILoginConfigure loginConfigure=LoginConfigureFactory.getLoginConfigureInstance();
    private final Set<IFollowMissionTask> set = new HashSet<IFollowMissionTask>();

    public FlyerTaskSet(final int userid, final String lastip, final String domain, int loginRegistrationId) {
    	addTaskSet(new PageSpreadTask(userid, lastip), loginRegistrationId);
    	addTaskSet(new RRFarmTask(userid, domain), loginRegistrationId);
    	addTaskSet(new RRDogTask(domain, userid), loginRegistrationId);
        //        set.add(new FilteAdsByIPTask(userid, lastip)); 2011年3月11日 关闭海外去广告
    }
    /**
     * 判断是否需要加入此任务
     * @param task
     * @param loginRegistrationId
     */
    private void addTaskSet(IFollowMissionTask task, int loginRegistrationId){
    	Integer addTaskFlag=loginConfigure.getUniqLoginConfigureForInt(loginRegistrationId, task.getName());
    	if(addTaskFlag!=null){
    		this.set.add(task);
    	}
    }
    @Override
    public String getName() {
        return "FlyerTaskSet";
    }

    @Override
    public void run() {
        for (final IFollowMissionTask task : this.set) {
            try {
                if (this.logger.isDebugEnabled()) {
                    this.logger.debug(task.getName() + " start, " + task.toString());
                }
                task.run();
            } catch (final Exception e) {
                new Exception(task.getName(), e).printStackTrace();
            }
        }
    }

}
