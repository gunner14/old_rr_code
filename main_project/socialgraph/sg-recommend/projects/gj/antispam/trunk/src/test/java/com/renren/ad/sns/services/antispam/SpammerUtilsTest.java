package com.renren.ad.sns.services.antispam;

import junit.framework.TestCase;

import com.google.code.morphia.Datastore;
import com.google.code.morphia.query.Query;
import com.renren.ad.sns.data.Goods;

public class SpammerUtilsTest extends TestCase {

    /* goods id only for testing purpose */
    private static long GOODS_ID = 2000000000000000000L;

    private static Datastore datastore = GJDataStore.getDefaultDataStore();

    public void testBanGoods() {
        Goods goods = new Goods();
        goods.setGoodsId(GOODS_ID);
        goods.setAvenueSts(0);

        try {
            // insert a test goods
            datastore.save(goods);

            // do ban
            SpammerUtils.banGoods(GOODS_ID);

            // validate
            Query<Goods> query = datastore.createQuery(Goods.class).filter("goodsId =", GOODS_ID);
            assertNotNull(query);
            assertNotNull(query.get());
            assertEquals(-1, query.get().getAvenueSts());
        } finally {
            // remove test goods
            datastore.delete(goods);
        }
    }

}
