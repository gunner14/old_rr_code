package com.renren.sns.guide.model;

import java.net.URLEncoder;
import java.util.Collections;
import java.util.List;

import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.domain.network.WorkplaceInfo;

import org.json.JSONObject;

import com.renren.sns.guide.logic.GuideRelationSearch;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.search2.condition.GuideCondition;
import com.xiaonei.search2.condition.subcondition.WorkplaceCondition;

public class GuideCoWorkerRelationModel extends GuideAbstractModel {

    public GuideCoWorkerRelationModel(User host, int limit) {
        super(host, limit);
    }

    @Override
    public List<GuideIRelation> getRelations(User host, int offset, int limit) {
        this.setBegin(System.currentTimeMillis());
        try {
            // 获得资料
            List<WorkplaceInfo> workInfoList = AdapterFactory.getNetworkAdapter()
                    .getWorkplaceInfoList(host.getId()); // 工作资料信息
            Collections.reverse(workInfoList);// 逆序排，后加的为更新的
            // 构建工作搜索条件
            WorkplaceCondition condition;
            for (WorkplaceInfo workplaceInfo : workInfoList) {
                GuideCondition builder = new GuideCondition();
                condition = new WorkplaceCondition(0, workplaceInfo.getWorkplaceName().trim());
                builder.addWorkplaceInfo(condition);
                GuideSearchRelation relation = GuideRelationSearch.getInstance().search(
                        host.getId(), offset, limit, builder);

                relation.setTitle(workplaceInfo.getWorkplaceName());
                relation.setCount(relation.getCount());
                relation.setType("work");
                relation.setCount(relation.getCount());
                relation.setTypeName(workplaceInfo.getWorkplaceName());
                JSONObject json = new JSONObject();
                json.put("t", "work");
                json.put("name", URLEncoder.encode(workplaceInfo.getWorkplaceName(), "UTF-8"));
                relation.setMoreLink("p=[" + json.toString() + "]&s=0");
                getResult().add(relation);
            }
        } catch (Exception e) {
            System.err.println(e + "\t Tout class:GuideCoWorkerRelationModel");
            e.printStackTrace();
        }
        this.setEnd(System.currentTimeMillis());        
        return this.getResult();
    }

}
