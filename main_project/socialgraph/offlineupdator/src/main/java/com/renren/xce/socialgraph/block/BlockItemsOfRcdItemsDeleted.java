/*
 * Function: SocialGraph blockers of the user's deleted recommended friends
 * pages, minisites etc. from HTable 
 * Devleoper: XiongJunWu
 * EMail: junwu.xiong@renren-inc.com
 * Date: 2012-11-13
 */
package com.renren.xce.socialgraph.block;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.KeyValue;
import org.apache.hadoop.hbase.client.Get;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.client.Result;
import org.apache.hadoop.hbase.util.Bytes;

import xce.socialgraph.util.Clogging;

import com.renren.xce.socialgraph.builder.FoFHTable;
import com.renren.xce.socialgraph.updator.FoFUpdator;

public class BlockItemsOfRcdItemsDeleted {
	public static BlockItemsOfRcdItemsDeleted instance = null;
	public final static byte[] kSgBlockHTable = Bytes.toBytes(FoFUpdator.kSgBlockHTableName);
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
	
	//Block HTable record form: 
	//key: source_id, qualifier: block_id, value" create_time + "," + business name
	public List<Long> getSgBlockList(int hostId) {
		List<Long> sgBlockList = new ArrayList<Long>();
		Result values = null;
		try {
			Get get = new Get(Bytes.toBytes(String.valueOf(hostId)));
			get.addFamily(kSgBlockHTableColumnFamily);
			get.setCacheBlocks(false);
			values = sgBlockHTable.get(get);
			Clogging.Info("values size " + values.size());
			for (KeyValue kv : values.raw()) {
//				byte[] bytes = new byte[Bytes.SIZEOF_LONG];
//				String str = kv.getKeyString();
//				Clogging.Info("str " + new String(kv.getQualifier()));
//				System.arraycopy(kv.getQualifier(), 0, bytes, 0, Bytes.SIZEOF_LONG);
				sgBlockList.add(Long.valueOf(new String(kv.getQualifier())));
			}			
//			for (int i = 0 ; i < values.size(); ++i) {
//				Clogging.Info("hostId: " + hostId + " blockId " + sgBlockList.get(i));
//			}
//			System.out.println("sgBlockList size " +  sgBlockList.size());
		} catch (IOException e) {
			values = null;
			e.printStackTrace();
		}		
	    return sgBlockList;
	}
	
	
	

}
