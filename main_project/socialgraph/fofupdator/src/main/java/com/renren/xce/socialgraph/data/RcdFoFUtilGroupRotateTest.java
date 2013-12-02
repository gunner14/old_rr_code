/*
 * Function: Unit test case of RcdFoFUtilGroupRotate class   
 * Developer: xiongjunwu
 * Email: Junwu.xiong@renren-inc.com
 * Date: 2012-09-01
 */

package com.renren.xce.socialgraph.data;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
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


public class RcdFoFUtilGroupRotateTest {
	private static RcdFoFUtilGroupRotateTest instance = null;  
	private static Configuration conf;
	private static HTable fofTable;
	private static HTable fofExpHTable;
	private RcdFoFUtilGroupRotateTest() {	
	}
	
	public static RcdFoFUtilGroupRotateTest getInstance() {
		if (null == instance) {
			synchronized (RcdFoFUtilGroupRotateTest.class) {
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
						instance = new RcdFoFUtilGroupRotateTest();
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
            	   Clogging.Info("getInnerGroupRecords fofList size: " +  fofList.size());
    				return fofList;
    		   }							
    		   if ((limit < 0 && fofCounter >= FoFUpdator.kFoFMaxLoadNumber) ) {
    			   Clogging.Info("getInnerGroupRecords fofList size: " +  fofList.size());
    			    if (limit > 0 && limit >= FoFUpdator.kFoFMaxLoadNumber) {
    					return fofList;
    				}
    		   }		
           } else {
        	   break;
           }
        }     
		Clogging.Info("getInnerGroupRecords fofList size: " +  fofList.size());
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
				Clogging.Info("getRcdFoFData: group Id " + groupItem.groupId);
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
				Clogging.Info("getRcdFoFData: hostId " + hostId + " kFoFHTableName "+ FoFUpdator.kFoFExpHTableName);

			} else {
				Clogging.Info("getRcdFoFData: hostId " + hostId + " kFoFHTableName "+ FoFUpdator.kFoFHTableName);
				resultArray = fofTable.get(getList);
			}
			if ((resultArray != null) && (resultArray.length > 0)) {
				for (Result result : resultArray) {
					if (result.size() > 0) {
						NavigableMap<byte[], byte[]> fofQualifierValueMap = result.getFamilyMap(FoFHTable.kColumnFamily);
						Clogging.Info("getRcdFoFData: fofQuailifierValueMap size: " + fofQualifierValueMap.size());					
						for (FoFGroup groupItem : fofGroupList) {
							Clogging.Info("groupId: " + groupItem.groupId);
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
									Clogging.Info("getRcdFoFData: hostId " + hostId + " subFoFList fofList size: " +  fofList.size());
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
	
	public static List<Integer> getIdListFromFile(String filePath) {
		List<Integer> idList = new ArrayList<Integer>();		
		File file = new File(filePath);
		if (!file.exists()) {
			Clogging.Error(filePath + " doesn't exist!");
			return idList;
		}
		try {
			BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(file)), 1024*1024*4);			
			String line = reader.readLine();
			while (null != line) {
				int id = Integer.valueOf(line);
				idList.add(id);			
				line = reader.readLine();
			}
			reader.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} 
		return idList;
	}	
	
	public static void main(String[] args) {
		if (2 != args.length) {
			Clogging.Error("sgutil error args!");
			System.exit(0);
		}
		final int kMaxFectchNumber = 40;
		String kHostIdsFile = args[0];  
		List<Integer> idList = RcdFoFUtilGroupRotateTest.getIdListFromFile(kHostIdsFile);   
		if (!idList.isEmpty()) {
			System.out.println("*******************************************************data load over**************************************************");
			System.out.println("load id number " + idList.size());		
			Clogging.Debug("idList size:" + idList.size());
			Long startTime = System.currentTimeMillis();
			Long endTime;
			int getCounter = 0;
			final int kGetCounterUnit = 100;
			if (!idList.isEmpty()) {
				for (int id : idList) {
					startTime = System.currentTimeMillis();
					List<RcdFoFData>  fofList = RcdFoFUtilGroupRotateTest.getInstance().getRcdFoFData(id, kMaxFectchNumber);
					getCounter++;
					endTime = System.currentTimeMillis();
					Clogging.Info("getCounter " + getCounter + " toal time cost " + (endTime - startTime) + 
							" time period " + (endTime - startTime) + " hostId " + id + " size " + fofList.size());
					if (getCounter % kGetCounterUnit == 0) {
						Clogging.Info("getCounter " + getCounter + " toal time cost " + (endTime - startTime) + 
								" time period " + (endTime - startTime) + " hostId " + id + " size " + fofList.size());
					}
				}
			}
		}
		System.exit(0);
	}	
}	