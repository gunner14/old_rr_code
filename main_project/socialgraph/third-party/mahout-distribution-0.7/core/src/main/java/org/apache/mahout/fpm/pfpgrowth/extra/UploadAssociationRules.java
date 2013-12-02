package org.apache.mahout.fpm.pfpgrowth.extra;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

import org.apache.commons.cli.BasicParser;
import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileStatus;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import xce.tripod.TripodException;

public class UploadAssociationRules {

    private static final Logger LOG = LoggerFactory.getLogger(UploadAssociationRules.class);
    public static final String RESULT_FILE_PREFIX = "part-r-";
    public static final int DEFAULT_NUM_THREADS = 30;
    private static FileSystem fileSystem;

    private ExecutorService service;
    private Path resultPath;
    private Stat stat;

    public UploadAssociationRules(Path resultPath, int numThreads) throws IOException {
        this.resultPath = resultPath;
        fileSystem = FileSystem.get(new Configuration());
        this.stat = this.new Stat();
        this.service = Executors.newFixedThreadPool(numThreads);
    }

    /*class Stats {

        Map<String, Stat> data = new HashMap<String, Stat>();

        public void addStat(Stat stat) {
            data.put(stat.getName(), stat);
        }

        @Override
        public String toString() {
            long firstStartTime = Long.MAX_VALUE;
            long lastEndTime = Long.MIN_VALUE;
            int numRecords = 0;
            int numSuccess = 0;
            int numFail = 0;

            for (Stat stat : data.values()) {
                if (stat.getStartTime() < firstStartTime) {
                    firstStartTime = stat.getStartTime();
                }

                if (stat.getEndTime() > lastEndTime) {
                    lastEndTime = stat.getEndTime();
                }

                numSuccess += stat.getNumSuccess();
                numFail += stat.getNumFail();
                numRecords += (stat.getNumSuccess() + stat.getNumFail());
            }

            return String.format(
                    "Total files:%d, records:%d, success:%d(%.2f%%), fail:%d, elapse:%s, avg-speed:%d records/s", data
                            .size(), numRecords, numSuccess, 100.0f * numSuccess / numRecords, numFail, CommonUtils
                            .toHumanReadableTime(lastEndTime - firstStartTime), 1000 * numRecords
                            / (lastEndTime - firstStartTime));
        }
    }*/

    class Stat {

        private static final int PROGRESS_INTERVAL = 10000;
        private String name = "default";
        private long numSuccess = 0;
        private long numFail = 0;
        private int numHandledFile = 0;
        private int numTotalFile = 0;
        private String currFile = "";
        private long startTime;
        private long endTime;
        private final Object lock = new byte[0];

        public Stat() {
        }

        public Stat(String name) {
            this.name = name;
        }

        public void incSuccess() {
            synchronized (lock) {
                numSuccess++;
            }
            reportProgress();
        }

        public void incFail() {
            synchronized (lock) {
                numFail++;
            }
            reportProgress();
        }

        public void incFile(String file) {
            currFile = file;
            numHandledFile++;
        }

        private void reportProgress() {
            long numTotal = numSuccess + numFail;
            if (numTotal % PROGRESS_INTERVAL == 0) {
                System.out
                        .println(String
                                .format(
                                        "name:%s, files:%s(%d/%d), records:%d, success:%d(%.2f%%), fail:%s, elapse:%s, speed:%d records/s%n%s",
                                        name, currFile, numHandledFile, numTotalFile, numTotal, numSuccess, 100.0f
                                                * numSuccess / numTotal, numFail, CommonUtils
                                                .toHumanReadableTime(System.currentTimeMillis() - startTime), 1000
                                                * numTotal / (System.currentTimeMillis() - startTime), CacheUtils
                                                .getSummaryStat()));
            }
        }

        @Override
        public String toString() {
            long numTotal = numSuccess + numFail;
            return String.format(
                    "name:%s, totalFiles:%d, records:%d, success:%d(%.2f%%), fail:%d, elapse:%s, speed:%d records/s",
                    name, numTotalFile, numTotal, numSuccess, 100.0f * numSuccess / numTotal, numFail, CommonUtils
                            .toHumanReadableTime(endTime - startTime), 1000 * numTotal / (endTime - startTime));
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

        public String getName() {
            return name;
        }

        public long getNumSuccess() {
            return numSuccess;
        }

        public long getNumFail() {
            return numFail;
        }

        public void setNumTotalFile(int numTotalFile) {
            this.numTotalFile = numTotalFile;
        }

        public void setName(String name) {
            this.name = name;
        }

    }

    static class UploadToCacheTask implements Runnable {

        private int userID;
        private List<AssociationRule> rules;
        private Stat stat;

        public UploadToCacheTask(int userID, List<AssociationRule> rules, Stat stat) {
            super();
            this.userID = userID;
            this.rules = rules;
            this.stat = stat;
        }

