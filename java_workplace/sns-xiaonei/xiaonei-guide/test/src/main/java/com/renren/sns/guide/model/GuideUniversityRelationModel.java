package com.renren.sns.guide.model;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.util.Collections;
import java.util.List;

import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.domain.network.UniversityInfo;

import org.json.JSONObject;

import com.renren.sns.guide.logic.GuideRelationSearch;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.search2.condition.GuideCondition;
import com.xiaonei.search2.condition.subcondition.UniversityCondition;
import com.xiaonei.search2.model.UserFieldNames;

public class GuideUniversityRelationModel extends GuideAbstractModel {

    public GuideUniversityRelationModel(User host, int limit) {
        super(host, limit);
    }

    @Override
    public List<GuideIRelation> getRelations(User host, int offset, int limit) {
        this.setBegin(System.currentTimeMillis());
        // 获得资料
        List<UniversityInfo> univInfoList = AdapterFactory.getNetworkAdapter().getUniversityInfoList(host.getId()); // 大学资料信息
        // 构建大学搜索条件
        Collections.reverse(univInfoList);
        UniversityCondition condition;
        for (UniversityInfo universityInfo : univInfoList) {
            GuideCondition builder = new GuideCondition();
            condition = new UniversityCondition(universityInfo.getUniversityId(),
                    universityInfo.getUniversityName().trim(), universityInfo.getDepartment(),
                    "", universityInfo.getEnrollYear(),
                    UserFieldNames.UNIVERSITY_INFO_YEAR_FLAG_MUST);
            builder.addUniversityInfo(condition);
            GuideSearchRelation relation = GuideRelationSearch.getInstance().search(host.getId(), offset,
                    limit, builder);
            relation.setTitle(universityInfo.getUniversityName());
            relation.setCount(relation.getCount());
            relation.setType("univ");
            relation.setTypeId(universityInfo.getUniversityId());
            relation.setTypeName(universityInfo.getUniversityName()
                    + (!"".equalsIgnoreCase(universityInfo.getDepartment()) ? universityInfo.getDepartment() : ""));
            int year = universityInfo.getEnrollYear();
            JSONObject json = new JSONObject();
            json.put("t", "univ");
            try {
				json.put("name", URLEncoder.encode(universityInfo.getUniversityName(), "UTF-8"));
				json.put("id", universityInfo.getUniversityId());
                json.put("depa", URLEncoder.encode(universityInfo.getDepartment(), "UTF-8"));
			} catch (NullPointerException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (UnsupportedEncodingException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
            if (year != 0) {
                json.put("year", year);
            }
            relation.setMoreLink("p=[" + json.toString() + "]&s=0");
            getResult().add(relation);
        }
        this.setEnd(System.currentTimeMillis());        
        return this.getResult();
    }

}
 