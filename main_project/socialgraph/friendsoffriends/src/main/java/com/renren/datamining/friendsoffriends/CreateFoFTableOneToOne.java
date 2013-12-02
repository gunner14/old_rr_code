/**************************************************************************************************
 * Developer:  Xiongjunwu
 * Email:  junwu.xiong@renren-inc.com
 * Function: Write model final ranking values to fof table in HBase
 * Date:  2012-04-19
*********************************************************************************************************************
*  HBase table format
*  rowkey             columnfamily:quailfier:value
*  hostIdfofId		   f|rank1|rank2(-1)|mutualfriendnumber|mutualfriendIdList 
*********************************************************************************************************************/

package com.renren.datamining.friendsoffriends;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.client.Put;
import org.apache.hadoop.hbase.util.Bytes;


public class CreateFoFTableOneToOne extends CreateFoFTable{	
	private byte[] columnFamily = Bytes.toBytes("f");
	private int kMaxFoFNumber = 200;
	private HTable desTable = null;
	
    public CreateFoFTableOneToOne(byte[] cf, int maxFoF, HTable table){
      columnFamily = cf;
      kMaxFoFNumber = maxFoF;
      desTable = table;
    }
	
	public void AddReocrd(int hostId, List<FoFRank> fofRankList) throws IOException {	
		 final byte[] bits = GetHashedKey(String.valueOf(hostId)).getBytes();
		 int counter = 0;		
		 //int mutualFriendsNumber = 0;		
		 Iterator<FoFRank> it = fofRankList.iterator();
		 FoFRank ffr = null;
		 List<Put> puts = new ArrayList<Put>();		
		 while ((it.hasNext()) && (counter++ < kMaxFoFNumber)) {
			 ffr = it.next();
			 Put put = new Put(Bytes.add(bits, Bytes.toBytes(ffr.fofId)));
		     //mutualFriendsNumber = ffr.mutualFriendIdList.size();
		     if (ffr.mutualFriendIdListSize > 0) {
			     byte[] mutualFriendIds = new byte[ffr.mutualFriendIdListSize*Bytes.SIZEOF_INT];
			     for (int i = 0; i < ffr.mutualFriendIdListSize; ++i) {
			    	 System.arraycopy(Bytes.toBytes(ffr.mutualFriendIdList[i]), 0, mutualFriendIds, i*Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
			     }
			     put.add(columnFamily, Bytes.toBytes(ffr.rank), Bytes.add(Bytes.toBytes(-1), Bytes.toBytes(ffr.mutualFriendNumber), mutualFriendIds));
			     put.setWriteToWAL(false);
				 puts.add(put);
		     }
		 }
		 desTable.put(puts);
	}
}





































//public class CreateFoFTable {
//	
//	/**********************************************************************************************
//	 * fof table record format:
//	 * rowkey              w0(columnfamily, original weight) mf(columnfamily,mutualFriends)                                      w1(columnfamily, updated weight)  
//	 * userid-fofId(key)   w0(qualifier):weight              mutualfriends number(qualifier):mutualFriendId1...:mutualFriendIdN) w1(qualifier):weight     
//	 ***********************************************************************************************/
//	private final static String columnFamily0 = "w0";
//	private final static String qualifier0 = "u";
////	private final static String columnFamily2 = "w1";
////	private final static String qualifier2 = "w1";
//	private final static String columnFamily1 = "f";
//	
////	public class IntArrayWritable extends ArrayWritable {
////		public IntArrayWritable() {
////			super(IntWritable.class);
////		}
////	}
//	
//	static public void AddReocrd(int hostId, List<FoFRank> fofRankList) throws IOException {		 
//		 List<Put> puts = new ArrayList<Put>();
//		 Iterator<FoFRank> it = fofRankList.iterator();
//		 while (it.hasNext()) {
//			 FoFRank ffr = it.next();
//			 String key = String.valueOf(hostId) + "-" + ffr.fofId;
////			 System.out.println("key " + key);
////			 Put put = new Put(Bytes.toBytes(key));
////			 Put put = new Put(Bytes.add(Bytes.toBytes(hostId), Bytes.toBytes("-"), Bytes.toBytes(ffr.fofId)));
//			 Put put = new Put(Bytes.add(Bytes.toBytes(hostId), Bytes.toBytes(ffr.fofId)));
//		     put.add(Bytes.toBytes("u"), Bytes.toBytes(qualifier0), Bytes.toBytes(ffr.rank));  
////		     System.out.println("rank " + ffr.rank);
////		     IntArrayWritable mutualFriendsIds = new IntArrayWritable();		   
//		     put.add(Bytes.toBytes("f"), Bytes.toBytes(ffr.mutualFriendNumber), 
//		    		 Bytes.toBytes(ffr.mutualFriendIdList.toString())); 
////		     System.out.println("key " + key + " mutual number " + ffr.mutualFriendNumber);
////		     Iterator<Integer> mf = ffr.mutualFriendIdList.iterator();
////		     while (mf.hasNext()) {
////		    	 System.out.println(" mf " + mf.next());
////		     }
//			 puts.add(put);
//		 }
//		 RecommendFriendsOfFriends_FromSequenceFileToHBase.ReadExtendRelationMapper.desTable.put(puts);
//	}
//}

//static public final byte[] myReverse(byte[] bits) {
//	int n = Bytes.SIZEOF_INT - 1;
//	System.out.println("bits length" + bits.length);
//	for (int j = (n-1) >> 1; j >= 0; --j) {
//	    byte temp = bits[j];
//	    byte temp2 = bits[n - j];
//	    bits[j] = temp2;
//	    bits[n - j] = temp;
//	}
//	return bits;
//}	

//if (!hasSurrogate) {
//hasSurrogate = (temp >= Character.MIN_SURROGATE && temp <= Character.MAX_SURROGATE)
//    || (temp2 >= Character.MIN_SURROGATE && temp2 <= Character.MAX_SURROGATE);
//}

//Iterator<Integer> mf = ffr.mutualFriendIdList.iterator();
//while (mf.hasNext()) {
//	 System.out.println(" mf " + mf.next());
//}

//private final static byte[] columnFamily0 = Bytes.toBytes("u");
//private final static byte[] qualifier0 = Bytes.toBytes("u");
//private final static String columnFamily2 = "w1";
//private final static String qualifier2 = "w1";
//private final static byte[] columnFamily1 = Bytes.toBytes("u");
//private final static byte[] qualifier1 = Bytes.toBytes("u");
//public class IntArrayWritable extends ArrayWritable {
//	public IntArrayWritable() {
//		super(IntWritable.class);
//	}
//}
//
//static public String myReserve(int hostId){
//
//	int tempNum = hostId, count, result=0;
//	StringBuffer tmp = new StringBuffer();
//
//	for(count=0; tempNum > 0; tempNum /= 10,count++);//算出该数有多少位  
//
//	while(hostId > 0){
//		int num = hostId %10;//取出该位上的数.  
//		for(int i =1 ; i<count; i++){//count标识当前正在处理第几位数.  
//			num *= 10;
//		}
//		count--;//标识向前移一位.  
//		result += num;
//		tmp.append(num);
//		hostId /= 10;//切掉处理过的位数.  
//	};
////	return result;
//	return tmp.toString();
//}

//put.add(columnFamily2, Bytes.toBytes(ffr.mutualFriendNumber), mutualFriendIds); 


//String key = String.valueOf(hostId) + "-" + ffr.fofId;
//System.arraycopy(bytes, j * Bytes.SIZEOF_INT, friendId, 0,
//			Bytes.SIZEOF_INT);
//Put put = new Put(Bytes.add(Bytes.toBytes(Integer.MAX_VALUE - hostId), Bytes.toBytes(ffr.fofId)));
//Put put = new Put(Bytes.add(reverseHostId, Bytes.toBytes(ffr.fofId)));
//
//System.out.println("hostId " +  hostId + " hostId bits" + Bytes.toBytes(hostId));

//System.out.println("key size " + Bytes.add(Bytes.toBytes(hostId), Bytes.toBytes(ffr.fofId)).length);
//System.out.println("reversed hostId bits " + sb.reverse().toString());
//System.out.println(" reversed hostId bits " + myReverse(Bytes.toBytes(hostId)));
//System.out.println("original hostId " + Bytes.toInt( myReverse( myReverse(Bytes.toBytes(hostId)))) );
//Put put = new Put(Bytes.toBytes(key));
//put.add(columnFamily, qualifier1, Bytes.toBytes(ffr.rank));   
//System.out.println("rank " + ffr.rank);
//IntArrayWritable mutualFriendsIds = new IntArrayWritable();	

//put.add(columnFamily1, Bytes.toBytes(ffr.mutualFriendNumber), 
//Bytes.toBytes(ffr.mutualFriendIdList.toString())); 

//final byte[] reverseHostId = myReverse(Bytes.toBytes(hostId));
//System.arraycopy(bits, 0, bits, 2, bits.length-2);
//System.out.println("before " + bits);
//System.arraycopy(bits, 1, bits, 0, bits.length -1);
//System.out.println("after " + bits );
//StringBuffer sb = new StringBuffer(String.valueOf(hostId));
//final String reverseId = sb.reverse().toString();
//System.out.println("hostId " +  hostId +  "reversed hostId bits " + reverseId);
//System.out.println("hostId " +  hostId + " reversedId " + Bytes.toInt(reverseHostId) );


//if (puts.isEmpty()) {
//System.out.println("null ");
//} else {
//System.out.println("not null");
//}
//RecommendFriendsOfFriends_FromSequenceFileToHBase.ReadExtendRelationMapper.desTable[hostId % 10].put(puts);