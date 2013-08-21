package com.renren.sns.wiki.stat.service.fingerprint;

import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.sns.wiki.xoa.IWikiParserService;
import com.renren.sns.wiki.xoa.ParseContext;
import com.renren.sns.wiki.xoa.ParseResult;
import com.renren.sns.wiki.xoa.UGCType;
import com.renren.xoa2.client.ServiceFactory;
import com.xiaonei.admin.util.text.MD5Utils;

/**
 * 同样一段话出现相同的词，且词的顺序相同则去掉
 * 
 * @author yi.li@renren-inc.com since 2012-7-20
 * 
 */
public class MultiRelatedWordsStrategy implements IdentityStrategy {

    private FingerPrintDict<Integer, String> dict = new FingerPrintDict<Integer, String>();

    private static Map<Integer, String> significantMap = new HashMap<Integer, String>();

    static {
        significantMap.put(701, "coverImages.desc");
        //        significantMap.put(709, "title");
    }

    private Log logger = LogFactory.getLog(this.getClass());

    private List<Integer> getWikiWords(String text) {
        IWikiParserService service = ServiceFactory.getService(IWikiParserService.class);
        ParseContext ctx = new ParseContext();
        ctx.setUgcId(1);
        ctx.setUgcType(UGCType.DOING);
        List<String> txts = new ArrayList<String>();
        txts.add(text);
        ctx.setTexts(txts);

        ParseResult result = service.parseText(ctx);
        if (null == result) {
            return null;
        }

        if (logger.isDebugEnabled()) {
            logger.debug(String.format("Query result wiki_ids-%s wikiWords-%s", result.wikiIds,
                    result.wikiWords));
        }

        return result.wikiIds;
    }

    @Override
    public boolean isDuplicate(int wikiId, int feedType, Map<String, Object> feed) {
        // 暂时只处理照片新鲜事
        if (feedType != 701 && feedType != 709) {
            return false;
        }

        String text = FingerPrintUtil.getSignificantString(significantMap, feed);
        if (StringUtils.isEmpty(text)) {
            return false;
        }
        // getWikiWords
        List<Integer> ids = getWikiWords(text);
        if (null == ids || ids.size() <= 1) {
            return false;
        }

        String wikiPrint = ids.toString();
        // calculate 
        try {
            String md5 = MD5Utils.getMD5DigestBase64(wikiPrint);
            // dictionary match
            boolean result = dict.checkDuplicateAndSet(wikiId, md5);

            return result;
        } catch (NoSuchAlgorithmException e) {
            logger.error("", e);
        }

        return false;
    }

    @Override
    public String getStrategyName() {
        return "MultiRelatedWordsStrategy";
    }

    @Override
    public boolean supportFeedType(int feedType) {
        if (feedType == 701 || feedType == 709) {
            return true;
        }
        return false;
    }

}
