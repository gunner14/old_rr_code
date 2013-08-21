package com.xiaonei.platform.core.opt.permission;

import java.util.List;

import com.xiaonei.platform.core.opt.permission.strategy.Ret;

public interface Permission {

    /**
     * 检查两个用户是否隶属同一网络
     * 
     * @param guester 访问者
     * @param owner 所有者
     * @return
     */
    public void addStrages(String action, List<Strategy> strategys);

    public Ret hasSelect(Source guester, Source source);

    public Ret hasUpdate(Source guester, Source source);

    public Ret hasInsert(Source guester, Source source);

    public Ret hasDelete(Source guester, Source source);

    public List<Strategy> getSelect();

    public List<Strategy> getInsert();

    public List<Strategy> getUpdate();

    public List<Strategy> getDelete();

}
