package com.xiaonei.xnmonitor.utils;

import java.util.Comparator;
import java.util.Date;

import org.jrobin.core.FetchData;
import org.jrobin.core.FetchRequest;
import org.jrobin.core.RrdDb;
import org.jrobin.core.RrdDef;
import org.jrobin.core.Util;

public class SortFile implements Comparator {

    long start;

    long end;

    String dsName;

    String prefix;

    public SortFile(String prefix, long start, long end, String dsName) {
        this.start = start;
        this.end = end;
        this.dsName = dsName;
        this.prefix = prefix;
    }

    @Override
    public int compare(Object fileStr1, Object fileStr2) {
        return excute(fileStr1, fileStr2);
    }

    public int excute(Object fileStr, Object fileStr2) {
        RrdDb rrd1;
        RrdDb rrd2;
        FetchRequest request1;
        FetchRequest request2;
        FetchData data1;
        FetchData data2;
        boolean flag = false;
        try {
            rrd1 = new RrdDb(prefix + "/" + fileStr, true);
            request1 = rrd1.createFetchRequest("AVERAGE", end, start);
            rrd2 = new RrdDb(prefix + "/" + fileStr2, true);
            request2 = rrd2.createFetchRequest("AVERAGE", end, start);
            data1 = request1.fetchData();
            data2 = request2.fetchData();
            flag = data1.getAggregate(dsName, "AVERAGE") >= data2.getAggregate(dsName, "AVERAGE");
            //			Date begin= new Date(start*1000);
            //			Date endDate = new Date(end*1000);
            //			System.out.println(begin+"<---->"+endDate+"<----->");
            //		
            //			System.out.println(rrd1.getPath()+data1.getAggregate(dsName, "AVERAGE") +"<----->"+rrd2.getPath()+ data2.getAggregate(
            //					dsName, "AVERAGE")+"<----->"+flag);
        } catch (Exception e) {
            e.printStackTrace();
        }
        if (flag) {
            return 0;
        } else {
            return 1;

        }

    }
}
