package com.renren.sns.wiki.controllers.ajax;

import java.util.Map;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.wiki.delegator.security.AntiSpamDelegator;
import com.renren.sns.wiki.exception.WikiExceptionType;
import com.renren.sns.wiki.model.UserActionRecord;
import com.renren.sns.wiki.service.UserActionRecordService;
import com.renren.sns.wiki.utils.RemoteAddrUtil;
import com.renren.sns.wiki.utils.remotelog.RemoteLogType;
import com.renren.sns.wiki.utils.remotelog.annotation.RemoteLog;
import com.xiaonei.antispam.model.CheckResult;
import com.xiaonei.antispam.model.CheckType;
import com.xiaonei.antispam.validator.HtmlBean;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.commons.interceptors.access.annotation.ValidateRequestToken;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;

/**
 * 想看 再看 取消收藏 评分相关收藏wiki操作的controller
 * 
 * @author weiwei.wang@renren-inc.com since 2012-5-21
 * 
 */
@Path("")
public class UserActionRecordController extends BaseAjaxController {

    @Autowired
    private HostHolder hostHolder;

    @Autowired
    private UserActionRecordService userActionRecordService;

    @Autowired
    private AntiSpamDelegator antiSpamDelegator;

    @Post("/interest/${wikiId:\\d+}")
    @ValidateRequestToken
    @LoginRequired
    @RemoteLog(type = RemoteLogType.INTEREST, extras = { "userActionRecord.interest",
            "userActionRecord.rating", "userActionRecord.comment" })
    public String interest(Invocation inv, @Param("wikiId") int wikiId,
            @Param("userActionRecord") @HtmlBean UserActionRecord userActionRecord) {

        User host = hostHolder.getUser();
        String userIp = RemoteAddrUtil.getClientIP(inv.getRequest());

        String comment = userActionRecord.getComment();
        boolean audit = false;
        if (comment != null) {
            if (comment.length() > 240) {
                comment = comment.substring(0, 240);
                userActionRecord.setComment(comment);
            }
            //违禁词 敏感词过滤
            CheckResult checkResult = antiSpamDelegator.antispamFilter(host.getId(), 0,
                    CheckType.SOCIAL_WIKI_COMMENT, comment, userIp);

            //违禁词,不能发布
            if (CheckResult.PROHIBITED == checkResult.getFlag()) {
                return writeJson(WikiExceptionType.PROHIBITED.getCode(),
                        checkResult.getShowMessage(), "");
            } else if (CheckResult.CONTENT_NEED_AUDIT == checkResult.getFlag()
                    || CheckResult.AUDIT_AND_NOT_NOTIFY == checkResult.getFlag()) {
                //是否需要审核
                audit = true;
            }
        }

        userActionRecordService.interest(wikiId, host, userActionRecord, audit);

        Map<String, Object> dataMap = userActionRecordService.getInterestInfoMap(wikiId,
                host.getId());

        return writeJsonData(dataMap);
    }

    /**
     * 删除收藏
     * 
     * @param inv
     * @param wikiId
     * @param userActionRecord
     * @return
     */
    @Post("/interest/remove/${wikiId:\\d+}")
    @ValidateRequestToken
    public String removeInterest(Invocation inv, @Param("wikiId") int wikiId) {
        User host = hostHolder.getUser();
        userActionRecordService.romove(wikiId, host);
        Map<String, Object> dataMap = userActionRecordService.getInterestInfoMap(wikiId,
                host.getId());
        return writeJsonData(dataMap);
    }

    /**
     * 获取用户交互的状态
     * 
     * @param inv
     * @param wikiId
     * @return
     */
    @Get("/interest/status/${wikiId:\\d+}")
    @Post("/interest/status/${wikiId:\\d+}")
    @LoginRequired
    public String status(Invocation inv, @Param("wikiId") int wikiId) {

        User host = hostHolder.getUser();

        Map<String, Object> dataMap = userActionRecordService.getInterestInfoMap(wikiId,
                host.getId());
        return writeJsonData(dataMap);
    }

}
