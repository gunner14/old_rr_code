package com.xiaonei.passport.task.task;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.passport.dao.IpLoginDAO;
import com.xiaonei.passport.task.followmission.IFollowMissionTask;
import com.xiaonei.passport.task.followmission.IFollowMissionTaskBatch;
import com.xiaonei.platform.core.utility.sched.impl.IpLoginObject;

/**
 * TODO LastIpTask <br>
 * 
 * @author tai.wang@opi-corp.com Apr 16, 2010 - 4:36:46 PM
 */
public class LastIpTask implements IFollowMissionTask, IFollowMissionTaskBatch {

	private int userid;
	private String ip;
	private int logType = 0;

	public LastIpTask(int userid, String ip) {
		super();
		this.userid = userid;
		this.ip = ip;
	}

	public LastIpTask(int userid, String ip, int logType) {
		this(userid, ip);
		this.logType = logType;
	}

	@Override
	public void run() {
		IpLoginObject ilo = new IpLoginObject();
		ilo.setUserId(userid);
		ilo.setIp(ip);
		ilo.setType(logType);

		try {
			IpLoginDAO.getInstance().save(ilo);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	@Override
	public String getName() {
		return "LastIp";
	}

	private static class DD {
		public static BlockingQueue<LastIpTask> queue = new LinkedBlockingQueue<LastIpTask>();
		public static volatile int insertS = 100;
		public static volatile boolean force = false;
	}

	@Override
	public void batch(IFollowMissionTask task) {
		if (null == task || !(task instanceof LastIpTask)) {
			return;
		}
		LastIpTask task2 = (LastIpTask) task;
		if (DD.force || DD.queue.size() >= DD.insertS) {
			Collection<LastIpTask> c = new ArrayList<LastIpTask>();
			DD.queue.drainTo(c, DD.insertS);
			String values = "";
			boolean isfirst = true;
			for(LastIpTask t : c){
				if(isfirst){
					isfirst =false;
				}else{
					values = values+",";
				}
				values = String.format("(%s, '%s', %s, now())", t.userid,t.ip,t.logType);
			}
			if(StringUtils.isNotEmpty(values)){
				try {
					IpLoginDAO.getInstance().saveBatch(values);
				} catch (SQLException e) {
					new Exception(values, e).printStackTrace();
				}
			}
		}
		DD.queue.add(task2);
	}

}
