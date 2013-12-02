package com.renren.xce.socialgraph.builder;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.apache.thrift.TException;


import xce.socialgraph.recommend.shoppingmaster.ShoppingMasterData;
import xce.socialgraph.recommend.shoppingmaster.ShoppingMasterRecommendUtil;

import com.renren.ad.sns.services.usercache.UserCacheServiceProxy;
import com.renren.xce.socialgraph.common.DataBuilder;
import com.renren.xce.socialgraph.common.DataItem;
import com.renren.xce.socialgraph.updator.CreateRecommendDataThread;
import com.xiaonei.xce.socialgraph.proto.GeneratedItems.Item;

/**
 * shopping master recommend
 * 
 * @author xiaoyun.yang
 * @email xiaoyun.yang@renren-inc.com
 */
// compare class
class ComparatorUser implements Comparator {
	public int compare(Object arg0, Object arg1) {
		ShoppingMasterData item0 = (ShoppingMasterData) arg0;
		ShoppingMasterData item1 = (ShoppingMasterData) arg1;
		//比较shares
		int flag = item0.getSharesCount().compareTo(item1.getSharesCount());
		return -flag;
	}
}

public class ShoppingMasterBuilder implements DataBuilder {
	private static int CACHE_SIZE = 3;// show 3 commonfriends
	public List<DataItem> getData(int hostId) {
		List<DataItem> resultList = new ArrayList<DataItem>();
		List<ShoppingMasterData> shopMasterList = ShoppingMasterRecommendUtil
				.getInstance().getShoppingMasterRecommendList(hostId);
		// Collections.shuffle(commList); //random strategy
		ComparatorUser comparator = new ComparatorUser();
		Collections.sort(shopMasterList, comparator);// sort by the rank.

		//get the masterid who have been watched by the hostid
		Set<Integer> fliterMasterId = new HashSet<Integer>();
		UserCacheServiceProxy proxy = new UserCacheServiceProxy("s.jie.d.xiaonei.com",
				9632);
		List<Long> follower = null;
		try {
			follower = proxy.getFollower(hostId, 0);
		} catch (TException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		//change to the list<int>
		List<Integer> resfollower = new ArrayList<Integer>();
		for(Long tmp : follower) {
			Integer a = tmp.intValue();
			resfollower.add(a);
		}
		// filter the master who has been watched by the uid
		
//		for(Long tmp : follower)
//			System.out.println(tmp);
//		for(ShoppingMasterData tmp :  shopMasterList)
//			System.out.println(tmp.getShoppingMasterId());
//		System.out.println();
//		shopMasterList.removeAll(resfollower);
		List<ShoppingMasterData> shopMasterListResult = new ArrayList<ShoppingMasterData>();
		for(ShoppingMasterData tmp : shopMasterList) {
			if(follower.contains(tmp.getUserId()))
				continue;
			else
				shopMasterListResult.add(tmp);
		}
//		for(ShoppingMasterData tmp :  shopMasterListResult)
//                        System.out.println(tmp.getShoppingMasterId());		
		
		for (ShoppingMasterData shopdata : shopMasterListResult) {
			Item.Builder item = Item.newBuilder();
			item.setId(shopdata.getShoppingMasterId());
			item.setField(shopdata.getSharesCount());
			item.setMessage(CreateRecommendDataThread.RSHOPMASTER);
			for (Integer shareId : shopdata.getShareList()) {
				if (item.getFieldsCount() >= CACHE_SIZE) {
					break;
				}
				item.addFields(shareId);
			}

			DefaultDataItem commItem = new DefaultDataItem(
					(long) shopdata.getShoppingMasterId(), item);
			resultList.add(commItem);
		//	if(resultList.size()>=1)
		//		break;//just recommend one shopping master.
		}
		return resultList;
	}

	public String getBuilderType() {
		return CreateRecommendDataThread.RSHOPMASTER;
	}
}
