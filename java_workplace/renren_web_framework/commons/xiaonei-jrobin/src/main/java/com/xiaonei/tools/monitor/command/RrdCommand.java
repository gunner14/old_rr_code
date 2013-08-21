package com.xiaonei.tools.monitor.command;

import java.io.IOException;
import java.util.Random;

import org.jrobin.core.RrdDb;
import org.jrobin.core.RrdException;
import org.jrobin.core.Sample;

import com.xiaonei.tools.monitor.Global;
import com.xiaonei.tools.monitor.RrdDbManager;

public class RrdCommand implements Command {

    public final static Random rand = new Random();

    String rrdFile = null;

    public RrdCommand() {

    }

    /**
     * @param items
     * @param context
     * @param resultChildDir
     * @param dsName httplog 为HTTP_LOG dblog 为DB_LOG HTTP404/500为
     *        HTTPERR_LOG
     * @throws IOException
     * @throws RrdException
     */
    @Override
    public boolean execute(String[] items, CommandContext context, String dsName, String dsType,
            long startTime, long cost, String... rrdFilePaths) throws IOException, RrdException {
        RrdDbManager pool = RrdDbManager.getInstance();
        RrdDb rrdb = null;
        boolean flag = false;
        for (String rrdFilePath : rrdFilePaths) {
            this.setRrdFile(rrdFilePath);
            rrdb = pool.requestRrd(rrdFile, startTime, dsName, dsType);
            if (startTime <= rrdb.getLastUpdateTime()) continue;
            Sample sample = rrdb.createSample(startTime);
            sample.setValue(dsName, cost);
            sample.update();
            flag=true;
        }
        return flag;
    }

    /**
     * This method is used just for counting http404 and 500 happened times
     * .
     * 
     * @param items
     * @param context
     * @param dsName
     * @param dsType
     * @param startTime
     * @param rrdFilePaths
     * @throws IOException
     * @throws RrdException
     */

    public String getRrdFile() {
        return rrdFile;
    }

    public void setRrdFile(String rrdFile) {
        this.rrdFile = rrdFile;
    }

    @Override
    public void execute(String[] items, CommandContext context, String dsName, String dsType,
            long startTime, long cost, long lastUpdateTime, String... rrdFilePaths)
            throws Exception {
        // TODO Auto-generated method stub

    }

}
