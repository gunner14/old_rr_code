package com.renren.ad.sns.services.avenue;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.ad.sns.data.Goods;

/**
 * @author xiong.hua
 */
public class GoodsFeatureTokenizer implements TargetingFeatureTokenizer {
    private static final Log LOG = LogFactory
            .getLog(GoodsFeatureTokenizer.class);

    private List<TargetingFeature> features;

    // iterator implementation
    private Iterator<TargetingFeature> iterator;

    /**
     * Creates a new instance of GoodsFeatureTokenizer.
     * 
     * @param goodsFeature
     */
    public GoodsFeatureTokenizer(GoodsMeta goods) {
        features = tokenize(goods);

        iterator = features.iterator();
    }

    private List<TargetingFeature> tokenize(GoodsMeta goods) {
        List<TargetingFeature> features = new ArrayList<TargetingFeature>();

        int cid = goods.getCategoryId();
        features.add(new TargetingFeature(FeatureType.Category, cid));
//        LOG.debug(FeatureType.Category.name() + cid);

        features.add(new TargetingFeature(FeatureType.Tag, 0));
        List<Long> tags = goods.getTagList();
        if (tags != null) {
            for (long tid: tags) {
                features.add(new TargetingFeature(FeatureType.Tag, tid));
//                LOG.debug(FeatureType.Tag.name() + tid);
            }
        }

        features.add(new TargetingFeature(FeatureType.PRICE_LEVEL, FeatureType.findPriceLevel(goods.getPrice())));
        return features;
    }

    public TargetingFeature nextFeature() {
        return iterator.next();
    }

    public boolean hasNext() {
        return iterator.hasNext();
    }

    @Override
    public List<TargetingFeature> getAll() {
        return features;
    }

}
