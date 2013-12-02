/*
 * Function: SocialGraph FoF friends recommendation service inter group incremental
 * strategy implementation, which loads data in a certain contiguous address space in 
 * different group with rotated style and limitation of max number of kMaxFoFLoadNumber
 * from hbase each time for each user   
 * Developer: xiongjunwu
 * Email: Junwu.xiong@renren-inc.com
 * Date: 2012-09-01
 */

package com.renren.xce.socialgraph.builder;
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

import com.renren.xce.socialgraph.builder.FoFRow.FoFGroup;
import com.renren.xce.socialgraph.updator.OfflineUpdator;


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
//				Clogging.Info("getFoFProbabilityPosition: " + groupItem.fofPositionSet.size());
			}			
		}		
	}
	
	public static List<RcdFoFRankData> getInnerGroupRecords(int hostId, int limit, byte[] value, FoFGroup groupItem) {
		List<RcdFoFRankData> fofList = new ArrayList<RcdFoFRankData>();
		int leftPos = groupItem.groupId * FoFRow.kFoFGroupSize + groupItem.innerGroupStartIndex;
		int rightPos = groupItem.groupId * FoFRow.kFoFGroupSize + groupItem.innerGroupEndIndex;
		TreeSet<Integer> fofPositionSet = groupItem.fofPositionSet;
		if ((hostId < 0) || (value.length == 0) || (leftPos < 0) || (leftPos < 0) || (leftPos > rightPos)) {
			Clogging.Error("getInnerGroupRecords: Wrong group left and right position parameters!");
			return fofList;
		}
		
		if (!fofPositionSet.isEmpty()) {
//			Clogging.Info("getInnerGroupRecords: " + " leftPos: " + leftPos + " rightPos " + 
//					rightPos + " fofPositionSet size " + fofPositionSet.size());
			int fofCounter = 0;
			for (Integer j : fofPositionSet) {
				if (j + 1 > rightPos) {
					continue;
				}
	        	int basePosition = (j % FoFRow.kFoFGroupSize) * FoFRow.kRecordSize;	 
	    	    RcdFoFRankData ffrd = new RcdFoFRankData();
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
	            	    int actualDispNumber = mutualFriendNumber > OfflineUpdator.kDispMutFrdNum ? OfflineUpdator.kDispMutFrdNum : mutualFriendNumber;
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
//	            	   Clogging.Info("getInnerGroupRecords fofList size: " +  fofList.size());
	    				return fofList;
	    		    }							
	    		    if ((limit < 0 && fofCounter >= OfflineUpdator.kFoFMaxLoadNumber) ) {
//	    			   Clogging.Info("getInnerGroupRecords fofList size: " +  fofList.size());
	    			    if (limit > 0 && limit >= OfflineUpdator.kFoFMaxLoadNumber) {
	    					return fofList;
	    				}
	    		    }		
	            } else {
	        	    break;
	           }
	        }     
		}	
