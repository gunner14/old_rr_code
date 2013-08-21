package com.renren.sns.wiki.controllers.ajax;

import java.util.Map;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.wiki.exception.WikiExceptionType;
import com.renren.sns.wiki.interceptor.WikiHolder;
import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.service.UserActionRecordService;
import com.renren.sns.wiki.utils.remotelog.RemoteLogType;
import com.renren.sns.wiki.utils.remotelog.annotation.RemoteLog;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.platform.core.model.User;

/**
 * card弹层信息
 * 
 * @author weiwei.wang@renren-inc.com since 2012-4-1
 */
@Path("/card/${wikiId:number}")
public class WikiCardController extends BaseAjaxController {

    @Autowired
    private HostHolder hostHolder;

    @Autowired
    private WikiHolder wikiHolder;

    @Autowired
    private UserActionRecordService userActionRecordService;

    @Get("")
    @RemoteLog(type = RemoteLogType.CARD_DISPLAY)
    public String wikiCard(Invocation inv, @Param("wikiId") int wikiId) {
        //wiki为空的过滤器会拦截此请求话不会执行到此,无需判断wiki是否为null
        Wiki wiki = wikiHolder.getWiki();

        User host = hostHolder.getUser();

        int code = host == null ? WikiExceptionType.LOGIN_REQUIRED.getCode() : 0;

        Map<String, Object> dataMap = userActionRecordService.getFullInfoMap(wiki, host);
        return writeJson(code, "", dataMap);

    }

}
