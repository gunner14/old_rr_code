package com.xiaonei.passport.web.account;

import com.xiaonei.passport.constants.MultiAccountStatus;
import com.xiaonei.platform.core.model.User;

public interface ISwitchAccount {

    /**
     * 获取当前账号的确切状态，DoubleAccount、PageManager、VirtualId、single
     * @param user
     * @return
     */
    public MultiAccountStatus getMultiAccountStatus(User user);
    
    
    /**
     * 获取对应的ID
     * @param accountId
     * @return
     */
    public int getTransUserId(int accountId);
    
    
    /**
     * 获取对应的主用户
     * @param accountId
     * @return
     */
    public User getReallyUser(int accountId);
    
    /**
     * 获取对应的Page用户
     * @param accountId
     * @return
     */
    public User getTransUser(int accountId);
}
