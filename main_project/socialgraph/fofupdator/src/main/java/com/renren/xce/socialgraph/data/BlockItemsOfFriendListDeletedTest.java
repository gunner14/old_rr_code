/*
 * Function: Unit test of BlockItemsOfFriendListDeleted class 
 * Devleoper: XiongJunWu
 * EMail: junwu.xiong@renren-inc.com
 * Date: 2012-11-28
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

public class BlockItemsOfFriendListDeletedTest {

	public static BlockItemsOfFriendListDeletedTest instance = null;
	public final static byte[] kSgFriendListDeletedHTableName = Bytes
			.toBytes(FoFUpdator.kSgFriendListDeletedHTableName);
	public final static byte[] kSgFriendListDeletedHTableColumnFamily = Bytes
			.toBytes("f");
	private static Configuration conf;
	private static HTable friendListHTable;

	public static BlockItemsOfFriendListDeletedTest getInstance() {
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
						instance = new BlockItemsOfFriendListDeletedTest();
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
			Clogging.Info("values size " + values.size());
			for (KeyValue kv : values.raw()) {
				// if the friend number less than the maximum friend number
				friendListDeleted.add(Integer.valueOf(new String(kv
						.getQualifier())));
			}
			for (int i = 0; i < values.size(); ++i) {
				Clogging.Info("hostId: " + hostId + " deleted friendID "
						+ friendListDeleted.get(i));
			}
			Clogging.Info("friendListDeleted size " + friendListDeleted.size());
		} catch (IOException e) {
			values = null;
			e.printStackTrace();
		}
		return friendListDeleted;
	}

}
