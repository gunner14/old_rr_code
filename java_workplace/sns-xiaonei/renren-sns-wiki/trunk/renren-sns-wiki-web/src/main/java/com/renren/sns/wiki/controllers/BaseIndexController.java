package com.renren.sns.wiki.controllers;

import java.util.List;
import java.util.Map;

import net.paoding.rose.web.Invocation;
import net.sf.json.JSONObject;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.wiki.enums.ItemType;
import com.renren.sns.wiki.enums.WikiFeedType;
import com.renren.sns.wiki.interceptor.WikiHolder;
import com.renren.sns.wiki.model.UserActionRecord;
import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.model.WikiCover;
import com.renren.sns.wiki.model.info.WikiInfo;
import com.renren.sns.wiki.service.FollowService;
import com.renren.sns.wiki.service.HighLightService;
import com.renren.sns.wiki.service.SuperAdminService;
import com.renren.sns.wiki.service.UserActionRecordService;
import com.renren.sns.wiki.service.WikiCoverService;
import com.renren.sns.wiki.service.WikiFeedService;
import com.renren.sns.wiki.service.WikiService;
import com.renren.sns.wiki.service.info.WikiInfoService;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.platform.core.model.User;

public class BaseIndexController {

    private static final Log logger = LogFactory.getLog(BaseIndexController.class);

    @Autowired
    private HostHolder hostHolder;

    @Autowired
    private WikiHolder wikiHolder;

    @Autowired
    private WikiService wikiService;

    @Autowired
    private WikiCoverService wikiCoverService;

    @Autowired
    private WikiInfoService wikiInfoService;

    @Autowired
    private UserActionRecordService userActionRecordService;

    @Autowired
    private WikiFeedService wikiFeedService;

    @Autowired
    private FollowService followService;

    @Autowired
    private SuperAdminService superAdminService;

    @Autowired
    private HighLightService highLightService;

    /**
     * 返回的页面都需要的信息,wikiInfo wikiCover 喜欢 照片 视频 等
     * 
     * @param inv
     * @param wikiId
     * @param curPage
     * @param pageSize
     * @param wikiFeedType
     * @param itemType
     */
    public void wrapWikiIndexInvocation(Invocation inv, Wiki wiki, User host, int curPage,
            int pageSize, String pageType) {

        inv.addModel("pageType", pageType);
        inv.addModel("curPage", curPage);
        inv.addModel("pageSize", pageSize);

        int wikiId = wiki.getId();
        //是否是超管
        boolean isSuperAdmin = false;
        if (host != null && superAdminService.isSuperAdmin(host.getId())) {
            isSuperAdmin = true;
        }
        inv.addModel("isSuperAdmin", isSuperAdmin);

        //平均分
        String valuationDisplayChars = wikiService.getValuationDisplayChars(wiki);
        inv.addModel("valuationDisplayChars", valuationDisplayChars);

        //资料项,cover
        WikiInfo wikiInfo = wikiInfoService.getWikiInfoByWiki(wiki);
        WikiCover wikiCover = wikiCoverService.getWikiCover(wiki);

        inv.addModel("wikiInfo", wikiInfo);
        inv.addModel("wikiCover", wikiCover);

        //获取用户userActionRecord的信息
        inv.addModel("user", host);
        if (host != null) {
            //交互信息
            UserActionRecord userActionRecord = userActionRecordService.getUserActionRecord(wikiId,
                    host.getId());
            inv.addModel("userActionRecord", userActionRecord);
        }

        wrapFollowRelationData(inv, wikiId, host == null ? 0 : host.getId());
        wrapHighLightData(inv, wikiId);

        Map<String, Object> fullInfoMap = userActionRecordService.getFullInfoMap(wiki, host);
        inv.addModel("wikiInfoFromServ", JSONObject.fromObject(fullInfoMap).toString());

    }

    /**
     * 包装feed相关的数据,在hot latest中调用这个结果
     * 
     * @param inv
     * @param wikiId
     * @param curPage
     * @param pageSize
     * @param wikiFeedType
     * @param itemType
     */
    public void wrapWikiFeedInvocation(Invocation inv, Wiki wiki, User host, int curPage,
            int pageSize, String wikiFeedType, String itemType) {

        inv.addModel("wikiFeedType", wikiFeedType);
        inv.addModel("itemType", ItemType.getItemTypeByValue(itemType).getValue());

        int wikiId = wiki.getId();
        //好友相关的信息
        List<Map<String, Object>> friendsInfo = null;
        if (host != null) {
            friendsInfo = userActionRecordService.getFriendsInfoMap(wiki, host);
        }
        inv.addModel("friendsInfo", friendsInfo != null ? net.sf.json.JSONArray.fromObject(
                friendsInfo).toString() : "\"\"");

        //好友发布的内容
        Map<String, Object> friendsContentMap = null;
        if (host != null) {
            try {
                //取前${friendPageSize}条数据
                int page = 0;
                int friendPageSize = 12;
                friendsContentMap = wikiFeedService.getFeedMap(wikiId, host, null, page,
                        friendPageSize, WikiFeedType.FRIEND);
            } catch (Exception e) {
                logger.error("获取好友新鲜事异常", e);
            }
        }
        inv.addModel("friendsContent", friendsContentMap != null ? JSONObject.fromObject(
                friendsContentMap).toString() : "\"\"");
    }

    private void wrapFollowRelationData(Invocation inv, int wikiId, int userId) {
        inv.addModel("isFollowing", followService.isFollowing(wikiId, userId));
        inv.addModel("followingList", followService.listFollowersShortcuts(wikiId, userId));
        inv.addModel("followCount", followService.getFollowersCount(wikiId));
        inv.addModel("friendsFollowCount", followService.getFollowingFriendsCount(wikiId, userId));
    }

    public void wrapHighLightData(Invocation inv, int wikiId) {
        inv.addModel("photoHighLight", highLightService.getPhotoHighLightSummary(wikiId));
        inv.addModel("videoHighLight", highLightService.getVideoHighLightSummary(wikiId));
    }

}
