/**************************************************************************************************
 * Developer:  Xiongjunwu
 * Email:  junwu.xiong@renren-inc.com
 * Function: Write model final ranking values to fof table in HBase
 * Date:  2012-04-19
*********************************************************************************************************************
*  HBase table format
*  rowkey             columnfamily:quailfier:value
*  hostId             f | fofId	| fof value structure(unit: rank1|rank2(-1)|mutualfriendnumber|mutualFriendIds)  	   
*********************************************************************************************************************/

package com.renren.datamining.friendsoffriends;

import java.io.IOException;
import java.util.Iterator;
import java.util.List;

import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.client.Put;
import org.apache.hadoop.hbase.util.Bytes;

public class CreateFoFTableOneToAll extends CreateFoFTable {

	private byte[] columnFamily = null;
    private int maxFoFNumber = 1000; 
    private HTable desTable = null;
    
    public CreateFoFTableOneToAll(byte[] cf, int maxFoF, HTable table){
      columnFamily = cf;
      maxFoFNumber = maxFoF;
      desTable = table;
    }
    
	public void AddReocrd(int hostId, List<FoFRank> fofRankList) throws IOException {	
		final byte[] bits = GetHashedKey(String.valueOf(hostId)).getBytes();
		Put put = new Put(bits);
		
		Iterator<FoFRank> it = fofRankList.iterator();
		int counter = 0;
		while ((it.hasNext()) && (counter++ < maxFoFNumber)) {
		    FoFRank ffr = it.next();
		    
		    // Pre-allocate the array to avoid copies.
		    // Format: rank, mutalFriendNum, Displayed mutalFriend ids.
		    byte[] mutualFriendIds = new byte[Bytes.SIZEOF_FLOAT + Bytes.SIZEOF_INT + ffr.mutualFriendIdListSize * Bytes.SIZEOF_INT];
		    System.arraycopy(Bytes.toBytes(ffr.rank), 0, mutualFriendIds, 0, Bytes.SIZEOF_FLOAT);
		    System.arraycopy(Bytes.toBytes(ffr.mutualFriendNumber), 0, mutualFriendIds, Bytes.SIZEOF_FLOAT, Bytes.SIZEOF_INT);
		    for (int i = 0; i < ffr.mutualFriendIdListSize; ++i) 
		    	System.arraycopy(Bytes.toBytes(ffr.mutualFriendIdList[i]), 0, mutualFriendIds, (i + 2) * Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
		    
		    put.add(columnFamily, Bytes.toBytes(ffr.fofId), mutualFriendIds); 
		}
	    put.setWriteToWAL(false);
		desTable.put(put);
	}
}																						


















































//List<Put> puts = new ArrayList<Put>();
//StringBuffer tmp = new StringBuffer(hostId);
//System.out.println("hostId " + hostId);
//System.out.println("reversed hostId " + tmp.reverse().toString());
//System.out.println("reversed hostId " + myReserve(hostId));
//Put put = new Put(Bytes.toBytes(tmp.reverse().toString()));
//Put put = new Put(Bytes.toBytes(myReserve(hostId)));
//Put put = new Put(Bytes.toBytes(Integer.MAX_VALUE-hostId));
//final byte[] bits = Bytes.toBytes(hostId);
//System.arraycopy(bits, 0, bits, 1, bits.length-1);



//static public int myReserve(int hostId){  
//
//int tempNum = hostId, count, result=0;  
//
//for(count=0; tempNum > 0; tempNum /= 10,count++);//算出该数有多少位  
//
//while(hostId > 0){  
//    int num = hostId %10;//取出该位上的数.  
//    for(int i =1 ; i<count; i++){//count标识当前正在处理第几位数.  
//         num *= 10;  
//    }  
//    count--;//标识向前移一位.  
//    result += num;  
//    hostId /= 10;//切掉处理过的位数.  
//};  
//return result;  
//}  
//Put put = new Put(Bytes.toBytes(kMaxUserNumber - hostId));


//put.add(columnFamily1, Bytes.toBytes(ffr.mutualFriendNumber), 
//Bytes.toBytes(ffr.mutualFriendIdList.toString())); 
//System.out.println("key " + key + " mutual number " + ffr.mutualFriendNumber);
//Iterator<Integer> mf = ffr.mutualFriendIdList.iterator();
//while (mf.hasNext()) {
//System.out.println(" mf " + mf.next());
//}
//puts.add(put);
//public static final int kMaxUserNumber = 2*Integer.MAX_VALUE;	


//String key = String.valueOf(hostId) + "-" + ffr.fofId;
//System.arraycopy(bytes, j * Bytes.SIZEOF_INT, friendId, 0,
//			Bytes.SIZEOF_INT);
//Put put = new Put(Bytes.add(Bytes.toBytes(hostId), Bytes.toBytes(ffr.fofId)));
//put.add(columnFamily, Bytes.toBytes(ffr.fofId), Bytes.toBytes(ffr.rank));   
//System.out.println("rank " + ffr.rank);
//IntArrayWritable mutualFriendsIds = new IntArrayWritable();	