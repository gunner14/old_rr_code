package com.renren.sns.wiki.controllers.adminwiki;

import java.util.List;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.wiki.service.SuperAdminService;
import com.xiaonei.platform.core.model.WUserCache;

@Path("superAdmin")
public class SuperAdminController {

    private Log logger = LogFactory.getLog(SuperAdminController.class);

    @Autowired
    private SuperAdminService superAdminService;

    @Get("")
    public String list(Invocation inv) {
        
        inv.addModel("navName", "superAdmin");

        List<WUserCache> userCacheList = superAdminService.getSuperAdminList();

        inv.addModel("userCacheList", userCacheList);

        return "superadmin-list";
    }

    /**
     * 添加超管
     * 
     * @param inv
     * @param uid
     * @return
     */
    @Post("addSuperAdmin")
    public String addSuperAdmin(Invocation inv, @Param("uid") String uid) {

        if (StringUtils.isBlank(uid) || !uid.matches("\\d+")) {
            logger.info("uid is null or not numbers");
        } else {
            int userId = Integer.parseInt(uid);
            boolean result = superAdminService.addSuperAdmin(userId);
            if (!result) {
                logger.info("添加超管失败, userId=" + userId);
            }
        }
        return "r:/adminwiki/superAdmin";
    }

    /**
     * 移除超管
     * 
     * @param inv
     * @param uid
     * @return
     */
    @Get("removeSuperAdmin")
    @Post("removeSuperAdmin")
    public String removeSuperAdmin(Invocation inv, @Param("uid") String uid) {

        if (StringUtils.isBlank(uid) || !uid.matches("\\d+")) {
            logger.info("uid is null or not numbers");
        } else {
            int userId = Integer.parseInt(uid);
            boolean result = superAdminService.removeSuperAdmin(userId);
            if (!result) {
                logger.info("添加超管失败, userId=" + userId);
            }
        }
        return "r:/adminwiki/superAdmin";
    }

}
