package com.xiaonei.login.model;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.xiaonei.xce.passport.Account;

/**
 * LoginModel <br>
 * 
 * @author tai.wang@opi-corp.com Jul 30, 2010 - 3:31:11 PM
 */
public class LoginModel {

    private Account account;

    private String domain;

    private String ip;

    private String pwd;

    private boolean isPer;

    private HttpServletRequest request;

    private HttpServletResponse response;

    private String userName;

    public boolean isPer() {
        return isPer;
    }

    public void setPer(boolean isPer) {
        this.isPer = isPer;
    }

    public HttpServletResponse getResponse() {
        return response;
    }

    public void setResponse(HttpServletResponse response) {
        this.response = response;
    }

    public Account getAccount() {
        return account;
    }

    public String getDomain() {
        return domain;
    }

    /**
     * @return the ip
     */
    public String getIp() {
        return ip;
    }

    public String getPwd() {
        return pwd;
    }

    public HttpServletRequest getRequest() {
        return request;
    }

    public String getUserName() {
        return userName;
    }

    public void setAccount(Account account) {
        this.account = account;
    }

    public void setDomain(String domain) {
        this.domain = domain;
    }

    /**
     * @param ip the ip to set
     */
    public void setIp(String ip) {
        this.ip = ip;

    }

    public void setPwd(String pwd) {
        this.pwd = pwd;
    }

    public void setRequest(HttpServletRequest request) {
        this.request = request;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    @Override
    public String toString() {
        return "LoginModel [account=" + account + ", domain=" + domain + ", ip=" + ip + ", isPer="
                + isPer + ", userName=" + userName + "]";
    }
}
