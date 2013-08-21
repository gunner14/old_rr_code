package com.xiaonei.xnmonitor.controllers.im;

import java.io.File;
import java.io.FilenameFilter;
import java.sql.Timestamp;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;

import javax.servlet.http.HttpServletRequest;

import kce.adapter.ipseek.IpSeekAdapter;

import net.paoding.rose.ar.Ar;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.ReqMapping;
import net.paoding.rose.web.var.Model;

import org.hibernate.SessionFactory;
import org.jrobin.core.Util;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;

import com.xiaonei.xnmonitor.dto.ErrInfo;
import com.xiaonei.xnmonitor.dto.IMInfo;
import com.xiaonei.xnmonitor.dto.TimeParam;
import com.xiaonei.xnmonitor.utils.Global;
import com.xiaonei.xnmonitor.utils.MyAr;
import com.xiaonei.xnmonitor.utils.ObjectPack;
import com.xiaonei.xnmonitor.utils.SortFile;
import com.xiaonei.xnmonitor.utils.UtilTools;

public class ImController {

	public String imInfoList(TimeParam timeParam,
			@Param("urlOrDns") String urlOrDns, Model model,
			@Param("hostOrUrl") String hostOrUrl) {
		Timestamp begin = UtilTools.getTimeStamp(timeParam.getByear(),
				timeParam.getBmonth(), timeParam.getBday(), timeParam
						.getBhour(), timeParam.getBminute(), timeParam
						.getBsecond());
		Timestamp end = UtilTools.getTimeStamp(timeParam.getEyear(), timeParam
				.getEmonth(), timeParam.getEday(), timeParam.getEhour(),
				timeParam.getEminute(), timeParam.getEsecond());
		if (null == urlOrDns || "".equals(urlOrDns)) {
			urlOrDns = "dns";
		}
		StringBuilder sql = new StringBuilder();
		sql
				.append("select distinct url,type,avg(cost_time) as average_cost ,count(id) as total from im where type='"
						+ urlOrDns
						+ "' and happen_time between '"
						+ begin
						+ "' and '"
						+ end
						+ "' group by url,type order by average_cost desc");
		// System.out.println(sql.toString());
		List<Object[]> imInfoObject = Ar.sql(sql.toString());
		List<IMInfo> imInfoList = ObjectPack.getImInfoList(imInfoObject);
		model.add("imInfoList", imInfoList);
		return "im-imInfoList";
		// model.add("errInfoList", errInfoList);
	}

	public String imInfoDetailList(TimeParam timeParam,
			@Param("type") String type, @Param("hostOrUrl") String hostOrUrl,
			Model model) {
		Timestamp begin = UtilTools.getTimeStamp(timeParam.getByear(),
				timeParam.getBmonth(), timeParam.getBday(), timeParam
						.getBhour(), timeParam.getBminute(), timeParam
						.getBsecond());
		Timestamp end = UtilTools.getTimeStamp(timeParam.getEyear(), timeParam
				.getEmonth(), timeParam.getEday(), timeParam.getEhour(),
				timeParam.getEminute(), timeParam.getEsecond());

		StringBuilder sql = new StringBuilder();
		sql
				.append("select type,cost_time,url,happen_time,IP from im where type='"
						+ type
						+ "' and happen_time between '"
						+ begin
						+ "' and '"
						+ end
						+ "' and url='"
						+ hostOrUrl
						+ "' order by happen_time desc ");
		System.out.println(sql.toString());
		List<Object[]> imInfoObject = Ar.sql(sql.toString());
		List<IMInfo> imInfoDetailList = ObjectPack
				.getImInfoDetailList(imInfoObject);
		model.add("imInfoList", imInfoDetailList);
		return "im-imInfoDetailList";
		// model.add("errInfoList", errInfoList);
	}
	public String imInfoAnalysis(Invocation inv, TimeParam timeParam,
			@Param("type") String type) {
		if (null == type || "".equals(type)) {
			type = "dns";
		}
		Timestamp begin = UtilTools.getTimeStamp(timeParam.getByear(),
				timeParam.getBmonth(), timeParam.getBday(), timeParam
						.getBhour(), timeParam.getBminute(), timeParam
						.getBsecond());
		Timestamp end = UtilTools.getTimeStamp(timeParam.getEyear(), timeParam
				.getEmonth(), timeParam.getEday(), timeParam.getEhour(),
				timeParam.getEminute(), timeParam.getEsecond());
		// get the most slow remoteIp.
		String sql = "select distinct remote_host,remote_host from im where happen_time between "
				+ "? and ? and type=? and remote_host !='NULL' order by cost_time desc limit 100";
		List<Object[]> mostSlow = Ar.sql(sql, begin, end, type);
		List<IMInfo> mostSlowIMInfoList = new LinkedList<IMInfo>();
		for (Object[] result : mostSlow) {
			IMInfo iminfo = new IMInfo();
			iminfo.setRemoteHost(result[0].toString());
			iminfo.setRemoteLocation(IpSeekAdapter.getInstance().getAddress(
					iminfo.getRemoteHost().trim()));
			mostSlowIMInfoList.add(iminfo);
		}

		inv.addModel("mostSlowIMInfoList", mostSlowIMInfoList);
		return "im-imInfoAnalysis";

	}

