package com.xiaonei.platform.core.utility;

/**
 * 散表用工具类
 * 
 * @author Admin
 * 
 */
public class PartitionTableUtils {
	/**
	 * 根据id散表，得到散表的表名
	 * 
	 * @param tablePrefix
	 *            表的前缀
	 * @param id
	 *            根据Id散表
	 * @param tableCount
	 *            散表的数量
	 * @return
	 */
	public static String getPartitionTableNameByUserId(String tablePrefix,
			int id, int tableCount) {
		// return tablePrefix.substring(0,tablePrefix.length()-1);
		return tablePrefix + getTableSuffix(id , tableCount);
	}

	/**
	 * 得到散表后表名的后缀，如散表blog_64，则返回64
	 * @param id
	 * @param tableCount
	 * @return
	 */
	public static int getTableSuffix(int id, int tableCount) {
		return id % tableCount;
	}
}