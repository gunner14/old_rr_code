package com.kaixin.register.model;

import java.io.Serializable;

/**
 * 
 * @author wei.cheng@opi-corp.com
 * @date
 */
public class MultiAccountsMap implements Serializable {
    
    private static final long serialVersionUID = 1335250155474617606L;
    
    /**
     * 账号标志 email
     */
    public static final int ACCOUNT_TYPE_EMAIL = 1;
    /**
     * 账号标志 手机号
     */
    public static final int ACCOUNT_TYPE_MOBILE = 2;
    /**
     * 账号标志 xid
     */
    public static final int ACCOUNT_TYPE_XID = 3;
    private String account;
    private int user_id;
    private int account_type;

    public String getAccount() {
        return account;
    }

    public void setAccount(String account) {
        this.account = account;
    }

    public int getUser_id() {
        return user_id;
    }

    public void setUser_id(int user_id) {
        this.user_id = user_id;
    }

    public int getAccount_type() {
        return account_type;
    }

    public void setAccount_type(int account_type) {
        this.account_type = account_type;
    }
}
