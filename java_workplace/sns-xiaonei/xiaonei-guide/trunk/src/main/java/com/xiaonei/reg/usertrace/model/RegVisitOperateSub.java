package com.xiaonei.reg.usertrace.model;

import java.io.Serializable;
import java.util.Date;

/**
 * 在注册页面上点击注册按钮产生的用户行为
 * 2008-8-28
 *
 * @author wei.cheng@opi-corp.com
 */
public class RegVisitOperateSub implements Serializable {

    private static final long serialVersionUID = -4652480374681114362L;

    //主键
    private int id;
    //actionid
    private int actionid;
    //email
    private String email;
    //用户id
    private int userid;
    //时间
    private Date time;
    //source
    private String s1;
    private String s2;
    private String s3;
    private String s4;

    public int getActionid() {
        return actionid;
    }

    public void setActionid(int actionid) {
        this.actionid = actionid;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getS1() {
        return s1;
    }

    public void setS1(String s1) {
        this.s1 = s1;
    }

    public String getS2() {
        return s2;
    }

    public void setS2(String s2) {
        this.s2 = s2;
    }

    public String getS3() {
        return s3;
    }

    public void setS3(String s3) {
        this.s3 = s3;
    }

    public String getS4() {
        return s4;
    }

    public void setS4(String s4) {
        this.s4 = s4;
    }

    public Date getTime() {
        return time;
    }

    public void setTime(Date time) {
        this.time = time;
    }

    public int getUserid() {
        return userid;
    }

    public void setUserid(int userid) {
        this.userid = userid;
    }

    /**
     * Constructs a <code>String</code> with all attributes
     * in name = value format.
     *
     * @return a <code>String</code> representation 
     * of this object.
     */
    public String toString()
    {
        final String TAB = "    ";
        
        String retValue = "";
        
        retValue = "RegVisitOperateSub ( "
            + super.toString() + TAB
            + "id = " + this.id + TAB
            + "actionid = " + this.actionid + TAB
            + "email = " + this.email + TAB
            + "userid = " + this.userid + TAB
            + "time = " + this.time + TAB
            + "s1 = " + this.s1 + TAB
            + "s2 = " + this.s2 + TAB
            + "s3 = " + this.s3 + TAB
            + "s4 = " + this.s4 + TAB
            + " )";
    
        return retValue;
    }
    
    
}
