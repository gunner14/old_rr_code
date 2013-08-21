package com.xiaonei.xce.footprintunite;

import mop.hi.oce.adapter.AdapterFactory;

import org.apache.log4j.BasicConfigurator;

public class HackFootprint {

    public static void main(String[] args) {
        BasicConfigurator.configure();
        try {
            FootprintUniteAdapter impl = AdapterFactory.getHomeFootprintAdapter();
            int hostId = 224456830;
            int count  = 94091;
            System.out.println(impl.getUserVisitCount(hostId));
//            impl.setUserVisitCount(hostId, count);
            
        } catch (Exception e) {
            e.printStackTrace();
        }

        System.exit(0);

    }

}
