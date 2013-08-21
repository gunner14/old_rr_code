/**
 *
 */
package com.xiaonei.reg.register.model;

import com.xiaonei.reg.register.constants.IMultiAccountMap;

import java.io.Serializable;

/**
 * MultiAccountsMap
 *
 * @author wang-tai(tai.wang@opi-corp.com) 2009-3-30 - 上午11:44:25
 */
public class MultiAccountsMap implements Serializable, IMultiAccountMap {
    private static final long serialVersionUID = 1335250155474617606L;

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
