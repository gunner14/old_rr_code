package com.renren.sns.guide.rose.controllers;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.util.List;

import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.domain.network.UniversityInfo;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.portal.Portal;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.sns.platform.core.rose.cache.provider.UserProvider;

/**
 * @author: wei.xiang
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-9-1 上午10:47:12
 */
@Path("ajaxGuideSearchUniversitySchoolMate")
@LoginRequired
public class AjaxGuideSearchUniversitySchoolMateController {

    @Get
    public String index(Invocation inv, Portal portal, CacheManager cm) throws InterruptedException {
        User user = UserProvider.getInstance().get(cm);
        List<UniversityInfo> univInfoList = AdapterFactory.getNetworkAdapter().getUniversityInfoList(user.getId()); // 大学资料信息
        int enrollYear = 0;
        int universityId = 0;
        String universityName = "";
        if (univInfoList != null) {
            for (UniversityInfo universityInfo : univInfoList) {
                if (universityInfo.getEnrollYear() > enrollYear) {
                    enrollYear = universityInfo.getEnrollYear();
                    universityId = universityInfo.getUniversityId();
                    universityName = universityInfo.getUniversityName();
                }
            }
        }
        StringBuffer sb = new StringBuffer();
        try {
            sb.append(OpiConstants.urlBrowse + "/searchEx.do?s=0&p=");    
            sb.append("[{\"t\":\"univ\",\"id\":\""+universityId+"\",");
            if(enrollYear!=0){
                sb.append("\"year\":\""+enrollYear+"\",");
            }
            sb.append("\"name\":\""+URLEncoder.encode(universityName, "UTF-8")+"\"}]");
        } catch (UnsupportedEncodingException e) {
            // TODO Auto-generated catch block
            System.err.println("AjaxGuideSearchUniversitySchoolMateController:UnsupportedEncodingException:"+e.toString());
        }
        try {
            inv.getResponse().sendRedirect(sb.toString());
        } catch (IOException e) {
            // TODO Auto-generated catch block
            System.err.println("AjaxGuideSearchUniversitySchoolMateController:IOException:"+e.toString());
        }
        return "@";
    }
}
