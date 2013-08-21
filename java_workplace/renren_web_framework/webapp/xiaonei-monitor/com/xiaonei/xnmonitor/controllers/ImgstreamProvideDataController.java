package com.xiaonei.xnmonitor.controllers;

import java.awt.Color;
import java.io.IOException;
import java.io.OutputStream;
import java.sql.Timestamp;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

import javax.servlet.http.HttpServletResponse;

import org.jrobin.core.RrdException;
import org.jrobin.graph.CubicSplineInterpolator;
import org.jrobin.graph.LinearInterpolator;
import org.jrobin.graph.Plottable;
import org.jrobin.graph.RrdGraph;
import org.jrobin.graph.RrdGraphDef;

import com.xiaonei.xnmonitor.data.FetchData;
import com.xiaonei.xnmonitor.dto.IMInfo;
import com.xiaonei.xnmonitor.dto.TimeParam;
import com.xiaonei.xnmonitor.utils.UtilTools;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;

/**
 * 需要直接提供画图的数据的 这个是给IM监控提供画图数据的。计算方差。
 * 
 * @author lightning
 */
public class ImgstreamProvideDataController {

    private static final SimpleDateFormat DATEFORMAT = new SimpleDateFormat(
            "yyyy-MM-dd HH:mm:ss.sss");

    public void getImg(Invocation inv, TimeParam timeParam, @Param("bigPic") String bigPic,
            @Param("type") String type, @Param("hostOrUrl") String hostOrUrl)
            throws ParseException, RrdException, IOException {
        Timestamp begin = UtilTools.getTimeStamp(timeParam.getByear(), timeParam.getBmonth(),
                timeParam.getBday(), timeParam.getBhour(), timeParam.getBminute(), timeParam
                        .getBsecond());
        Timestamp end = UtilTools.getTimeStamp(timeParam.getEyear(), timeParam.getEmonth(),
                timeParam.getEday(), timeParam.getEhour(), timeParam.getEminute(), timeParam
                        .getEsecond());
        List<IMInfo> IMDNSDataList = new FetchData().fetchIMDNSData(begin, end, type, hostOrUrl);
        //        Plottable plottable = new Plottable() {
        //        };
        Date[] dateArr = getDatesArr(IMDNSDataList);
        transferDate(dateArr);
        double[] valueCostTimeArr = getCostTimeValuesArr(IMDNSDataList);
        double[] valueVarianceArr = getCostTimeVarianceValuesArr(IMDNSDataList);
        //        for (IMInfo iminfo:IMDNSDataList){
        //            System.out.println("Date:"+iminfo.getHappenTime()+":::variance:::"+iminfo.getVariance());
        //        }
        LinearInterpolator costTimeLine = new LinearInterpolator(dateArr, valueCostTimeArr);
        LinearInterpolator costTimeVarianceLine = new LinearInterpolator(dateArr, valueVarianceArr);

        RrdGraphDef gDef = new RrdGraphDef(begin, end);
        gDef.setVerticalLabel("Xiaonei Web Framework Team");
        gDef.setShowSignature(false);
        gDef.setTitle(begin + "~" + end + ":Cost Time and Variance");
        gDef.datasource("costTime", costTimeLine);
        gDef.datasource("costTimeVariance", costTimeVarianceLine);

        gDef.stack("costTime", Color.RED, "costTime");
        gDef.stack("costTimeVariance", Color.GREEN, "costTimeVariance@r");

        RrdGraph graph = new RrdGraph(gDef);
        HttpServletResponse response = inv.getResponse();
        response.setContentType("image/png");
        OutputStream outputStream = response.getOutputStream();
        if ("true".equals(bigPic)) {
            outputStream.write(graph.getPNGBytes(1000, 520));
        } else {
            bigPic = "true";
            outputStream.write(graph.getPNGBytes());
        }
        outputStream.flush();
        outputStream.close();

    }

    public Date[] getDatesArr(List<IMInfo> IMDNSDataList) throws ParseException {
        Date[] dateArr = new Date[IMDNSDataList.size()];
        for (int i = 0; i < IMDNSDataList.size(); i++) {

            dateArr[i] = DATEFORMAT.parse(IMDNSDataList.get(i).getHappenTime());
        }
        return dateArr;
    }

    public double[] getCostTimeValuesArr(List<IMInfo> IMDNSDataList) throws ParseException {
        double[] valueArr = new double[IMDNSDataList.size()];
        for (int i = 0; i < IMDNSDataList.size(); i++) {
            valueArr[i] = IMDNSDataList.get(i).getCostTime();
        }
        return valueArr;
    }

    public double[] getCostTimeVarianceValuesArr(List<IMInfo> IMDNSDataList) throws ParseException {
        double[] valueArr = new double[IMDNSDataList.size()];
        for (int i = 0; i < IMDNSDataList.size(); i++) {
            //            System.out.println(i + ".....getVarianceValueArr" + IMDNSDataList.get(i).getVariance());
            IMDNSDataList.get(i).getVariance();
            valueArr[i] = IMDNSDataList.get(i).getVariance();
        }
        return valueArr;
    }

    public void transferDate(Date[] dates) {
        Calendar ca = Calendar.getInstance();
        Date datePointer = new Date(dates[0].getTime() - 1000);//标记前一个时间。
        for (int i = 0; i < dates.length; i++) {
            System.out.println("datePointer:" + datePointer + ":date:" + dates[i]);
            if (dates[i].getTime() <= datePointer.getTime()) {
                ca.setTimeInMillis(datePointer.getTime() + 1000);
                System.out.println("date:" + dates[i] + "ca.changedate" + ca.getTime());
                dates[i] = ca.getTime();
            }
            datePointer = dates[i];
        }

    }

}
