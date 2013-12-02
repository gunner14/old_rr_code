package com.renren.ad.sns.services.avenue.filter;

import java.util.ArrayList;
import java.util.List;

import com.renren.ad.sns.data.Goods;
import com.renren.ad.sns.services.avenue.DataHammer;

public class TaobaoShareFilter implements GoodsCandidateFilter{
	@Override
	public List<Goods> filter(List<Goods> raw, DataHammer dataHammer) {
		List<Goods> result = new ArrayList<Goods>();
		for(Goods goods: raw){
			if(goods.getSource()==1){
				continue;
			}
			result.add(goods);
		}
		System.out.println("taobao filter has filtered "+(raw.size()-result.size()));
		return result;
	}
}
