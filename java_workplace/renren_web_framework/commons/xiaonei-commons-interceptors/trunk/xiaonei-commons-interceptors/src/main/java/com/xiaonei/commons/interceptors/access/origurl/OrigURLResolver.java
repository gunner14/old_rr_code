package com.xiaonei.commons.interceptors.access.origurl;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Ignored;
import net.paoding.rose.web.paramresolver.ParamMetaData;
import net.paoding.rose.web.paramresolver.ParamResolver;

/**
 * 
 * @author zhiliang.wang
 * 
 */
//不启用
@Ignored
public class OrigURLResolver implements ParamResolver {

    @Override
    public boolean supports(ParamMetaData metaData) {
        return metaData.getParamType() == OrigURL.class;
    }

    @Override
    public Object resolve(Invocation inv, ParamMetaData metaData) throws Exception {
        Invocation head = inv;
        while (head.getPreInvocation() != null) {
            head = head.getPreInvocation();
        }
        OrigURL origURL = (OrigURL) head.getAttribute(OrigURL.class.getName());
        if (origURL == null) {
            synchronized (head) {
                origURL = (OrigURL) head.getAttribute(OrigURL.class.getName());
                if (origURL == null) {
                    origURL = new OrigURLImpl(inv);
                    head.setAttribute(OrigURL.class.getName(), head);
                }
            }
        }
        return origURL;
    }
}
