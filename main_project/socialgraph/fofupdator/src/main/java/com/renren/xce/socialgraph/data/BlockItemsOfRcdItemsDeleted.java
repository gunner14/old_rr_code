/*
 * Function: SocialGraph blockers of the user's deleted recommended friends
 * pages, minisites etc. from HTable 
 * Devleoper: XiongJunWu
 * EMail: junwu.xiong@renren-inc.com
 * Date: 2012-11-13
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

public class BlockItemsOfRcdItemsDeleted {
	public static BlockItemsOfRcdItemsDeleted instance = null;
	public final static byte[] kSgBlockHTable = Bytes
			.toBytes(FoFUpdator.kSgBlockHTableName);
	public final static byte[] kSgBlockHTableColumnFamily = Bytes.toBytes("f");
	private static Configuration conf;
	private static HTable sgBlockHTable;

	public static BlockItemsOfRcdItemsDeleted getInstance() {
		if (null == instance) {
			synchronized (BlockItemsOfFriendList.class) {
				if (null == instance) {
					try {
						conf = new Configuration();
						HBaseConfiguration.addHbaseResources(conf);
						try {
							sgBlockHTable = new HTable(conf, kSgBlockHTable);
						} catch (Exception e) {
							// TODO: handle exception
							e.printStackTrace();
						}
						instance = new BlockItemsOfRcdItemsDeleted();
					} catch (Exception e) {
						// TODO: handle exception
						instance = null;
						sgBlockHTable = null;
						e.printStackTrace();
					}
				}
			}
		}
		return instance;
	}

	// Block HTable record form:
	// key: source_id, qualifier: block_id, value" create_time + "," + business
	// name
	public List<Long> getSgBlockList(int hostId) {
		List<Long> sgBlockList = new ArrayList<Long>();
		Result values = null;
		try {
			Get get = new Get(Bytes.toBytes(String.valueOf(hostId)));
			get.addFamily(kSgBlockHTableColumnFamily);
			values = sgBlockHTable.get(get);
			for (KeyValue kv : values.raw()) {
				sgBlockList.add(Long.valueOf(new String(kv.getQualifier())));
			}
		} catch (IOException e) {
			values = null;
			e.printStackTrace();
		}
		return sgBlockList;
	}
	
	public HashMap<Integer,Set<Integer>> getSgBlockList(List<Integer> hostIdList){
		HashMap<Integer,Set<Integer>> sgBlockMap = new HashMap<Integer,Set<Integer>>();
		if(hostIdList == null || hostIdList.size() <= 0){
			return sgBlockMap;
		}
		List<Get> gets = new ArrayList<Get>();
		for(Integer hostId : hostIdList){
				Get get = new Get(Bytes.toBytes(String.valueOf(hostId)));
				get.addFamily(kSgBlockHTableColumnFamily);
		}
		Result[] resultArray = null;
		try {
			resultArray = sgBlockHTable.get(gets);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		if ((resultArray != null) && (resultArray.length > 0)) {
			for (Result result : resultArray) {
				if (result.size() > 0) {
					HashSet<Integer> idSet = new HashSet<Integer>();
					Integer hostId = Bytes.toInt(result.getRow());
					for (KeyValue kv : result.raw()) {
						idSet.add(Integer.valueOf(new String(kv.getQualifier())));
					}
					sgBlockMap.put(hostId, idSet);
				}
			}
		}
		return sgBlockMap;
	}
}
