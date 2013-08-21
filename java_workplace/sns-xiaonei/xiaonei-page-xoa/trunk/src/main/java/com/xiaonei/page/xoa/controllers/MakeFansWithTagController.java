package com.xiaonei.page.xoa.controllers;

import java.util.List;

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
import com.xiaonei.page.service.GuideTagService;
import com.xiaonei.page.service.PageService;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;

@Path("mfswt")
public class MakeFansWithTagController {

    @Autowired
    private FansService fansService;
    
    @Autowired
    private PageService pageService;
    
    @Autowired
    private GuideTagService guideTagService;

    private static final Logger logger = Logger.getLogger("PAGEXOA");

    @Get
    public String index(@Param("tag") String tag, @Param("userId") int userId,
            @Param("clientIp") String clientIp, @Param("biz") String biz) {

        List<Integer> pages = null;
        User user = null;
        try {
            pages = guideTagService.getPages(tag);
            user = WUserAdapter.getInstance().get(userId);
        } catch (Exception e) {
            logger.error("make fans failed!tag is:" + tag + ",userid is:" + userId);
            logger.error(e.getMessage(), e);
            return "@-1";
        }

        int result = -1;
        if (pages != null && user != null) {
            try {
                for(int pageId:pages){
                    Page page = pageService.get(pageId);
                    if(page==null)
                        continue;
                    //打远程日志
                    LogEntry remoteLog = LogEntryUtil.buildLogEntry(pageId, userId, pageId, "TYPE_XOA",
                            LogType.MAKE_FANS, null);
                    remoteLog.setIp(clientIp);
                    remoteLog.setAttach(biz);
                    int o_result = fansService.makeFans(page, user, true, remoteLog);
                    if(o_result>result)
                        result = o_result;
                }
            } catch (PageException e) {
                logger.error(e.getMessage(), e);
                return "@-1";
            }
        }
        else{
            logger.error("tag is:" + tag + ",userid is:" + userId+",");
        }
        return "@" + String.valueOf(result);
    }
}
