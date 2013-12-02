package com.renren.ad.sns.services.avenue.score;

import java.util.HashMap;
import java.util.Map;

import org.junit.BeforeClass;
import org.junit.Test;

import com.renren.ad.sns.data.Spammer;
import com.renren.ad.sns.services.avenue.GoodsMeta;

public class DefaultGoodsScorerTest {

    private static DefaultGoodsScorer goodsScorer;

    private static Map<Long, Integer> id2friendCount = new HashMap<Long, Integer>();

    private static Map<Long, Spammer> spammers = new HashMap<Long, Spammer>();
    
    @BeforeClass
    public static void init() {
        goodsScorer = new DefaultGoodsScorer();
        goodsScorer.setSpammers(spammers);
        goodsScorer.setScoreParam(new GoodsScoreParam());
        goodsScorer.setId2friendCount(id2friendCount);
    }

    @Test
    public void testScore() {
        /*
         * public GoodsMeta(long id, int avenueSts, long goodsId,
            boolean defaultOwnerPic, int shareCount, int likeCount,
            long ownerId, int categoryId, int source, String link,
            List<Long> tagList, List<Long> userLikeList)
         */
        GoodsMeta gm = new GoodsMeta(0, 0, 1207271430343333487L, false, 50, 50, 779011581, 0, 0,
                "link",23, null, null);
        gm.setRealLikeCount(50);
        
        System.out.println("初始: " + goodsScorer.score(gm));
        gm.setGoodsId(1208011430343333487L);
        System.out.println("发布时间较新: " + goodsScorer.score(gm));

        gm.setSource(1);
        System.out.println("淘宝来源: " + goodsScorer.score(gm));

        gm.setRealLikeCount(10);
        System.out.println("喜欢数从50降到10: " + goodsScorer.score(gm));
        gm.setRealLikeCount(100);
        System.out.println("喜欢数从10升到100: " + goodsScorer.score(gm));
        gm.setRealLikeCount(200);
        System.out.println("喜欢数从100升到200: " + goodsScorer.score(gm));

        gm.setShareCount(10);
        System.out.println("分享数从50降到10: " + goodsScorer.score(gm));
        gm.setShareCount(20);
        System.out.println("分享数从10升到20: " + goodsScorer.score(gm));
        gm.setRealLikeCount(10);
        System.out.println("喜欢数从200降到10: " + goodsScorer.score(gm));
        gm.setRealLikeCount(20);
        System.out.println("喜欢数从10升到20: " + goodsScorer.score(gm));

        
        id2friendCount.put(779011581L, 10);
        System.out.println("好友数为10: " + goodsScorer.score(gm));
        id2friendCount.put(779011581L, 100);
        System.out.println("好友数为100: " + goodsScorer.score(gm));
        
        
        spammers.put(779011581L, null);
        System.out.println("是spam: " + goodsScorer.score(gm));
        
        gm.setDefaultOwnerPic(true);
        System.out.println("默认头像: " + goodsScorer.score(gm));
        
        gm.setCredible(-1);
        System.out.println("可信度-1: " + goodsScorer.score(gm));
        gm.setCredible(1);
        System.out.println("可信度1: " + goodsScorer.score(gm));
        
        //正常用户
        gm.setRealLikeCount(20);
        gm.setShareCount(10);
        gm.setSource(0);
        spammers.remove(779011581L);
        gm.setDefaultOwnerPic(false);
        gm.setCredible(0);
        gm.setReputation(20);
        id2friendCount.put(779011581L, 150);
        System.out.println("普通用户: " + goodsScorer.score(gm));
    }

}
