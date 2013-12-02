/**
 * @(#)Command.java, 2012-10-31. 
 * 
 * Copyright 2012 RenRen, Inc. All rights reserved.
 */
package com.renren.xcs.optool;

/**
 * @author Xun Dai <xun.dai@renren-inc.com>
 * 
 */
public interface Command {

    /**
     * 
     * @param args 命令参数
     * @throws CommandException
     */
    public void execute(String[] args) throws CommandException;
}
