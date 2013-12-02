package com.renren.xce.socialgraph.updator;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import xce.socialgraph.util.Clogging;
import xce.tripod.TripodException;
import xce.tripod.client.EntryType;
import xce.tripod.client.TripodCacheClient;
import xce.tripod.client.TripodCacheClientFactory;

import com.google.protobuf.InvalidProtocolBufferException;
import com.renren.xce.socialgraph.block.BlockFetcherWithHTables;
import com.renren.xce.socialgraph.builder.BuilderFactory;
import com.renren.xce.socialgraph.common.BlockFetcher;
import com.renren.xce.socialgraph.common.Category;
import com.renren.xce.socialgraph.common.DataBuilder;
import com.renren.xce.socialgraph.common.DataItem;
import com.renren.xce.socialgraph.common.Strategy;
import com.renren.xce.socialgraph.strategy.RcdFoFStrategy;
import com.xiaonei.xce.socialgraph.proto.GeneratedItems.Item;
import com.xiaonei.xce.socialgraph.proto.GeneratedItems.Items;

public class CreateRecommendDataThreadTest implements Runnable {
	public static final String FREINDS_CLUSTER = "FFUCR";
	public static final String RFOF = "FOF";

	private static int expireTime = 0;
	private Integer userId;

	public CreateRecommendDataThreadTest(Integer userId) {
		this.userId = userId;
	}

	public void run() {
		try {
			// process each user
			Process(this.userId); 
		} catch (Exception e) {
			e.printStackTrace();
			Clogging.Warn("Process::run exception userid:" + this.userId);
		} finally {
			this.userId = null;
		}
	}

	private void Process(int userId) throws TripodException {
		Long startTime = System.currentTimeMillis();

		List<Category> categoryList;
		Strategy strategy = new RcdFoFStrategy();
		categoryList = strategy.getStrategy(userId);
		PrintCategoryList(categoryList);
		// builderList for  creating data by different type
		List<DataBuilder> builderList = new ArrayList<DataBuilder>(); 
		for (Category each : categoryList) {
			DataBuilder builder = BuilderFactory.createBuilder(each.getType());
			builderList.add(builder);
		}

		if (categoryList.size() != builderList.size()) {
			Clogging.Error("[Process] size of categoryList is not as same as size of builderList!!");
			return;
		}
		// buddy by applicant cache data, socialgraph block table and friend list
		// BlockFetcher blockFetcher = new BlockFetchWithBuddyApplicant(); 
		
		// BlockFetcher blockFetcher = new BlockFetchWithOwnerFriends(); //fetch
		// block list from different block sources
		// BlockFetcher blockFetcher = new BlockFetchWithDeletedFriends();
		// buddy by applicant cache data, socialgraph block table and friend list
		BlockFetcher blockFetcher = new BlockFetcherWithHTables();
		List<Long> blockList = blockFetcher.getBlockList(userId); // fetch block list from specific fetcher
		Set<Long> filterSet = new HashSet<Long>();
		filterSet.addAll(blockList);
		Items.Builder result = Items.newBuilder();
		int typeSize = 0; // number of each type, changing according to size of pre-type
		for (int i = 0; i < categoryList.size(); ++i) {
			typeSize += categoryList.get(i).getNum(); // get size of current
														// type
			if (null == builderList.get(i)
					|| categoryList.get(i).getType() != builderList.get(i)
							.getBuilderType()) {
				continue;
			}
			int actualTypeSize = 0;
			List<DataItem> dataList = builderList.get(i).getData(userId); // create data
			if (!dataList.isEmpty()) {
				for (DataItem it : dataList) { // combine data of the type
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
			Clogging.Info("userId: " + userId
					+ " Original data builder list size: " + dataList.size()
					+ " Preallocated type size: " + typeSize
					+ " Actual recommended type size: " + actualTypeSize);
			typeSize = typeSize - actualTypeSize; // left remnant to next type
			Clogging.Info("Original data builder list size: " + dataList.size()
					+ " Preallocated type size: " + typeSize
					+ " Actual recommended type size: " + actualTypeSize);
			dataList = null; // clean
		}
		PrintItems(result);
		byte[] resultData = result.build().toByteArray();
		if (null != resultData && resultData.length != 0) {
			String key = String.valueOf(userId);
			TripodCacheClient tcc = TripodCacheClientFactory
					.getClient(EntryType.BIZ_SOCIALGRAPH_RCD);
			TripodCacheClient tccBak = TripodCacheClientFactory
					.getClient(EntryType.BIZ_SOCIALGRAPH_RCD_BAK);
			try {
				tcc.binarySet(key, resultData, expireTime);
				tccBak.binarySet(key, resultData, expireTime);
				Clogging.Info("key " + key + " " + " result length "
						+ resultData.length + " value size "
						+ tcc.binaryGet(String.valueOf(userId)).length);
			} catch (TripodException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			Items items;
			try {
				items = Items.parseFrom(tcc.binaryGet(String.valueOf(userId)));
				System.out.println("hostId " + userId + " item count "
						+ items.getItemsCount());
				for (Item item : items.getItemsList()) {
					if (CreateRecommendDataThread.RFOF
							.equals(item.getMessage())) {
						System.out.print(" " + item.getId());
						System.out.println("hostId " + userId + " fofId "
								+ item.getId() + " rank " + item.getMessages(0)
								+ " mut num " + item.getField());
						for (int i : item.getFieldsList()) {
							System.out.print(" " + i);
						}
					}
				}
				System.out.println("");
			} catch (InvalidProtocolBufferException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

			tcc.remove(key);
			System.out.println("key " + key + " " + " value "
					+ tcc.binaryGet(String.valueOf(userId)));
			/*TripodCacheClient client = new TripodCacheClient("xce_misc", "like");
			client.binarySet("test1", new String("aa bb").getBytes(), 0);
			System.out.println(new String(client.binaryGet("test1")));
			client.remove("test1");
			System.out.println(client.binaryGet("test1"));
			client.binarySet("test1", new String("aa bb").getBytes(), 0);
			client.binarySet("test2", new String("cc dd").getBytes(), 0);
			List<String> keys = new ArrayList<String>();
			keys.add("test1");
			keys.add("test2");
			System.out.println(client.binaryMultiGet(keys));
			Long endTime = System.currentTimeMillis();
			Clogging.Debug("Success! UserId: " + userId
					+ " Succeeded result data size: " + resultData.length
					+ " Time cost: " + (endTime - startTime) + "ms");*/
		} else {
			Clogging.Debug("Failed! UserId: " + userId
					+ " Failed result data size: " + resultData.length);
		}
		result = null;
		resultData = null;
		strategy = null;
		categoryList = null;
		builderList = null;
		blockFetcher = null;
		filterSet = null;
		blockList = null;
	}

	private void PrintCategoryList(List<Category> categoryList) {
		for (Category category : categoryList) {
			Clogging.Debug(category.getType() + " : " + category.getNum());
		}
	}

	private void PrintItems(Items.Builder result) {
		for (Item item : result.getItemsList()) {
			Clogging.Debug(item.getMessage());
			Clogging.Debug("" + item.getId());
			Clogging.Debug("" + item.getField());
			String messagesList = "";
			for (String mes : item.getMessagesList()) {
				messagesList += mes + " ";
			}
			Clogging.Debug(messagesList);
			String fieldsList = "";
			for (Integer fid : item.getFieldsList()) {
				fieldsList += fid + " ";
			}
			Clogging.Debug(fieldsList);
		}
	}
}
