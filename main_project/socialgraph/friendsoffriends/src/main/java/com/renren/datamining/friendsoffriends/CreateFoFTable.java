package com.renren.datamining.friendsoffriends;

import java.io.IOException;
import java.util.List;

import org.apache.hadoop.hbase.util.MD5Hash;

public abstract class CreateFoFTable {
  public abstract void AddReocrd(int hostId, List<FoFRank> fofRankList) throws IOException;
  
  public static String GetHashedKey(String key) {
	  String hash = MD5Hash.getMD5AsHex(key.getBytes());
	  String rehash = MD5Hash.getMD5AsHex((hash + key).getBytes());
	  return rehash.substring(0, 10).toUpperCase() + key;
  }
  
  public String GetReversedKey(String key) {
	  StringBuffer sb = new StringBuffer(key);
	  return sb.reverse().toString();
  }
  
  public static void main(String[] args) {
	  String a = "256404211";
	  System.out.println(GetHashedKey(a));
  }
}
