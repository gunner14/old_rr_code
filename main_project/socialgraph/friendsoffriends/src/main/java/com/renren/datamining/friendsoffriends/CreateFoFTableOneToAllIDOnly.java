package com.renren.datamining.friendsoffriends;

import java.io.IOException;
import java.util.List;

import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.client.Put;
import org.apache.hadoop.hbase.util.Bytes;

public class CreateFoFTableOneToAllIDOnly extends CreateFoFTable {
  
  private byte[] columnFamily = null;
  private int maxFoFNumber = 10000;
  private HTable desTable = null;
  
  public CreateFoFTableOneToAllIDOnly(byte[] cf, int maxFoF, HTable table){
    columnFamily = cf;
    maxFoFNumber = maxFoF;
    desTable = table;
  }
  
  public void AddReocrd(int hostId, List<FoFRank> fofRankList) throws IOException {
	final byte[] hostBits = GetHashedKey(String.valueOf(hostId)).getBytes();
    Put put = new Put(hostBits);
    
    int num = Math.min(maxFoFNumber, fofRankList.size());
    byte[] cell = new byte[num * Bytes.SIZEOF_INT];
    for (int i = 0; i < num; ++i)
      System.arraycopy(Bytes.toBytes(fofRankList.get(i).fofId), 0, cell, i * Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
    put.add(columnFamily, Bytes.toBytes("f"), cell);
    put.setWriteToWAL(false);
    desTable.put(put);
  }
  

}
