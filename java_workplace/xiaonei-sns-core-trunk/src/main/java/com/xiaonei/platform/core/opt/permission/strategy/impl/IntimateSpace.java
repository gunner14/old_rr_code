package com.xiaonei.platform.core.opt.permission.strategy.impl;

import com.xiaonei.platform.core.opt.permission.AbcStrategy;
import com.xiaonei.platform.core.opt.permission.Source;
import com.xiaonei.platform.core.opt.permission.Strategy;
import com.xiaonei.platform.core.opt.permission.define.RelationDef;
import com.xiaonei.platform.core.opt.permission.impl.RelationExpImpl;
import com.xiaonei.platform.core.opt.permission.source.Status;
import com.xiaonei.platform.core.opt.permission.strategy.Ret;

public class IntimateSpace extends AbcStrategy {

    public Ret doStrategy(Source guester, Source source) {


        Status visiter = (Status) guester;
        Status owner = (Status) source;
        int path = visiter.getFromToPath();
        if (path == RelationDef.notInit) append("RelationNotInit|");
        path = RelationExpImpl.getInstance().getRelationPath(guester, source);
        //System.out.println("--------from:"+visiter+"|to:"+owner+"|path:"+path) ;
        if (path == RelationDef.uuGuestRequest) {
            append("RelationGuestRequest|");
            return Strategy.RET_TRUE;
        }
        if (path == RelationDef.uuBlocked) {
            append("RelationBlocked|");
            return new Ret(false, false, this.getClassName() + ".Block");
        }

        if (path <= owner.getControl()) {
            return Strategy.RET_TRUE;
        } else {
            //            if (RelationExpImpl.getInstance().isGouYinGuo(guester, source)) {
            //                append("RelationGouYinGuo|");
            //                visiter.setFromToPath(RelationDef.uuGuestRequest);
            //                // System.out.println("--------going:"+visiter.getUserId()) ;
            //                return Strategy.RET_TRUE;
            //            }
            if (path == RelationDef.uuFriend) {
                append("RelationFriend|");
                return new Ret(false, false, this.getClassName() + ".Friend");
            }

            if (path == RelationDef.uuSchoolmate) {

                append("RelationSchoolMate|");
                return new Ret(false, false, this.getClassName() + ".Network");
            }
            if (path == RelationDef.uuSameStage) {
                append("RelationSameStage|");
                return new Ret(false, false, this.getClassName() + ".Stage");
            }
        }
        /*
        if(visiter.getStage()==owner.getControl()) {
           
        	return this.RET_TRUE ;
        
        }
        */
        return Strategy.RET_CONTINUE;
    }
}
