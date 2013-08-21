package com.xiaonei.platform.core.opt.permission.strategy.impl;

import com.xiaonei.platform.core.opt.permission.AbcStrategy;
import com.xiaonei.platform.core.opt.permission.Source;
import com.xiaonei.platform.core.opt.permission.Strategy;
import com.xiaonei.platform.core.opt.permission.impl.RoleImpl;
import com.xiaonei.platform.core.opt.permission.source.Status;
import com.xiaonei.platform.core.opt.permission.strategy.Ret;

public class Admin extends AbcStrategy {

	@Override
    public Ret doStrategy(Source guester, Source source) {
		Status visiter = (Status) guester;
//		Status owner = (Status) source;
		if (RoleImpl.getInstance().isSuperAdmin(visiter)) {
            return Strategy.RET_TRUE;
        }
//This request from Tangwenjuan
//		if (owner.getStage() == Stage.STAGE_HEIGHSCHOOL) {
//			if (RoleImpl.getInstance().isAdmin(visiter)
//					&& visiter.getStage() == owner.getStage()) {
//				return this.RET_TRUE;
//			} else
//				return this.RET_CONTINUE;
//
//		}
		return RoleImpl.getInstance().isAdmin(visiter) == true ? Strategy.RET_TRUE
				: Strategy.RET_CONTINUE;
	}
}
