package com.xiaonei.reg.usertrace.q;

import java.sql.SQLException;
import java.util.HashMap;

import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegVisitCookieLogView;

public class QueueCookie {

    public HashMap<Integer, RegVisitCookieLogView> data;
    
    private static QueueCookie qc;
    public int maxSize;

    public int size;
    public int front; //允许删除的一端
    public int rear;  //允许插入的一端
    
    static {
		if (Globals.DEBUG_ENV) {
			qc = new QueueCookie(1);        
		} else {
			qc = new QueueCookie(UTUtils.COOKIE_MaxSize);        
		}
    }
    
    private static QueueCookie _instance = null;

    public static QueueCookie getInstance() {
        if (_instance == null) {
        	_instance = qc;
            /*synchronized (QueueCookie.class) {
                if (_instance == null)
            }*/
        }
        return _instance;
    }
    
    public QueueCookie(int maxSize) {
        this.maxSize = maxSize;
        this.data = new HashMap<Integer, RegVisitCookieLogView>(maxSize);
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
     * @param data 
     * @return 
     */
    public synchronized boolean push(RegVisitCookieLogView dt) throws Exception {

        if (isFull()){
            RegLogger.getLoggerWithType(QueueCookie.class.getSimpleName(), UTUtils.authorEmail).debug("QueueCookie::Queue is full!");
            pr(this.data);
            this.data = new HashMap<Integer, RegVisitCookieLogView>(maxSize);
            this.size = 0;
        }
        this.size ++;
        this.data.put(rear, dt);
        rear = (rear + 1) % maxSize;
        return true;
    }

    private void pr(HashMap<Integer, RegVisitCookieLogView> a) throws SQLException{
        if(a!=null){
            UserTraceLogic.getInstance().saveBatchRegActCookie(a);
        }
    }
}

