package com.xiaonei.xnmonitor.utils;

import org.jrobin.core.RrdException;
import org.jrobin.core.Util;
import org.jrobin.graph.RrdGraphDef;

public class ImgUtil {

    public static void setTimePeriod(RrdGraphDef gdef, String type) throws RrdException {
        long endTime = Util.getTime();
        if (type.equals("hour")) {
            System.out.println("begin" + (endTime - 3600) + ":end" + endTime);
            gdef.setTimePeriod(endTime - 3600, endTime);
        } else if (type.equals("day")) {
            gdef.setTimePeriod(endTime - 24 * 3600, endTime);
        } else if (type.equals("week")) {
            gdef.setTimePeriod(endTime - 24 * 3600 * 7, endTime);
        } else if (type.equals("month")) {
            gdef.setTimePeriod(endTime - 24 * 3600 * 30, endTime);
        }

    }

}
