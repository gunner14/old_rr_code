package com.xiaonei.login.task.set;

import java.util.HashSet;
import java.util.Set;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.login.task.followmission.IFollowMissionTask;
import com.xiaonei.login.task.followmission.IFollowMissionTaskSet;
import com.xiaonei.login.task.task.PageSpreadTask;
import com.xiaonei.login.task.task.RRDogTask;
import com.xiaonei.login.task.task.RRFarmTask;

/**
 * FlyerTaskSet <br>
 * 发一些奖励相关的把
 * 
 * @author tai.wang@opi-corp.com Mar 10, 2010 - 11:08:47 AM
 */
public class FlyerTaskSet implements IFollowMissionTaskSet {

    protected Log logger = LogFactory.getLog(this.getClass());

    private final Set<IFollowMissionTask> set = new HashSet<IFollowMissionTask>();

    public FlyerTaskSet(final int userid, final String lastip, final String domain) {
        this.set.add(new PageSpreadTask(userid, lastip));
        // 农场和狗都是开心的产品，已经停止
//        this.set.add(new RRFarmTask(userid, domain));
//        this.set.add(new RRDogTask(domain, userid));
        //        set.add(new FilteAdsByIPTask(userid, lastip)); 2011年3月11日 关闭海外去广告
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
