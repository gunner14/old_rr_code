/*
 * Created on 2003-12-16
 */
package com.xiaonei.platform.core.utility.sched;

import com.xiaonei.platform.core.utility.MathUtils;


/**
 * ����������̣߳�����߳�����ͬһ�̳߳أ���������������ȡ����
 * ���ȡ���ִ�У�����ȴ�
 * 
 * @author <a href="jing.huang@opi-corp.com">Huang Jing</a>
 */
public class SchedThread extends Thread {
	
	public static final int SCHED_WAIT = 3000;
	
	protected boolean interrupted = false;
	
	protected SchedTaskQueue queue;
	
	protected SchedThreadPool pool;
	
	public SchedThread(SchedTaskQueue q, SchedThreadPool p, String name) {
		super(p, name);
		this.setDaemon(true);

		this.queue = q;
		this.pool = p;
	}
	
	public void interrupt() {
		this.interrupted = true;
	}
	
	public void run() {
		while ( !interrupted ) {
			SchedTask task = null;
			try {
				task = queue.getTask();
			} catch ( Exception e ) {
				e.printStackTrace();
			}
			if ( task != null ) {
				try {
					task.execute();
				} catch ( Exception e ) {
					e.printStackTrace();
				}
			} else {
				try {
					Thread.sleep(MathUtils.randomInt(SCHED_WAIT));
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		}
	}

}
