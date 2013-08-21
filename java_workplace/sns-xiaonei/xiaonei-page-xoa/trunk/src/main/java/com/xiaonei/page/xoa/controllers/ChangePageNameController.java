package com.xiaonei.page.xoa.controllers;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.page.PageException;
import com.xiaonei.page.service.PageService;
import com.xiaonei.page.xoa.util.PageXoaUtil;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Post;

@Path("chgname")
public class ChangePageNameController {

    @Autowired
    private PageService pageService;

    @Post("")
    public String index(@Param("uid") int uid, @Param("pid") int pid,
            @Param("newname") String newName, @Param("ip") String ip) {
        try {
            User user = SnsAdapterFactory.getUserAdapter().get(uid);
            if (user == null) {
                return "@"
                        + PageXoaUtil.getInstance()
                                .getJsonRs(11, "[Change Name] get user info error").toString();
            }
            pageService.updatePageName(user, pid, newName, ip);
        } catch (PageException e) {
            return "@"
                    + PageXoaUtil.getInstance().getJsonRs(e.getCode(), e.getMessage()).toString();
        } catch (Exception e) {
            e.printStackTrace();
            return "@"
                    + PageXoaUtil.getInstance().getJsonRs(10, "[Change Name] unknown exception")
                            .toString();
        }

        return "@" + PageXoaUtil.getInstance().getJsonRs(0, "[Change Name] success").toString();
    }
}
