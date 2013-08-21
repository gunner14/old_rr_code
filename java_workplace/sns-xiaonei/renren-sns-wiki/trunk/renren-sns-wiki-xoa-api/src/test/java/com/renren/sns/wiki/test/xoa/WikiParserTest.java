package com.renren.sns.wiki.test.xoa;

import java.util.ArrayList;
import java.util.List;

import org.junit.Test;

import com.renren.sns.wiki.xoa.IWikiParserService;
import com.renren.sns.wiki.xoa.IWikiXoaService;
import com.renren.sns.wiki.xoa.ParseContext;
import com.renren.sns.wiki.xoa.ParseResult;
import com.renren.sns.wiki.xoa.UGCType;
import com.renren.sns.wiki.xoa.WikiProfile;
import com.renren.sns.wiki.xoa.WikiState;
import com.renren.sns.wiki.xoa.util.WikiWordsCacheUtil;
import com.renren.xoa2.client.ServiceFactory;

/**
 * @author yi.li@renren-inc.com since 2012-4-6
 * 
 */
public class WikiParserTest {

    private void setProperties() {
        System.setProperty("xoa2.hosts.social_wiki.sns.xoa.renren.com", "10.3.25.27:9100");
    }

    private IWikiXoaService getWikiXoaService() {
        IWikiXoaService service = ServiceFactory.getService(IWikiXoaService.class);
        return service;
    }

    @Test
    public void testMatch() {
        try {
            setProperties();
            IWikiParserService service = ServiceFactory.getService(IWikiParserService.class);
            List<String> texts = new ArrayList<String>();
            texts.add("仙剑奇侠传三s");
            ParseContext context = new ParseContext(texts, 1111, UGCType.BLOG);

            long e1 = System.currentTimeMillis();
            for (int i = 0; i < 20; i++) {
                try {
                    ParseResult result = service.parseText(context);
                    if (result != null) {
                        System.out.println(result.wikiIds.size() + "  ==> " + result.wikiWords);
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
                if (i % 100 == 0) System.out.print("*");
            }
            long e2 = System.currentTimeMillis();

            System.out.println("time2: " + (e2 - e1));
        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    @Test
    public void testAsync() {
        setProperties();
        IWikiParserService service = ServiceFactory.getService(IWikiParserService.class);
        List<String> texts = new ArrayList<String>();
        texts.add("仙剑奇侠传三");
        ParseContext context = new ParseContext(texts, 1111, UGCType.BLOG);
        for (int i = 0; i < 100; i++) {
            service.asyncReloadResult(context);
        }
        try {
            Thread.sleep(5000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    @Test
    public void testPrivacy() {
        setProperties();
        IWikiParserService service = ServiceFactory.getService(IWikiParserService.class);
        List<String> texts = new ArrayList<String>();
        texts.add("仙剑奇侠传三");
        ParseContext context = new ParseContext(texts, 5981683174L, UGCType.PHOTO);
        context.setOwnerId(365323815);
        service.asyncUpdateParseResult(context, "2");
        System.out.println("done!!!");
    }

    @Test
    public void testDeleteSimpleComment() {
        setProperties();
        IWikiXoaService service = ServiceFactory.getService(IWikiXoaService.class);
        service.deleteSimpleComment(610000011, 431716353);
        System.out.println("done!!!");
    }

    @Test
    public void testBatchReload() {
        setProperties();
        IWikiParserService service = ServiceFactory.getService(IWikiParserService.class);

        long ugcId = 2000L;
        String text = "仙剑奇侠传三等等等泰山盗梦空间";
        List<String> pText = new ArrayList<String>();
        pText.add(text);

        String text1 = "";
        List<String> pText1 = new ArrayList<String>();
        pText1.add(text1);

        List<ParseContext> pList = new ArrayList<ParseContext>();
        for (int i = 0; i < 60; i++) {
            ParseContext ctx = new ParseContext();
            ctx.setTexts(pText);
            if (i % 5 == 0) {
                ctx.setTexts(pText1);
            }
            ctx.setUgcId(ugcId + i);
            ctx.setUgcType(UGCType.PHOTO);
            pList.add(ctx);
        }

        service.asyncBatchReloadResult(pList);

        try {
            Thread.sleep(10000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("done!!!");
    }

    private String parseTextInner(long ugcId, UGCType type, String text) {
        setProperties();
        IWikiParserService service = ServiceFactory.getService(IWikiParserService.class);
        List<String> texts = new ArrayList<String>();
        texts.add(text);
        ParseContext context = new ParseContext(texts, ugcId, type);
        ParseResult result = service.parseText(context);

        try {
            Thread.sleep(500);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        if (result != null) {
            System.out.println(result.wikiIds.size() + "  ==> " + result.wikiWords);
            return result.getWikiWords();
        }
        return null;
    }

    @Test
    public void testCacheCorrectSet() {
        setProperties();
        UGCType type = UGCType.BLOG;
        long ugcId = 1115L;
        WikiWordsCacheUtil.removeWikiWords(type, ugcId);
        String cacheString = WikiWordsCacheUtil.getWikiWords(type, ugcId);
        if (cacheString != null) {
            throw new RuntimeException("should be null");
        }

        WikiWordsCacheUtil.removeWikiWords(type, ugcId);
        parseTextInner(ugcId, type, "");
        cacheString = WikiWordsCacheUtil.getWikiWords(type, ugcId);
        if (!"{}".equals(cacheString)) {
            throw new RuntimeException("should be {}");
        }

        WikiWordsCacheUtil.removeWikiWords(type, ugcId);
        parseTextInner(ugcId, type, "仙剑奇侠传三 ");
        cacheString = WikiWordsCacheUtil.getWikiWords(type, ugcId);
        if (cacheString == null || cacheString.length() < 4) {
            throw new RuntimeException("return result not right:" + cacheString);
        }

        WikiWordsCacheUtil.removeWikiWords(type, ugcId);
        parseTextInner(ugcId, type, "仙传三");
        cacheString = WikiWordsCacheUtil.getWikiWords(type, ugcId);
        if (!"{}".equals(cacheString)) {
            throw new RuntimeException("should be {}");
        }

    }

    @Test
    public void testForbiddenWiki() {
        setProperties();
        IWikiXoaService service = getWikiXoaService();
        int wikiId = 610000005;
        boolean result = service.forbiddenWiki(wikiId);
        if (result != true) {
            throw new RuntimeException("forbidden error happens");
        }

        result = service.pauseWiki(wikiId);
        if (result != true) {
            throw new RuntimeException("pause error happens");
        }

        result = service.reopenWiki(wikiId);
        if (result != true) {
            throw new RuntimeException("reopen error happens");
        }
        System.out.println("finished");
    }

    @Test
    public void queryWikiState() {
        setProperties();
        IWikiXoaService service = getWikiXoaService();
        WikiState state = service.getWikiState(610000005);
        System.out.println(state.getValue());
    }

    @Test
    public void getWikiProfile() {
        setProperties();
        IWikiXoaService service = getWikiXoaService();
        WikiProfile wikiProfile = service.getWikiProfile(43171635);
        if(wikiProfile.isProfileUser()){
            System.out.println("在个人主页展示");
            System.out.println("想看电影的数目:"+wikiProfile.getWishMovieCount());
            System.out.println("看过电影的数目:"+wikiProfile.getCollectMovieCount());
        }else{
            System.out.println("不在个人主页展示");
        }
    }

}
