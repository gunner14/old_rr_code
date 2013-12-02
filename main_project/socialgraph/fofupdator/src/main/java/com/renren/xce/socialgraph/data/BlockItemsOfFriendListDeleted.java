/*
 * Function: SocialGraph blockers of the user's list of friends deleted 
 * from HTable
 * Devleoper: XiongJunWu
 * EMail: junwu.xiong@renren-inc.com
 * Date: 2012-11-28
 */

package com.renren.xce.socialgraph.data;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.KeyValue;
import org.apache.hadoop.hbase.client.Get;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.client.Result;
import org.apache.hadoop.hbase.util.Bytes;

import com.renren.xce.socialgraph.updator.FoFUpdator;

public class BlockItemsOfFriendListDeleted {

	public static BlockItemsOfFriendListDeleted instance = null;
	public final static byte[] kSgFriendListDeletedHTableName = Bytes
			.toBytes(FoFUpdator.kSgFriendListDeletedHTableName);
	public final static byte[] kSgFriendListDeletedHTableColumnFamily = Bytes
			.toBytes("f");
	private static Configuration conf;
	private static HTable friendListHTable;

	public static BlockItemsOfFriendListDeleted getInstance() {
		if (null == instance) {
			synchronized (BlockItemsOfFriendList.class) {
				if (null == instance) {
					try {
						conf = new Configuration();
						HBaseConfiguration.addHbaseResources(conf);
						try {
							friendListHTable = new HTable(conf,
									kSgFriendListDeletedHTableName);
						} catch (Exception e) {
							// TODO: handle exception
							e.printStackTrace();
						}
						instance = new BlockItemsOfFriendListDeleted();
					} catch (Exception e) {
						// TODO: handle exception
						instance = null;
						friendListHTable = null;
						e.printStackTrace();
					}
				}
			}
		}
		return instance;
	}

	public List<Integer> getFriendListDeleted(int hostId) {
		List<Integer> friendListDeleted = new ArrayList<Integer>();
		Result values = null;
		try {
			Get get = new Get(Bytes.toBytes(String.valueOf(hostId)));
			get.addFamily(kSgFriendListDeletedHTableColumnFamily);
			values = friendListHTable.get(get);
			for (KeyValue kv : values.raw()) {
				// if the friend number less than the maximum friend number
				friendListDeleted.add(Integer.valueOf(new String(kv
						.getQualifier())));
			}
		} catch (IOException e) {
			values = null;
			e.printStackTrace();
		}
		return friendListDeleted;
	}
	
	public HashMap<Integer,Set<Integer>> getFriendListDeleted(List<Integer> hostIdList){
		HashMap<Integer,Set<Integer>> friendMap = new HashMap<Integer,Set<Integer>>();
		if(hostIdList == null || hostIdList.size() <= 0){
			return friendMap;
		}
		List<Get> gets = new ArrayList<Get>();
		for(Integer hostId : hostIdList){
				Get get = new Get(Bytes.toBytes(String.valueOf(hostId)));
				get.addFamily(kSgFriendListDeletedHTableColumnFamily);
		}
		Result[] resultArray = null;
		try {
			resultArray = friendListHTable.get(gets);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		if ((resultArray != null) && (resultArray.length > 0)) {
			for (Result result : resultArray) {
				if (result.size() > 0) {
					HashSet<Integer> idSet = new HashSet<Integer>();
					Integer hostId = Bytes.toInt(result.getRow());
					int pos = 0;
					for (KeyValue kv : result.raw()) {
						// if the friend number less than the maximum friend number
						idSet.add(Integer.valueOf(new String(kv
								.getQualifier())));
					}
					friendMap.put(hostId, idSet);
				}
			}
		}
		return friendMap;
	}

}
