package com.xiaonei.commons.interceptors.injection.messages;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.paramresolver.ParamMetaData;
import net.paoding.rose.web.paramresolver.ParamResolver;

/**
 * {@link PageMessagesResolver} 负责解析action中声明的 {@link PageMessages}对象
 * 
 * @author 王志亮 zhiliang.wang@opi-corp.com
 */

public class PageMessagesResolver implements ParamResolver {

    @Override
    public boolean supports(ParamMetaData metaData) {
        return PageMessages.class == metaData.getParamType();
    }

    @Override
    public Object resolve(Invocation inv, ParamMetaData metaData) throws Exception {
        return PageMessagesUtil.getPageMessages(inv);
    }

}
