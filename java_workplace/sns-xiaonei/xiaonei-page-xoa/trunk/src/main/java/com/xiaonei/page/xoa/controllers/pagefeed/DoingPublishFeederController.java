package com.xiaonei.page.xoa.controllers.pagefeed;

import org.apache.log4j.Logger;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import com.renren.xoa.commons.annotation.JsonParam;
import com.renren.xoa.commons.bean.XoaBizErrorBean;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.xoa.controllers.pagefeed.builder.StatusBuilder;
import com.xiaonei.page.xoa.domain.PageXoaErrors;
import com.xiaonei.page.xoa.domain.feed.StatusFeed;
import com.xiaonei.page.xoa.util.PageIdChecker;
import com.xiaonei.page.xoa.util.PageXoaUtil;
import com.xiaonei.platform.core.model.User;

/**
 * 
 * page发布状态时发新鲜事。新版。
 * 
 * @author happyshelocks@gmail.com
 * 
 */
@Path("dpf")
public class DoingPublishFeederController {

    private final static Logger logger = Logger.getLogger("PAGEXOA");
    
    @Get
    public Object index(@Param("pageId") int pageId, @Param("curHost") int curHost,
            @Param("feed")@JsonParam StatusFeed feed) {
        //如果curHost不是page，直接不发
        if (pageId != 600002123) {
            return new XoaBizErrorBean(PageXoaErrors.ERROR.getType(), PageXoaErrors.ERROR.getDesc());
        }
        if (!PageIdChecker.isPageDomainIn(curHost)) {
            return new XoaBizErrorBean(PageXoaErrors.ERROR.getType(), PageXoaErrors.ERROR.getDesc());
        }
        //取page
        Page page = PageXoaUtil.getInstance().getPage(pageId);
        if (page == null) {
            return new XoaBizErrorBean(PageXoaErrors.PAGE_NOT_EXIST.getType(),
                    PageXoaErrors.PAGE_NOT_EXIST.getDesc());
        }

        //取user
        User host = PageXoaUtil.getInstance().getUser(curHost);
        if (host == null) {
            return new XoaBizErrorBean(PageXoaErrors.USER_NOT_EXIST.getType(),
                    PageXoaErrors.USER_NOT_EXIST.getDesc());
        }

        //判断权限失败,不是page的管理员
        if (!PageXoaUtil.getInstance().authUserAndPage(curHost, pageId)) {
            return new XoaBizErrorBean(PageXoaErrors.AUTH_FAILED.getType(),
                    PageXoaErrors.AUTH_FAILED.getDesc());
        }
        try {
            StatusBuilder.getInstance().DispatchFeed(feed, null);
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            return new XoaBizErrorBean(PageXoaErrors.SEND_FEED_FAILED.getType(),
                    PageXoaErrors.SEND_FEED_FAILED.getDesc());
        }
        return "@";
    }
}
