/**
 * @(#)AbstractCommand.java, 2012-10-31. 
 * 
 * Copyright 2012 RenRen, Inc. All rights reserved.
 */
package com.renren.xcs.optool;

import com.renren.ad.datacenter.find.optool.OPTool;

/**
 * @author Xun Dai <xun.dai@renren-inc.com>
 * 
 */
public abstract class AbstractCommand implements Command {

    private OPTool optool;

    /**
     * @throws Exception
     * 
     */
    public AbstractCommand(OPTool optool) {
        this.optool = optool;
    }
}
