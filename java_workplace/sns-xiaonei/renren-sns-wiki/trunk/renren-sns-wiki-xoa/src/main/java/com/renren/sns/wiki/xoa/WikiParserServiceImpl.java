package com.renren.sns.wiki.xoa;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import xce.tripod.client.EntryType;
import xce.tripod.client.TripodCacheClient;
import xce.tripod.client.TripodCacheClientFactory;

import net.sf.json.JSONObject;

import com.renren.sns.wiki.delegator.search.MatchResult;
import com.renren.sns.wiki.delegator.search.SearchMatchDelegator;
import com.renren.sns.wiki.delegator.search.TripodCacheDelegator;
import com.renren.sns.wiki.delegator.xce.FeedLogicDelegator;
import com.renren.sns.wiki.model.DuplicatedContentInfo;
import com.renren.sns.wiki.utils.SpringBeanUtil;
import com.renren.sns.wiki.utils.WebJSONUtil;
import com.renren.sns.wiki.utils.algorithm.TextHashFingerPrintAlgorithm;
import com.renren.sns.wiki.utils.cache.CachePrefix;
import com.renren.sns.wiki.utils.concurrent.AsynTaskExecutorUtil;

/**
 * @author yi.li@renren-inc.com since 2012-4-6
 * 
 */
public class WikiParserServiceImpl implements IWikiParserService {

    private Log logger = LogFactory.getLog(WikiParserServiceImpl.class);

    private static final String EMPTY_RESULT = "{}";

    private static Map<UGCType, List<Integer>> UGC_FEED_MAP = new HashMap<UGCType, List<Integer>>();

    // UGC对应的新鲜事，在修改隐私的时候需要对应上stype
    static {
        UGC_FEED_MAP.put(UGCType.DOING, Arrays.asList(502));
        UGC_FEED_MAP.put(UGCType.BLOG, Arrays.asList(601));
        UGC_FEED_MAP.put(UGCType.ALBUM, Arrays.asList(709));
        UGC_FEED_MAP.put(UGCType.PHOTO, Arrays.asList(701, 709));
        UGC_FEED_MAP.put(UGCType.SHARE, Arrays.asList(110));
    }

    private ParseResult emptyResult() {
        ParseResult result = new ParseResult();
        List<Integer> ids = Collections.emptyList();
        result.setWikiIds(ids);
        result.setWikiWords(EMPTY_RESULT);
        return result;
    }

    private ParseResult buildResult(Map<Integer, String> map, ParseContext context) {
        ParseResult result = new ParseResult();
        List<Integer> ids = new ArrayList<Integer>(map.keySet());

        String queryStr = buildQueryStr(context);
        if (context.getUgcType() == UGCType.DOING && queryStr.contains("转自")) {
            result.setWikiIds(new ArrayList<Integer>(0));
        } else {
            result.setWikiIds(ids);
        }

        String wikiWords = WebJSONUtil.formatSingleQuoteJSON(JSONObject.fromObject(map));
        result.setWikiWords(wikiWords);

        return result;
    }

    /**
     * 获取切词服务代理
     */
    private SearchMatchDelegator getSearchService() {
        return SpringBeanUtil.getInstance().getBean(SearchMatchDelegator.class);
    }

    /**
     * 获取通用Cache服务代理
     */
    private TripodCacheDelegator getCacheDelegator() {
        return SpringBeanUtil.getInstance().getBean(TripodCacheDelegator.class);
    }

    /**
     * 这个地方要满足三个TestCase
     * <p>
     * 1) 切出词来->缓存中查到数据<br>
     * 2) 没切到词->缓存中查到"{}"<br>
     * 3) 输入空词->缓存中查到"{}"<br>
     * 
     */
    private ParseResult innerParseText(ParseContext context) {
        List<String> texts = context.getTexts();
        if (null == texts) {
            return emptyResult();
        }

        List<ParseContext> contexts = new ArrayList<ParseContext>();
        contexts.add(context);
        Map<String, ParseResult> result = multiInnerParserText(contexts);
        if (result == null || result.size() == 0) {
            setToCache(context, emptyResult());
            return emptyResult();
        }

        ParseResult pResult = result.get(context.getUgcId() + "");
        if (null == pResult) {
            setToCache(context, emptyResult());
            return emptyResult();
        }

        setToCache(context, pResult);
        return pResult;
    }

