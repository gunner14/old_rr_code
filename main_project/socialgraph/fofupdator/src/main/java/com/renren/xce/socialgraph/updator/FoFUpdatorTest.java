/*
 * Function: Unit test of FoFUpdatorTest class 
 * Developer: xiongjunwu
 * Email: Junwu.xiong@renren-inc.com
 * Date: 2012-12-11
 */

package com.renren.xce.socialgraph.updator;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import xce.socialgraph.util.Clogging;

public class FoFUpdatorTest {
	
	/* Friends-Of-Friends recommendation configuration parameters, including
	 * hostId files, hbase table name, hbase column group size, minimum and 
	 * maximum fof recommendation number dumped to cache, maximum fof loaded
	 * number from hbase etc.
	 */		
    private static String kHostIdsFile;  
	public static String kWhiteHostIdsFile;
    public static HashSet<Integer> whiteIdListSet = new HashSet<Integer>();

    public static String kFoFHTableName;  
    public static int kFoFMaxGroupNumber;
	public static String kFoFGroupConfFile;
    public static int kFoFGroupSize;
	public static int kFoFMaxLoadNumber;	
	public static int kFoFMaxRcdNumber;
	public static final int kDisplayMutualFriendNumber = 10;
	public static final int kMaxFriendNumber = 10000;
	
    public static String kFoFExpHTableName;  
	public static String kSgFriendListHTableName = "relation_20120825";
	public static String kSgBlockHTableName = "sg_block";
	public static String kSgFriendListDeletedHTableName = "sg_friendrelation_block";
	
	public static boolean kFoFRotateStrategy;
	public static boolean kFoFModelModifyStrategy;	
	public static boolean kFoFProbabilityStrategy;

