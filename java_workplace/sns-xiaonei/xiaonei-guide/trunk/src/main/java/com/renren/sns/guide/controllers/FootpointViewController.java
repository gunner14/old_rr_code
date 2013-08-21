package com.renren.sns.guide.controllers;

import java.util.List;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import net.sf.json.JSONArray;
import net.sf.json.JSONObject;

import com.renren.sns.guide.model.SnsFootprintResult;
import com.xiaonei.platform.core.head.HeadUrlUtil;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.reg.guide.logic.VisitLogLogic;
import com.xiaonei.reg.guide.model.SnsFootprintView;

public class FootpointViewController {

    @Post
    public String index(Invocation inv) {
        JSONObject jsonO = new JSONObject();
        Integer hostId;
        try {
            hostId = BaseThreadUtil.currentHost(inv.getRequest()).getId();
        } catch (Exception e) {
            e.printStackTrace();
            return "@";
        }
        SnsFootprintResult result = VisitLogLogic.getSnsFootprintResult(hostId, 0, 100);
        List<SnsFootprintView> visitList = result.getFootprintViewList();
        if (result.getUserVisitCount() > 0) {
            jsonO.put("viewcount", result.getUserVisitCount());
        }
        if (visitList != null && visitList.size() > 0) {
            int i = 0;
            JSONArray jsonA = new JSONArray();
            for (SnsFootprintView footView : visitList) {
                JSONObject jsonView = new JSONObject();
                jsonView.put("isOnline", footView.isOnline());
                jsonView.put("isVip", footView.isVip());
                jsonView.put("isKeepUse", footView.isKeepUse());
                jsonView.put("profileUrl", OpiConstants.urlMain
                        + "/profile.do?portal=homeFootprint&ref=home_footprint&id="
                        + footView.getGuest() + "&a=1");
                jsonView.put("name", footView.getGuestName());
                jsonView.put("headUrl", HeadUrlUtil.getHeadFullUrl(footView.getGuestUrl()));
                jsonView.put("date", footView.getDateTipString());
                jsonA.add(jsonView);
                if (++i >= 100) {
                    break;
                }
            }
            jsonO.put("visitList", jsonA);
        }
        return "@" + jsonO.toString();
    }

    @Get
    public String clickStat(Invocation inv, @Param("act") String action) {
        if ("l".equals(action)) {

        } else if ("r".equals(action)) {

        } else if ("h".equals(action)) {

        }
        return "@";
    }

}
