package com.renren.sns.guide.model;

import java.util.List;

import mop.hi.oce.domain.network.CollegeInfo;

import org.json.JSONObject;

import com.dodoyo.datum.client.DatumInfoClient;
import com.renren.sns.guide.logic.GuideRelationSearch;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.search2.condition.GuideCondition;
import com.xiaonei.search2.condition.subcondition.CollegeCondition;

public class GuideCollegeSchoolRelationModel extends GuideAbstractModel {

    public GuideCollegeSchoolRelationModel(User host, int limit) {
        super(host, limit);
    }

    @Override
    public List<GuideIRelation> getRelations(User host, int offset, int limit) {
        this.setBegin(System.currentTimeMillis());
        GuideCondition builder = null;
        // 规整搜索结果
        GuideSearchRelation relation = null;
        try {
            // 获得资料

            List<CollegeInfo> colleges = DatumInfoClient.getInstance().getCollegeInfos(host);
            if (colleges != null && colleges.size() != 0) {
                CollegeInfo collegeInfo = colleges.get(0);
                if (!"".equalsIgnoreCase(collegeInfo.getCollegeName())) {
                    CollegeCondition condition = new CollegeCondition(collegeInfo.getCollegeId(),
                            collegeInfo.getCollegeName().trim(), collegeInfo.getDepartment(),
                            collegeInfo.getEnrollYear());
                    builder = new GuideCondition();
                    builder.addCollegeInfo(condition);
                    relation = GuideRelationSearch.getInstance().search(host.getId(), offset,
                            limit, builder);
                    relation.setTitle("我们在 " + collegeInfo.getCollegeName() + " 为你找到了"
                            + relation.getCount() + "位同学");
                    relation.setType("college");
                    relation.setTypeId(collegeInfo.getCollegeId());
                    relation.setTypeName(collegeInfo.getCollegeName());
                    int year = collegeInfo.getEnrollYear();
                    JSONObject jo = new JSONObject();
                    //t 这个值需要问一下老罗
                    jo.put("t", "sect");
                    jo.put("name", java.net.URLEncoder
                            .encode(collegeInfo.getCollegeName(), "UTF-8"));
                    if (year != 0) {
                        jo.put("year", year);
                    }

                    relation.setMoreLink("p=" + "[" + jo.toString() + "]" + "&s=0");
                    getResult().add(relation);
                }
            }
        } catch (Exception e) {
            System.err.println(e + "\t Tout class:GuideCollegeSchoolRelationModel");
            e.printStackTrace();
        }
        this.setEnd(System.currentTimeMillis());
        return getResult();
    }

}
