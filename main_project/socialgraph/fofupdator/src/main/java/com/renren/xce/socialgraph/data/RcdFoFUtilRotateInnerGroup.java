/*
 * Function: SocialGraph FoF friends recommendation service inner group rotation
 * strategy implementation, which loads data in a certain contiguous address space in a group 
 * with rotated style and limitation of max number of kMaxFoFLoadNumber from hbase each time 
 * for each user   
 * Developer: xiongjunwu
 * Email: Junwu.xiong@renren-inc.com
 * Date: 2012-09-01
 */

package com.renren.xce.socialgraph.data;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.client.Get;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.client.Result;
import org.apache.hadoop.hbase.util.Bytes;
import org.apache.hadoop.hbase.util.MD5Hash;

import xce.socialgraph.util.Clogging;

import com.renren.xce.socialgraph.data.RcdFoFData;
import com.renren.xce.socialgraph.updator.FoFUpdator;


public class RcdFoFUtilRotateInnerGroup {
	private static RcdFoFUtilRotateInnerGroup instance = null;  
	private static Configuration conf;
	private static String kHTableName;
	private static HTable fofTable;
    public static Set<Integer> hostIdSet = new HashSet<Integer>();
	public final static byte[] FAMILILY = Bytes.toBytes("f");
	private final static int kFoFSingleGroupIndex = 0;
	public static int kFoFMinPosition = 20;
	public static int kFoFMaxPosition = 90;	
	public final static byte[] QUALIFIER = Bytes.toBytes(kFoFSingleGroupIndex * FoFUpdator.kFoFGroupSize);
    public final static int kGroupSize = FoFUpdator.kFoFGroupSize;
    public final static int kGroupIndex = kFoFSingleGroupIndex;
    
	
	private RcdFoFUtilRotateInnerGroup() {	
	}
	
	public static RcdFoFUtilRotateInnerGroup getInstance() {
		if (null == instance) {
			synchronized (RcdFoFUtilRotateInnerGroup.class) {
				if (null == instance) { 
					try {
						conf = new Configuration();
						HBaseConfiguration.addHbaseResources(conf);
						kHTableName = FoFUpdator.kFoFHTableName;					
						try {
							fofTable  = new HTable(conf, Bytes.toBytes(kHTableName));
  					    } catch (IOException e) {							
							e.printStackTrace();
						}						
						instance = new RcdFoFUtilRotateInnerGroup();
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

	public List<RcdFoFData> getRcdFoFData(int hostId, int limit) {
		List<RcdFoFData> fofList = new ArrayList<RcdFoFData>();		
		if (hostId < 0) {
			return fofList;
		}
		
		String keyString = String.valueOf(hostId);
		String hash = MD5Hash.getMD5AsHex(keyString.getBytes());
		String rehash = MD5Hash.getMD5AsHex((hash + keyString).getBytes());
		String key = rehash.substring(0,10).toUpperCase() + keyString;
		final Get get = new Get(key.getBytes());
		get.setCacheBlocks(false);
		Result result;
		try {
			result = fofTable.get(get.addColumn(FAMILILY, QUALIFIER));			
			if (!result.isEmpty()) {
				byte[] value = result.getValue(FAMILILY, QUALIFIER);
				if (value.length > 0) {
					System.out.println("value " + value.length);
				    final int recordSize = Bytes.SIZEOF_INT + Bytes.SIZEOF_FLOAT + Bytes.SIZEOF_INT + 
				    		FoFUpdator.kDisplayMutualFriendNumber * Bytes.SIZEOF_INT;
				    int leftPos = kGroupIndex * kGroupSize + kFoFMinPosition;
				    int rightPos = kGroupIndex * kGroupSize + kFoFMaxPosition;	
				    if (leftPos > rightPos) {
				    	Clogging.Error("getRcdFoFData: Wrong kMinPosistion and kMaxPosition parameters!");
				    	return fofList;
				    }
				    if ((leftPos >= 0) && (rightPos >= 0)) {
				    	if (leftPos < kGroupIndex * kGroupSize) {
				    		leftPos = kGroupIndex * kGroupSize;
				    	}
				    	if (rightPos > kGroupSize * (kGroupIndex + 1)) {
				    		rightPos = kGroupSize * (kGroupIndex + 1);
				    	}
				    } else {
				    	return fofList;
				    }		
				    int fofCounter = 0;
		            for (int j = leftPos; j < rightPos; ++j) {
		        	   RcdFoFData ffrd = new RcdFoFData();
		        	   ffrd.setHostId(hostId);
			           int basePos = (j - leftPos) * recordSize;
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
				} else {//end if if (value.length > 0)
					Clogging.Error("getRcdFoFData: Getting value from the qualifier is null, value length: "+ value.length);
				}
			} else { //end if (!result.isEmpty()
				Clogging.Error("getRcdFoFData: Fecthing result fof data from hbase failed, result size: " + result.size() 
						+ " group index: " + kFoFSingleGroupIndex + " FAMILILY length "	+ FAMILILY.length + "  " + 
						" QUALIFIER length " + QUALIFIER.length);
			}
		} catch (IOException e) {
			e.printStackTrace();
			return fofList;
		}		
		return fofList;
	}		
}	