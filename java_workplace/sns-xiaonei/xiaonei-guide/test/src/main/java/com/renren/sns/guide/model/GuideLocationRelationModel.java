package com.renren.sns.guide.model;

import java.net.URLEncoder;
import java.util.List;

import org.json.JSONObject;

import com.dodoyo.datum.client.DatumInfoClient;
import com.dodoyo.datum.model.RegionInfo;
import com.renren.sns.guide.logic.GuideRelationSearch;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.search2.condition.GuideCondition;
import com.xiaonei.search2.condition.subcondition.RegionCondition;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

public class GuideLocationRelationModel extends GuideAbstractModel {

    public GuideLocationRelationModel(User host, int limit) {
        super(host, limit);
    }

    @Override
    public List<GuideIRelation> getRelations(User host, int offset, int limit) {
        this.setBegin(System.currentTimeMillis());
        UserBasic ub = SnsAdapterFactory.getUserBasicAdapter().get(host.getId());
        try {
            // 获得资料
            // 构建工作搜索条件
            GuideCondition builder = new GuideCondition();
            RegionInfo regionInfo = DatumInfoClient.getInstance().getRegionInfo(host);
            if (regionInfo != null && regionInfo.getProvince_name() != null
                    && regionInfo.getCity_name() != null) {
                builder.addRegionInfo(new RegionCondition(0, regionInfo.getProvince_name(),
                        regionInfo.getCity_name()));
            } else {
                return null;
            }
            if (!host.getUserState().isGuideDone() && host.getStage() == 90) {
                builder.addCurrentStage(90);
            }
            builder.addHomeCityShould(ub.getHomeCity());
            builder.addHomeProvinceMust(ub.getHomeProvince());
            builder.addBirthYear(ub.getBirthYear());
            GuideSearchRelation relation = GuideRelationSearch.getInstance().search(host.getId(),
                    offset, limit, builder);
            relation.setTitle("我们在 " + regionInfo.getProvince_name() + "的" + ub.getHomeProvince()
                    + " 为你找到了" + relation.getCount() + "人 ");
            relation.setCount(relation.getCount());
            relation.setType("location");
            relation.setTypeId(regionInfo.getRegion_id());
            relation.setTypeName("在" + regionInfo.getProvince_name() + "的" + ub.getHomeProvince()
                    + "人");
            JSONObject json = new JSONObject();
            json.put("t", "base");
            json.put("prov", URLEncoder.encode(ub.getHomeProvince(), "UTF-8"));
            json.put("city", URLEncoder.encode(ub.getHomeCity(), "UTF-8"));
            relation.setMoreLink("p=[" + json.toString() + "]&s=0");
            getResult().add(relation);
        } catch (Exception e) {
            System.err.println(e + "\t Tout class:GuideLocationRelationModel");
            e.printStackTrace();
        }
        this.setEnd(System.currentTimeMillis());        
        return this.getResult();
    }

}
