package com.xiaonei.xnmonitor.controllers.http;

import java.sql.Timestamp;
import java.util.LinkedList;
import java.util.List;

import javax.servlet.http.HttpServletRequest;

import org.apache.commons.lang.StringUtils;

import net.paoding.rose.ar.Ar;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.REST;
import net.paoding.rose.web.var.Model;

import com.xiaonei.xnmonitor.controllers.imgstreamHttpErrController;
import com.xiaonei.xnmonitor.dto.ErrInfo;
import com.xiaonei.xnmonitor.dto.GroupedDomains;
import com.xiaonei.xnmonitor.dto.RenRenGroup;
import com.xiaonei.xnmonitor.dto.TimeParam;
import com.xiaonei.xnmonitor.utils.ObjectPack;
import com.xiaonei.xnmonitor.utils.UtilTools;

@REST(get = "showList")
public class ErrorController {

	// static {
	// ApplicationContext arConfig = new ClassPathXmlApplicationContext(
	// "applicationContext-ar.xml");
	// Ar.setSessionFactory((SessionFactory)
	// arConfig.getBean("sessionFactory"));
	// }

	public String showList(@Param("time") String time) {
		return "@${time}, 你好";
	}
	public String httpErr(Invocation inv, @Param("dayType") String dayType) {
		System.out.println(dayType + "----------dayType");
		if (dayType == null || "".equals(dayType)) {
			dayType = "day";
			inv.addModel("dayType", dayType);
		}
		List<RenRenGroup> groups = UtilTools.listRenRenGroups();
		inv.addModel("groups", groups);
		return "error-httpErr";

	}

	public String httpErrSingle(Invocation inv,
			@Param("groupName") String groupName,
			@Param("dayType") String dayType) throws Exception {
		System.out.println(dayType + "----DayTYpe----");
		if (StringUtils.isEmpty(groupName)) {
			groupName = "all";
		}
		if (StringUtils.isBlank(dayType)) {
			dayType = "day";
		}

		List<String> domainsList = UtilTools.listGroupedDomains(groupName);
		List<GroupedDomains> groupedDomains = new LinkedList<GroupedDomains>();
		for (String domain : domainsList) {
			List<ErrInfo> errInfo404 = imgstreamHttpErrController
					.getErrInfoList(dayType, "404", "single", groupName, domain);
			List<ErrInfo> errInfo500 = imgstreamHttpErrController
					.getErrInfoList(dayType, "500", "single", groupName, domain);
			if (errInfo404.size() > 2 && errInfo500.size() > 2) {
				GroupedDomains groupedDomain = new GroupedDomains();
				groupedDomain.setDomain(domain);
				groupedDomain.getErrTypes().add("404");
				groupedDomain.getErrTypes().add("500");
				groupedDomains.add(groupedDomain);
			}
			if (errInfo404.size() > 2 && errInfo500.size() < 3) {
				GroupedDomains groupedDomain = new GroupedDomains();
				groupedDomain.setDomain(domain);
				groupedDomain.getErrTypes().add("404");
				groupedDomains.add(groupedDomain);

			}
			if (errInfo404.size() < 3 && errInfo500.size() > 2) {
				GroupedDomains groupedDomain = new GroupedDomains();
				groupedDomain.setDomain(domain);
				groupedDomain.getErrTypes().add("500");
				groupedDomains.add(groupedDomain);
			}

		}
		List<RenRenGroup> renrenGroups = UtilTools.listRenRenGroups();
		inv.addModel("renrenGroups", renrenGroups);
		inv.addModel("groupedDomains", groupedDomains);
		return "error-httpErrSingle";
	}

