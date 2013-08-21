package com.xiaonei.page.xoa.controllers.identify;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.PageService;
import com.xiaonei.page.service.identify.IdentifyService;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Post;

/**
 * 公共主页认证XOA接口
 * 
 * @author yi.li@renren-inc.com since 2012-2-8
 * 
 */
@Path("")
public class IdentifyController {

    @Autowired
    private PageService pageService;

    @Autowired
    private IdentifyService identifyService;

    /**
     * 将指定pageId的Page添加官方认证
     * 
     * @param pageId PAGE id
     * @param desc 认证描述长度不能大于20
     * @param excutor 执行者id
     * @return
     */
    @Post("add/{page-id:[0-9]+}/official")
    public Boolean addOfficialIdentity(@Param("page-id") int pageId, @Param("desc") String desc,
            @Param("executor") int excutor) {
        try {
            Page page = pageService.get(pageId);
            if (page == null) {
                return false;
            }
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
        
        if (desc.length() > 20) {
            return false;
        }

        identifyService.addRenRenIdentifier(pageId, desc, "", excutor);
        return true;
    }
}
