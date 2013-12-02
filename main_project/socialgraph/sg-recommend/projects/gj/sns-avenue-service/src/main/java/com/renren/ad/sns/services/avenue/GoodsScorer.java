package com.renren.ad.sns.services.avenue;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Map;
import java.util.Random;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.ad.sns.data.Spammer;
import com.renren.ad.sns.util.Constant;

/**
 * a scorer for Goods
 * 
 * @author xiong.hua
 */
//public class GoodsScorer implements Scorer<GoodsMeta> {
//    private static final Log LOG = LogFactory.getLog(GoodsScorer.class);
//    private static final SimpleDateFormat SDF = new SimpleDateFormat(
//            "yyMMddHHmm");
//    private long bound = 0;
//    private double ratio = 0.5;
//    private int upbound = 10;
//    private Random rand = new Random(System.currentTimeMillis());
//    private Map<Long, Spammer> spammers;
//    private AvenueSortParameter avenueSortParameter;
//    @Override
//    public double score(GoodsMeta goods) {
//        double point = 0.0;
//        try {
//            if (goods.getAvenueSts() > Constant.AVENUE_STS_NORMAL) {
//                point = 100 + goods.getAvenueSts();
//            } else {
//                long date = goods.getGoodsId() / 1000000000l;
//                // most recently goods (1h)
////                point = Math.log(1.0
////                        + date
////                        * avenueSortParameter.getTimeWeight()/*0.0001*/
////                        + Math.log(1.0 + Math.log(1.0 + goods.getRealLikeCount()
////                                * avenueSortParameter.getLikeWeight() + goods.getShareCount() * avenueSortParameter.getFwdWeight())));
//                point = Math.log(Math.log(1+date*10000)+Math.log(1.0 + Math.log(goods.getRealLikeCount()
//                        * avenueSortParameter.getLikeWeight() + goods.getShareCount() * avenueSortParameter.getFwdWeight())));
//                LOG.debug(goods.getGoodsId()+" init score " + point);
//                LOG.debug(goods.getGoodsId()+" real like count " + goods.getRealLikeCount()+" orgi like count "+goods.getLikeCount());
//                boolean punish = false;
//                if(goods.getCredible()==1&&avenueSortParameter.isOpenCredibleForNotLikeSort()){
//                	point+=avenueSortParameter.getCrediblePoint();
//                }
//                // taobao slow down
//                if (goods.getSource() == 1) {
//                    point += avenueSortParameter.getTaobaoWeight();
//                    punish = true;
//                }
//                if (spammers != null
//                        && spammers.containsKey(goods.getOwnerId())) {
//                    point += spammers.get(goods.getOwnerId()).getScoreDelta();
//                    punish = true;
//                }
//                // dec untrust owner's point [renren pic is default pic]
//                if (goods.isDefaultOwnerPic()) {
//                    point +=avenueSortParameter.getDefaultOwnerPic();
//                    punish = true;
//                }
//                if (!punish && upbound > 0 && bound > 0
//                        && calculate(bound, date) <= 5
//                        && rand.nextDouble() < ratio) {
//                    point += 0.0000075;
//                    --upbound;
//                }
//                if(goods.getCredible()==-1){
//                	point +=avenueSortParameter.getUncrediblePoint();
//                	punish = true;
//                }
//                double value=goods.getReputation()+avenueSortParameter.getReputationCorrectionValue()-1;
//                if(value<0){
//                	point+=value*avenueSortParameter.getReputationCorrectionParameter();
//                    LOG.debug("reputation correction "+goods.getGoodsId()+":"+value*avenueSortParameter.getReputationCorrectionParameter());
//                }
//                
//
//            }
//        } catch (Exception e) {
//            LOG.error("score goods [ gid = " + goods.getGoodsId()
//                    + " ] error! ", e);
//        }
//        LOG.debug(goods.getGoodsId() + "\t" + point+", user parameters is "+avenueSortParameter.toString());
//        return point;
//    }
    
//    
//    /**
//     * avoke it before u use score method
//     * @param spammers
//     * @param upbound
//     */
//    public void init(Map<Long, Spammer> spammers, int upbound, AvenueSortParameter avenueSortParameter) {
//        String tmp = SDF.format(new Date());
//        bound = Long.parseLong(tmp);
//        this.upbound = upbound;
//        this.spammers = spammers;
//        this.avenueSortParameter=avenueSortParameter;
//    }
//
//    static long calculate(long time1, long time2) {
//        long diff = 0;
//        try {
//            Date d1 = SDF.parse(String.valueOf(time1));
//            Date d2 = SDF.parse(String.valueOf(time2));
//            diff = d1.getTime() - d2.getTime();
//            diff = diff / 60000;
//        } catch (Exception e) {
//            LOG.error(e, e);
//        }
//        return diff;
//    }
//}
