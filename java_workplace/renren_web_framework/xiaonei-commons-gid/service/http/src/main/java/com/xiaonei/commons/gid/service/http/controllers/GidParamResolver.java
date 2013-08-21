package com.xiaonei.commons.gid.service.http.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.paramresolver.ParamMetaData;
import net.paoding.rose.web.paramresolver.ParamResolver;

import com.xiaonei.commons.gid.util.G;
import com.xiaonei.commons.gid.util.Type;

/**
 * {@link GidParamResolver}实现了 {@link ParamResolver}，能够解析名称为"gid"的参数。
 * <p>
 * 合法的gid格式有两种：<br>
 * <ul>
 * <li>一个由数字组成的long字符串，比如 3674937300193008573</li>
 * <li>一个由类型（大小写不敏感）加一个原ID组成的字符串，两个字符串通过连接号("-")连接，比如photo-4259410044</li>
 * </ul>
 * 
 * @author qieqie.wang@gmail.com
 * 
 */
public class GidParamResolver implements ParamResolver {

    @Override
    public boolean supports(ParamMetaData metaData) {
        boolean supports = "gid".equals(metaData.getParamName());
        if (supports) {
            if (metaData.getParamType() != Long.class && metaData.getParamType() != long.class) {
                throw new IllegalArgumentException("illegal gid type :"
                        + metaData.getParamType().getName());
            }
        }
        return supports;
    }

    @Override
    public Object resolve(Invocation inv, ParamMetaData metaData) throws Exception {
        String paramName = metaData.getParamName();
        String gidString = inv.getParameter(paramName);
        if (gidString == null || gidString.length() == 0) {
            if (metaData.getParamType().isPrimitive()) {
                throw new NullPointerException("the request'gid param value is null");
            } else {
                return null;
            }
        }
        char first = gidString.charAt(0);
        if (first >= '0' && first <= '9') {
            return Long.parseLong(gidString);
        } else {
            int index = gidString.indexOf('-');
            if (index == -1) {
                throw new IllegalArgumentException("illegal gid param value: " + gidString);
            }
            String typeString = gidString.substring(0, index);
            long xid = Long.parseLong(gidString.substring(index + 1));
            if ("gid".equals(typeString)) {
                return xid;
            }
            Type type = Type.toType(typeString);
            return G.id(xid, type);
        }
    }

}
