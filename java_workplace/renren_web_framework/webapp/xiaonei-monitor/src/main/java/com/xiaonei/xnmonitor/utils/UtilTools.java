package com.xiaonei.xnmonitor.utils;

import java.io.File;
import java.sql.Timestamp;
import java.util.Calendar;
import java.util.Date;
import java.util.LinkedList;
import java.util.List;

import org.jrobin.core.FetchData;
import org.jrobin.core.FetchRequest;
import org.jrobin.core.RrdDb;

import net.paoding.rose.ar.Ar;

import com.xiaonei.xnmonitor.dto.ErrInfo;
import com.xiaonei.xnmonitor.dto.RenRenGroup;
import com.xiaonei.xnmonitor.dto.TimeParam;

public class UtilTools {

	List<String> files = new LinkedList<String>();

	public List<ErrInfo> errInfoList = new LinkedList<ErrInfo>();

	/**
	 * 判断字符串数组中strArr是不是同时都不等于NULL和空串。
	 * 
	 * @param strArr需要验证的字符串数组
	 * @return true表示strArr数组中不存在空串,也同时不存在NULL
	 */
	public static boolean isStrEmptyOrNull(String... strArr) {
		for (String str : strArr) {
			if (isNull(str) || isEmptyString(str)) {
				return true;
			}
		}
		return false;
	}

	/**
	 * 判断字符串str是不是等于Null
	 * 
	 * @param str
	 * @return str==null返回true,str!= null返回false;
	 */
	public static boolean isNull(String str) {
		return null == str;
	}

	/**
	 * 判断字符串str是不是空串“”
	 * 
	 * @param str
	 * @return str如果是空串“”则返回true,否则返回false
	 */
	public static boolean isEmptyString(String str) {
		return "".equals(str);
	}

	/**
	 * 查询group中给定的groupname 的分组是否已经存在
	 * 
	 * @param groupName
	 *            组的名称:UGC,SNS等等
	 * @return如果给定的groupName 在Group中已经存在了返回true，如果不存在则返回False
	 */
	public static boolean isGroupExsits(String groupName) {
		String sql = "select * from groups where group_name= ?";
		List<Object[]> resultList = Ar.sql(sql, groupName);
		if (resultList.size() > 0)
			return true;
		else
			return false;

	}

	/**
	 * 返回RenRenGroup里面现在已经有了的所有的Group列表。
	 * 
	 * @return返回List里面都是 RenRenGroup的对象表示的现在已经创建的所有的组名称
	 */
	public static List<RenRenGroup> listRenRenGroups() {
		String sql = "select distinct group_name,group_ful_name, leader_name ,"
				+ "leader_email ,group_email ,leader_mobile from groups order by group_name ";
		List<Object[]> resultList = Ar.sql(sql);
		List<RenRenGroup> renrenGroupList = ObjectPack
				.getRenRenGroupList(resultList);
		return renrenGroupList;
	}

	public static RenRenGroup listRenRenGroup(String groupName) {
		String sql = "select distinct group_name,group_ful_name, leader_name ,"
				+ "leader_email ,group_email ,leader_mobile from groups where group_name=? ";
		List<Object[]> resultList = Ar.sql(sql, groupName);
		List<RenRenGroup> renrenGroupList = ObjectPack
				.getRenRenGroupList(resultList);
		return renrenGroupList.get(0);
	}

	/**
	 * 返回所有已经归档的域名列表
	 * 
	 * @return
	 */
	public static List<String> listGroupedDomains() {
		String sql = "select distinct domain ,domain from domainclassfication";
		List<Object[]> resultList = Ar.sql(sql);
		List<String> groupedDomainsList = ObjectPack.getDomainsList(resultList);
		return groupedDomainsList;
	}

