package com.xiaonei.page.xoa.controllers.bizservice.doing;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.xoa.commons.bean.XoaBizErrorBean;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.DoingService;
import com.xiaonei.page.service.PageServiceHome;
import com.xiaonei.page.util.Keys;
import com.xiaonei.page.util.MemcachedUtil;
import com.xiaonei.page.xoa.domain.PageXoaErrors;
import com.xiaonei.page.xoa.util.PageXoaUtil;
import com.xiaonei.platform.component.xfeed.helper.FeedDefinition;
import com.xiaonei.platform.component.xfeed.remove.XFeedRemover;
import com.xiaonei.platform.core.model.User;

@Path("dd")
public class DelDoingController {

    private final static Logger logger = Logger.getLogger("PAGEXOA");

    @Autowired
    private PageServiceHome pageServiceHome;

    @Autowired
    private DoingService doingService;

    /**
     * 
     * 
     * @param pageId
     * @param doingId
     * @param executorId
     * @return 成功返回 dongId，失败，{@link XoaBizErrorBean}
     */
    @Post
    public Object index(@Param("pageId") int pageId, @Param("doingId") long doingId,
            @Param("opUserId") int opUserId) {
        //取page
        Page page = PageXoaUtil.getInstance().getPage(pageId);
        if (page == null) {
            return new XoaBizErrorBean(PageXoaErrors.PAGE_NOT_EXIST.getType(),
                    PageXoaErrors.PAGE_NOT_EXIST.getDesc());
        }

        //取user
        User host = PageXoaUtil.getInstance().getUser(opUserId);
        if (host == null) {
            return new XoaBizErrorBean(PageXoaErrors.USER_NOT_EXIST.getType(),
                    PageXoaErrors.USER_NOT_EXIST.getDesc());
        }

        //判断权限失败
        if (!PageXoaUtil.getInstance().authUserAndPage(opUserId, pageId)) {
            return new XoaBizErrorBean(PageXoaErrors.AUTH_FAILED.getType(),
                    PageXoaErrors.AUTH_FAILED.getDesc());
        }

        //干正事
        try {
            doingService.deleteDoingWithLog(page, doingId, host, null);
            MemcachedUtil.delete(Keys.DOING_COUNT_INTER, page.getId());
            pageServiceHome.deleteDoingCache(page.getId());//删缓存
            XFeedRemover.getInstance().removeFeedBySource(doingId,
                    FeedDefinition.PAGE_STATUS_UPDATE);
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            return new XoaBizErrorBean(PageXoaErrors.ERROR.getType(), PageXoaErrors.ERROR.getDesc());
        }

        return "@" + doingId;
    }
}