//		Clogging.Info("getInnerGroupRecords fofList size: " +  fofList.size());
		return fofList;
	}

	public List<RcdFoFRankData> getRcdFoFData(int hostId, int limit) {		
		List<RcdFoFRankData> fofList = new ArrayList<RcdFoFRankData>();		
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
//				Clogging.Info("getRcdFoFData: group Id " + groupItem.groupId);
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
			if (OfflineUpdator.kFoFModelModifyStrategy) {
//				Clogging.Info("getRcdFoFData: hostId " + hostId + " kFoFHTableName "+ OfflineUpdator.kFoFExpHTableName);
				resultArray = fofExpHTable.get(getList);
			} else {
//				Clogging.Info("getRcdFoFData: hostId " + hostId + " kFoFHTableName "+ OfflineUpdator.kFoFHTableName);
				resultArray = fofTable.get(getList);
			}
//			Clogging.Info("getRcdFoFData: resultArray length" + resultArray.length);
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
									List<RcdFoFRankData> subFoFList = new ArrayList<RcdFoFRankData>();	
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
		getFoFProbabilityPosition(fofGroupList);  //get fof probability-based positions 
//		if( getFoFProbabilityPosition(fofPositionListMap) ) {		
			HashMap<Integer, Integer> fofGroupNumberMapTest = new HashMap<Integer, Integer>(); 		
//			if( getFoFProbabilityPosition(fofPositionListMap) ) {		
				for (Entry<Integer, Integer> kv : fofGroupNumberMapTest.entrySet()) {
					System.out.println("fofId " + kv.getKey() + " group number " + kv.getValue());
				}
				
				HashSet<Integer> fofGroupNumbers = new HashSet<Integer>(fofGroupNumberMapTest.values());
				for (Integer fofGroupNumber : fofGroupNumbers) {
					System.out.println("fofGroupNumber " + fofGroupNumber);
				}		
//			}
//		} 
	}		
}	

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
//	try {
//	fofTable  = new HTable(conf, Bytes.toBytes(kHTableName));
//} catch (IOException e) {
//	e.printStackTrace();
//}
	
//	private static HTable[] fofTable;
//	private static final int kHTableClientNumber = 1;	
//	result = fofTable[hostId % kHTableClientNumber].get(get);
//	fofTable = new HTable[kHTableClientNumber];
//	for (int i = 0; i < kHTableClientNumber; ++i) {
//		fofTable[i] = new HTable(conf, Bytes.toBytes(kHTableName));
//	}
	
	
//	Clogging.Info("hostId " + hostId + " limit " + limit);

	
//	srcTable = new HTable(context.getConfiguration().get("INPUT_RTABLE"));
//	this.desTable = new HTable[hTableClientNumber]; 			
//	desTable.setAutoFlush(false, false);
//	desTable.setWriteBufferSize(2000 * 1024 * 1024);			
//	for (int i = 0; i < hTableClientNumber; ++i) {
//		this.desTable[i] = new HTable(context.getConfiguration().get("OUTPUT_RTABLE"));
//		this.desTable[i].setWriteBufferSize(10 * 1024 * 1024);
//		this.desTable[i].setAutoFlush(false, false);
//	}			
//	srcTable.close();
//	desTable.close();			
//    System.out.println("fof mut number" + ffrd.getMutualFriendNumber());
//	   System.out.println(" hostId " + hostId + " fofId " + fofId + " rank " + rank + " mutual friend number " + mutualFriendNumber);
	
	
//	public static List<Integer> getIdListFromFile(String filePath) {
//		List<Integer> idList = new ArrayList<Integer>();
//		
//		File file = new File(filePath);
//		if (!file.exists()) {
//			Clogging.Error(filePath + " doesn't exist!");
//			return idList;
//		}
//		try {
//			BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(file)));
//			String line = reader.readLine();
//			while (null != line) {
//				int id = Integer.valueOf(line);
//				idList.add(id);
//				line = reader.readLine();
//			}
//			reader.close();
//		} catch (FileNotFoundException e) {
//			e.printStackTrace();
//		} catch (IOException e) {
//			e.printStackTrace();
//		} 
//		return idList;
//	}
	
	
//	public static void main(String[] args) {
//		if (2 != args.length) {
//			Clogging.Error("sgutil error args!");
//			System.exit(0);
//		}
//		final int kMaxFectchNumber = 40;
//		Clogging.InitLogger("sgutil");
////		Set<Integer> hostIdSet = xce.socialgraph.recommend.fof.fromhbase.RcdFoFUtil.getIdListFromFile();
//		String kHostIdsFile = args[0];  
//		String hTableName = args[1];  
//		List<Integer> idList = RcdFoFUtil_OneToAll_Group.getIdListFromFile(kHostIdsFile);   
//		if (!idList.isEmpty()) {
//			System.out.println("*******************************************************data load over**************************************************");
//			System.out.println("load id number " + idList.size());		
////			List<Integer> idList = new ArrayList<Integer>();
////			idList.addAll(hostIdSet);
//			Clogging.Debug("NewRecommendUpdator idList size:" + idList.size());
////			RcdFoFUtil.getInstance(idList, hTableName);
//			if (!idList.isEmpty()) {
//				initialTime = System.currentTimeMillis();
//				for (int id : idList) {
//					startTime = System.currentTimeMillis();
//					List<RcdFoFRankData>  fofList = RcdFoFUtil_OneToAll_Group.getInstance(idList, hTableName).getRcdFoFData(id, kMaxFectchNumber);
//					getCounter++;
//					endTime = System.currentTimeMillis();
////					Clogging.Info("getCounter " + getCounter + " toal time cost " + (endTime - initialTime) + 
////							" time period " + (endTime - startTime) + " hostId " + id + " size " + fofList.size());
//					if (getCounter % GetCounterUnit == 0) {
//						Clogging.Info("getCounter " + getCounter + " toal time cost " + (endTime - initialTime) + 
//								" time period " + (endTime - startTime) + " hostId " + id + " size " + fofList.size());
//					}
//				}
//			}
//		}
//		System.exit(0);
//	}

		
		
