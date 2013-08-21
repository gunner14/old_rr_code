package com.renren.sns.wiki.service;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.biz.SuperAdminBiz;
import com.renren.sns.wiki.delegator.xce.WUserCacheDelegator;
import com.renren.sns.wiki.model.admin.SuperAdmin;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;

/**
 * 
 * @author weiwei.wang@renren-inc.com since 2012-7-4
 * 
 */
@Service
public class SuperAdminService {

    @Autowired
    private SuperAdminBiz superAdminBiz;

    @Autowired
    private WUserCacheDelegator wUserCacheDelegator;

    /**
     * 是否是超管
     * 
     * @param userId
     * @return
     */
    public boolean isSuperAdmin(int userId) {
        return superAdminBiz.isSuperAdmin(userId);
    }

    /**
     * 获取超管的人人Id列表
     * 
     * @return
     */
    public List<Integer> getSuperAdminIdList() {
        return superAdminBiz.getSuperAdminIdList();
    }

    /**
     * 获取超管列表
     * 
     * @return
     */
    public List<WUserCache> getSuperAdminList() {
        List<Integer> userIdList = superAdminBiz.getSuperAdminIdList();
        List<WUserCache> wUserCacheList = wUserCacheDelegator.getCacheUsers(userIdList);
        return wUserCacheList;
    }

    /**
     * 添加超管
     */
    public boolean addSuperAdmin(int userId) {
        User user = WUserAdapter.getInstance().get(userId);
        if (user != null) {
            SuperAdmin superAdmin = new SuperAdmin();

            superAdmin.setUserId(userId);
            superAdmin.setName(user.getName());

            superAdminBiz.addSuperAdmin(superAdmin);
            return true;
        } else {
            return false;
        }
    }

    public boolean removeSuperAdmin(int userId) {
        if (superAdminBiz.isSuperAdmin(userId)) {
            superAdminBiz.removeSuperAdmin(userId);
            return false;
        } else {
            return false;
        }
    }

}
