package com.xiaonei.xnmonitor.data;

import java.sql.Timestamp;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.LinkedList;
import java.util.List;

import net.paoding.rose.ar.Ar;
import net.paoding.rose.web.annotation.Param;

import com.xiaonei.xnmonitor.dto.IMInfo;
import com.xiaonei.xnmonitor.dto.TimeParam;
import com.xiaonei.xnmonitor.utils.ObjectPack;

public class FetchData {

    private static final SimpleDateFormat DATEFORMAT = new SimpleDateFormat(
            "yyyy-MM-dd HH:mm:ss.sss");

    /**
     * 获得DNS画图的数据
     * 
     * @param begin
     * @param end
     * @return
     * @throws ParseException
     */
    public List<IMInfo> fetchIMDNSData(Timestamp begin, Timestamp end, @Param("type") String type,
            @Param("hostOrUrl") String hostOrUrl) throws ParseException {
        //        String sql = "select cost_time,happen_time from im where type ='dns' and happen_time between '"
        //                + begin + "' and '" + end + "' order by happen_time ";
        FetchData fetchData = new FetchData();
        StringBuilder sql = new StringBuilder();
        sql.append("select cost_time,happen_time from im where type='" + type
                + "' and happen_time between '" + begin + "' and '" + end + "' and url='"
                + hostOrUrl + "' order by happen_time  ");
        System.out.println(sql.toString());
        List<Object[]> IMDNSDataObjectArrList = Ar.sql(sql.toString());
        List<IMInfo> IMDNSDataList = ObjectPack.getIMDNSDataList(IMDNSDataObjectArrList);
        List<List<IMInfo>> seperateIMDNSDataList = fetchData.seperateIMDNSDataList(IMDNSDataList,
                begin, end);
        fetchData.setVariance(seperateIMDNSDataList);
        //        for (IMInfo imInfo : IMDNSDataList) {
        //            System.out.println("----" + imInfo.getVariance());
        //        }
        return IMDNSDataList;
    }

    /**
     * 设置方差
     * 
     * @param IMDNSDataList
     */
    public void setVariance(List<List<IMInfo>> separateIMDNSDataList) {
        for (List<IMInfo> imInfoList : separateIMDNSDataList) {
            if (imInfoList.size() == 0) break;
            double averageCostTime = getAverageCostTime(imInfoList);
            double varianceTotal = 0;
            double variance = 0;
            System.out.println("averageCostTime:" + averageCostTime);
            for (IMInfo imInfo : imInfoList) {
                varianceTotal += (imInfo.getCostTime() - averageCostTime)
                        * (imInfo.getCostTime() - averageCostTime);
            }
            variance = varianceTotal / imInfoList.size();
            for (IMInfo imInfo : imInfoList) {
                imInfo.setVariance(Math.sqrt(variance));
            }
            //            for (IMInfo imInfo : imInfoList) {
            //                imInfo.setVariance(averageCostTime - imInfo.getCostTime());
            //            }

        }
        //        for (List<IMInfo> imInfoList : separateIMDNSDataList) {
        //            for (IMInfo imInfo : imInfoList) {}
        //
        //        }

    }

    public List<List<IMInfo>> seperateIMDNSDataList(List<IMInfo> IMDNSDataList, Timestamp begin,
            Timestamp end) throws ParseException {
        if (IMDNSDataList.size() == 0) return null;
        Calendar ca = Calendar.getInstance();
        ca.setTime(begin);
        ca.set(Calendar.MILLISECOND, 0);
        ca.set(Calendar.SECOND, 0);
        ca.set(Calendar.MINUTE, 0);
        ca.add(Calendar.HOUR_OF_DAY, 1);
        List<List<IMInfo>> seperatedIMDNSDataList = new LinkedList<List<IMInfo>>();
        List<IMInfo> imInfoList = new LinkedList<IMInfo>();
        for (int i = 0; i < IMDNSDataList.size(); i++) {
            if (DATEFORMAT.parse(IMDNSDataList.get(i).getHappenTime()).getTime() < ca
                    .getTimeInMillis()) {
                imInfoList.add(IMDNSDataList.get(i));

            } else if (DATEFORMAT.parse(IMDNSDataList.get(i).getHappenTime()).getTime() >= ca
                    .getTimeInMillis()) {
                if (imInfoList.size() > 0) {
                    seperatedIMDNSDataList.add(imInfoList);
                    imInfoList = new LinkedList<IMInfo>();
                }
                ca.add(Calendar.HOUR_OF_DAY, 1);
                i--;
            }
            if (i == IMDNSDataList.size() - 1) {
                seperatedIMDNSDataList.add(imInfoList);
            }

        }
        return seperatedIMDNSDataList;
    }

    /**
     * 取得平均时间
     * 
     * @param IMDNSDataList
     * @return
     */
    public double getAverageCostTime(List<IMInfo> IMDNSDataList) {
        if (IMDNSDataList.size() < 1) return 0;
        double totalCostTime = 0;
        for (IMInfo iminfo : IMDNSDataList) {
            totalCostTime += iminfo.getCostTime();
        }
        return totalCostTime / IMDNSDataList.size();
    }

}
