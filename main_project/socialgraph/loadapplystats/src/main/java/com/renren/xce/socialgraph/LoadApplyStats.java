package com.renren.xce.socialgraph;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.text.SimpleDateFormat;
import java.util.BitSet;
import java.util.Calendar;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

import com.xiaonei.xce.XceAdapter;
import com.xiaonei.xce.socialgraph.BusinessCacheAdapter;
import com.xiaonei.xce.socialgraph.proto.GeneratedItems.Item;
import com.xiaonei.xce.socialgraph.proto.GeneratedItems.Items;

import xce.socialgraph.util.Clogging;

/**
 * Load data of applying from db to cache
 * @author zhangnan
 * @email zhangnan@renren-inc.com
 */
public class LoadApplyStats {
	private static int bitsetSize = 31;		//set 31 as size of bitset
	private static String biz = "USERAPPLY";		//business name
	private static Map<Integer, Integer> currentMonthMap = new HashMap<Integer, Integer>();
	private static Map<Integer, Integer> lastMonthMap = new HashMap<Integer, Integer>();
	private static Map<Integer, BitSet> resultMap = new HashMap<Integer, BitSet>();
	
	/**
	 * load user's apply data into cache
	 */
	public static void loadUserApplyStats() {
		createMonthMap(getCurrentTime(), currentMonthMap);
		Clogging.Debug("currentMonthMap size:" + currentMonthMap.size());
		
		createMonthMap(getLastTime(), lastMonthMap);
		Clogging.Debug("lastMonthMap size:" + lastMonthMap.size());
		
		createResultMap(currentMonthMap, lastMonthMap);
		
		for (Entry<Integer, BitSet> item : resultMap.entrySet()) {
			int userId = item.getKey();
			int applyFrequence = getInteger(item.getValue());
			insertIntoCache(userId, applyFrequence);			//insert into cache named USERAPPLY
		}
	}
	
	/**
	 * Create month map
	 * @param time
	 * @param monthMap
	 */
	private static void createMonthMap(String time, Map<Integer, Integer> monthMap) {
		final String dbSourceName = "socialgraph_recommend_apply";
		String tableName = "user_apply_" + time;
		Clogging.Debug("tableName:" + tableName);
		int lowBoundary = 0;
		
		Connection applyConn = null;
		Statement stmt = null;
		ResultSet result = null;
		try {
			applyConn = XceAdapter.getInstance().getReadConnection(dbSourceName);
			stmt = applyConn.createStatement();
			while (true) {
				Clogging.Debug("[loadUserApplyStats] lowBounday : " + lowBoundary);
				String sql = "SELECT userid, apply_days FROM " + tableName
						+ " WHERE userid > " + lowBoundary + " LIMIT 1000";
				result = stmt.executeQuery(sql);
				
				if (result.next()) {
					do {
						try {
							int applyDays = result.getInt("apply_days");
							int userId = result.getInt("userid");
//							Clogging.Debug("userId:" + userId + " applyDays:" + applyDays + " bbsize:" + bb.length);
							lowBoundary = userId;
							monthMap.put(userId, applyDays);
						} catch (Exception e) {
							lowBoundary = result.getInt("userid") + 1;
							continue;
						}
					} while(result.next());
				} else {
					break;		//get empty result set
				}
			}
		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			try {
				if (null != applyConn)			//clean
					applyConn.close();			
				if (null != stmt)
					stmt.close();
				if (null != result)
					result.close();
			} catch (SQLException e) {
				e.printStackTrace();
			}
		}
	}
	
	private static void createResultMap(Map<Integer, Integer> currentMonthMap, Map<Integer, Integer> lastMonthMap) {
		for (Entry<Integer, Integer> it : currentMonthMap.entrySet()) {
			BitSet itemSet = new BitSet(bitsetSize);
			resultMap.put(it.getKey(), itemSet);
		}
		for (Entry<Integer, Integer> it : lastMonthMap.entrySet()) {
			if (!resultMap.containsKey(it.getKey())) {
				BitSet itemSet = new BitSet(bitsetSize);
				resultMap.put(it.getKey(), itemSet);
			}
		}
		
		Calendar cal = Calendar.getInstance();
		Date currentTime = new Date();
		currentTime.setTime(cal.getTimeInMillis() - 86400000);		//yesterday
		int currentDate = currentTime.getDate();		//get yesterday date
		
		cal.setTime(currentTime);
		cal.add(Calendar.DAY_OF_MONTH, -1);
		int lastMonthDay = cal.get(Calendar.DAY_OF_MONTH);		//get maxdate of last month
		int last = bitsetSize - currentDate;		//get size of last month
		
		Clogging.Debug("currentDate:" + currentDate + " lastMonthDay:" + lastMonthDay);
		for (Entry<Integer, BitSet> it : resultMap.entrySet()) {		//from left to right stored with apply status in recent 30 days
			if (currentMonthMap.containsKey(it.getKey())) {
				BitSet item = getBitSet(currentMonthMap.get(it.getKey()));
				for (int i = currentDate; i > 0; --i) {		
					if (item.get(i - 1)) {
						it.getValue().set(currentDate - i);
					} 
				}
			}
			if (lastMonthMap.containsKey(it.getKey())) {
				BitSet item = getBitSet(lastMonthMap.get(it.getKey()));
				for (int i = lastMonthDay; i > lastMonthDay - last; --i) {
					if (item.get(i - 1)) {
						it.getValue().set(lastMonthDay - i + currentDate);
					}
				}
			}
//			String rr = "";
//			for (int i = 0; i < 31; ++i) {
//				if (it.getValue().get(i)) {
//					rr += "1";
//				} else {
//					rr += "0";
//				}
//			}
//			Clogging.Debug("result:" + rr);
		}
	}
	
