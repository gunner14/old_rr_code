package com.xiaonei.tools.monitor;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FilenameFilter;
import java.io.IOException;
import java.sql.SQLException;
import java.text.Format;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;

import org.apache.log4j.Logger;
import org.jrobin.core.RrdException;

import com.xiaonei.tools.monitor.command.Command;
import com.xiaonei.tools.monitor.command.CommandContext;
import com.xiaonei.tools.monitor.command.HttpErrDBCommand;
import com.xiaonei.tools.monitor.command.IMDBCommand;
import com.xiaonei.tools.monitor.command.RrdCommand;
import com.xiaonei.tools.monitor.entity.HttpErrEntity;

public class HttpErrAnalyze implements Runnable {

    public final static SimpleDateFormat timeFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");

    private final static String LOGPREFIX = "404.log";

    private final int LOGARRLEN = 6;

    static Logger logger = Logger.getLogger(HttpErrAnalyze.class);

    private String logDir;

    private LinkedList<Command> commandChain = new LinkedList<Command>();

    private CommandContext context;

    private LinkedList<HttpErrEntity> httpErrList = new LinkedList<HttpErrEntity>();

    private HashMap<String, Long> counter = new HashMap<String, Long>();//

    private String latestDBTimeStr = null;

    HttpErrAnalyze() {
    }

    private HttpErrAnalyze(String logDir) {
        this.logDir = logDir;
        context = new CommandContext();
    }

    public void addCommand(Command command) {
        if (commandChain.contains(command)) return;
        commandChain.add(command);
    }

    public String getCurFileStr() {
        return LOGPREFIX;
    }

    public long analyze(long offset) {// offset是文件的偏移量
        String curFileName = getCurFileStr();
        File root = new File(logDir);
        if (!root.exists() || !root.isDirectory()) {
            logger.info(logDir + " not exists");
            System.exit(0);
        }
        File[] files = root.listFiles(new FilenameFilter() {

            public boolean accept(File dir, String name) {
                File file = new File(dir.getPath() + "/" + name);
                if (file.isFile() && name.startsWith(LOGPREFIX)) return true;
                return false;
            }
        });
        if (files == null) {
            logger.info("Files is null,IO Error ? ");
            return offset;

        }
        if (files.length > 1) {
            Arrays.sort(files);
            changeOrder(files);
        }
        for (File f : files) {
            try {
                offset = parser(f, offset);
                if (!curFileName.equals(f.getName())) {
                    offset = mv(f);// 移动文件以后就将offset 标记为0
                }
                // return offset;
            } catch (IOException e) {
                e.printStackTrace();
            } catch (RrdException e) {
                e.printStackTrace();
            }
        }
        return offset;// 只会有一个offset
    }

    private File[] changeOrder(File[] files) {
        File curFile = files[0];//404.log
        int length = files.length;
        for (int i = 0; i < length - 1; i++) {
            files[i] = files[i + 1];
        }
        files[length - 1] = curFile;
        return files;
    }

    private long parser(File file, long offset) throws IOException, RrdException {
        logger.info("Start Parser " + file.getName());
        BufferedReader bufferReader = new BufferedReader(new FileReader(file));
        bufferReader.skip(offset);
        String line = null;
        while ((line = bufferReader.readLine()) != null) {
            offset += line.length() + 1;
            String[] items = line.split(" ");
            if (items.length != LOGARRLEN) continue;
            execute(items);
        }
        bufferReader.close();
        return offset;
    }

    private long getHappenTime(String timeStr) throws ParseException {
        Date date = timeFormat.parse(timeStr);
        return date.getTime() / 1000;
    }

    private String getPrefix(HttpErrEntity httpErrEntity) {
        if (null == httpErrEntity || "".equals(httpErrEntity)) {
            return null;
        }
        if (httpErrEntity.getCurUrl().contains("xiaonei.com")) {
            return "xiaonei";
        } else if (httpErrEntity.getCurUrl().contains("kaixin.com")) {
            return "kaixin";
        }
        if (httpErrEntity.getCurUrl().contains("renren.com")) {
            return "renren";
        }
        return null;
    }

    private Long getCounter(String key) {
        if (counter.get(key) == null) {
            counter.put(key, 1l);
            return 1l;
        } else {
            long count = counter.get(key);
            counter.put(key, ++count);
            return count;
        }

    }

    private boolean needSaveToDB(String timeStr) throws SQLException, ParseException {
        if (latestDBTimeStr == null) {
            latestDBTimeStr = HttpErrDBCommand.getDBLatestTime();
            if (latestDBTimeStr == null) {
                latestDBTimeStr = timeStr;
                return true;
            }
        }
        long latestDBTimeLong = getTime(latestDBTimeStr);
        long timeStrLong = getTime(timeStr);
        if (timeStrLong < latestDBTimeLong) {
            return false;
        } else if (timeStrLong == latestDBTimeLong) {
            return true;
        } else {
            latestDBTimeStr = timeStr;
            return true;
        }
    }

    private long getTime(String timeStr) throws ParseException {

        return timeFormat.parse(timeStr).getTime();
    }

    private void execute(String[] items) {
        Iterator<Command> iter = commandChain.iterator();
        context.setContext("logDir", logDir);
        while (iter.hasNext()) {
            Command command = iter.next();
            try {
                //                HttpErrEntity httpErrEntity = new HttpErrDBCommand().excute(items, httpErrList);
                HttpErrDBCommand dbcommand = new HttpErrDBCommand();
                HttpErrEntity httpErrEntity = dbcommand.getHttpErrEntity(items);
                String rrdFilePre = getPrefix(httpErrEntity);
                if (null != httpErrEntity && rrdFilePre != null && !rrdFilePre.equals("")) {
                    long startTime = getHappenTime(httpErrEntity.getHappernTime());
                    Long counter = getCounter(rrdFilePre + httpErrEntity.getErrorType());//causion
                    String rrdFilePath = Global.JROBIN_RESULT_DIR + Global.JRD_CHILD_DIR_HTTPERR
                            + "/" + rrdFilePre + httpErrEntity.getErrorType() + ".rrd";// 保存的rrd文件的Path
                    command.execute(items, context, Global.DSNAME_HTTPERR, Global.DSTYPE_HTTPERR,
                            startTime, counter, rrdFilePath);
                    if (needSaveToDB(httpErrEntity.getHappernTime())) {
                        dbcommand.excute(httpErrEntity, httpErrList);
                    }
                }
            } catch (Exception ex) {
                ex.printStackTrace();
            }
        }
        context.reset();
    }

    private long mv(File file) throws IOException {
        String destDirStr = Global.HTTPERRLOG_BACK;
        File destDir = new File(destDirStr);
        if (!destDir.exists()) {
            destDir.mkdirs();
        }
        String destPath = destDir.getAbsolutePath() + "/" + file.getName();
        File dest = new File(destPath);
        boolean suceese = file.renameTo(dest);
        logger.info(suceese + ":Rename to ------->:" + file);
        return 0;
    }

    public void run() {
        HttpErrAnalyze http = new HttpErrAnalyze(Global.HTTPERRLOG_DIR);
        http.addCommand(new RrdCommand());
        long offset = 0;
        while (true) {
            Date begin = new Date();
            offset = http.analyze(offset);
            Date end = new Date();
            logger.info("The Analyze HTTP404/500 cost: " + (end.getTime() - begin.getTime())
                    + "  Milliseconds,Then rest for 1 minutes.Next Analysis begins:" + offset);
            try {
                Thread.currentThread().sleep(60 * 1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }
}
