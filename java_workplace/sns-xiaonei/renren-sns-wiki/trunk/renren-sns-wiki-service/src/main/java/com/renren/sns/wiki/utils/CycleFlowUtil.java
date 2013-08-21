package com.renren.sns.wiki.utils;

import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * 用于执行循环操作的工具类
 * 
 * @author yi.li@renren-inc.com since Aug 6, 2012
 * 
 */
public class CycleFlowUtil {

    private static int DEFAULT_PAGE_SIZE = 50;

    private static int DEFAULT_MAX_PAGES = 20;

    private static int CONTINUOUS_EXCEPTIONS = 30;

    private static Log logger = LogFactory.getLog(CycleFlowUtil.class);

    /**
     * 提交一个cycle任务
     * 
     * @param pageSize 每一次cycle执行获取数据的最大值
     * @param maxPages cycle执行的最大次数,当maxPages小于0时，cycle会运行至没有取到数据为止
     * @param cycleUnit cycle执行中的逻辑执行单元
     */
    public static <E> void submitCycleTask(int pageSize, int maxPages, CycleUnit<E> cycleUnit) {
        int limit = pageSize;
        int currentPage = 0;
        int currentContinuousExceptions = 0;

        while (maxPages < 0 || currentPage < maxPages) {
            try {
                if (logger.isDebugEnabled()) {
                    logger.debug(String.format("Cycle query currentPage: %d pageSize: %d",
                            currentPage, limit));
                }

                if (currentContinuousExceptions > CONTINUOUS_EXCEPTIONS) {
                    logger.warn("exceed max continuous exceptions: " + currentContinuousExceptions);
                    break;
                }

                List<E> cycleResults = cycleUnit.queryCycle(currentPage * pageSize, limit);
                if (null == cycleResults || cycleResults.size() == 0) {
                    break;
                }
                cycleUnit.processCycle(cycleResults);
                currentContinuousExceptions = 0;
            } catch (Exception e) {
                logger.error("", e);
                currentContinuousExceptions++;
            }
            currentPage++;
        }
    }

    public static <E> void submitCycleTask(CycleUnit<E> task) {
        submitCycleTask(DEFAULT_PAGE_SIZE, DEFAULT_MAX_PAGES, task);
    }

    public static abstract class CycleUnit<T> {

        /**
         * 用于获取每一次循环的结果
         * 
         * @param begin 起始查询位置
         * @param limit 最大获取的数据条数
         * @return 返回查询结果
         */
        public abstract List<T> queryCycle(int begin, int limit);

        /**
         * 处理每一次循环的数据
         * 
         * @param datas
         */
        public abstract void processCycle(List<T> datas);
    }
}
