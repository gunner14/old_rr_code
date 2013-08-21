package com.renren.sns.wiki.stat.importer;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Future;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import net.sf.json.JSONObject;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.SequenceFile;
import org.apache.hadoop.io.Text;

import com.renren.sns.wiki.stat.service.WikiWordsDictService;
import com.renren.sns.wiki.utils.SpringBeanUtil;

/**
 * 热门新鲜事导入器。将Hadoop跑出来的数据导入到热门新鲜事的DB中
 * 
 * @author yi.li@renren-inc.com since 2012-6-25
 * 
 */
public class HotFeedImporter {

    private static Log logger = LogFactory.getLog(HotFeedImporter.class);

    private static Map<Integer, Integer> stypeMap = new HashMap<Integer, Integer>();

    private Map<Integer, List<HotShareRecord>> wikiBoard = new HashMap<Integer, List<HotShareRecord>>();

    static {
        stypeMap.put(1, 601);
        stypeMap.put(2, 701);
        stypeMap.put(8, 709);
        stypeMap.put(10, 110);
    }

    //    private static int[] testwiki = { 610005623, 610006762, 610007137, 610007446 };

    public Map<Integer, List<HotShareRecord>> getWikiBoard() {
        return wikiBoard;
    }

    public void importHotFeed(String filePath, long position) {
        String fileName = filePath.substring(filePath.lastIndexOf("/") + 1);
        System.out.println(fileName);

        long lastPosition = position;

        try {
            FileSystem fs = FileSystem.getLocal(new Configuration());
            SequenceFile.Reader reader = new SequenceFile.Reader(fs, new Path(filePath),
                    new Configuration());

            // start from indicated position
            if (position > 0) {
                reader.seek(position);
            }

            LongWritable key = new LongWritable();
            Text value = new Text();

            int i = 0;
            Counter counter = new Counter();
            while (reader.next(key, value)) {
                lastPosition = reader.getPosition();
                JSONObject obj = JSONObject.fromObject(value.toString());
                Integer type = (Integer) obj.get("type");

                processRecord(obj);
                counter.doCount(stypeMap.get(type));

                i++;
                if (i % 1000 == 0) {
                    logger.info(String.format("HotFeedImporter. processing file-%s position-%d",
                            fileName, lastPosition));
                }
            }

            logStatistic(fileName + " => global", -1, counter);

        } catch (IOException e) {
            logger.warn(String.format("HotFeedImporter. interrupted file-%s position-", fileName,
                    lastPosition));
            e.printStackTrace();
        }
    }

    private void processRecord(JSONObject obj) {
        int owner = obj.getInt("resource_user_id");
        long sourceId = obj.getLong("resource_id");
        int shareCount = obj.getInt("share_count");
        int type = obj.getInt("type");
        int stype = stypeMap.get(type);

        try {
            String key_words = obj.getString("key_words");
            long albumId = 0;
            if (stype == 2) {
                albumId = obj.getLong("album_id");
            }

            List<Integer> wikiIds = getWikiIds(key_words);
            if (wikiIds.size() <= 0) {
                return;
            }

            //            long feedId = getFeedId(owner, sourceId, stype);

            for (Integer wikiId : wikiIds) {

                //                HotFeedRecord record = buildHotFeedRecord(wikiId, feedId, stype, (int) shareCount);
                HotShareRecord record = buildHotShareRecord(sourceId, owner, stype, shareCount,
                        albumId);
                addHotFeed(wikiId, record);
            }
        } catch (Exception e) {
            logger.error(String.format(
                    "HotFeedImporter. owner-%d sourceId-%d stype-%d shareCount-%d", owner,
                    sourceId, stype, shareCount), e);
        }
    }

    private void addHotFeed(int wikiId, HotShareRecord record) {
        List<HotShareRecord> feedList = wikiBoard.get(wikiId);
        if (null == feedList) {
            feedList = new ArrayList<HotShareRecord>();
            wikiBoard.put(wikiId, feedList);
        }
        feedList.add(record);
    }

    private List<Integer> getWikiIds(String keywords) {
        WikiWordsDictService dictService = SpringBeanUtil.getInstance().getBean(
                WikiWordsDictService.class);
        List<String> words = Arrays.asList(keywords.split(";"));
        return dictService.getWikiIds(words);
    }

    private HotShareRecord buildHotShareRecord(long sourceId, int owner, int stype, int shareCount,
            long albumId) {
        HotShareRecord record = new HotShareRecord();
        record.setOwnerId(owner);
        record.setSourceId(sourceId);
        record.setStype(stype);
        record.setShareCount(shareCount);
        record.setAlbumId(albumId);
        return record;
    }

    private void printStatistic() {
        printStatistic("inner", this.wikiBoard);
    }

    private static void printStatistic(String tag, Map<Integer, List<HotShareRecord>> wikiBoard) {
        Set<Integer> wikiIds = wikiBoard.keySet();
        Counter gCounter = new Counter();
        for (Integer wikiId : wikiIds) {
            List<HotShareRecord> recordList = wikiBoard.get(wikiId);
            for (HotShareRecord record : recordList) {
                gCounter.doCount(record.getStype());
            }

            sweepResult(tag + "_inner_sweep", wikiId, recordList);
        }
        logStatistic(tag + "_outer", 0, gCounter);
    }

