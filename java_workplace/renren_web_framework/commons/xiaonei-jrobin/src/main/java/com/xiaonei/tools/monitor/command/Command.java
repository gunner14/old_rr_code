package com.xiaonei.tools.monitor.command;

public interface Command {

    // public void execute(String[] items, CommandContext context, String
    // dsName,long startTime,long cost,
    // String... rrdFilePaths
    public boolean execute(String[] items, CommandContext context, String dsName, String dsType,
            long startTime, long cost, String... rrdFilePaths) throws Exception;

    public void execute(String[] items, CommandContext context, String dsName, String dsType,
            long startTime, long cost, long lastUpdateTime, String... rrdFilePaths)
            throws Exception;
}
