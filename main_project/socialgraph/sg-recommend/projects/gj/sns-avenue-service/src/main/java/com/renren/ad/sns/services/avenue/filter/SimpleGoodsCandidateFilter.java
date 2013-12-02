package com.renren.ad.sns.services.avenue.filter;

import java.util.ArrayList;
import java.util.List;

import com.renren.ad.sns.data.Goods;
import com.renren.ad.sns.services.avenue.DataHammer;


public class SimpleGoodsCandidateFilter implements GoodsCandidateFilter {

    public static final String BAN_LINK_PREFIX = "http://s.click.taobao.com/";
    
    @Override
    public List<Goods> filter(List<Goods> raw, DataHammer dataHammer) {
        List<Goods> result = new ArrayList<Goods>();
        
        if(raw != null && raw.size() > 0){
            for(Goods goods: raw){
                if(goods.getLink() !=null && goods.getLink().startsWith(BAN_LINK_PREFIX)){
                    continue;
                }
                result.add(goods);
            }
        }
        System.out.println("simple filter has filtered "+(raw.size()-result.size()));
        return result;
    }

}
