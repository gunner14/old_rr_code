package com.renren.sns.wiki.service.admin;

import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.biz.SuperAdminBiz;
import com.renren.sns.wiki.service.SuperAdminService;
import com.renren.sns.wiki.utils.cache.CacheDelegator;
import com.renren.sns.wiki.utils.cache.CacheNameSpace;
import com.renren.sns.wiki.utils.cache.CachePrefix;
import com.renren.sns.wiki.utils.cache.CacheDelegator.QueryAction;
import com.xiaonei.platform.core.model.User;

/**
 * 
 * @author weiwei.wang@renren-inc.com since 2012-7-3
 * 
 */
@Service
public class AdminService implements InitializingBean {

    @Autowired
    private SuperAdminService superAdminService;

    @Autowired
    private SuperAdminBiz superAdminBiz;

    private Set<Integer> adminIds;

    /**
     * 判断是否是管理员
     * 
     * @param adminTicket
     * @return
     */
    public boolean isWikiAdmin(String adminTicket) {
        String result = CacheDelegator.get(new QueryAction<String>(CacheNameSpace.WIKI,
                CachePrefix.WIKI_ADMIN_TICKET, String.class, adminTicket) {

            @Override
            public String getFromOther() {
                return null;
            }
        }, null);
        return !StringUtils.isBlank(result);
    }

    /**
     * 添加生成的uuid,用来验证是否是管理员
     * 
     * @param uuid 随机生成的uuid
     * @param 验票返回的value
     */
    public void addAdminTicket(String uuid, String value) {
        CacheDelegator.set(CacheNameSpace.WIKI, CachePrefix.WIKI_ADMIN_TICKET, String.class, uuid,
                value);
    }

    /**
     * 是不是超管
     * 
     * @param host
     * @return
     */
    public boolean isSuperAdmin(User host) {
        boolean isSuperAdmin = false;
        if (host != null) {
            isSuperAdmin = superAdminService.isSuperAdmin(host.getId());
        }
        return isSuperAdmin;
    }

    /**
     * 判断是否是超级管理员
     * 
     * @param userId
     * @return true if is super administrator otherwise false
     */
    public boolean isSuperAdminInFly(int userId) {
        if (adminIds.contains(userId)) {
            return true;
        }
        return false;
    }

    @Override
    public void afterPropertiesSet() throws Exception {
        List<Integer> superAdmins = superAdminBiz.getSuperAdminIdList();
        if (null == superAdmins || superAdmins.size() == 0) {
            return;
        }
        adminIds = new HashSet<Integer>();
        for (Integer adminId : superAdmins) {
            adminIds.add(adminId);
        }
    }

}
