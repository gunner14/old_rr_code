package com.xiaonei.commons.gid.service.http.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;

import com.xiaonei.commons.gid.service.http.routers.Router;
import com.xiaonei.commons.gid.util.G;

/**
 * {@link HomeController}用来跳转gid到具体的终端页
 * 
 * @author qieqie.wang@gmail.com
 * 
 */
@Path("{gid}")
public class HomeController {

    /** pc上的跳转 */
    @Autowired
    @Qualifier("g.pcRouter")
    private Router pcRouter;

    /**
     * 将http://www.renren.com/g/123456进行跳转
     * <p>
     * 可以给这个地址带一个u=xxx的参数表示对象的作者(所属人)
     * 
     * @param inv
     * @param gid
     * @return
     */
    @Get
    public String home(Invocation inv, @Param("gid") long gid) {
        String url = pcRouter.getTarget(gid, inv.getRequest());
        if (url != null) {
            if (url.startsWith("@")) {
                return url;
            } else {
                String query = inv.getRequest().getQueryString();
                return merge("r:" + url, query);
            }
        } else {
            return "error:404;not found router for " + gid + "(" + new G(gid) + ")";
        }
    }

    //------------------------------------------------------------

    private String merge(String url, String queryString) {
        return merge(url, url.lastIndexOf('?') != -1, queryString);
    }

    private String merge(String url, boolean urlWithQueryString, String queryString) {
        if (queryString == null) {
            return url;
        }
        if (urlWithQueryString) {
            return url + "&" + queryString;
        } else {
            return url + "?" + queryString;
        }
    }
}