    private Map<String, ParseResult> multiInnerParserText(List<ParseContext> contexts) {
        Map<String, String> queryMap = new HashMap<String, String>();
        Map<String, ParseContext> ctxMap = new HashMap<String, ParseContext>();
        for (ParseContext context : contexts) {
            queryMap.put(context.getUgcId() + "", buildQueryStr(context));
            ctxMap.put(context.getUgcId() + "", context);
        }

        if (logger.isDebugEnabled()) {
            logger.debug("MultiInner Parse Text: " + queryMap.keySet());
        }

        Map<String, MatchResult[]> resultMap = getSearchService().matchQuery(queryMap);
        Map<String, ParseResult> parseResult = new HashMap<String, ParseResult>();
        if (null == resultMap || resultMap.size() == 0) {
            return parseResult;
        }

        for (String key : resultMap.keySet()) {
            MatchResult[] matches = resultMap.get(key);
            if (null == matches) {
                continue;
            }

            Map<Integer, String> wikiWords = new HashMap<Integer, String>();
            for (MatchResult mr : matches) {
                wikiWords.put(mr.getWikiId(), mr.getMatchString());
            }

            ParseContext context = ctxMap.get(key);
            parseResult.put(key, buildResult(wikiWords, context));
        }

        if (logger.isDebugEnabled()) {
            logger.debug("MultiInner result: " + parseResult);
        }

        return parseResult;
    }

    private String buildQueryStr(ParseContext ctx) {
        StringBuilder sb = new StringBuilder();
        for (String txt : ctx.getTexts()) {
            sb.append(txt);
        }

        return sb.toString();
    }

    @Override
    public ParseResult parseText(ParseContext context) {
        ParseResult result = this.innerParseText(context);
        filterDuplicateContent(context, result);
        return result;
    }

    /**
     * 过滤重复内容
     * 
     * @param context
     * @param result
     */
    private void filterDuplicateContent(ParseContext context, ParseResult result) {
        List<Integer> wikiIds = result.getWikiIds();
        if (null == wikiIds || wikiIds.size() == 0) {
            return;
        }

        List<String> texts = context.getTexts();
        if (null == texts || texts.size() == 0) {
            return;
        }

        if (context.getUgcType() != UGCType.BLOG && context.getUgcType() != UGCType.DOING
                && context.getUgcType() != UGCType.SHARE) {
            return;
        }

        String duplicatedStr = texts.get(0);
        if (logger.isDebugEnabled()) {
            logger.debug(String.format("String used for duplicate caculation: %s, wikiIds: %s",
                    duplicatedStr, result.getWikiIds()));
        }
        long fingerPrint = TextHashFingerPrintAlgorithm.getInstance().getFinalFingerPrint(
                duplicatedStr);

        // compose cache keys
        Map<String, Integer> cacheMap = new HashMap<String, Integer>();
        for (int wikiId : result.getWikiIds()) {
            String cacheKey = CachePrefix.WIKI_DUPLICATE_CONTENT.getPrefix()
                    + context.getUgcType().getValue() + "_" + wikiId + "_" + fingerPrint;
            cacheMap.put(cacheKey, wikiId);
        }

        TripodCacheClient cacheClient = TripodCacheClientFactory
                .getClient(EntryType.BIZ_SOCIAL_WIKI_FEED);
        List<String> keys = new ArrayList<String>(cacheMap.keySet());
        Map<String, Object> resultMap = cacheClient.multiGet(keys);

        List<Integer> dispatchWikiIds = new ArrayList<Integer>();
        for (final Entry<String, Integer> item : cacheMap.entrySet()) {
            final Object value;
            if (resultMap.containsKey(item.getKey())) {
                value = resultMap.get(item.getKey());
            } else {
                dispatchWikiIds.add(item.getValue());
                DuplicatedContentInfo dpInfo = new DuplicatedContentInfo();
                dpInfo.setOwnerId(context.getOwnerId());
                dpInfo.setUgcId(context.getUgcId());
                dpInfo.setFeedType(UGC_FEED_MAP.get(context.getUgcType()).get(0));

                value = dpInfo;
            }

            AsynTaskExecutorUtil.getAsynExecutor().addTask("DuplicateContentMark", item.getKey(),
                    new Runnable() {

                        @Override
                        public void run() {
                            TripodCacheClient cacheClient = TripodCacheClientFactory
                                    .getClient(EntryType.BIZ_SOCIAL_WIKI_FEED);
                            if (logger.isDebugEnabled()) {
                                logger.debug("expire seconds: "
                                        + CachePrefix.WIKI_DUPLICATE_CONTENT.getExpireSeconds());
                            }
                            cacheClient.set(item.getKey(), value,
                                    CachePrefix.WIKI_DUPLICATE_CONTENT.getExpireSeconds());
                        }
                    });
        }

        result.setWikiIds(dispatchWikiIds);

        if (logger.isDebugEnabled()) {
            logger.debug("After filter duplicated content, wikiIds: " + result.getWikiIds());
        }
    }

