package org.apache.mahout.fpm.pfpgrowth.extra;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Reader;
import java.io.Writer;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

import org.apache.commons.cli.BasicParser;
import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.Options;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.KeyValue;
import org.apache.hadoop.hbase.client.Get;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.client.Result;
import org.apache.hadoop.hbase.util.Bytes;

public class ExportFriendList {

    public static final String DEFAULT_TABLE_NAME = "relation_20120825";
    public static final int DEFAULT_NUM_THREADS = 30;
    public static final String SEP = " ";
    public static final int REPORT_INTERVAL = 1000;
    public static final int DEFAULT_NUM_FRIENDS_T = 500;
    public static final int NUM_SPLIT = 10;

    private ExecutorService service;
    private int numThreads = DEFAULT_NUM_THREADS;
    private String tableName = DEFAULT_TABLE_NAME;
    private String userIDFile;
    private String friendListFile;
    private Object lock = new byte[0];
    private Stat stat;
    private long totalRecords = -1;
    private int numFriendsThreshold = DEFAULT_NUM_FRIENDS_T;

    public ExportFriendList() {
    }

    public ExportFriendList(String userIDFile, String friendListFile) {
        super();
        this.stat = this.new Stat();
        this.userIDFile = userIDFile;
        this.friendListFile = friendListFile;
        service = Executors.newFixedThreadPool(numThreads);
    }

    public ExportFriendList(String userIDFile, String friendListFile, int numThreads, String tableName,
            long totalRecords, int numFriendsThreshold) {
        super();
        this.stat = this.new Stat();
        this.userIDFile = userIDFile;
        this.friendListFile = friendListFile;
        this.numThreads = numThreads;
        service = Executors.newFixedThreadPool(numThreads);
        this.tableName = tableName;
        this.totalRecords = totalRecords;
        this.numFriendsThreshold = numFriendsThreshold;
    }

    class Stat {

        private long records;
        private long startTime;
        private long endTime;

        public Stat() {
            records = 0;
        }

        public void inc() {
            records++;
            reportProgress();
        }

        private void reportProgress() {
            if (records % REPORT_INTERVAL == 0) {
                if (totalRecords != -1) {
                    float percent = (100.0f * records) / totalRecords;
                    CommonUtils.printWithTS((String.format("records:%d, elapse:%s, speed:%d records/s, percent:%.1f%%",
                            records, CommonUtils.toHumanReadableTime(System.currentTimeMillis() - startTime), records
                                    * 1000 / (System.currentTimeMillis() - startTime), percent)));
                } else {
                    CommonUtils.printWithTS((String.format("records:%d, elapse:%s, speed:%d records/s", records,
                            CommonUtils.toHumanReadableTime(System.currentTimeMillis() - startTime), records * 1000
                                    / (System.currentTimeMillis() - startTime))));
                }
            }
        }

        @Override
        public String toString() {
            return String.format("records:%d, elapse:%s, speed:%d records/s", records, CommonUtils
                    .toHumanReadableTime(endTime - startTime), records * 1000 / (endTime - startTime));
        }

        public long getStartTime() {
            return startTime;
        }

        public void setStartTime(long startTime) {
            this.startTime = startTime;
        }

        public long getEndTime() {
            return endTime;
        }

        public void setEndTime(long endTime) {
            this.endTime = endTime;
        }
    }

    static class ExportTask implements Runnable {

        private String userID;
        private HTable table;
        private FileWriter writer;
        private Object lock;
        private Stat stat;
        private int numFriendT;

        public ExportTask(String userID, HTable table, FileWriter writer, Object lock, Stat stat, int numFriendsT) {
            this.userID = userID;
            this.table = table;
            this.writer = writer;
            this.lock = lock;
            this.stat = stat;
            this.numFriendT = numFriendsT;
        }

