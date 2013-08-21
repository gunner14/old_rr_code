package com.xiaonei.platform.core.opt.permission;

import com.xiaonei.platform.core.opt.permission.strategy.Ret;

public interface Strategy {

    public static final Ret RET_CONTINUE = new Ret(true, false, "");

    public static final Ret RET_TRUE = new Ret(false, true, "");

    public abstract Ret doStrategy(Source guester, Source source);

    public String getClassName();

    public void setClassName(String name);

    public void setLogAppender(StringBuilder logSb);

}
