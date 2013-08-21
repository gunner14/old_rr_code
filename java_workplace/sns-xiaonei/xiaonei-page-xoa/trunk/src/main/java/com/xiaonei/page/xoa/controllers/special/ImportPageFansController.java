package com.xiaonei.page.xoa.controllers.special;

import java.util.List;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.page.dao.PageFansDAO;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.domain.PageFans;
import com.xiaonei.page.remotelog.LogEntry;
import com.xiaonei.page.remotelog.LogEntryUtil;
import com.xiaonei.page.remotelog.LogType;
import com.xiaonei.page.service.FansService;
import com.xiaonei.page.service.PageService;
import com.xiaonei.page.xoa.util.MailUtil;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;

/**
 * 
 * 两个page之间导粉丝
 * 
 * @author happyshelocks@gmail.com
 * 
 */
@Path("ipf")
public class ImportPageFansController {

    @Autowired
    private FansService fansService;

    @Autowired
    private PageService pageService;

    private static final Logger logger = Logger.getLogger("PAGEXOA");

    @SuppressWarnings("static-access")
    @Get
    public void index(@Param("sourcePageId") int sourcePageId,
            @Param("targetPageId") int targetPageId, @Param("mailTo") String mailTo) {
        int importedFansCount = 0;
        try {
            Page sourcePage = pageService.get(sourcePageId);
            Page targetpage = pageService.get(targetPageId);
            if (sourcePage == null || targetpage == null) {
                String msg = String.format(
                        "some page not exsit!source page is %s,target page is %s", sourcePageId,
                        targetPageId);
                logger.info(msg);
            }
            //获取粉丝数
            int fansCount = PageFansDAO.getInstance(sourcePage.isBig()).findCountByState(
                    sourcePageId, PageFans.STATE_ACTIVE);
            
            //查询粉丝,且状态为PageFans.STATE_ACTIVE
            List<Integer> fans = PageFansDAO.getInstance(sourcePage.isBig()).findPageFansIds(
                    fansCount, sourcePageId);

            for (Integer fan : fans) {
                User host = null;
                try {
                    host = WUserAdapter.getInstance().get(fan);
                    if (!fansService.isFans(targetPageId, host)) {//如果不是page的粉丝
                        //打远程日志
                        LogEntry remoteLog = LogEntryUtil.buildLogEntry(targetpage, host,
                                targetPageId, null, LogType.MAKE_FANS, null);
                        //不发新鲜事
                        fansService.makeFans(targetpage, host, false, remoteLog);
                        //停一会，数据库压力
                        Thread.currentThread().sleep(100);
                        importedFansCount++;
                    }

                } catch (Exception e) {
                    logger.error(String
                            .format("some makeFans error happend!page id is %s,user id is %s.error msg is %s",
                                    targetPageId, host.getId()));
                    logger.error(e.getMessage(), e);
                }
            }
        } catch (Exception e) {
            logger.error(String.format(
                    "some db error happend!source page id is %s,target page id is %s.",
                    sourcePageId, targetPageId));
            logger.error(e.getMessage(), e);
        }
        String msg = String.format(
                "source page id is %s,target page id is %s,import fans count is %s", sourcePageId,
                targetPageId, importedFansCount);
        logger.info(msg);

        //send mails
        if (mailTo != null && !mailTo.equals("")) {
            try {
                String[] mails=mailTo.split(";");
                for (String mail : mails) {
                    new MailUtil("Import Fans Between Pages.", msg).sendTo(mail);
                } 
            } catch (Exception e) {
                logger.error(
                        String.format(
                                "Send mail failed!Mail address is %s.Source Page Id is %s.Target Page Id is %s.Imported Fans Count is %s",
                                mailTo, sourcePageId, targetPageId, importedFansCount), e);
            }
        }

    }
}
