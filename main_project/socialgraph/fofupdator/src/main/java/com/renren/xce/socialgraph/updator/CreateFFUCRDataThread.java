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

import com.renren.xce.socialgraph.block.BlockFetcherWithHTables;
import com.renren.xce.socialgraph.builder.BuilderFactory;
import com.renren.xce.socialgraph.common.BlockFetcher;
import com.renren.xce.socialgraph.common.Category;
import com.renren.xce.socialgraph.common.DataBuilder;
import com.renren.xce.socialgraph.common.DataItem;
import com.renren.xce.socialgraph.common.Strategy;
import com.renren.xce.socialgraph.strategy.RcdFFUCRStrategy;
import com.xiaonei.xce.socialgraph.proto.GeneratedItems.Items;

public class CreateFFUCRDataThread implements Runnable {
	public static final String FREINDS_CLUSTER = "FFUCR";

	public static final String RFOF = "FOF";

	private static int expireTime = 0;
	private Integer userId;

	public CreateFFUCRDataThread(Integer userId) {
		this.userId = userId;
	}

	public void run() {
		try {
			Process(this.userId); // process each user
		} catch (Exception e) {
			e.printStackTrace();
			Clogging.Warn("Process::run exception userid:" + this.userId);
		} finally {
			this.userId = null;
		}
	}

	private void Process(int userId) {
		Long startTime = System.currentTimeMillis();

		List<Category> categoryList;
		Strategy strategy = new RcdFFUCRStrategy();
		categoryList = strategy.getStrategy(userId);
		// builderList for creating data by different type
		List<DataBuilder> builderList = new ArrayList<DataBuilder>();
		for (Category each : categoryList) {
			DataBuilder builder = BuilderFactory.createBuilder(each.getType());
			builderList.add(builder);
		}

		if (categoryList.size() != builderList.size()) {
			Clogging.Error("[Process] size of categoryList is not as same as size of builderList!!");
			return;
		}

		BlockFetcher blockFetcher = new BlockFetcherWithHTables();
		// fetch block list from specific fetcher
		List<Long> blockList = blockFetcher.getBlockList(userId);
		
		Set<Long> filterSet = new HashSet<Long>();
		filterSet.addAll(blockList);
		Items.Builder result = Items.newBuilder();
		// number of each type, changing according to size of pre-type
		int typeSize = 0;

		for (int i = 0; i < categoryList.size(); ++i) {
			// get size of current type
			typeSize += categoryList.get(i).getNum();

			if (null == builderList.get(i)
					|| categoryList.get(i).getType() != builderList.get(i)
							.getBuilderType()) {
				continue;
			}
			int actualTypeSize = 0;

			// create data
			List<DataItem> dataList = builderList.get(i).getData(userId);
			if (!dataList.isEmpty()) {
				for (DataItem it : dataList) {
					// combine data of the type
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
			dataList = null;
		}
		byte[] resultData = result.build().toByteArray();
		if (null != resultData && resultData.length != 0) {
			String key = String.valueOf(userId);
			TripodCacheClient tcc = TripodCacheClientFactory
					.getClient(EntryType.BIZ_SOCIALGRAPH_FRIEND_CLUSTER);
			try {
				tcc.binarySet(key, resultData, expireTime);
				Clogging.Info("key " + key + " " + " result length "
						+ resultData.length + " value size "
						+ tcc.binaryGet(String.valueOf(userId)).length);
			} catch (TripodException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			// Items items;
			// try {
			// items = Items.parseFrom(tcc.binaryGet(String.valueOf(userId)));
			// Clogging.Info("item count " + items.getItemsCount());
			// for (Item item: items.getItemsList()) {
			// if (CreateRecommendDataThread.RFOF.equals(item.getMessage())) {
			// System.out.print(" " + item.getId());
			// // Clogging.Info("hostId " + userId + " fofId " + item.getId() +
			// " rank " + item.getMessages(0) + " mut num " + item.getField());
			// // for (int i : item.getFieldsList()) {
			// // Clogging.Info(" " + i);
			// // }
			// }
			// }
			// } catch (InvalidProtocolBufferException e) {
			// // TODO Auto-generated catch block
			// e.printStackTrace();
			// }

			// tcc.remove(key);
			// System.out.println("key " + key + " " + " value " +
			// tcc.binaryGet(String.valueOf(userId)));
			// TripodCacheClient client = new TripodCacheClient("xce_misc",
			// "like");
			// client.binarySet("test1", new String("aa bb").getBytes(), 0);
			// System.out.println(new String(client.binaryGet("test1")));
			// client.remove("test1");
			// System.out.println(client.binaryGet("test1"));
			// client.binarySet("test1", new String("aa bb").getBytes(), 0);
			// client.binarySet("test2", new String("cc dd").getBytes(), 0);
			// List<String> keys = new LinkedList<String>();
			// keys.add("test1");
			// keys.add("test2");
			// System.out.pringetFoFPositionListMaptln(client.binaryMultiGet(keys));
			Long endTime = System.currentTimeMillis();
			Clogging.Debug("FFUCR Success! UserId: " + userId
					+ " Succeeded result data size: " + resultData.length
					+ " Time cost: " + (endTime - startTime) + "ms");
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
}
