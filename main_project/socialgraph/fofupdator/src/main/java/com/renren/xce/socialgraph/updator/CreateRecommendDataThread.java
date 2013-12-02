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
import com.renren.xce.socialgraph.strategy.RcdFoFStrategy;
import com.xiaonei.xce.socialgraph.proto.GeneratedItems.Items;

public class CreateRecommendDataThread implements Runnable {
	public static final String FREINDS_CLUSTER = "FFUCR";
	public static final String RFOF = "FOF";
	private static int expireTime = 0;
	private Integer userId;

	public CreateRecommendDataThread(Integer userId) {
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
			List<DataItem> dataList = builderList.get(i).getData(userId); // create
																			// data
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
			Clogging.Info("FoFUpdator userId: " + userId
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
			Long endTime = System.currentTimeMillis();
			Clogging.Debug("FoFUpdator Success! UserId: " + userId
					+ " Succeeded result data size: " + resultData.length
					+ " Time cost: " + (endTime - startTime) + "ms");
		} else {
			Clogging.Debug("FoFUpdator Failed! UserId: " + userId
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
