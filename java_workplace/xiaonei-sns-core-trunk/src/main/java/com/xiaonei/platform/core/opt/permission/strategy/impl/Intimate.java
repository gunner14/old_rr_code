package com.xiaonei.platform.core.opt.permission.strategy.impl;

import com.xiaonei.platform.core.opt.permission.AbcStrategy;
import com.xiaonei.platform.core.opt.permission.Source;
import com.xiaonei.platform.core.opt.permission.Strategy;
import com.xiaonei.platform.core.opt.permission.define.RelationDef;
import com.xiaonei.platform.core.opt.permission.impl.RelationExpImpl;
import com.xiaonei.platform.core.opt.permission.source.Status;
import com.xiaonei.platform.core.opt.permission.strategy.Ret;

public class Intimate extends AbcStrategy{

	public Ret doStrategy(Source guester, Source source) {
		Status visiter = (Status)guester ;
		Status owner = (Status)source ;
		int path =visiter.getFromToPath() ;
		if(path==RelationDef.notInit)
			path=RelationExpImpl.getInstance().getRelationPath(guester, source) ;
		
		if(path==RelationDef.uuGuestRequest)
			return Strategy.RET_TRUE ;
		if(path ==RelationDef.uuBlocked)
			return new Ret(false,false,this.getClassName()+".Block") ;
		if(path <=owner.getControl())
			return Strategy.RET_TRUE ;
		else{
			
			if(path==RelationDef.uuFriend)
				return new Ret(false,false,this.getClassName()+".Friend") ;
			if(path==RelationDef.uuSchoolmate)
				return new Ret(false,false,this.getClassName()+".Network") ;
			if(path==RelationDef.uuSameStage)
				return new Ret(false,false,this.getClassName()+".Stage") ;
			
			
				
		}
		/*
		if(visiter.getStage()==owner.getControl()) 
		    return this.RET_TRUE ;
		*/
		return Strategy.RET_CONTINUE ;
	}
	

}
