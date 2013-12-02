/*
 * Function: SocialGraph FoF friends recommendation service inter group incremental
 * strategy implementation, which loads data in a certain contiguous address space in 
 * different group with rotated style and limitation of max number of kMaxFoFLoadNumber
 * from hbase each time for each user   
 * Developer: xiongjunwu
 * Email: Junwu.xiong@renren-inc.com
 * Date: 2012-09-01
 */

package com.renren.xce.socialgraph.data;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.NavigableMap;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.client.Get;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.client.Result;
import org.apache.hadoop.hbase.util.Bytes;
import org.apache.hadoop.hbase.util.MD5Hash;

import xce.socialgraph.util.Clogging;

import com.renren.xce.socialgraph.data.FoFRow.FoFGroup;
import com.renren.xce.socialgraph.data.RcdFoFData;
import com.renren.xce.socialgraph.updator.FoFUpdator;


public class RcdFoFUtilRotateInterGroup {
	private static RcdFoFUtilRotateInterGroup instance = null;  
	private static Configuration conf;
	private static HTable fofTable;
	public static int fofCounter;
    
	private RcdFoFUtilRotateInterGroup() {	
	}
	
	public static RcdFoFUtilRotateInterGroup getInstance() {
		if (null == instance) {
			synchronized (RcdFoFUtilRotateInterGroup.class) {
				if (null == instance) { 
					try {
						conf = new Configuration();
						HBaseConfiguration.addHbaseResources(conf);
						try {
							fofTable  = new HTable(conf, FoFHTable.kFoFHTable);
  					    } catch (IOException e) {							
							e.printStackTrace();
						}						
						instance = new RcdFoFUtilRotateInterGroup();
					} catch (Exception e) {
						e.printStackTrace();
						fofTable = null;
						instance = null;
					}				
				}
			}
		}
		return instance;
	}	
	
	public static List<RcdFoFData> getInnerGroupRecords(int hostId, int limit, byte[] value, int leftPos, int rightPos) {
		List<RcdFoFData> fofList = new ArrayList<RcdFoFData>();		
		if ((hostId < 0) || (value.length == 0) || (leftPos < 0) || (leftPos < 0) || (leftPos > rightPos)) {
			Clogging.Error("getInnerGroupRecords: Wrong input parameters!");
			return fofList;
		}
        for (int j = leftPos; j < rightPos; ++j) {
    	   RcdFoFData ffrd = new RcdFoFData();
    	   ffrd.setHostId(hostId);
           int basePos = (j - leftPos) * FoFRow.kRecordSize;
           int fofId= Bytes.toInt(value, basePos, Bytes.SIZEOF_INT);
           ffrd.setFoFId(fofId);
           float rank = Bytes.toFloat(value, basePos + Bytes.SIZEOF_FLOAT);
           ffrd.setRank(rank);
           int mutualFriendNumber = Bytes.toInt(value, basePos + Bytes.SIZEOF_INT + Bytes.SIZEOF_FLOAT, Bytes.SIZEOF_INT);  
           ffrd.setMutualFriendNumber(mutualFriendNumber);
           int idBasePos = basePos + Bytes.SIZEOF_INT + Bytes.SIZEOF_FLOAT + Bytes.SIZEOF_INT;
           if (mutualFriendNumber > 0) {
        	   int actualDispNumber = mutualFriendNumber > FoFUpdator.kDisplayMutualFriendNumber ? FoFUpdator.kDisplayMutualFriendNumber : mutualFriendNumber;
	           for (int k = 0; k < actualDispNumber; ++k) {
	        	   int mutualFriendId = Bytes.toInt(value, idBasePos + k * Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
				   if (mutualFriendId > 0) {     
					   ffrd.getmutualFriendIdList().add(mutualFriendId);
					}
	            }
           }           
           fofList.add(ffrd);		
           fofCounter++;	
           if (limit > 0 && fofCounter >= limit) {
				return fofList;
		   }							
		   if ((limit < 0 && fofCounter >= FoFUpdator.kFoFMaxLoadNumber) ) {
			    if (limit > 0 && limit >= FoFUpdator.kFoFMaxLoadNumber) {
					return fofList;
				}
		   }		
        }     
		return fofList;
	}

	public List<RcdFoFData> getRcdFoFData(int hostId, int limit) {
		List<RcdFoFData> fofList = new ArrayList<RcdFoFData>();		
		if (hostId < 0) {
			return fofList;
		}

		String keyString = String.valueOf(hostId);
		String hash = MD5Hash.getMD5AsHex(keyString.getBytes());
		String rehash = MD5Hash.getMD5AsHex((hash + keyString).getBytes());
		String key = rehash.substring(0,10).toUpperCase() + keyString;
		List<Get> getList = new ArrayList<Get>();
		Result[] resultArray = null;	
		List<FoFGroup> fofGroupList = FoFRow.getInstance().getFoFGroupList();
		if (!fofGroupList.isEmpty()) {
			for (FoFGroup groupItem : fofGroupList) {
				System.out.println("fofGroupId " + groupItem.groupId);
				byte[] qualifier= Bytes.toBytes(groupItem.groupId * FoFRow.kFoFGroupSize);				
				Get get = new Get(key.getBytes());
				get.setCacheBlocks(false);
				get.addColumn(FoFHTable.kColumnFamily, qualifier);
				getList.add(get);				
			}	
		} else {
			Clogging.Error("getRcdFoFData: fofGroupIdList is empty!");
			return fofList;
		}
		try {		
			resultArray = fofTable.get(getList);  
			if ((resultArray != null) && (resultArray.length > 0)) {
				fofCounter = 0;
				for (Result result : resultArray) {
					Clogging.Info("result array length: " + resultArray.length);
					List<RcdFoFData> subFoFList = new ArrayList<RcdFoFData>();	
					NavigableMap<byte[], byte[]> fofQualifierValueMap = result.getFamilyMap(FoFHTable.kColumnFamily);
					Clogging.Info("fofQuailifierValueMap size: " + fofQualifierValueMap.size());					
					for (FoFGroup groupItem : fofGroupList) {
						Clogging.Info("groupId: " + groupItem.groupId);
						byte[] qualifier= Bytes.toBytes(groupItem.groupId * FoFRow.kFoFGroupSize);	
						if (fofQualifierValueMap.containsKey(qualifier)) {
							byte[] value = fofQualifierValueMap.get(qualifier);
							int leftPosition = groupItem.groupId * FoFRow.kFoFGroupSize + groupItem.innerGroupStartIndex;
							int rightPosition = groupItem.groupId * FoFRow.kFoFGroupSize + groupItem.innerGroupEndIndex;		
							if (value.length > 0) {
								subFoFList = getInnerGroupRecords(hostId, limit, value, leftPosition, rightPosition);
							}
							if (!subFoFList.isEmpty()) {
								fofList.addAll(subFoFList);
							}
						}
					}				
				}	//end for Result
			} else { //end if resultArray
				Clogging.Error("getRcdFoFData: Fecthing result fof data from hbase failed, result array length: " + resultArray.length);
				return fofList;
			}			
		} catch (IOException e) {
			e.printStackTrace();
			return fofList;
		}	
		Clogging.Info("getRcdFoFData fofList size: " +  fofList.size());
		return fofList;
	}		
}	