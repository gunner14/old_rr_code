/*
 * Function: SocialGraph friends recommendation service data items builder, 
 * including hostId, fofId, rank, mutualFriendNumber, mutualFriendIds 
 * Developer: XiongJunWu
 * Email: Junwu.xiong@renren-inc.com
 * Date: 2012-03-27
 */

package com.renren.xce.socialgraph.builder;

import java.util.ArrayList;
import java.util.List;

import com.renren.xce.socialgraph.common.DataBuilder;
import com.renren.xce.socialgraph.common.DataItem;
import com.renren.xce.socialgraph.updator.CreateRecommendDataThread;
import com.renren.xce.socialgraph.updator.OfflineUpdator;
import com.xiaonei.xce.socialgraph.proto.GeneratedItems.Item;

public class RcdFoFBuilder implements DataBuilder {
	public List<DataItem> getData(int hostId) { 
		List<DataItem> resultList = new ArrayList<DataItem>();
//		if (OfflineUpdator.whiteIdListSet.contains(hostId)) {
//			RcdFoFUtilGroupRotate.htableName = FoFHTable.kFoFExpHTable;
//		} else {
//			RcdFoFUtilGroupRotate.htableName = FoFHTable.kFoFHTable;
//		}
		List<RcdFoFRankData> fofList = new ArrayList<RcdFoFRankData>();
		if (OfflineUpdator.kFoFProbabilityStrategy) {
			fofList = RcdFoFUtilGroupProbability.getInstance().getRcdFoFData(hostId, OfflineUpdator.kFoFMaxLoadNumber);
		} else {
			fofList = RcdFoFUtilGroupRotate.getInstance().getRcdFoFData(hostId, OfflineUpdator.kFoFMaxLoadNumber);
		}
		
//		Collections.shuffle(fofList);
		if (!fofList.isEmpty()) {
			for (RcdFoFRankData fof : fofList) {
				Item.Builder item = Item.newBuilder();
				item.setId(fof.getFoFId());
				item.setField(fof.getMutualFriendNumber());
				item.addMessages(String.valueOf(fof.getRank()));
//				Clogging.Info("hostId " + hostId + " fofId " + fof.getFoFId() + " rank " + fof.getRank() + " mut " + fof.getMutualFriendNumber());
				item.setMessage(CreateRecommendDataThread.RFOF);
				for (Integer id : fof.getmutualFriendIdList()) {
						item.addFields(id);
//						Clogging.Info(" " + id);
				}
				DefaultDataItem fofItem = new DefaultDataItem((long)fof.getFoFId(), item);
				resultList.add(fofItem);
			}		
		}		
		return resultList;
	}
	
	public String getBuilderType() {
		return CreateRecommendDataThread.RFOF;
	}
}
















































//List<RcdFoFRankData> fofList = RcdFoFUtil.getInstance(NewRecommendUpdator.kHTableName).getRcdFoFData();
//List<RcdFoFRankData> fofList = RcdFoFUtil.hostId2FoFListMap.get(hostId);
//Collections.shuffle(fofList);		//random strategy
//Clogging.Info("hostId fof size " + fofList.size());
//public class RcdFoFBuilder implements DataBuilder {
//	static final int kMaxNumber = 10;
//	public List<DataItem> getData(int hostId) { 
//		List<DataItem> resultList = new ArrayList<DataItem>();
////		List<RcdFoFRankData> fofList = RcdFoFUtil.getInstance().getRcdFoFData();
//		List<RcdFoFRankData> fofList = RcdFoFUtil.hostId2FoFListMap.get(hostId);
////		Collections.shuffle(fofList);		//random strategy
//		Clogging.Info("**************************************************************************************");
//		Clogging.Info("hostId " + hostId);
//		for (RcdFoFRankData fof : fofList) {
//			Item.Builder item = Item.newBuilder();
//			item.setId(fof.getFoFId());
//			Clogging.Info(" fofId " + fof.getFoFId());
//			item.setField(fof.getMutualFriendNumber());
//			Clogging.Info("mutnum " + fof.getMutualFriendNumber());
//			item.setMessage(CreateRecommendDataThread.RFOF);
//			Clogging.Info(" mutids ");
//			for (Integer id : fof.getmutualFriendIdList()) {
//					item.addFields(id);
//					Clogging.Info(" " + id);
//			}
//			DefaultDataItem fofItem = new DefaultDataItem((long)fof.getHostId(), item);
//			resultList.add(fofItem);
//		}		
//		Clogging.Info("**************************************************************************************");
//		return resultList;
//	}
//	
//	public String getBuilderType() {
//		return CreateRecommendDataThread.RFOF;
////		return CreateRecommendDataThread.COMMON_FRIENDS;
//	}
//}