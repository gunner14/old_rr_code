package com.xiaonei.platform.core.opt.permission.strategy.impl;

import com.xiaonei.platform.core.opt.permission.AbcStrategy;
import com.xiaonei.platform.core.opt.permission.Source;
import com.xiaonei.platform.core.opt.permission.define.RelationDef;
import com.xiaonei.platform.core.opt.permission.impl.RelationExpImpl;
import com.xiaonei.platform.core.opt.permission.source.Status;
import com.xiaonei.platform.core.opt.permission.strategy.Ret;

public class BlockEachOther extends AbcStrategy {

	@Override
	public Ret doStrategy(Source guester, Source source) {
		Status visiter = (Status)guester ;
		int path =visiter.getFromToPath() ;
		if(path==RelationDef.notInit) {
			path=RelationExpImpl.getInstance().getRelationPath(guester, source) ;
		}
		if(path ==RelationDef.uuBlocked || path == RelationDef.uuBlock) {
			return new Ret(false,false,this.getClassName()+".Block") ;
		}
		return RET_CONTINUE;
	}

}
