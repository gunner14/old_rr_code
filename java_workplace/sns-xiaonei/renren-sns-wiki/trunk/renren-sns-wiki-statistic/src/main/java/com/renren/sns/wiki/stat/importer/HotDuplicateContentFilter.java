package com.renren.sns.wiki.stat.importer;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.apache.commons.lang.StringUtils;

import com.renren.sns.wiki.dao.WikiDAO;
import com.renren.sns.wiki.dao.dict.WikiDictionaryDAO;
import com.renren.sns.wiki.enums.WikiFeedType;
import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.model.dict.WikiWord;
import com.renren.sns.wiki.service.feed.WikiHotFeedService;
import com.renren.sns.wiki.stat.service.fingerprint.HotDuplicateContentFilterService;
import com.renren.sns.wiki.utils.CycleFlowUtil;
import com.renren.sns.wiki.utils.CycleFlowUtil.CycleUnit;
import com.renren.sns.wiki.utils.SpringBeanUtil;
import com.renren.sns.wiki.utils.concurrent.AsynTaskExecutorUtil;

/**
 * 用来过滤热门内容种重复的内容
 * <p>
 * 
 * 这里面有几个JVM的参数可以用来配置各种功能：
 * <ul>
 * <li>hotdp:只对指定的wikiId进行去重，添加该参数后，运行参数要添加对应的wikiId</li>
 * <li>filterHot:默认是对最新新鲜事进行去重，在这个参数被设置后对最热新鲜事进行去重 </li>
 * <li>noStatus:添加这个参数后，状态将不被考虑在去重范围内。这样做的好处是状态太多时可能翻20页不能很好的去除重复的照片</li>
 * <li>difflevel:这个参数可以设置图片相似度的判断指数（范围是0-64）数值越小表示相似度判断越严格。</li>
 * </ul>
 * 
 * @author yi.li@renren-inc.com since Jul 23, 2012
 * 
 */
public class HotDuplicateContentFilter {

    private static long startTime;

    private static volatile int currentFinishedCount = 0;

    private static volatile boolean cycleFinished = false;

    private static Set<Integer> totalWikiIds = new HashSet<Integer>();

    private static Object lock = new Object();

    public static void main(String args[]) {
        String hotdp = System.getProperty("hotdp");
        startTime = System.currentTimeMillis();
        if (!StringUtils.isEmpty(hotdp)) {
            int wikiId = Integer.parseInt(args[0]);

            System.out.println("Use hotdp wiki id: " + wikiId);
            totalWikiIds.add(wikiId);
            filterDuplicatedContent(new ArrayList<Integer>(totalWikiIds));
        } else {
            doCycleFilterTask();
        }

        //最后兜底
        cycleFinished = true;
        finishTask(0);
    }

    private static void doCycleFilterTask() {

        CycleFlowUtil.submitCycleTask(100, -1, new CycleUnit<Integer>() {

            @Override
            public List<Integer> queryCycle(int begin, int limit) {
                List<Integer> rawIds;
                if (isHotChoosed()) {
                    rawIds = getWikiIdsFromAllWiki(begin, limit);
                } else {
                    rawIds = getWikiIdsFromDict(begin, limit);
                }

                if (null == rawIds) {
                    return null;
                }

                List<Integer> wikiIds = new ArrayList<Integer>();
                for (Integer wikiId : rawIds) {
                    synchronized (lock) {
                        if (!totalWikiIds.contains(wikiId)) {
                            wikiIds.add(wikiId);
                            totalWikiIds.add(wikiId);
                        }
                    }
                }
                return wikiIds;
            }

            @Override
            public void processCycle(List<Integer> wikiIds) {
                filterDuplicatedContent(wikiIds);
            }
        });
    }

    private static List<Integer> getWikiIdsFromDict(int begin, int limit) {
        WikiDictionaryDAO dictDAO = SpringBeanUtil.getInstance().getBean(WikiDictionaryDAO.class);

        List<WikiWord> wikiWords = dictDAO.getWikiWords(begin, limit);
        if (null == wikiWords || wikiWords.size() == 0) {
            return null;
        }

        List<Integer> result = new ArrayList<Integer>();
        for (WikiWord word : wikiWords) {
            result.add(word.getWikiId());
        }
        return result;
    }

    private static List<Integer> getWikiIdsFromAllWiki(int begin, int limit) {
        WikiDAO dao = SpringBeanUtil.getInstance().getBean(WikiDAO.class);
        List<Wiki> wikis = dao.getWikiList(begin, limit);
        if (null == wikis || wikis.size() == 0) {
            return null;
        }

        List<Integer> result = new ArrayList<Integer>();
        for (Wiki wiki : wikis) {
            result.add(wiki.getId());
        }
        return result;
    }

    private static boolean isHotChoosed() {
        String isHot = System.getProperty("filterHot");
        if (StringUtils.isEmpty(isHot)) {
            return false;
        }

        return true;
    }

    private static void filterDuplicatedContent(List<Integer> wikiIds) {

        final WikiFeedType hotOrLatest;
        if (!isHotChoosed()) {
            hotOrLatest = WikiFeedType.LATEST;
        } else {
            hotOrLatest = WikiFeedType.HOT;
        }

        for (final Integer wikiId : wikiIds) {
            try {
                AsynTaskExecutorUtil.getAsynExecutor().addTask("DuplicateSweepTask", wikiId + "",
                        new Runnable() {

                            @Override
                            public void run() {
                                try {
                                    System.out.println("wiki id: " + wikiId);

                                    HotDuplicateContentFilterService service = SpringBeanUtil
                                            .getInstance().getBean(
                                                    HotDuplicateContentFilterService.class);
                                    service.doFilter(wikiId, hotOrLatest);
                                } finally {
                                    finishTask(wikiId);
                                }

                            }
                        }, -1);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    private static void reloadHotFeed(int wikiId) {
        try {
            if (!isHotChoosed()) {
                return;
            }
            WikiHotFeedService feedService = SpringBeanUtil.getInstance().getBean(
                    WikiHotFeedService.class);
            feedService.reloadWikiHotFeed(wikiId);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void finishTask(int wikiId) {
        if (wikiId != 0) {
            currentFinishedCount++;
            reloadHotFeed(wikiId);
        }

        int totalSize = totalWikiIds.size();
        System.out.println("Wiki " + wikiId + " finished. " + totalSize + " currentFSize: "
                + currentFinishedCount);
        if (cycleFinished && currentFinishedCount >= totalSize) {
            long endTime = System.currentTimeMillis();
            System.out.println("Task finished in " + (endTime - startTime) + " ms");
//            System.out.println("==>" + totalWikiIds);
            System.out.println("==>" + totalWikiIds.size());
            System.exit(0);
        }
    }

}
