/**
 * $Id: SnsBlockCutInvocationHandler.java 15289 2010-01-19 07:28:09Z yan.liu@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.sns.platform.core.opt.ice.impl;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;

import org.apache.commons.lang.builder.ToStringBuilder;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.platform.core.control.BlockCounter;
import com.xiaonei.platform.core.control.BlockCounterFactory;
import com.xiaonei.platform.core.control.BlockException;
import com.xiaonei.platform.core.control.BlockExecutor;
import com.xiaonei.platform.core.control.ConfigCenter;
import com.xiaonei.platform.core.control.MethodInvocationBlockable;
import com.xiaonei.platform.core.control.TimeCostUtil;

/**
 * Sns Block Cutter proxy
 * 
 * @author xylz(xylz@live.cn)
 * @since 2009-12-31
 */
public class SnsBlockCutInvocationHandler implements InvocationHandler {

    private static Log log = LogFactory.getLog(SnsAdapterFactory.class);

    private static BlockCounterFactory bcf = BlockCounterFactory.getInstance();
    final BlockExecutor blockExcutor = new BlockExecutor();

    String name;

    Object obj;

    public SnsBlockCutInvocationHandler(String name, Object obj) {
        this.name = name;
        this.obj = obj;
    }

    public Object invoke(Object proxy, final Method method, final Object[] args) throws Throwable {
        if (method.getDeclaringClass() == Object.class) return method.invoke(obj, args);
        if (ConfigCenter.getBoolean("debug_snsadapter", false)) {
            System.out.println("block->" + bcf.getBlockCounter(name));
        }
        if (bcf.enter(name)) {
            //统计单独出来
            long begin = TimeCostUtil.logBegin();
            Object ret=null;
            try {
                ret= blockExcutor.run(new MethodInvocationBlockable(method, obj, args, name));
            } finally {//clear the counter
                bcf.outer(name);
                if(log.isDebugEnabled()) {
                    log.debug(String.format("<%s> execute method[%s] return:%s", name,method.getName(),ToStringBuilder.reflectionToString(ret)));
                }
                TimeCostUtil.logEnd(begin, name, method.getName());
            }
            return ret;
        } else {// out of limited? throw exception
            BlockCounter bc = new BlockCounter(bcf.getBlockCounter(name));//not use clone!
            log.error(bc + " BLOCKED!");
            throw new BlockException("!XCE BLOCKED:" + bc);
        }

    }

}
