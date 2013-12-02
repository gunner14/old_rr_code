package com.renren.xce.socialgraph.builder;

import com.renren.xce.socialgraph.common.DataBuilder;
import com.renren.xce.socialgraph.updator.CreateRecommendDataThread;
import com.renren.xce.socialgraph.updator.CreateFFUCRDataThread;

/**
 * BuilderFactory used to create different builder by type
 * @author zhangnan
 * @email zhangnan@renren-inc.com
 */
public class BuilderFactory {
	public static DataBuilder createBuilder(String type) {
		if (type.equals(CreateRecommendDataThread.RFOF)) {
			return new RcdFoFBuilder();			
		}	
		if (type.equals(CreateFFUCRDataThread.FREINDS_CLUSTER)) {
			return new RcdFFUCRBuilder();			
		}
		return null;
	}
}


//public class BuilderFactory {
//	public static DataBuilder createBuilder(String type) {
//		if (type.equals(CreateRecommendDataThread.COMMON_FRIENDS)) {
//			return new CommonFriendsBuilder();
//		} else if (type.equals(CreateRecommendDataThread.FREINDS_CLUSTER)) {
//			return new FriendClusterBuilder();
//		} else if (type.equals(CreateRecommendDataThread.PAGE)) {
//			return new PageBuilder();
//		}	else if (type.equals(CreateRecommendDataThread.INFO)) {
//			return new SameInfoFriendsBuilder();
//		} else if (type.equals(CreateRecommendDataThread.RVIDEO)) {
//			return new VideoRecommendDataBuilder();
//		} else if (type.equals(CreateRecommendDataThread.RBLOG)) {
//			return new BlogRecommendDataBuilder();
//		} else if (type.equals(CreateRecommendDataThread.RSITE)) {
//			return new RcdSiteBuilder();
//		}	else if (type.equals(CreateRecommendDataThread.RAPP)) {
//			return new AppRecommendDataBuilder();
//		} else if (type.equals(CreateRecommendDataThread.RFOF)) {
//			return new RcdFoFBuilder();			
//		} else if (type.equals(CreateRecommendDataThread.RDESK)) {
//			return new DeskRecommendDataBuilder();			
//		} else if (type.equalsIgnoreCase(CreateRecommendDataThread.RFORUM)) {
//			return new ForumRecommendDataBuilder();
//		}
//		
//		return null;
//	}
//}