	public String imInfoAnalysisDetail(Invocation inv, TimeParam timeParam,
			@Param("type") String type, @Param("remoteHost") String remoteHost) {
		Timestamp begin = UtilTools.getTimeStamp(timeParam.getByear(),
				timeParam.getBmonth(), timeParam.getBday(), timeParam
						.getBhour(), timeParam.getBminute(), timeParam
						.getBsecond());
		Timestamp end = UtilTools.getTimeStamp(timeParam.getEyear(), timeParam
				.getEmonth(), timeParam.getEday(), timeParam.getEhour(),
				timeParam.getEminute(), timeParam.getEsecond());
		// get the most slow remoteIp.
		String sql = "select remote_host,url,ip,cost_time ,type,happen_time from im where happen_time between ? and ? and type=? and remote_host =? order by id";
		List<Object[]> mostSlow = Ar.sql(sql, begin, end, type, remoteHost);
		List<IMInfo> mostSlowIMInfoDetailList = new LinkedList<IMInfo>();
		inv.addModel("province", IpSeekAdapter.getInstance().getProvice(
				remoteHost));
		for (Object[] result : mostSlow) {
			IMInfo iminfo = new IMInfo();
			iminfo.setRemoteHost(result[0].toString());
			iminfo.setRemoteLocation(IpSeekAdapter.getInstance().getAddress(
					iminfo.getRemoteHost().trim()));
			iminfo.setUrlOrHost(result[1].toString());
			iminfo.setIP(result[2].toString());
			iminfo.setLocation(IpSeekAdapter.getInstance().getAddress(
					iminfo.getIP()));
			iminfo.setCostTime(UtilTools.strToLong(result[3].toString()));
			iminfo.setType(result[4].toString());
			iminfo.setHappenTime(result[5].toString());
			mostSlowIMInfoDetailList.add(iminfo);
		}
		inv.addModel("mostSlowIMInfoDetailList", mostSlowIMInfoDetailList);
		inv.addModel("timeParam", timeParam);
		// TODO Analysis
		return "im-imInfoAnalysisDetail";
	}

	public String urlAndLocation(Invocation inv, TimeParam timeParam,
			@Param("url") String url) {
		Timestamp begin = UtilTools.getTimeStamp(timeParam.getByear(),
				timeParam.getBmonth(), timeParam.getBday(), timeParam
						.getBhour(), timeParam.getBminute(), timeParam
						.getBsecond());
		Timestamp end = UtilTools.getTimeStamp(timeParam.getEyear(), timeParam
				.getEmonth(), timeParam.getEday(), timeParam.getEhour(),
				timeParam.getEminute(), timeParam.getEsecond());
		String sql ="select remote_host,ip,cost_time ,happen_time,type from im  where happen_time between ? and ? and url = ? and cost_time >100 order by cost_time desc  ";
		List<Object[]> slowResult = Ar.sql(sql, begin, end, url);
		List<IMInfo> mostSlowIMInfoDetailList = new LinkedList<IMInfo>();
		for (Object[] result : slowResult) {
			IMInfo iminfo = new IMInfo();
			iminfo.setRemoteHost(result[0].toString());
			iminfo.setRemoteLocation(IpSeekAdapter.getInstance().getAddress(
					iminfo.getRemoteHost().trim()));
			iminfo.setIP(result[1].toString());
			iminfo.setCostTime(UtilTools.strToLong(result[2].toString()));
			iminfo.setHappenTime(result[3].toString());
			iminfo.setType(result[4].toString());
			iminfo.setLocation(IpSeekAdapter.getInstance().getAddress(
					iminfo.getIP()));
			mostSlowIMInfoDetailList.add(iminfo);
		}
		inv.addModel("mostSlowIMInfoDetailList", mostSlowIMInfoDetailList);
		return "im-urlAndLocation";
	}
//	// @ReqMapping(path="@")
//	public String testReferer(Invocation inv) {
//		HttpServletRequest request = inv.getRequest();
//		System.out.println("referer" + request.getHeader("Referer"));
//		return "im-testReferer";
//
//	}
}
