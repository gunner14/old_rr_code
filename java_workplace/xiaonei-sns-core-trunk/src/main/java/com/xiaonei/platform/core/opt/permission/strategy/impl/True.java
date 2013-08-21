package com.xiaonei.platform.core.opt.permission.strategy.impl;

import com.xiaonei.platform.core.opt.permission.AbcStrategy;
import com.xiaonei.platform.core.opt.permission.Source;
import com.xiaonei.platform.core.opt.permission.Strategy;
import com.xiaonei.platform.core.opt.permission.strategy.Ret;

public class True extends AbcStrategy{
	 public Ret doStrategy(Source guester,Source source) {
		   return Strategy.RET_TRUE ;
	    	
	   }
	
}
