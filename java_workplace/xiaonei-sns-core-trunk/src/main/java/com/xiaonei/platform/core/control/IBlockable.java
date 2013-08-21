/**
 * $Id: IBlockable.java 14183 2010-01-06 11:20:53Z yan.liu@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.platform.core.control;

import java.util.concurrent.Callable;

/**
 * a runner who can be cutted.
 * 
 * @author xylz(xylz@live.cn)
 * @since 2010-1-6
 */
public interface IBlockable extends Callable<Object> {

    /**
     * You will get the 'name' message when an exception is happending.
     * 
     * @return block name
     */
    String getName();
}