    private static void sweepResult(String tag, int wikiId, List<HotShareRecord> recordList) {
        // extract result
        Collections.sort(recordList, new Comparator<HotShareRecord>() {

            @Override
            public int compare(HotShareRecord o1, HotShareRecord o2) {
                if (o1.getShareCount() < o2.getShareCount()) {
                    return 1;
                } else if (o1.getShareCount() > o2.getShareCount()) {
                    return -1;
                }
                return 0;
            }
        });

        Counter wCounter = new Counter();
        //begin sweep 
        for (Iterator<HotShareRecord> it = recordList.iterator(); it.hasNext();) {
            HotShareRecord item = it.next();
            if (wCounter.needSweep(item.getStype())) {
                it.remove();
                continue;
            }
            wCounter.doCount(item.getStype());
        }

        logStatistic(tag, wikiId, wCounter);
    }

    private static void logStatistic(String tag, int wikiId, Counter counter) {
        logger.info(String.format(
                "HotFeedImport DashBoard, [%s] wiki-%d photo-%d album-%d blog-%d video-%d", tag,
                wikiId, counter.getPhoto(), counter.getAlbum(), counter.getBlog(),
                counter.getVideo()));
    }

    public static void sweepAll(Map<Integer, List<HotShareRecord>> resultMap) {
        for (Entry<Integer, List<HotShareRecord>> entry : resultMap.entrySet()) {
            sweepResult("merge", entry.getKey(), entry.getValue());
        }
    }

    public static Map<Integer, List<HotShareRecord>> mergeResult(
            List<Future<Map<Integer, List<HotShareRecord>>>> futureList) {

        Map<Integer, List<HotShareRecord>> finalResult = new HashMap<Integer, List<HotShareRecord>>();

        int i = 0;
        for (Future<Map<Integer, List<HotShareRecord>>> future : futureList) {
            try {
                Map<Integer, List<HotShareRecord>> result = future.get();
                for (Entry<Integer, List<HotShareRecord>> entry : result.entrySet()) {
                    List<HotShareRecord> records = finalResult.get(entry.getKey());
                    if (null == records) {
                        finalResult.put(entry.getKey(), entry.getValue());
                    } else {
                        records.addAll(entry.getValue());
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
            i++;
            if (i % 10 == 0) {
                sweepAll(finalResult);
            }
        }
        sweepAll(finalResult);
        return finalResult;
    }

    private static void syncToDB(Map<Integer, List<HotShareRecord>> finalResult) {
        FeedTransformer tran = new FeedTransformer();
        for (int wikiId : finalResult.keySet()) {
            List<HotShareRecord> records = finalResult.get(wikiId);

            try {
                tran.cleanHotFeed(wikiId);
            } catch (Exception e) {
                logger.error("clean db", e);
            }

            int offset = 0;
            while (offset < records.size()) {
                List<HotShareRecord> syncList = records.subList(offset,
                        offset + 100 > records.size() ? records.size() : offset + 100);
                try {
                    tran.transformToFeed(syncList, wikiId);
                } catch (Exception e) {
                    logger.error("sync to db error" + wikiId, e);
                }
                offset += 100;
            }
        }
    }

    public static void main(String[] args) {
        long start = System.currentTimeMillis();
        //        int cpuCount = Runtime.getRuntime().availableProcessors();
        int cpuCount = 4;
        BlockingQueue<Runnable> queue = new LinkedBlockingQueue<Runnable>();
        ExecutorService executor = new ThreadPoolExecutor(cpuCount, cpuCount, 0,
                TimeUnit.MILLISECONDS, queue);

        String dir = System.getProperty("hotfeed.import.filepath");

        File path = new File(dir);
        if (!path.exists() || !path.isDirectory()) {
            System.out.println("Not a directory.");
            System.exit(1);
        }

        File[] files = path.listFiles();
        if (null == files || files.length == 0) {
            System.out.println("No file found.");
            System.exit(1);
        }

        List<Future<Map<Integer, List<HotShareRecord>>>> futureList = new ArrayList<Future<Map<Integer, List<HotShareRecord>>>>();
        for (File file : files) {
            final String filepath = file.getAbsolutePath();

            // submit task
            Future<Map<Integer, List<HotShareRecord>>> future = executor
                    .submit(new Callable<Map<Integer, List<HotShareRecord>>>() {

                        @Override
                        public Map<Integer, List<HotShareRecord>> call() throws Exception {
                            HotFeedImporter importer = new HotFeedImporter();

                            importer.importHotFeed(filepath, 0);
                            importer.printStatistic();

                            return importer.getWikiBoard();
                        }
                    });
            futureList.add(future);
        }

        Map<Integer, List<HotShareRecord>> finalResult = mergeResult(futureList);
        System.out.println("begin to print final result");
        printStatistic("Final", finalResult);

        long end = System.currentTimeMillis();
        syncToDB(finalResult);
        long end2 = System.currentTimeMillis();

        System.out.println("HotFeedImporter. done in " + (end - start) + "ms");
        System.out.println("HotFeedImporter. sync db done in " + (end2 - end) + "ms");
        System.exit(0);
    }
}
