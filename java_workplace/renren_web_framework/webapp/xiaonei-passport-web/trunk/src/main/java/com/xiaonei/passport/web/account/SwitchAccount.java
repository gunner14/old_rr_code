package com.xiaonei.passport.web.account;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.stereotype.Service;

import com.xiaonei.passport.constants.MultiAccountStatus;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserState;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.xce.idcache.IdInfoAdapter;

@Service
public class SwitchAccount implements ISwitchAccount{
    private IdInfoAdapter idInfoAdapter = new IdInfoAdapter();
    protected Log logger = LogFactory.getLog(this.getClass());
    
    
    
    @Override
    public MultiAccountStatus getMultiAccountStatus(User user) {
        MultiAccountStatus status = MultiAccountStatus.single();
        UserState userState = user.getUserState();
        //如果不是DoubleAccount,也不是VirtualID,也不是Page管理员，那就不是多账号
        // 添加其他的判断
        if (userState.isDoubleAccount()) {
            status.add(MultiAccountStatus.DOUBLE_ACCOUNT);
        }
        //悲催的判断接口,只要有一个关联账号是pageAdmin，那就返回true，太受打击了
        if (userState.isPageManager()) {
            status.add(MultiAccountStatus.PAGE_ADMIN);
        }
        //        if(idInfoAdapter.isVirtualId(user.getId())){
        //            status.add(MultiAccountStatus.VIRTUAL_ID);
        //        }

        return status;
    }
    
    @Override
    public int getTransUserId(int accountId){
        if (idInfoAdapter.isVirtualId(accountId)) {
            return idInfoAdapter.getRelatedTransId(accountId);
        }else{
            return accountId;
        }
    }
    
    @Override
    public User getReallyUser(int accountId) {
        User realUser = null;
        if (idInfoAdapter.isVirtualId(accountId)) {
            int transId = idInfoAdapter.getRelatedUserId(accountId);
            try {
                realUser = SnsAdapterFactory.getUserAdapter().get(transId);
            } catch (Exception e) {
                this.logger.error(
                        "getReallyUser-SnsAdapterFactory.getUserAdapter().get Error,transId="
                                + transId, e);
            }
        } else {
            try {
                realUser = SnsAdapterFactory.getUserAdapter().get(accountId);
            } catch (Exception e) {
                this.logger.error(
                        "getReallyUser-SnsAdapterFactory.getUserAdapter().get Error,accountId="
                                + accountId, e);
            }
        }
        return realUser;
    }
    
    @Override
    public User getTransUser(int accountId) {
        User transUser = null;
        if (idInfoAdapter.isVirtualId(accountId)) {
            int transId = idInfoAdapter.getRelatedTransId(accountId);
            try {
                transUser = SnsAdapterFactory.getUserAdapter().get(transId);
            } catch (Exception e) {
                this.logger.error(
                        "getTransUser-SnsAdapterFactory.getUserAdapter().get Error transId="
                                + transId, e);
            }
        } else {
            try {
                transUser = SnsAdapterFactory.getUserAdapter().get(accountId);
            } catch (Exception e) {
                this.logger.error(
                        "getTransUser-SnsAdapterFactory.getUserAdapter().get Error accountId="
                                + accountId, e);
            }

        }
        return transUser;
    }
}
