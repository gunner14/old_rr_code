package com.renren.ad.sns.services.antispam.model;

import java.util.ArrayList;
import java.util.List;

import junit.framework.TestCase;

import com.google.code.morphia.Datastore;
import com.renren.ad.sns.data.Goods;
import com.renren.ad.sns.data.Spammer;
import com.renren.ad.sns.data.User;
import com.renren.ad.sns.data.UserAbsInfo;
import com.renren.ad.sns.services.antispam.GJDataStore;


public class SpamHandlerTest extends TestCase {

    
    public static final long USER_ID = 2000000000000000000L;
    public static final long GOODS_ID1 = 2000000000000000000L;
    public static final long GOODS_ID2 = 2000000000000000001L;
    private static Datastore datastore = GJDataStore.getDefaultDataStore();
    
    private User testUser;
    private Goods testGoods1;
    private Goods testGoods2;
    private List<AntiSpamRecord> records;
    
    private void prepareData(){
        //1. User表中新增测试用户，使之不通过checkUser
        testUser = new User();
        testUser.setUserId(USER_ID);
        UserAbsInfo uInfo = new UserAbsInfo();
        uInfo.setFansCount(0);
        testUser.setUserAbsInfo(uInfo);
        datastore.save(testUser);
        
        //2. 增加2个测试商品，所有者为上述用户
        testGoods1 = new Goods();
        testGoods1.setGoodsId(GOODS_ID1);
        testGoods1.setOwnerId(USER_ID);
        datastore.save(testGoods1);
        
        testGoods2 = new Goods();
        testGoods2.setGoodsId(GOODS_ID2);
        testGoods2.setOwnerId(USER_ID);
        datastore.save(testGoods2);
        
        // 增加相应的AntispamRecord
        records = new ArrayList<AntiSpamRecord>();
        AntiSpamRecord rec1 = new AntiSpamRecord();
        rec1.goodsId = GOODS_ID1;
        rec1.spamLevel = 1;
        
        AntiSpamRecord rec2 = new AntiSpamRecord();
        rec2.goodsId = GOODS_ID2;
        rec2.spamLevel = 2;
        
        records.add(rec1);
        records.add(rec2);
    }
    
    /**
     * 清除测试用的所有数据
     */
    private void clearData(){
        datastore.delete(testUser);
        datastore.delete(testGoods1);
        datastore.delete(testGoods2);
        datastore.delete(datastore.createQuery(Spammer.class).filter("_id =", USER_ID));
    }
    
    
    public void testHandleSpamGoods() {
        try{
            prepareData();
            
            // 处理第一个商品，该用户不在spammer中，因此scoreDelta=-1
            // 处理第二个商品，用户已存在spammer中，因此scoreDelta=-3
            SpamHandler.handleSpamGoods(records);
            
            assertTrue(validateResult());
        }finally {
            clearData();
        }
    }

    private boolean validateResult() {
        Spammer spammer = datastore.find(Spammer.class, "_id", USER_ID).get();
        if(spammer != null && spammer.getScoreDelta() == -3){
            return true;
        }
        
        return false;
    }

}
