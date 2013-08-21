/**
 * 
 */
package com.xiaonei.platform.core.opt.permission.strategy.impl;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.permission.AbcStrategy;
import com.xiaonei.platform.core.opt.permission.Source;
import com.xiaonei.platform.core.opt.permission.Strategy;
import com.xiaonei.platform.core.opt.permission.define.RelationDef;
import com.xiaonei.platform.core.opt.permission.source.Status;
import com.xiaonei.platform.core.opt.permission.strategy.Ret;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

/**
 * @author lookis (comic.liu@gmail.com)
 * @author tai.wang@opi-corp.com Dec 1, 2010 - 4:13:29 PM
 */
public class OperationBan extends AbcStrategy {

    public OperationBan() {
    }

    @Override
    public Ret doStrategy(Source guester, Source source) {
        User user = SnsAdapterFactory.getUserAdapter().get(source.getUserId());
        if (user.getUserDangerInfo().isOnlySeenBySelf()) {
            //如果只有自己可看
            Status visiter = (Status) guester;
            if (visiter.getFromToPath() <= RelationDef.uuSelf) {
                return Strategy.RET_TRUE;
            } else {
                return new Ret(false, false, this.getClassName());
            }
        } else if (user.getUserDangerInfo().isOnlySeenByFriend()) {
            // 如果只有好友可看
            Status visiter = (Status) guester;
            if (visiter.getFromToPath() <= RelationDef.uuFriend) {
                return Strategy.RET_TRUE;
            } else {
                return new Ret(false, false, this.getClassName());
            }
        }
        // 走正常的流程
        return Strategy.RET_CONTINUE;
    }

}
