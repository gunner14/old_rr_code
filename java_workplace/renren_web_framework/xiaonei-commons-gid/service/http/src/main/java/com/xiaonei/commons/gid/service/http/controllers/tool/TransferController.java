package com.xiaonei.commons.gid.service.http.controllers.tool;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.rest.Get;

import com.xiaonei.commons.gid.util.G;
import com.xiaonei.commons.gid.util.Type;

/**
 * 
 * @author chuanyu.ban@renren-inc.com
 * @author qieqie.wang@gmail.com
 * 
 */
public class TransferController {

    @Get({ "", "{gid}" })
    public String getGid(Invocation inv, @Param("gid") Long gid) throws IllegalArgumentException,
            IllegalAccessException {
        inv.addModel("types", Type.allTypes());
        if (gid != null) {
            inv.addModel("g", new G(gid));
        }
        return "home";
    }

}
