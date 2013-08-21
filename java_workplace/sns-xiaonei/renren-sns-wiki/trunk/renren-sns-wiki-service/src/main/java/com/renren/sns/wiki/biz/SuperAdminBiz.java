package com.renren.sns.wiki.biz;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.dao.adminwiki.SuperAdminDAO;
import com.renren.sns.wiki.model.admin.SuperAdmin;

/**
 * 
 * @author weiwei.wang@renren-inc.com since 2012-7-4
 * 
 */
@Service
public class SuperAdminBiz extends BaseBiz {

    @Autowired
    private SuperAdminDAO superAdminDAO;

    /**
     * 是否是超管
     * 
     * @param userId
     * @return
     */
    public boolean isSuperAdmin(int userId) {
        Integer resultUserId = superAdminDAO.getSuperAdminUserIdByUserId(userId);
        return resultUserId != null;
    }

    /**
     * 获取超管的人人Id列表
     * 
     * @return
     */
    public List<Integer> getSuperAdminIdList() {
        return superAdminDAO.getSuperAdminIdList();
    }

    /**
     * 添加超管
     */
    public int addSuperAdmin(SuperAdmin superAdmin) {
        return superAdminDAO.addSuperAdmin(superAdmin);
    }

    public int removeSuperAdmin(int userId) {
        return superAdminDAO.removeSuperAdmin(userId);
    }

}
