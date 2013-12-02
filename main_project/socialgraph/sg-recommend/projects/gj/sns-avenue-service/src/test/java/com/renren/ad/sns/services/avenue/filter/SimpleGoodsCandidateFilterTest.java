package com.renren.ad.sns.services.avenue.filter;

import static org.junit.Assert.*;

import java.util.ArrayList;
import java.util.List;

import org.junit.Test;

import com.renren.ad.sns.data.Goods;


public class SimpleGoodsCandidateFilterTest {

    private SimpleGoodsCandidateFilter filter = new SimpleGoodsCandidateFilter();
    
    private static Goods createGoods(long goodsId, String link){
        Goods g = new Goods();
        g.setGoodsId(goodsId);
        g.setLink(link);
        return g;
    }
    
    
    @Test
    public void testFilter() {
        List<Goods> testGoods = new ArrayList<Goods>();
        Goods g1 = createGoods(1, "test1");
        testGoods.add(g1);
        testGoods.add(createGoods(2, SimpleGoodsCandidateFilter.BAN_LINK_PREFIX + "test1"));
        Goods g2 = createGoods(3, "test2");
        testGoods.add(g2);
        testGoods.add(createGoods(4, SimpleGoodsCandidateFilter.BAN_LINK_PREFIX + "test2"));
        
        List<Goods> filtered = filter.filter(testGoods, null);
        assertNotNull(filtered);
        assertEquals(2, filtered.size());
        assertTrue(filtered.contains(g1) && filtered.contains(g2));
    }

}
