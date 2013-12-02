/*
 * Function: SocialGraph FoF Adapter providing recommendation data
 * Developer: xiongjunwu
 * Email: Junwu.xiong@renren-inc.com
 * Date: 2012-12-05
 */

package com.renren.xce.socialgraph.data;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.NavigableMap;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.client.Get;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.client.Result;
import org.apache.hadoop.hbase.util.Bytes;
import org.apache.hadoop.hbase.util.MD5Hash;

import com.renren.xce.socialgraph.adapter.rcd.fof.RcdFoFAdapter;
import com.renren.xce.socialgraph.data.FoFRow.FoFGroup;

import xce.socialgraph.util.Clogging;

public class RcdFoFDataUtilOfAdapter {
	public final static int kFoFMaxLoadNumber = 100;
	private final static int kDisplayMutualFriendNumber = 10;
	private final static String kRcdFoFHTableStringName = "fof_1000group_incr2";
	public final static byte[] kFoFHTable = Bytes
			.toBytes(kRcdFoFHTableStringName);
	public final static byte[] kColumnFamily = Bytes.toBytes("f");
	private final static int kFoFGroupSize = 100;
	private final static int kRecordSize = Bytes.SIZEOF_INT
			+ Bytes.SIZEOF_FLOAT + Bytes.SIZEOF_INT
			+ kDisplayMutualFriendNumber * Bytes.SIZEOF_INT;

	private static List<FoFGroup> fofGroupList;

	private static RcdFoFDataUtilOfAdapter instance = null;
	private static Configuration conf;
	private static HTable fofTable;

	private RcdFoFDataUtilOfAdapter() {
	}

	public static RcdFoFDataUtilOfAdapter getInstance() {
		if (null == instance) {
			synchronized (RcdFoFDataUtilOfAdapter.class) {
				if (null == instance) {
					try {
						conf = new Configuration();
						HBaseConfiguration.addHbaseResources(conf);
						// fofTable = new HTable(conf, kRcdFoFHTableStringName);
						FoFGroup fofGroup = new FoFRow().new FoFGroup();
						fofGroup.groupId = 0;
						fofGroup.innerGroupStartIndex = 0;
						fofGroup.innerGroupEndIndex = 100;
						fofGroupList = new ArrayList<FoFGroup>();
						fofGroupList.add(fofGroup);
						instance = new RcdFoFDataUtilOfAdapter();
					} catch (Exception e) {
						e.printStackTrace();
						fofTable = null;
						instance = null;
					}
				}
			}
		}
		return instance;
	}