	private static long initialTime = 0;
	private static long endTime = 0;

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
			Clogging.Info("getIdListFromFile: whiteIdListSet size: " + whiteIdListSet.size());
			while (null != line) {
				int id = Integer.valueOf(line);
				if ((!kFoFModelModifyStrategy && !kFoFRotateStrategy && !kFoFProbabilityStrategy))  { //if is data loading is online loading 
					if (!whiteIdListSet.contains(id)) { //online loading, excluding the white id list for experiment
						idList.add(id);
					}
				} else { //the white id list for experiment
					idList.add(id);
				}
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
	
	public static void createRecommendData(List<Integer> idList) {
		ThreadPoolExecutor exec = new ThreadPoolExecutor(150, 300, 10,
				TimeUnit.SECONDS, new LinkedBlockingDeque<Runnable>());
		
		for (Integer id : idList) {
			exec.execute(new CreateRecommendDataThread(id));
		}		
		while (exec.getActiveCount() > 0) {
			try {
				Thread.sleep(20000);
				Clogging.Debug("activeCount : " + exec.getActiveCount());
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		
		try {
			Thread.sleep(200000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		exec.shutdown();
 	}
	
	public static void main(String[] args) {
		if (15 != args.length) {
			Clogging.Error("OfflineUpdator args wrong! ");  
			Clogging.Error("OfflineUpdator args list: ");  
			Clogging.Error("0 kFoFHTableName " + args[0]);
			Clogging.Error("1 kFoFMaxGroupNumber " + args[1]);
			Clogging.Error("2 kFoFGroupSize " + args[2]);
			Clogging.Error("3 kHostIdsFile " + args[3]);
			Clogging.Error("4 kFoFMaxRcdNumber " + args[4]);
			Clogging.Error("5 kFoFMaxLoadNumber " + args[5]);
			Clogging.Error("6 kFoFGroupConfFile " + args[6]);
			Clogging.Error("7 kWhiteHostIdsFile " + args[7]);
			Clogging.Error("8 kFoFExpHTableName " + args[8]);
			Clogging.Error("9 kFoFRotateStrategy " + args[9]);
			Clogging.Error("10 kFoFModelModifyStrategy " + args[10]);
			Clogging.Error("11 kFoFProbabilityStrategy " + args[11]);
			Clogging.Error("12 kSgFriendListHTableName " + args[12]);
			Clogging.Error("13 kSgBlockHTableName" + args[13]);
			Clogging.Error("14 kSgFriendListDeletedHTableName" + args[14]);
			System.exit(0);
		} 
		Clogging.InitLogger("OfflineUpdator");
		kFoFHTableName = args[0];  
		kFoFMaxGroupNumber = Integer.valueOf(args[1]);
		kFoFGroupSize = Integer.valueOf(args[2]);
		kHostIdsFile = args[3];
		kFoFMaxRcdNumber = Integer.valueOf(args[4]);
		kFoFMaxLoadNumber = Integer.valueOf(args[5]);
		kFoFGroupConfFile = args[6];
		kWhiteHostIdsFile = args[7];
		kFoFExpHTableName = args[8];
		kFoFRotateStrategy = Boolean.valueOf(args[9]);
		kFoFModelModifyStrategy = Boolean.valueOf(args[10]);
		kFoFProbabilityStrategy = Boolean.valueOf(args[11]);
		kSgFriendListHTableName = args[12];
		kSgBlockHTableName = args[13];
		kSgFriendListDeletedHTableName = args[14];
		
		List<Integer> whiteIdList = new ArrayList<Integer>();
		whiteIdList = FoFUpdatorTest.getIdListFromFile(kWhiteHostIdsFile);	
		Clogging.Info("whiteIdList size: " + whiteIdList.size());
		whiteIdListSet.addAll(whiteIdList);
		Clogging.Info("whiteIdListSet size: " + whiteIdListSet.size());
		List<Integer> idList = FoFUpdatorTest.getIdListFromFile(kHostIdsFile);	
		
		initialTime = System.currentTimeMillis();
		if (!idList.isEmpty()) {
			if (!kFoFModelModifyStrategy && !kFoFRotateStrategy && !kFoFProbabilityStrategy) {
				Clogging.Info("*******************************************************idList load over**************************************************");
				Clogging.Info("initialTime " + initialTime + " idList id number " + idList.size());
				FoFUpdatorTest.createRecommendData(idList);
				endTime = System.currentTimeMillis();
				Clogging.Info("endTime " + endTime + " kHostIdsFile: " + kHostIdsFile + " idList size: " + idList.size() + " total time cost " + (endTime - initialTime));
			}			
		} 
		
		if (!whiteIdList.isEmpty() && (kFoFRotateStrategy || kFoFModelModifyStrategy || kFoFProbabilityStrategy)) {
			System.out.println("*******************************************************whiteIdList load over**************************************************");
			if (kFoFRotateStrategy) {
				Clogging.Info("kFoFRotateStrategy beginning");
			}
			if (kFoFProbabilityStrategy) {
				Clogging.Info("kFoFProbabilityStrategy beginning");
			}
			if (kFoFModelModifyStrategy) {
				Clogging.Info("kFoFModelModifyStrategy beginning");
			}
			System.out.println("initialTime" + initialTime + " whiteIdList id number " + whiteIdList.size());
			FoFUpdatorTest.createRecommendData(whiteIdList);	
			endTime = System.currentTimeMillis();
			Clogging.Info("endTime " + endTime + " kWhiteHostIdsFile: " + kWhiteHostIdsFile + " whiteIdList size: " + whiteIdList.size() + " total time cost " + (endTime - initialTime));	
		}
		System.exit(0);
	}
}  


















///**
// * 获取昨天登录用户的id列表
// * @return
// */
//public static List<Integer> getIdListFromLogin() {
//	List<Integer> idList = new ArrayList<Integer>();
//	
//	Calendar cal = Calendar.getInstance();
//	cal.set(Calendar.HOUR_OF_DAY, 0);
//	cal.set(Calendar.MINUTE, 0);
//	cal.set(Calendar.SECOND, 0);
//	cal.set(Calendar.MILLISECOND, 0);
//	long millisTime = cal.getTimeInMillis() - 24 * 3600 * 1000; // 昨天的时间秒数	
//	
//	SimpleDateFormat formater = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
//	int interval = 10 * 1000;				//以10秒为时间间隔
//	for (int i = 10 * 1000; i < 24 * 3600 * 1000; i = i + interval) {
//		Date startTime = new Date();
//		startTime.setTime(millisTime);
//		String startTimeStr = formater.format(startTime);
//		
//		millisTime += interval;
//		Date endTime = new Date();
//		endTime.setTime(millisTime);
//		String endTimeStr = formater.format(endTime);
//		try {
//			Connection userTimeConn = XceAdapter.getInstance().getReadConnection("user_time");
//			Statement stmt = userTimeConn.createStatement();
//			String sql = "SELECT * FROM user_time WHERE lastlogintime BETWEEN \""
//					+ startTimeStr + "\" AND \"" + endTimeStr + "\"";
//			Clogging.Debug(sql + " idlist_size:" + idList.size());
//			ResultSet result = stmt.executeQuery(sql);
//			if (result != null) {
//				while (result.next()) {
//					idList.add(result.getInt("id"));
//				}
//			} 
//			userTimeConn.close();
//			stmt.close();
//		} catch (SQLException e) {
//			e.printStackTrace();
//		}
//	}
//	return idList;
//}