    @Override
    public void asyncUpdateParseResult(ParseContext context, String privacy) {
        if (logger.isDebugEnabled()) {
            logger.debug("Async update parse Result: type-" + context.getUgcType() + " id-"
                    + context.getUgcId());
        }

        try {
            innerParseText(context);

            // update feed data
            int int_privacy = Integer.parseInt(privacy);
            FeedLogicDelegator feedLogic = SpringBeanUtil.getInstance().getBean(
                    FeedLogicDelegator.class);
            List<Integer> stypes = UGC_FEED_MAP.get(context.getUgcType());
            if (null != stypes) {
                for (Integer stype : stypes) {
                    feedLogic.changeWikiFeedPrivacy(context.getOwnerId(), context.getUgcId(),
                            stype, int_privacy);
                }
            }
        } catch (Exception e) {
            logger.error("", e);
        }
    }

    @Override
    public void asyncReloadResult(ParseContext context) {
        if (logger.isDebugEnabled()) {
            logger.debug("Async reload parse Result: type-" + context.getUgcType() + " id-"
                    + context.getUgcId());
        }

        innerParseText(context);
    }

    private void setToCache(final ParseContext context, final ParseResult result) {
        AsynTaskExecutorUtil.getAsynExecutor().addTask("WikiWordsSetCache",
                context.getUgcType() + "_" + context.getUgcId(), new Runnable() {

                    @Override
                    public void run() {
                        getCacheDelegator().setToCache(context.getUgcType(), context.getUgcId(),
                                result.getWikiWords());
                    }
                });
    }

    @Override
    public void asyncBatchReloadResult(final List<ParseContext> contexts) {
        if (contexts.size() == 0) {
            logger.warn("AsyncBatchReload contexts size: " + contexts.size());
            return;
        }
        if (contexts.size() > 30) {
            logger.warn("query size exceed 30: " + contexts.size());
        }

        AsynTaskExecutorUtil.getAsynExecutor().addTask("WikiBatchAsyncReload",
                contexts.hashCode() + ":" + System.nanoTime(), new Runnable() {

                    @Override
                    public void run() {
                        Map<String, ParseResult> mResult = multiInnerParserText(contexts);
                        for (ParseContext ctx : contexts) {
                            ParseResult parseResult = mResult.get(ctx.getUgcId() + "");
                            if (null == parseResult) {
                                if (logger.isDebugEnabled()) {
                                    logger.debug("Batch did not get result " + ctx.getUgcId());
                                }
                                setToCache(ctx, emptyResult());
                                continue;
                            }
                            setToCache(ctx, parseResult);
                        }
                    }
                });
    }

}