	/**
	 * Convert integer to BitSet Util
	 * @param value
	 * @return
	 */
	private static BitSet getBitSet(Integer value) {
		BitSet item = new BitSet(bitsetSize);
		for (int i = 0; i < 31; ++i) {
			if ((value & 1) != 0) {			//the low position of bitset stored with low position of interger
				item.set(i);
			} 
			value = value >>> 1;
		}
		return item;
	}
	
	/**
	 * Convert BitSet to Integer Util 
	 * @param bitSet
	 * @return
	 */
	private static Integer getInteger(BitSet bitSet) {
	    int bitInteger = 0;
	    for(int i = 0 ; i < 31; i++)
	        if(bitSet.get(i)) {				//the low position of integer stored with low position of bitset
	            bitInteger |= (1 << i);
	        }
	    return bitInteger;
	}
	
	/**
	 * Insert data into USERAPPLY cache
	 * @param userId
	 * @param applyFrequence
	 * @return
	 */
	private static boolean insertIntoCache(int userId, int applyFrequence) {
		boolean insert = true;
		Items.Builder result = Items.newBuilder();
		Item.Builder item = Item.newBuilder();
		item.setId(userId);		//store userId in Item.id session
		item.setField(applyFrequence);		//store applyFrequence in Item.field session
		result.addItems(item);		//add into result
		byte[] resultData = result.build().toByteArray();
		insert = BusinessCacheAdapter.getInstance().set(biz, userId, resultData);
		
		if (insert) {
			Clogging.Debug("Success " + userId + " " + Integer.toBinaryString(applyFrequence) + " " + resultData.length);
		} else {
			Clogging.Debug("Failed " + userId + " " + Integer.toBinaryString(applyFrequence) + " " + resultData.length);
		}
		
		return insert;
	}
	
	/**
	 * Get postfix of user_appyl table
	 * @return
	 */
	private static String getCurrentTime() {
		String time = "";
		Calendar cal = Calendar.getInstance();
		long millisTime = cal.getTimeInMillis();
		Date currentTime = new Date();
		currentTime.setTime(millisTime - 86400000);		//yesterday
		
		SimpleDateFormat formater = new SimpleDateFormat("yyyyMM");
		time = formater.format(currentTime);
		return time;
	}
	
	/**
	 * get time of last month
	 * @return
	 */
	private static String getLastTime() {
		String time = "";
		Calendar cal = Calendar.getInstance();
		long millisTime = cal.getTimeInMillis();
		Date currentTime = new Date();
		currentTime.setTime(millisTime - 86400000);		//yesterday
		cal.setTime(currentTime);
		
		int year = 0;
		int month = cal.get(Calendar.MONTH); 	// 上个月月份
		if (month == 0) {
			year = cal.get(Calendar.YEAR) - 1;
			month = 12;
		} else {
			year = cal.get(Calendar.YEAR);
		}
		if (month < 10) {
			time = year + "0" + month;
		} else {
			time = String.valueOf(year) + month;
		}
		return time;
	}
	
//	/**
//	 * For test
//	 * @param result
//	 */
//	private static void printItems(Items.Builder result) {
//	for (Item item : result.getItemsList()) {
//		Clogging.Debug(item.getMessage());
//		Clogging.Debug("" + item.getId());
//		Clogging.Debug("" + item.getField());
//		String messagesList = "";
//		for (String mes : item.getMessagesList()) {
//			messagesList += mes + " ";
//		}
//		Clogging.Debug(messagesList);
//		String fieldsList = "";
//		for (Integer fid : item.getFieldsList()) {
//			fieldsList += fid + " ";
//		}
//		Clogging.Debug(fieldsList);
//		
//		Clogging.Debug("---------------------------------------------------------------------");
//	}
//}
	
	public static void main(String[] args) {
		Clogging.InitLogger("LoadApplyStats");
		LoadApplyStats.loadUserApplyStats();
		System.exit(0);
	}
}