	public String errInfo(Invocation inv, TimeParam timeParam,
			@Param("errTypes") String errTypes,
			@Param("domains") String domains, Model model) {
		HttpServletRequest request = inv.getRequest();
		if (request.getParameter("byear") == null
				|| "".equals(request.getParameter("byear"))) {
			timeParam = new TimeParam(1);
			model.add("timeParam", timeParam);
		}

		Timestamp begin = UtilTools.getTimeStamp(timeParam.getByear(),
				timeParam.getBmonth(), timeParam.getBday(), timeParam
						.getBhour(), timeParam.getBminute(), timeParam
						.getBsecond());
		Timestamp end = UtilTools.getTimeStamp(timeParam.getEyear(), timeParam
				.getEmonth(), timeParam.getEday(), timeParam.getEhour(),
				timeParam.getEminute(), timeParam.getEsecond());
		// System.out.println("at last bday:"+timeParam.getBday());
		// System.out.println(begin+"begin:end"+end)

		// System.out.println("In ErrInfo begin:" + begin + "--end--:" + end);
		StringBuilder sql = new StringBuilder();
		sql
				.append("select distinct err_server ,err_type,pre_url,cur_url,count(id) as total from http_error where happen_time between  '"
						+ begin + "' and '" + end + "' and ");
		String[] errTypeArr = null;
		if (null != errTypes && !"".equals(errTypes)) {
			errTypeArr = errTypes.trim().split(",");
		}
		String[] domainArr = null;
		if (null != domains && !"".equals(domains)) {
			domainArr = domains.trim().split(",");
		}
		if (null == errTypeArr || errTypeArr.length == 0 || null == domainArr
				|| domainArr.length == 0) {
			System.out
					.println((null == errTypeArr) + ":" + (null == domainArr));
			return "error-errInfo";
		}
		if (null != errTypes && errTypes.length() != 0) {
			sql.append(" (");
			for (String str : errTypeArr) {
				sql.append(" err_type='" + str.trim() + "' or ");
			}
			sql.delete(sql.lastIndexOf("or"), sql.length());
			sql.append(") and ");
		}
		if (null != domainArr && domainArr.length != 0) {
			for (String str : domainArr) {
				sql.append(" cur_domain ='" + str.trim() + "' or ");
			}
			sql.delete(sql.lastIndexOf("or"), sql.length());
		}
		sql.append("group by cur_url,pre_url,err_server order by total desc ");
		System.out.println(sql.toString());
		List<Object[]> errInfoObject = Ar.sql(sql.toString());
		List<ErrInfo> errInfoList = ObjectPack.getErrInfoList(errInfoObject);
		model.add("errInfoList", errInfoList);
		return "error-errInfo";
	}

	public String errInfoFullUrl(TimeParam timeParam,
			@Param("errType") String errType, @Param("preUrl") String preUrl,
			@Param("curUrl") String curUrl,
			@Param("errServer") String errServer, Model model) {
		Timestamp begin = UtilTools.getTimeStamp(timeParam.getByear(),
				timeParam.getBmonth(), timeParam.getBday(), timeParam
						.getBhour(), timeParam.getBminute(), timeParam
						.getBsecond());
		Timestamp end = UtilTools.getTimeStamp(timeParam.getEyear(), timeParam
				.getEmonth(), timeParam.getEday(), timeParam.getEhour(),
				timeParam.getEminute(), timeParam.getEsecond());
		StringBuilder sqlsb = new StringBuilder();
		sqlsb
				.append("select happen_time,pre_ful_url,cur_ful_url from http_error where err_type= '"
						+ errType.trim()
						+ "' and happen_time between '"
						+ begin
						+ "' and '"
						+ end
						+ "'  and pre_url='"
						+ preUrl
						+ "' and cur_url='"
						+ curUrl
						+ "' and err_server='"
						+ errServer.trim() + "' order by happen_time desc ");
		// System.out.println("SQLSB"+sqlsb.toString());
		List<Object[]> errInfoObject = Ar.sql(sqlsb.toString());
		List<ErrInfo> errInfoFullUrlList = ObjectPack
				.getErrInfoFullUrlList(errInfoObject);
		model.add("errInfoFullUrlList", errInfoFullUrlList);
		return "error-errInfoFullUrl";
	}

}
