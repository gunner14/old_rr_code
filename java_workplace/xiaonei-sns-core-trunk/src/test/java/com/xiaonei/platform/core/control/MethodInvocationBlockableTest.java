/**
 * $Id: MethodInvocationBlockableTest.java 15136 2010-01-18 08:27:24Z yan.liu@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.platform.core.control;

import static org.junit.Assert.*;

import org.junit.Test;


/**
 * @author xylz(xylz@live.cn)
 * @since 2010-1-18
 */
public class MethodInvocationBlockableTest {

    /**
     * Test method for {@link com.xiaonei.platform.core.control.MethodInvocationBlockable#toArguments()}.
     */
    @Test
    public void testToArguments() {
        MethodInvocationBlockable mib = new MethodInvocationBlockable(null, null, new Object[] {}, "test");
        assertEquals("", mib.toArguments());
        //
        MethodInvocationBlockable mib2 = new MethodInvocationBlockable(null, null, new Object[] {"abc"}, "test");
        assertEquals("abc", mib2.toArguments());
        //
        MethodInvocationBlockable mib3 = new MethodInvocationBlockable(null, null, new Object[] {"abc",123}, "test");
        assertEquals("abc,123", mib3.toArguments());
    }
    @Test
    public void testFormatLog() throws Exception{
        MethodInvocationBlockable mib = new MethodInvocationBlockable(String.class.getMethod("indexOf", new Class[] {String.class}), "Hello, World", new Object[] {"World"}, "hello");
        assertEquals("service timeout:hello (51/50:1) indexOf [World] @_@",mib.formatLog(51, 50));
    }

}
