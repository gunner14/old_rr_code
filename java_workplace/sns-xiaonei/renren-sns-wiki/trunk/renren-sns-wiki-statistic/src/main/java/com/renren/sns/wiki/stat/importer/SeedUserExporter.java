package com.renren.sns.wiki.stat.importer;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.concurrent.atomic.AtomicInteger;

import com.renren.sns.wiki.enums.ItemType;
import com.renren.sns.wiki.enums.WikiFeedType;
import com.renren.sns.wiki.enums.WikiType;
import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.service.WikiFeedService;
import com.renren.sns.wiki.service.WikiService;
import com.renren.sns.wiki.utils.CycleFlowUtil;
import com.renren.sns.wiki.utils.CycleFlowUtil.CycleUnit;
import com.renren.sns.wiki.utils.SpringBeanUtil;
import com.renren.sns.wiki.utils.concurrent.AsynTaskExecutorUtil;

/**
 * 导出电影类种子用户
 * 
 * @author yi.li@renren-inc.com since Aug 9, 2012
 * 
 */
public class SeedUserExporter {

    private final static Map<Integer, AtomicInteger> dashBoard = new HashMap<Integer, AtomicInteger>();

    private final static Set<Integer> wikiIds = new HashSet<Integer>();

    private static volatile int currentSize = 0;

    private static volatile boolean cycleFinished = false;

    /**
     * @param args
     */
    public static void main(String[] args) {

        CycleFlowUtil.submitCycleTask(100, -1, new CycleUnit<Integer>() {

            @Override
            public List<Integer> queryCycle(int begin, int limit) {
                WikiService wikiService = SpringBeanUtil.getInstance().getBean(WikiService.class);
                List<Wiki> wikiLs = wikiService.getWikiList(false, WikiType.MOVIE.getWikiType(),
                        begin / limit, 0, 0, limit, null);
                if (null == wikiLs || wikiLs.size() == 0) {
                    return null;
                }

                List<Integer> result = new ArrayList<Integer>();
                for (Wiki wiki : wikiLs) {
                    result.add(wiki.getId());
                }
                return result;
            }

            @Override
            public void processCycle(List<Integer> datas) {
                calculateUserScore(datas);
            }
        });

        cycleFinished = true;
    }

    private static void calculateUserScore(List<Integer> datas) {
        for (final Integer wikiId : datas) {
            System.out.println("=> begin to process " + wikiId);
            wikiIds.add(wikiId);
            AsynTaskExecutorUtil.getAsynExecutor().addTask("SeedUserExport", wikiId + "",
                    new Runnable() {

                        @Override
                        public void run() {
                            doFeedCycle(wikiId);
                        }
                    }, -1);
        }
    }

    private static void doFeedCycle(final int wikiId) {
        CycleFlowUtil.submitCycleTask(100, -1, new CycleUnit<Map<String, Object>>() {

            @Override
            public List<Map<String, Object>> queryCycle(int begin, int limit) {
                WikiFeedService feedService = SpringBeanUtil.getInstance().getBean(
                        WikiFeedService.class);
                return feedService.getRawFeed(wikiId, null, ItemType.WIHTOUT_STATUS, begin, limit,
                        WikiFeedType.HOT);
            }

            @Override
            public void processCycle(List<Map<String, Object>> datas) {
                for (Map<String, Object> feed : datas) {
                    int feedType = (Integer) feed.get("feedType");
                    if (feedType == 110) {
                        continue;
                    }
                    int userId = Integer.parseInt((String) feed.get("ownerId"));
                    increaseScore(dashBoard, userId);
                }
            }
        });
        finishTask();
    }

    private static void increaseScore(Map<Integer, AtomicInteger> dashBoard, int userId) {
        synchronized (dashBoard) {
            AtomicInteger score = dashBoard.get(userId);
            if (null == score) {
                System.out.println("==> add user " + userId);
                score = new AtomicInteger(0);
                dashBoard.put(userId, score);
            }
            score.getAndIncrement();
        }
    }

    private static void finishTask() {
        currentSize++;
        System.out.println("==> currentSize: " + currentSize + " totalSize: " + wikiIds.size());
        if (cycleFinished && wikiIds.size() <= currentSize) {
            printDashBoard();
            System.exit(0);
        }
    }

    private static void printDashBoard() {
        Set<Entry<Integer, AtomicInteger>> entries = dashBoard.entrySet();
        System.out.println("==>dash " + dashBoard.size());
        List<Entry<Integer, AtomicInteger>> records = new ArrayList<Map.Entry<Integer, AtomicInteger>>(
                entries);
        Collections.sort(records, new Comparator<Entry<Integer, AtomicInteger>>() {

            @Override
            public int compare(Entry<Integer, AtomicInteger> o1, Entry<Integer, AtomicInteger> o2) {
                if (o1.getValue().intValue() > o2.getValue().intValue()) {
                    return 1;
                } else if (o1.getValue().intValue() < o2.getValue().intValue()) {
                    return -1;
                } else {
                    return 0;
                }
            }
        });

        Collections.reverse(records);
        for (Entry<Integer, AtomicInteger> entry : records) {
            System.out.println("dashboard " + entry.getKey() + " - " + entry.getValue().intValue());
        }
    }

}