        public UploadToCacheTask(String dataStr, Stat stat) {
            String[] data = dataStr.split("\t");
            userID = Integer.parseInt(data[0]);
            rules = AssociationRule.parseRuleList(data[1]);
            this.stat = stat;
        }

        @Override
        public void run() {
            try {
                if (CacheUtils.setAssociationRuleList(userID, rules)) {
                    stat.incSuccess();
                } else {
                    stat.incFail();
                }
            } catch (TripodException e) {
                stat.incFail();
                e.printStackTrace();
            }
        }
    }

    static class RemoveCacheTask implements Runnable {

        private int userID;
        private Stat stat;

        public RemoveCacheTask(int userID, Stat stat) {
            super();
            this.userID = userID;
            this.stat = stat;
        }

        public RemoveCacheTask(String dataStr, Stat stat) {
            String[] data = dataStr.split("\t");
            userID = Integer.parseInt(data[0]);
            this.stat = stat;
        }

        @Override
        public void run() {
            if (CacheUtils.remove(userID)) {
                stat.incSuccess();
            } else {
                stat.incFail();
            }
        }
    }

    /**
     * Get all files we will handle
     * 
     * @return
     */
    private FileStatus[] getAllResultFiles() {
        List<FileStatus> result = new ArrayList<FileStatus>();
        try {
            for (FileStatus fs : fileSystem.listStatus(resultPath)) {
                if (fs.getPath().getName().startsWith(RESULT_FILE_PREFIX)) {
                    result.add(fs);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return result.toArray(new FileStatus[0]);
    }

    public void upload() {
        BufferedReader reader = null;
        stat.setName("setCache");
        stat.setStartTime(System.currentTimeMillis());
        stat.setNumTotalFile(getAllResultFiles().length);

        for (FileStatus fs : getAllResultFiles()) {
            try {
                stat.incFile(CommonUtils.getFileName(fs.getPath().getName()));
                reader = new BufferedReader(new InputStreamReader(fileSystem.open(fs.getPath())));
                String line = reader.readLine();
                while (line != null) {
                    service.execute(new UploadToCacheTask(line, stat));
                    line = reader.readLine();
                }
            } catch (IOException e) {
                e.printStackTrace();
            } finally {
                if (reader != null) {
                    try {
                        reader.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        }

        try {
            service.shutdown();
            service.awaitTermination(Long.MAX_VALUE, TimeUnit.DAYS);
            stat.setEndTime(System.currentTimeMillis());
            System.out.println(stat.toString() + "\n" + CacheUtils.getSummaryStat());
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public void remove() {
        BufferedReader reader = null;
        stat.setName("removeCache");
        stat.setStartTime(System.currentTimeMillis());
        stat.setNumTotalFile(getAllResultFiles().length);

        for (FileStatus fs : getAllResultFiles()) {
            try {
                stat.incFile(CommonUtils.getFileName(fs.getPath().getName()));
                reader = new BufferedReader(new InputStreamReader(fileSystem.open(fs.getPath())));
                String line = reader.readLine();
                while (line != null) {
                    service.execute(new RemoveCacheTask(line, stat));
                    line = reader.readLine();
                }
            } catch (IOException e) {
                e.printStackTrace();
            } finally {
                if (reader != null) {
                    try {
                        reader.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        }

        try {
            service.shutdown();
            service.awaitTermination(Long.MAX_VALUE, TimeUnit.DAYS);
            stat.setEndTime(System.currentTimeMillis());
            System.out.println(stat.toString() + "\n" + CacheUtils.getSummaryStat());
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    private static Options buildCliOptions() {
        Options options = new Options();
        options.addOption("input", "input", true, "the input path");
        options.addOption("remove", "remove", false, "remove cache");
        options.addOption("n", "numThreads", true, "number of threads");
        options.addOption("h", "help", false, "print the help message");
        return options;
    }

    private static void printHelpMsg() {
        String helpMsg = "Usage: UploadAssociationRules --input inputPath [--remove]";
        System.out.println(helpMsg);
    }

    public static void main(String[] args) throws IOException, ParseException {
        CommandLine cmd = new BasicParser().parse(buildCliOptions(), args);
        if (cmd.hasOption('h') || cmd.hasOption("help") || !cmd.hasOption("input")) {
            printHelpMsg();
            return;
        }

        Path inputPath = new Path(cmd.getOptionValue("input"));
        int numThreads = cmd.hasOption("numThreads") ? Integer.parseInt(cmd.getOptionValue("numThreads"))
                : DEFAULT_NUM_THREADS;
        if (cmd.hasOption("remove")) {
            new UploadAssociationRules(inputPath, numThreads).remove();
        } else {
            new UploadAssociationRules(inputPath, numThreads).upload();
        }
        System.exit(0);
    }
}
