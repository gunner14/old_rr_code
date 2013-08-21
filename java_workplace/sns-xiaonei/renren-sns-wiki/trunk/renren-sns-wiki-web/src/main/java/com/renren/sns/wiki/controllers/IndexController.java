package com.renren.sns.wiki.controllers;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import net.sf.json.JSONObject;

import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.wiki.constant.WikiConstant;
import com.renren.sns.wiki.enums.InterestType;
import com.renren.sns.wiki.enums.WikiType;
import com.renren.sns.wiki.model.UserActionRecord;
import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.model.admin.OperateIndexSlide;
import com.renren.sns.wiki.model.info.WikiInfo;
import com.renren.sns.wiki.service.UserActionRecordService;
import com.renren.sns.wiki.service.WikiService;
import com.renren.sns.wiki.service.adminwiki.OperateIndexService;
import com.renren.sns.wiki.service.adminwiki.OperateIndexSlideService;
import com.renren.sns.wiki.service.info.WikiInfoService;
import com.renren.sns.wiki.utils.remotelog.RemoteLogType;
import com.renren.sns.wiki.utils.remotelog.annotation.RemoteLog;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;

@Path("")
public class IndexController {

    @Autowired
    private HostHolder hostHolder;

    @Autowired
    private UserActionRecordService userActionRecordService;

    @Autowired
    private WikiService wikiService;
    
    @Autowired
    private WikiInfoService wikiInfoService;

    @Autowired
    private OperateIndexService operateIndexService;
    
    @Autowired
    private OperateIndexSlideService operateIndexSlideService;

    @Get("")
    @LoginRequired
    @RemoteLog(type = RemoteLogType.HOME)
    public String index(Invocation inv) {
        //电影的数
        int movieCount2012 = WikiConstant.movieIds2012.length;
        int movieCount2011 = WikiConstant.movieIds2011.length;
        int movieCountEarlier = WikiConstant.movieIdsEarlier.length;

        User host = hostHolder.getUser();
        if (host != null) {
            List<Integer> wishWikiIdList = userActionRecordService.getWikiIdList(host.getId(),
                    WikiType.MOVIE, InterestType.WISH);
            List<Integer> collectWikiIdList = userActionRecordService.getWikiIdList(host.getId(),
                    WikiType.MOVIE, InterestType.COLLECT);
            inv.addModel("movieCountWish", wishWikiIdList.size());
            inv.addModel("movieCountCollect", collectWikiIdList.size());
        }

        inv.addModel("movieCount2012", movieCount2012);
        inv.addModel("movieCount2011", movieCount2011);
        inv.addModel("movieCountEarlier", movieCountEarlier);

        return "wiki-home";
    }

    @Get("index")
    @Post("index")
    public String home(Invocation inv) {
        WikiType[] wikiTypes = { WikiType.MOVIE, WikiType.BOOK, WikiType.PLACE, WikiType.GAME };
        InterestType[] InterestTypes = { InterestType.COLLECT, InterestType.WISH };
        User host = hostHolder.getUser();
        if (host != null) {

            Map<String, Integer> wishCountMap = new HashMap<String, Integer>();
            Map<String, Integer> collectCountMap = new HashMap<String, Integer>();

            Map<String, List<Wiki>> wishWikiListMap = new HashMap<String, List<Wiki>>();
            Map<String, List<Wiki>> collectWikiListMap = new HashMap<String, List<Wiki>>();

            for (WikiType wikiType : wikiTypes) {
                int count = userActionRecordService.getCount(host.getId(), wikiType,
                        InterestType.WISH);
                wishCountMap.put(wikiType.getTypeName(), count);

                List<Integer> wikiIdList = userActionRecordService.getWikiIdList(host.getId(),
                        wikiType, InterestType.WISH, 0, 3);
                if (wikiIdList != null && wikiIdList.size() > 0) {
                    List<Wiki> wikiList = new ArrayList<Wiki>();
                    for (int wikiId : wikiIdList) {
                        Wiki wiki = wikiService.getWikiById(wikiId);
                        wikiList.add(wiki);
                    }
                    wishWikiListMap.put(wikiType.getTypeName(), wikiList);
                }
            }

            for (WikiType wikiType : wikiTypes) {
                int count = userActionRecordService.getCount(host.getId(), wikiType,
                        InterestType.COLLECT);
                collectCountMap.put(wikiType.getTypeName(), count);

                List<Integer> wikiIdList = userActionRecordService.getWikiIdList(host.getId(),
                        wikiType, InterestType.COLLECT, 0, 3);
                if (wikiIdList != null && wikiIdList.size() > 0) {
                    List<Wiki> wikiList = new ArrayList<Wiki>();
                    for (int wikiId : wikiIdList) {
                        Wiki wiki = wikiService.getWikiById(wikiId);
                        wikiList.add(wiki);
                    }
                    collectWikiListMap.put(wikiType.getTypeName(), wikiList);
                }
            }

            inv.addModel("wishCountMap", wishCountMap);
            inv.addModel("collectCountMap", collectCountMap);

            inv.addModel("wishWikiListMap", wishWikiListMap);
            inv.addModel("collectWikiListMap", collectWikiListMap);

        }
        inv.addModel("host", host);

        //slide的信息
        Map<String, Object> slideMap = new HashMap<String, Object>();
        for (WikiType wikiType : wikiTypes) {
            
            Map<String, Object> slideOneMap = new HashMap<String, Object>();
            
            List<OperateIndexSlide> operateIndexSlideList = operateIndexSlideService
                    .getList(wikiType);
            //拼装成...
            List<Map<String, Object>> wikiMapList = new ArrayList<Map<String, Object>>();
            int totalWidth = 0;
            for (OperateIndexSlide operateIndexSlide : operateIndexSlideList) {
                int operateWikiId = operateIndexSlide.getWikiId();
                Wiki wiki = wikiService.getWikiById(operateWikiId);
                if(wiki==null){
                    continue;
                }
                WikiInfo wikiInfo = wikiInfoService.getWikiInfoByWiki(wiki);
                UserActionRecord userActionRecord = null;
                if(host!=null){
                    userActionRecord = userActionRecordService.getUserActionRecord(wiki.getId(), host.getId());
                }
                Map<String, Object> infoMap = operateIndexService.getInfoMap(host, wiki, wikiInfo, userActionRecord);
                
                Map<String, Object> imageMap = new HashMap<String, Object>();
                imageMap.put("url", operateIndexSlide.getImageUrl());
                imageMap.put("width", operateIndexSlide.getImageWidth());
                imageMap.put("height", operateIndexSlide.getImageHeight());
                
                totalWidth += operateIndexSlide.getImageWidth();
                
                infoMap.put("image", imageMap);
                
                wikiMapList.add(infoMap);
            }
            slideOneMap.put("totalWidth", totalWidth);
            slideOneMap.put("wikiList", wikiMapList);
            
            slideMap.put(wikiType.getTypeName(), slideOneMap);
        }
        
        inv.addModel("slideInfo", JSONObject.fromObject(slideMap).toString());

        return "wiki-home2";
    }
}
