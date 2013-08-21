package com.renren.sns.guide.model;

import java.util.List;

import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.domain.network.JuniorHighSchoolInfo;

import org.json.JSONObject;

import com.renren.sns.guide.logic.GuideRelationSearch;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.search2.condition.GuideCondition;
import com.xiaonei.search2.condition.subcondition.JuniorHighSchoolCondition;

public class GuideJuniorSchoolRelationModel extends GuideAbstractModel {

    public GuideJuniorSchoolRelationModel(User host, int limit) {
        super(host, limit);
    }

    @Override
    public List<GuideIRelation> getRelations(User host, int offset, int limit) {
        this.setBegin(System.currentTimeMillis());
        GuideCondition builder;
        // 规整搜索结果
        GuideSearchRelation relation;
        try {
            // 获得资料
            JuniorHighSchoolInfo junInfo = AdapterFactory.getNetworkAdapter()
                    .getJuniorHighSchoolInfo(host.getId()); // 初中资料信息
            // 构建初中搜索条件
            if (junInfo != null) {
                JuniorHighSchoolCondition junCondition = new JuniorHighSchoolCondition(junInfo
                        .getJunHighentarySchoolId(), junInfo.getJunHighentarySchoolName().trim(),
                        junInfo.getJunHighentarySchoolYear());
                if (!"".equalsIgnoreCase(junInfo.getJunHighentarySchoolName())) {
                    builder = new GuideCondition();
                    builder.addJuniorHighSchoolInfo(junCondition);
                    relation = GuideRelationSearch.getInstance().search(host.getId(), offset,
                            limit, builder);
                    relation.setTitle("我们在 " + junInfo.getJunHighentarySchoolName() + " 为你找到了"
                            + relation.getCount() + "位同学 ");
                    relation.setCount(relation.getCount());
                    relation.setType("junior");
                    relation.setTypeId(junInfo.getJunHighentarySchoolId());
                    relation.setTypeName(junInfo.getJunHighentarySchoolName());
                    int year = junInfo.getJunHighentarySchoolYear();
                    JSONObject jo = new JSONObject();
                    jo.put("t", "juni");
                    jo.put("name", java.net.URLEncoder.encode(junInfo.getJunHighentarySchoolName(),
                            "UTF-8"));
                    if (year != 0) {
                        jo.put("year", year);
                    }
                    relation.setMoreLink("p=" + "[" + jo.toString() + "]" + "&s=0");
                    getResult().add(relation);
                }
            }

        } catch (Exception e) {
            System.err.println(e + "\t Tout class:GuideJuniorSchoolRelationModel");
            e.printStackTrace();
        }
        this.setEnd(System.currentTimeMillis());
        return this.getResult();
    }

}
