package com.xiaonei.reg.usertrace.model;

import java.io.Serializable;
import java.util.Date;
import org.apache.commons.lang.builder.ToStringStyle;
import org.apache.commons.lang.builder.ToStringBuilder;

/**
 * 在注册页面上点击注册按钮产生的用户行为
 * 2008-8-28
 *
 * @author wei.cheng@opi-corp.com
 */
public class RegVisitOperate implements Serializable {

    private static final long serialVersionUID = -4652480374681114362L;

    //主键
    private int id;
    //actionid
    private int actionid;
    //ip地址
    private String ip;
    //email
    private String email;
    //用户id
    private int userid;
    //时间
    private Date time;
    //阶段状态
    private int stage;
    //cookie中的sessionId
    private String xnSessId;
    //source
    private String s1;
    private String s2;
    private String s3;
    private String s4;
    //访问的域名
    private String domainName;
    //邀请人id
    private int inviterId;

    public String toString() {
		return new ToStringBuilder(this, ToStringStyle.SHORT_PREFIX_STYLE)
				.append("id", id).append("actionid", actionid).append("ip", ip)
				.append("email", email).append("userid", userid).append("time",
						time).append("stage", stage).append("xnSessId",
						xnSessId).append("s1", s1).append("s2", s2).append(
						"s3", s3).append("s4", s4).append("domainName",
						domainName).append("inviterId", inviterId).toString();
	}

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

    public String getIp() {
        return ip;
    }

    public void setIp(String ip) {
        this.ip = ip;
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

    public int getStage() {
        return stage;
    }

    public void setStage(int stage) {
        this.stage = stage;
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
}
