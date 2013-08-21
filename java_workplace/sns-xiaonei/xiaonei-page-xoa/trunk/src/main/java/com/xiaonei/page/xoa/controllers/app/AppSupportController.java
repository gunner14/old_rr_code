package com.xiaonei.page.xoa.controllers.app;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.page.service.app.AppSupportService;
import com.xiaonei.page.xoa.util.PageXoaUtil;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Post;

/**
 * 为APP提供的服务，暂时是将APP关联到Page的相关服务
 * 
 * @author yi.li@renren-inc.com since 2012-1-4
 * 
 */
@Path("")
public class AppSupportController {

    @Autowired
    private AppSupportService appSupportService;

    /**
     * <p>
     * 将ID为appId的应用关联到ID是pageId的PAGE
     * 
     * @param appId app ID
     * @param pageId page ID
     * @param uid user ID
     * @return
     */
    @Post("relate")
    public String relateAppToPage(@Param("appId") int appId, @Param("pageId") int pageId,
            @Param("user-id") int uid) {
        try {
            User user = SnsAdapterFactory.getUserAdapter().get(uid);
            if (user == null) {
                return "@"
                        + PageXoaUtil.getInstance()
                                .getJsonRs(1, "[Relate APP] get user info error").toString();
            }
            appSupportService.relateAppToPage(appId, pageId, user);
        } catch (Exception e) {
            e.printStackTrace();
            return "@"
                    + PageXoaUtil.getInstance().getJsonRs(2, "[Relate APP] exception happens")
                            .toString();
        }
        
        return "@" + PageXoaUtil.getInstance().getJsonRs(0, "[Relate APP] success").toString();
    }
}
