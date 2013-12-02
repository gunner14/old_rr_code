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
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map.Entry;
import java.util.NavigableMap;
import java.util.TreeSet;

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


public class RcdFoFUtilGroupProbability {
	private static RcdFoFUtilGroupProbability instance = null;  
	private static Configuration conf;
	private static HTable fofTable;
	private static HTable fofExpHTable;
	private static List<FoFGroup> fofGroupList;
	private RcdFoFUtilGroupProbability() {	
	}
	
	public static RcdFoFUtilGroupProbability getInstance() {
		if (null == instance) {
			synchronized (RcdFoFUtilGroupProbability.class) {
				if (null == instance) { 
					try {
						conf = new Configuration();
						HBaseConfiguration.addHbaseResources(conf);
						try {
							fofTable  = new HTable(conf, FoFHTable.kFoFHTable);
							fofExpHTable = new HTable(conf, FoFHTable.kFoFExpHTable);
							fofGroupList = FoFRow.getInstance().getFoFGroupList();
  					    } catch (IOException e) {							
							e.printStackTrace();
						}						
						instance = new RcdFoFUtilGroupProbability();
					} catch (Exception e) {
						e.printStackTrace();
						fofTable = null;
						fofExpHTable = null;
						fofGroupList = null;
						instance = null;
					}				
				}
			}
		}
		return instance;
	}	
	
	/*
	 * Get recommended fof data position in different hbase group, which conforms to a certain
	 * distribution, such as uniform distribution, long tail distribution etc. 
	 */	
	public static void getFoFProbabilityPosition(List<FoFGroup> fofGroupList) {
		if (!fofGroupList.isEmpty()) {
			for (FoFGroup groupItem : fofGroupList) {
				int fofItemCounter = groupItem.innerGroupItemNumber;
				//minimum possible position of the recommended fof data in the group in hbase, inclusive minPosition
//				final int leftPos =  groupItem.groupId * FoFRow.kFoFGroupSize + groupItem.innerGroupStartIndex;
				final int leftPos =  groupItem.groupId * FoFRow.kFoFGroupSize;
				//maximum possible position of the recommended fof data in the group in hbase, exclusive maxPosition 
				final int rightPos = groupItem.groupId * FoFRow.kFoFGroupSize + groupItem.innerGroupEndIndex;
//				final int rightPos = groupItem.groupId * FoFRow.kFoFGroupSize + FoFRow.kFoFGroupSize;
				groupItem.fofPositionSet = new TreeSet<Integer>();
				for (; groupItem.fofPositionSet.size() < fofItemCounter; ) {
					int possiblePosition = leftPos + new java.util.Random().nextInt(rightPos - leftPos);	
					groupItem.fofPositionSet.add(possiblePosition);
				}	
			}			
		}		
	}
	
	public static List<RcdFoFData> getInnerGroupRecords(int hostId, int limit, byte[] value, FoFGroup groupItem) {
		List<RcdFoFData> fofList = new ArrayList<RcdFoFData>();
		int leftPos = groupItem.groupId * FoFRow.kFoFGroupSize + groupItem.innerGroupStartIndex;
		int rightPos = groupItem.groupId * FoFRow.kFoFGroupSize + groupItem.innerGroupEndIndex;
		TreeSet<Integer> fofPositionSet = groupItem.fofPositionSet;
		if ((hostId < 0) || (value.length == 0) || (leftPos < 0) || (leftPos < 0) || (leftPos > rightPos)) {
			Clogging.Error("getInnerGroupRecords: Wrong group left and right position parameters!");
			return fofList;
		}
		
		if (!fofPositionSet.isEmpty()) {
			int fofCounter = 0;
			for (Integer j : fofPositionSet) {
				if (j + 1 > rightPos) {
					continue;
				}
	        	int basePosition = (j % FoFRow.kFoFGroupSize) * FoFRow.kRecordSize;	 
	    	    RcdFoFData ffrd = new RcdFoFData();
	    	    ffrd.setHostId(hostId);
	            if (basePosition < value.length) { 
	        	    int fofId= Bytes.toInt(value, basePosition, Bytes.SIZEOF_INT);
	                ffrd.setFoFId(fofId);
	                float rank = Bytes.toFloat(value, basePosition + Bytes.SIZEOF_FLOAT);
	                ffrd.setRank(rank);
	                int mutualFriendNumber = Bytes.toInt(value, basePosition + Bytes.SIZEOF_INT + Bytes.SIZEOF_FLOAT, Bytes.SIZEOF_INT);  
	                ffrd.setMutualFriendNumber(mutualFriendNumber);
	                int idBasePos = basePosition + Bytes.SIZEOF_INT + Bytes.SIZEOF_FLOAT + Bytes.SIZEOF_INT;
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
		if (!fofGroupList.isEmpty()) {
			for (FoFGroup groupItem : fofGroupList) {
				byte[] qualifier = Bytes.toBytes(groupItem.groupId * FoFRow.kFoFGroupSize);				
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
				getFoFProbabilityPosition(fofGroupList);  //get fof probability-based positions 
				for (Result result : resultArray) {
					if (result.size() > 0) {
						NavigableMap<byte[], byte[]> fofQualifierValueMap = result.getFamilyMap(FoFHTable.kColumnFamily);
						for (FoFGroup groupItem : fofGroupList) {
							byte[] qualifier = Bytes.toBytes(groupItem.groupId * FoFRow.kFoFGroupSize);	
							if (fofQualifierValueMap.size() > 0) {
								if (fofQualifierValueMap.containsKey(qualifier)) {
									byte[] value = fofQualifierValueMap.get(qualifier);									
									List<RcdFoFData> subFoFList = new ArrayList<RcdFoFData>();	
									if (value.length > 0) {	
										subFoFList = getInnerGroupRecords(hostId, limit, value, groupItem);
										if (!subFoFList.isEmpty()) {
											fofList.addAll(subFoFList);
											Clogging.Info("getRcdFoFData: hostId " + hostId + " subFoFList fofList size: " +  subFoFList.size());
										}
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
	
	public static void main(String[] args) {		
		List<FoFGroup> fofGroupList = FoFRow.getInstance().getFoFGroupList();
		getFoFProbabilityPosition(fofGroupList); // get fof probability-based
													// positions
		HashMap<Integer, Integer> fofGroupNumberMapTest = new HashMap<Integer, Integer>();
		for (Entry<Integer, Integer> kv : fofGroupNumberMapTest.entrySet()) {
			System.out.println("fofId " + kv.getKey() + " group number "
					+ kv.getValue());
		}

		HashSet<Integer> fofGroupNumbers = new HashSet<Integer>(
				fofGroupNumberMapTest.values());
		for (Integer fofGroupNumber : fofGroupNumbers) {
			System.out.println("fofGroupNumber " + fofGroupNumber);
		}
	}
}	

	