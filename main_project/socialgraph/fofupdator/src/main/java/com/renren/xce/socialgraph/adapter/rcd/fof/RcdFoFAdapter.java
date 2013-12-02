/**************************************************************************************************
 * Developer:  Xiongjunwu
 * Email:  	   junwu.xiong@renren-inc.com
 * Function:   Offer friends-of-friends recommendation 
 * Date:  2012-12-05
 **********************************************************************************************************************/

package com.renren.xce.socialgraph.adapter.rcd.fof;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;

import com.renren.xce.socialgraph.block.BlockFetcherWithHTables;
import com.renren.xce.socialgraph.common.BlockFetcher;
import com.renren.xce.socialgraph.data.RcdFoFData;
import com.renren.xce.socialgraph.data.RcdFoFDataUtilOfAdapter;

public class RcdFoFAdapter {
	private static RcdFoFAdapter instance = null;

	private RcdFoFAdapter() {
	}

	public static RcdFoFAdapter getInstance() {
		if (null == instance) {
			synchronized (RcdFoFAdapter.class) {
				if (null == instance) {
					try {
						instance = new RcdFoFAdapter();
					} catch (Exception e) {
						e.printStackTrace();
						instance = null;
					}
				}
			}
		}
		return instance;
	}

	public static List<RcdFoFData> getRcdFoFList(int hostId, int limit)
			throws IOException {
		List<RcdFoFData> rcdFoFRankData = new ArrayList<RcdFoFData>();
		if (hostId <= 0 || limit == 0) {
			return rcdFoFRankData;
		}
		BlockFetcher blockFetcher = new BlockFetcherWithHTables();
		List<Long> blockList = blockFetcher.getBlockList(hostId);
		HashSet<Long> filterSet = new HashSet<Long>();
		filterSet.addAll(blockList);

		List<RcdFoFData> fofList = new ArrayList<RcdFoFData>();
		fofList = RcdFoFDataUtilOfAdapter.getInstance().getRcdFoFData(hostId,
				RcdFoFDataUtilOfAdapter.kFoFMaxLoadNumber);
		if (fofList.isEmpty()) {
			return rcdFoFRankData;
		}
		for (RcdFoFData fof : fofList) {
			int fofId = fof.getFoFId();
			if (!filterSet.contains(fofId)) {
				if (rcdFoFRankData.size() < limit) {
					rcdFoFRankData.add(fof);
				} else {
					return rcdFoFRankData;
				}
			}
		}
		return rcdFoFRankData;
	}

	public static HashMap<Integer, List<RcdFoFData>> getRcdFoFList(
			List<Integer> hostIdList, int limit) throws IOException {
		HashMap<Integer, List<RcdFoFData>> rcdFoFRankData = new HashMap<Integer, List<RcdFoFData>>();
		if (hostIdList == null || hostIdList.size() <= 0 || limit == 0) {
			return rcdFoFRankData;
		}
		BlockFetcher blockFetcher = new BlockFetcherWithHTables();
		HashMap<Integer,Set<Integer>> blockSetMap = blockFetcher.getBlockSet(hostIdList);

		HashMap<Integer, List<RcdFoFData>> fofListMap = new HashMap<Integer, List<RcdFoFData>>();
		fofListMap = RcdFoFDataUtilOfAdapter.getInstance().getRcdFoFData(
				hostIdList, RcdFoFDataUtilOfAdapter.kFoFMaxLoadNumber);
		if (fofListMap.isEmpty()) {
			return rcdFoFRankData;
		}
		Iterator<Integer> iter = fofListMap.keySet().iterator();
		Integer hostId;
		List<RcdFoFData> fofDataList;
		Integer fofId;
		Set<Integer> blockSet = null;
		while(iter.hasNext()){
			hostId = iter.next();
			fofDataList = fofListMap.get(hostId);
			blockSet = blockSetMap.get(hostId);
			ArrayList<RcdFoFData> rcdDataList = new ArrayList<RcdFoFData>();
			for (RcdFoFData fof : fofDataList) {
				fofId = fof.getFoFId();
				if (!blockSet.contains(fofId)) {
					if (rcdDataList.size() < limit) {
						rcdDataList.add(fof);
					} else {
						continue;
					}
				}
			}
			rcdFoFRankData.put(hostId, rcdDataList);
		}
		return rcdFoFRankData;
	}

