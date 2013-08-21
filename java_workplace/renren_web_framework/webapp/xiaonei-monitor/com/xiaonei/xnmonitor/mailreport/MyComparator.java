package com.xiaonei.xnmonitor.mailreport;

import java.util.Comparator;

import com.xiaonei.xnmonitor.dto.IMInfo;

public class MyComparator implements Comparator<IMInfo> {

    @Override
    public int compare(IMInfo o1, IMInfo o2) {
        // TODO Auto-generated method stub
        if (o1.getPercent() >= o2.getPercent()) {
            return 0;
        } else {
            return 1;
        }
    }

}
