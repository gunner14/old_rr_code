package com.xiaonei.platform.core.opt.permission.strategy.impl;

import com.xiaonei.platform.core.opt.permission.AbcStrategy;
import com.xiaonei.platform.core.opt.permission.Source;
import com.xiaonei.platform.core.opt.permission.Strategy;
import com.xiaonei.platform.core.opt.permission.strategy.Ret;

public class StarUser extends AbcStrategy{

	public Ret doStrategy(Source guester, Source source) {
		//Status gst=(Status)guester ;
		//if(gst.getStage()==Stage.STAGE_UNIV||gst.getStage()==Stage.STAGE_HEIGHSCHOOL||gst.getStage() == Stage.STAGE_NONE)
			return Strategy.RET_CONTINUE ;
		//return RoleImpl.getInstance().isStar((Status)guester)==true?this.RET_CONTINUE: new Ret(false,false,this.getClassName()) ;
	}

}
