package com.xiaonei.page.xoa.controllers.bizservice.fans;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.xoa.commons.bean.XoaBizErrorBean;
import com.xiaonei.page.PageException;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.FansService;
import com.xiaonei.page.xoa.domain.PageXoaErrors;
import com.xiaonei.page.xoa.util.PageXoaUtil;
import com.xiaonei.platform.core.model.User;

@Path("pagefans")
public class PageFansController {
    @Autowired
    private FansService fansService;

    private static final Logger logger = Logger.getLogger("PAGEXOA");

    @Get("isfans")
    public Object isFans(@Param("pageId") int pageId, @Param("userId") int userId) {
        //取page
        Page page = PageXoaUtil.getInstance().getPage(pageId);
        if (page == null) {
            return new XoaBizErrorBean(PageXoaErrors.PAGE_NOT_EXIST.getType(),
                    PageXoaErrors.PAGE_NOT_EXIST.getDesc());
        }

        //取user
        User user = PageXoaUtil.getInstance().getUser(userId);
        if (user == null) {
            return new XoaBizErrorBean(PageXoaErrors.USER_NOT_EXIST.getType(),
                    PageXoaErrors.USER_NOT_EXIST.getDesc());
        }
        
        try {
            //打远程日志
            return fansService.isFans(page, user);
        } catch (PageException e) {
            logger.error(e.getMessage(), e);
            return new XoaBizErrorBean(PageXoaErrors.ERROR.getType(), PageXoaErrors.ERROR.getDesc());
        }
    }
}
