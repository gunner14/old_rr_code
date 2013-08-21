package com.xiaonei.platform.core.opt.permission.strategy.impl;

import com.xiaonei.platform.core.opt.permission.AbcStrategy;
import com.xiaonei.platform.core.opt.permission.Source;
import com.xiaonei.platform.core.opt.permission.Strategy;
import com.xiaonei.platform.core.opt.permission.source.Status;
import com.xiaonei.platform.core.opt.permission.strategy.Ret;

public class SameStage extends AbcStrategy{

	@Override
	public Ret doStrategy(Source guester, Source source) {
		Status visiter = (Status)guester ;
		Status owner = (Status)source ;
		if(visiter.getStage()!=owner.getStage())
			 return new Ret(false,false,this.getClassName()) ;
		return Strategy.RET_CONTINUE ;
	}

}
