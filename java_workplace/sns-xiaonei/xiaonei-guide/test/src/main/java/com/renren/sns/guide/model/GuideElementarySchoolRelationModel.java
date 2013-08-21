package com.renren.sns.guide.model;

import java.util.List;

import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.domain.network.ElementarySchoolInfo;

import org.json.JSONObject;

import com.renren.sns.guide.logic.GuideRelationSearch;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.search2.condition.GuideCondition;
import com.xiaonei.search2.condition.subcondition.ElementarySchoolCondition;

public class GuideElementarySchoolRelationModel extends GuideAbstractModel {

    public GuideElementarySchoolRelationModel(User host, int limit) {
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
            ElementarySchoolInfo eleInfo = AdapterFactory.getNetworkAdapter()
                    .getElementarySchoolInfo(host.getId()); // 小学资料信息
            if (eleInfo!=null && !"".equalsIgnoreCase(eleInfo.getElementarySchoolName())) {
                // 构建小学搜索条件
                String schoolName = eleInfo.getElementarySchoolName().trim();
                ElementarySchoolCondition condition = new ElementarySchoolCondition(0, schoolName,
                        eleInfo.getElementarySchoolYear());
                builder = new GuideCondition();
                builder.addElementarySchoolInfo(condition);
                relation = GuideRelationSearch.getInstance().search(host.getId(), offset, limit,
                        builder);
                relation.setTitle("我们在 " + eleInfo.getElementarySchoolName() + " 为你找到了"
                        + relation.getCount() + "位同学 ");
                relation.setCount(relation.getCount());
                relation.setType("elementary");
                relation.setTypeId(eleInfo.getElementarySchoolId());
                relation.setTypeName(eleInfo.getElementarySchoolName());
                int year = eleInfo.getElementarySchoolYear();

                JSONObject jo = new JSONObject();
                jo.put("t", "elem");
                jo.put("name", java.net.URLEncoder.encode(eleInfo.getElementarySchoolName(),
                        "UTF-8"));
                if (year != 0) {
                    jo.put("year", year);
                }
                relation.setMoreLink("p=" + "[" + jo.toString() + "]" + "&s=0");
                getResult().add(relation);
            }
        } catch (Exception e) {
            System.err.println(e + "\t Tout class:GuideElementarySchoolRelationModel");
            //e.printStackTrace();
        }
        this.setEnd(System.currentTimeMillis());        
        return this.getResult();
    }

}
