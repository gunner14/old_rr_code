package com.xiaonei.commons.gid.service.http.routers.pc;

import java.util.HashMap;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;

import com.xiaonei.commons.gid.service.http.routers.Router;
import com.xiaonei.commons.gid.util.G;
import com.xiaonei.commons.gid.util.Type;

/**
 * 
 * @author 王志亮 zhiliang.wang@opi-corp.com
 * 
 */
public class PCRouter implements Router {

    private Map<Type, Router> routers = new HashMap<Type, Router>();

    public PCRouter() {
    }

    public void setRoutersByName(Map<String, Router> routers) {
        for (Map.Entry<String, Router> entry : routers.entrySet()) {
            Type type = Type.toType(entry.getKey());
            if (type == null) {
                throw new IllegalArgumentException("wrong GType name " + entry.getKey());
            }
            this.routers.put(type, entry.getValue());
        }
    }

    @Override
    public String getTarget(long gid, HttpServletRequest request) {
        Router router = routers == null ? null : routers.get(G.extractType(gid));
        return router == null ? null : router.getTarget(gid, request);
    }

}