//		int hostId = 2200;
//		int limit = 0;
//		try {
////		Scanner scan = new Scanner(System.in);
//		BufferedReader br = new BufferedReader(new InputStreamReader(System.in)); 
//		System.out.println("Input the hostId: ");
//		String in1 = br.readLine();
////		int hostId = scan.next();
//		hostId = Integer.parseInt(in1);
//		System.out.println("Input the limit: ");
//		String in2 = br.readLine();
////		int limit = scan.next();
//		limit = Integer.parseInt(in2);
//		} catch (IOException e) {
//			e.printStackTrace();
//		}
//		List<RcdFoFRankData> result = RcdFoFUtil.getInstance().getRcdFoFData(hostId, limit);
//		for (RcdFoFRankData record : result) {
//			System.out.println(record.getHostId() + " : " + record.getFoFId() + " : " 
//					+ record.getRank() + " : " + record.getMutualFriendNumber() + " : " 
//					);
//			String ids = "";
//			for (Integer id : record.getmutualFriendIdList()) {
//				ids += id + " ";
//			}
//			System.out.println(ids);
//		}
////		System.out.println("result size: " + result.size());
////		System.exit(0);
//	}
//}


//List<RcdFoFRankData> ffrdl = new ArrayList<RcdFoFRankData>();
//List<RcdFoFRankData> fofList = new ArrayList<RcdFoFRankData>();
//try {
//	resultScanner = htable.getScanner(scan);
////	int mutualFriendNumber = 0;
//	int mutualFriendId = 0;
////	List<RcdFoFRankData> ffrdl = new ArrayList<RcdFoFRankData>();
//	for (Result item : resultScanner) {
//		int currentHostId = Bytes.toInt(item.getRow(), 0, Bytes.SIZEOF_INT);
//		if (currentHostId == hostId) {
//			RcdFoFRankData ffrd = new RcdFoFRankData();
//			ffrd.setHostId(currentHostId);
//			ffrd.setFoFId( Bytes.toInt(item.getRow(), Bytes.SIZEOF_INT, Bytes.SIZEOF_INT)) ;					
////			System.out.println("hostId " + hostId + " fofId " + ffrd.getFoFId() );
//			for (KeyValue kv : item.list()) {
//				ffrd.setRank(Bytes.toFloat(kv.getValue(), 0));
////				System.out.print(" rank " + ffrd.getRank());
//				ffrd.setMutualFriendNumber( Bytes.toInt(kv.getValue(), 2*Bytes.SIZEOF_INT, Bytes.SIZEOF_INT));
////				System.out.print(" mutual friend number " + ffrd.getMutualFriendNumber());
//				if (ffrd.getMutualFriendNumber() > 0) {
////						byte[] mutualFriendIds = new byte[kv.getValueLength()];
//					for (int i = 0; i < ffrd.getMutualFriendNumber(); ++i) {								
//						mutualFriendId = Bytes.toInt(kv.getValue(), (i+3)*Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
//						ffrd.getmutualFriendIdList().add(mutualFriendId);
////						System.out.print(" " + i + " "+ mutualFriendId);
//					}
//					System.out.println("");
//				}
//			}	
//			ffrdl.add(ffrd);
//		}
//		else {
//			System.out.println(" currentHostId " + currentHostId);
//		}
//	}			
////	Collections.sort(ffrdl);			
//	for (RcdFoFRankData ffrd : ffrdl) {
//		if (fofList.size() < kMaxFoFNumber) {
//			fofList.add(ffrd);
//		}
//	}
////	List<RcdFoFData> fofList = new ArrayList
////	for (RcdFoFRankData ffrd : ffrdl) {
////		System.out.println("fofId " + ffrd.getFoFId() +  " rank " + ffrd.getRank());
//////		RcdFoFData rcdFoFData = new RcdFoFData();
//////		rcdFoFData.setHostId(hostId);
//////		rcdFoFData.setFoFId(ffr.fofId);
//////		rcdFoFData.setRank(ffr.rank);
//////		rcdFoFData.setMutualFriendNumber(ffr.mutualFriendNumber);
//////		rcdFoFData.setmutualFriendIdList(ffr.mutualFriendIdList);
////	}
//} catch (IOException e) {
//	// TODO Auto-generated catch block
//	e.printStackTrace();
//}  finally {
//	if (null != resultScanner) {
//		resultScanner.close();
//	}
//}		
//return fofList;
//}




