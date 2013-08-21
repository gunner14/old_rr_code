package com.xiaonei.xnmonitor.controllers.im;

import java.io.File;
import java.io.FilenameFilter;
import java.sql.Timestamp;
import java.util.Arrays;
import java.util.List;

import javax.servlet.http.HttpServletRequest;

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

//    static {
//        ApplicationContext arConfig = new ClassPathXmlApplicationContext(
//                "applicationContext-ar.xml");
//        Ar.setSessionFactory((SessionFactory) arConfig.getBean("sessionFactory"));
//    }

    public String  imInfo(Model model, @Param("dayType") String dayType, @Param("dsName") String dsName) {
        final File file = new File(Global.JROBIN_RESULT_DIR + "imlog/");
        if (null == dayType || "".equals(dayType)) {
            dayType = "day";
        }
        if (null == dsName || "".equals(dsName)) {
            dsName = Global.DSNAME_IM;
            model.add("dsName", dsName);
        }
        final long end = Util.getTime();
        final long start = UtilTools.getStartTime(dayType, end);
        String[] rrdFiles = file.list(new FilenameFilter() {

            public boolean accept(File dir, String name) {
                if (name.endsWith(".rrd") && !name.contains("127.0.0.1")) {
                    return !UtilTools.isNaN(file.getAbsolutePath(), Global.DSNAME_IM, name, end,
                            start);
                } else {
                    return false;
                }
            }
        });
        Arrays.sort(rrdFiles, new SortFile(file.getAbsolutePath(), end, UtilTools.getStartTime(
                dayType, end), Global.DSNAME_IM));

        model.add("rrdFiles", rrdFiles);
        model.add("dayType", dayType);
        return "im-imInfo";
    }

    public String imInfoSep(@Param("dayType") String dayType, @Param("logType") String logType,
            @Param("rrdFile") String rrdFile, @Param("childDir") String childDir,
            @Param("bigPic") String bigPic, @Param("dsName") String dsName) {
    	  return "im-imInfoSep";

    }

    public String imInfoDetail(Model model, @Param("dayType") String dayType,
            @Param("dsName") String dsName, @Param("childDir") String childDir) {
        String path = Global.JROBIN_RESULT_DIR + "imlog/";
        if (null == dsName || "".equals(dsName)) {
            dsName = Global.DSNAME_IM;
            model.add("dsName", dsName);
        }
        if (null != childDir && childDir.length() > 1) {
            if (childDir.startsWith("/")) {
                childDir = childDir.substring(1, childDir.length());
            }
            if (childDir.endsWith("/")) {
                childDir = childDir.substring(0, childDir.length() - 1);
            }
            if (childDir.endsWith(".rrd")) {
                childDir = childDir.substring(0, childDir.length() - 4);
            }
            path += childDir;
        }
        final String filePath = path;
        File file = new File(path);
        final long end = Util.getTime();
        final long start = UtilTools.getStartTime(dayType, end);
        UtilTools tools = new UtilTools();
        List<String> rrdFilesList = tools.getFileList(path, path.length() + 1);
        for (int i = 0; i < rrdFilesList.size();) {
            if (rrdFilesList.get(i).contains("127.0.0")) {
                rrdFilesList.remove(i);

            }
            if (UtilTools.isNaN(filePath, Global.DSNAME_IM, rrdFilesList.get(i), end, start)) {
                rrdFilesList.remove(i);
                continue;
            }
            i++;
        }
        String[] rrdFiles = rrdFilesList.toArray(new String[0]);
        Arrays.sort(rrdFiles, new SortFile(path, end, start, Global.DSNAME_IM));
        model.add("dsName", dsName);
        model.add("rrdFiles", rrdFiles);
        return "im-imInfoDetail";
    }

    public String imInfoDetailSep(@Param("dayType") String dayType, @Param("logType") String logType,
            @Param("rrdFile") String rrdFile, @Param("childDir") String childDir,
            @Param("bigPic") String bigPic, @Param("dsName") String dsName) {
    	  return "im-imInfoDetailSep";

    }

    public String imInfoList(TimeParam timeParam, @Param("urlOrDns") String urlOrDns, Model model,
            @Param("hostOrUrl") String hostOrUrl) {
        Timestamp begin = UtilTools.getTimeStamp(timeParam.getByear(), timeParam.getBmonth(),
                timeParam.getBday(), timeParam.getBhour(), timeParam.getBminute(), timeParam
                        .getBsecond());
        Timestamp end = UtilTools.getTimeStamp(timeParam.getEyear(), timeParam.getEmonth(),
                timeParam.getEday(), timeParam.getEhour(), timeParam.getEminute(), timeParam
                        .getEsecond());
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
        //        System.out.println(sql.toString());
        List<Object[]> imInfoObject = Ar.sql(sql.toString());
        List<IMInfo> imInfoList = ObjectPack.getImInfoList(imInfoObject);
        model.add("imInfoList", imInfoList);
        //        model.add("errInfoList", errInfoList);
        return "im-imInfoList";
    }

    public String imInfoDetailList(TimeParam timeParam, @Param("type") String type,
            @Param("hostOrUrl") String hostOrUrl, Model model) {
        Timestamp begin = UtilTools.getTimeStamp(timeParam.getByear(), timeParam.getBmonth(),
                timeParam.getBday(), timeParam.getBhour(), timeParam.getBminute(), timeParam
                        .getBsecond());
        Timestamp end = UtilTools.getTimeStamp(timeParam.getEyear(), timeParam.getEmonth(),
                timeParam.getEday(), timeParam.getEhour(), timeParam.getEminute(), timeParam
                        .getEsecond());

        StringBuilder sql = new StringBuilder();
        sql.append("select type,cost_time,url,happen_time,IP from im where type='" + type
                + "' and happen_time between '" + begin + "' and '" + end + "' and url='"
                + hostOrUrl + "' order by happen_time desc ");
        System.out.println(sql.toString());
        List<Object[]> imInfoObject = Ar.sql(sql.toString());
        List<IMInfo> imInfoDetailList = ObjectPack.getImInfoDetailList(imInfoObject);
        model.add("imInfoList", imInfoDetailList);
        //        model.add("errInfoList", errInfoList);
        return "im-imInfoDetailList";
    }

    //    @ReqMapping(path="@")
    public String testReferer(Invocation inv) {
        HttpServletRequest request = inv.getRequest();
        System.out.println("referer" + request.getHeader("Referer"));
        return "im-testReferer";

    }
}
