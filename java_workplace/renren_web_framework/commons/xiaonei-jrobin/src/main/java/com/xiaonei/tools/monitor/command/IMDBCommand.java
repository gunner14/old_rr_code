package com.xiaonei.tools.monitor.command;

import java.io.File;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.LinkedList;
import java.util.Random;

import com.xiaonei.tools.monitor.db.Connect;
import com.xiaonei.tools.monitor.entity.HttpErrEntity;
import com.xiaonei.tools.monitor.entity.IMEntity;

public class IMDBCommand {

	Random random = new Random();

	public static final SimpleDateFormat format = new SimpleDateFormat(
			"yyyy-MM-dd HH:mm:ss");

	private final String SQL_PREFIX = "insert into im (happen_time,type,cost_time,url,IP,remote_host) values ";

	private final int IMLOGARRLEN_URL = 3;

	private final int IMLOGARRLEN_DNS = 4;

	private final String SEPRATOR = " ";

	public void excute(IMEntity imEntity, LinkedList<IMEntity> imEntityList)
			throws SQLException {

		// HttpErrEntity httpErrEntity = getHttpErrEntity(items);
		imEntityList.add(imEntity);
		if (imEntityList.size() < 10) {

		} else {
			saveToDB(imEntityList);
			imEntityList.clear();
		}
	}

	public void saveToDB(LinkedList<IMEntity> imEntityList) throws SQLException {
		String sql = getSql(imEntityList);
//		System.out.println(sql);
		// System.out.println("aaa" + sql);
		Connect.insert(sql);
	}

	// public String[] getRecordArr(String record) {
	// return record.split(" ");
	//
	// }

	public IMEntity getIMEntity(String[] items, String line, File file,
			String remoteHost) {
		if (!validateLog(line)) {
			// System.out.println("validatelog err" + items[4] + "-----" +
			// items[5]);
			return null;
		}
		IMEntity imEntity = new IMEntity();
		imEntity.setHappenTime(getHappenTime(file));
		imEntity.setCostTime(getCostTime(items));
		imEntity.setUrlOrHost(getUrl(line));
		imEntity.setType(getType(line));
		imEntity.setIP(getIP(items));
		imEntity.setRemoteHost(remoteHost);
		return imEntity;
	}

	public String getType(String line) {
		if (line.startsWith("dns")) {
			return "dns";
		} else if (line.startsWith("url")) {
			return "url";
		}
		return null;
	}

	public String getHappenTime(File file) {
		String fileName = file.getName();
		String timeStr = fileName.substring(0, fileName.indexOf(".log"));
		long time = (long) strToDouble(timeStr);
		return format.format(new Date(time));
	}

	public String getUrl(String url) {
		try {

			if (url.startsWith("url")) {
				if (url.indexOf("?") == -1) {
					return url.substring(url.indexOf("url:") + 11).trim();
				} else {
					return url.substring(url.indexOf("url:") + 11,
							url.indexOf("?")).trim();
				}
			} else if (url.startsWith("dns")) {
				return url.substring(url.indexOf("host:") + 5).trim();
			} else {
				return null;
			}
		} catch (Exception e) {
			// System.out.println("cutURL:" + url + ":" +
			// file.getAbsolutePath());
			return null;
		}
	}

	public String getIP(String[] items) {
		// dns time:1005 ip:220.181.92.38 host:home.xiaonei.com
		if (!items[0].startsWith("dns")) {
			return "undefined";
		}
		String IP;
		IP = items[2].substring(items[2].indexOf("ip:") + 3).trim();
		return IP;
	}

	/**
	 * check if has evil code .
	 * 
	 * @param arr
	 * @return
	 */

	public String getSql(LinkedList<IMEntity> imEntityList) {
		StringBuilder sb = new StringBuilder();
		sb.append(SQL_PREFIX);
		for (IMEntity imEntity : imEntityList) {
			// (happen_time,type,cost_time,url) values ";
			sb.append("('");
			sb.append(imEntity.getHappenTime());
			sb.append("','");
			sb.append(imEntity.getType());
			sb.append("','");
			sb.append(imEntity.getCostTime());
			sb.append("','");
			sb.append(imEntity.getUrlOrHost());
			sb.append("','");
			sb.append(imEntity.getIP());
			sb.append("','");
			sb.append(imEntity.getRemoteHost());
			sb.append("')");
			sb.append(",");
		}
		return sb.substring(0, sb.length() - 1);
	}

	private long strToLong(String longStr) {
		return Long.parseLong(longStr);
	}

	private double strToDouble(String doubleStr) {
		return Double.parseDouble(doubleStr);
	}

	private long getCostTime(String[] items) {
		try {
			String timeStr = items[1].trim();
			String doubleStr = timeStr.substring(timeStr.indexOf("time:") + 5);
			return strToLong(doubleStr);
		} catch (Exception e) {
			// System.out.println(file.getAbsolutePath());
			return -1;
		}
	}

	private boolean validateLog(String line) {
		if (!line.startsWith("dns") && !line.startsWith("url")) {
			return false;
		}
		String[] items = line.split(SEPRATOR);
		if (line.startsWith("dns") && items.length != IMLOGARRLEN_DNS)
			return false;
		if (line.startsWith("url") && items.length != IMLOGARRLEN_URL)
			return false;
		if (getCostTime(items) == -1)
			return false;
		String urlOrDns = getUrl(line);
		if ("dns".equals(items[0].trim()) && null == urlOrDns) {
			return false;
		}
		if ("url".equals(items[0].trim()) && null == urlOrDns) {
			return false;
		}

		if (urlOrDns == null || hasEvilCode(urlOrDns.toCharArray())) {
			return false;
		}
		if (urlOrDns.contains("%") || urlOrDns.contains("'"))
			return false;
		if (!urlOrDns.contains("renren") && !urlOrDns.contains("xnimg")
				&& !urlOrDns.contains("xiaonei"))
			return false;
		return true;
	}

	public static boolean hasEvilCode(char[] arr) {
		for (char c : arr) {
			if (c > 40869) {
				return true;
			}
		}
		return false;
	}

	public static String getDBLatestTime() throws SQLException {
		String sql = "select happen_time from im order by happen_time desc limit 1";
		ResultSet set = Connect.querySet(sql);
		while (set.next()) {
			return set.getString("happen_time");
		}
		return null;
	}

}
