/*
 * Function: Unit test of BlockItemsOfFriendList class 
 * Devleoper: XiongJunWu
 * EMail: junwu.xiong@renren-inc.com
 * Date: 2012-11-12
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

public class BlockItemsOfFriendListTest {
	public static BlockItemsOfFriendListTest instance = null;
	public final static byte[] kSgFriendListHTableName = Bytes
			.toBytes(FoFUpdator.kSgFriendListHTableName);
	public final static byte[] kSgFriendListHTableColumnFamily = Bytes
			.toBytes("f");
	private static Configuration conf;
	private static HTable friendListHTable;

	public static BlockItemsOfFriendListTest getInstance() {
		if (null == instance) {
			synchronized (BlockItemsOfFriendListTest.class) {
				if (null == instance) {
					try {
						conf = new Configuration();
						HBaseConfiguration.addHbaseResources(conf);
						try {
							friendListHTable = new HTable(conf,
									kSgFriendListHTableName);
						} catch (Exception e) {
							// TODO: handle exception
							e.printStackTrace();
						}
						instance = new BlockItemsOfFriendListTest();
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

	public List<Integer> getFriendList(int hostId, int limit) {
		List<Integer> friendList = new ArrayList<Integer>();
		Result values = null;
		try {
			Get get = new Get(Bytes.toBytes(String.valueOf(hostId)));
			get.addFamily(kSgFriendListHTableColumnFamily);
			values = friendListHTable.get(get);
			int pos = 0;
			Clogging.Info("values size " + values.size());
			for (KeyValue kv : values.raw()) {
				// if the friend number less than the maximum friend number
				if (limit <= 0 || (limit > 0 && (pos / 2) < limit)
						&& (pos / 2) < FoFUpdator.kMaxFriendNumber) {
					friendList.add(Bytes.toInt(kv.getQualifier()));
					pos = pos + 2;
				} else {
					break;
				}
			}
			for (int i = 0; i < values.size(); ++i) {
				Clogging.Info("hostId: " + hostId + " friendID "
						+ friendList.get(i));
			}
			Clogging.Info("friendList size " + friendList.size());
		} catch (IOException e) {
			values = null;
			e.printStackTrace();
		}
		return friendList;
	}

}
