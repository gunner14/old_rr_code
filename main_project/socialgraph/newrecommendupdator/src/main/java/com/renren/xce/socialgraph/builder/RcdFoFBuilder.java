/*
 * Function: SocialGraph group SocialGraphFoF friends recommendation service data items
 * builder, including hostId, fofId, rank, mutualFriendNumber, mutualFriendIds 
 * Developer: xiongjunwu
 * Email: Junwu.xiong@renren-inc.com
 * Date: 2012-03-27
 */

package com.renren.xce.socialgraph.builder;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import xce.socialgraph.recommend.fof.RcdFoFData;
import xce.socialgraph.recommend.fof.RcdFoFUtil;

import com.renren.xce.socialgraph.common.DataBuilder;
import com.renren.xce.socialgraph.common.DataItem;
import com.renren.xce.socialgraph.updator.CreateRecommendDataThread;
import com.xiaonei.xce.socialgraph.proto.GeneratedItems.Item;

public class RcdFoFBuilder implements DataBuilder {
	public List<DataItem> getData(int hostId) { 
		List<DataItem> resultList = new ArrayList<DataItem>();
		List<RcdFoFData> fofList = RcdFoFUtil.getInstance().getRcdFoFData(hostId, -1);
		Collections.shuffle(fofList);		//random strategy
		for (RcdFoFData fof : fofList) {
			Item.Builder item = Item.newBuilder();
			item.setId(fof.getFoFId());
			item.setField(fof.getMutualFriendNumber());
			item.setMessage(CreateRecommendDataThread.RFOF);
//			item.setMessage(CreateRecommendDataThread.COMMON_FRIENDS);
			for (Integer id : fof.getMutualFriendIds()) {
					item.addFields(id);
			}
			DefaultDataItem fofItem = new DefaultDataItem((long)fof.getHostId(), item);
			resultList.add(fofItem);
		}	
		
		return resultList;
	}

	public String getBuilderType() {
		return CreateRecommendDataThread.RFOF;
//		return CreateRecommendDataThread.COMMON_FRIENDS;
	}
}
