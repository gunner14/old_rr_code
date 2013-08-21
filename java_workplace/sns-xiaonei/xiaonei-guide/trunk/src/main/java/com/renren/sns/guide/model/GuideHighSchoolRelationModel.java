package com.renren.sns.guide.model;

import java.util.Collections;
import java.util.List;

import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.domain.network.HighSchoolInfo;

import org.json.JSONObject;

import com.renren.sns.guide.logic.GuideRelationSearch;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.search2.condition.GuideCondition;
import com.xiaonei.search2.condition.subcondition.HighSchoolCondition;

public class GuideHighSchoolRelationModel extends GuideAbstractModel {

    public GuideHighSchoolRelationModel(User host, int limit) {
        super(host, limit);
    }

    @Override
    public List<GuideIRelation> getRelations(User host, int offset, int limit) {
        this.setBegin(System.currentTimeMillis());
        try {
            // 获得资料
            List<HighSchoolInfo> hsInfoList = AdapterFactory.getNetworkAdapter().getHighSchoolInfoList(host.getId()); // 高中资料信息
            // 构建高中搜索条件
            Collections.reverse(hsInfoList); // 逆序排
            HighSchoolCondition condition;
            for (HighSchoolInfo hsInfo : hsInfoList) {
                GuideCondition builder = new GuideCondition();
                condition = new HighSchoolCondition(hsInfo.getHighSchoolId(), hsInfo.getHighSchoolName().trim(), "", "", "", hsInfo.getEnrollYear());
                builder.addHighSchoolInfo(condition);                
                GuideSearchRelation relation = GuideRelationSearch.getInstance().search(host.getId(), offset, limit, builder);    
                relation.setTitle(hsInfo.getHighSchoolName());
                relation.setCount(relation.getCount());
                relation.setType("high");
                relation.setTypeId(hsInfo.getHighSchoolId());
                relation.setTypeName(hsInfo.getHighSchoolName());
                int year = hsInfo.getEnrollYear();
                JSONObject json = new JSONObject();
                json.put("t", "high");
                json.put("name", java.net.URLEncoder.encode(hsInfo.getHighSchoolName(), "UTF-8"));
                if (year != 0) {
                    json.put("year", year);
                }
                relation.setMoreLink("p=[" + json.toString() + "]&s=0");
                getResult().add(relation);
            }
        } catch (Exception e) {
            System.err.println(e + "\t Tout class:GuideHighSchoolRelationModel");
            e.printStackTrace();
        }
        this.setEnd(System.currentTimeMillis());        
        return this.getResult();
    }

}
