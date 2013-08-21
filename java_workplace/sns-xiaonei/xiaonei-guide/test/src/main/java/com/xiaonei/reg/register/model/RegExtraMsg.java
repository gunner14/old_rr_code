package com.xiaonei.reg.register.model;

import java.io.Serializable;
import java.util.Date;


public class RegExtraMsg implements Serializable {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = -6105859041149759591L;

    private int id;
    
    private int uid;

    private String input_key;

    private String input_value;

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

    
    public String getInput_key() {
        return input_key;
    }

    
    public void setInput_key(String input_key) {
        this.input_key = input_key;
    }

    
    public String getInput_value() {
        return input_value;
    }

    
    public void setInput_value(String input_value) {
        this.input_value = input_value;
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



	@Override
	public String toString() {
		StringBuilder builder = new StringBuilder();
		builder.append("RegExtraMsg [id=");
		builder.append(id);
		builder.append(", input_key=");
		builder.append(input_key);
		builder.append(", input_value=");
		builder.append(input_value);
		builder.append(", reg_flag=");
		builder.append(reg_flag);
		builder.append(", reg_time=");
		builder.append(reg_time);
		builder.append(", uid=");
		builder.append(uid);
		builder.append("]");
		return builder.toString();
	}
    
    
}
