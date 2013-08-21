package com.renren.entries.xoa.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.paramresolver.ParamMetaData;
import net.paoding.rose.web.paramresolver.ParamResolver;

import com.xiaonei.commons.gid.util.Type;

/**
 * 
 * @author qieqie.wang@gmail.com
 * 
 */
public class TypeResolver implements ParamResolver {

    @Override
    public boolean supports(ParamMetaData metaData) {
        return metaData.getParamType() == Type.class;
    }

    @Override
    public Object resolve(Invocation inv, ParamMetaData metaData) throws Exception {
        String name = inv.getRequest().getParameter(metaData.getParamName());
        return name == null ? null : Type.toType(name);
    }

}
