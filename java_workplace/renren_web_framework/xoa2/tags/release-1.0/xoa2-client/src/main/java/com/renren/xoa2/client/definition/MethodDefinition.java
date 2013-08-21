package com.renren.xoa2.client.definition;

import java.lang.reflect.Method;

/**
 * 保存方法信息，如sharding等
 * 
 * @author yanghe.liang
 * @since 2011-12-11 下午01:46:26
 */
public class MethodDefinition {

    private Method method;

    public MethodDefinition(Method method) {
        this.method = method;
    }

    public Method getMethod() {
        return method;
    }

    public void setMethod(Method method) {
        this.method = method;
    }

}
