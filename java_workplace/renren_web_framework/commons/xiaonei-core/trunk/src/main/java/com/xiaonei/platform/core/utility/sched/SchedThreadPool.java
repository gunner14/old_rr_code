/*
 * Created on 2003-12-16
 */
package com.xiaonei.platform.core.utility.sched;

/**
 * ��������̳߳أ�ThreadGroup�ļ�)չ
 * 
 * ���ںܶ�������Ӧ�ö���Ҫʹ���̳߳أ�����)չ��ǿ��Ĺ��?�ܣ���ɸ���������
 * 
 * @author <a href="jing.huang@opi-corp.com">Huang Jing</a>
 */
public class SchedThreadPool extends ThreadGroup {

	protected SchedThreadDeathListener listener;
	
	public SchedThreadPool(String name, SchedThreadDeathListener l) {
		super(name);
		this.listener = l;
	}

	/* (non-Javadoc)
	 * @see java.lang.ThreadGroup#uncaughtException(java.lang.Thread, java.lang.Throwable)
	 */
	public void uncaughtException(Thread t, Throwable e) {
		super.uncaughtException(t, e);
		
		if ( !(e instanceof ThreadDeath) && this.listener != null ) {
			this.listener.notifyThreadEvent(this, t, e);
		}

	}

	/* (non-Javadoc)
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		return super.toString();
	}

	/* (non-Javadoc)
	 * @see java.lang.ThreadGroup#activeCount()
	 */
	public int activeCount() {
		return super.activeCount();
	}

}
