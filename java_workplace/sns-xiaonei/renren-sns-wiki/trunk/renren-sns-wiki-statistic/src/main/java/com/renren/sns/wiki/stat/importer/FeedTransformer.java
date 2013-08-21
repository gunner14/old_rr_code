package com.renren.sns.wiki.stat.importer;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.sns.wiki.dao.feed.HotFeedDAO;
import com.renren.sns.wiki.delegator.xce.FeedRetrieverDelegator;
import com.renren.sns.wiki.model.hotfeed.HotFeedRecord;
import com.renren.sns.wiki.utils.SpringBeanUtil;
import com.xiaonei.admin.util.text.MD5Utils;
import com.xiaonei.platform.component.xfeed.display.model.Feed;
import com.xiaonei.platform.component.xfeed.display.model.FeedSet;
import com.xiaonei.xce.feed.FeedLogicAdapter;

/**
 * 用来将分享的UGC转化成为Feed
 * 
 * @author yi.li@renren-inc.com since 2012-6-27
 * 
 */
public class FeedTransformer {

    private Log logger = LogFactory.getLog(FeedTransformer.class);

    private Map<String, Set<String>> vedio_dic = new HashMap<String, Set<String>>();

    private static Pattern ykPattern = Pattern.compile("/sid/(.*)+/v.swf$");

    private HotFeedDAO getHotFeedDAO() {
        return SpringBeanUtil.getInstance().getBean(HotFeedDAO.class);
    }

    private HotFeedRecord buildHotFeedRecord(int wikiId, long feedId, int stype, int position) {
        HotFeedRecord record = new HotFeedRecord();
        record.setFeedId(feedId);
        record.setFeedType(stype);
        record.setWikiId(wikiId);
        record.setPosition(position);

        return record;
    }

    private long getFeedId(int wikiId, int owner, long sourceId, int stype) {
        long feedId = FeedLogicAdapter.getInstance().getFeedId(sourceId, stype, owner);
        if (stype == 110) {
            try {
                FeedSet feedSet = getFeedRetriverDelegator().getFeedDataById("", owner, 2,
                        new long[] { feedId });
                Feed feed = feedSet.getData()[0];
                String swfurl = feed.getData().get("f_share_video_url").get(0);

                if (logger.isDebugEnabled()) {
                    logger.debug("Share vedio url found " + swfurl);
                }

                if (null != swfurl && !("".equals(swfurl))) {
                    swfurl = extractUrl(swfurl);
                    String digital = MD5Utils.getMD5DigestBase64(swfurl);
                    Set<String> urls = vedio_dic.get(wikiId + "");
                    if (null == urls) {
                        urls = new HashSet<String>();
                        vedio_dic.put(wikiId + "", urls);
                    }

                    if (!urls.contains(digital)) {
                        urls.add(digital);
                        return feedId;
                    } else {
                        logger.info(String.format("Url already exists %d %d %d %s %s", owner,
                                sourceId, wikiId, swfurl, digital));
                        return 0;
                    }
                } else {
                    return 0;
                }
            } catch (Exception e) {
                logger.error("FeedTransform get feed id error. " + owner + " - " + sourceId, e);
            }
        }

        return feedId;
    }

    private String extractUrl(String url) {
        if (url.contains("youku")) {
            Matcher m = ykPattern.matcher(url);
            if (m.find()) {
                return m.group();
            } else {
                return url;
            }
        } else {
            return url;
        }
    }

    private FeedRetrieverDelegator getFeedRetriverDelegator() {
        return SpringBeanUtil.getInstance().getBean(FeedRetrieverDelegator.class);
    }

    /**
     * 清除热门feed
     * 
     * @param wikiId
     */
    public void cleanHotFeed(int wikiId) {
        getHotFeedDAO().clearWikiHotFeedRecord(wikiId);
    }

    /**
     * 将热门的UGC内容转成Wiki的最热新鲜事内容
     * 
     * @param records 热门分享UGC记录列表
     * @param wikiId
     */
    public void transformToFeed(List<HotShareRecord> records, int wikiId) {
        List<HotFeedRecord> feedList = new ArrayList<HotFeedRecord>();
        for (HotShareRecord record : records) {
            try {
                long feedId = getFeedId(wikiId, record.getOwnerId(), record.getSourceId(),
                        record.getStype());

                if (feedId <= 0) {
                    continue;
                }

                HotFeedRecord feedrecord = buildHotFeedRecord(wikiId, feedId, record.getStype(),
                        record.getShareCount());
                feedList.add(feedrecord);
            } catch (Exception e) {
                logger.error("get feed id error", e);
            }
        }

        logger.info(String.format(
                "Transform begin to sync to DB: wikiId-%d origin_size-%d size-%d", wikiId,
                records.size(), feedList.size()));
        getHotFeedDAO().insertWikiHotFeedRecord(feedList, wikiId);
    }

}
