package com.xiaonei.reg.guide.flows.oldactions.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;


/**
 * ShowPreviewController.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-9-1 为了兼容老的 "previewhs.do","previewdx.do" 
 * 2010-9-1 为了兼容老的 "pv-090729.do" 
 */

@LoginRequired
@Path({"previewhs.do","previewdx.do","pv-090729.do","preview-s-bl","preview-s-dx","preview-s-gz","preview-s-qt"})
public class ShowPreviewController {
    @Get
    @Post
    public String index(Invocation inv){
        return "/views/guide/oldactions/preview.jsp";
    }
}
