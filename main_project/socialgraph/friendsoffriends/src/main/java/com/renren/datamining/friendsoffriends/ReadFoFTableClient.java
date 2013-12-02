package com.renren.datamining.friendsoffriends;

import java.io.IOException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.KeyValue;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.client.Result;
import org.apache.hadoop.hbase.client.ResultScanner;
import org.apache.hadoop.hbase.client.Scan;
import org.apache.hadoop.hbase.util.Bytes;

public class ReadFoFTableClient {

	private String fofTableName;
	private Configuration conf;
	public ReadFoFTableClient (String fofTableName, Configuration conf) {
		this.fofTableName = fofTableName;
		this.conf = conf;
	}
	
	static final long CountUnit = 10;
	static long startTime = 0;
	static long endTime = 0;
	static long scanCounter = 0;
	public void ScanTable() throws IOException {
		HTable fofTable = new HTable(this.conf, Bytes.toBytes(this.fofTableName));
		Scan scan = new Scan();
		ResultScanner resultScanner = null;
		try {
			resultScanner = fofTable.getScanner(scan);
			startTime = System.currentTimeMillis();
			for (Result item : resultScanner) {
				final byte[] key = item.getRow();
				final int hostId = Bytes.toInt(key, 0, Bytes.SIZEOF_INT);
				final int fofId= Bytes.toInt(key, Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
//				System.out.print(" hostId " + hostId + " fofId " + fofId);
				int mutualFriendNumber = 0;				
				KeyValue[] kvs = item.raw();			
				float rank = Bytes.toFloat(kvs[0].getValue(), 0);
//				System.out.print(" rank " + rank);
				mutualFriendNumber = Bytes.toInt(kvs[0].getValue(), 2*Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
//				mutualFriendNumber = Bytes.toInt(kvs[0].getValue(), 4*Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
//				System.out.print(" mutual friend number " + mutualFriendNumber);
				int mutualFriendId = 0;
				if (mutualFriendNumber > 0  &&  (mutualFriendNumber + 3 ==  (kvs[0].getValueLength()) / 4 *Bytes.SIZEOF_BYTE)) {
					for (int i = 0; i < mutualFriendNumber; ++i) {
						mutualFriendId = Bytes.toInt(kvs[0].getValue(), (i+3)*Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
//							mutualFriendId = Bytes.toInt(kvs[0].getValue(), (i+5)*Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
//						System.out.print(" " + i + " "+ mutualFriendId);
					} 
				} else {
//					System.out.println("mutualFriendNumber " + mutualFriendNumber + " value length " + kvs[0].getValueLength());
				}
				
				if (++scanCounter % CountUnit == 0) {
					endTime = System.currentTimeMillis(); 
					System.out.println("scanCounter " + scanCounter + " time cost " + (endTime - startTime));
				}
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} finally {
			resultScanner.close();
		}	
	}	
	
	public static void main(String[] arguments) throws IOException {
		String fofTableName = "fof_201206262256";
		Configuration conf = HBaseConfiguration.create();
		ReadFoFTableClient fofTableClient = new ReadFoFTableClient(fofTableName, conf);		
		fofTableClient.ScanTable();		
	}	
}

