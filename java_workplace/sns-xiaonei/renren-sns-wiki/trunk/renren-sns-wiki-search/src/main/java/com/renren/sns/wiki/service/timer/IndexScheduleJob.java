package com.renren.sns.wiki.service.timer;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.quartz.JobExecutionContext;
import org.quartz.JobExecutionException;
import org.springframework.scheduling.quartz.QuartzJobBean;

import com.renren.sns.wiki.biz.event.IndexChangeEvent;
import com.renren.sns.wiki.biz.search.WikiSearchBiz;
import com.renren.sns.wiki.utils.event.EventUtil;

/**
 * 用来定时构建Wiki索引
 * 
 * @author yi.li@renren-inc.com since Aug 16, 2012
 * 
 */
public class IndexScheduleJob extends QuartzJobBean {

    private WikiSearchBiz wikiSearchBiz;

    private Log logger = LogFactory.getLog(this.getClass());

    public void setWikiSearchBiz(WikiSearchBiz wikiSearchBiz) {
        this.wikiSearchBiz = wikiSearchBiz;
    }

    @Override
    protected void executeInternal(JobExecutionContext context) throws JobExecutionException {
        long start = System.currentTimeMillis();
        try {
            logger.info("begin to rebuild index");
            wikiSearchBiz.rebuildIndex();

            EventUtil.getEventManager().dispatchEvent(new IndexChangeEvent());
        } catch (Exception e) {
            logger.error("", e);
        } finally {
            long end = System.currentTimeMillis();
            logger.info("finish rebuild index in " + (end - start) + " ms");
        }

    }

}
