package com.xiaonei.platform.core.opt.permission;

import com.xiaonei.platform.core.opt.permission.strategy.Ret;

/**
 * 权限职责抽象类 每一个继承对像完成一个职责检查。
 * 
 * @author yunlong.bai@opi-corp.com
 * 
 */
public abstract class AbcStrategy implements Strategy {

    private String className = "";

    public abstract Ret doStrategy(Source guester, Source source);

    public String getClassName() {
        return this.className;
    }

    public void setClassName(String name) {
        this.className = name;
    }

    private StringBuilder logSb = null;

    @Override
    public void setLogAppender(StringBuilder logSb) {
        this.logSb = logSb;

    }

    protected void append(String message) {
        if (logSb != null) logSb.append(message);
    }

}
