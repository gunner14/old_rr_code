package com.renren.sns.wiki.stat.importer;

import java.util.List;
import java.util.Map;

import org.apache.commons.httpclient.HttpClient;
import org.apache.commons.httpclient.HttpMethod;
import org.apache.commons.httpclient.HttpStatus;
import org.apache.commons.httpclient.methods.GetMethod;
import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.json.JSONArray;
import org.json.JSONObject;

import com.renren.sns.wiki.enums.WikiFeedType;
import com.renren.sns.wiki.model.dict.WikiWord;
import com.renren.sns.wiki.service.WikiFeedService;
import com.renren.sns.wiki.service.dic.WikiDictionaryService;
import com.renren.sns.wiki.service.feed.WikiHotFeedService;
import com.renren.sns.wiki.utils.SpringBeanUtil;
import com.renren.sns.wiki.utils.concurrent.AsynTaskExecutorUtil;

/**
 * <p>
 * 过滤最热内容中已经不存在的内容，这部分内容可能是由于历史原因导致
 * <p>
 * 注意：在运行时需要将validateUrl中的Cookie换成可用的cookie，该实现是用httpclient去请求地址判断内容是否存在，
 * 所以需要登录的cookie
 * 
 * @author yi.li@renren-inc.com since 2012-7-16
 * 
 */
public class DeletedContentFilter {

    private static int PAGE_SIZE = 50;

    private Log logger = LogFactory.getLog(DeletedContentFilter.class);

    private WikiFeedService getWikiFeedService() {
        return SpringBeanUtil.getInstance().getBean(WikiFeedService.class);
    }

    private WikiHotFeedService getWikiHotFeedService() {
        return SpringBeanUtil.getInstance().getBean(WikiHotFeedService.class);
    }

    private String buildUrl(int ownerId, long sourceId, int feedType) {
        String url = null;
        switch (feedType) {
            case 601:
                url = "http://blog.renren.com/blog/" + ownerId + "/" + sourceId; // 404
                break;
            case 709:
                url = "http://photo.renren.com/photo/" + ownerId + "/album-" + sourceId;
                break;
            case 701:
                url = "http://photo.renren.com/photo/" + ownerId + "/photo-" + sourceId;
                break;
            case 110:
                url = "http://share.renren.com/share/" + ownerId + "/" + sourceId; // 404
                break;
            default:
                break;
        }

        return url;
    }

    /**
     * 按WikiId来过滤掉不存在的内容
     * 
     * @param wikiId
     */
    private void doFilter(final int wikiId) {
//        int startPage = 0;
//        boolean hasMore = true;
//        int exceptionCount = 0;
//        while (hasMore) {
//            try {
//                Map<String, Object> feedObj = getWikiFeedService().getFeedMap(wikiId, null, null,
//                        startPage, PAGE_SIZE, WikiFeedType.HOT);
//                if (logger.isDebugEnabled()) {
//                    logger.debug("Query JSON result: " + feedObj.toString());
//                }
//
//                List feeds = (List)feedObj.get("feeds");
//
//                logger.info(String.format("SweepQuery wikiId-%d len-%d", wikiId, feeds.length()));
//                for (int i = 0; i < feeds.length(); i++) {
//                    JSONObject feed = feeds.getJSONObject(i);
//                    int feedType = feed.getInt("feedType");
//                    int ownerId = feed.getInt("ownerId");
//                    long sourceId = feed.getLong("sourceId");
//                    final long feedId = feed.getLong("feedId");
//                    final String url = buildUrl(ownerId, sourceId, feedType);
//                    if (StringUtils.isEmpty(url)) {
//                        logger.warn(String.format(
//                                "Url is empty feedType-%d ownerId-%d sourceId-%d feedId-%d",
//                                feedType, ownerId, sourceId, feedId));
//                    } else {
//                        AsynTaskExecutorUtil.getAsynExecutor().addTask("Sweep404Url",
//                                feedId + "-" + wikiId, new Runnable() {
//
//                                    @Override
//                                    public void run() {
//                                        validateUrl(url, feedId, wikiId);
//                                    }
//                                });
//                    }
//                }
//
//                hasMore = Boolean.valueOf(feedObj.getString("hasMore"));
//                startPage++;
//                exceptionCount = 0;
//            } catch (Exception e) {
//                logger.error("", e);
//                exceptionCount++;
//                if (exceptionCount > 3) {
//                    startPage++;
//                }
//            }
//        }
    }

