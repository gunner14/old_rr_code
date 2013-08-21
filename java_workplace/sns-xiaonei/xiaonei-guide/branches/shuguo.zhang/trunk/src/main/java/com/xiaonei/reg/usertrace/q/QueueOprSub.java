package com.xiaonei.reg.usertrace.q;

import java.sql.SQLException;
import java.util.HashMap;

import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegVisitOperateSub;

public class QueueOprSub {

    public HashMap<Integer, RegVisitOperateSub> data;
    
    private static QueueOprSub cq;
    public int maxSize;

    public int size;
    public int front; //允许删除的一端
    public int rear;  //允许插入的一端
    
    static {
    	if (Globals.DEBUG_ENV) {
    		cq = new QueueOprSub(1);
		} else {
			cq = new QueueOprSub(UTUtils.OPRSub_MaxSize);
		}
    }
    
    private static QueueOprSub _instance = null;

    public static QueueOprSub getInstance() {
        if (_instance == null) {
        	_instance = cq;
            /*synchronized (QueueOprSub.class) {
                if (_instance == null)
            }*/
        }
        return _instance;
    }
    
    public QueueOprSub(int maxSize) {
        this.maxSize = maxSize;
        this.data = new HashMap<Integer, RegVisitOperateSub>(maxSize);
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
    public synchronized boolean push(RegVisitOperateSub dt, String domain) throws Exception {

        if (isFull()){
            RegLogger.getLoggerWithType(QueueOprSub.class.getSimpleName(), UTUtils.authorEmail).debug("QueueOprSub::Queue is full!");
            pr(this.data, domain);
            this.data = new HashMap<Integer, RegVisitOperateSub>(maxSize);
            this.size = 0;
        }
        this.size ++;
        this.data.put(rear, dt);
        rear = (rear + 1) % maxSize;
        return true;
    }
    
    private void pr(HashMap<Integer, RegVisitOperateSub> a, String domain) throws SQLException{
        if(a!=null){
            UserTraceLogic.getInstance().saveBatchRegActSub(a, domain);
        }
    }
}

