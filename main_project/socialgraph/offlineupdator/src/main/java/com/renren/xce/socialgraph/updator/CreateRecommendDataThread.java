package com.renren.xce.socialgraph.updator;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.jruby.RubyProcess.Sys;

import xce.socialgraph.util.Clogging;
import xce.tripod.TripodException;
import xce.tripod.client.EntryType;
import xce.tripod.client.TripodCacheClient;
import xce.tripod.client.TripodCacheClientFactory;

import com.google.protobuf.InvalidProtocolBufferException;
import com.renren.xce.socialgraph.block.BlockFetchWithBuddyApplicant;
import com.renren.xce.socialgraph.block.BlockFetchWithDeletedFriends;
import com.renren.xce.socialgraph.block.BlockFetchWithOwnerFriends;
import com.renren.xce.socialgraph.builder.BuilderFactory;
import com.renren.xce.socialgraph.common.BlockFetcher;
import com.renren.xce.socialgraph.common.Category;
import com.renren.xce.socialgraph.common.DataBuilder;
import com.renren.xce.socialgraph.common.DataItem;
import com.renren.xce.socialgraph.common.Strategy;
import com.renren.xce.socialgraph.strategy.RcdFoFStrategy;
import com.xiaonei.xce.socialgraph.proto.GeneratedItems.Items;

/**
 * 创建用户推荐数据线程
 * @author zhangnan
 */
public class CreateRecommendDataThread implements Runnable {
	public static final String USER_FEATURE = "USERFEATURE";		//数据源
	//public static final String RECOMMEND_DATA = "RCD";
	//public static final String RECOMMEND_DATA = "FOF";
	public static final String FREINDS_CLUSTER = "FFUCR";
	public static final String COMMON_FRIENDS = "FFW";
	public static final String NEW_COMMER = "FFNCW";
	public static final String RECONNECT = "FFWR";
	public static final String PAGE = "PAGE";
	public static final String INFO = "INFO";
	public static final String RVIDEO = "RCDVIDEO";
	public static final String RBLOG = "RCDBLOG";
	public static final String RAPP = "RCDAPP";
	public static final String RFORUM = "RCDFORUM";
	  
	public static final String AUTO_GROUP = "FFAG";
	public static final String SAME_AREA = "OFSA";
	public static final String BLOG = "BLOG";
	public static final String BLOG_TAG = "BLOGTAG";
	public static final String BLOG_ACTION = "BLOGACTION";  
	public static final String RSITE = "RCDSITE";
	public static final String RDESK = "RCDDESK";
	
	public static final String RFOF = "FOF";
//	public static int kActualRcdSize = 0;
	
	  
    private static int expireTime = 0;
	  private Integer userId;
	
	public CreateRecommendDataThread(Integer userId) {
		this.userId = userId;
	}

	public void run() {
		try {
			Process(this.userId); 		// process each user
//			Thread.sleep(200000);
		} catch (Exception e) {
			e.printStackTrace();
			Clogging.Warn("Process::run exception userid:" + this.userId);
		} finally {
			this.userId = null;
		}
	}
	
