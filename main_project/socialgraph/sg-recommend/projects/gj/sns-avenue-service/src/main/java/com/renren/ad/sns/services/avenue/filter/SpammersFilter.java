package com.renren.ad.sns.services.avenue.filter;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import com.renren.ad.sns.data.Goods;
import com.renren.ad.sns.data.Spammer;
import com.renren.ad.sns.services.avenue.DataHammer;

public class SpammersFilter implements GoodsCandidateFilter{
	@Override
	public List<Goods> filter(List<Goods> raw, DataHammer dataHammer) {
		Map<Long, Spammer> spamers = dataHammer.getAvaliableSpammers();
		List<Goods> result = new ArrayList<Goods>();
		for(Goods goods: raw){
			Spammer spammer=spamers.get(goods.getOwnerId());
			if(spammer==null){
				result.add(goods);
			}
		}
		System.out.println("spammer filter has filtered "+(raw.size()-result.size()));
		return result;
	}

}
