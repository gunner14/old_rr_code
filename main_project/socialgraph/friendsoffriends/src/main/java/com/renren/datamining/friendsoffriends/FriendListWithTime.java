package com.renren.datamining.friendsoffriends;

import java.io.DataInput;
import java.io.DataInputStream;
import java.io.DataOutput;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.hadoop.io.Writable;

public class FriendListWithTime implements Writable {

  public final Map<Integer, Integer> data = new HashMap<Integer, Integer>();

  public static FriendListWithTime parseFromHBase(DataInputStream in)
      throws IOException {
    FriendListWithTime result = new FriendListWithTime();
    int size = in.readInt();
    int[] ids = new int[size];
    for (int i = 0; i < size; ++i) {
      ids[i] = in.readInt();
    }
    for (int i = 0; i < size; ++i) {
      result.data.put(ids[i], in.readInt());
    }
    return result;
  }

  public void readFields(DataInput in) throws IOException {
    int size = in.readInt();
    for (int i = 0; i < size; ++i) {
//      Integer time = in.readInt();
      this.data.put(in.readInt(), in.readInt());
//      System.out.println("read time " + time);
    }
  }

  public void write(DataOutput out) throws IOException {
    out.writeInt(this.data.size());
    for(Entry<Integer, Integer> entry: this.data.entrySet()){
      out.writeInt(entry.getKey());
      out.writeInt(entry.getValue());  
//      Integer time = entry.getValue();
//      System.out.println("write time " + time);
//      out.writeInt(time);    	
    }
  }
}