        @Override
        public void run() {
            try {
                Result result = table.get(new Get(Bytes.toBytes(userID)));
                int numFriends = 0;
                StringBuffer sb = new StringBuffer(this.userID);
                for (KeyValue kv : result.raw()) {
                    /* getKey() method is deprecated */
                    String userID = Bytes.toString(kv.getQualifier());
                    if (valid(userID) && !userID.equals(this.userID)) {
                        sb.append(SEP + userID);
                        numFriends++;
                    }
                }
                sb.append("\n");

                if (numFriends > 0 && numFriends <= numFriendT) {
                    synchronized (lock) {
                        writer.write(sb.toString());
                        stat.inc();
                    }
                }

            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    /**
     * Check whether this is a valid userID
     * 
     * @param raw
     * @return
     */
    private static boolean valid(String raw) {
        boolean valid = false;
        try {
            int userID = Integer.parseInt(raw);
            if (userID > 0) {
                valid = true;
            }
        } catch (NumberFormatException e) {}

        return valid;
    }

    private void close(Reader reader, Writer[] writers) {
        if (reader != null) {
            try {
                reader.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        if (writers != null) {
            for (Writer writer : writers) {
                try {
                    writer.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

    }

    public void export() {
        BufferedReader reader = null;
        FileWriter[] writers = null;
        try {
            HTable table = new HTable(HBaseConfiguration.create(), tableName);
            reader = new BufferedReader(new FileReader(new File(userIDFile)));
            // initialize file writers
            writers = new FileWriter[NUM_SPLIT];
            for (int i = 0; i < NUM_SPLIT; i++) {
                writers[i] = new FileWriter(friendListFile + "." + i);
            }
            stat.setStartTime(System.currentTimeMillis());

            String userID = reader.readLine();
            while (userID != null) {
                if (valid(userID)) {
                    service.execute(new ExportTask(userID.trim(), table, writers[Integer.parseInt(userID) % NUM_SPLIT],
                            lock, stat, numFriendsThreshold));
                } else {
                    System.err.println(String.format("Skip bad record:%s", userID));
                }
                userID = reader.readLine();
            }

            service.shutdown();
            service.awaitTermination(Long.MAX_VALUE, TimeUnit.DAYS);
            stat.setEndTime(System.currentTimeMillis());
            CommonUtils.printWithTS(stat.toString());
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            close(reader, writers);
        }
    }

    public void setTotalRecords(long totalRecords) {
        this.totalRecords = totalRecords;
    }

    private static Options buildCliOptions() {
        Options options = new Options();
        options.addOption("u", "userIDFile", true, "user ID file");
        options.addOption("f", "friendListFile", true, "friend ID list file");
        options.addOption("t", "tableName", true, "htable name");
        options.addOption("n", "numThreads", true, "thread pool size");
        options.addOption("r", "totalRecords", true, "number of total records");
        options.addOption("nf", "numFriendsThreshold", true, "number of friends threshold");
        options.addOption("h", "help", false, "print the help message");
        return options;
    }

    private static void printHelpMsg() {
        String helpMsg = "Usage: ExportFriendList userIDFile friendListFile [numThreads] [tableName] [numFriendsThreshold]";
        System.out.println(helpMsg);
    }

    public static void main(String[] args) throws Exception {
        CommandLine cmd = new BasicParser().parse(buildCliOptions(), args);
        if (cmd.hasOption('h') || cmd.hasOption("help")) {
            printHelpMsg();
            return;
        }
        if (!cmd.hasOption("userIDFile") || !cmd.hasOption("friendListFile")) {
            printHelpMsg();
            return;
        }

        String userIDFile = cmd.getOptionValue("userIDFile");
        String friendListFile = cmd.getOptionValue("friendListFile");
        int numThreads = cmd.getOptionValue("numThreads") != null ? Integer.parseInt(cmd.getOptionValue("numThreads"))
                : DEFAULT_NUM_THREADS;
        String tableName = cmd.getOptionValue("tableName") != null ? cmd.getOptionValue("tableName")
                : DEFAULT_TABLE_NAME;
        long totalRecords = cmd.getOptionValue("totalRecords") != null ? Long.parseLong(cmd
                .getOptionValue("totalRecords")) : -1;
        int numFriendsThreshold = cmd.getOptionValue("numFriendsThreshold") != null ? Integer.parseInt(cmd
                .getOptionValue("numFriendsThreshold")) : DEFAULT_NUM_FRIENDS_T;

        new ExportFriendList(userIDFile, friendListFile, numThreads, tableName, totalRecords, numFriendsThreshold)
                .export();
        System.exit(0);
    }

}
