package com.xiaonei.reg.guide.flows.mobile.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

//http://guide.renren.com/suggest/Suggest.do?query=s&stype=WORKPLACENAME





/**
 * MobileSuggestController.java
 * @author inshion(xin.yin@opi-corp.com)
 *
 * @date 2010-8-24
 */
@Path("/suggest/Suggest.do")
public class MobileSuggestController {

    @Get
    @Post
    public String index(Invocation inv) {
        String query = inv.getParameter("query");
        String stype = inv.getParameter("stype");
        inv.setAttribute("query", query);
        inv.setAttribute("stype", stype);
        return "" + "/views/guide/ajax/ajax_input_suggest.jsp";
    }
}
