/*
 * Created on 2003-12-16
 */
package com.xiaonei.platform.core.utility.sched;

import java.util.LinkedList;

/**
 * ����������
 * 
 * @author <a href="jing.huang@opi-corp.com">Huang Jing</a>
 */
public class SchedTaskQueue implements Runnable {
	
	protected LinkedList taskList;
	
	public SchedTaskQueue() {
		this.taskList = new LinkedList();
	}
	
	public synchronized boolean addTask(SchedTask t) {
		if ( !this.taskList.contains(t) ) {
			this.taskList.addLast(t);
			return true;
		}
		return false;
	}
	
	protected synchronized SchedTask getTask() {
		SchedTask t = null;

		int count = this.taskList.size();
		for (int i=0; i<count; i++) {
			t = (SchedTask) this.taskList.remove(i);
			
			if ( t != null ) {
				if ( t.timeToExec() ) {
					return t;
				} else {
					this.releaseTask(t);
				}
			}
		}

//		for (int i=0; i<count; i++) {
//			t = (SchedTask) this.taskList.get(i);
//			
//			if ( t != null ) {
//				if ( t.timeToExec() ) {
//					this.taskList.remove(i);
//					return t;
//				}
//			}
//		}

		return null;
	}
	
	protected synchronized boolean releaseTask(SchedTask t) {
		if ( !this.taskList.contains(t) ) {
			this.taskList.addLast(t);
			return true;
		}
		return false;
	}
	
	protected synchronized void removeTask(SchedTask t) {
		this.taskList.remove(t);
	}
	
	public synchronized boolean removeTask(String id) {
		if ( null == id ) return false;
		
		for (int i=0; i<this.taskList.size(); i++) {
			SchedTask t = (SchedTask) this.taskList.get(i);
			if ( id.equals(t.getTaskId()) ) {
				this.taskList.remove(i);
				return true;
			}
		}
		return false;
	}
	
	public synchronized int getActiveCount() {
		return this.taskList.size();
	}
	
	public synchronized SchedTask[] getTasks() {
		SchedTask[] arr = (SchedTask[]) this.taskList.toArray( new SchedTask[]{} );
		return arr;
	}
	
	public synchronized void clear() {
		while (this.taskList.size() > 0) {
			this.taskList.removeLast();
		}
	}
	
	public void run() {
	}

}