//
//Connection rcdFoFConnection = null;
//Statement rcdFoFStatement = null;
//ResultSet rcdFoFResultSet = null;
//try { 
//	final String kTableName = TableName(kTableNamePrefix, hostId);
//	rcdFoFConnection = XceAdapter.getInstance().getReadConnection(kDbInstanceName, kTableName);
//	rcdFoFStatement = rcdFoFConnection.createStatement();
//	String sql = "";
//	if (limit <= 0) {
//		sql = "SELECT host_id, fof_id, rank, mutual_friend_number, mutual_friend_ids" +
//				" FROM " + kTableName + " WHERE host_id = " + hostId + " LIMIT " + 100;	    				
//	} else {
//		sql = "SELECT host_id, fof_id, rank, mutual_friend_number, mutual_friend_ids" +
//				" FROM " + kTableName + " WHERE host_id = " + hostId + " LIMIT " + limit;	 
//	}
//	rcdFoFResultSet = rcdFoFStatement.executeQuery(sql);
//	if (null == rcdFoFResultSet) {
//		return rcdFoFList;
//	}
//	
//	while (rcdFoFResultSet.next()) {
//		RcdFoFData record = new RcdFoFData();
//		if (kEmptyId != rcdFoFResultSet.getInt("fof_id")) {
//    		record.setHostId(rcdFoFResultSet.getInt("host_id"));
//    		record.setFoFId(rcdFoFResultSet.getInt("fof_id"));
//    		record.setRank(rcdFoFResultSet.getFloat("rank"));
//    		record.setMutualFriendNumber(rcdFoFResultSet.getInt("mutual_friend_number"));
//    		record.setMutualFriendIds(IntSeqSerialize.unserialize(rcdFoFResultSet.getBytes("mutual_friend_ids")));
//	    	rcdFoFList.add(record);
//    	} else {
//    		break;
//    	}
//	}	    
//	
//} catch (SQLException e) {
//	e.printStackTrace();
//} finally {
//	try {
//    	if (null != rcdFoFConnection) {
//    		rcdFoFConnection.close();
//    	}
//    	if (null != rcdFoFStatement) {
//    		rcdFoFStatement.close();
//    	}
//    	if (null != rcdFoFResultSet) {
//    		rcdFoFResultSet.close();
//    	}
//	} catch (SQLException e) {
//		e.printStackTrace();
//	}
//}
