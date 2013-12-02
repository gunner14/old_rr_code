/*
 * Function: SocialGraph FoF friends recommendation service inter group 
 * strategy implementation, which loads data in a certain contiguous address space in 
 * different group(with incremental group id) with rotated style and limitation of 
 * max number of kMaxFoFLoadNumber from htable each time for each user   
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


public class RcdFoFUtilGroupRotate {
	private static RcdFoFUtilGroupRotate instance = null;  
	private static Configuration conf;
	private static HTable fofTable;
	private static HTable fofExpHTable;
	private RcdFoFUtilGroupRotate() {	
	}
	
	public static RcdFoFUtilGroupRotate getInstance() {
		if (null == instance) {
			synchronized (RcdFoFUtilGroupRotate.class) {
				if (null == instance) { 
					try {
						conf = new Configuration();
						HBaseConfiguration.addHbaseResources(conf);
						try {
							fofTable  = new HTable(conf, FoFHTable.kFoFHTable);
							fofExpHTable = new HTable(conf, FoFHTable.kFoFExpHTable);
  					    } catch (IOException e) {							
							e.printStackTrace();
						}						
						instance = new RcdFoFUtilGroupRotate();
					} catch (Exception e) {
						e.printStackTrace();
						fofTable = null;
						fofExpHTable = null;
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
			Clogging.Error("getInnerGroupRecords: Wrong group left and right position parameters!");
			return fofList;
		}
		
		int fofCounter = 0;
        for (int j = leftPos; j < rightPos; ++j) {
    	   RcdFoFData ffrd = new RcdFoFData();
    	   ffrd.setHostId(hostId);
           int basePos = (j - leftPos) * FoFRow.kRecordSize;
           if (basePos < value.length) { 
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
           } else {
        	   break;
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
			if (FoFUpdator.kFoFModelModifyStrategy) {
				resultArray = fofExpHTable.get(getList);
			} else {
				resultArray = fofTable.get(getList);
			}
			if ((resultArray != null) && (resultArray.length > 0)) {
				for (Result result : resultArray) {
					if (result.size() > 0) {
						NavigableMap<byte[], byte[]> fofQualifierValueMap = result.getFamilyMap(FoFHTable.kColumnFamily);
						for (FoFGroup groupItem : fofGroupList) {
							byte[] qualifier= Bytes.toBytes(groupItem.groupId * FoFRow.kFoFGroupSize);	
							if (fofQualifierValueMap.size() > 0) {
								if (fofQualifierValueMap.containsKey(qualifier)) {
									byte[] value = fofQualifierValueMap.get(qualifier);
									int leftPosition = groupItem.groupId * FoFRow.kFoFGroupSize + groupItem.innerGroupStartIndex;
									int rightPosition = groupItem.groupId * FoFRow.kFoFGroupSize + groupItem.innerGroupEndIndex;
									List<RcdFoFData> subFoFList = new ArrayList<RcdFoFData>();	
									if (value.length > 0) {									
										subFoFList = getInnerGroupRecords(hostId, limit, value, leftPosition, rightPosition);
									}
									if (!subFoFList.isEmpty()) {
										fofList.addAll(subFoFList);
									}
								}
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
		return fofList;
	}		
	
}	