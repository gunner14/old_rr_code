package com.xiaonei.page.xoa.controllers.bizservice.fans;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.xoa.commons.bean.XoaBizErrorBean;
import com.xiaonei.page.PageException;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.remotelog.LogEntry;
import com.xiaonei.page.remotelog.LogEntryUtil;
import com.xiaonei.page.remotelog.LogType;
import com.xiaonei.page.service.FansService;
import com.xiaonei.page.xoa.domain.PageXoaErrors;
import com.xiaonei.page.xoa.util.PageXoaUtil;
import com.xiaonei.platform.core.model.User;

@Path("mfs")
public class MakeFansController {

    @Autowired
    private FansService fansService;

    private static final Logger logger = Logger.getLogger("PAGEXOA");

    @Get
    public Object index(@Param("pageId") int pageId, @Param("userId") int userId,
            @Param("isSendFeed") boolean isSendFeed) {

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
            LogEntry remoteLog = LogEntryUtil.buildLogEntry(page, user, pageId, null,
                    LogType.MAKE_FANS, null);
            fansService.makeFans(page, user, isSendFeed, remoteLog);
        } catch (PageException e) {
            logger.error(e.getMessage(), e);
            return new XoaBizErrorBean(PageXoaErrors.ERROR.getType(), PageXoaErrors.ERROR.getDesc());
        }

        return "@";
    }
}
