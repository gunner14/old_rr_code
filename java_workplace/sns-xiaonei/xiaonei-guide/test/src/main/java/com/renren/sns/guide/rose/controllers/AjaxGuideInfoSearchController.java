package com.renren.sns.guide.rose.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.portal.Portal;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;

/**
 * @author: wei.xiang
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-5-20 下午02:24:40
 */
@LoginRequired
public class AjaxGuideInfoSearchController {

    @Get
    public String index(Invocation inv, Portal portal, CacheManager cm) throws InterruptedException {
        /*User user = UserProvider.getInstance().get(cm);
        try {
            int amount = 49;
            long beginMayKnown = LogTimeUtils.logBegin();
            List<GuideIRelationModel> list = new ArrayList<GuideIRelationModel>();
            //获取各个阶段的Model
            GuideIRelationModel workerModel = GuideRelationModelFactory.create(user, amount, GuideRelationModelFactory.COWORKER_RELATION_MODEL);
            GuideIRelationModel universityModel = GuideRelationModelFactory.create(user, amount, GuideRelationModelFactory.UNIVERSITY_RELATION_MODEL);
            GuideIRelationModel collegeModel = GuideRelationModelFactory.create(user, amount, GuideRelationModelFactory.COLLEGE_RELATION_MODEL);
            GuideIRelationModel highModel = GuideRelationModelFactory.create(user, amount, GuideRelationModelFactory.HIGH_RELATION_MODEL);
            GuideIRelationModel juniorModel = GuideRelationModelFactory.create(user, amount, GuideRelationModelFactory.JUNIOR_RELATION_MODEL);
            GuideIRelationModel elementaryModel = GuideRelationModelFactory.create(user, amount, GuideRelationModelFactory.ELEMENTARY_RELATION_MODEL);
            GuideIRelationModel locationModel = GuideRelationModelFactory.create(user, amount, GuideRelationModelFactory.LOCATION_RELATION_MODEL);
            list.add(workerModel);
            list.add(universityModel);
            list.add(collegeModel);
            list.add(highModel);
            list.add(juniorModel);
            list.add(elementaryModel);
            list.add(locationModel);
            List<GuideIRelation> mayFriends = GuideFriendsUtil.findPeople(user, list);
            LogTimeUtils.logEnd(beginMayKnown, "GuideInfoSearch");
            long beginSameIP = LogTimeUtils.logBegin();
            int limit = -1;//全取出来
            String ip = BizFilterUtil.getClientIP(inv.getRequest());
            int[] co = IPRecordAdapter.getInstance().getUsers(ip, limit);
            List<Integer> userIds = GuideXFiveIDsPreviewLogic.getInstance().getIntegerList(co); 
            LogTimeUtils.logEnd(beginSameIP, "GuideAjaxSameIp");
            String json = GuideUtils.getInstance().getJSONPeopleMayKnowForGuide(mayFriends, userIds);
            return "@" + json;

        } catch (Exception e) {
            e.printStackTrace();
        }*/
        return "@";
    }

}
