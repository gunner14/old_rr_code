package com.xiaonei.reg.guide.flows.mobile.controllers;

import java.util.HashMap;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.guide.flows.oldactionsutil.ActionForm;
import com.xiaonei.reg.guide.flows.oldactionsutil.ActionForward;
import com.xiaonei.reg.guide.flows.oldactionsutil.ActionMapping;
import com.xiaonei.reg.guide.util.GuideLogger;

/**
 * SkipHeadUploadController.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-9-2 新建文件
 */
@LoginRequired
@Path("skipupload.do")
public class SkipHeadUploadController{

    //<action path="/skipupload" type="com.xiaonei.reg.guide.action.a090729.SkipHeadUploadAction"></action>
    @Get
    @Post
    public String index(Invocation inv) {
        HttpServletRequest request = inv.getRequest();
        HttpServletResponse response = inv.getResponse();
        Map<String, ActionForward> map = new HashMap<String, ActionForward>();
        ActionMapping mapping = new ActionMapping(map);
        return executePt(mapping, null, request, response).toString();
    }
    
    
	public ActionForward executePt(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		User host = BaseThreadUtil.currentHost(request);
		GuideLogger.printLog("host:"+host.getId());
		String headurl = Globals.urlGuide + "/hu-090729.do"+"?skip=1";
		return new ActionForward(headurl, true);
	}

}
