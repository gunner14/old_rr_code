/*
 * Function: SocialGraph FoF friends recommendation service HBase Table row
 * record relevant parameters, including group format, recommended fof record format etc.
 * Developer: xiongjunwu
 * Email: junwu.xiong@renren-inc.com
 * Date: 2012-09-04
 */

package com.renren.xce.socialgraph.builder;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.TreeSet;

import org.apache.hadoop.hbase.util.Bytes;

import xce.socialgraph.util.Clogging;

import com.renren.xce.socialgraph.updator.OfflineUpdator;

public class FoFRow {

	public static FoFRow instanceFoFRow = null;
	public static int kMaxFoFGroupNumber = OfflineUpdator.kFoFMaxGroupNumber;
    public final static int kRecordSize = Bytes.SIZEOF_INT + Bytes.SIZEOF_FLOAT + Bytes.SIZEOF_INT + OfflineUpdator.kDispMutFrdNum * Bytes.SIZEOF_INT;
    public final static int kFoFGroupSize = OfflineUpdator.kFoFGroupSize;
    public final static String kFoFGroupConfFile = OfflineUpdator.kFoFGroupConfFile;
//	public static List<FoFGroup> kFoFGroupList;
	
    
	public static FoFRow getInstance() {
		if (null == instanceFoFRow) {
			synchronized (FoFRow.class) {
				if (null == instanceFoFRow) { 
					try {
						instanceFoFRow = new FoFRow();
//						kFoFGroupList = getFoFGroupListFromFile(OfflineUpdator.kFoFGroupConfFile);						
					} catch (Exception e) {
						e.printStackTrace();
					}				
				}
			}
		}
		return instanceFoFRow;
	}		
    
	public List<FoFGroup> getFoFGroupList() {
		List<FoFGroup> fofGroupList = new ArrayList<FoFGroup>();		
		File file = new File(kFoFGroupConfFile);
		if (!file.exists()) {
			Clogging.Error("getFoFGroupListFromFile: " + kFoFGroupConfFile + " doesn't exist!");
			return fofGroupList;
		}
		try {
			BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(file)));
			String line = reader.readLine();
			//fof group configuration file format:
			//groupId:startIndex:endIndex
			int fofItemCounter = 0;
			while (null != line) {
				String[] groupFileds = line.split(":");
				if (groupFileds[0].isEmpty() || groupFileds[1].isEmpty() || groupFileds[2].isEmpty()) {
					Clogging.Error("getFoFGroupListFromFile: FoF group configuration fileds wrong!");
					return fofGroupList;
				}
				FoFGroup fofGroup = new FoFRow().new FoFGroup();
				fofGroup.groupId = Integer.valueOf(groupFileds[0]);
				fofGroup.innerGroupStartIndex = Integer.valueOf(groupFileds[1]);
				fofGroup.innerGroupEndIndex = Integer.valueOf(groupFileds[2]);
//				Clogging.Info("groupId: " + fofGroup.groupId + " innerGroupStartIndex: " + fofGroup.innerGroupStartIndex 
//						+ " innerGroupEndIndex: " + fofGroup.innerGroupEndIndex);
				if ((fofGroup.groupId > FoFRow.kMaxFoFGroupNumber) 
						|| (fofGroup.innerGroupStartIndex >= fofGroup.innerGroupEndIndex) 
						|| (fofGroup.innerGroupStartIndex < 0) 
						|| (fofGroup.innerGroupEndIndex < 0) 
						|| (fofGroup.innerGroupEndIndex > FoFRow.kFoFGroupSize)) {
					Clogging.Error("getFoFGroupListFromFile: FoF group configuration fileds wrong!");
					return fofGroupList;
				}
				fofGroup.innerGroupItemNumber = fofGroup.innerGroupEndIndex - fofGroup.innerGroupStartIndex;	
				if (fofGroup.innerGroupItemNumber > FoFRow.kFoFGroupSize) {
					Clogging.Error("getFoFGroupListFromFile: FoF group item number beyond the group size!");
					return fofGroupList;
				}
				fofItemCounter += fofGroup.innerGroupItemNumber;
				if (fofItemCounter > OfflineUpdator.kFoFMaxLoadNumber) {
					if (!OfflineUpdator.kFoFProbabilityStrategy) {
						Clogging.Error("getFoFGroupListFromFile: FoF total group item number in the configuration beyond kFoFMaxLoadNumber!");
						return fofGroupList;
					}					
				}
				fofGroupList.add(fofGroup);
				line = reader.readLine();
			}
			reader.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} 
		return fofGroupList;
	}
	
    public class FoFGroup {
    	public int groupId = 0;
    	public int innerGroupStartIndex = 0;
    	public int innerGroupEndIndex = 0;
    	public int innerGroupItemNumber = 0;
    	public TreeSet<Integer> fofPositionSet;
    	public FoFGroup() {
    		this.groupId = 0;
    		//inner group relative start and end positions
        	this.innerGroupStartIndex = 0;
        	this.innerGroupEndIndex = 0;
        	this.innerGroupItemNumber = 0;
		}
    }
}