package com.xiaonei.platform.core.opt.permission.strategy.impl;

import com.xiaonei.platform.core.opt.permission.AbcStrategy;
import com.xiaonei.platform.core.opt.permission.Source;
import com.xiaonei.platform.core.opt.permission.Strategy;
import com.xiaonei.platform.core.opt.permission.impl.RoleImpl;
import com.xiaonei.platform.core.opt.permission.source.Status;
import com.xiaonei.platform.core.opt.permission.strategy.Ret;

public class SocietyUser extends AbcStrategy {

    public Ret doStrategy(Source guester, Source source) {
        Status owner = (Status) source;
        if (RoleImpl.getInstance().isSuicide(owner)) {
            return new Ret(false, false, this.getClassName() + ".Suicide");
        }
        if (RoleImpl.getInstance().isBanned(owner)) {
            return new Ret(false, false, this.getClassName() + ".Banned");
        }

        return Strategy.RET_CONTINUE;

    }

}
