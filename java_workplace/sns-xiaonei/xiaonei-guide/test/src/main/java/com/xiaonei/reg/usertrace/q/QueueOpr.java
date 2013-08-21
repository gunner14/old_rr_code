package com.xiaonei.reg.usertrace.q;

import java.sql.SQLException;
import java.util.HashMap;

import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegVisitOperate;

public class QueueOpr {

    public HashMap<Integer, RegVisitOperate> data;
    
    private static QueueOpr cq;
    public int maxSize;

    public int size;
    public int front; //允许删除的一端
    public int rear;  //允许插入的一端
    
    static {
    	if (Globals.DEBUG_ENV) {
    		cq = new QueueOpr(1);        
		} else {
			cq = new QueueOpr(UTUtils.OPR_MaxSize);        
		}
    }
    
    private static QueueOpr _instance = null;

    public static QueueOpr getInstance() {
        if (_instance == null) {
        	_instance = cq;
            /*synchronized (QueueOpr.class) {
                if (_instance == null)
            }*/
        }
        return _instance;
    }
    
    public QueueOpr(int maxSize) {
        this.maxSize = maxSize;
        this.data = new HashMap<Integer, RegVisitOperate>(maxSize);
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
    public synchronized boolean push(RegVisitOperate dt) throws Exception {

        if (isFull()){
            RegLogger.getLoggerWithType(QueueOpr.class.getSimpleName(), UTUtils.authorEmail).debug("QueueOpr::Queue is full!");
            pr(this.data);
            this.data = new HashMap<Integer, RegVisitOperate>(maxSize);
            this.size = 0;
        }
        this.size ++;
        this.data.put(rear, dt);
        rear = (rear + 1) % maxSize;
        return true;
    }
    
    private void pr(HashMap<Integer, RegVisitOperate> a) throws SQLException{
        if(a!=null){
            UserTraceLogic.getInstance().saveBatchRegHis(a);
        }
    }
}

