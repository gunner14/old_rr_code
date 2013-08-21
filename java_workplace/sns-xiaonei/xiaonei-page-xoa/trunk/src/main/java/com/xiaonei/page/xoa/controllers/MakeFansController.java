package com.xiaonei.page.xoa.controllers;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.page.PageException;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.remotelog.LogEntry;
import com.xiaonei.page.remotelog.LogEntryUtil;
import com.xiaonei.page.remotelog.LogType;
import com.xiaonei.page.service.FansService;
import com.xiaonei.page.service.PageService;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;

@Path("mfs")
public class MakeFansController {

    @Autowired
    private FansService fansService;

    @Autowired
    private PageService pageService;

    private static final Logger logger = Logger.getLogger("PAGEXOA");

    @Get
    public String index(@Param("pageId") int pageId, @Param("userId") int userId,
            @Param("isSendFeed") boolean isSendFeed) {

        Page page = null;
        User user = null;
        try {
            page = pageService.get(pageId);
            user = WUserAdapter.getInstance().get(userId);
        } catch (Exception e) {
            logger.error("make fans failed!pageid is:" + pageId + ",userid is:" + userId);
            logger.error(e.getMessage(), e);
            return "@-1";
        }

        int result = -1;
        if (page != null && user != null) {
            try {
                //打远程日志
                LogEntry remoteLog = LogEntryUtil.buildLogEntry(page, user, pageId, null,
                        LogType.MAKE_FANS, null);
                remoteLog.setUserAgent("TYPE_XOA");
                result = fansService.makeFans(page, user, isSendFeed, remoteLog);
            } catch (PageException e) {
                logger.error(e.getMessage(), e);
                return "@-1";
            }

        }
        return "@" + String.valueOf(result);
    }
}
