package com.renren.sns.wiki.controllers.ajax;

import com.renren.sns.wiki.utils.remotelog.RemoteLogType;
import com.renren.sns.wiki.utils.remotelog.annotation.RemoteLog;

import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

/**
 * 用于前段打日志
 * 
 * @author yi.li@renren-inc.com since Aug 7, 2012
 * 
 */
@Path("click_log")
public class ClickLogController extends BaseAjaxController {

    /**
     * 用来记录点击日志。
     * <ul>
     * <li>tag表示点击对象，用来区别不同的业务</li>
     * <li>ref代表动作来源，即在哪发生的动作</li>
     * </ul>
     */
    @Get("")
    @Post("")
    @RemoteLog(type = RemoteLogType.CLICK_LOG, extras = { "tag", "ref" })
    public String doLog() {
        return this.writeJsonData(null);
    }
}
