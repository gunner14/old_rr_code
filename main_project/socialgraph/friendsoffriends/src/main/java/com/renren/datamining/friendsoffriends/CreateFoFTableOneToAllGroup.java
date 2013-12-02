package com.renren.datamining.friendsoffriends;

import java.io.IOException;
import java.util.List;

import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.client.Put;
import org.apache.hadoop.hbase.util.Bytes;

public class CreateFoFTableOneToAllGroup extends CreateFoFTable {
  
  private byte[] columnFamily = null;
  private int maxFoFNumber = 300;
  private HTable desTable = null;
  private int grpSize = -1;
  
  public CreateFoFTableOneToAllGroup(byte[] cf, int maxFoF, int groupSize, HTable table){
    columnFamily = cf;
    maxFoFNumber = maxFoF;
    grpSize = groupSize;
    desTable = table;
  }

  public void AddReocrd(int hostId, List<FoFRank> fofRankList) throws IOException {
	final byte[] hostBits = GetHashedKey(String.valueOf(hostId)).getBytes();
    Put put = new Put(hostBits);
    // Record format: fofId, rank, mutFriendCount, mutFriend ids.    
    int recordSize = Bytes.SIZEOF_INT + Bytes.SIZEOF_FLOAT + Bytes.SIZEOF_INT 
                     + FoFRankModel.kDispMutFrdNum * Bytes.SIZEOF_INT;
    int grpCount = (Math.min(fofRankList.size(), maxFoFNumber) + grpSize - 1) / grpSize;
    
    for (int i = 0; i < grpCount; ++i) {
      int leftPos = i * grpSize;
      int rightPos = Math.min((i + 1) * grpSize, fofRankList.size());
      byte[] cell = new byte[(rightPos - leftPos) * recordSize];
      
      for (int j = leftPos; j < rightPos; ++j) {
        FoFRank r = fofRankList.get(j);
        int basePos = (j - leftPos) * recordSize;
        System.arraycopy(Bytes.toBytes(r.fofId), 0, cell, basePos, Bytes.SIZEOF_INT);
        System.arraycopy(Bytes.toBytes(r.rank), 0, cell, basePos + Bytes.SIZEOF_INT, Bytes.SIZEOF_FLOAT);
        System.arraycopy(Bytes.toBytes(r.mutualFriendNumber), 0, cell, basePos + Bytes.SIZEOF_INT + Bytes.SIZEOF_FLOAT, Bytes.SIZEOF_INT);
        
        int idBasePos = basePos + Bytes.SIZEOF_INT + Bytes.SIZEOF_FLOAT + Bytes.SIZEOF_INT;
        for (int k = 0; k < r.mutualFriendIdList.length; ++k)
          System.arraycopy(Bytes.toBytes(r.mutualFriendIdList[k]), 0, cell, idBasePos + k * Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
      }
      
      put.add(columnFamily, Bytes.toBytes(leftPos), cell);
    }

    put.setWriteToWAL(false);
    desTable.put(put);
  }
}
