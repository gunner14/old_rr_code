package com.renren.sns.wiki.controllers.adminwiki;

import java.util.UUID;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;

import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.wiki.constant.WikiWebConstant;
import com.renren.sns.wiki.service.admin.AdminService;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.CookieManager;

/**
 * 管理后台登陆验票
 * 
 * @author weiwei.wang@renren-inc.com since 2012-7-3
 * 
 */
public class SuperAdminInterceptor extends ControllerInterceptorAdapter {

    @Autowired
    private AdminService adminService;

    @Autowired
    private HostHolder hostHolder;

    public SuperAdminInterceptor() {
        setPriority(100);
    }

    @Override
    public Object before(Invocation invocation) throws Exception {
        HttpServletRequest request = invocation.getRequest();
        HttpServletResponse response = invocation.getResponse();

        User host = hostHolder.getUser();
        // 没有登录，或者不是超级管理员,跳转到人人首页
        if ((host == null) || !adminService.isSuperAdmin(host)) {
            String rediectUrl = "http://www.renren.com/SysHome.do?origURL=http%3A%2F%2Fwiki.renren.com%2Fadminwiki";
            response.sendRedirect(rediectUrl);
        } else {
            //判断是否是管理员
            String adminTicket = CookieManager.getInstance().getCookie(request, "wiki_admin_t");
            boolean isWikiAdmin = adminService.isWikiAdmin(adminTicket);
            if (!isWikiAdmin) {
                String ticket = invocation.getParameter("ticket");
                String ticketResult = null;
                if (!StringUtils.isBlank(ticket)) {
                    ticketResult = PassportUtils.getPassport(ticket);
                    //生成uuid,添加到缓存和cookie中去
                    String uuid = UUID.randomUUID().toString().replaceAll("[^\\w]", "");
                    adminService.addAdminTicket(uuid, ticketResult);
                    CookieManager.getInstance().saveCookie(invocation.getResponse(),
                            "wiki_admin_t", uuid, 24 * 3600, "/");
                }
                if (StringUtils.isBlank(ticket) || StringUtils.isBlank(ticketResult)) {
                    String rediectUrl = "https://passport.no.opi-corp.com/login.php?forward="
                            + WikiWebConstant.urlWiki + "/adminwiki";
                    response.sendRedirect(rediectUrl);
                }
            }
        }
        return true;
    }

}
