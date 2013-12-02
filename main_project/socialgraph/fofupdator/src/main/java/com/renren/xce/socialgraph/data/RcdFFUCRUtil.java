/*************************************************************************************
 * Function: SocialGraph friends recommendation of friend cluster algorithm data items
 * accessing interfaces 
 * Developer: xiongjunwu
 * Email: Junwu.xiong@renren-inc.com
 * Date: 2012-12-07
*************************************************************************************/
/*
 *     FFUCR raw file record format      
 *     hostId  /t rcdFrdId1,FFUCR,ClusterSize,OptClusterMembers,ExplanationTypeFlag;rcdFrdId2,...;rcdFrdIdn,... 
*/

package com.renren.xce.socialgraph.data;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import xce.socialgraph.util.Clogging;


public class RcdFFUCRUtil {
	private volatile static RcdFFUCRUtil instance = null;    
    private static String kFFUCRFileName = null;
//    private final static int kMaxDisplayNumber = 10;
	private static final int kMaxRcdFriendNumber = 100; 
    private RcdFFUCRUtil(){
	}

	public static Map<Integer, List<RcdFFUCRData> > hostId2RcdFriendListMap = new HashMap<Integer, List<RcdFFUCRData> >(); //to store all fofs id with time	
    
	public static RcdFFUCRUtil getInstance(String ffucrFile) {
		if (null == instance) {
			synchronized (RcdFFUCRUtil.class) {
				if (null == instance) { 
					try {
						kFFUCRFileName = ffucrFile;
						getRcdFoFData();
						instance = new RcdFFUCRUtil();	
						
					} catch (Exception e) {
						e.printStackTrace();
						instance = null;
					}				
				}
			}
		}
		
		return instance;
	}	
	
	public static boolean getRcdFoFData() {
		Clogging.Info("Read clustering file");
		List<RcdFFUCRData> rcdFoFRankList = new ArrayList<RcdFFUCRData>();
		File file = new File(kFFUCRFileName);
		BufferedReader reader = null;
		try {
			reader = new BufferedReader(new FileReader(file));
			String record = null;
			int rcdFriendId = 0;
			int clusterSize = 0;
			
			int explanFlag = 0;
			int actualDisplayNumber = 0;
			while (null != (record = reader.readLine())) {
			//	System.out.println("debug:\t" + record);
				String[] totalSplits = record.split("\t");
				int hostId = Integer.valueOf(totalSplits[0]);				
						
				String[] rcdPartSplits = totalSplits[1].split(";");
  			    for (int i = 0; i < rcdPartSplits.length; ++i) {
					String[] fields = rcdPartSplits[i].split(",");	
					RcdFFUCRData item = new RcdFFUCRData();
					item.setHostId(hostId);		
					rcdFriendId = Integer.valueOf(fields[0]);
					item.setRcdFriendId(rcdFriendId);	
					clusterSize = Integer.valueOf(fields[2]);
					item.setClusterSize(clusterSize);
				//	actualDisplayNumber = clusterSize > kMaxDisplayNumber ? clusterSize : kMaxDisplayNumber;
					actualDisplayNumber = fields.length -4;
					
					if (actualDisplayNumber > 0) {
						for (int j = 0; j < actualDisplayNumber; ++j) {								
							item.getClusterMemberList().add(Integer.valueOf(fields[j+3]));
							
						}
					}
					explanFlag = Integer.valueOf(fields[3 + actualDisplayNumber]);
					item.setExplanFlag(explanFlag);
					
					if (hostId2RcdFriendListMap.isEmpty()) {
						rcdFoFRankList = new ArrayList<RcdFFUCRData>();
						rcdFoFRankList.add(item); 
						hostId2RcdFriendListMap.put(hostId, rcdFoFRankList);
						
					} else {
						if (null == hostId2RcdFriendListMap.get(hostId)) {
							rcdFoFRankList = new ArrayList<RcdFFUCRData>();
							rcdFoFRankList.add(item); 
							hostId2RcdFriendListMap.put(hostId, rcdFoFRankList);
						}  else {
							if (kMaxRcdFriendNumber > hostId2RcdFriendListMap.get(hostId).size()) {
								hostId2RcdFriendListMap.get(hostId).add(item);
							}
						}
					} 
				}	
  			    
  			    
			}							
		reader.close();
		Clogging.Info("Reading cluster file over");
		return true;
		} catch (IOException e) {
			e.printStackTrace();
		}  finally {
			if (reader != null) {
				try {
					reader.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			} 
	    }		
		return false;
	}
}