	/**
	 * 按照给定的GroupName 返回在Groupname旗下的所有域名
	 * 
	 * @param groupName
	 * @return
	 */
	public static List<String> listGroupedDomains(String groupName) {
		if (groupName.equals("all")){
			return UtilTools.listAllDomains();
		}
		if(groupName.equals("other")){
			return UtilTools.listUngroupedDomains();
		}
		String sql = "select distinct domain ,domain from domainclassfication where group_name =? order by domain ";
		List<Object[]> resultList = Ar.sql(sql, groupName);
		List<String> groupedDomainsList = ObjectPack.getDomainsList(resultList);
		return groupedDomainsList;
	}

	/**
	 * 返回所有的Domains
	 * 
	 * @return
	 */

	public static List<String> listAllDomains() {
		String sql = "select distinct domain ,domain from domains order by domain";
		List<Object[]> resultList = Ar.sql(sql);
		List<String> allDomainsList = ObjectPack.getDomainsList(resultList);
		return allDomainsList;
	}

	/**
	 * 返回所有的没有归类域名
	 * 
	 * @return
	 */
	public static List<String> listUngroupedDomains() {
		List<String> allDomainsList = listAllDomains();
		List<String> groupedDomainsList = listGroupedDomains();
		List<String> toFilterDomainsList = getFilterDomains();
		allDomainsList = filterDomains(allDomainsList, toFilterDomainsList);
		allDomainsList = filterDomains(allDomainsList, groupedDomainsList);
		return allDomainsList;
	}

	/**
	 * 返回所有需要filter的域名。
	 * 
	 * @return
	 */
	public static List<String> getFilterDomains() {
		List<String> toFilterDomains = new LinkedList<String>();
		return toFilterDomains;
	}

	/**
	 * 过滤掉需要过滤的域名。
	 * 
	 * @param allDomains
	 * @param filterDomains
	 * @return
	 */
	public static List<String> filterDomains(List<String> allDomains,
			List<String> filterDomains) {
		for (int i = 0; i < filterDomains.size(); i++) {
			for (int j = 0; j < allDomains.size();) {
				if (allDomains.get(j).equals(filterDomains.get(i))) {
					allDomains.remove(j);
					break;
				}
				j++;
			}
		}
		return allDomains;
	}

	public static long getStartTime(String dayType, long end) {
		return getStartTime(dayType, end, true);
	}

	public static long getStartTime(String dayType, long end, boolean forJrobin) {
		long start = 0;
		if (forJrobin) {
			System.out.println("for jrobin " + dayType);
			if ("hour".equals(dayType)) {
				start = end - 3600;
			}
			if ("day".equals(dayType)) {
				start = end - 3600 * 24;
			}
			if ("week".equals(dayType)) {
				start = end - 3600 * 24 * 7;
			}
			if ("month".equals(dayType)) {
				start = end - 3600 * 24 * 30;
			}
		} else {

			System.out.println("not for jrobin " + dayType);
			if ("hour".equals(dayType)) {
				start = end - 3600 * 1000;
			}
			if ("day".equals(dayType)) {
				start = end - 24 * 3600 * 1000;
			}
			if ("week".equals(dayType)) {
				start = end - 3600 * 24 * 7 * 1000;
			}
			if ("month".equals(dayType)) {
				start = end - 3600 * 24 * 30 * 1000;
			}
		}
		return start;
	}

	public static boolean isNaN(String prefix, String dsName, String fileName,
			long end, long start) {
		RrdDb rrd;
		FetchRequest request;
		FetchData data;
		try {
			rrd = new RrdDb(prefix + "/" + fileName, true);
			request = rrd.createFetchRequest("AVERAGE", start, end);
			data = request.fetchData();
			if (!Double.isNaN(data.getAggregate(dsName, "AVERAGE"))) {
				// System.out.println("Not Double .NaN"+rrd.getPath()+data.getAggregate(dsName,
				// "AVERAGE")+"-----MAX-----"+data.getAggregate(dsName,
				// "MAX")+"" +
				// "From "+end +"-->"+start);
				return false;
			}

		} catch (Exception e) {
			e.printStackTrace();
		}
		return true;
	}

