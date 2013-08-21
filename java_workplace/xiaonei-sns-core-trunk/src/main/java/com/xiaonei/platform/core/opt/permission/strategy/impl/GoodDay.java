package com.xiaonei.platform.core.opt.permission.strategy.impl;

import java.util.Map;

import org.apache.commons.lang.BooleanUtils;

import com.xiaonei.platform.core.opt.permission.AbcStrategy;
import com.xiaonei.platform.core.opt.permission.Source;
import com.xiaonei.platform.core.opt.permission.Strategy;
import com.xiaonei.platform.core.opt.permission.define.RelationDef;
import com.xiaonei.platform.core.opt.permission.source.Status;
import com.xiaonei.platform.core.opt.permission.strategy.Ret;
import com.xiaonei.xce.storm.XceStormAdapter;
import com.xiaonei.xce.storm.XceStormListener;

/**
 * 重要日子增加动态可调的安全策略。 现在策略是重要日期，强制用户只可看好友页面。
 * 
 * @author zhongming.jiang@opi-corp.com
 * @date 2009-09-09
 */
public class GoodDay extends AbcStrategy {

    private static boolean flag = false;

    private static final String CACHEKEY = "admin.relationLevel";

    public GoodDay() {
        XceStormListener listener = new XceStormListenerImpl();   /****实现监听方法类***/
        XceStormAdapter.getInstance().subscribe(CACHEKEY, listener); /***注册监听方法***/
    }

    class XceStormListenerImpl extends XceStormListener {

        private static final long serialVersionUID = 1L;

        @Override
        public void handle(Map<String, String> context) {
            flag = BooleanUtils.toBoolean(context.get(CACHEKEY));
            System.out.println(context.get(CACHEKEY) + " ------------------------------------------------");
        }
    }

    @Override
    public Ret doStrategy(Source guester, Source source) {
        if (flag) {
            // 如果不可以看，查看是否是好友，不是就没有权限查看。
            Status visiter = (Status) guester;
            if (visiter.getFromToPath() > RelationDef.uuFriend) {
                return new Ret(false, false, this.getClassName());
            }
        }
        // 如果可以看，走正常的流程
        return Strategy.RET_CONTINUE;
    }

    @Override
    public String toString() {
        return this.getClass().getCanonicalName() + " the admin set flag is:" + flag;
    }

}
