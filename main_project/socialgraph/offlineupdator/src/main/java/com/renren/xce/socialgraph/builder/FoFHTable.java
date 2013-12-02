/*
 * Function: SocialGraph FoF friends recommendation service HBase Table schema
 * relevant parameters, including HTable name, column family name etc.
 * Developer: xiongjunwu
 * Email: junwu.xiong@renren-inc.com
 * Date: 2012-09-04
 */

package com.renren.xce.socialgraph.builder;

import org.apache.hadoop.hbase.util.Bytes;

import com.renren.xce.socialgraph.updator.OfflineUpdator;

public class FoFHTable {
	
	public final static byte[] kFoFHTable = Bytes.toBytes(OfflineUpdator.kFoFHTableName);
	public final static byte[] kFoFExpHTable = Bytes.toBytes(OfflineUpdator.kFoFExpHTableName);
	public final static byte[] kColumnFamily = Bytes.toBytes("f");
}
