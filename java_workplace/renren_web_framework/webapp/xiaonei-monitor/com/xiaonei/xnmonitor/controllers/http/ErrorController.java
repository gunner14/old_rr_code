package com.xiaonei.xnmonitor.controllers.http;

import java.sql.Timestamp;
import java.util.List;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.ar.Ar;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.REST;
import net.paoding.rose.web.var.Model;

import com.xiaonei.xnmonitor.dto.ErrInfo;
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

	/**
	 * public void httpErr(Model model, @Param("dayType") String dayType,
	 * 
	 * @Param("dsName") String dsName) { final File file = new
	 *                  File(Global.JROBIN_RESULT_DIR + "httperrlog/"); if (null
	 *                  == dayType || "".equals(dayType)) { dayType = "day"; }
	 *                  if (null == dsName || "".equals(dsName)) { dsName =
	 *                  "HTTPERR_LOG"; model.add("dsName", dsName); } final long
	 *                  end = Util.getTime(); final long start =
	 *                  UtilTools.getStartTime(dayType, end); String[] rrdFiles
	 *                  = file.list(new FilenameFilter() { public boolean
	 *                  accept(File dir, String name) { if
	 *                  (name.endsWith(".rrd") && !name.contains("127.0.0.1")) {
	 *                  return !UtilTools.isNaN(file.getAbsolutePath(),
	 *                  Global.DSNAME_HTTPERR, name, end, start); // return
	 *                  true; } else { return false; } } });
	 *                  Arrays.sort(rrdFiles, new
	 *                  SortFile(file.getAbsolutePath(), end,
	 *                  UtilTools.getStartTime( dayType, end),
	 *                  Global.DSNAME_HTTPERR)); model.add("rrdFiles",
	 *                  rrdFiles); model.add("dayType", dayType); }
	 */
	public String httpErr(Invocation inv, Model model,
			@Param("dayType") String dayType) {
		System.out.println(dayType + "----------dayType");
		if (dayType == null || "".equals(dayType)) {
			dayType = "day";
			model.add("dayType", dayType);
		}
		return "error-httpErr";

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
		System.out.println(errTypes + ":" + domains);
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