    private void validateUrl(String url, long feedId, int wikiId) {
        HttpClient client = new HttpClient();
        HttpMethod method = new GetMethod(url);

        //设置cookie，运行时需要修改这个值，这不是一个好的做法，但是这样最快。
        method.setRequestHeader(
                "Cookie",
                "_r01_=1; mop_uniq_ckid=127.0.0.1_1328601716_1398670243; __utma=10481322.1392776321.1342425884.1342425884.1342429153.2; __utmz=10481322.1342425884.1.1.utmcsr=(direct)|utmccn=(direct)|utmcmd=(none); anonymid=h0yn2tux-aryirv; l4pager=0; copyLinkTip20100311=0; depovince=BJ; jebecookies=3454eb4b-6705-4977-82fb-78ff6bf8b924|||||; ick_login=a7c1d5e6-637b-4517-b6d4-31d997389415; loginfrom=syshome; wiki_admin_t=6e60fe64af224d3e9d1c250bf919ef4c; XNESSESSIONID=53379ac1c5e2; JSESSIONID=abcecNIQOefqc7rzaKoIt; __utmc=10481322; ick=75f837a9-74ad-4cf3-8184-c16fca851a1f; idc=tel; _de=ECD58450FB48DA19C5FD3A0EF6808BF35212E40F3D18115C; p=ac3d5a0c3f5afef2b152d34877d763255; first_login_flag=1; t=bf0f0a410618485ba513dc14de4a3f485; societyguester=bf0f0a410618485ba513dc14de4a3f485; id=365323815; xnsid=70dc224f");

        try {
            if (logger.isDebugEnabled()) {
                logger.debug(String.format("httpclient request url-%s feedId-%d wikiId-%d", url,
                        feedId, wikiId));
            }

            boolean delete = false;
            int code = client.executeMethod(method);
            if (logger.isDebugEnabled()) {
                logger.debug(String.format(
                        "httpclient code url-%s feedId-%d wikiId-%d code-%d content:%s", url,
                        feedId, wikiId, code, new String(method.getResponseBody())));
            }

            if (code == HttpStatus.SC_NOT_FOUND) {
                delete = true;
            } else if (code == HttpStatus.SC_OK) {
                String body = new String(method.getResponseBody());
                if (body.contains("你点击的链接无法显示")) {
                    delete = true;
                }
            }

            if (delete) {
                logger.warn(String.format("content not exists wikiId-%d feedId-%d url-%s", wikiId,
                        feedId, url));
                getWikiHotFeedService().deleteHotFeedRecord(wikiId, feedId);
            }
        } catch (Exception e) {
            logger.error(
                    String.format("Validate url url-%s feedId-%d wikiId-%d", url, feedId, wikiId),
                    e);
        }

    }

    public static void main(String args[]) {
        WikiDictionaryService service = SpringBeanUtil.getInstance().getBean(
                WikiDictionaryService.class);
        List<WikiWord> wikiWords = service.getWikiWordList(0, 0, 1, 500);

        for (WikiWord word : wikiWords) {
            System.out.println("Begin to process wiki: " + word.getWikiId());
            DeletedContentFilter filter = new DeletedContentFilter();
            filter.doFilter(word.getWikiId());

            try {
                //缓冲一下，避免很快将异步队列跑满
                Thread.sleep(2000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