	@SuppressWarnings("static-access")
	public static void main(String[] args) throws IOException {
		Long startTime = System.currentTimeMillis();
		int hostId = 256404211;
		int limit = 8;
		List<RcdFoFData> rcdFoFRankData = RcdFoFAdapter.getInstance()
				.getRcdFoFList(hostId, limit);
		System.out.println("Recommended fof for user " + hostId + " is: ");
		for (RcdFoFData item : rcdFoFRankData) {
			System.out.println(item.getFoFId());
		}
		Long endTime = System.currentTimeMillis();
		System.out.println("Success! UserId: " + hostId
				+ " Succeeded result data size: " + rcdFoFRankData.size()
				+ " Time cost: " + (endTime - startTime) + "ms");
		
		ArrayList<Integer> hostIdList = new ArrayList<Integer>();
		
		hostIdList.add(256404211);
		hostIdList.add(221177832);
		hostIdList.add(100174017);
		hostIdList.add(100480819);
		hostIdList.add(100576165);
		hostIdList.add(100605351);
		hostIdList.add(100606323);
		hostIdList.add(101034679);
		hostIdList.add(101122437);
		hostIdList.add(101190252);
		hostIdList.add(1011986535);
		hostIdList.add(101297146);
		hostIdList.add(101500022);
		hostIdList.add(101724907);
		hostIdList.add(101746129);
		hostIdList.add(102030898);
		hostIdList.add(102073152);
		hostIdList.add(102641456);
		hostIdList.add(1027036839);
		hostIdList.add(102750491);
		hostIdList.add(102916749);
		hostIdList.add(102990512);
		hostIdList.add(103149407);
		hostIdList.add(103370005);
		hostIdList.add(103443888);
		hostIdList.add(103546686);
		hostIdList.add(103664342);
		hostIdList.add(103697896);
		hostIdList.add(104126096);
		hostIdList.add(104478690);
		hostIdList.add(104527758);
		hostIdList.add(104585358);
		hostIdList.add(1047230507);
		hostIdList.add(104842820);
		hostIdList.add(104921129);
		hostIdList.add(104962214);
		hostIdList.add(105190326);
		hostIdList.add(105236415);
		hostIdList.add(105284908);
		hostIdList.add(105345568);
		hostIdList.add(105397877);
		hostIdList.add(105482430);
		hostIdList.add(105532678);
		hostIdList.add(105582673);
		hostIdList.add(105636286);
		hostIdList.add(105686362);
		hostIdList.add(105703551);
		hostIdList.add(105836608);
		hostIdList.add(106097752);
		hostIdList.add(106154758);
		hostIdList.add(106170840);
		hostIdList.add(106280884);
		hostIdList.add(106289236);
		hostIdList.add(106363909);
		hostIdList.add(106560658);
		hostIdList.add(106623144);
		hostIdList.add(1066635749);
		hostIdList.add(106714440);
		hostIdList.add(106834979);
		hostIdList.add(106903288);
		hostIdList.add(106931115);
		hostIdList.add(106997203);
		hostIdList.add(107061712);
		hostIdList.add(107202481);
		hostIdList.add(107232460);
		hostIdList.add(107365887);
		hostIdList.add(107555219);
		hostIdList.add(107598988);
		hostIdList.add(107618813);
		hostIdList.add(107655795);
		hostIdList.add(107750311);
		hostIdList.add(107790209);
		hostIdList.add(108032902);
		hostIdList.add(1081866146);
		hostIdList.add(108528605);
		hostIdList.add(108756675);
		hostIdList.add(108813031);
		hostIdList.add(108824787);
		hostIdList.add(108929781);
		hostIdList.add(1089338238);
		hostIdList.add(109147968);
		hostIdList.add(109308464);
		hostIdList.add(109596159);
		hostIdList.add(109674035);
		hostIdList.add(109845972);
		hostIdList.add(109991411);
		hostIdList.add(110124954);
		hostIdList.add(110155662);
		hostIdList.add(110156058);
		hostIdList.add(110273517);
		hostIdList.add(110284623);
		hostIdList.add(110320442);
		hostIdList.add(1104070510);
		hostIdList.add(1104672242);
		hostIdList.add(110550085);
		hostIdList.add(110576104);
		hostIdList.add(110616514);
		hostIdList.add(110710708);
		hostIdList.add(110711969);
		hostIdList.add(110716757);
		hostIdList.add(110883716);
		hostIdList.add(111013963);
		hostIdList.add(111015132);
		hostIdList.add(111028552);
		hostIdList.add(1110876571);
		hostIdList.add(1111735231);
		hostIdList.add(111284567);
		hostIdList.add(111659039);
		hostIdList.add(111674843);
		hostIdList.add(111876380);
		hostIdList.add(111902759);
		hostIdList.add(111962474);
		hostIdList.add(112062968);
		hostIdList.add(112165153);
		hostIdList.add(112401051);
		hostIdList.add(112623460);
		hostIdList.add(112681178);
		hostIdList.add(112807501);

		Long s2 = System.currentTimeMillis();
		HashMap<Integer, List<RcdFoFData>> datas = RcdFoFAdapter.getInstance().getRcdFoFList(hostIdList, limit);
		Long s3 = System.currentTimeMillis();
		
		System.out.println("---------Success-!"
				+ " Time cost: " + (s3 - s2) + "ms");
		Iterator<Integer> iter = datas.keySet().iterator(); 
		while(iter.hasNext()){
			Integer host = iter.next();
			List<RcdFoFData> rcdFoFRankDatas = datas.get(host);
			System.out.println("UserId: " + host
					+ " Succeeded result data size: " + rcdFoFRankDatas.size());
			for(RcdFoFData d : rcdFoFRankDatas){
				System.out.println(d.getFoFId() + "\t" + d.getHostId() + "\t" + d.getMutualFriendNumber());
				
			}
		}
	}
}
