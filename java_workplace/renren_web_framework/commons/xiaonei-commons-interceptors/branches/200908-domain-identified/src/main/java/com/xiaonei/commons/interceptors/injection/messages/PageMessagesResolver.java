package com.xiaonei.commons.interceptors.injection.messages;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.paramresolver.Resolver;

/**
 * {@link PageMessagesResolver} 负责解析action中声明的 {@link PageMessages}对象
 * 
 * @author 王志亮 zhiliang.wang@opi-corp.com
 */

public class PageMessagesResolver implements Resolver {

    @Override
    public boolean accepted(Class<?> parameterType) {
        return PageMessages.class == parameterType;
    }

    @Override
    public Object resolve(Class<?> parameterType, int replicatedCount, int indexOfReplicated,
            Invocation invocation, String parameterName, Param paramAnnotation) throws Exception {
        PageMessages pms = (PageMessages) invocation.getModel().get(PageMessages.MODEL_KEY);
        if (pms == null) {
            pms = new PageMessagesImpl();
            invocation.getModel().add(PageMessages.MODEL_KEY, pms);
        }
        return pms;
    }

}
