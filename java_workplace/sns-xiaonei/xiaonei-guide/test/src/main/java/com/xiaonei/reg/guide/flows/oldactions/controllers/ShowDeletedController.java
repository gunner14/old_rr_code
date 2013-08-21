package com.xiaonei.reg.guide.flows.oldactions.controllers;

import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

@LoginRequired
@Path({"xreqFrd","pagelanding","addpages"})
@IgnoreUserStatusImportedValidation
public class ShowDeletedController {
    @Get
    @Post
    public String index(Invocation inv){
    	return "@";
    }
}
