package com.xiaonei.commons.gid.service.http.controllers.tool;

import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

/**
 * 
 * @author qieqie
 * 
 */
@Path("")
public class IndexController {

    /**
     * 跳转到 {@link TransferController}
     * 
     * @return
     */
    @Get
    public String redirect() {
        return "r:c:transfer";
    }
}
