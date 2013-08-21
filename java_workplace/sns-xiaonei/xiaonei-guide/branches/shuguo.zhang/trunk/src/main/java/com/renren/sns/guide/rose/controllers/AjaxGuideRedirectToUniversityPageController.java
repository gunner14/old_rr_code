package com.renren.sns.guide.rose.controllers;

import java.io.IOException;
import java.sql.SQLException;
import java.util.List;

import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.domain.network.UniversityInfo;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.portal.Portal;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.highschool.dao.PageAPIDAO;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.sns.platform.core.rose.cache.provider.UserProvider;

/**
 * @author: wei.xiang
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-8-31 下午03:12:36
 */
@LoginRequired
@Path("ajaxGuideRedirectToUniversityPage")
public class AjaxGuideRedirectToUniversityPageController {

    @Get
    public String index(Invocation inv, Portal portal, CacheManager cm) throws InterruptedException {
        User user = UserProvider.getInstance().get(cm);
        List<UniversityInfo> univInfoList = AdapterFactory.getNetworkAdapter().getUniversityInfoList(user.getId()); // 大学资料信息
        int enrollYear = -1;
        int universityId = -1;
        if (univInfoList != null) {
            for (UniversityInfo universityInfo : univInfoList) {
                if (universityInfo.getEnrollYear() > enrollYear) {
                    enrollYear = universityInfo.getEnrollYear();
                    universityId = universityInfo.getUniversityId();
                }
            }
        }
        int pageId=0;
        try {
            pageId = PageAPIDAO.getInstance().getPidBySchoolId(universityId);
        } catch (SQLException e1) {
            // TODO Auto-generated catch block
            System.err.println("AjaxGuideRedirectToUniversityPageController:SQLException:" + e1.toString());
        }
        try {
            if (pageId == 0) {
                inv.getResponse().sendRedirect(OpiConstants.urlSchool.toString());
            } else {
                inv.getResponse().sendRedirect(OpiConstants.urlSchool + "/s/" + pageId);
            }
        } catch (IOException e) {
            System.err.println("AjaxGuideRedirectToUniversityPageController:IOException:" + e.toString());
        }
        return "@";
    }
}
