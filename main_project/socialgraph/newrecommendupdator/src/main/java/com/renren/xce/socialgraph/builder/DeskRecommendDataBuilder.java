package com.renren.xce.socialgraph.builder;

import java.util.ArrayList;
import java.util.List;

import xce.socialgraph.recommend.desk.DesktopRecommendUtil;

import com.renren.xce.socialgraph.common.DataBuilder;
import com.renren.xce.socialgraph.common.DataItem;
import com.renren.xce.socialgraph.updator.CreateRecommendDataThread;
import com.xiaonei.xce.socialgraph.proto.GeneratedItems.Item;

/**
 * building data of desk-top recommend
 * @author zhangnan
 * @email zhangnan@renren-inc.com
 */
public class DeskRecommendDataBuilder implements DataBuilder {

	public List<DataItem> getData(int hostId) {
		// TODO Auto-generated method stub
		List<DataItem> resultList = new ArrayList<DataItem>();
		List<Integer> deskList = DesktopRecommendUtil.getInstance().getDeskRecommendList(hostId);
		for (Integer deskId : deskList) {
			Item.Builder item = Item.newBuilder();
			item.setId(deskId);
			item.setMessage(CreateRecommendDataThread.RDESK);
			
			DefaultDataItem deskItem = new DefaultDataItem((long)deskId, item);
			resultList.add(deskItem);
		}
		
		return resultList;
	}

	public String getBuilderType() {
		// TODO Auto-generated method stub
		return CreateRecommendDataThread.RDESK;
	}

}
