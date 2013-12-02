/*
 * Function: SocialGraph FoF friends recommendation service HBase Table schema
 * relevant parameters, including HTable name, column family name etc.
 * Developer: xiongjunwu
 * Email: junwu.xiong@renren-inc.com
 * Date: 2012-09-04
 */

package com.renren.xce.socialgraph.data;

import org.apache.hadoop.hbase.util.Bytes;

import com.renren.xce.socialgraph.updator.FoFUpdator;

public class FoFHTable {

	public final static byte[] kFoFHTable = Bytes
			.toBytes(FoFUpdator.kFoFHTableName);
	public final static byte[] kFoFExpHTable = Bytes
			.toBytes(FoFUpdator.kFoFExpHTableName);
	public final static byte[] kColumnFamily = Bytes.toBytes("f");
}
