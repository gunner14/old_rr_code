package com.xiaonei.reg.usertrace.q;

import java.sql.SQLException;
import java.util.HashMap;

import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegVisitLogView;

public class Queue {

	public HashMap<Integer, RegVisitLogView> data;

	private static Queue cq;
	public int maxSize;

	public int size;
	public int front; // 允许删除的一端
	public int rear; // 允许插入的一端

	static {
		if (Globals.DEBUG_ENV) {
			cq = new Queue(1);
		} else {
			cq = new Queue(UTUtils.VISIT_MaxSize);
		}
	}

	private static Queue _instance = null;

	public static Queue getInstance() {
		if (_instance == null) {
			_instance = cq;
			/*synchronized (Queue.class) {
				if (_instance == null)
			}*/
		}
		return _instance;
	}

	public Queue(int maxSize) {
		this.maxSize = maxSize;
		this.data = new HashMap<Integer, RegVisitLogView>(maxSize);
		size = 0;
		rear = 0;
		front = 0;
	}

	public boolean isEmpty() {
		return size == 0;
	}

	public boolean isFull() {
		return size == maxSize;
	}

	/**
	 * 循环队列
	 * 
	 * @param data
	 * @return
	 */
	public synchronized boolean push(RegVisitLogView dt) throws Exception {

		if (isFull()) {
			RegLogger.getLoggerWithType(Queue.class.getSimpleName(),
					UTUtils.authorEmail).debug("Queue::Queue is full!");
			pr(this.data);
			this.data = new HashMap<Integer, RegVisitLogView>(maxSize);
			this.size = 0;
		}
		this.size++;
		this.data.put(rear, dt);
		rear = (rear + 1) % maxSize;
		return true;
	}

	private void pr(HashMap<Integer, RegVisitLogView> a) throws SQLException {
		if (a != null) {
			UserTraceLogic.getInstance().saveBatchVisitHisFirst(a);
		}
	}
}
