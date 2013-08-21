package com.xiaonei.reg.register.model;

import java.io.Serializable;
import java.util.Date;

/**
 * @author wei.cheng@opi-corp.com
 * @version 1.0
 * @date 2010.03.09 9:43:13 AM
 * 保存直接激活的用户到表
 */
public class RegNoActivateRecord implements Serializable {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = 5049916580577234619L;

    private int id;
    
    private int uid;
    
    private Date reg_time;
    
    private int reg_flag;

    
    public int getId() {
        return id;
    }

    
    public void setId(int id) {
        this.id = id;
    }

    
    public int getUid() {
        return uid;
    }

    
    public void setUid(int uid) {
        this.uid = uid;
    }

    
    public Date getReg_time() {
        return reg_time;
    }

    
    public void setReg_time(Date reg_time) {
        this.reg_time = reg_time;
    }

    
    public int getReg_flag() {
        return reg_flag;
    }

    
    public void setReg_flag(int reg_flag) {
        this.reg_flag = reg_flag;
    }
    
}
