package com.xiaonei.platform.core.opt.permission.strategy.impl;

import com.xiaonei.platform.core.opt.permission.AbcStrategy;
import com.xiaonei.platform.core.opt.permission.Source;
import com.xiaonei.platform.core.opt.permission.Strategy;
import com.xiaonei.platform.core.opt.permission.impl.RelationExpImpl;
import com.xiaonei.platform.core.opt.permission.strategy.Ret;

public class NotSameUser extends AbcStrategy{

	public Ret doStrategy(Source guester, Source source) {
		return RelationExpImpl.getInstance().isSamer(guester.getUserId(), source.getUserId())?new Ret(false,false,this.getClassName()):Strategy.RET_CONTINUE ;
	}

}
