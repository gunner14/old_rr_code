package com.xiaonei.platform.core.opt.permission.strategy.impl;

import com.xiaonei.platform.core.opt.permission.AbcStrategy;
import com.xiaonei.platform.core.opt.permission.Source;
import com.xiaonei.platform.core.opt.permission.Strategy;
import com.xiaonei.platform.core.opt.permission.impl.RoleImpl;
import com.xiaonei.platform.core.opt.permission.source.Status;
import com.xiaonei.platform.core.opt.permission.strategy.Ret;

public class ActiveUser extends AbcStrategy{
	 public Ret doStrategy(Source guester,Source source) {
		    Status visiter = (Status)guester ;
	    	return RoleImpl.getInstance().isActive(visiter)==true?Strategy.RET_TRUE:new Ret(false,false,this.getClassName()) ;
	    	
	   }
	
}