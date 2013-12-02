package com.renren.ad.sns.services.avenue.filter;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import com.renren.ad.sns.data.Goods;
import com.renren.ad.sns.data.SimpleUserReputation;
import com.renren.ad.sns.services.avenue.DataHammer;

public class ReputationFilter implements GoodsCandidateFilter{
	@Override
	public List<Goods> filter(List<Goods> raw, DataHammer dataHammer) {
		Map<Long, SimpleUserReputation> reputations = dataHammer.getAvaliableReputations();
		List<Goods> result = new ArrayList<Goods>();
		for(Goods goods: raw){
			SimpleUserReputation reputation=reputations.get(goods.getOwnerId());
			if(reputation==null){
				continue;
			}
			if(reputation.getScore()<25&&goods.getLikeCount()==0){
				continue;
			}
//			if(reputation.getScore()<30&&goods.getLikeCount()==0){
//				continue;
//			}
			
			result.add(goods);
		}
		System.out.println("reputation filter has filtered "+(raw.size()-result.size()));
		return result;
	}
}


