package com.renren.sns.wiki.stat.processor;

import java.sql.Date;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.ListIterator;
import java.util.Map;
import java.util.Set;

import com.renren.sns.wiki.model.hotfeed.HotFeedRecord;
import com.renren.sns.wiki.service.feed.WikiHotFeedService;
import com.renren.sns.wiki.utils.SpringBeanUtil;

/**
 * <p>
 * 用来统计每个wiki的最热内容。
 * <p>
 * 暂时按每小时去统计。按每小时统计，一个是信息更新速度可以满足要求，另一个是可以将db的写操作分散开来。
 * 
 * @author yi.li@renren-inc.com since 2012-4-24
 * 
 */
public class HotFeedProcessor extends AbstractProcessor {

    private Map<String, Map<String, HotFeedRecord>> scoreTable = new HashMap<String, Map<String, HotFeedRecord>>();

    private WikiHotFeedService wikiHotFeedService;

    private static final int MAX_HOT_FEED = 1200;

    private Comparator<HotFeedRecord> comparator;

    public HotFeedProcessor() {
        wikiHotFeedService = SpringBeanUtil.getInstance().getBean(WikiHotFeedService.class);
        comparator = Collections.reverseOrder(new HotFeedComparator());
    }

    @Override
    public boolean support(String logType) {
        return logType.equals("FEED_LIKE") || logType.equals("COMMENT");
    }

    @Override
    public void parseLog(String[] params) {
        String feedStr = params[5];
        String wikiStr = params[0];
        String stypeStr = params[11];
        Long feedId = 0L;
        int wikiId = 0;
        int stype = 0;
        try {
            feedId = Long.parseLong(feedStr);
            wikiId = Integer.parseInt(wikiStr);
            stype = Integer.parseInt(stypeStr);
        } catch (NumberFormatException e) {
            return;
        }
        Map<String, HotFeedRecord> wikiTable = scoreTable.get(wikiId + "");
        String feedKey = feedId + "";
        if (null == wikiTable) {
            wikiTable = new HashMap<String, HotFeedRecord>();
            scoreTable.put(wikiId + "", wikiTable);
        }

        HotFeedRecord score = wikiTable.get(feedKey);
        if (null == score) {
            score = buildHotFeedRecord(wikiId, feedId, stype);
            wikiTable.put(feedKey, score);
        }

        score.setPosition(score.getPosition() + 1);
    }

    private HotFeedRecord buildHotFeedRecord(int wikiId, long feedId, int stype) {
        HotFeedRecord record = new HotFeedRecord();
        record.setFeedId(feedId);
        record.setFeedType(stype);
        record.setWikiId(wikiId);
        record.setPosition(0);
        record.setUpdateTime(new Date(System.currentTimeMillis()));

        return record;
    }

    @Override
    public void finish() {
        logger.info("[Hot Feed statistic] begin to generating dashboard...");
        logger.info("Wiki update size: " + scoreTable.size());

        Set<String> wikiIds = scoreTable.keySet();
        for (String wikiId : wikiIds) {
            try {
                Map<String, HotFeedRecord> wikiTable = scoreTable.get(wikiId);

                int id = Integer.parseInt(wikiId);
                mergeDataToDB(id, wikiTable);
            } catch (Exception e) {
                logger.error("", e);
            }
        }

        logger.debug("Hot Feed statistic finished generating dashboard.");
    }

    private void mergeDataToDB(int wikiId, Map<String, HotFeedRecord> wikiTable) {

        List<HotFeedRecord> oldRecords = getWikiTableFromDB(wikiId);
        List<HotFeedRecord> newRecords = new ArrayList<HotFeedRecord>(wikiTable.values());
        if (oldRecords.size() != 0) {
            DecayAlgorithm alg = new DecayAlgorithm();
            for (HotFeedRecord feed : oldRecords) {
                String feedKey = feed.getFeedId() + "";
                if (wikiTable.containsKey(feedKey)) {
                    HotFeedRecord newFeed = wikiTable.get(feedKey);
                    newFeed.setPosition(feed.getPosition() + newFeed.getPosition());
                    continue;
                }

                int decayedValue = alg.decay(feed.getPosition());
                feed.setPosition(decayedValue);
                newRecords.add(feed);
            }
        }
        filterFeed(newRecords);

        logger.info(String.format(
                "[Hot Feed update] wiki-id: %d newfeed-size: %d oldfeed-size: %d merged-size: %d",
                wikiId, wikiTable.size(), oldRecords.size(), newRecords.size()));

        wikiHotFeedService.updateHotFeedRecord(newRecords, wikiId);
    }

    private List<HotFeedRecord> getWikiTableFromDB(int wikiId) {
        return wikiHotFeedService.getWikiHotFeedRecord(wikiId);
    }

    private void filterFeed(List<HotFeedRecord> records) {
        if (records.size() < MAX_HOT_FEED) {
            return;
        }

        Collections.sort(records, comparator);
        ListIterator<HotFeedRecord> itr = records.listIterator(MAX_HOT_FEED);
        while (itr.hasNext()) {
            itr.next();
            itr.remove();
        }
    }

    private static class HotFeedComparator implements Comparator<HotFeedRecord> {

        @Override
        public int compare(HotFeedRecord o1, HotFeedRecord o2) {
            if (o1.getPosition() > o2.getPosition()) {
                return 1;
            } else if (o1.getPosition() == o2.getPosition()) {
                if (o1.getUpdateTime().getTime() > o2.getUpdateTime().getTime()) {
                    return 1;
                } else if (o1.getUpdateTime().getTime() == o2.getUpdateTime().getTime()) {
                    return 0;
                } else {
                    return -1;
                }
            } else {
                return -1;
            }
        }

    }

    /**
     * 权重衰减算法（简版），按照自然数衰减，不考虑时间因素
     */
    private static class DecayAlgorithm {

        // 使用该因素可以使得频繁访问的wiki 100次喜欢的数据经过大约两周之后衰减为0
        private static final double FACTOR = 0.01;

        public int decay(int weight) {
            double d_weight = weight;
            double d_decay = d_weight * Math.pow(Math.E, FACTOR * (-1D));
            return (int) d_decay;
        }
    }

}
