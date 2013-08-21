package com.xiaonei.reg.guide.logic;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import com.renren.sns.guide.model.SnsFootprintResult;
import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.reg.guide.model.SnsFootprintView;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.xce.footprintunite.FootprintView;
import com.xiaonei.xce.footprintunite.HomeFootprintResult;

/**
 * 
 * VisitLogLogic.java
 * 
 * @author wei.xiang@opi-corp.com 2009-6-27 2:25:25
 */
public class VisitLogLogic {    

    /**
     * 
     * @param hostId the id for host
     * @param list the guest list
     * @return a list with guest name and tiny url
     * @throws Exception
     * @author wei.xiang@opi-corp.com
     * @since 20090917
     * @author wei.xiang@opi-corp.com Jun 1, 2010 - 4:04:53 PM
     */
    public static List<SnsFootprintView> convertFootprintNewView2SnsFootprintViews(int hostId,List<FootprintView> list) {
        if (list == null) return null;
        List<SnsFootprintView> ret = new ArrayList<SnsFootprintView>(list.size());
        if (list.size() == 0) return ret;
        List<Integer> guestIds = new ArrayList<Integer>(list.size());
        for (FootprintView ppv : list) {
            guestIds.add(ppv.getVisitor());
        }
        Map<Integer, WUserCache> userCacheMap = SnsAdapterFactory.getUserCacheAdapter().getUserCacheMap(guestIds);
        for (FootprintView ppv : list) {
            int id = ppv.getVisitor();
            WUserCache uc = userCacheMap.get(id);
            if (uc == null) continue;//忽略那些获取用户信息失败的访问者
            SnsFootprintView sfpv = new SnsFootprintView();
            sfpv.setCharity(uc.isCharity());
            sfpv.setHost(hostId);
            sfpv.setGuest(id);
            sfpv.setGuestName(uc.getName());
            sfpv.setGuestUrl(uc.getTinyUrl());
            sfpv.setTime(ppv.getTime());
            sfpv.setCharity(uc.isCharity());
            sfpv.setKeepUse(uc.isKeepUse());
            sfpv.setOnline(uc.isOnline());
            sfpv.setVip(uc.isVipMember());
            sfpv.setWapOnline(uc.isWapOnline());
            ret.add(sfpv);
        }
        return ret;
    }

    public static SnsFootprintResult getSnsFootprintResult(int hostId, int begin, int limit) {
        SnsFootprintResult result = new SnsFootprintResult();
        HomeFootprintResult fr = SnsAdapterFactory.getFootprintAdapter().getAllFootprints(hostId, begin, limit);
        if(null == fr){
            return result;
        }
        List<SnsFootprintView> sfv = convertFootprintNewView2SnsFootprintViews(hostId, fr.getFootprintViews());
        result.getFootprintViewList().addAll(sfv);
        result.setUserVisitCount(fr.getVisitCount());
        return result;
    }

}
