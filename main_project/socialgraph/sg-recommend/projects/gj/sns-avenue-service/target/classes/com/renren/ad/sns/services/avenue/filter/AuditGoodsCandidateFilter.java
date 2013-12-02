package com.renren.ad.sns.services.avenue.filter;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import com.google.code.morphia.Datastore;
import com.renren.ad.sns.data.Goods;
import com.renren.ad.sns.data.GoodsAudit;
import com.renren.ad.sns.services.avenue.DataHammer;

public class AuditGoodsCandidateFilter  implements GoodsCandidateFilter{
	private Datastore datastore;
	@Override
	public List<Goods> filter(List<Goods> raw, DataHammer dataHammer) {
		Set<Long> unAuditGoods=findAllUnauditGoods();
		List<Goods> result = new ArrayList<Goods>();
		for(Goods goods: raw){
			if(!unAuditGoods.contains(goods.getGoodsId())){
				result.add(goods);
			}
		}
		System.out.println("audit goods filter has filtered "+(raw.size()-result.size()));
		return result;
	}
	public Set<Long> findAllUnauditGoods(){
		List<GoodsAudit> list=this.datastore.createQuery(GoodsAudit.class).retrievedFields(true, "goodsId").limit(80000).asList();
		if(list==null)return new HashSet<Long>();
		Set<Long> result=new HashSet<Long>();
		for(GoodsAudit audit: list){
			result.add(audit.getGoodsId());
		}
		return result;
	}
	public void setDatastore(Datastore datastore) {
		this.datastore = datastore;
	}

}
