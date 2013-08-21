package com.xiaonei.xnmonitor.controllers;

import java.awt.Color;
import java.io.OutputStream;
import java.sql.Timestamp;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.LinkedList;
import java.util.List;

import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.ar.Ar;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;

import org.jrobin.core.Util;
import org.jrobin.graph.LinearInterpolator;
import org.jrobin.graph.RrdGraph;
import org.jrobin.graph.RrdGraphDef;

import com.xiaonei.xnmonitor.dto.ErrInfo;
import com.xiaonei.xnmonitor.dto.TimeParam;
import com.xiaonei.xnmonitor.utils.ObjectPack;
import com.xiaonei.xnmonitor.utils.UtilTools;

public class imgstreamHttpErrController {

	public static SimpleDateFormat dateFormat = new SimpleDateFormat(
			"yyyy-MM-dd HH:mm:ss");

	/**
	 * @param inv
	 * @param timeParam
	 * @param dayType
	 *            月，周，日，小时
	 * @param errType
	 *            404 500
	 * @param type
	 *            single ,total .
	 * @param groupName
	 *            all,other,ugc,sns etc.
	 * @param domain
	 * 
	 * @throws Exception
	 */

	public Object getImg(Invocation inv, TimeParam timeParam,
			@Param("dayType") String dayType, @Param("errType") String errType,
			@Param("groupName") String groupName, @Param("type") String type,
			@Param("domain") String domain, @Param("bigPic") String bigPic)
			throws Exception {
		if (null == dayType || "".endsWith(dayType)) {
			dayType = "day";
		}
		final long end = new Date().getTime();
		final long begin = UtilTools.getStartTime(dayType, end, false);
		String beginStr = dateFormat.format(new Date(begin));
		String endStr = dateFormat.format(new Date(end));
		List<ErrInfo> errInfoList = getErrInfoList(dayType, errType, type,
				groupName, domain);
		Date[] dateArr = getTimeArr(errInfoList);
		double[] valueArr = getValueArr(errInfoList);
		LinearInterpolator ErrCounter = new LinearInterpolator(dateArr,
				valueArr);
		RrdGraphDef gDef = new RrdGraphDef(begin / 1000, end / 1000);
		gDef.setVerticalLabel("Xiaonei Web Framework Team");
		gDef.setShowSignature(false);
		gDef.setTitle("" + type + errType + ":" + beginStr + "~" + endStr);
		gDef.datasource("ErrCounter", ErrCounter);
		gDef.line("ErrCounter", Color.RED, "ErrCounter@r");
		RrdGraph graph = new RrdGraph(gDef);

		HttpServletResponse response = inv.getResponse();
		response.setContentType("image/png");
		// OutputStream outputStream = response.getOutputStream();
		// outputStream.write(graph.getPNGBytes(1000, 520));
		// outputStream.flush();
		// outputStream.close();
		if ("small".equals(bigPic)) {
			return graph.getPNGBytes(500, 240);
		} else {
			return graph.getPNGBytes(1000, 520);
		}
	}

	public static int getStep(String dayType) throws Exception {
		if (dayType.equals("hour")) {
			return 30;// 半15s统计一次
		} else if (dayType.equals("day")) {
			return 5 * 12;// 每分钟统计一次

		} else if (dayType.equals("week")) {
			return 5 * 12 * 7;// 每7分钟统计一次
		} else if (dayType.equals("month")) {
			return 5 * 12 * 30;// 每一个小时统计一次。
		}
		throw new Exception("NO dayType can catch ");
	}

	public static int getSubStringLength(String dayType) throws Exception {
		if (dayType.equals("hour")) {
			return 8;// 梅5秒统计一次
		} else if (dayType.equals("day")) {
			return 8;// 每分钟统计一次

		} else if (dayType.equals("week")) {
			return 7;// 每7分钟统计一次
		} else if (dayType.equals("month")) {
			return 6;// 每一个小时统计一次。
		}
		throw new Exception("NO dayType can catch ");
	}

	public Date[] getTimeArr(List<ErrInfo> errInfoList) throws Exception {
		Date[] timeArr = new Date[errInfoList.size()];
		for (int i = 0; i < errInfoList.size(); i++) {
			timeArr[i] = errInfoList.get(i).getHappenTimeDate();
		}
		return timeArr;
	}

	public double[] getValueArr(List<ErrInfo> errInfoList) {
		double[] valueArr = new double[errInfoList.size()];
		for (int i = 0; i < errInfoList.size(); i++) {
			valueArr[i] = errInfoList.get(i).getTotal();
		}
		return valueArr;
	}

	/**
	 * 拼装Sql
	 * 
	 * @param type
	 * @param groupName
	 * @param domain
	 * @return
	 */
	public static String getSql(String type, String groupName, String domain) {
		String sql = "";
		if ("total".equals(type) || null == type || "".equals(type)) {
			List<String> domains = UtilTools.listGroupedDomains(groupName);
			StringBuilder domainBuilder = new StringBuilder();
			domainBuilder.append(" and (");
			System.out.println(groupName + "::::" + domains.size());

			for (String domainStr : domains) {
				domainBuilder.append(" cur_domain ='" + domainStr + "' or ");
			}
			domainBuilder = domainBuilder.delete(domainBuilder
					.lastIndexOf("or"), domainBuilder.length() - 1);
			domainBuilder.append(")");
			sql = "select distinct  cast(substring(unix_timestamp(happen_time)/? ,1,? )as char) "
					+ "as unixtime, count(1) as total from http_error where err_type= ?  and "
					+ " happen_time between  ? and ?  "
					+ domainBuilder.toString()
					+ " group by unixtime  order by unixtime";

		} else if ("single".equals(type)) {
			sql = "select distinct  cast(substring(unix_timestamp(happen_time)/? ,1,? )as char) "
					+ "as unixtime, count(1) as total from http_error where err_type= ?  and "
					+ " happen_time between  ? and ?  and cur_domain= '"
					+ domain + "' group by unixtime  order by unixtime";
			// Elseif语句需要加强。
		}
		return sql;
	}

	public static List<ErrInfo> getErrInfoList(String dayType, String errType,
			String type, String groupName, String domain) throws Exception {
		dayType = dayType.trim();
		final long end = new Date().getTime();
		final long begin = UtilTools.getStartTime(dayType, end, false);
		String beginStr = dateFormat.format(new Date(begin));
		String endStr = dateFormat.format(new Date(end));
		int step = getStep(dayType);
		int subStringLen = getSubStringLength(dayType);
		String sql = getSql(type, groupName, domain);
		List<Object[]> result = Ar.sql(sql, step, subStringLen, errType,
				beginStr, endStr);
		List<ErrInfo> errInfoList = ObjectPack.getDateForHttpErrPic(result,
				step);
		return errInfoList;
	}
}
