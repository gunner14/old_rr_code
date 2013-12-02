package com.renren.ad.sns.services.avenue.util;

import java.text.ParseException;
import java.text.SimpleDateFormat;


/**
 * 将x=[0, N]归一化到y=[0,1]
 * @author xiangzhang
 *
 */
public class ValueNorm {

    /**
     * 
     * @param threshold y=0.9时对应的x取值
     * @param oldValue
     * @return
     */
    public static double getNormValue(double threshold, double oldValue){
        double a = threshold/3.0;
        return (1-Math.exp(-oldValue/a))/(1+Math.exp(-oldValue/a));
    }
    
    public static void main(String[] args) throws ParseException {
        SimpleDateFormat DF = new SimpleDateFormat("yyMMddhhmm");
        String publishTime = "" + 1207140815130811632l / 1000000000L;
        long publishInterval = 0;
        publishInterval = (System.currentTimeMillis() - DF.parse(
                publishTime).getTime())
                / (60 * 1000);
        System.out.println(-30*getNormValue(4320, publishInterval));
    }
}
