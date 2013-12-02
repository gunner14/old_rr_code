/*
 * Function: Unit test case of BlockItemsOfRcdItemsDeleted  
 * Devleoper: XiongJunWu
 * EMail: junwu.xiong@renren-inc.com
 * Date: 2012-11-13
 */
package com.renren.xce.socialgraph.data;

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

import com.renren.xce.socialgraph.updator.FoFUpdator;

public class BlockItemsOfRcdItemsDeletedTest {
	public static BlockItemsOfRcdItemsDeletedTest instance = null;
	public final static byte[] kSgBlockHTable = Bytes
			.toBytes(FoFUpdator.kSgBlockHTableName);
	public final static byte[] kSgBlockHTableColumnFamily = Bytes.toBytes("f");
	private static Configuration conf;
	private static HTable sgBlockHTable;

	public static BlockItemsOfRcdItemsDeletedTest getInstance() {
		Clogging.Debug("[BlockItemsOfRcdItemsDeleted] getInstance start");
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
						instance = new BlockItemsOfRcdItemsDeletedTest();
					} catch (Exception e) {
						// TODO: handle exception
						instance = null;
						sgBlockHTable = null;
						e.printStackTrace();
					}
				}
			}
		}
		Clogging.Debug("[BlockItemsOfRcdItemsDeleted] getInstance over");
		return instance;
	}

	// Block HTable record form:
	// key: source_id, qualifier: block_id, value" create_time + "," + business
	// name
	public List<Long> getSgBlockList(int hostId) {
		Clogging.Debug("[getSgBlockList] started");
		List<Long> sgBlockList = new ArrayList<Long>();
		Result values = null;
		try {
			Clogging.Debug("[getSgBlockList] get started");
			Get get = new Get(Bytes.toBytes(String.valueOf(hostId)));
			get.addFamily(kSgBlockHTableColumnFamily);
			values = sgBlockHTable.get(get);
			Clogging.Debug("[getSgBlockList] get over ");
			Clogging.Info("values size " + values.size());
			for (KeyValue kv : values.raw()) {
				sgBlockList.add(Long.valueOf(new String(kv.getQualifier())));
			}
			for (int i = 0; i < values.size(); ++i) {
				Clogging.Info("hostId: " + hostId + " blockId "
						+ sgBlockList.get(i));
			}
			Clogging.Info("sgBlockList size " + sgBlockList.size());
		} catch (IOException e) {
			values = null;
			e.printStackTrace();
		}
		Clogging.Debug("[getSgBlockList] over");
		return sgBlockList;
	}
}
