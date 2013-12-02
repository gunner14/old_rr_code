package com.renren.ad.sns.services.uranus;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import org.junit.BeforeClass;
import org.junit.Ignore;
import org.junit.Test;

import com.google.code.morphia.Datastore;
import com.renren.ad.sns.data.Goods;
import com.renren.ad.sns.data.Spammer;
import com.renren.ad.sns.services.uranus.DefaultCandidatesFilter.GoodsInfo;

public class DefaultCandidatesFilterTest {

    private static DefaultCandidatesFilter filter;

    /* goods id only for testing purpose */
    private static long GOODS_ID = 2000000000000000000L;

    private List<Spammer> testSpammers = new ArrayList<Spammer>();
    
    private List<Candidate> validCandidates = new ArrayList<Candidate>();
    
    @BeforeClass
    public static void init() {
        DefaultCandidatesFilter.MAX_GOODS_SIZE = 0;
        filter = new DefaultCandidatesFilter(CategoryCandidatePickerTest.getGJDataStore());
    }

    @Test
    public void testLoadSpammer() {
        assertNotNull(filter.getSpammer());
        assertTrue(filter.getSpammer().size() > 0);
    }

    /**
     * Create a goods with default properties.
     * 
     * @return
     */
    private Goods createGoods() {
        Goods goods = new Goods();
        goods.setPicLinkList(Arrays
                .asList("http://www.test.com/1.jpg", "http://www.test.com/2.jpg"));
        goods.setGoodsId(++GOODS_ID);
        goods.setLink("www.link.com/"+goods.getGoodsId());
        return goods;
    }
    
    private Goods createGoods(long gid){
        Goods goods = new Goods();
        goods.setPicLinkList(Arrays
                .asList("http://www.test.com/1.jpg", "http://www.test.com/2.jpg"));
        goods.setGoodsId(gid);
        goods.setLink("www.link.com/"+goods.getGoodsId());
        return goods;
    }

    /**
     * 测试数据的组成:
     * 1）一个商品，goodsId = gid
     * 2）两个商品，goodsId相同
     * 3）一个商品，ownerId in spammerIdList
     * 4）一个商品，ownerId = uid
     * 5）两个商品，link相同
     * 6）一个商品，不含图片
     * 7）一个商品，link以http://s.click.taobao.com开头
     * 
     * @param uid
     * @param gid
     * @return
     */
    private List<Candidate> prepareData(long uid, long gid) {
        testSpammers.clear();
        validCandidates.clear();
        List<Candidate> result = new ArrayList<Candidate>();
//        Datastore ds = filter.getDatastore();
        
        // insert a test spammer
        long spammerUserId = 1111111111L;
        /*Spammer spammer = new Spammer(spammerUserId, "" + System.currentTimeMillis(), -1, "test");
        ds.save(spammer);
        testSpammers.add(spammer);*/

        //1）一个商品，goodsId = gid
        Goods goodsSameWithGid = createGoods(gid);
        result.add(new Candidate(goodsSameWithGid.getGoodsId(), 0));
        filter.addGoodsInfo(GoodsInfo.fromGoods(goodsSameWithGid));

        //2）两个商品，goodsId相同
        Goods goodsSame1 = createGoods(-1);
        Goods goodsSame2 = createGoods(-1);
        result.add(new Candidate(goodsSame1.getGoodsId(), 0));
        result.add(new Candidate(goodsSame2.getGoodsId(), 0));
        filter.addGoodsInfo(GoodsInfo.fromGoods(goodsSame1));
        filter.addGoodsInfo(GoodsInfo.fromGoods(goodsSame2));
        validCandidates.add(new Candidate(goodsSame1.getGoodsId(), 0));

        //3）一个商品，ownerId in spammerIdList
        Goods goodsOwnerIsSpammer = createGoods();
        goodsOwnerIsSpammer.setOwnerId(spammerUserId);
        result.add(new Candidate(goodsOwnerIsSpammer.getGoodsId(), 0));
        filter.addGoodsInfo(GoodsInfo.fromGoods(goodsOwnerIsSpammer));
        filter.addSpammer(spammerUserId);
        filter.addSpamGoods(goodsOwnerIsSpammer.getGoodsId());

        //4）一个商品，ownerId = uid
        Goods goodsSameOwnerId = createGoods();
        goodsSameOwnerId.setOwnerId(uid);
        result.add(new Candidate(goodsSameOwnerId.getGoodsId(), 0));
        filter.addGoodsInfo(GoodsInfo.fromGoods(goodsSameOwnerId));

        //5）两个商品，link相同
        Goods goodsSameLink1 = createGoods();
        Goods goodsSameLink2 = createGoods();
        goodsSameLink1.setLink("http://www.test.com/test");
        goodsSameLink2.setLink("http://www.test.com/test");
        result.add(new Candidate(goodsSameLink1.getGoodsId(), 0));
        result.add(new Candidate(goodsSameLink2.getGoodsId(), 0));
        filter.addGoodsInfo(GoodsInfo.fromGoods(goodsSameLink1));
        filter.addGoodsInfo(GoodsInfo.fromGoods(goodsSameLink2));
        validCandidates.add(new Candidate(goodsSameLink1.getGoodsId(), 0));
        
        //6）一个商品，不含图片
        Goods goodsNoPic = createGoods();
        goodsNoPic.setPicLinkList(new ArrayList<String>());
        result.add(new Candidate(goodsNoPic.getGoodsId(), 0));
        filter.addGoodsInfo(GoodsInfo.fromGoods(goodsNoPic));

        //7）一个商品，link以http://s.click.taobao.com开头
        Goods goodsStartsWithBanUrl = createGoods();
        goodsStartsWithBanUrl.setLink("http://s.click.taobao.com/test");
        result.add(new Candidate(goodsStartsWithBanUrl.getGoodsId(), 0));
        filter.addGoodsInfo(GoodsInfo.fromGoods(goodsStartsWithBanUrl));
        
        return result;
    }

    private void clearData() {
        Datastore ds = filter.getDatastore();
        for(Spammer s : testSpammers){
            ds.delete(s);
        }
    }

    @Ignore
    @Test
    public void testFilter() {
        try{
            long uid = 1234567890L;
            long gid = 1234567890L;
            List<Candidate> candidates = prepareData(uid, gid);
//            filter.loadSpammer();   //re-load the spammer
            List<Candidate> fCandidates = filter.filter(candidates, uid, gid);
            assertNotNull(fCandidates);
            assertNotNull(validCandidates);
            assertEquals(validCandidates.size(), fCandidates.size());
            Collections.sort(fCandidates);
            Collections.sort(validCandidates);
            for(int i=0; i<fCandidates.size(); i++){
                assertTrue(fCandidates.get(i).equals(validCandidates.get(i)));
            }
        }finally {
            clearData();
        }
    }
    
}
