package com.renren.sns.guide.rose.controllers;

import java.io.IOException;
import java.sql.SQLException;
import java.util.List;

import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.domain.network.HighSchoolInfo;
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
 * @create time：2010-8-31 下午03:44:06
 */
@LoginRequired
@Path("ajaxGuideRedirectToHighschoolPage")
public class AjaxGuideRedirectToHighschoolPageController {

    @Get
    public String index(Invocation inv, Portal portal, CacheManager cm) throws InterruptedException {
        User user = UserProvider.getInstance().get(cm);
        List<HighSchoolInfo> hsInfoList = AdapterFactory.getNetworkAdapter().getHighSchoolInfoList(user.getId()); // 高中资料信息
        int enrollYear = -1;
        int highSchoolId = 0;
        if (hsInfoList != null) {
            for (HighSchoolInfo highSchoolInfo : hsInfoList) {
                if (highSchoolInfo.getEnrollYear() > enrollYear) {
                    enrollYear = highSchoolInfo.getEnrollYear();
                    highSchoolId = highSchoolInfo.getHighSchoolId();
                }
            }
        }
        int pageId=0;
        try {
            pageId = PageAPIDAO.getInstance().getPidBySchoolId(highSchoolId);
        } catch (SQLException e1) {
            // TODO Auto-generated catch block
            System.err.println("AjaxGuideRedirectToHighschoolPageController:SQLException:" + e1.toString());
        }
        try {
            if (pageId == 0) {
                inv.getResponse().sendRedirect(OpiConstants.urlSchool.toString());
            } else {
                inv.getResponse().sendRedirect(OpiConstants.urlSchool.toString() + "/" + pageId);
            }
        } catch (IOException e) {
            System.err.println("AjaxGuideRedirectToHighschoolPageController:IOException:" + e.toString());
        }
        return "@";
    }
}
