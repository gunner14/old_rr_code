/*
 * Function: SocialGraph friends recommendation of friend cluster algorithm
 * Developer: XiongJunWu
 * Email: Junwu.xiong@renren-inc.com
 * Date: 2012-12-07
 */

package com.renren.xce.socialgraph.builder;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import com.renren.xce.socialgraph.common.DataBuilder;
import com.renren.xce.socialgraph.common.DataItem;
import com.renren.xce.socialgraph.data.RcdFFUCRData;
import com.renren.xce.socialgraph.data.RcdFFUCRUtil;
import com.renren.xce.socialgraph.updator.CreateRecommendDataThread;
import com.renren.xce.socialgraph.updator.FFUCRUpdator;
import com.xiaonei.xce.socialgraph.proto.GeneratedItems.Item;

public class RcdFFUCRBuilder implements DataBuilder {
	
	public List<DataItem> getData(int hostId) { 
		List<DataItem> resultList = new ArrayList<DataItem>();		
		RcdFFUCRUtil.getInstance(FFUCRUpdator.kFFUCRFile);
	//	if (RcdFFUCRUtil.getRcdFoFData()) {
		if (RcdFFUCRUtil.hostId2RcdFriendListMap.size() > 0) {
			Map<Integer, List<RcdFFUCRData> > hostId2RcdFriendListMap = RcdFFUCRUtil.hostId2RcdFriendListMap;
			List<RcdFFUCRData> rcdFriendList = hostId2RcdFriendListMap.get(hostId);
			
			if (!rcdFriendList.isEmpty()) {
				for (RcdFFUCRData rcdFFUCRData : rcdFriendList) {
					Item.Builder item = Item.newBuilder();
					item.setId(rcdFFUCRData.getRcdFriendId());
					item.setField(rcdFFUCRData.getClusterSize());					
					item.addMessages(String.valueOf(rcdFFUCRData.getExplanFlag()));
//					Clogging.Info("hostId " + hostId + " fofId " + fof.getFoFId() + " rank " + fof.getRank() + " mut " + fof.getMutualFriendNumber());
					item.setMessage(CreateRecommendDataThread.FREINDS_CLUSTER);
					for (Integer id : rcdFFUCRData.getClusterMemberList()) {
							item.addFields(id);
//							Clogging.Info(" " + id);
					}
					DefaultDataItem fofItem = new DefaultDataItem((long)rcdFFUCRData.getRcdFriendId(), item);
					resultList.add(fofItem);
				}
			}		
		}
		
		return resultList;
	}
	
	public String getBuilderType() {
		return CreateRecommendDataThread.FREINDS_CLUSTER;
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