	/**
	 * process each person 
	 * @param userId
	 */
	private void Process(int userId) {
		Long startTime = System.currentTimeMillis();
		
		List<Category> categoryList;
		Strategy strategy = new RcdFoFStrategy();		
		categoryList = strategy.getStrategy(userId);
//		PrintCategoryList(categoryList);		
		List<DataBuilder> builderList = new ArrayList<DataBuilder>();  	//builderList for creating data by different type
		for (Category each : categoryList) {
			DataBuilder builder = BuilderFactory.createBuilder(each.getType());
			builderList.add(builder);
		}
		
		if (categoryList.size() != builderList.size()) {
			Clogging.Error("[Process] size of categoryList is not as same as size of builderList!!");
			return;
		}
		
//		BlockFetcher blockFetcher = new BlockFetchWithBuddyApplicant(); //buddy by applicant cache data, socialgraph block table and friend list
//		BlockFetcher blockFetcher = new BlockFetchWithOwnerFriends(); //fetch block list from different block sources  
		BlockFetcher blockFetcher = new BlockFetchWithDeletedFriends(); //buddy by applicant cache data, socialgraph block table and friend list
		List<Long> blockList = blockFetcher.getBlockList(userId);		//fetch block list from specific fetcher
		Set<Long> filterSet = new HashSet<Long>();
		filterSet.addAll(blockList);	
		Items.Builder result = Items.newBuilder();
		int typeSize = 0;		//number of each type, changing according to size of pre-type 
		for (int i = 0; i < categoryList.size(); ++i) {
			typeSize += categoryList.get(i).getNum();			//get size of current type
			if (null == builderList.get(i)
					|| categoryList.get(i).getType() != builderList.get(i)
							.getBuilderType()) {
				continue;
			}
			int actualTypeSize = 0;
			List<DataItem> dataList = builderList.get(i).getData(userId);			//create data
			if (!dataList.isEmpty()) {
				for (DataItem it : dataList) {				//combine data of the type
					if (filterSet.contains(it.getId())) {
						continue;
					} else {
						if (actualTypeSize < typeSize) {
							filterSet.add(it.getId());
							result.addItems(it.getItem());
							++actualTypeSize;
						} else {
							break;
						}
					}
				}	
			}			
			Clogging.Info("userId: " + userId + " Original data builder list size: " + dataList.size() + " Preallocated type size: " + 
			typeSize + " Actual recommended type size: " + actualTypeSize);
			typeSize = typeSize - actualTypeSize;		//left remnant to next type 
//			if (typeSize > 0) {
//				OfflineUpdator.kMaxFoFRcdNumber = (typeSize + typeSize) > OfflineUpdator.kGroupSize ? OfflineUpdator.kGroupSize : (typeSize + typeSize);
////				kActualRcdSize = (typeSize + typeSize) > OfflineUpdator.kGroupSize ? OfflineUpdator.kGroupSize : (typeSize + typeSize);
//				Process(this.userId); 	
//				dataList = null;			//clean
//				break;
//			}
//			Clogging.Info("Original data builder list size: " + dataList.size() + " Preallocated type size: " + typeSize + " Actual recommended type size: " + actualTypeSize);
			dataList = null;			//clean
		}
//		PrintItems(result);
		byte[] resultData = result.build().toByteArray();		
//		boolean insert = false;	
		if (null != resultData && resultData.length != 0) {
			String key = String.valueOf(userId);
			TripodCacheClient tcc = TripodCacheClientFactory.getClient(EntryType.BIZ_SOCIALGRAPH_RCD);		
			try {
				tcc.binarySet(key, resultData, expireTime);
			} catch (TripodException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
//			Clogging.Info("key " + key + " " + " result length " + resultData.length +" value size " + tcc.binaryGet(String.valueOf(userId)).length);			
//			Items items;
//			try {
//				items = Items.parseFrom(tcc.binaryGet(String.valueOf(userId)));
//				Clogging.Info("item count " + items.getItemsCount());
//				for (Item item: items.getItemsList()) {
//					if (CreateRecommendDataThread.RFOF.equals(item.getMessage())) {
//						System.out.print(" " + item.getId());
////						Clogging.Info("hostId " + userId + " fofId " + item.getId() + " rank " + item.getMessages(0) + " mut num " + item.getField());
////						for (int i : item.getFieldsList()) {
////							Clogging.Info(" " + i);
////						}
//					}
//				}
//			} catch (InvalidProtocolBufferException e) {
//				// TODO Auto-generated catch block
//				e.printStackTrace();
//			}				

//			tcc.remove(key);
//			System.out.println("key " + key + " " + " value " + tcc.binaryGet(String.valueOf(userId)));
//	    	TripodCacheClient client = new TripodCacheClient("xce_misc", "like");
//	    	client.binarySet("test1", new String("aa bb").getBytes(), 0);
//	    	System.out.println(new String(client.binaryGet("test1")));
//	    	client.remove("test1");
//	    	System.out.println(client.binaryGet("test1"));
//	    	client.binarySet("test1", new String("aa bb").getBytes(), 0);
//	    	client.binarySet("test2", new String("cc dd").getBytes(), 0);
//	    	List<String> keys = new LinkedList<String>();
//	    	keys.add("test1");
//	    	keys.add("test2");
//	    	System.out.pringetFoFPositionListMaptln(client.binaryMultiGet(keys));	
			Long endTime = System.currentTimeMillis();	
			Clogging.Debug("Success! UserId: " + userId + " Succeeded result data size: "  + resultData.length + " Time cost: " + (endTime - startTime) + "ms");			
		}	else {
			Clogging.Debug("Failed! UserId: " + userId + " Failed result data size: "  + resultData.length);			
		}
//	          + resultData.length + " cost_time:" + (endTime - startTime) + "ms");
//    if (insert) {
//      Clogging.Debug("userId:" + userId + " Success resultData size:"
//          + resultData.length + " cost_time:" + (endTime - startTime) + "ms");
//    } else {
//      Clogging.Debug("userId:" + userId + " Fail resultData size:"
//          + resultData.length + " cost_time:" + (endTime - startTime) + "ms");
//    }
    
			result = null;			//clean
			resultData = null;
			strategy = null;
			categoryList = null;
			builderList = null;
			blockFetcher = null;
			filterSet = null;
			blockList = null;
	}
	
	/**
	 * Add function of white list
	 * @param userId
	 * @return
	 */
//	private boolean isInWhiteList(int userId) {
//		boolean result = false;
//		
//		Set<Integer> whiteSet = new HashSet<Integer>();
//
//		//QA test Ids 
//		whiteSet.add(390205330);
//		whiteSet.add(385971452);
//		whiteSet.add(388144806);
//		whiteSet.add(391930993);
//		whiteSet.add(222677625);		//高雅楠
//		whiteSet.add(238111132);		//张楠
//		whiteSet.add(44674);			//张叶银
//		whiteSet.add(256404211);		//熊君武
//		whiteSet.add(231815969);		//袁源
//		whiteSet.add(258276022);		//张子忠
//		whiteSet.add(250137860);		//谭译泽
//		
////		whiteSet.add(229406110);		//李伟
////		whiteSet.add(128487631);		//张铁安
////		whiteSet.add(85432256);		//黄晶
////		whiteSet.add(142443436);		//洪振亚
////		whiteSet.add(299213989);		//李金山
//
//		if (whiteSet.contains(userId)) {
//			result = true;
//		}
//		return result;
//	}

//  For test
//	private void PrintCategoryList(List<Category> categoryList) {
//		for (Category category : categoryList) {
//			Clogging.Debug(category.getType() + " : " + category.getNum());
//		}
//	}
	
//	private void PrintItems(Items.Builder result) {
//		for (Item item : result.getItemsList()) {
//			Clogging.Debug(item.getMessage());
//			Clogging.Debug("" + item.getId());
//			Clogging.Debug("" + item.getField());
//			String messagesList = "";
//			for (String mes : item.getMessagesList()) {
//				messagesList += mes + " ";
//			}
//			Clogging.Debug(messagesList);
//			String fieldsList = "";
//			for (Integer fid : item.getFieldsList()) {
//				fieldsList += fid + " ";
//			}
//			Clogging.Debug(fieldsList);
//			
//			Clogging.Debug("---------------------------------------------------------------------");
//		}
//	}
}
