package com.renren.ad.sns.services.avenue.filter;

import java.util.List;

import com.renren.ad.sns.data.Goods;
import com.renren.ad.sns.services.avenue.DataHammer;


public interface GoodsCandidateFilter {
    
    /**
     * 对原始的商品集进行过滤
     * @param raw
     * @param dataHammer 可以获取dataHammer对应的数据。这个数据对某些过滤器有用。
     * @return
     */
    public List<Goods> filter(List<Goods> raw, DataHammer dataHammer);
    
}