	public static List<RcdFoFData> getInnerGroupRecords(int hostId, int limit,
			byte[] value, int leftPos, int rightPos) {
		List<RcdFoFData> fofList = new ArrayList<RcdFoFData>();
		if ((hostId < 0) || (value.length == 0) || (leftPos < 0)
				|| (leftPos < 0) || (leftPos > rightPos)) {
			Clogging.Error("getInnerGroupRecords: Wrong group left and right position parameters!");
			return fofList;
		}
		int fofCounter = 0;
		for (int j = leftPos; j < rightPos; ++j) {
			RcdFoFData ffrd = new RcdFoFData();
			ffrd.setHostId(hostId);

			int basePos = (j - leftPos) * kRecordSize;
			if (basePos < value.length) {
				int fofId = Bytes.toInt(value, basePos, Bytes.SIZEOF_INT);
				ffrd.setFoFId(fofId);
				float rank = Bytes.toFloat(value, basePos + Bytes.SIZEOF_FLOAT);
				ffrd.setRank(rank);
				int mutualFriendNumber = Bytes.toInt(value, basePos
						+ Bytes.SIZEOF_INT + Bytes.SIZEOF_FLOAT,
						Bytes.SIZEOF_INT);
				ffrd.setMutualFriendNumber(mutualFriendNumber);
				int idBasePos = basePos + Bytes.SIZEOF_INT + Bytes.SIZEOF_FLOAT
						+ Bytes.SIZEOF_INT;
				if (mutualFriendNumber > 0) {
					int actualDispNumber = mutualFriendNumber > kDisplayMutualFriendNumber ? kDisplayMutualFriendNumber
							: mutualFriendNumber;
					for (int k = 0; k < actualDispNumber; ++k) {
						int mutualFriendId = Bytes.toInt(value, idBasePos + k
								* Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
						if (mutualFriendId > 0) {
							ffrd.getmutualFriendIdList().add(mutualFriendId);
						}
					}
				}
				fofList.add(ffrd);
				fofCounter++;
				if (limit > 0 && fofCounter >= limit) {
					return fofList;
				}
				if ((limit < 0 && fofCounter >= RcdFoFDataUtilOfAdapter.kFoFMaxLoadNumber)) {
					if (limit > 0
							&& limit >= RcdFoFDataUtilOfAdapter.kFoFMaxLoadNumber) {
						return fofList;
					}
				}
			} else {
				break;
			}
		}
		return fofList;
	}

	public List<RcdFoFData> getRcdFoFData(int hostId, int limit)
			throws IOException {
		List<RcdFoFData> fofList = new ArrayList<RcdFoFData>();
		if (hostId < 0) {
			return fofList;
		}
		String keyString = String.valueOf(hostId);
		String hash = MD5Hash.getMD5AsHex(keyString.getBytes());
		String rehash = MD5Hash.getMD5AsHex((hash + keyString).getBytes());
		String key = rehash.substring(0, 10).toUpperCase() + keyString;
		List<Get> getList = new ArrayList<Get>();
		fofTable = new HTable(conf, kRcdFoFHTableStringName);
		Result[] resultArray = null;
		if (!fofGroupList.isEmpty()) {
			for (FoFGroup groupItem : fofGroupList) {
				byte[] qualifier = Bytes.toBytes(groupItem.groupId
						* kFoFGroupSize);
				Get get = new Get(key.getBytes());
				get.setCacheBlocks(false);
				get.addColumn(kColumnFamily, qualifier);
				getList.add(get);
			}
		} else {
			Clogging.Error("getRcdFoFData: fofGroupIdList is empty!");
			return fofList;
		}
		try {
			resultArray = fofTable.get(getList);
			if ((resultArray != null) && (resultArray.length > 0)) {
				for (Result result : resultArray) {
					if (result.size() > 0) {
						NavigableMap<byte[], byte[]> fofQualifierValueMap = result
								.getFamilyMap(kColumnFamily);
						for (FoFGroup groupItem : fofGroupList) {
							byte[] qualifier = Bytes.toBytes(groupItem.groupId
									* kFoFGroupSize);
							if (fofQualifierValueMap.size() > 0) {
								if (fofQualifierValueMap.containsKey(qualifier)) {
									byte[] value = fofQualifierValueMap
											.get(qualifier);
									int leftPosition = groupItem.groupId
											* kFoFGroupSize
											+ groupItem.innerGroupStartIndex;
									int rightPosition = groupItem.groupId
											* kFoFGroupSize
											+ groupItem.innerGroupEndIndex;
									List<RcdFoFData> subFoFList = new ArrayList<RcdFoFData>();
									if (value.length > 0) {
										subFoFList = getInnerGroupRecords(
												hostId, limit, value,
												leftPosition, rightPosition);
									}
									if (!subFoFList.isEmpty()) {
										fofList.addAll(subFoFList);
									}
								}
							}
						}
					}
				} // end for Result
			} else { // end if resultArray
				Clogging.Error("getRcdFoFData: Fecthing result fof data from hbase failed, result array length: "
						+ resultArray.length);
				return fofList;
			}
		} catch (IOException e) {
			e.printStackTrace();
			return fofList;
		}
		return fofList;
	}

	public HashMap<Integer, List<RcdFoFData>> getRcdFoFData(
			List<Integer> hostIdList, int limit) throws IOException {
		HashMap<Integer, List<RcdFoFData>> fofListMap = new HashMap<Integer, List<RcdFoFData>>();
		if (hostIdList == null || hostIdList.size() <= 0) {
			return fofListMap;
		}
		List<Get> getList = new ArrayList<Get>();
		HashMap<String, Integer> keyMap = new HashMap<String, Integer>(); 
		fofTable = new HTable(conf, kRcdFoFHTableStringName);
		for(Integer hostId : hostIdList){
			String keyString = String.valueOf(hostId);
			String hash = MD5Hash.getMD5AsHex(keyString.getBytes());
			String rehash = MD5Hash.getMD5AsHex((hash + keyString).getBytes());
			String key = rehash.substring(0, 10).toUpperCase() + keyString;
			keyMap.put(key, hostId);
			Get get = new Get(key.getBytes());
			get.setCacheBlocks(false);
			get.addFamily(kColumnFamily);
			getList.add(get);
		}
		Result[] resultArray = null;
		try {
			resultArray = fofTable.get(getList);
			if ((resultArray != null) && (resultArray.length > 0)) {
				for (Result result : resultArray) {
					if (result.size() > 0) {
						NavigableMap<byte[], byte[]> fofQualifierValueMap = result
								.getFamilyMap(kColumnFamily);
						int hostId = keyMap.get(Bytes.toString(result.getRow()));  
						for (FoFGroup groupItem : fofGroupList) {
							byte[] qualifier = Bytes.toBytes(groupItem.groupId
									* kFoFGroupSize);
							if (fofQualifierValueMap.size() > 0) {
								if (fofQualifierValueMap.containsKey(qualifier)) {
									byte[] value = fofQualifierValueMap
											.get(qualifier);
									int leftPosition = groupItem.groupId
											* kFoFGroupSize
											+ groupItem.innerGroupStartIndex;
									int rightPosition = groupItem.groupId
											* kFoFGroupSize
											+ groupItem.innerGroupEndIndex;
									List<RcdFoFData> subFoFList = new ArrayList<RcdFoFData>();
									if (value.length > 0) {
										subFoFList = getInnerGroupRecords(
												hostId, limit, value,
												leftPosition, rightPosition);
									}
									if (!subFoFList.isEmpty()) {
										fofListMap.put(hostId, subFoFList);
									}
								}
							}
						}
					}
				} // end for Result
			} else { // end if resultArray
				Clogging.Error("getRcdFoFData: Fecthing result fof data from hbase failed, result array length: "
						+ resultArray.length);
				return fofListMap;
			}
		} catch (IOException e) {
			e.printStackTrace();
			return fofListMap;
		}
		return fofListMap;
	}
	
	public static void main(String[]args){
		ArrayList<Integer> hostIdList = new ArrayList<Integer>();
		hostIdList.add(256404211);
		hostIdList.add(221177832);
		HashMap<Integer, List<RcdFoFData>> datas;
		try {
			Long s2 = System.currentTimeMillis();
			datas = RcdFoFDataUtilOfAdapter.getInstance().getRcdFoFData(hostIdList, 100);
			Long s3 = System.currentTimeMillis();
			System.out.println("---------Success-!"
					+ " Time cost: " + (s3 - s2) + "ms");
			for(Integer host : hostIdList){
				List<RcdFoFData> rcdFoFRankData = datas.get(host);
				System.out.println("UserId: " + host
						+ " Succeeded result data size: " + rcdFoFRankData.size());
				for(RcdFoFData d : rcdFoFRankData){
					System.out.println(d.getFoFId() + "\t" + d.getHostId() + "\t" + d.getMutualFriendNumber());
					
				}
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
}
