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
	 * @param errKey
	 *            kaixin ,renren .
	 * @throws Exception
	 */

	public Object getImg(Invocation inv, TimeParam timeParam,
			@Param("dayType") String dayType, @Param("errType") String errType,
			@Param("errKey") String errKey) throws Exception {
		if (null == dayType || "".endsWith(dayType)) {
			dayType = "day";
		}
		// Timestamp begin = UtilTools.getTimeStamp(timeParam.getByear(),
		// timeParam.getBmonth(),
		// timeParam.getBday(), timeParam.getBhour(), timeParam.getBminute(),
		// timeParam
		// .getBsecond());
		// Timestamp end = UtilTools.getTimeStamp(timeParam.getEyear(),
		// timeParam.getEmonth(),
		// timeParam.getEday(), timeParam.getEhour(), timeParam.getEminute(),
		// timeParam
		// .getEsecond());
		dayType = dayType.trim();
		final long end = new Date().getTime();
		final long begin = UtilTools.getStartTime(dayType, end, false);
		String beginStr = dateFormat.format(new Date(begin));
		String endStr = dateFormat.format(new Date(end));
		int step = getStep(dayType);
		int subStringLen = getSubStringLength(dayType);
		String sql = "select distinct  cast(substring(unix_timestamp(happen_time)/"
				+ step
				+ ",1,"
				+ subStringLen
				+ ")as char) as unixtime, count(1) as total from http_error where err_type='"
				+ errType
				+ "' and  happen_time between '"
				+ beginStr
				+ "' and '"
				+ endStr
				+ "'  and key_word='"
				+ errKey.trim()
				+ "' group by unixtime  order by unixtime;";
		System.out.println(sql + "IMGStreamHttpErr");
		List<Object[]> result = Ar.sql(sql);
		List<ErrInfo> errInfoList = ObjectPack.getDateForHttpErrPic(result,
				step);
		Date[] dateArr = getTimeArr(errInfoList);
		double[] valueArr = getValueArr(errInfoList);
		// for (int i = 0; i < valueArr.length; i++) {
		// System.out.println(dateArr[i] + "<--->" + valueArr[i]);
		// }
		LinearInterpolator ErrCounter = new LinearInterpolator(dateArr,
				valueArr);
		RrdGraphDef gDef = new RrdGraphDef(begin / 1000, end / 1000);
		gDef.setVerticalLabel("Xiaonei Web Framework Team");
		gDef.setShowSignature(false);
		gDef.setTitle("" + errKey + errType + ":" + beginStr + "~" + endStr);
		gDef.datasource("ErrCounter", ErrCounter);
		gDef.line("ErrCounter", Color.RED, "ErrCounter@r");
		RrdGraph graph = new RrdGraph(gDef);

		HttpServletResponse response = inv.getResponse();
		response.setContentType("image/png");
		// OutputStream outputStream = response.getOutputStream();
		// outputStream.write(graph.getPNGBytes(1000, 520));
		// outputStream.flush();
		// outputStream.close();
		return graph.getPNGBytes(1000, 520);
	}

	public int getStep(String dayType) throws Exception {
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

	public int getSubStringLength(String dayType) throws Exception {
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
}
