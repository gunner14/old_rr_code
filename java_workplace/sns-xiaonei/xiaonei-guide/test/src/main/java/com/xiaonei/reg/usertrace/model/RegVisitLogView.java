package com.xiaonei.reg.usertrace.model;

import java.io.Serializable;
import java.util.Date;

import org.apache.commons.lang.builder.ReflectionToStringBuilder;

/**
 * 用户第一次访问页面，日志记录
 * 2008-8-28
 *
 * @author wei.cheng@opi-corp.com
 */
public class RegVisitLogView implements Serializable {

    private static final long serialVersionUID = -5748212577653742536L;
    //主键
    private int id;
    //cookie中的sessionId
    private String xnSessId;
    //IP
    private String ip;
    //referer
    private String referer;
    //时间
    private Date time;
    //action id
    private int actionid;
    //source
    private String s1;
    private String s2;
    private String s3;
    private String s4;
    //访问的域名
    private String domainName;
    //邀请人id
    private int inviterId;

    public int getActionid() {
        return actionid;
    }

    public void setActionid(int actionid) {
        this.actionid = actionid;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getIp() {
        return ip;
    }

    public void setIp(String ip) {
        this.ip = ip;
    }

    public String getReferer() {
        return referer;
    }

    public void setReferer(String referer) {
        this.referer = referer;
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

    public String getXnSessId() {
        return xnSessId;
    }

    public void setXnSessId(String xnSessId) {
        this.xnSessId = xnSessId;
    }

    public String getDomainName() {
        return domainName;
    }

    public void setDomainName(String domainName) {
        this.domainName = domainName;
    }

    public int getInviterId() {
        return inviterId;
    }

    public void setInviterId(int inviterId) {
        this.inviterId = inviterId;
    }

    public String toString() {
    	return ReflectionToStringBuilder.toString(this);
    }
}