	public List<String> getFileList(String dir, int index) {
		File file = new File(dir);
		for (File f : file.listFiles()) {
			listFile(f, index);
		}
		return files;

	}

	public void listFile(File file, int index) {
		if (file.isDirectory()) {
			for (File f : file.listFiles()) {
				listFile(f, index);
			}
		} else {
			files.add(file.getAbsolutePath().substring(index));
		}
	}

	/**
	 * 返回time stamp 对象
	 * 
	 * @param year
	 * @param month
	 * @param day
	 * @return
	 */
	public static Timestamp getTimeStamp(String year, String month, String day) {
		if (validateStr(year) || validateStr(month) || validateStr(day)) {
			return getTimeStamp(new Date());
		} else {
			Calendar calendar = Calendar.getInstance();
			calendar.set(strToInt(year), strToInt(month) - 1, strToInt(day), 0,
					0, 0);
			return new Timestamp(calendar.getTimeInMillis());
		}
	}

	public static Timestamp getTimeStamp(int year, int month, int day) {
		return getTimeStamp(year, month, day, 0, 0, 0);
	}

	public static Timestamp getTimeStamp(int year, int month, int day,
			int hour, int minute, int second) {
		Calendar calendar = Calendar.getInstance();
		calendar.set(year, month - 1, day, hour, minute, second);
		calendar.set(Calendar.MILLISECOND, 0);
		return new Timestamp(calendar.getTimeInMillis());
	}

	public static Timestamp getTimeStamp(TimeParam timeParam, boolean isbegin) {
		if (isbegin) {
			return getTimeStamp(timeParam.getByear(), timeParam.getBmonth(),
					timeParam.getBday(), timeParam.getBhour(), timeParam
							.getBminute(), timeParam.getBsecond());
		} else {
			return getTimeStamp(timeParam.getEyear(), timeParam.getEmonth(),
					timeParam.getEday(), timeParam.getEhour(), timeParam
							.getEminute(), timeParam.getEsecond());
		}
	}

	public static Timestamp getTimeStamp(String year, String month, String day,
			String hour, String minute, String second) {
		System.out.println("year:" + year + ":month:" + month + ":day:" + day);
		if (validateStr(year) || validateStr(month) || validateStr(day)
				|| validateStr(hour) || validateStr(minute)
				|| validateStr(second)) {
			return getTimeStamp(new Date());
		} else {
			Calendar calendar = Calendar.getInstance();
			calendar.set(strToInt(year), strToInt(month) - 1, strToInt(day),
					strToInt(hour), strToInt(minute), strToInt(second));
			return new Timestamp(calendar.getTimeInMillis());
		}
	}

	/**
	 * 将String 转化为int
	 * 
	 * @param str
	 * @return
	 */
	public static int strToInt(String str) {
		return Integer.parseInt(str.trim());
	}

	public static long strToLong(String str) {
		return Long.parseLong(str.trim());
	}

	/**
	 * 将String 转化为double
	 * 
	 * @param str
	 * @return
	 */
	public static double strToDouble(String str) {
		return Double.parseDouble(str.trim());
	}

	/**
	 * 根据date 返回timestamp 。
	 * 
	 * @param date
	 * @return
	 */

	public static Timestamp getTimeStamp(Date date) {
		Calendar ca = Calendar.getInstance();
		ca.setTime(date);
		Timestamp time = new Timestamp(ca.getTimeInMillis());
		return time;
	}

	/**
	 * 验证是否是evil code ,存在evil code 返回true
	 * 
	 * @param str
	 * @return
	 */
	// evil code 返回true
	public static boolean validateStr(String str) {
		// 判断是否为空串或则为Null
		if (null == str || str.trim().equals("")) {
			return true;
		}
		// 是否都是合法的数字
		char[] charArr = str.trim().toCharArray();
		for (char c : charArr) {
			if (c > '9' || c < '0') {
				return true;
			}
		}
		return false;
	}
}
