package com.xiaonei.tools.monitor;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FilenameFilter;
import java.io.IOException;
import java.text.Format;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Date;
import java.util.Iterator;
import java.util.LinkedList;

import org.apache.log4j.Logger;
import org.jrobin.core.RrdException;

import com.xiaonei.tools.monitor.command.Command;
import com.xiaonei.tools.monitor.command.CommandContext;
import com.xiaonei.tools.monitor.command.RrdCommand;

public class SqlAnalyze implements Runnable {

    public final static Format fileFormat = new SimpleDateFormat("yyyy-MM-dd-HH");

    public final static String DBLOG_PREFIX = "DataAccessMgr.";

    public final static SimpleDateFormat dateFormate = new SimpleDateFormat(
            "yyyy-MM-dd HH:mm:ss,sss");

    static Logger logger = Logger.getLogger(SqlAnalyze.class);

    private String logDir;

    private LinkedList<Command> commandChain = new LinkedList<Command>();

    private CommandContext context;

    public SqlAnalyze() {

    }

    public SqlAnalyze(String logDir) {
        this.logDir = logDir;
        context = new CommandContext();
    }

    public void addCommand(Command command) {
        if (commandChain.contains(command)) return;
        commandChain.add(command);
    }

    public String getCurFileStr() {
        Date now = new Date();
        String time = fileFormat.format(now);
        return DBLOG_PREFIX + time;
        // log.2009-07-15-10
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
                if (file.isFile() && name.startsWith(DBLOG_PREFIX)) return true;
                return false;
            }
        });
        if (files == null) {
            logger.info("Files is null,IO Error ? ");
            return offset;

        }
        if (files.length > 1) {
            Arrays.sort(files);
        }
        for (File f : files) {
            //            System.out.println(f + "aaaaaaaa");
            try {
                // if (curFileName.equals(f.getName())||offset!=0) {
                // System.out.print("Analysis "+
                // f+"  and the Offset before Parse is "+ offset);
                offset = parser(f, offset);
                // System.out.println("  and after Parse the offset is "+offset);
                // System.out.println("!curFileName.equals(f.getName()):<-->"+curFileName+"<<>>"+!curFileName.equals(f.getName()));
                // }
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

    private long parser(File file, long offset) throws IOException, RrdException {
        logger.info("Start Parser " + file.getName());
        BufferedReader bufferReader = new BufferedReader(new FileReader(file));
        bufferReader.skip(offset);
        String line = null;
        while ((line = bufferReader.readLine()) != null) {
            offset += line.length() + 1;
            // System.out.println("Offset :"+offset+":--->"+line);
            String[] items = line.split("\\|");
            if (items.length != 7) continue;
            execute(items);
        }
        bufferReader.close();
        return offset;
    }

    private void execute(String[] items) {
        Iterator<Command> iter = commandChain.iterator();
        context.setContext("logDir", logDir);
        // System.out.println("In excute ..........");
        while (iter.hasNext()) {
            Command command = iter.next();
            try {
                long startTime = Long.parseLong(items[3]) / 1000;//seconds
                long cost = Long.parseLong(items[2]);
                if (null == items[4] || "".equals(items[4])) {
                    items[4] = "unknownBiz";
                }
                String rrdFilePath = Global.JROBIN_RESULT_DIR + Global.JRD_CHILD_DIR_DB + "/"
                        + items[4] + ".rrd";// 保存的rrd文件的Path
                String sqlRrdFileName = getSqlName(items[5]);//Utils.getOutOfSpace(items[5]);
                //                String sqlRrdFileName = Utils.getOutOfSpace(items[5]);
                if (null != sqlRrdFileName && !"".equals(sqlRrdFileName)) {
//                    String rrdFilePath2 = Global.JROBIN_RESULT_DIR + Global.JRD_CHILD_DIR_DB + "/"
//                            + items[4] + "/" + sqlRrdFileName + ".rrd";// 保存的rrd文件的Path
                    command.execute(items, context, Global.DSNAME_DB, Global.DSTYPE_DB, startTime,
                            cost, rrdFilePath);
                } else {
                    command.execute(items, context, Global.DSNAME_DB, Global.DSTYPE_DB, startTime,
                            cost, rrdFilePath);
                }
            } catch (Exception ex) {
                ex.printStackTrace();
            }
        }
        context.reset();
    }

    private long mv(File file) throws IOException {
        String destDirStr = Global.DBLOG_BACK;
        File destDir = new File(destDirStr);
        if (!destDir.exists()) {
            destDir.mkdirs();
        }
        String destPath = destDir.getAbsolutePath() + "/" + file.getName();
        File dest = new File(destPath);
        boolean suceese = file.renameTo(dest);
        // logger.info("test");
        logger.info(suceese + ":Rename to ------->:" + file);
        // System.out.println("Move the file "+file+" and Init Offset to 0;");
        return 0;
    }

    public String getSqlName(String sql) {
        sql = Utils.getOutOfSpace(sql);//已经去掉多余的空格了。
        int sqlLength = sql.length();
        if (sqlLength > 254) {
            sql = cutSelectClause(sql);
            if (sql.length() > 254) {
                return null;
            }
        }
        return sql;
    }

    public String cutSelectClause(String sqlStr) {//sqlStr已经去掉了多余的空格了
        int sqlLength = sqlStr.length();
        //        int needToCut = sqlLength - 243; //243 =255-" ..cuted.. ".length -1 需要砍掉的字符数目
        int fromIndex = sqlStr.indexOf("from");//From在sql中出现第一次的位置
        int selectIndex = sqlStr.indexOf("select");//Select 在sql中出现第一次的位置
        //        int selectCanCut = fromIndex - selectIndex - 6; //6 is the length of select 
        if (fromIndex == -1 || selectIndex == -1) {
            return sqlStr;
        }
        String sqlFileName = sqlStr.substring(0, selectIndex + 6);
        //        System.out.println(sqlStr);
        sqlFileName += " ..cuted.. " + sqlStr.substring(fromIndex);
        return sqlFileName;
    }

    //    public String cut

    public void run() {
        SqlAnalyze sqlAnalyze = new SqlAnalyze(Global.DBLOG_DIR);
        sqlAnalyze.addCommand(new RrdCommand());
        long offset = 0;
        while (true) {
            Date begin = new Date();
            // System.out.println("RUN ... The offset now is "+offset);
            offset = sqlAnalyze.analyze(offset);
            Date end = new Date();
            logger
                    .info("The Analyze DB cost: " + (end.getTime() - begin.getTime())
                            + "  Milliseconds,Then rest for 1 minutes.Next DBLog Analysis begins:"
                            + offset);
            try {
                Thread.currentThread().sleep(60 * 1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }
}
