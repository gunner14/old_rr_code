package com.xiaonei.page.xoa.controllers;

import com.xiaonei.page.PageException;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.AdminService;
import com.xiaonei.page.service.PageService;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

/**
 * User: shunlong.jin@renren-inc.com
 * Date: 11-11-28
 * Time: 下午12:02
 */
@Path("apa")
public class AddPageAdminController {
    @Autowired
    private AdminService adminService;

    @Autowired
    private PageService pageService;

    private final static Logger logger = Logger.getLogger("PAGEXOA");

    @Get
    public String index(@Param("userId") int userId,
                        @Param("pageId") int pageId) {
        Page page;
        try {
            page = pageService.get(pageId);
            if (page == null) {
                return "@NO_PAGE";
            }
            int currentOwnerId = page.getOwnerId();
            adminService.addAdmin(page, userId, currentOwnerId);
            //PAGE-379 如果添加的管理员是page owner那么赋予系统管理员的身份
            if (userId > 0 && (userId == page.getOwnerId())) {
                adminService.addSystemAdmin(pageId, userId);
            }
        } catch (PageException e) {
            logger.error(e.getMessage(), e);
            return "@MAX_PAGE_ADMIN";
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            return "@OTHER_ERROR";
        }
        return "@true";
    }
}
